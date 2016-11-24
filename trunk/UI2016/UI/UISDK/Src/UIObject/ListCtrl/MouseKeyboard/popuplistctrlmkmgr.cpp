#include "stdafx.h"
#include "popuplistctrlmkmgr.h"
#include "Src\UIObject\ListCtrl\listctrlbase.h"
#include "Src\UIObject\ListCtrl\ListItemBase\listitembase.h"
#include "Src\Base\Application\uiapplication.h"

using namespace UI;

PopupListCtrlMKMgr::PopupListCtrlMKMgr()
{
    m_ptLastMousePos.x = m_ptLastMousePos.y = -1;
}

BOOL  PopupListCtrlMKMgr::DoProcessMessage(UIMSG* pMsg)
{
    bool bInterestMsg = true;
    switch (pMsg->message)
    {
    case WM_MOUSEMOVE:
        OnMouseMove(pMsg);
        break;

    case WM_MOUSELEAVE:
        OnMouseLeave(pMsg);
        break;

    case WM_LBUTTONDOWN:
        OnLButtonDown(pMsg);
        break;

    case WM_LBUTTONUP:
        OnLButtonUp(pMsg);
        break;

    case WM_KEYDOWN:
        OnKeyDown(pMsg, &bInterestMsg);
        break;

    default:
        return __super::DoProcessMessage(pMsg);
        break;
    }

    if (bInterestMsg)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void  PopupListCtrlMKMgr::OnMouseMove(UIMSG* pMsg)
{
    POINT  ptWindow = { GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam) };

    // ����ԭ���͹�����WM_MOUSEMOVE��Ϣ������������Ӱ��ʹ�����·�����л���ǰ��
    if (ptWindow.x == m_ptLastMousePos.x && ptWindow.y == m_ptLastMousePos.y) 
        return;
    m_ptLastMousePos = ptWindow;

    ListItemBase*  pNewHover = GetItemByPos(ptWindow);

    if (pNewHover != m_pItemHover)
    {
        if (NULL == pNewHover)  // ��ȡ����ǰ������������һ���µ�hover����
        {
            if (m_pUIApplication)
                m_pUIApplication->HideToolTip();

            return;  
        }

        SetHoverItem(pNewHover);

        ListItemBase* pSelItem = m_pListCtrlBase->GetFirstSelectItem();
        if (pSelItem)
            this->m_pListCtrlBase->InvalidateItem(pSelItem);  // ����hover��ʱ������ʾselected��
    }
    if (m_pItemHover)
    {
        UISendMessage(m_pItemHover->GetIListItemBase(), WM_MOUSEMOVE, pMsg->wParam, pMsg->lParam);
    }	
}

void  PopupListCtrlMKMgr::OnMouseLeave(UIMSG* pMsg)
{
    if (m_pItemHover)
    {
        UISendMessage(m_pItemHover->GetIListItemBase(), WM_MOUSELEAVE, pMsg->wParam, pMsg->lParam);
        if (m_pUIApplication)
            m_pUIApplication->HideToolTip();
    }
}

// ��갴��ʱ��ѡ�У�����ʱ��ѡ��
void  PopupListCtrlMKMgr::OnLButtonUp(UIMSG* pMsg)
{
    BOOL bHandled = FALSE;
    if (m_pItemPress)
    {
        ListItemBase* pSave = m_pItemPress;
        this->SetPressItem(NULL);
        long lRet = UISendMessage(pSave->GetIListItemBase(), WM_LBUTTONUP, pMsg->wParam, pMsg->lParam,0, 0,0, &bHandled);

        POINT pt = {0};
        ::GetCursorPos(&pt);
        ::ScreenToClient(m_pListCtrlBase->GetIListCtrlBase()->GetHWND(), &pt);
        ListItemBase*  pNowHover = m_pListCtrlBase->GetItemByWindowPoint(pt);
       
        if (!(lRet & IMKER_DONT_CHANGE_SELECTION))
        {
            if (m_pItemHover)  // ѡ���������µ���һ���������굱�����µ�����
            {
                if (m_pItemHover == pNowHover)
                {
                    // ֻѡ��һ��
                    // ListItemBase* pItemSave = m_pItemHover;
                    m_pListCtrlBase->SelectItem(m_pItemHover);
                }
            }
            else
            {
                m_pListCtrlBase->ClearSelectItem(true);
            }
        }

        if (!(lRet & (IMKER_HANDLED_BY_ITEM|IMKER_HANDLED_BY_INNERCONTROL)))
        {
            if (pSave &&  pSave == pNowHover)
            {
//                 UIMSG  msg;
//                 msg.message = UI_MSG_NOTIFY;
//                 msg.nCode   = UI_NM_CLICK;
//                 msg.wParam  = (WPARAM)pSave->GetIListItemBase();
//                 msg.pMsgFrom = m_pListCtrlBase->GetIListCtrlBase();
//                 msg.pMsgFrom->DoNotify(&msg);

                m_pListCtrlBase->click.emit(
                    m_pListCtrlBase->GetIListCtrlBase(),
                    pSave ? pSave->GetIListItemBase() : NULL);
            }
        }
    }
}
void  PopupListCtrlMKMgr::OnLButtonDown(UIMSG* pMsg)
{
    ListCtrlMKMgrBase::OnLButtonDown(pMsg);
}

