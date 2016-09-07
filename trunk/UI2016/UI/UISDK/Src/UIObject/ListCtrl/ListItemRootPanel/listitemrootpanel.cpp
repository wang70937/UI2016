#include "stdafx.h"
#include "listitemrootpanel.h"
#include "Src\UIObject\ListCtrl\listctrlbase.h"
#include "Src\UIObject\ListCtrl\ListItemBase\listitembase.h"

namespace UI
{

UI_IMPLEMENT_INTERFACE(ListItemRootPanel, Panel);

IListItemBase*  IListItemRootPanel::GetListItem()
{
    ListItemBase* p = __pImpl->GetListItem();
	if (p)
		return p->GetIListItemBase();

	return NULL;
}

ListItemRootPanel::ListItemRootPanel(IListItemRootPanel* p):Panel(p)
{
    m_pIListItemRootPanel = p;

    m_pListCtrl = NULL;
    m_pListItem = NULL;

	OBJSTYLE s = { 0 };
	s.transparent = 1;
	m_pIListItemRootPanel->ModifyObjectStyle(NULL, &s);  // ��֤�ɼ��������Ӷ����ڵ���ˢ��ʱ���Ͳ��ü������ϱ�����
}


void  ListItemRootPanel::OnEraseBkgnd(IRenderTarget* pRenderTarget)
{
	UIASSERT(0);
#if 0
    if (NULL == pRenderTarget)
        return;

    if (NULL == m_pListCtrl || NULL == m_pListItem)
        return;

    // ��DCƫ�����Ƶ�listctrl���Ͻ�
    POINT ptSave = {0, 0};
    pRenderTarget->GetOrigin((int*)&ptSave.x, (int*)&ptSave.y);

    CRect  rcWindow;
    this->GetWindowRect(&rcWindow);
    RenderContext roc(*pContext);
    
    roc.m_ptOffset.x -= m_pIListItemRootPanel->GetImpl()->GetParentRectL();
    roc.m_ptOffset.y -= m_pIListItemRootPanel->GetImpl()->GetParentRectT();
    roc.Update(pRenderTarget);

    m_pListCtrl->RedrawItemByInnerCtrl(pRenderTarget, &roc, m_pListItem);
    pContext->Update(pRenderTarget);
#endif
}

void  ListItemRootPanel::SetListCtrlItem(ListCtrlBase* p1, ListItemBase* p2)
{
    m_pListCtrl = p1;
    m_pListItem = p2;
}


}