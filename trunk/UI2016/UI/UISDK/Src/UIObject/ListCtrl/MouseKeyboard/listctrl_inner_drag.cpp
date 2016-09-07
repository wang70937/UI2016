#include "stdafx.h"
#include "listctrl_inner_drag.h"
#include <assert.h>
#include "..\listctrlbase.h"
#include "..\ListItemBase\listitembase.h"
#include "..\animate_callback\listctrl_animate_callback.h"


enum{
	ANIMATE_UNIQUE_ID = 163162232
};


ListCtrlInnerDragMgr::ListCtrlInnerDragMgr()
{
	m_pItemDrag = NULL;
	m_ptPress.x = m_ptPress.y = -32000;
	m_nOldHScrollPos = 0;
	m_nOldVScrollPos = 0;
	SetRectEmpty(&m_rcOldPressItem);
	m_nOldPressItemIndex = 0;
	m_bDraging = false;
	m_bLButtondown = false;
}

ListCtrlInnerDragMgr::~ListCtrlInnerDragMgr()
{
}

BOOL UI::ListCtrlInnerDragMgr::ProcessMessage(UIMSG* pMsg)
{
	BOOL bHandled = FALSE;
	switch (pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			bHandled = OnDragMouseMove(pMsg->wParam, pMsg->lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			OnDragLButtonDown(pMsg->wParam, pMsg->lParam);
		}
		break;
	case WM_LBUTTONUP:
		{
			bHandled = OnDragLButtonUp(pMsg->wParam, pMsg->lParam);
		}
		break;
	case WM_KEYDOWN:
		{
			if (pMsg->wParam == VK_ESCAPE)
			{
                if (m_bDraging)
                {
				    CancelDrag();
                    bHandled = TRUE;
                }
			}
		}
		break;

	case WM_KILLFOCUS:
	case WM_CANCELMODE:
		{
			if (m_bLButtondown)
				m_bLButtondown = FALSE;

			AcceptDrag();
		}
		break;
	}
	return bHandled;
}
void  ListCtrlInnerDragMgr::SetListCtrlBase(ListCtrlBase*  pCtrl)
{
    m_pListCtrlBase = pCtrl;
}

void  ListCtrlInnerDragMgr::OnRemoveAll()
{
    __EndDrag();
}

void  ListCtrlInnerDragMgr::OnRemoveItem(ListItemBase* pItem)
{
    if (pItem == m_pItemDrag)
        m_pItemDrag = NULL;

    CancelDrag();
}

BOOL UI::ListCtrlInnerDragMgr::OnDragMouseMove(WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = FALSE;
	if (!m_bLButtondown)
		return FALSE;

	POINT ptNow = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

	if (!m_bDraging)
	{
        // �п�������ק����ˣ���������û�н�����
        if (m_pItemDrag)
            return bHandled;

		if (abs(ptNow.x-m_ptPress.x) > 5 ||
			abs(ptNow.y-m_ptPress.y) > 5)
		{
			bHandled = TRUE;
			StartDrag(ptNow);
		}
	}
	else
	{
		bHandled = TRUE;
		Draging(ptNow);
	}
	return bHandled;
}

void UI::ListCtrlInnerDragMgr::OnDragLButtonDown(WPARAM wParam, LPARAM lParam)
{
	m_ptPress.x = GET_X_LPARAM(lParam);
	m_ptPress.y = GET_Y_LPARAM(lParam);
	m_bLButtondown = true;
	m_nOldHScrollPos = m_pListCtrlBase->GetScrollOffset(
			&m_nOldHScrollPos, &m_nOldVScrollPos);
}

BOOL UI::ListCtrlInnerDragMgr::OnDragLButtonUp(WPARAM wParam, LPARAM lParam)
{
    BOOL  bHandled = FALSE;

	if (m_bDraging)
	{
		AcceptDrag();

        //bHandled = TRUE;  // <-- ��������ΪTRUE�����ý���mkȥ�ͷ�press item
	}
	m_bLButtondown = false;

    return bHandled;
}

void UI::ListCtrlInnerDragMgr::StartDrag(POINT pt)
{
	ListItemBase*  pPressItem = m_pListCtrlBase->GetPressItem();
	if (!pPressItem)
		return;

    if (pPressItem->IsFloat())
        return;

	//CListItemMoveAnimate::Cancel(*pPressItem);
	LISTITEMSTYLE s = {0};
	s.bMoveAnimating = 1;
	if (pPressItem->TestStyle(s))
		return;

	m_pItemDrag = pPressItem;

    // ��հ��µĿؼ������⿪ʼ��קʱ����������ڿؼ����棬
    // ��ק����ʱ�ֻᴥ������ؼ��Ĳ�����
    if (m_pListCtrlBase->GetMKManager())
        m_pListCtrlBase->GetMKManager()->SetPressObject(NULL);

	m_bDraging = true;

    CRect rc2Float;
        
    m_pItemDrag->GetParentRect(&rc2Float);
    m_pListCtrlBase->ItemRect2ObjectRect(&rc2Float, &rc2Float);
    m_pItemDrag->SetFloat(true);
    m_pItemDrag->SetFloatRect(&rc2Float);

    m_rcOldPressItem = rc2Float;
}

