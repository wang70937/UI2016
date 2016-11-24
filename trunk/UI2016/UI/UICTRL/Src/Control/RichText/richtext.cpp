#include "stdafx.h"
#include "richtext.h"
//#include "parser\htmlparser.h"
#include "richtext_desc.h"
#include "..\UISDK\Inc\Util\util.h"
#include "..\UISDK\Inc\Interface\ilayout.h"

using namespace UI;



RichText::RichText(IRichText* p) :MessageProxy(p)
{
	m_pIRichText = p;
    m_bNeedLayout = false;

	p->SetDescription(RichTextDescription::Get());
}

RichText::~RichText()
{

}

void  RichText::OnInitialize()
{
	m_doc.Init(this);
}

void  RichText::try_layout()
{
    if (!m_bNeedLayout)
        return;
    
    CRect rc;
    m_pIRichText->GetClientRectInObject(&rc);
    int  w = rc.Width();
    int  h = rc.Height();

    if (w < 0)
        w = 0;
    if (h < 0)
        h = 0;
    m_doc.LayoutAll(w, h);

    SetLayoutDirty(false);
}

void  RichText::OnPaint(IRenderTarget* pRT)
{
    if (m_bNeedLayout)
    {
        try_layout();
    }

	HDC hDC = pRT->GetHDC();
	m_doc.Render2DC(hDC);

    // m_caret.OnControlPaint(m_pIRichText, pRT);

    Util::FixRenderTargetClipRegionAlpha(pRT);
}

void  RichText::OnSize(UINT nType, int cx, int cy)
{
    SetMsgHandled(FALSE);
    SetLayoutDirty(true);
}

void  RichText::OnMouseMove(UINT nFlags, POINT point)
{
	POINT ptDoc = {0};
	m_pIRichText->WindowPoint2ObjectClientPoint(&point, &ptDoc, true);

    m_doc.GetMouseKey().OnMouseMove(nFlags, ptDoc);
	return ;
}

void  RichText::OnMouseLeave()
{
    m_doc.GetMouseKey().OnMouseLeave();
	return ;
}

void  RichText::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (!m_doc.GetMouseKey().OnKeyDown(nChar))
        SetMsgHandled(FALSE);
}

BOOL  RichText::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message)
{
    if (!m_doc.GetMouseKey().OnSetCursor())
        SetMsgHandled(FALSE);
    return TRUE;
}

void  RichText::OnLButtonDown(UINT nFlags, POINT point)
{
    POINT ptDoc = { 0 };
    m_pIRichText->WindowPoint2ObjectClientPoint(&point, &ptDoc, true);

    m_doc.GetMouseKey().OnLButtonDown(nFlags, ptDoc);
}

void  RichText::OnLButtonUp(UINT nFlags, POINT point)
{
    POINT ptDoc = { 0 };
    m_pIRichText->WindowPoint2ObjectClientPoint(&point, &ptDoc, true);

    m_doc.GetMouseKey().OnLButtonUp(nFlags, ptDoc);
}

void  RichText::OnSetFocus(IObject* pOldFocusObj)
{
//     if (m_eEditMode == RICHEDIT_EDIT_MODE_DISABLE)
//         return;
// 
//     m_caret.CreateCaret(m_pIRichText, NULL, 1,
//         17/*GetCaretHeight()*/,
//         CARET_TYPE_UNKNOWN,
//         (ICaretUpdateCallback*)COORD_CONTROLCLIENT);
//     //this->UpdateCaretByPos();
//     m_caret.SetCaretPos(m_pIRichText, 0, 0);
//     m_caret.ShowCaret(m_pIRichText);
}

void  RichText::OnKillFocus(IObject* pNewFocusObj)
{
//     if (m_eEditMode == RICHEDIT_EDIT_MODE_DISABLE)
//         return;
// 
//     m_caret.DestroyCaret(m_pIRichText, true);
}

void  RichText::LoadHtml(LPCTSTR szHtml)
{
// 	if (!szHtml)
// 		return;
// 
// 	Clear();
// 
// 	HtmlParser parse(m_doc);
// 	parse.Parse(szHtml);
// 
//     SetLayoutDirty(true);
// 
//     ILayout*  pLayout = (ILayout*)UISendMessage(
//         m_pIRichText->GetParentObject(), UI_MSG_GETLAYOUT);
//     if (pLayout)
//     {
//         pLayout->ChildObjectContentSizeChanged(m_pIRichText);
//     }
}

void  RichText::Clear()
{
    m_doc.Release();
}

void  RichText::SetText(LPCTSTR szText)
{
    Clear();
    AppendText(szText);
}

