#include "stdafx.h"
#include "multisellistctrlmkmgr.h"
#include "Src\UIObject\ListCtrl\listctrlbase.h"
#include "Src\UIObject\ListCtrl\ListItemBase\listitembase.h"
#include "Src\Base\Application\uiapplication.h"
#include "Src\UIObject\Window\windowbase.h"

using namespace UI;
using namespace UI::Util;

MultiSelListCtrlMKMgr::MultiSelListCtrlMKMgr()
{
    m_pSelectRegionRender = NULL;
}
MultiSelListCtrlMKMgr::~MultiSelListCtrlMKMgr()
{
    SAFE_DELETE(m_pSelectRegionRender);
}

BOOL  MultiSelListCtrlMKMgr::DoProcessMessage(UIMSG* pMsg)
{
    bool bInterestMsg = true;
    switch (pMsg->message)
    {
    case WM_LBUTTONDOWN:
        OnLButtonDown(pMsg);
        break;

    case WM_MOUSEMOVE:
        OnMouseMove(pMsg);
        break;

    case WM_RBUTTONDOWN:
        {
            OnRButtonDown(pMsg);
            return TRUE;
        }
        break;

    case WM_KEYDOWN:
        {
            BOOL bHandled = __super::DoProcessMessage(pMsg);
            if (bHandled)
                return TRUE;

            OnKeyDown(pMsg, &bInterestMsg);
        }
        break;

    case WM_MOUSELEAVE:
        OnMouseLeave(pMsg);
        break;;

    case WM_LBUTTONUP:
        OnLButtonUp(pMsg);
        break;

    default:
        return __super::DoProcessMessage(pMsg);
        break;
    }

    return FALSE;
}

void  MultiSelListCtrlMKMgr::OnMouseMove(UIMSG* pMsg)
{
    if (pMsg->wParam & MK_LBUTTON)
    {
        if (NULL == m_pItemPress && NULL == m_pObjPress)
        {
            // ����ѡ��
            OnMouseMove_DragSelectRegion(pMsg);
            return;
        }
    }

    __super::OnMouseMove(pMsg);
}

// ������ѡ��
void  MultiSelListCtrlMKMgr::OnLButtonDown(UIMSG* pMsg)
{
    __super::OnLButtonDown(pMsg);

    bool bCtrlDown = IsKeyDown(VK_CONTROL);
    bool bShiftDown = IsKeyDown(VK_SHIFT);

    ListItemBase*  pItemPress = m_pItemPress;
    if (pItemPress)
    {
        if (pItemPress->IsSelectable())
        {
            // �����ǰû��ѡ���Լ�����ֱ��ȡ������ѡ�����Ϊ�Լ���
            // ����Լ���ѡ���ˣ�������Ҫ������ק�������򲻲����������û�з�����ק������UP�н�ѡ������Ϊ�Լ�
            if (!bCtrlDown && !bShiftDown && !pItemPress->IsSelected())
            {
                m_pListCtrlBase->SelectItem(pItemPress);
            }
        }
        else if (pItemPress->IsFocusable())
        {
            m_pListCtrlBase->SetFocusItem(pItemPress);
        }
    }
}

