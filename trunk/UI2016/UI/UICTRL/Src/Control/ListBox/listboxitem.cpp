#include "stdafx.h"
#include "listboxitem.h"
#include "listbox.h"

namespace UI
{

ListBoxItemShareData::ListBoxItemShareData(IListBoxItemShareData* p):MessageProxy(p)
{
    m_pIListBoxItemShareData = p;
}

ListBoxItem::ListBoxItem(IListBoxItem* p):MessageProxy(p)
{
    m_pIListBoxItem = p; 
    m_pShareData = NULL; 
}

void  ListBoxItem::OnInitialize()
{
    DO_PARENT_PROCESS(IListBoxItem, IListItemBase);

	m_pIListBoxItem->SetItemType(IListBoxItem::FLAG);

    IListCtrlBase*  pListCtrl = m_pIListBoxItem->GetIListCtrlBase();
    IListBoxItemShareData*  pData = static_cast<IListBoxItemShareData*>(
        pListCtrl->GetItemTypeShareData(IListBoxItem::FLAG));

    if (!pData)
    {
        pData = IListBoxItemShareData::CreateInstance(pListCtrl->GetSkinRes());
        m_pShareData = pData->GetImpl();
        pListCtrl->SetItemTypeShareData(IListBoxItem::FLAG, pData);
    }
    else
    {
        m_pShareData = pData->GetImpl();
    }
}

void  ListBoxItem::GetDesiredSize(SIZE* pSize)
{
    pSize->cx = pSize->cy = 0;

    ITextRenderBase* pTextRender = m_pIListBoxItem->GetIListCtrlBase()->GetTextRender();
    if (pTextRender)
    {
        *pSize = pTextRender->GetDesiredSize(m_pIListBoxItem->GetText());
    }

	IListCtrlBase*  pListCtrl = m_pIListBoxItem->GetIListCtrlBase();

	REGION4 r = {0};
	pListCtrl->GetItemContentPadding(&r);	
	pSize->cx += r.left + r.right;
}

void ListBoxItem::OnLButtonDown(UINT nFlags, POINT point)
{
    // ����listitembase��OnLButtonDown��������굯��ʱѡ��
	LISTCTRLSTYLE s = {0};
	s.popuplistbox = 1;
    if (m_pIListBoxItem->GetIListCtrlBase()->TestListCtrlStyle(&s)) 
        return;
    
    SetMsgHandled(FALSE);
}

void ListBoxItem::OnLButtonUp(UINT nFlags, POINT point)
{
    // popuplistctrlmousemgr�����ˣ����ﲻ���ٴ���
//     IListCtrlBase*  pListCtrl = m_pIListBoxItem->GetIListCtrlBase();
//     if (pListCtrl->TestStyleEx(LISTCTRLBASE_STYLE_POPUPLISTBOX))    // �������굯������������Ϊ��ѡ�е���
//     {
//         IListItemBase* pNewHover = pListCtrl->HitTest(point);  // ����ȡhover������Ϊ������Ƶ�listbox����ʱ������Ȼ�������һ��hover item
// 
//         int nUpdateFlag = 0;
//         if (pNewHover && pNewHover != pListCtrl->GetFirstSelectItem())
//             pListCtrl->SelectItem(pListCtrl->GetHoverItem(), false);  // ��Ϊ����Ҫ�ر��ˣ����Բ�ˢ��
//     }

    SetMsgHandled(FALSE);
}

void  ListBoxItem::OnPaint(IRenderTarget* pRenderTarget)
{
    IListCtrlBase*  pListCtrl = m_pIListBoxItem->GetIListCtrlBase();

    CRect  rcParent;
    m_pIListBoxItem->GetParentRect(&rcParent);

    UINT nRenderState = 0;
    IRenderBase* pRender = pListCtrl->GetForeRender();
    if (pRender)
    {
        nRenderState = m_pIListBoxItem->GetItemRenderState();
        pRender->DrawState(pRenderTarget, &rcParent, nRenderState);
    }

	REGION4 rItemPadding = {0};
	pListCtrl->GetItemContentPadding(&rItemPadding);

    CRect rcText = rcParent;
	rcText.DeflateRect(&rItemPadding);

    ITextRenderBase* pTextRender = pListCtrl->GetTextRender();
    if (!pTextRender)
        pTextRender = pListCtrl->CreateDefaultTextRender();
    if (pTextRender)
    {
        LPCTSTR szText = m_pIListBoxItem->GetText();
        pTextRender->DrawState(pRenderTarget, &rcText, nRenderState, szText);
    }
}


}