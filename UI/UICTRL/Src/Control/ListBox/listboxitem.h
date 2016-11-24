#pragma once 
#include "Inc\Interface\ilistbox.h"

namespace UI
{
class ListBoxItemShareData : public MessageProxy
{
public:
    ListBoxItemShareData(IListBoxItemShareData* p);

	UI_BEGIN_MSG_MAP()
    UI_END_MSG_MAP_CHAIN_PARENT_Ixxx(ListBoxItemShareData, IListItemShareData)


public:
    IListBoxItemShareData*  m_pIListBoxItemShareData;
};

class ListBoxItem : public MessageProxy
{
public:
    ListBoxItem(IListBoxItem* p);

	UI_BEGIN_MSG_MAP()
        UIMSG_PAINT(OnPaint)
        UIMSG_GETDESIREDSIZE(GetDesiredSize)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_LBUTTONUP(OnLButtonUp)
        UIMSG_INITIALIZE(OnInitialize)
    UI_END_MSG_MAP_CHAIN_PARENT_Ixxx(ListBoxItem, IListItemBase)

public:
    void  OnInitialize();
    void  GetDesiredSize(SIZE* pSize);
    void  OnLButtonDown(UINT nFlags, POINT point);
    void  OnLButtonUp(UINT nFlags, POINT point);
    void  OnPaint(IRenderTarget* pRenderTarget);

protected:
    IListBoxItem*  m_pIListBoxItem;
    ListBoxItemShareData*  m_pShareData;
};
}