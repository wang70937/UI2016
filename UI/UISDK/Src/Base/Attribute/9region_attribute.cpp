#include "stdafx.h"
#include "9region_attribute.h"
#include "attribute.h"

using namespace UI;

namespace UI
{
    AttributeBase*  CreateRegion9Attribute()
    {
        return new Region9Attribute();
    }
}

Region9Attribute::Region9Attribute() 
{
    m_pIRegion9Attribute = NULL;
    m_pBindValue = NULL;

    _this = NULL;
    _setter = NULL;
    _getter = NULL;
}

Region9Attribute::~Region9Attribute()
{
    SAFE_DELETE(m_pIRegion9Attribute);
}

void  Region9Attribute::SetBindValue(void* p)
{
    m_pBindValue = (C9Region*)p;
}

void  Region9Attribute::SetBindFuction(void* _this, void* _setter, void* _getter)
{
    this->_this = _this;
    this->_setter = (pfn9RegionSetter)_setter;
    this->_getter = (pfn9RegionGetter)_getter;
}

Region9Attribute*  Region9Attribute::SetDefault(C9Region* pr)
{
	if (pr)
		m_rDefault.Copy(*pr);
	else
		m_rDefault.Set(0);

	return this;
}

void  Region9Attribute::get(C9Region* pr)
{
    if (m_pBindValue)
    {
		pr->Copy(*m_pBindValue);
    }
    else if (_getter && _this)
    {
        long localVarThis = (long)_this;
        long localVarGetter = (long)_getter;

        __asm
        {
            mov eax, pr
            push eax;

			mov ecx, localVarThis;  
			call localVarGetter;    
        }
    }
    else
    {
		pr->Copy(m_rDefault);
    }
}

void  Region9Attribute::set(C9Region* pr)
{
    if (m_pBindValue)
    {
		m_pBindValue->Copy(*pr);
    }
    else if (_setter && _this)
    {
        long localVarThis = (long)_this;
        long localVarSetter = (long)_setter;

        __asm
        {
            mov eax, pr   // ѹ�����
            push eax;

            mov ecx, localVarThis;   // ����this
            call localVarSetter;     // ����
        }
    }
}

LPCTSTR  Region9Attribute::Get()
{
	C9Region r;
    get(&r);

	TCHAR* szText = GetTempBuffer(64);
	if (r.IsSimpleValue())
	{
		_stprintf(szText, TEXT("%d"), r.left);
	}
	else if (r.IsSimpleRect())
	{
		_stprintf(szText, TEXT("%d,%d,%d,%d"), r.left, r.top, r.right, r.bottom);
	}
	else
	{
		_stprintf(szText, TEXT("%d,%d,%d,%d,%d,%d,%d,%d"),
				r.topleft, r.top, r.topright, r.left, r.right, r.bottomleft, r.bottom, r.bottomright);
	}
    return szText;
}

void  Region9Attribute::Set(LPCTSTR szValue)
{
	C9Region r;
	if (Util::TranslateImage9Region(szValue, &r, XML_SEPARATOR))
	{
		set(&r);
	}
}

void  Region9Attribute::Reset()
{
	set(&m_rDefault);
}

bool  Region9Attribute::IsDefaultValue()
{
	C9Region r;
	get(&r);

	return r.IsEqual(m_rDefault);
}

void  Region9Attribute::Editor(AttributeEditorProxy* p, EditorAttributeFlag e)
{
    p->Region92Editor(this, e);
}

IRegion9Attribute*  Region9Attribute::GetIRegion9Attribute()
{
    if (!m_pIRegion9Attribute)
        m_pIRegion9Attribute = new IRegion9Attribute(this);
    return m_pIRegion9Attribute;
}