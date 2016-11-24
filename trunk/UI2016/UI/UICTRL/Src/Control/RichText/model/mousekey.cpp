#include "stdafx.h"
#include "mousekey.h"
#include "..\model\doc.h"
#include "..\model\page.h"
#include "..\model\line.h"
#include "..\model\run.h"
#include "..\model\element\element.h"
#include "..\UISDK\Inc\Util\struct.h"
#include "..\UISDK\Inc\Interface\iscrollbarmanager.h"
#include "..\richtext.h"

using namespace UI;
using namespace RT;

MouseKey::MouseKey(RT::Doc& pDoc) : m_doc(pDoc)
{
	m_pUIApp = NULL;
    m_sizeContent.cx = m_sizeContent.cy = 0;
    m_sizeView.cx = m_sizeView.cy = 0;
}

MouseKey::~MouseKey()
{
    IScrollBarManager::DestroyIScrollBarManager(m_pMgrScrollBar);
}

void  MouseKey::Init(RichText* pRichText)
{
    UIASSERT(pRichText);
    IRichText* pIRichText = pRichText->GetIRichText();

    m_pUIApp = pIRichText->GetUIApplication();

    m_pMgrScrollBar = IScrollBarManager::CreateIScrollBarManager();
    m_pMgrScrollBar->SetBindObject(pIRichText);
    m_pMgrScrollBar->SetScrollBarVisibleType(HSCROLLBAR, SCROLLBAR_VISIBLE_NONE);
    m_pMgrScrollBar->SetScrollBarVisibleType(VSCROLLBAR, SCROLLBAR_VISIBLE_AUTO);

    OBJSTYLE s = { 0 };
    s.vscroll = 1;
    pIRichText->ModifyObjectStyle(&s, 0);
}

void  MouseKey::Clear()
{
    m_pHoverPage = nullptr;
    m_pHoverLine = nullptr;
    m_pHoverRun = nullptr;
    m_pHoverElement = nullptr;
    m_pPressElement = nullptr;
}

bool  MouseKey::IsPageHover(RT::Page*  pPage)
{
    if (pPage)
        return m_pHoverPage == pPage ? true:false;

    return false;
}
bool  MouseKey::IsLineHover(RT::Line*  pLine)
{
    if (pLine)
        return m_pHoverLine == pLine ? true:false;

    return false;
}
bool  MouseKey::IsRunHover(RT::Run*  pRun)
{
    if (pRun)
        return m_pHoverRun == pRun ? true:false;

    return false;
}
bool  MouseKey::IsElementHover(RT::Element*  pElement)
{
    if (pElement)
        return m_pHoverElement == pElement ? true:false;

    return false;
}

void  MouseKey::OnMouseMove(UINT nFlags, POINT ptDoc)
{
    // ����1���������ݶ��󣬴Ӹ��������һ����������Doc->Para->Unit->Element
    //       Doc/Para��Щ������û���������ݵģ�û����hittest
    // �İ�2�����ݲ��ֶ���Page->Line->Run������run���Ƶõ���ǰelement
    //

	HitTest(ptDoc, &m_pHoverPage, &m_pHoverLine, &m_pHoverRun);

    Element* pOldHover = m_pHoverElement;
    m_pHoverElement = nullptr;
    if (m_pHoverRun)
    {
        m_pHoverElement = m_pHoverRun->GetElement();
    }

    if (pOldHover != m_pHoverElement)
    {
        Node* pLeaveNotifyTo = nullptr;
        Node* pMoveNotifyTo = nullptr;

        // ��Ȿ����Ҫ֪ͨ�����Ƚ�㡣��Ϊold��new���й�ͬ�ĸ���㣬��Щ�����״̬û�з���
        // �ı䡣��������õ������Ƚϵķ�ʽ���Ӹ������������Ƚϣ�ֱ���ҵ���ͬ�����Ƚ�㡣
        if (pOldHover && m_pHoverElement)
        {
            vector<Node*> vecOld;
            vector<Node*> vecHover;

            Node* p = pOldHover;
            while (p)
            {
                vecOld.push_back(p);
                p = p->GetParentNode();
            }
            p = m_pHoverElement;
            while (p)
            {
                vecOld.push_back(p);
                p = p->GetParentNode();
            }

            vector<Node*>::reverse_iterator iterOld = vecOld.rbegin();
            vector<Node*>::reverse_iterator iterHover = vecHover.rbegin();
            for (; iterOld != vecOld.rend() && iterHover != vecHover.rend();
                    ++iterOld, ++iterHover)
            {
                pLeaveNotifyTo = *iterOld;
                pMoveNotifyTo = *iterHover;
                if (*iterOld != *iterHover)
                {
                    break;
                }
            }
        }
        if (pOldHover)
        {
            pOldHover->BubbleMouseLeave(pLeaveNotifyTo);
        }
        if (m_pHoverElement)
        {
            m_pHoverElement->BubbleMouseMove(nFlags, ptDoc, pMoveNotifyTo);
        }
    }

	// TODO: ���������ʽ
// 	if (!m_pPressUnit && pOldHover != m_pHoverUnit)
// 	{
// 		if (!m_pHoverUnit || !m_pHoverUnit->OnSetCursor())
// 		{
// 			static HCURSOR sArrowHandle = LoadCursor(NULL, IDC_ARROW);
// 			SetCursor(sArrowHandle);
// 		}
// 	}

    if ((nFlags & MK_LBUTTON) && m_doc.GetEditMode() != EDITABLE::Disable)
    {
        // ��קѡ��
        Run* pHitRun = get_nearest_run(ptDoc);
        if (pHitRun)
        {
            CaretPos nOffsetInElement = 
                pHitRun->SelectionHitTestByDocPoint(ptDoc);
            m_doc.GetSelection().SetFocus(
                pHitRun->GetElement(), nOffsetInElement);
        }
    }
    m_doc.Refresh();
}

