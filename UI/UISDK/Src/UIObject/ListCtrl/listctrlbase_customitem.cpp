#include "stdafx.h"
#include "listctrlbase.h"
#include "Src\Resource\skinres.h"
#include "CustomItem\custom_listitem.h"
#include "Src\UIObject\Panel\panel.h"
#include "ListItemRootPanel\listitemrootpanel.h"

CustomListItem*  ListCtrlBase::InsertCustomItem(
                    LPCTSTR  szLayoutName,
                    long lId,
                    IListItemBase* pParent, 
                    IListItemBase* pInsertAfter)
{
    if (!szLayoutName)
        return NULL;

    UIElementProxy element = m_pSkinRes->
            GetLayoutManager().FindListItemElement(szLayoutName);
    if (!element)
        return NULL;

    ICustomListItem* pItem = ICustomListItem::
            CreateInstance(m_pSkinRes->GetISkinRes());

    CustomListItem* pImpl = pItem->GetImpl();
    if (!this->InsertItem(pImpl, pParent, pInsertAfter))
    {
        SAFE_RELEASE(pItem);
        return NULL;
    }

    pImpl->SetId(lId);
    pImpl->CreateRootPanel();

	Panel* pRootPanel = pImpl->GetRootPanel();

    // �����ӿؼ�
    pRootPanel->LoadAttributeFromXml(element.get(), false);

    // �����ӿؼ�
    m_pSkinRes->GetLayoutManager().ParseChildElement(
        element.get(), pRootPanel, NOTIFY_TARGET_NULL);
 
    return pImpl;
}

CustomListItem*  ListCtrlBase::GetCustomListItem(Object* pChildControl)
{
	if (!pChildControl)
		return NULL;

	Object* pParent = NULL;
	while (pParent = pChildControl->GetParentObject())
	{
		if (pParent == static_cast<Object*>(this))
		{
			ListItemBase* pItem = 
				static_cast<ListItemRootPanel*>(pChildControl)->GetListItem();

			return static_cast<CustomListItem*>(pItem);
		}
		pChildControl = pParent;
	}

	return NULL;
}