#include "stdafx.h"
#include "bool_attribute.h"
#include "attribute.h"

using namespace UI;

namespace UI
{
    AttributeBase*  CreateBoolAttribute()
    {
        return new BoolAttribute();
    }
}

BoolAttribute::BoolAttribute() 
{
    m_pIBoolAttribute = NULL;
    m_pBindValue = NULL;
    m_bDefault = false;

    _this = NULL;
    _setter = NULL;
    _getter = NULL;
}

BoolAttribute::~BoolAttribute()
{
    SAFE_DELETE(m_pIBoolAttribute);
}
IBoolAttribute*  BoolAttribute::GetIBoolAttribute()
{
    if (!m_pIBoolAttribute)
        m_pIBoolAttribute = new IBoolAttribute(this);

    return m_pIBoolAttribute;
}

void  BoolAttribute::SetBindValue(void* p)
{
    m_pBindValue = (bool*)p;
}

void  BoolAttribute::SetBindFuction(void* _this, void* _setter, void* _getter)
{
    this->_this = _this;
    this->_setter = (pfnBoolSetter)_setter;
    this->_getter = (pfnBoolGetter)_getter;
}

BoolAttribute*  BoolAttribute::SetDefault(bool b)
{
	m_bDefault = b;
	return this;
}

//
// ע������ֱ��д��b = _getter();
//     �����Ľ���ǽ�BoolAttribute��this��mov��ecx��
//
bool  BoolAttribute::GetBool()
{
    if (m_pBindValue)
    {
        return (*m_pBindValue);
    }
    else if (_getter && _this)
    {
        bool bReturn = m_bDefault;
        long localVarThis = (long)_this;
        long localVarGetter = (long)_getter;

        __asm
        {
            mov ecx, localVarThis;  // ����this
            call localVarGetter;    // ����
            mov bReturn, al;        // ��ȡ����ֵ
        }

        return bReturn;
    }
    else
    {
        return m_bDefault;
    }
}
void  BoolAttribute::SetBool(bool b)
{
    if (m_pBindValue)
    {
        (*m_pBindValue) = b;
    }
    else if (_setter && _this)
    {
        long localVarThis = (long)_this;
        long localVarSetter = (long)_setter;

        __asm
        {
            movzx eax, byte ptr [b]   // ѹ�����
            push eax;

            mov ecx, localVarThis;   // ����this
            call localVarSetter;     // ����
        }
    }
}
LPCTSTR  BoolAttribute::Get()
{
    bool b = GetBool();

	TCHAR* szText = GetTempBuffer(64);
    _tcscpy(szText, b ? XML_BOOL_VALUE_1 : XML_BOOL_VALUE_0);
    
    return szText;
}

void  BoolAttribute::Set(LPCTSTR szValue)
{
    bool b = false;
    if (0 == _tcscmp(szValue, XML_BOOL_VALUE_TRUE)  || 
        0 == _tcscmp(szValue, XML_BOOL_VALUE_1)     ||
        0 == _tcscmp(szValue, XML_BOOL_VALUE_YES))
    {
        b = true; 
    }
//  else if (0 == _tcscmp(szValue, XML_BOOL_VALUE_FALSE) ||
//      0 == _tcscmp(szValue, XML_BOOL_VALUE_0)      ||
//      0 == _tcscmp(szValue, XML_BOOL_VALUE_NO))
//  {
//      b = false;
//  }

    SetBool(b);
}

void  BoolAttribute::Reset()
{
	SetBool(m_bDefault);
}

bool  BoolAttribute::IsDefaultValue()
{
    return GetBool() == m_bDefault;
}

void  BoolAttribute::Editor(AttributeEditorProxy* p, EditorAttributeFlag e)
{
    p->Bool2Editor(this, e);
}