void  MouseKey::OnMouseLeave()
{
    if (m_pHoverElement)
    {
        m_pHoverElement->BubbleMouseLeave(nullptr);
    }

	m_pHoverPage = NULL;
	m_pHoverLine = NULL;
	m_pHoverRun = NULL;
	m_pHoverElement = NULL;
	m_doc.Refresh();
}

BOOL  MouseKey::OnKeyDown(UINT nChar)
{
    if (nChar == TEXT('C') && IsKeyDown(VK_CONTROL))
    {
        m_doc.CopySelection();
    }
    else if (nChar == TEXT('A') && IsKeyDown(VK_CONTROL))
    {
        if (!IsKeyDown(VK_LBUTTON))
            m_doc.SelectAll();
    }
    return FALSE;
}

BOOL  MouseKey::OnSetCursor()
{
    BOOL bHandled = FALSE;
//     if (m_pHoverRun)
//     {
//         Element* pElement = m_pHoverRun->GetElement();
//         if (pElement)
//         {
//             return pElement->OnSetCursor();
//         }
//     }

    if (m_pHoverElement)
    {
        bHandled = m_pHoverElement->OnSetCursor();
    }

    if (!bHandled && m_doc.GetEditMode() != EDITABLE::Disable)
    {
        SetCursor(LoadCursor(NULL, IDC_IBEAM));
        return TRUE;
    }
    return bHandled;
}

void  MouseKey::doc_point_2_page_point(const POINT& ptDoc, POINT& ptPage)
{
#if 0
    m_doc.PageHitTest(ptDoc, &ptPage);
#endif
}

// ��ȡ�뵱ǰ��������һ�����ֶΣ�����ѡ������
RT::Run*  MouseKey::get_nearest_run(const POINT& ptDoc)
{
    if (m_pHoverRun)
    {
        return m_pHoverRun;
    }

    Line* pHoverLine = m_pHoverLine;
    if (!pHoverLine)
    {
        Page* pHoverPage = m_pHoverPage;
        // �����page�����ˣ�Ҫ������л�ĩ��
        if (!pHoverPage)
        {
            if (m_doc.GetPageMode() == PageMode::multipage)
                return nullptr;

            pHoverPage = m_doc.GetFirstPage();
            if (!pHoverPage)
                return nullptr;
        }

        pHoverLine = pHoverPage->NearestLineHitTest(ptDoc);
    }

    // ������������
    if (!pHoverLine)
        return nullptr;
    
    RECT rcLine;
    pHoverLine->GetContentRectInDoc(&rcLine);

    // TBD: ����ǿհ׻س��У���һ����û��run�ģ�ֻ�ܷ���һ�е�run
    while (pHoverLine && !pHoverLine->GetFirstRun())
    {
        pHoverLine = pHoverLine->GetPrevLine();
    }
    if (!pHoverLine)
        return nullptr;

    if (ptDoc.x <= rcLine.left)
    {
        return pHoverLine->GetFirstRun();
    }
    else if (ptDoc.x >= rcLine.right)
    {
        return pHoverLine->GetLastRun();
    }

    POINT ptInLine = { ptDoc.x - rcLine.left, ptDoc.y - rcLine.top };
    // �м���
    return pHoverLine->RunHitTest(ptInLine, nullptr);
}

