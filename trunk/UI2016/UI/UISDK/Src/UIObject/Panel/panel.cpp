#include "stdafx.h"
#include "panel.h"
#include "Inc\Interface\ilayout.h"
#include "Inc\Interface\iwindow.h"
#include "Src\Helper\layout\layout.h"
#include "Inc\Interface\imapattr.h"
#include "Src\UIObject\Window\windowbase.h"
#include "Src\Base\Attribute\attribute.h"
#include "Src\Base\Attribute\enum_attribute.h"
#include "Src\Base\Application\uiapplication.h"
#include "Src\Base\Attribute\stringselect_attribute.h"
#include "Inc\Interface\irenderbase.h"

namespace UI
{

Panel::Panel(IPanel* p) : Object(p)
{
    m_pIPanel = p;
	m_pLayout = NULL;

    m_pTextureRender = NULL;
    m_rcTextureRenderRegion.SetRectEmpty();

    m_pMaskRender = NULL;
    m_rcMaskRenderRegion.SetRectEmpty();

	SetRectEmpty(&m_rcBkgndRenderRegion);
	SetRectEmpty(&m_rcForegndRenderRegion);

	OBJSTYLE s = {0};
	s.default_transparent = 1;
	s.default_reject_self_mouse_msg = 1;
	ModifyObjectStyle(&s, 0);

    this->SetDescription(PanelDescription::Get());
}

Panel::~Panel()
{
	SAFE_RELEASE(m_pLayout);
    SAFE_RELEASE(m_pTextureRender);
    SAFE_RELEASE(m_pMaskRender);
}

ILayout* Panel::GetLayout()
{
	return this->m_pLayout; 
}


void  Panel::SetLayoutType(LAYOUTTYPE eLayoutType)
{
    SAFE_RELEASE(m_pLayout);

	GetUIApplication()->GetLayoutFactory().Create(
			eLayoutType, m_pIPanel, &m_pLayout);
}

LAYOUTTYPE  Panel::GetLayoutType()
{
    if (!m_pLayout)
        return LAYOUT_TYPE_CANVAS;
    
    return m_pLayout->GetLayoutType();
}

LRESULT Panel::OnGetLayoutPtr(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)m_pLayout;
}

void  Panel::virtualOnSize(UINT nType, UINT cx, UINT cy)
{
	Object::virtualOnSize(nType, cx, cy);
    if (m_pLayout)
        m_pLayout->Arrange(NULL);
}

void  Panel::OnGetDesiredSize(SIZE* pSize)
{
    pSize->cx = pSize->cy = 0;

    if (this->m_pLayout)
        *pSize= this->m_pLayout->Measure();

    REGION4 rcNonClient = {0};
    GetNonClientRegion(&rcNonClient);

    pSize->cx += rcNonClient.left + rcNonClient.right;
    pSize->cy += rcNonClient.top + rcNonClient.bottom;
}