void  MultiSelListCtrlMKMgr::OnLButtonUp(UIMSG* pMsg)
{
    if (m_pSelectRegionRender)
    {
        m_pSelectRegionRender->LButtonUp();
        SAFE_DELETE(m_pSelectRegionRender);
    }

    ListItemBase*  pItemPress = m_pItemPress;
    __super::OnLButtonUp(pMsg);

//     long lRet = __super::OnLButtonUp(pMsg);
//     if (lRet & IMKER_DONT_CHANGE_SELECTION)
//         return;

    bool bCtrlDown = IsKeyDown(VK_CONTROL);
    bool bShiftDown = IsKeyDown(VK_SHIFT);

    if (NULL == pItemPress)
    {
        if (!bCtrlDown && !bShiftDown)
        {
            m_pListCtrlBase->ClearSelectItem(true);
        }
        return;
    }

    if (pItemPress->IsSelectable())
    {
        if (bShiftDown)
        {
            // ����գ����û�а���ctrl�Ļ�
            ListItemBase*  pFirstSelect = m_pListCtrlBase->GetFirstSelectItem();
            if (pFirstSelect)
            {
                if (!bCtrlDown)
                {
                    m_pListCtrlBase->ClearSelectItem(false);
                }
            }
            else
            {
                if (!m_pItemFocus)
                    return;

                pFirstSelect = m_pItemFocus;
            }

            // ��Ӵ�pFirstSelect -> pressitem֮����б���
            if (!pItemPress)
                return;

            if (pItemPress == pFirstSelect)
            {
                m_pListCtrlBase->SelectItem(pFirstSelect);
                return;
            }

            bool bLoopFromTopToBottom = true;  // ��������
            if (pItemPress->GetParentRectPtr()->top < pFirstSelect->GetParentRectPtr()->top)
            {
                bLoopFromTopToBottom = false;
            }
            else if (pItemPress->GetParentRectPtr()->top > pFirstSelect->GetParentRectPtr()->top)
            {
                bLoopFromTopToBottom = true;
            }
            else
            {
                if (pItemPress->GetParentRectPtr()->left < pFirstSelect->GetParentRectPtr()->left)
                {
                    bLoopFromTopToBottom = false;
                }
            }

            ListItemBase* pItem = pFirstSelect;
            while (pItem)
            {
                if (pItem->IsSelectable() && pItem->IsMySelfVisible())
                {
                    m_pListCtrlBase->AddSelectItem(pItem, false);
                }
                if (pItem == pItemPress)
                    break;

                // �������±���
                if (bLoopFromTopToBottom)
                    pItem = pItem->GetNextTreeItem();
                else
                    pItem = pItem->GetPrevItem();
            }

            SetFocusItem(pItemPress);  // �����������Ϊfoucs�����̲��������ڸ���
            m_pListCtrlBase->FireSelectItemChanged(NULL);
        }
        else if (bCtrlDown)
        {
            if (pItemPress->IsSelected())
                m_pListCtrlBase->RemoveSelectItem(pItemPress, true);
            else
                m_pListCtrlBase->AddSelectItem(pItemPress, true);
            
            SetFocusItem(pItemPress);
        }
        else
        {
            // �Լ����ڶ�ѡʱ��������Լ������û�з�����ק���������ڵ���ʱֻѡ���Լ�
            // ��ע����������ק���ͷ�ʱ�������ߵ�LBUTTONUP��Ϣ�ˣ�
            if (pItemPress->IsSelected() && m_pListCtrlBase->IsSelectMulti())
            {
                POINT ptWnd = {GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam)};
                ListItemBase*  pNewHover = GetItemByPos(ptWnd);
                if (pItemPress == pNewHover)
                {
                    m_pListCtrlBase->SelectItem(pItemPress);
                }
            }
        }
    }

}
void  MultiSelListCtrlMKMgr::OnMouseLeave(UIMSG* pMsg)
{
    if (m_pSelectRegionRender)
    {
        m_pSelectRegionRender->LButtonUp();
        SAFE_DELETE(m_pSelectRegionRender);
    }

    __super::OnMouseLeave(pMsg);
}


void  MultiSelListCtrlMKMgr::OnRButtonDown(UIMSG* pMsg)
{
    __super::OnRButtonDown(pMsg);

//     if (m_pListCtrlBase->TestBaseStyle(LISTCTRLBASE_STYLE_RBUTTONDOWN_KEEPSELECTION))
//         return;

    if (m_pItemHover)
    {
        if (m_pItemHover->IsSelectable())
        {
            if (m_pItemHover->IsSelected())
            {
                // ���޸�
            }
            else
            {
                // ֻѡ��һ��
                m_pListCtrlBase->SelectItem(m_pItemHover);
            }

        }
    }
    else
    {
        m_pListCtrlBase->ClearSelectItem(true);
    }
}

void  MultiSelListCtrlMKMgr::OnKeyDown(UIMSG* pMsg, bool* pbInterestMsg)
{
    if (pbInterestMsg)
        *pbInterestMsg = true;

    switch (pMsg->wParam)
    {
    case VK_DOWN:
    case VK_RIGHT:  // icon listview
        OnKeyDown_down(pMsg);
        return;

    case VK_UP:
    case VK_LEFT:  // icon listview
        OnKeyDown_up(pMsg);
        return;

    case VK_SPACE:
        OnKeyDown_space(pMsg);
        break;

    case VK_PRIOR:
        OnKeyDown_prior(pMsg);
        break;

    case VK_NEXT:
        OnKeyDown_next(pMsg);
        break;;

    case VK_HOME:
        OnKeyDown_home(pMsg);
        break;

    case VK_END:
        OnKeyDown_end(pMsg);
        break;

    case 'A':
        if (!IsKeyDown(VK_CONTROL))
        {
            if (pbInterestMsg)
                *pbInterestMsg = false;
            break;
        }
        m_pListCtrlBase->SelectAll(false);
        break;

    default:
        if (pbInterestMsg)
            *pbInterestMsg = false;
        break;
    }
}

