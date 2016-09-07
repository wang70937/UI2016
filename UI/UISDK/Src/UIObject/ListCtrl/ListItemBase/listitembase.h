#pragma once
#include "Inc\Interface\ilistitembase.h"
#include "Src\Base\Message\message.h"
#include "..\ListItemTooltip\listitemtooltip.h"

namespace UI
{

class RenderContext;
class ListItemRootPanel;
interface IPanel;
interface IListCtrlBase;
class ListCtrlBase;
class Panel;

//
//	�б������ݣ�����item�ɸ�����
//
class ListItemBase : public Message
{
public:
    ListItemBase(IListItemBase* p);
    virtual ~ListItemBase();

	UI_BEGIN_MSG_MAP()
        UIMSG_GETDESIREDSIZE(GetDesiredSize)
        if (m_pPanelRoot && ProcessItemMKMessage(pMsg))
        {
            return TRUE;
        }
        UICHAIN_MSG_MAP_MEMBER(m_ToolTipMgr);
        UIMSG_HANDLER_EX(UI_LISTITEM_MSG_DELAY_OP, OnDelayOp)
			UIMSG_HANDLER_EX(UI_LISTITEM_MSG_GETRENDERSTATE, OnGetRenderState)
        UIMSG_QUERYINTERFACE(ListItemBase)
        // UIMSG_WM_SETATTRIBUTE(SetAttribute)
	UI_END_MSG_MAP()

    IListItemBase*  GetIListItemBase() { return m_pIListItemBase; }
    void  SetIListCtrlBase(IListCtrlBase* p);
    IListCtrlBase*  GetIListCtrlBase();
    ListCtrlBase*  GetListCtrlBase();

public:
    ListItemBase*  GetNextItem() { return m_pNext; }
    ListItemBase*  GetPrevItem() { return m_pPrev; }
    ListItemBase*  GetParentItem() { return m_pParent; }
    ListItemBase*  GetChildItem()  { return m_pChild; }
    ListItemBase*  GetLastChildItem();
    ListItemBase*  GetAncestorItem();
    ListItemBase*  GetPrevSelection() { return m_pPrevSelection; }
    ListItemBase*  GetNextSelection() { return m_pNextSelection; }

    ListItemBase*  GetNextVisibleItem();
    ListItemBase*  GetPrevVisibleItem();
    ListItemBase*  GetNextTreeItem(); 
    ListItemBase*  GetPrevTreeItem();
    ListItemBase*  GetNextSelectableItem();
    ListItemBase*  GetPrevSelectableItem();
    ListItemBase*  GetNextFocusableItem();
    ListItemBase*  GetPrevFocusableItem();


    bool  HasChild() { return NULL != m_pChild; }
    bool  IsMyChildItem(ListItemBase* pChild, bool bTestdescendant);
    UINT  GetChildCount();
    UINT  GetDescendantCount();
    UINT  GetDescendantLeafCount();

    void  SetNextItem(ListItemBase* p) { m_pNext = p; }
    void  SetPrevItem(ListItemBase* p) { m_pPrev = p; }
    void  SetParentItem(ListItemBase* p) { m_pParent = p; }
    void  SetChildItem(ListItemBase* p)  { m_pChild = p; }
    void  SetLastChildItem(ListItemBase* p)  { m_pLastChild = p; }
    void  SetNextSelection(ListItemBase* p){ m_pNextSelection = p; }
    void  SetPrevSelection(ListItemBase* p){ m_pPrevSelection = p; }
	void  AddChild(ListItemBase* p);
	void  AddChildFront(ListItemBase* p);
    void  RemoveMeInTheTree();
    
    const CRect*  GetParentRectPtr()          { return &m_rcParent; }
    void  GetParentRect(RECT* prc)            { CopyRect(prc, &m_rcParent); }
    CRect*  GetParentRectRefPtr()             { return &m_rcParent; }   
    void  SetParentRect(LPCRECT prc);
    void  SetParentRectLightly(LPCRECT prc);
    int   GetWidth()                          { return m_rcParent.right - m_rcParent.left; }
    int   GetHeight()                         { return m_rcParent.bottom - m_rcParent.top; }

