#ifndef _UI_LISTCTRLBASE_H_
#define _UI_LISTCTRLBASE_H_
#include "icontrol.h"
#include "..\Util\struct.h"

namespace UI
{
    class ListCtrlBase;
    interface IListItemBase;
	interface ICustomListItem;
    interface IScrollBarManager;
    interface IListCtrlLayout;
    interface IListItemShareData;
    class RenderContext;
}

//////////////////////////////////////////////////////////////////////////
// ListCtrl Notify/Message
//////////////////////////////////////////////////////////////////////////

namespace UI
{
    struct  LISTCTRL_CLICKNOTIFY_DATA
    {
        IListItemBase*  pDowmItem;  // ��갴��ʱ��item���п��ܵ���ʱ���ڸ�item�ϣ������ⲿȥ�жϡ�
        WPARAM  wParam;
        LPARAM  lParam;
    };
}

//	˫��listctrl
//		message: UI_MSG_NOTIFY
//		code:    UI_LCN_DBCLICK
//		wparam:  LISTCTRL_CLICKNOTIFY_DATA*
#define UI_NM_DBCLICK   135311303

//	���listctrl
//		message: UI_MSG_NOTIFY
//		code:    UI_LCN_CLICK
//		wparam:  LISTCTRL_CLICKNOTIFY_DATA*
#define UI_NM_CLICK     135311304

//  �һ�listctrl
//		message: UI_MSG_NOTIFY
//		code:    UI_NM_RCLICK
//		wparam:  LISTCTRL_CLICKNOTIFY_DATA*
#define UI_NM_RCLICK     136291126

// ���ֵ��listctrl
//      message: UI_MSG_NOTIFY
//      code:    UI_NM_MCLICK
//      wParam: LISTCTRL_CLICKNOTIFY_DATA*
#define  UI_NM_MCLICK    158141138
 
//  ������Ϣ
//      message: UI_MSG_NOTIFY
//      code:    UI_NM_KEYDOWN
//      wParam:  vKey
//      lParam:  
//      return:  1��ʾ�Ѵ���0��ʾδ����
#define UI_NM_KEYDOWN    140081548

//	listctrl ��ǰѡ����ı�(combobox)
//		message: UI_MSG_NOTIFY
//		code:    UI_LCN_SELCHANGED
//		wparam:  IListItemBase* pOld
//		lparam:  IListItemBase* pNew
#define UI_LCN_SELCHANGED_SELF  135311306   // ���Լ������෢�͵�
#define UI_LCN_SELCHANGED  135311305  // ���Լ����͵�


//  listctrl��ǰhover��ı�\
//		message: UI_MSG_NOTIFY
//		code:    UI_LCN_HOVERITEMCHANGED
//		wparam:  IListItemBase* pOld
//		lparam:  IListItemBase* pNew    
#define UI_LCN_HOVERITEMCHANGED 135311306

// DrawItem
//      wParam: IRenderTarget*
//      lParam: IListItemBase*
#define UI_LCN_DRAWITEM 135311307

// ���һ������λ���ĸ��ɼ�item��
// HitTest
//     wParam: x (�ؼ����꣬�Ѽ�����ƫ�ƣ�������)
//     lParam: y
//     ret: IListItemBase*
//
#define UI_LCM_HITTEST  136041628

//
// ֪ͨlistctrl�����������һ��ITEM
// Message: UI_MSG_NOTIFY
// Code:    UI_LCN_REMOVEITEM
// WPARAM:  IListItemBase*
// LPARAM:  
//
#define UI_LCN_ITEMADD      136131220

// ֪ͨlistctrl����������ɾ��һ��ITEM
// Message: UI_MSG_NOTIFY
// Code:    UI_LCN_PRE_ITEMREMOVE
// WPARAM:  IListItemBase*
#define UI_LCN_PRE_ITEMREMOVE    136131221

// ֪ͨlistctrl����������ɾ��һ��ITEM
// Message: UI_MSG_NOTIFY
// Code:    UI_LCN_PRE_ITEMREMOVE
// WPARAM:  long lItemId
// LPARAM:  IListItemBase* (֪ͨ��֮�󣬸�item����ɾ��)
#define UI_LCN_ITEMREMOVE    136131632