void  MultiSelListCtrlMKMgr::OnKeyDown_up(UIMSG* pMsg)
{
    if (NULL == m_pItemFocus)
    {
        ListItemBase* pFirstVisbleItem = m_pListCtrlBase->FindSelectableItemFrom(NULL);
        if (pFirstVisbleItem)
        {
            this->SetFocusItem(pFirstVisbleItem);
            this->m_pListCtrlBase->SelectItem(pFirstVisbleItem);
        }
        return;
    }

    ListItemBase* pPrevItem = m_pItemFocus->GetPrevSelectableItem();

    bool bCtrlDown = IsKeyDown(VK_CONTROL);
    bool bShiftDown = IsKeyDown(VK_SHIFT);
    
    if (bShiftDown)
    {
        ListItemBase*  pFirstSelect = m_pListCtrlBase->GetFirstSelectItem();
        if (pFirstSelect)
        {
            bool bBelowFirstSelect = 
                (pFirstSelect->GetParentRectPtr()->left < m_pItemFocus->GetParentRectPtr()->left ||
                pFirstSelect->GetParentRectPtr()->top < m_pItemFocus->GetParentRectPtr()->top) ? 
                true : false;

            if (bBelowFirstSelect) // ȡ���Լ���ѡ��״̬����focus����Ϊ��һ��
            {
                if (m_pItemFocus == pFirstSelect) 
                {
                    if (pPrevItem)
                    {
                        m_pListCtrlBase->AddSelectItem(pPrevItem, true);
                        this->SetFocusItem(pPrevItem);
                    }
                }
                else
                {
                    if (!bCtrlDown)
                        m_pListCtrlBase->RemoveSelectItem(m_pItemFocus, true);
                    if (pPrevItem)
                        this->SetFocusItem(pPrevItem);
                }
            }
            else // ����ѡ��һ��
            {
                if (pPrevItem)
                {
                    m_pListCtrlBase->AddSelectItem(pPrevItem, true);
                    this->SetFocusItem(pPrevItem);
                }
            }
        }
        else
        {
            // ѡ��focus������ѡ��focus����һ��
            m_pListCtrlBase->AddSelectItem(m_pItemFocus, false);
            if (pPrevItem)
            {
                m_pListCtrlBase->AddSelectItem(pPrevItem, false);
                SetFocusItem(pPrevItem);
            }
            m_pListCtrlBase->FireSelectItemChanged(NULL);
        }
    }
    else if (bCtrlDown)
    {
        // ���ı�focusitem����Ӱ��selection
        if (pPrevItem)
            this->SetFocusItem(pPrevItem);
    }
    else
    {
        if (pPrevItem)
        {
            this->m_pListCtrlBase->SelectItem(pPrevItem);
        }
        else if (!m_pItemFocus->IsSelected())
        {
            this->m_pListCtrlBase->SelectItem(m_pItemFocus);
        }
    }
}