    void  SetText(LPCTSTR szText);
    LPCTSTR  GetText()                        { return m_strText.c_str(); }
    LPARAM  GetData()                         { return m_pData; } 
    void  SetData(LPARAM p)                   { m_pData = p; }
    LPARAM  GetCtrlData()                     { return m_pCtrlData; }
    void  SetCtrlData(LPARAM p)               { m_pCtrlData = p; }
    int   GetLineIndex()                      { return m_nLineIndex; }
    void  SetLineIndex(int n)                 { m_nLineIndex = n; }
    void  SetTreeIndex(int n)                 { m_nTreeIndex = n; }
    void  SetNeighbourIndex(int n)            { m_nNeighbourIndex = n; }
    int   GetTreeIndex()                      { return m_nTreeIndex; }
    int   GetNeighbourIndex()                 { return m_nNeighbourIndex; }
    int   CalcDepth();
    int   GetIndentByDepth();
    void  SetConfigWidth(int n)               { m_nConfigWidth = n; }
    void  SetConfigHeight(int n)              { m_nConfigHeight = n; }
    int   GetConfigWidth()                    { return m_nConfigWidth; }
    int   GetConfigHeight()                   { return m_nConfigHeight; }
    long  GetSort();
    void  SetSort(long);

    void  GetDesiredSize(SIZE* pSize);

    bool  IsFloat();
    void  SetFloat(bool b);
    void  SetFloatRect(LPCRECT prc);
    void  GetFloatRect(RECT* prc);
    void  GetParentOrFloatRect(RECT* prc);

    void  SetToolTip(LPCTSTR szText);
    LPCTSTR  GetToolTip();
    void  ShowSingleToolTip();

    IRenderBase*  GetIconRender() { return m_pIconRender; }
    void  SetIconRender(IRenderBase* p);
    void  SetIconRenderType(RENDER_TYPE eType);
    void  SetIconFromFile(LPCTSTR szIconPath);
    void  SetIconFromImageId(LPCTSTR szImageId);

    UINT  GetItemState(); 
    UINT  GetItemDelayOp();
    void  AddItemDelayOp(int n);
    void  RemoveDelayOp(int n);
    void  ClearDelayOp();
    void  SetLazyLoadData(bool b);

    bool  IsMySelfVisible();
    bool  IsVisible();
    bool  IsDisable();
    bool  IsHover();
    bool  IsPress();
    bool  IsFocus();
    bool  IsCollapsed();
    bool  IsExpand();
    bool  IsSelected();
    bool  IsChecked();
    bool  IsRadioChecked();
    bool  IsSelectable();
    bool  IsFocusable();
	bool  CanFocus();
    bool  IsDragDropHover();
    bool  IsDraging();
    void  ModifyStyle(LISTITEMSTYLE* pAdd, LISTITEMSTYLE* pRemove);
    bool  TestStyle(const LISTITEMSTYLE& s);

    void  SetPress(bool b, bool bNotify=true);
    void  SetHover(bool b, bool bNotify=true);
    void  SetExpand(bool b, bool bNotify=true);
    void  SetVisible(bool b, bool bNotify=true);
    void  SetFocus(bool b, bool bNotify=true);
    void  SetSelected(bool b, bool bNotify=true);
    void  SetDisable(bool b, bool bNotify=true);
    void  SetChecked(bool b, bool bNotify=true);
    void  SetRadioChecked(bool b, bool bNotify=true);
    void  SetSelectable(bool b);
    void  SetFocusable(bool b);
    void  SetDragDropHover(bool b, bool bNotify);
    void  SetDraging(bool b, bool bNotify);

