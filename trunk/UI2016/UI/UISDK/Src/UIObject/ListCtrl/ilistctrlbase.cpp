#include "stdafx.h"
#include "Inc\Interface\ilistctrlbase.h"
#include "Src\UIObject\ListCtrl\listctrlbase.h"
#include "Inc\Interface\ilistitembase.h"
#include "Src\UIObject\ListCtrl\ListItemBase\listitembase.h"
#include "CustomItem\custom_listitem.h"

namespace UI
{

UI_IMPLEMENT_INTERFACE(ListCtrlBase, Control)
UI_IMPLEMENT_INTERFACE(ListItemShareData, Message)

bool  IListCtrlBase::AddItem(IListItemBase* pItem)              
{ 
    if (!pItem)
        return false;

    return __pImpl->AddItem(pItem->GetImpl()); 
}

bool  IListCtrlBase::InsertItem(
            IListItemBase* pItem, 
            int nPos) 
{ 
    if (NULL == pItem)
        return false;

    return __pImpl->InsertItem(pItem->GetImpl(), nPos); 
}

bool  IListCtrlBase::InsertItem(
        IListItemBase* pItem, 
        IListItemBase* pInsertAfter) 
{ 
    if (NULL == pItem)
        return false;

    ListItemBase*  pAfter = NULL;
    if (pInsertAfter)
        pAfter = pInsertAfter->GetImpl();

    return __pImpl->InsertItem(pItem->GetImpl(), pAfter); 
}

bool  IListCtrlBase::InsertItem(
            IListItemBase* pItem, 
            IListItemBase* pParent, 
            IListItemBase* pInsertAfter)
{ 
    if (NULL == pItem)
        return false;

    return __pImpl->InsertItem(pItem->GetImpl(), pParent, pInsertAfter); 
}

ICustomListItem*  IListCtrlBase::InsertCustomItem(
                    LPCTSTR  szLayoutName,
                    long lId,
                    IListItemBase* pParent, 
                    IListItemBase* pInsertAfter)
{
    CustomListItem* p = __pImpl->InsertCustomItem(
            szLayoutName, lId, pParent, pInsertAfter);
    
    if (p)
        return p->GetICustomListItem();

    return NULL;
}

ICustomListItem*  IListCtrlBase::GetCustomListItem(IObject* p)
{
	if (!p)
		return NULL;

	CustomListItem* pItem = __pImpl->GetCustomListItem(p->GetImpl());
	if (pItem)
		return pItem->GetICustomListItem();

	return NULL;
}

void  IListCtrlBase::RemoveItem(IListItemBase* pItem)             
{ 
    if (NULL == pItem)
        return ;

    __pImpl->RemoveItem(pItem->GetImpl()); 
}   
void  IListCtrlBase::RemoveAllChildItems(IListItemBase* pParent)
{
    if (NULL == pParent)
        return;

    __pImpl->RemoveAllChildItems(pParent->GetImpl());
}

void  IListCtrlBase::RemoveItem(int nIndex)            
{
    __pImpl->RemoveItem(nIndex); 
}
void  IListCtrlBase::RemoveAllItem()                      
{
    __pImpl->RemoveAllItem(); 
}
void  IListCtrlBase::DelayRemoveItem(IListItemBase* pItem)
{ 
    if (NULL == pItem)
        return;

    __pImpl->DelayRemoveItem(pItem->GetImpl());
}

bool  IListCtrlBase::MoveItem(
        IListItemBase* p,
        IListItemBase* pNewParent, 
        IListItemBase* pInsertAfter)
{
	if (!p)
		return false;

	return __pImpl->MoveItem(p->GetImpl(), pNewParent, pInsertAfter);
}

void  IListCtrlBase::ModifyListCtrlStyle(LISTCTRLSTYLE* add, LISTCTRLSTYLE* remove)
{
	__pImpl->ModifyListCtrlStyle(add, remove);
}
bool  IListCtrlBase::TestListCtrlStyle(LISTCTRLSTYLE* test)
{
	return __pImpl->TestListCtrlStyle(test);
}

int  IListCtrlBase::GetItemCount()                                        
{
    return __pImpl->GetItemCount(); 
}
int  IListCtrlBase::GetVisibleItemCount()                                 
{
    return __pImpl->GetVisibleItemCount(); 
}
void  IListCtrlBase::CalcFirstLastVisibleItem()                           
{
    __pImpl->CalcFirstLastDrawItem(); 
} 
void  IListCtrlBase::SetCalcFirstLastVisibleItemFlag()                   
{
    __pImpl->SetCalcFirstLastDrawItemFlag(); 
}


void  IListCtrlBase::ToggleItemExpand(IListItemBase* pItem, bool bUpdate) 
{ 
    if (!pItem)
        return;
    __pImpl->ToggleItemExpand(pItem->GetImpl(), bUpdate);
}
void  IListCtrlBase::CollapseItem(IListItemBase* pItem, bool bUpdate)     
{
    if (!pItem)
        return;
    __pImpl->CollapseItem(pItem->GetImpl(), bUpdate);
}
void  IListCtrlBase::ExpandItem(IListItemBase* pItem, bool bUpdate)      
{ 
    if (!pItem)
        return;
    __pImpl->ExpandItem(pItem->GetImpl(), bUpdate); 
}
void  IListCtrlBase::CollapseAll(bool bUpdate)
{
    __pImpl->CollapseAll(bUpdate);
}
void  IListCtrlBase::ExpandAll(bool bUpdate)
{
    __pImpl->ExpandAll(bUpdate);
}


IListItemBase*  IListCtrlBase::GetHoverItem()           
{ 
    ListItemBase* p = __pImpl->GetHoverItem(); 
    if (p)
        return p->GetIListItemBase();

    return NULL;
}
IListItemBase*  IListCtrlBase::GetPressItem()           
{ 
    ListItemBase* p = __pImpl->GetPressItem(); 
    if (p)
        return p->GetIListItemBase();

    return NULL;
}
void  IListCtrlBase::SetHoverItem(IListItemBase* pItem)
{
    if (pItem)
        __pImpl->SetHoverItem(pItem->GetImpl());
    else
        __pImpl->SetHoverItem(NULL);
}
void  IListCtrlBase::SetPressItem(IListItemBase* pItem)
{
    if (pItem)
        __pImpl->SetPressItem(pItem->GetImpl());
    else
        __pImpl->SetPressItem(NULL);
}
IListItemBase*  IListCtrlBase::GetFocusItem()           
{ 
    ListItemBase* p = __pImpl->GetFocusItem(); 
    if (p)
        return p->GetIListItemBase();

    return NULL;
}
void  IListCtrlBase::SetFocusItem(IListItemBase* pItem) 
{
    if (pItem)
        __pImpl->SetFocusItem(pItem->GetImpl()); 
    else
        __pImpl->SetFocusItem(NULL); 
}
IObject*  IListCtrlBase::GetHoverObject()                                
{ 
    Object* p = __pImpl->GetHoverObject();
    if (p)
        return p->GetIObject();

    return NULL;
}
IObject*  IListCtrlBase::GetPressObject()                                 
{ 
    Object* p = __pImpl->GetPressObject(); 
    if (p)
        return p->GetIObject();

    return NULL;
}
void  IListCtrlBase::SetFocusObject(IObject* pObj)
{   
    __pImpl->SetFocusObject(pObj?pObj->GetImpl():NULL);
}
IObject*  IListCtrlBase::GetFocusObject()
{
    Object* p = __pImpl->GetFocusObject(); 
    if (p)
        return p->GetIObject();

    return NULL;
}

IListItemBase*  IListCtrlBase::GetItemByWindowPoint(POINT ptWindow)
{ 
    ListItemBase* p = __pImpl->GetItemByWindowPoint(ptWindow); 
    if (p)        
        return p->GetIListItemBase();
    else
        return NULL;
}
IScrollBarManager*  IListCtrlBase::GetIScrollBarMgr()                    
{ 
    return __pImpl->GetIScrollBarMgr();
}
void  IListCtrlBase::SetSortCompareProc(ListItemCompareProc p)           
{ 
    __pImpl->SetSortCompareProc(p); 
}
void  IListCtrlBase::Sort()
{ 
    __pImpl->Sort(); 
}
void  IListCtrlBase::SwapItemPos(IListItemBase*  p1, IListItemBase* p2) 
{ 
    if (NULL == p1 || NULL == p2)
        return;

    __pImpl->SwapItemPos(p1->GetImpl(), p2->GetImpl()); 
}
void  IListCtrlBase::ItemRect2WindowRect(LPCRECT prc, LPRECT prcRet)
{ 
    __pImpl->ItemRect2WindowRect(prc, prcRet);
}
void  IListCtrlBase::WindowPoint2ItemPoint(IListItemBase* pItem, const POINT* pt, POINT* ptRet)
{
	if (!pItem)
		return;

	__pImpl->WindowPoint2ItemPoint(pItem->GetImpl(), pt, ptRet);
}

IListItemBase*  IListCtrlBase::GetItemByPos(int i, bool bVisibleOnly) 
{ 
    ListItemBase* p = __pImpl->GetItemByPos(i, bVisibleOnly); 
    if (p)
        return p->GetIListItemBase();
    return NULL;
}
IListItemBase*  IListCtrlBase::GetItemUnderCursor()
{
    ListItemBase* p = __pImpl->GetItemUnderCursor(); 
    if (p)
        return p->GetIListItemBase();
    return NULL;
}
IListItemBase*  IListCtrlBase::GetItemById(long lId)
{
    ListItemBase* p = __pImpl->GetItemById(lId); 
    if (p)
        return p->GetIListItemBase();
    return NULL;
}
IListItemBase*  IListCtrlBase::FindItemByText(LPCTSTR szText, IListItemBase* pStart)
{
    ListItemBase* pStartImpl = NULL;
    if (pStart)
        pStartImpl = pStart->GetImpl();

    ListItemBase* p = __pImpl->FindItemByText(szText, pStartImpl);
    if (p)
        return p->GetIListItemBase();
    return NULL;
}
IListItemBase*  IListCtrlBase::FindChildItemByText(LPCTSTR szText, 
                                                   IListItemBase* pParent,
                                                   IListItemBase* pStart)
{
    ListItemBase* pStartImpl = NULL;
    ListItemBase* pParentImpl = NULL;
    if (pStart)
        pStartImpl = pStart->GetImpl();
    if (pParent)
        pParentImpl = pParent->GetImpl();

    ListItemBase* p = __pImpl->FindChildItemByText(szText, pParentImpl, pStartImpl);
    if (p)
        return p->GetIListItemBase();
    return NULL;
}
IListItemBase*  IListCtrlBase::GetFirstItem()     
{ 
    ListItemBase* p = __pImpl->GetFirstItem(); 
    if (p)
        return p->GetIListItemBase();
    return NULL;
}
IListItemBase*  IListCtrlBase::GetLastItem()      
{ 
    ListItemBase* p = __pImpl->GetLastItem(); 
    if (p)
        return p->GetIListItemBase();
    return NULL;
}
IListItemBase*  IListCtrlBase::GetFirstDrawItem() 
{ 
    ListItemBase* p = __pImpl->GetFirstDrawItem(); 
    if (p)
        return p->GetIListItemBase();
    return NULL;
}
IListItemBase*  IListCtrlBase::GetLastDrawItem()  
{
    ListItemBase* p = __pImpl->GetLastDrawItem(); 
    if (p)
        return p->GetIListItemBase();
    return NULL;
}
IListItemBase*  IListCtrlBase::FindVisibleItemFrom(IListItemBase* pFindFrom) 
{ 
    ListItemBase* p = __pImpl->FindVisibleItemFrom(pFindFrom ? pFindFrom->GetImpl() : NULL); 
    if (p)
        return p->GetIListItemBase();
    return NULL;
}
IListItemBase* IListCtrlBase::EnumItemByProc(ListItemEnumProc pProc, IListItemBase* pEnumFrom, WPARAM w, LPARAM l)
{
    ListItemBase* pEnumFrom2 = NULL;
    if (pEnumFrom)
        pEnumFrom2 = pEnumFrom->GetImpl();

    ListItemBase* pRet = __pImpl->EnumItemByProc(pProc, pEnumFrom2, w, l);
    if (pRet)
        return pRet->GetIListItemBase();

    return NULL;
}

IListItemBase*  IListCtrlBase::GetFirstSelectItem() 
{ 
    ListItemBase* p = __pImpl->GetFirstSelectItem(); 
    if (p)
        return p->GetIListItemBase();
    return NULL;
}
IListItemBase*  IListCtrlBase::GetLastSelectItem()  
{ 
    ListItemBase* p = __pImpl->GetLastSelectItem(); 
    if (p)
        return p->GetIListItemBase();
    return NULL;
}

IRenderBase*  IListCtrlBase::GetFocusRender()                               
{
    return __pImpl->GetFocusRender();
}
void  IListCtrlBase::SetFocusRender(IRenderBase* p)                        
{
    __pImpl->SetFocusRender(p); 
}
void  IListCtrlBase::SelectItem(IListItemBase* pItem, bool bUpdate, bool bNotify) 
{
    __pImpl->SelectItem(pItem ? pItem->GetImpl():NULL, bUpdate, bNotify); 
}

void  IListCtrlBase::ClearSelectItem(bool bNotify)
{
	__pImpl->ClearSelectItem(bNotify);
}
void  IListCtrlBase::UpdateListIfNeed()
{
    __pImpl->UpdateListIfNeed();
}
void  IListCtrlBase::SetNeedLayoutItems()
{
    __pImpl->SetNeedLayoutItems();
}
void  IListCtrlBase::LayoutItem(IListItemBase* pStart, bool bRedraw)       
{
    __pImpl->LayoutItem(pStart?pStart->GetImpl():NULL, bRedraw); 
}
void  IListCtrlBase::UpdateItemIndex(IListItemBase* pStart)
{
    __pImpl->UpdateItemIndex(pStart?pStart->GetImpl():NULL); 
}
void  IListCtrlBase::EnableInnerDrag(bool b)
{
	__pImpl->EnableInnerDrag(b);
}
void  IListCtrlBase::SetLayout(IListCtrlLayout* pLayout)                   
{
    __pImpl->SetLayout(pLayout); 
}
IListCtrlLayout*  IListCtrlBase::GetLayout()                                
{
    return __pImpl->GetLayout(); 
}
void  IListCtrlBase::SetLayoutFixedHeight()                                    
{
    __pImpl->SetLayoutFixedHeight(); 
}
void  IListCtrlBase::SetLayoutVariableHeight()                                   
{
    __pImpl->SetLayoutVariableHeight(); 
}

void  IListCtrlBase::InvalidateItem(IListItemBase* pItem) 
{
    if (NULL == pItem)
        return;

    __pImpl->InvalidateItem(pItem->GetImpl());
}

// void  IListCtrlBase::RedrawItem(IListItemBase** ppItemArray, int nCount) 
// {
//     if (nCount < 0)
//         return;
// 
//     ListItemBase** pp = new ListItemBase*[nCount];
//     for (int i = 0; i < nCount; i++)
//     {
//         if (ppItemArray[i])
//         {
//             pp[i] = ppItemArray[i]->GetImpl();
//         }
//     }
//     __pImpl->RedrawItem(pp, nCount);
//     SAFE_ARRAY_DELETE(pp);
// }
void  IListCtrlBase::RedrawItemByInnerCtrl(IRenderTarget* pRenderTarget, 
                                           IListItemBase* pItem) 
{ 
    __pImpl->RedrawItemByInnerCtrl(pRenderTarget, pItem?pItem->GetImpl():NULL); 
}
void  IListCtrlBase::MakeItemVisible(IListItemBase* pItem, bool* pbNeedUpdate) 
{
    __pImpl->MakeItemVisible(pItem?pItem->GetImpl():NULL, pbNeedUpdate); 
}

IListItemShareData*  IListCtrlBase::GetItemTypeShareData(int lType)    
{
    return __pImpl->GetItemTypeShareData(lType); 
}
void  IListCtrlBase::SetItemTypeShareData(int lType, IListItemShareData* pData) 
{
    __pImpl->SetItemTypeShareData(lType, pData); 
}
void  IListCtrlBase::RemoveItemTypeShareData(int lType)                   
{
    __pImpl->RemoveItemTypeShareData(lType);
}

SIZE  IListCtrlBase::GetAdaptWidthHeight(int nWidth, int nHeight)           
{
    return __pImpl->GetAdaptWidthHeight(nWidth, nHeight); 
}
short  IListCtrlBase::GetVertSpacing()                                      
{
    return __pImpl->GetVertSpacing(); 
}
short  IListCtrlBase::GetHorzSpacing()                                      
{
    return __pImpl->GetHorzSpacing(); 
}

int   IListCtrlBase::GetChildNodeIndent()                                   
{
    return __pImpl->GetChildNodeIndent(); 
}
void  IListCtrlBase::SetChildNodeIndent(int n)                              
{
    __pImpl->SetChildNodeIndent(n); 
}
void  IListCtrlBase::GetItemContentPadding(REGION4* prc)
{
	__pImpl->GetItemContentPadding(prc);
}
void  IListCtrlBase::SetItemContentPadding(REGION4* prc)
{
	__pImpl->SetItemContentPadding(prc);
}
void  IListCtrlBase::SetMinWidth(int n)                                     
{
    __pImpl->SetMinWidth(n); 
}
void  IListCtrlBase::SetMinHeight(int n)                                    
{
    __pImpl->SetMinHeight(n); 
}
void  IListCtrlBase::SetMaxWidth(int n)                                     
{
    __pImpl->SetMaxWidth(n); 
}
void  IListCtrlBase::SetMaxHeight(int n)                                    
{
    __pImpl->SetMaxHeight(n); 
}
int   IListCtrlBase::GetMinWidth()                                         
{
    return __pImpl->GetMinWidth(); 
} 
int   IListCtrlBase::GetMinHeight()                                         
{
    return __pImpl->GetMinHeight(); 
}
int   IListCtrlBase::GetMaxWidth()                                         
{
    return __pImpl->GetMaxWidth(); 
}
int   IListCtrlBase::GetMaxHeight()                                        
{
    return __pImpl->GetMaxHeight(); 
}
int  IListCtrlBase::GetItemHeight()                                        
{
    return __pImpl->GetItemHeight(); 
}
void  IListCtrlBase::SetItemHeight(int nHeight, bool bUpdate)              
{
    __pImpl->SetItemHeight(nHeight, bUpdate); 
}

void  IListCtrlBase::Scroll2Y(int nY, bool bUpdate)                         
{
    __pImpl->Scroll2Y(nY, bUpdate); 
}
void  IListCtrlBase::Scroll2X(int nX, bool bUpdate)                        
{
    __pImpl->Scroll2X(nX, bUpdate); 
}
void  IListCtrlBase::ScrollY(int nY, bool bUpdate)                          
{
    __pImpl->ScrollY(nY, bUpdate); 
}
void  IListCtrlBase::ScrollX(int nX, bool bUpdate)                         
{
    __pImpl->ScrollX(nX, bUpdate); 
}
void  IListCtrlBase::SetScrollPos(int nX, int nY, bool bUpdate)            
{
    __pImpl->SetScrollPos(nX, nY, bUpdate); 
}
void  IListCtrlBase::ListItemDragDropEvent(UI::DROPTARGETEVENT_TYPE eEvent, IListItemBase* pItem)
{
    __pImpl->ListItemDragDropEvent(eEvent, pItem); 
}
void  IListCtrlBase::ListCtrlDragScroll()
{
    __pImpl->ListCtrlDragScroll();
}

bool  IListCtrlBase::IsItemRectVisibleInScreen(LPCRECT prc)
{
    return __pImpl->IsItemRectVisibleInScreen(prc);
}

// void  IListCtrlBase::SetListItemRectChangedCallback(
//         bool (__stdcall *func)(IListItemBase& item,LPCRECT prcOld,LPCRECT prcNew))
// {
//     ListCtrlAnimateCallback::SetListItemRectChangedCallback(func);
// }

IListCtrlBase*  IListItemShareData::GetListCtrlBase()                   
{
    return __pImpl->GetListCtrlBase(); 
}


}