void  RichText::AppendText(LPCTSTR szText)
{
	m_doc.ReplaceText(szText);
    SetLayoutDirty(true);
    m_pIRichText->Invalidate();
}

void  RichText::AppendText2(LPCTSTR szText, CharFormat* pcf)
{
    m_doc.ReplaceTextEx(szText, pcf);
    SetLayoutDirty(true);
    m_pIRichText->Invalidate();
}

void  RichText::AppendImage(LPCTSTR szPath)
{
    m_doc.ReplaceImage(szPath);
	SetLayoutDirty(true);
    m_pIRichText->Invalidate();
}

RT::ILink*  RichText::AppendLink(LPCTSTR szText)
{
    UIASSERT(szText);
    RT::ILink* link = m_doc.ReplaceLink(szText);

    SetLayoutDirty(true);
    m_pIRichText->Invalidate();

    return link;
}

RT::ILink*  RichText::AppendLink2(LPCTSTR szText, CharFormat* pcf)
{
    UIASSERT(szText);
    RT::ILink* link = m_doc.ReplaceLinkEx(szText, pcf);

	SetLayoutDirty(true);
    m_pIRichText->Invalidate();

    return link;
}

void  RichText::GetDefaultCharFormat(CharFormat* pcf)
{
    if (!pcf)
        return;

    m_doc.CopyDefaultCharFormat(pcf);
}

void  RichText::GetDefaultLinkCharFormat(CharFormat* pcf)
{
	if (!pcf)
		return;

	m_doc.CopyDefaultCharFormat(pcf);
}

void  RichText::SetLayoutDirty(bool b)
{
    m_bNeedLayout = b;
}

PageMode  RichText::GetPageMode()
{
    return m_doc.GetPageMode();
}
void  RichText::SetPageMode(PageMode e)
{
    if (m_doc.GetPageMode() != e)
    {
        m_doc.SetPageMode(e);
        SetLayoutDirty(true);
    }
}

void  RichText::GetDesiredSize(SIZE* pSize)
{
    if (m_bNeedLayout)
    {
        // TODO: ����Ƕ��еģ�Ҫ���Ƶ�ǰ���
        int widthLimit = 0x7FFFFFFF;
        if (m_doc.GetPageMode() == PageMode::multiline)
        {
            CRect rc;
            m_pIRichText->GetClientRectInObject(&rc);
            if (rc.Width() > 0)
            {
                widthLimit = rc.Width();
            }
        }
        

        // ��������û�����ñ�ʶλ����Ϊ��ʱ�ؼ��Ĵ�Сû�ж���û������page��С
        m_doc.LayoutAll(widthLimit, 0x7FFFFFFF);
    }

    *pSize = m_doc.GetDesiredSize();
}

void UI::RichText::OnSerialize(SERIALIZEDATA* pData)
{
    DO_PARENT_PROCESS(IRichText, IControl);
    UI::AttributeSerializerWrap as(pData, TEXT("RichText"));

    as.AddEnum(XML_RICHTEXT_EDITABLE, &m_doc,
        memfun_cast<pfnLongSetter>(&RT::Doc::SetEditMode),
        memfun_cast<pfnLongGetter>(&RT::Doc::GetEditMode))
        ->AddOption((long)RT::EDITABLE::Disable,  XML_RICHTEXT_EDITABLE_DISABLE)
        ->AddOption((long)RT::EDITABLE::Readonly, XML_RICHTEXT_EDITABLE_READONLY)
        ->AddOption((long)RT::EDITABLE::Enable,   XML_RICHTEXT_EDITABLE_ENABLE);

    as.AddEnum(XML_RICHTEXT_PAGE_ALIGN_HORZ, *(long*)&m_doc.GetStyle().page_content_halign)
        ->AddOption((long)RT::HALIGN::left, XML_ALIGN_LEFT)
        ->AddOption((long)RT::HALIGN::right, XML_ALIGN_RIGHT)
        ->AddOption((long)RT::HALIGN::center, XML_ALIGN_CENTER);
    as.AddEnum(XML_RICHTEXT_PAGE_ALIGN_VERT, *(long*)m_doc.GetStyle().page_content_valign)
        ->AddOption((long)RT::VALIGN::top, XML_ALIGN_TOP)
        ->AddOption((long)RT::VALIGN::bottom, XML_ALIGN_BOTTOM)
        ->AddOption((long)RT::VALIGN::center, XML_ALIGN_CENTER);
}

// RichEdit QueryInterface ITextDocument2ʧ�����⣬guid����
extern "C" const GUID __declspec(selectany) IID_ITextDocument2 =
{ 0x01c25500, 0x4268, 0x11d1, { 0x88, 0x3a, 0x3c, 0x8b, 0x00, 0xc1, 0x00, 0x00 } };