    // obj item 
	IPanel*  GetIRootPanel();
    Panel*  GetRootPanel();
    bool  CreateRootPanel();

    long  GetId() { return m_lId; }
    void  SetId(long n);
    long  GetItemType() { return m_lItemType; }
    void  SetItemType(long n) { m_lItemType = n; }
    UINT  GetItemFlag() { return m_nItemFlag; }
    void  SetItemFlag(int n) { m_nItemFlag = n; }
    
    // listctrl���ã���Ҫ�Լ�ˢ��
    void  Draw(IRenderTarget* pRenderTarget);
    void  DrawItemInnerControl(IRenderTarget* pRenderTarget);
    void  DrawFloat(IRenderTarget* pRenderTarget);

    BOOL  ProcessItemMKMessage(UIMSG* pMSG);

protected:
    // void  SetAttribute(IMapAttribute* pMapAttr, bool bReload);
    void  OnSize();
    void  OnLazyLoadData();
    LRESULT  OnDelayOp(UINT, WPARAM, LPARAM);
    LRESULT  OnGetRenderState(UINT, WPARAM, LPARAM);

    // ����
protected:
    ListItemBase*  m_pPrev;
    ListItemBase*  m_pNext;
    ListItemBase*  m_pParent;
    ListItemBase*  m_pChild;
    ListItemBase*  m_pLastChild;      // ���Ǽ���������ֶΣ����ڽ��������������ӽ��ʱЧ�ʵ͵�����

    ListItemBase*  m_pNextSelection;  // ��һ����ѡ�еĶ���(�Լ��ѱ�ѡ��)
    ListItemBase*  m_pPrevSelection;  // ��һ����ѡ�еĶ���(�Լ��ѱ�ѡ��)

    // ����
protected:
    long    m_lId;
    String  m_strText;  

    // ��ʾ��
    ListITemToolTipMgr  m_ToolTipMgr;

    LISTITEMSTYLE  m_itemStyle;     // item��һЩ�������塣�����Ƿ��ѡ
    long    m_lItemType;      // һ�������п����ж������͵�item������ҵ�����
    UINT    m_nItemFlag;      // ����Ϊ�ؼ���ʶһ��item�����ͣ������� enum IxxxListItem::FLAG
    LPARAM  m_pData;          // ÿ��ITEM���Զ�������
    LPARAM  m_pCtrlData;      // ���ڿؼ�Ϊÿ��ITEM����һ���������͡���m_pData��ͬ��m_pData�������ⲿ���õ��߼����ݣ���
                              // m_pCtrlData�ǿؼ�Ϊitem���õ�һ���������ݣ�����MenuΪPopupMenuItem����һ��pSubMenu.

    long    m_lSort;          // ҵ������ֵ

    // �����������
    IRenderBase*  m_pIconRender;
    UINT    m_nItemState;       // hover/press/selected/checked/disable...
    UINT    m_nFlagsDelayOp;    // ��ʱ����
    CRect   m_rcParent;         // �����б�ؼ���client ���� 
    int     m_nLineIndex;       // ��¼����λ�ڵڼ���(�ɼ���)
    int     m_nTreeIndex;       // m_nLineIndex��ʶ�����з�����item�е�������m_nTreeIndex��ʶ�����ϵ�λ��
    int     m_nNeighbourIndex;  // m_nNeighbourIndex��ʶ��ͬһ�������е�����

    // ��� ���ϳ�������index���ǿ�ѡ�ġ������depth�������ؼ����ܾ��Ǳ���ģ�
    // ����Ҳ�ü��㡣ֻ�ṩһ��CalcDepth�����ˡ�
//    int     m_nDepth;           

    int     m_nConfigWidth;
    int     m_nConfigHeight;

    // ������������
    IListItemBase*  m_pIListItemBase;
    ListCtrlBase*   m_pListCtrlBase;
    IPanel*         m_pPanelRoot;
};
typedef ListItemBase* LPLISTITEMBASE;
}