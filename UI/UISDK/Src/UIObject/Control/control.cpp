#include "stdafx.h"
#include "control.h"
#include "Src\Base\Attribute\attribute.h"
#include "Src\Base\Attribute\bool_attribute.h"
#include "Src\Base\Application\uiapplication.h"
#include "Src\Base\Attribute\stringselect_attribute.h"
#include "Inc\Base\uidefine.h"
#include "Inc\Interface\itextrenderbase.h"
#include "Inc\Interface\iuiapplication.h"
#include "Src\Resource\skinres.h"

using namespace UI;

Control::Control(IControl* p) : Object(p)
{
    m_pIControl = p;
	memset(&m_controlStyle, 0, sizeof(m_controlStyle));
	m_objStyle.tabstop = 1;
    m_objStyle.default_tabstop = 1;
}

void  Control::OnSerialize(SERIALIZEDATA* pData)
{
    __super::OnSerialize(pData);

	{
		AttributeSerializer s(pData, TEXT("Control"));

		s.AddBool(XML_CONTROL_STYLE_TABABLE, this, 
			memfun_cast<pfnBoolSetter>(&Object::SetTabstop),
			memfun_cast<pfnBoolGetter>(&Object::IsTabstop))
			->SetDefault(m_objStyle.default_tabstop);

		s.AddBool(XML_CONTROL_STYLE_GROUP, this, 
			memfun_cast<pfnBoolSetter>(&Control::SetGroup),
			memfun_cast<pfnBoolGetter>(&Control::IsGroup));

		s.AddTextRenderBase(NULL, this, m_pTextRender);
// 		s.AddStringEnum(XML_TEXTRENDER_TYPE, this,
// 			memfun_cast<pfnStringSetter>(&Object::LoadTextRender),
// 			memfun_cast<pfnStringGetter>(&Object::SaveTextRender))
// 			->FillTextRenderBaseTypeData()
// 			->ReloadOnChanged();
// 
// 		// ����
// 		if (m_pTextRender)
// 		{
// 			SERIALIZEDATA data(*pData);
// 			data.szParentKey = XML_TEXTRENDER_TYPE;
// 			if (data.pUIApplication->IsDesignMode())
// 				data.SetErase(false);
// 			m_pTextRender->Serialize(&data);
// 		}

	}


}

void  Control::ModifyControlStyle(ControlStyle* add, ControlStyle* remove)
{
	if (add)
	{
		if (add->group)
			m_controlStyle.group = 1;
		if (add->ownerdraw)
			m_controlStyle.ownerdraw = 1;
	}

	if (remove)
	{
		if (remove->group)
			m_controlStyle.group = 0;
		if (remove->ownerdraw)
			m_controlStyle.ownerdraw = 0;
	}
}
bool  Control::TestControlStyle(ControlStyle* test)
{
	if (!test)
		return false;

	if (test->group && m_controlStyle.group)
		return true;
	if (test->ownerdraw && m_controlStyle.ownerdraw)
		return true;

	return false;
}

bool Control::IsGroup()
{
	return  m_controlStyle.group;
}
void Control::SetGroup(bool b)
{
	m_controlStyle.group = b;
}

// ��ȡ���壬���û�У���ʹ��Ĭ�ϵ�
ITextRenderBase*  Control::GetTextRenderDefault()
{
    if (!m_pTextRender)
        CreateDefaultTextRender();

    return m_pTextRender;
}

// ���û����Ƥ�����������壬���ⲿ�ɵ��øú�����Paintʱ����һ��Ĭ�ϵ�����
ITextRenderBase*  Control::CreateDefaultTextRender()
{
    if (!m_pTextRender)
    {
        GetUIApplication()->GetTextRenderFactroy().CreateTextRender(
            m_pSkinRes->GetISkinRes(), 
            TEXTRENDER_TYPE_SIMPLE,
            m_pIObject, 
            &m_pTextRender);

        if (m_pTextRender && m_pIMapAttributeRemain)
        {
            SERIALIZEDATA data = {0};
            data.pUIApplication = GetUIApplication()->GetIUIApplication();
			data.pSkinRes = m_pSkinRes->GetISkinRes();
            data.pMapAttrib = m_pIMapAttributeRemain;
            data.szPrefix = NULL;
            data.nFlags = SERIALIZEFLAG_LOAD|SERIALIZEFLAG_LOAD_ERASEATTR;
            m_pTextRender->Serialize(&data);
        }
    }

    return m_pTextRender;
}



LPCTSTR  Control::GetToolTipText()
{
	return GetAttribute(XML_TOOLTIP, false);
}
void  Control::SetToolTipText(LPCTSTR szText)
{
	if (!szText || 0 == szText[0])
	{
		GetAttribute(XML_TOOLTIP, true);
	}
	else
	{
		AddAttribute(XML_TOOLTIP, szText);
	}
}

BOOL  Control::OnGetToolTipInfo(TOOLTIPITEM* pToolTipItem, IToolTipUI* pToolTip)
{
	if (NULL == pToolTipItem || NULL == pToolTip)
		return 0;

	LPCTSTR szTooltip = GetToolTipText();
	if (!szTooltip || !szTooltip[0])
		return 0;

	pToolTip->SetText(szTooltip);
	return 1;
}
