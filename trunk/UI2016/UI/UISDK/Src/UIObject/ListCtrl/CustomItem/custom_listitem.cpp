#include "stdafx.h"
#include "custom_listitem.h"
#include "Inc\interface\ipanel.h"
#include "..\listctrlbase.h"

CustomListItem::CustomListItem(ICustomListItem* p) : ListItemBase(p)
{
	m_pICustomListItem = p;
}
CustomListItem::~CustomListItem()
{

}

IObject*  CustomListItem::FindControl(LPCTSTR szId)
{
	if (!m_pPanelRoot)
		return NULL;

	return m_pPanelRoot->FindChildObject(szId);
}


void  CustomListItem::OnPaint(IRenderTarget* pRenderTarget)
{
	UIASSERT (m_pListCtrlBase);

	CRect  rcParent;
	this->GetParentOrFloatRect(&rcParent);

	UINT nRenderState = 0;
	IRenderBase* pRender = m_pListCtrlBase->GetForeRender();
	if (pRender)
	{
		nRenderState = m_pICustomListItem->GetItemRenderState();
		pRender->DrawState(pRenderTarget, &rcParent, nRenderState);
	}
}