void  MultiSelListCtrlMKMgr::OnKeyDown_down(UIMSG* pMsg)
{
    if (NULL == m_pItemFocus)
    {
        ListItemBase* pFirstVisbleItem = m_pListCtrlBase->FindSelectableItemFrom(NULL);
        if (pFirstVisbleItem)
        {
            this->SetFocusItem(pFirstVisbleItem);
            this->m_pListCtrlBase->SelectItem(pFirstVisbleItem);
        }
        return;
    }

    ListItemBase* pNextItem = m_pItemFocus->GetNextSelectableItem();

    bool bCtrlDown = IsKeyDown(VK_CONTROL);
    bool bShiftDown = IsKeyDown(VK_SHIFT);

    if (bShiftDown)
    {
        ListItemBase*  pFirstSelect = m_pListCtrlBase->GetFirstSelectItem();
        if (pFirstSelect)
        {
            bool bBelowFirstSelect = 
                (pFirstSelect->GetParentRectPtr()->left < m_pItemFocus->GetParentRectPtr()->left ||
                pFirstSelect->GetParentRectPtr()->top < m_pItemFocus->GetParentRectPtr()->top) ? 
                true : false;

            if (bBelowFirstSelect) // ����ѡ��һ��
            {
                if (pNextItem)
                {
                    m_pListCtrlBase->AddSelectItem(pNextItem, true);
                    this->SetFocusItem(pNextItem);
                }
            }
            else // ȡ���Լ���ѡ��״̬����focus����Ϊ��һ��
            {
                if (m_pItemFocus == pFirstSelect) 
                {
                    if (pNextItem)
                    {
                        m_pListCtrlBase->AddSelectItem(pNextItem, true);
                        this->SetFocusItem(pNextItem);
                    }
                }
                else
                {
                    if (!bCtrlDown)
                        m_pListCtrlBase->RemoveSelectItem(m_pItemFocus, true);
                    if (pNextItem)
                        this->SetFocusItem(pNextItem);
                }
            }
        }
        else
        {
            // ѡ��focus������ѡ��focus����һ��
            m_pListCtrlBase->AddSelectItem(m_pItemFocus, false);
            if (pNextItem)
            {
                m_pListCtrlBase->AddSelectItem(pNextItem, false);
                SetFocusItem(pNextItem);
            }
            m_pListCtrlBase->FireSelectItemChanged(NULL);
        }
    }
    else if (bCtrlDown)
    {
        if (pNextItem)
        {
            this->SetFocusItem(pNextItem);
        }
    }
    else
    {
        if (pNextItem)
        {
            this->m_pListCtrlBase->SelectItem(pNextItem);
        }
        else if (!m_pItemFocus->IsSelected())
        {
            this->m_pListCtrlBase->SelectItem(m_pItemFocus);
        }
    }
}


void  MultiSelListCtrlMKMgr::OnKeyDown_space(UIMSG* pMsg)
{
    if (NULL == m_pItemFocus)
    {
        ListItemBase* pFirstSelectableItem = m_pListCtrlBase->FindSelectableItemFrom(NULL);
        if (pFirstSelectableItem)
        {
            this->m_pListCtrlBase->SelectItem(pFirstSelectableItem);
        }
    }
    else
    {
        this->m_pListCtrlBase->SelectItem(m_pItemFocus);
    }
}
void  MultiSelListCtrlMKMgr::OnKeyDown_prior(UIMSG* pMsg)
{
    if (NULL == m_pItemFocus)
    {
        ListItemBase* pFirstSelectableItem = m_pListCtrlBase->FindSelectableItemFrom(NULL);
        if (pFirstSelectableItem)
        {
            this->m_pListCtrlBase->SelectItem(pFirstSelectableItem);
        }
    }
    else
    {
        IScrollBarManager*  pScrollMgr = m_pListCtrlBase->GetIScrollBarMgr();
        if (pScrollMgr->DoPageUp())
        {
            this->m_pListCtrlBase->Invalidate();
        }

        this->m_pListCtrlBase->CalcFirstLastDrawItem();
        ListItemBase*  pNewFocusItem = m_pListCtrlBase->GetFirstDrawItem();
        this->m_pListCtrlBase->SelectItem(pNewFocusItem);
    }
}
void  MultiSelListCtrlMKMgr::OnKeyDown_next(UIMSG* pMsg)
{
    if (NULL == m_pItemFocus)
    {
        ListItemBase* pFirstSelectableItem = m_pListCtrlBase->FindSelectableItemFrom(NULL);
        if (pFirstSelectableItem)
        {
            this->m_pListCtrlBase->SelectItem(pFirstSelectableItem);
        }
    }
    else
    {
        IScrollBarManager*  pScrollMgr = m_pListCtrlBase->GetIScrollBarMgr();
        pScrollMgr->DoPageDown();

        this->m_pListCtrlBase->CalcFirstLastDrawItem();
        ListItemBase*  pNewFocusItem = m_pListCtrlBase->GetLastDrawItem();
        this->m_pListCtrlBase->SelectItem(pNewFocusItem);

        this->m_pListCtrlBase->Invalidate();
    }
}