//
// 1. ����rcFloat�����б������λ��
// 2. ����rcFloat�ڿͻ�����֮��
//
void  update_scrollpos_by_float_rect(
            CRect& rcFloat, 
            ListCtrlBase* pListCtrl, 
            bool& bScrollPosChanged)
{
    bScrollPosChanged = false;

    CRect rcClient;
    pListCtrl->GetIListCtrlBase()->GetClientRectInObject(&rcClient);

    if (rcFloat.top < rcClient.top)
    {
        if (pListCtrl->ScrollY(-10, false))
        {
            bScrollPosChanged = true;
        }

        OffsetRect(&rcFloat, 0, rcClient.top - rcFloat.top);
    }
    else if (rcFloat.bottom > rcClient.bottom)
    {
        if (pListCtrl->ScrollY(10, false))
        {
            bScrollPosChanged = true;
        }

        OffsetRect(&rcFloat, 0, rcClient.bottom - rcFloat.bottom);
    }

    if (rcFloat.left < rcClient.left)
    {
        if (pListCtrl->ScrollX(-10, false))
        {
            bScrollPosChanged = true;
        }

        OffsetRect(&rcFloat, rcClient.left - rcFloat.left, 0);
    }
    else if (rcFloat.right > rcClient.right)
    {
        if (pListCtrl->ScrollX(10, false))
        {
            bScrollPosChanged = true;
        }

        OffsetRect(&rcFloat, rcClient.right - rcFloat.right, 0);
    }

    if (bScrollPosChanged)
    {
        pListCtrl->SetCalcFirstLastDrawItemFlag();
    }
}

// �ж�rcIntersect�Ƿ񳬹���rcItem��һ��λ��
bool  test_intersect_more_than_half(CRect& rcItem, CRect& rcIntersect)
{
    CPoint ptCenter = rcItem.CenterPoint();

    if (rcIntersect.right <= ptCenter.x)
        return false;

    if (rcIntersect.left >= ptCenter.x)
        return false;

    if (rcIntersect.bottom <= ptCenter.y)
        return false;

    if (rcIntersect.top >= ptCenter.y)
        return false;
    
    return true;
}

// ������λ������ǰ���ˣ�����������ˡ�
// ��ǰ���ˣ���Ҫ�� insert before
// ������ˣ���Ҫ�� insert after
enum HIT_ITEM_RELATIVE_POS
{
    INVALID_POS,
    BEFORE_ME,
    AFTER_ME
};

//
// ��⵱ǰ��קλ�����ĸ�item���ཻ
// ע������Ĭ�����е�item��С��һ����
//
ListItemBase*  drag_hit_test(
                    CRect* prcDragParent, 
                    ListItemBase* pDragItem, 
                    ListCtrlBase* pListCtrl,
                    HIT_ITEM_RELATIVE_POS& ePos)
{
    assert (pDragItem && pListCtrl);
    ePos = INVALID_POS;

    CRect rcItem;
    CRect rcIntersect;

    ListItemBase* p = pListCtrl->GetFirstDrawItem();
    ListItemBase* pLast = pListCtrl->GetLastDrawItem();

    if (!p || !pLast)
        return NULL;

    pLast = pLast->GetNextVisibleItem();
    ePos = BEFORE_ME;

    LISTITEMSTYLE s = {0};
    s.bMoveAnimating = 1;

    for (; p != pLast; p = p->GetNextVisibleItem())
    {
        if (p == pDragItem)
        {
            ePos = AFTER_ME;
            continue;;
        }

        // ���ڶ���item������
        if (p->TestStyle(s))
            continue;

        p->GetParentRect(&rcItem);
        rcIntersect.IntersectRect(&rcItem, prcDragParent);

        // �ж��ཻ���
        if (!test_intersect_more_than_half(rcItem, rcIntersect))
            continue;

        return p;
    }

    ePos = INVALID_POS;
    return NULL;
}