// ֪ͨlistctrl����������ɾ������ITEM
// Message: UI_MSG_NOTIFY
// Code:    UI_LCN_REMOVEALLITEM
#define UI_LCN_PRE_ALLITEMREMOVE 136131222

// ֪ͨlistctrl����������ɾ������ITEM
// Message: UI_MSG_NOTIFY
// Code:    UI_LCN_REMOVEALLITEM
#define UI_LCN_ALLITEMREMOVE 136131223

// ����������
// wParam: IListItemBase*
#define UI_LCN_LAZYLOADDATA  156301212

// first/last draw item����(��Ҫfloat_group_head��ʽ)
// ���͸��б�������
#define UI_LCN_FIRSTLASTDRAWITEM_UPDATED  157021559


#define UITVI_ROOT    ((UI::IListItemBase*)TVI_ROOT)
#define UITVI_FIRST   ((UI::IListItemBase*)TVI_FIRST)
#define UITVI_LAST    ((UI::IListItemBase*)TVI_LAST)

namespace UI
{

typedef bool (*ListItemCompareProc)(IListItemBase* p1, IListItemBase* p2);
typedef bool (*ListItemEnumProc)(IListItemBase* pItem, WPARAM, LPARAM);   // ����falseʱֹͣenum

enum LISTITEM_VISIBLE_POS_TYPE
{
    LISTITEM_UNVISIBLE_ERROR = 0,
    LISTITEM_VISIBLE = 0x10000000,

    //--------------------------
    LISTITEM_UNVISIBLE_TOP = 0x01,      
    LISTITEM_VISIBLE_COVERTOP = 0x02,       
    LISTITEM_VISIBLE_COVERBOTTOM = 0x04,
    LISTITEM_UNVISIBLE_BOTTOM = 0x08,
    //--------------------------
    LISTITEM_UNVISIBLE_LEFT = 0x10,
    LISTITEM_VISIBLE_COVERLEFT = 0x20,
    LISTITEM_VISIBLE_COVERRIGHT = 0x40,
    LISTITEM_UNVISIBLE_RIGHT = 0x80,
};

#if 0
// ����б����һЩ��ʶ����Ҫ����������������ʱ��Ч��
enum LISTCTRL_OP_FLAG
{
	LF_NONE =             0x0000,
	// LF_REDRAW =           0x0001,    // ˢ�� ���ڣ�ͳһ�����ӳ�ˢ�£����治�ù�ˢ���߼�
	LF_LAYOUT =           0x0002,    // ����ÿ�������λ��
	LF_SORT =             0x0004,    // �������������һ��
	LF_UPDATEITEMINDEX =  0x0008,    // ��������
	LF_ALL =  (/*LF_REDRAW|*/LF_LAYOUT|LF_SORT|LF_UPDATEITEMINDEX),
};
typedef long LISTITEM_OPFLAGS;
#endif

typedef struct tagListCtrlStyle
{
	bool  sort_ascend : 1;        // ��������
	bool  sort_descend : 1;       // ��������
	bool  sort_child : 1; 
	bool  multiple_sel : 1;       // �Ƿ�֧�ֶ�ѡ
	bool  popuplistbox : 1;       // ����ʽlistboxʽ����������1.��갴��ʱ��ѡ����굯��ʱѡ�� 2. ��û��hoverʱ��ʾsel 3. ����hoverʱ��ʾhover 4. ����Ƴ��ؼ�ʱ��ȡ��hover
	bool  menu : 1;               // �˵�ʽ����������1. ��굯��ʱ���������pressitem�ϣ��򲻴���click
    bool  destroying : 1;         // ���ڱ�����(FinalRelease)
    bool  float_group_head : 1;   // ��Ҫ��ʾ������ͷ
    bool  dragwnd_if_clickblank : 1; // ������հ�����ʱ����ק����
    bool  changeselection_onlbuttonup : 1; // ��굯��ʱ���޸�ѡ��
	
}LISTCTRLSTYLE;


// ��Ϣӳ��ID���� 
// #define LISTCTRLBASE_KEYBOARDMOUSEMGR_PREMSG_ID   1   // mousemgr����ǰ�ȷ���listctrl�������Ϣӳ��ID
// #define LISTCTRLBASE_KEYBOARDMOUSEMGR_POSTMSG_ID  2   // mousemgr������ٷ���listctrl�������Ϣӳ��ID
// #define LISTCTRLBASE_INNER_CONTROL_MSG_ID  3  // �ڲ��ؼ�ID�����ڲ��ؼ�����DoNotify�ǣ����������ID��	
#define LISTCTRLBASE_EDIT_CONTROL_MSG_ID   4  // �༭�ؼ���Ϣ֪ͨID

interface UISDKAPI IListCtrlBase : public IControl
{
    UI_DECLARE_INTERFACE(ListCtrlBase);

