#pragma once
#include "..\ListItemBase\listitembase.h"

namespace UI
{
class CustomListItem : public ListItemBase
{
public:
	CustomListItem(ICustomListItem* p);
	~CustomListItem();

	UI_BEGIN_MSG_MAP()
        UIMSG_PAINT(OnPaint)
    UI_END_MSG_MAP_CHAIN_PARENT(ListItemBase)

	ICustomListItem*  GetICustomListItem() { return m_pICustomListItem; }

public:
	IObject*  FindControl(LPCTSTR szId);

protected:
	void  OnPaint(IRenderTarget* pRenderTarget);

private:
	ICustomListItem*  m_pICustomListItem;
};

}