void  ListCtrlInnerDragMgr::Draging(POINT pt)
{
    // ������ק���λ��
    int nyMove = pt.y - m_ptPress.y;
    int nxMove = pt.x - m_ptPress.x;

    CRect rcNewFloat = m_rcOldPressItem;
    OffsetRect(&rcNewFloat, nxMove, nyMove);

    // ��Ե����
    bool bScrollPosChanged = false;
    update_scrollpos_by_float_rect(
            rcNewFloat, 
            m_pListCtrlBase, 
            bScrollPosChanged);

    // ������ק���λ��
    CRect rcOldFloat;
    m_pItemDrag->GetFloatRect(&rcOldFloat);
	
	// ��������һ���ơ��ᵼ��item�Ƶ��߽�ʱ��������ⲿ�ƶ�Ҳ������hittest������
	// ʱ���ܸ�item���滹��һ��item��ȴ���Ὣ��item������
//     if (rcOldFloat.EqualRect(&rcNewFloat))
//     {
//         if (bScrollPosChanged)
//             m_pListCtrlBase->GetIListCtrlBase()->Invalidate();
//         
//         return;
//     }

    m_pItemDrag->SetFloatRect(&rcNewFloat);

    // ����Ҫռ�ݵ�λ��
    CRect  rcNewDragItemRect;
    m_pListCtrlBase->ObjectRect2ItemRect(&rcNewFloat, &rcNewDragItemRect);

    HIT_ITEM_RELATIVE_POS ePos = INVALID_POS;
    ListItemBase* pHitTest = drag_hit_test(
                    &rcNewDragItemRect, 
                    m_pItemDrag, 
                    m_pListCtrlBase,
                    ePos);
    if (!pHitTest)
    {
        m_pListCtrlBase->GetIListCtrlBase()->Invalidate();
        return;
    }

    // ����λ��
    IListItemBase* pInsertAfter = pHitTest->GetIListItemBase();
    if (ePos == BEFORE_ME)
    {
        ListItemBase* pPrev = pHitTest->GetPrevVisibleItem();
        if (pPrev)
            pInsertAfter = pPrev->GetIListItemBase();
        else
            pInsertAfter = UITVI_FIRST;
    }
    
    if (m_pListCtrlBase->MoveItem(
                m_pItemDrag, 
                NULL,
                pInsertAfter))
    {
        m_pListCtrlBase->LayoutItem(NULL, false);
    }

    // ˢ��
    m_pListCtrlBase->GetIListCtrlBase()->Invalidate();
}

void  FloatItemAnimateEndCallback(UI::ListItemBase* pItem, long* pData)
{
    if (pItem)
    {
        pItem->SetFloat(false);
    }
}

void  ListCtrlInnerDragMgr::AcceptDrag()
{
	if (!m_bDraging || !m_pItemDrag)
		return;

	CRect rcFrom;
	m_pItemDrag->GetFloatRect(&rcFrom);

	CRect rcTo;
	m_pItemDrag->GetParentRect(&rcTo);

	m_pListCtrlBase->ItemRect2ObjectRect(&rcTo, &rcTo);

// 	if (!ListCtrlAnimateCallback::HandleItemRectChanged(
// 			*m_pItemDrag, &rcFrom, &rcTo))
	{
		m_pItemDrag->SetFloat(false);
	}


//     LISTCTRL_INNERDRAG_FINISH_DATA  data;
//     data.pDragItem = m_pItemDrag->GetIListItemBase();

    __EndDrag();

// 	UIMSG  msg;
// 	msg.message = UI_MSG_NOTIFY;
// 	msg.nCode = UI_LCN_INNERDRAG_FINISH;
// 	msg.pMsgFrom = m_pListCtrlBase->GetIListCtrlBase();
// 	msg.wParam = (WPARAM)&data;
// 	msg.pMsgFrom->DoNotify(&msg);
}

void UI::ListCtrlInnerDragMgr::CancelDrag()
{
	if (!m_bDraging)
		return;

	int nOldHScrollPos = m_nOldHScrollPos;
	int nOldVScrollPos = m_nOldVScrollPos;

    // ��UpdateItemRect��Ȼ��������������
    // ��Ϊ��UpdateItemRect�з���parent rectһ�µĻ���
    // ��ȡ����ǰ����
    m_pListCtrlBase->LayoutItem(NULL, false);

    if (m_pItemDrag)
    {
		CRect rcFrom;
		m_pItemDrag->GetFloatRect(&rcFrom);

// 		if (!ListCtrlAnimateCallback::HandleItemRectChanged(
// 					*m_pItemDrag, &rcFrom, &m_rcOldPressItem))
		{
			m_pItemDrag->SetFloat(false);
		}
    }

	__EndDrag();

    m_pListCtrlBase->SetScrollPos(nOldHScrollPos, nOldVScrollPos, false);
	m_pListCtrlBase->GetIListCtrlBase()->Invalidate();
}

void UI::ListCtrlInnerDragMgr::__EndDrag()
{
    m_pItemDrag = NULL;
	m_bDraging = false;
	m_bLButtondown = false;
	m_ptPress.x = m_ptPress.y = -32000;
	m_nOldHScrollPos = 0;
	m_nOldVScrollPos = 0;
	m_nOldPressItemIndex = 0;
}

bool UI::ListCtrlInnerDragMgr::IsDraging()
{
	return m_bDraging;
}