    int   GetItemCount();
    int   GetVisibleItemCount();
    void  CalcFirstLastVisibleItem();
    void  SetCalcFirstLastVisibleItemFlag();

    bool  AddItem(IListItemBase* pItem);
    bool  InsertItem(IListItemBase* pItem, int nPos);
    bool  InsertItem(IListItemBase* pItem, IListItemBase* pInsertAfter);
    bool  InsertItem(IListItemBase* pItem, IListItemBase* pParent = UITVI_ROOT, IListItemBase* pInsertAfter = UITVI_LAST);
    void  RemoveItem(IListItemBase* pItem);
    void  RemoveItem(int nIndex);
    void  RemoveAllChildItems(IListItemBase* pParent);
    void  RemoveAllItem();
    void  DelayRemoveItem(IListItemBase* pItem);
	bool  MoveItem(IListItemBase*  p, IListItemBase* pNewParent, IListItemBase* pInsertAfter);

	void  ModifyListCtrlStyle(LISTCTRLSTYLE* add, LISTCTRLSTYLE* remove);
	bool  TestListCtrlStyle(LISTCTRLSTYLE* test);

    void  ToggleItemExpand(IListItemBase* pItem, bool bUpdate);
    void  CollapseItem(IListItemBase* pItem, bool bUpdate);
    void  ExpandItem(IListItemBase* pItem, bool bUpdate);
    void  CollapseAll(bool bUpdate);
    void  ExpandAll(bool bUpdate);

    IListItemBase*  GetHoverItem();
    IListItemBase*  GetPressItem();
    IListItemBase*  GetFocusItem();
    void  SetHoverItem(IListItemBase* pItem);
    void  SetPressItem(IListItemBase* pItem);

    IObject*  GetHoverObject();
    IObject*  GetPressObject();
    void      SetFocusObject(IObject* pObj);
    IObject*  GetFocusObject();

    IListItemBase*  GetItemByPos(int i, bool bVisibleOnly=true);
    IListItemBase*  GetItemUnderCursor();
    IListItemBase*  GetItemById(long lId);
    IListItemBase*  FindItemByText(LPCTSTR szText, IListItemBase* pStart=NULL);
    IListItemBase*  FindChildItemByText(LPCTSTR szText, IListItemBase* pParent=NULL, IListItemBase* pStart=NULL);
    IListItemBase*  GetItemByWindowPoint(POINT ptWindow);
    IListItemBase*  GetFirstItem();
    IListItemBase*  GetLastItem();
    IListItemBase*  GetFirstDrawItem();
    IListItemBase*  GetLastDrawItem();
    IListItemBase*  FindVisibleItemFrom(IListItemBase* pFindFrom=NULL);
    IListItemBase*  EnumItemByProc(ListItemEnumProc pProc, IListItemBase* pEnumFrom = NULL, WPARAM w = 0, LPARAM l = 0);
    IListItemBase*  GetFirstSelectItem();
    IListItemBase*  GetLastSelectItem();