void  PopupListCtrlMKMgr::OnKeyDown(UIMSG* pMsg, bool* pbInterestMsg)
{
    if (pbInterestMsg)
        *pbInterestMsg = true;

    switch (pMsg->wParam)
    {
    case VK_DOWN:
        OnKeyDown_down(pMsg);
        return;

    case VK_UP:
        OnKeyDown_up(pMsg);
        return;

    default:
        __super::OnKeyDown(pMsg, pbInterestMsg);
        break;
    }
}

void  PopupListCtrlMKMgr::OnKeyDown_up(UIMSG* pMsg)
{
    if (NULL == m_pItemHover)
    {
        ListItemBase* pSelItem = m_pListCtrlBase->GetFirstSelectItem();
        if (NULL == pSelItem)
        {
            ListItemBase* pItem = m_pListCtrlBase->FindSelectableItemFrom(NULL);
            if (pItem)
                m_pListCtrlBase->SelectItem(pItem, false);
        }
        else 
        {
            ListItemBase* pPrevItem = pSelItem->GetPrevSelectableItem();
            if (pPrevItem)
                m_pListCtrlBase->SelectItem(pPrevItem, false);
        }
    }
    else   // �ڴ���HOVER�����������棬ѡ��HOVER����һ������
    {
        ListItemBase* pPrevItem = m_pItemHover->GetPrevSelectableItem();
        if (pPrevItem)
            m_pListCtrlBase->SelectItem(pPrevItem, false);
        else
            m_pListCtrlBase->SelectItem(m_pItemHover, false);
    }

    if (m_pListCtrlBase->GetFirstSelectItem())  // ���hover������ʾselection����
    {
        SetHoverItem(NULL);
    }
}

void  PopupListCtrlMKMgr::OnKeyDown_down(UIMSG* pMsg)
{
    ListItemBase* pSelItem = m_pListCtrlBase->GetFirstSelectItem();
    if (NULL == m_pItemHover)
    {
        if (NULL == pSelItem)
        {
            ListItemBase* pItem = m_pListCtrlBase->FindSelectableItemFrom(NULL);
            if (pItem)
                m_pListCtrlBase->SelectItem(pItem, false);
        }
        else 
        {
            ListItemBase* pNextItem = pSelItem->GetNextSelectableItem();
            if (pNextItem)
                m_pListCtrlBase->SelectItem(pNextItem, false);		
        }
    }
    else   // �ڴ���HOVER�����������棬ѡ��HOVER����һ������
    {
        ListItemBase* pNextItem = m_pItemHover->GetNextSelectableItem();
        if (pNextItem)
        {
            if (pSelItem == pNextItem)
                m_pListCtrlBase->InvalidateItem(pSelItem);
            else
                m_pListCtrlBase->SelectItem(pNextItem, false);
        }
        else
        {
            m_pListCtrlBase->SelectItem(m_pItemHover, false);
        }
    }

    if (m_pListCtrlBase->GetFirstSelectItem())  // ���hover������ʾselection����
    {
        SetHoverItem(NULL);
    }
}