void  MultiSelListCtrlMKMgr::OnKeyDown_home(UIMSG* pMsg)
{
    if (NULL == m_pItemFocus)
    {
        ListItemBase* pFirstSelectableItem = m_pListCtrlBase->FindSelectableItemFrom(NULL);
        if (pFirstSelectableItem)
        {
            this->m_pListCtrlBase->SelectItem(pFirstSelectableItem);
        }
    }
    else
    {
        IScrollBarManager*  pScrollMgr = m_pListCtrlBase->GetIScrollBarMgr();
        pScrollMgr->DoHome();

        this->m_pListCtrlBase->CalcFirstLastDrawItem();
        ListItemBase*  pNewFocusItem = m_pListCtrlBase->GetFirstDrawItem();
        this->m_pListCtrlBase->SelectItem(pNewFocusItem);

        this->m_pListCtrlBase->Invalidate();
    }
}
void  MultiSelListCtrlMKMgr::OnKeyDown_end(UIMSG* pMsg)
{
    if (NULL == m_pItemFocus)
    {
        ListItemBase* pFirstSelectableItem = m_pListCtrlBase->FindSelectableItemFrom(NULL);
        if (pFirstSelectableItem)
        {
            this->m_pListCtrlBase->SelectItem(pFirstSelectableItem);
        }
    }
    else
    {
        IScrollBarManager*  pScrollMgr = m_pListCtrlBase->GetIScrollBarMgr();
        pScrollMgr->DoEnd();
        
        this->m_pListCtrlBase->CalcFirstLastDrawItem();
        ListItemBase*  pNewFocusItem = m_pListCtrlBase->GetLastDrawItem();
        this->m_pListCtrlBase->SelectItem(pNewFocusItem);
        
        this->m_pListCtrlBase->Invalidate();
    }
}

void  MultiSelListCtrlMKMgr::OnMouseMove_DragSelectRegion(UIMSG* pMsg)
{
    POINT point = {GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam)};
    if (NULL == m_pSelectRegionRender)
    {
        m_pSelectRegionRender = new DotLine_SelectRegionRender();
        if (false == m_pSelectRegionRender->Init(m_pListCtrlBase, point))
        {
            SAFE_DELETE(m_pSelectRegionRender);
            return;
        }
    }
    else
    {
        m_pSelectRegionRender->MouseMove(point);
    }

    CRect  rcSelectRegion;
    m_pSelectRegionRender->GetSelectRegion(&rcSelectRegion);
    rcSelectRegion.NormalizeRect();

    CRect  rcTest;
    ListItemBase* pItem = m_pListCtrlBase->GetFirstDrawItem();
    ListItemBase* pLastVisibleItem = m_pListCtrlBase->GetLastDrawItem();
    while (pItem)
    {
        CRect rcItem;
        pItem->GetParentRect(&rcItem);

        bool bOldSelect = m_pListCtrlBase->IsSelected(pItem);
        bool bNowSelect = rcTest.IntersectRect(&rcSelectRegion, &rcItem) ? true:false;

        if (bOldSelect != bNowSelect)
        {
            if (bNowSelect)
            {
                m_pListCtrlBase->AddSelectItem(pItem, false);
            }
            else
            {
                m_pListCtrlBase->RemoveSelectItem(pItem, true);
            }
            m_pListCtrlBase->InvalidateItem(pItem);
        }

        if (pItem == pLastVisibleItem)
            break;

        pItem = pItem->GetNextVisibleItem();
    }
}

//////////////////////////////////////////////////////////////////////////


DotLine_SelectRegionRender::DotLine_SelectRegionRender()
{
    m_hWndDC  = NULL;
    m_hWnd    = NULL;
    m_hClipRgn = NULL;
    m_rcClip.SetRectEmpty();
    m_rcView.SetRectEmpty();
}
DotLine_SelectRegionRender::~DotLine_SelectRegionRender()
{
    if (m_hWndDC)
    {
        ReleaseDC(m_hWnd, m_hWndDC);
    }
    SAFE_DELETE_GDIOBJECT(m_hClipRgn);
}