	ICustomListItem*  InsertCustomItem(
		LPCTSTR  szLayoutName,
		long lId,
		IListItemBase* pParent = UITVI_ROOT, 
		IListItemBase* pInsertAfter = UITVI_LAST);
	ICustomListItem*  GetCustomListItem(IObject* p);


    IListItemShareData*  GetItemTypeShareData(int lType);
    void  SetItemTypeShareData(int lType, IListItemShareData* pData);
    void  RemoveItemTypeShareData(int lType);

    IScrollBarManager*  GetIScrollBarMgr();
    void  SetSortCompareProc(ListItemCompareProc p);
    void  Sort();
    void  SwapItemPos(IListItemBase*  p1, IListItemBase* p2);
    void  ItemRect2WindowRect(LPCRECT prc, LPRECT prcRet);
	void  WindowPoint2ItemPoint(IListItemBase* pItem, const POINT* pt, POINT* ptRet);
    void  ListItemDragDropEvent(UI::DROPTARGETEVENT_TYPE eEvent, IListItemBase* pItem);
    void  ListCtrlDragScroll();
    bool  IsItemRectVisibleInScreen(LPCRECT prc);

    IRenderBase*  GetFocusRender();
    void  SetFocusRender(IRenderBase* p);
    void  SetFocusItem(IListItemBase* pItem);
    void  SelectItem(IListItemBase* pItem, bool bUpdate, bool bNotify=true);
	void  ClearSelectItem(bool bNotify);
   
    void  InvalidateItem(IListItemBase* pItem);
    void  RedrawItemByInnerCtrl(IRenderTarget* pRenderTarget, IListItemBase* pItem);
    void  MakeItemVisible(IListItemBase* pItem, bool* pbNeedUpdate);

    void  UpdateListIfNeed();
    void  SetNeedLayoutItems();
    void  LayoutItem(IListItemBase* pStart, bool bRedraw);
    void  SetLayout(IListCtrlLayout* pLayout);
    IListCtrlLayout*  GetLayout();
    void  SetLayoutFixedHeight();
    void  SetLayoutVariableHeight();
    void  UpdateItemIndex(IListItemBase* pStart);
	void  EnableInnerDrag(bool b);

    short  GetVertSpacing();
    short  GetHorzSpacing();
    SIZE  GetAdaptWidthHeight(int nWidth, int nHeight);
    int   GetChildNodeIndent();
    void  SetChildNodeIndent(int n);
	void  GetItemContentPadding(REGION4*);
	void  SetItemContentPadding(REGION4*);

    void  SetMinWidth(int n);
    void  SetMinHeight(int n);
    void  SetMaxWidth(int n);
    void  SetMaxHeight(int n);
    int   GetMinWidth(); 
    int   GetMinHeight();
    int   GetMaxWidth(); 
    int   GetMaxHeight();

    int   GetItemHeight();
    void  SetItemHeight(int nHeight, bool bUpdate=true);

    void  Scroll2Y(int nY, bool bUpdate);
    void  Scroll2X(int nX, bool bUpdate);
    void  ScrollY(int nY, bool bUpdate);
    void  ScrollX(int nX, bool bUpdate);
    void  SetScrollPos(int nX, int nY, bool bUpdate);

//     static void  SetListItemRectChangedCallback(
//                     bool (__stdcall *func)(IListItemBase& item,LPCRECT prcOld,LPCRECT prcNew));
};


class ListItemShareData;
interface UISDKAPI IListItemShareData : public IMessage
{
    IListCtrlBase*  GetListCtrlBase();

    UI_DECLARE_INTERFACE(ListItemShareData);
};

// ÿ�ֲ���Ҫʵ�ֵĻ���
interface IListCtrlLayout
{
    virtual ~IListCtrlLayout() = 0 {};
    virtual void  SetIListCtrlBase(IListCtrlBase* p) = 0;
    virtual void  Arrange(IListItemBase* pStartToArrange, SIZE* pSizeContent) = 0;
    virtual void  Measure(SIZE* pSize) = 0;
    virtual void  Release() = 0;
};

}

#endif // _UI_LISTCTRLBASE_H_