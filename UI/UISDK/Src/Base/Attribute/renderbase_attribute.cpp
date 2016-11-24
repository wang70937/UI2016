#include "stdafx.h"
#include "renderbase_attribute.h"
#include "attribute.h"
#include "..\Application\uiapplication.h"
#include "..\Object\object.h"

namespace UI
{
    AttributeBase*  CreateRenderBaseAttribute()
    {
        return new RenderBaseAttribute();
    }
}

RenderBaseAttribute::RenderBaseAttribute() 
{
    ReloadOnChanged();

    m_pIRenderBaseAttribute = NULL;
    m_ppBindValue = NULL;
    m_pObject = NULL;
//     _this = NULL;
//     _setter = NULL;
//     _getter = NULL;
}

RenderBaseAttribute::~RenderBaseAttribute()
{
	SAFE_DELETE(m_pIRenderBaseAttribute);
}

void  RenderBaseAttribute::SetBindValue(void* p)
{
    m_ppBindValue = (IRenderBase**)p;
}

void  RenderBaseAttribute::Set(LPCTSTR szType)
{
    if (!m_ppBindValue)
        return;

	if (!m_pUIApplication || !m_pObject)
    {
        UIASSERT(0);
        return;
    }

    SAFE_RELEASE(*m_ppBindValue);
    m_pUIApplication->CreateRenderBaseByName(
		szType, m_pObject->GetIObject(), m_ppBindValue);
}

void  RenderBaseAttribute::Reset()
{
	if (m_ppBindValue)
	{
		SAFE_RELEASE(*m_ppBindValue);
	}
	else
	{

	}
}

LPCTSTR  RenderBaseAttribute::Get()
{
    if (!m_ppBindValue)
        return NULL;
	if (!*m_ppBindValue)
		return NULL;

	RENDER_TYPE eType = (*m_ppBindValue)->GetType();
	RenderBaseFactory& factory = m_pUIApplication->GetRenderBaseFactory();
	LPCTSTR szType = factory.GetRenderBaseName(eType);
	return szType;
}

void  RenderBaseAttribute::do_child_action(SERIALIZEDATA* pData)
{
	// ���������л�
	if (m_ppBindValue && *m_ppBindValue)
	{
		LPCTSTR szOldPrefix = pData->szPrefix;
		LPCTSTR szOldParentKey = pData->szParentKey;

		pData->szPrefix = m_strPrefix.c_str();
		pData->szParentKey = GetKey();

		(*m_ppBindValue)->Serialize(pData);

		pData->szPrefix = szOldPrefix;
		pData->szParentKey = szOldParentKey;
	}
}

void  RenderBaseAttribute::Load(SERIALIZEDATA* pData)
{
	// �����Լ�������
	__super::Load(pData);

	do_child_action(pData);
}

void  RenderBaseAttribute::Save(SERIALIZEDATA* pData)
{
	// �����Լ�������
	__super::Save(pData);

	do_child_action(pData);
}

bool  RenderBaseAttribute::IsDefaultValue()
{
	if (m_ppBindValue)
	{
		if (*m_ppBindValue == NULL)
			return true;
	}

	return false;
}

void  RenderBaseAttribute::Editor(
		SERIALIZEDATA* pData,
		AttributeEditorProxy* p,
		EditorAttributeFlag e)
{
    p->RenderBase2Editor(this, e);

	if (pData && e == EDITOR_ATTRIBUTE_ADD)
	{
		// ���������л�
		do_child_action(pData);
	}
}

IRenderBaseAttribute*  RenderBaseAttribute::GetIRenderBaseAttribute()
{
    if (!m_pIRenderBaseAttribute)
        m_pIRenderBaseAttribute = new IRenderBaseAttribute(this);

    return m_pIRenderBaseAttribute;
}