bool  DotLine_SelectRegionRender::Init(Object* pObj, POINT pt)
{
    __super::Init(pObj, pt);
    m_hWnd = m_pObject->GetHWND();
#ifdef _DEBUG
    long lStyleEx = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
    if (lStyleEx & WS_EX_LAYERED)
    {
        return false;
    }
#endif

    // ����ü�����
    CRect rcVisible;
    CRect rcClient;
    if (false == m_pObject->GetRectInWindow(&rcVisible, true))   // �ö����ڴ����ϲ��ɼ���������
        return false;

    m_hWndDC = GetDC(m_hWnd);

    m_pObject->GetClientRectInWindow(&rcClient);
    m_rcClip.IntersectRect(&rcClient, &rcVisible);
    m_hClipRgn = CreateRectRgnIndirect(&m_rcClip);
    ::SelectClipRgn(m_hWndDC, m_hClipRgn);

    // ����DCƫ��
    int xOffset = 0, yOffset = 0;
    m_pObject->GetScrollOffset(&xOffset, &yOffset);
    ::OffsetViewportOrgEx(m_hWndDC, rcClient.left-xOffset, rcClient.top-yOffset, NULL);

    // ���Ʒ�Χ
    int xRange = 0, yRange = 0;
    m_pObject->GetScrollRange(&xRange, &yRange);
    m_rcView.right = max(rcClient.Width(), xRange);
    m_rcView.bottom = max(rcClient.Height(), yRange);

    // ��ʼλ��
    POINT ptClient = {0,0};
    m_pObject->WindowPoint2ObjectClientPoint(&pt, &ptClient, true);
    m_rcSelectRegion.SetRect(ptClient.x, ptClient.y, ptClient.x, ptClient.y);

    CRect rcTempNew = NormalizeRect(&m_rcSelectRegion);
    Draw(&rcTempNew);
    return true;
}

void  DotLine_SelectRegionRender::MouseMove(POINT point)
{
    POINT ptClient = {0,0};
    m_pObject->WindowPoint2ObjectClientPoint(&point, &ptClient, true);

    if (ptClient.x < m_rcView.left)
        ptClient.x = m_rcView.left;
    else if (ptClient.x > m_rcView.right)
        ptClient.x = m_rcView.right;

    if (ptClient.y < m_rcView.top)
        ptClient.y = m_rcView.top;
    else if (ptClient.y > m_rcView.bottom)
        ptClient.y = m_rcView.bottom;

    CRect rcPrevMouseMove(0,0,0,0);
    ::CopyRect(&rcPrevMouseMove, &m_rcSelectRegion);

    m_rcSelectRegion.right = ptClient.x;
    m_rcSelectRegion.bottom = ptClient.y;

    CRect rcTempOld = NormalizeRect(&rcPrevMouseMove);
    CRect rcTempNew = NormalizeRect(&m_rcSelectRegion);

    Draw(&rcTempOld);
    Draw(&rcTempNew);
}
void  DotLine_SelectRegionRender::LButtonUp()
{
    RECT rcTemp = NormalizeRect(&m_rcSelectRegion);
    Draw(&rcTemp);

    ::SelectClipRgn(m_hWndDC, NULL);
}

void  DotLine_SelectRegionRender::Draw(RECT* prc)
{
    UIASSERT(0);
    // TODO:
//     WindowBase* pWindow = m_pObject->GetWindowObject();
//     HDC hDC = pWindow->GetRenderChainMemDC();
//     ::DrawFocusRect(hDC, prc);
//     pWindow->CommitDoubleBuffet2Window(NULL, NULL, prc);
}
// 
// void  DotLine_SelectRegionRender::PrePaint()
// {
//     WindowBase* pWindow = m_pObject->GetWindowObject();
//     if (pWindow)
//     {
//         pWindow->AddCommitWindowBufferListener(static_cast<ICommitWindowBufferListener*>(this));
//     }
// 
//     RECT rcTemp = NormalizeRect(&m_rcSelectRegion);
//     ::DrawFocusRect(m_hWndDC, &rcTemp);
// }
// 
// void  DotLine_SelectRegionRender::PreCommitWindowBuffer(HDC hDC, HDC hMemDC, IWindowBase* pWindow, RECT* prcCommit, int nRectCount)
// {
// 
// }
// void  DotLine_SelectRegionRender::PostCommitWindowBuffer(HDC hDC, HDC hMemDC, IWindowBase* pWindow, RECT* prcCommit, int nRectCount)
// {
//     RECT rcTemp = NormalizeRect(&m_rcSelectRegion);
//     ::DrawFocusRect(m_hWndDC, &rcTemp);
// }