void  Panel::OnSerialize(SERIALIZEDATA* pData)
{
	__super::OnSerialize(pData);

	if (pData->IsReload())
	{
		SAFE_RELEASE(m_pLayout);
		m_objStyle.post_paint = 0;
	}

    {
        AttributeSerializer s(pData, TEXT("Panel"));

        // �����
        //s.AddRenderBase(XML_TEXTURE_RENDER_PREFIX, XML_RENDER_TYPE, m_pIPanel, m_pTextureRender);
        s.AddStringEnum(XML_TEXTURE_RENDER_PREFIX XML_RENDER_TYPE, 
            this,
            memfun_cast<pfnStringSetter>(&Panel::LoadTextureRender),
            memfun_cast<pfnStringGetter>(&Panel::
                GetTextureRenderName))
                ->FillRenderBaseTypeData();
        s.AddRect(XML_TEXTURE_RENDER_PREFIX XML_PANEL_RENDER_REGION, 
            m_rcTextureRenderRegion);

        // �������ֲ�
        //s.AddRenderBase(XML_MASK_RENDER_PREFIX, XML_RENDER_TYPE, m_pIPanel, m_pMaskRender);
        s.AddStringEnum(XML_MASK_RENDER_PREFIX XML_RENDER_TYPE, this,
            memfun_cast<pfnStringSetter>(&Panel::LoadMaskRender),
            memfun_cast<pfnStringGetter>(&Panel::GetMaskRenderName))
            ->FillRenderBaseTypeData();
        s.AddRect(XML_MASK_RENDER_PREFIX XML_PANEL_RENDER_REGION, 
            m_rcMaskRenderRegion);

        s.AddRect(XML_BACKGND_RENDER_PREFIX XML_PANEL_RENDER_REGION, 
            m_rcBkgndRenderRegion);
        s.AddRect(XML_FOREGND_RENDER_PREFIX XML_PANEL_RENDER_REGION, 
            m_rcForegndRenderRegion);

		s.AddEnum(XML_LAYOUT_TYPE, this,
            memfun_cast<pfnLongSetter>(&Panel::SetLayoutType),
            memfun_cast<pfnLongGetter>(&Panel::GetLayoutType))
            ->FillLayoutTypeData();
    }
    if (pData->IsLoad()) 
    {
        if (m_pMaskRender)
        {
            m_objStyle.post_paint = 1;
        }
        if (!m_pLayout)
        {
            SetLayoutType(LAYOUT_TYPE_CANVAS);
        }
    }

    if (m_pLayout)
    {
        SERIALIZEDATA  data(*pData);
        data.szParentKey = XML_LAYOUT;
        m_pLayout->Serialize(&data);
    }

    if (m_pMaskRender)
    {
        SERIALIZEDATA data(*pData);
        data.szPrefix = XML_MASK_RENDER_PREFIX;
        m_pMaskRender->Serialize(&data);
    }

    if (m_pTextureRender)
    {
        SERIALIZEDATA data(*pData);
        data.szPrefix = XML_TEXTURE_RENDER_PREFIX;
        m_pTextureRender->Serialize(&data);
    }
}

void Panel::OnEraseBkgnd(IRenderTarget* pRenderTarget)
{
    if (m_pLayout && m_pLayout->IsDirty())
    {
        m_pLayout->Arrange(NULL);
    }

	CRect rc(0,0, this->GetWidth(), this->GetHeight());
	
	if (m_pBkgndRender)
	{
		CRect rcBkgnd(&rc);
		rcBkgnd.DeflateRect(&m_rcBkgndRenderRegion);
		m_pBkgndRender->DrawState(pRenderTarget, &rcBkgnd, 0);
	}
	
    if (m_pTextureRender)
    {
        CRect rcTextureRegion(&rc);
        rcTextureRegion.DeflateRect(&m_rcTextureRenderRegion);
        m_pTextureRender->DrawState(pRenderTarget, &rcTextureRegion, 0);
    }
}

void  Panel::OnPaint(IRenderTarget* pRenderTarget)
{
    if (m_pForegndRender)
    {
        CRect rcForegnd(0,0, this->GetWidth(), this->GetHeight());
        rcForegnd.DeflateRect(&m_rcForegndRenderRegion);
        m_pForegndRender->DrawState(pRenderTarget, &rcForegnd, 0);
    }
}
void  Panel::OnPostPaint(IRenderTarget* pRenderTarget)
{
    if (m_pMaskRender)
    {
        CRect rcMask(0,0, this->GetWidth(), this->GetHeight());
        rcMask.DeflateRect(&m_rcMaskRenderRegion);
        m_pMaskRender->DrawState(pRenderTarget, &rcMask, 0);
    }
}

void Panel::SetTextureRender(IRenderBase* p)
{
    SAFE_RELEASE(m_pTextureRender);
    m_pTextureRender = p;

//     if (m_pTextureRender)
//         m_pTextureRender->AddRef();
}
IRenderBase*  Panel::GetTextureRender()
{
    return m_pTextureRender;
}

}