void  MouseKey::OnLButtonDown(UINT nFlags, POINT ptDoc)
{
//     if (m_pHoverUnit)
//     {
//         SetPressUnit(m_pHoverUnit);
//     }
    
    if (m_pHoverElement)
    {
        m_pPressElement = m_pHoverElement;
        m_pPressElement->OnLButtonDown(nFlags, ptDoc);
    }

    // selection
    if (m_doc.GetEditMode() != EDITABLE::Disable)
    {
        Run* pHitRun = get_nearest_run(ptDoc);
        if (pHitRun)
        {
            CaretPos nOffsetInElement =
                pHitRun->SelectionHitTestByDocPoint(ptDoc);
            m_doc.GetSelection().SetAnchor(
                pHitRun->GetElement(), nOffsetInElement);
        }
	}


	m_doc.Refresh();

}
void  MouseKey::OnLButtonUp(UINT nFlags, POINT ptDoc)
{
    if (m_pPressElement)
    {
        Element* pOldPress = m_pPressElement;

        pOldPress->OnLButtonUp(nFlags, ptDoc);
        if (pOldPress == m_pPressElement && 
            pOldPress == m_pHoverElement)
        {
            pOldPress->OnClick(nFlags, ptDoc);
        }

        m_pPressElement = nullptr;
    }

    if (m_doc.GetEditMode() != EDITABLE::Disable)
    {
        Run* pHitRun = get_nearest_run(ptDoc);
        if (pHitRun)
        {
            CaretPos nOffsetInElement = 
                pHitRun->SelectionHitTestByDocPoint(ptDoc);
            m_doc.GetSelection().SetFocus(
                pHitRun->GetElement(), nOffsetInElement);
        }
    }

// 	if (m_pPressUnit && m_pHoverUnit == m_pPressUnit)
// 		m_pPressUnit->OnClick();

	//SetPressUnit(NULL);

    m_doc.Refresh();
}

void  MouseKey::HitTest(POINT ptDoc, Page** ppHoverPage, Line** ppHoverLine, Run** ppHoverRun)
{
	if (ppHoverPage)
		*ppHoverPage = NULL;
	if (ppHoverLine)
		*ppHoverLine = NULL;
	if (ppHoverRun)
		*ppHoverRun = NULL;

	// Page����
	POINT ptInPage = {0};
	Page* pHoverPage = m_doc.PageHitTest(ptDoc, &ptInPage);
	if (!pHoverPage)
		return;

	if (ppHoverPage)
		*ppHoverPage = pHoverPage;

	// Line����
	POINT ptInLine = {0};
	Line* pHoverLine = pHoverPage->LineHitTest(ptInPage, &ptInLine);
	if (!pHoverLine)
		return;

	if (ppHoverLine)
		*ppHoverLine = pHoverLine;

	// Run����
	POINT ptInRun = {0};
	Run* pHoverRun = pHoverLine->RunHitTest(ptInLine, &ptInRun);
	if (!pHoverRun)
		return;

	if (ppHoverRun)
		*ppHoverRun = pHoverRun;
}

// BOOL  MouseKey::OnMouseWheel(UINT nFlags, short zDelta)
// {
//     UIMSG msg;
//     msg.message = WM_MOUSEWHEEL;
//     msg.wParam = nFlags;
//     msg.lParam = zDelta;
//     return m_pMgrScrollBar->ProcessMessage(&msg);
// }

void  MouseKey::SetContentSize(int cx, int cy)
{
    m_sizeContent.cx = cx;
    m_sizeContent.cy = cy;
    m_pMgrScrollBar->SetVScrollRange(cy);
}

void  MouseKey::SetViewSize(int cx, int cy)
{
    m_sizeView.cx = cx;
    m_sizeView.cy = cy;

    m_pMgrScrollBar->SetVScrollPage(cy);
}

BOOL  MouseKey::ProcessMessage(UIMSG* pMsg, int mapid, bool hook)
{
    if (!m_pMgrScrollBar)
        return FALSE;
    return m_pMgrScrollBar->ProcessMessage(pMsg, mapid, hook);
}