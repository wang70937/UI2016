#pragma once

// 2015/7/2
//
//  ����float��Ҫ��ʲô
//   
//  1. �б�ؼ�����float_group_head��ʽ
//
//  2. �б�ؼ���Ӧ UI_LCN_FIRSTLASTDRAWITEM_UPDATED ��Ϣ���������ʾ����
//
//  3. float list item��ӦWM_SIZE��Ϣ������float rect
//
//  4. float list item��onpaint�У���Ҫ����GetParentOrFloatRect���л���
//
// ��������
//
//  1. float item�����ǲ�͸���ġ���Ϊˢ��float itemʱ�����ô���float item�����item��ˢ��
//
//  2. Ŀǰ�ĸ�����ͷ�仯��������CalcFirstLastDrawItem�з���һ����Ϣ���ؼ���Ч����Щ����
//
//
/*
TreeView::~TreeView()
{
    if (m_pFloatGroup)
    {
        m_pFloatGroup->GetIListItemBase()->RemoveDelayRef((void**)&m_pFloatGroup);
        m_pFloatGroup = NULL;
    }
}
LRESULT  TreeView::OnFirstLastDrawItemUpdated(UINT, WPARAM, LPARAM)
{
    if (!m_pFirstDrawItem)
        return 0;

    // �߼�1��
    // 1. ����һ���ɼ�layoutΪ�ӽ��ʱ���������ĸ����
    // 2. �Լ���Ϊ����㣬���Լ����۵������ģ����� 
    // 3. �����һ�����Ҳ�����㣬�������Ҫ���ø�����ͷ��λ��
    ListItemBase* pFloatItem = NULL;

    ListItemBase* pNextVisibleItem = NULL;
    if (m_pFirstDrawItem)
    {
        pFloatItem = m_pFirstDrawItem->GetAncestorItem();
        
        if (pFloatItem->IsCollapsed())
            pFloatItem = NULL;

        if (pFloatItem)
            pNextVisibleItem = m_pFirstDrawItem->GetNextVisibleItem();
    }
    
    // �������ڵ�float item
    if (m_pFloatGroup && m_pFloatGroup != pFloatItem)
    {
        m_pFloatGroup->SetFloat(false);
        m_pFloatGroup->GetIListItemBase()->RemoveDelayRef((void**)&m_pFloatGroup);
        m_pFloatGroup = NULL;
    }

    // ����/���� float item rect
    if (pFloatItem && pFloatItem->GetItemFlag() == IOfficeSmsSelectContactTreeGroupItem::ITEMFLAG)
    {
        m_pFloatGroup = pFloatItem;
        pFloatItem->GetIListItemBase()->AddDelayRef((void**)&m_pFloatGroup);

        CRect rcItem;
        pFloatItem->GetParentRect(rcItem);
        CRect rcFloat(0,0, rcItem.Width(), rcItem.Height());

        // ��һ��Ҳ��һ���飬��ǰ���������ͷҪ�����ߣ�������ס�������
        if (pNextVisibleItem && pNextVisibleItem->GetItemFlag() == IOfficeSmsSelectContactTreeGroupItem::ITEMFLAG)
        {
            RECT rcNext;
            pNextVisibleItem->GetParentRect(&rcNext);
            ItemRect2ObjectRect(&rcNext, &rcNext);

            if (rcNext.top < rcFloat.Height())
            {
                rcFloat.OffsetRect(0, rcNext.top - rcFloat.Height());
            }
        }

        pFloatItem->SetFloatRect(&rcFloat);
    }
    
    return 0;
}
*/

namespace UI
{
class ListItemBase;
class ListCtrlBase;

struct FloatItemData
{
    ListItemBase*  pItem;

    // �����������������洢,��Ϊ����ÿ��ITEM����Ҫ.
    RECT  rcFloat;
    // ����z�򣬳����������
    long  zOrder;  
};

class FloatItemMgr
{
public:
    FloatItemMgr();
    ~FloatItemMgr();
    void  SetListCtrlBase(ListCtrlBase* p);

    void  OnRemoveItem(ListItemBase* pItem);
    void  OnRemoveAllItem();

    void  AddFloatItem(ListItemBase* p);
    void  RemoveFloatItem(ListItemBase* p);
    void  SetItemFloatRect(ListItemBase* pItem, LPCRECT prcFloat);
    bool  GetItemFloatRect(ListItemBase* pItem, RECT* prcFloat);

    void  DoPaint(IRenderTarget* pRenderTarget);
    ListItemBase*  HitTest(POINT ptWindow);

private:
    FloatItemData&  add_float_item(ListItemBase* pItem);

private:
    typedef map<ListItemBase*,  FloatItemData> _map;
    typedef map<ListItemBase*,  FloatItemData>::iterator _mapiter;
    map<ListItemBase*,  FloatItemData>  m_mapFloatItems;

    ListCtrlBase*  m_pListCtrl;
};

}