#pragma once
#include "Inc/Interface/ilistctrlbase.h"

namespace UI
{
interface IListItemBase;
interface IListCtrlBase;
class ListCtrlBase;
class ListItemBase;

enum LISTCTRL_ITEM_LAYOUT_TYPE
{
    // 1. ��ͨϵͳlistbox. [�̶��и�]
    //    ��������  ����NONE������AUTO
    //    ��  �֣�  ����ռ��client width��������������
    //    AUTOSIZE: ����0(��֧��AUTO), ����total height
    LISTCTRL_ITEM_LAYOUT_TYPE_1,

    // 2. ��ͨϵͳlistbox��֧�ֺ������. [�̶��и�]
    //    ��������  ����AUTO������AUTO
    //    ��  �֣�  ����ȡmax width������client width��������������
    //    AUTOSIZE: ����max width, ����total height
    LISTCTRL_ITEM_LAYOUT_TYPE_2,
};


class  ListCtrlLayoutMgr
{
public:
    ListCtrlLayoutMgr();
    ~ListCtrlLayoutMgr();
    
    void  SetListCtrlBase(ListCtrlBase* pListCtrlBase);

    IListCtrlLayout*  GetLayout();
    void  SetLayout(IListCtrlLayout* p);

    bool  Arrange(IListItemBase* pStartToArrange, SIZE* pSizeContent);
    void  Measure(SIZE* pSize);

private:
    ListCtrlBase*  m_pListCtrl;
    IListCtrlLayout*  m_pLayout;
};


class ListCtrlItemSimpleLayout : public IListCtrlLayout
{
public:
    ListCtrlItemSimpleLayout();

    virtual void  SetIListCtrlBase(IListCtrlBase* p) { m_pIListCtrlBase = p; }
    virtual void  Arrange(IListItemBase* pStartToArrange, SIZE* pSizeContent);
    virtual void  Measure(SIZE* pSize);
    virtual void  Release() { delete this; }

protected:
    IListCtrlBase*  m_pIListCtrlBase;
};

class ListCtrlItemVariableHeightLayout : public IListCtrlLayout
{
public:
    ListCtrlItemVariableHeightLayout();

    virtual void  SetIListCtrlBase(IListCtrlBase* p) { m_pIListCtrlBase = p; }
    virtual void  Arrange(IListItemBase* pStartToArrange, SIZE* pSizeContent);
    virtual void  Measure(SIZE* pSize);
    virtual void  Release() { delete this; }

protected:
    IListCtrlBase*  m_pIListCtrlBase;
};


// TAB���֣�����ƽ���Ų�
class ListCtrlItemHorzAverageLayout : public IListCtrlLayout
{
    virtual void  SetIListCtrlBase(IListCtrlBase* p) { m_pIListCtrlBase = p; }
    virtual void  Arrange(IListItemBase* pStartToArrange, SIZE* pSizeContent);
    virtual void  Measure(SIZE* pSize){};
    virtual void  Release() { delete this; }

protected:
    IListCtrlBase*  m_pIListCtrlBase;
};


#define UI_MSGMAPID_LC_LAYOUT 142901215

// ���б�ؼ���ȡ�м��
#define UI_LCM_LAYOUT_GET_LINESPACE  142901216
// ���б�ؼ���ȡitem���
#define UI_LCM_LAYOUT_GET_ITEMSPACE  142901217

// ���б����ռ��һ��
#define UI_LISTCTRL_LAYOUT_HORZ_SINGLELINE -3
// ���б���ռ����ǰ�е�ʣ��ռ�
//#define UI_LISTCTRL_LAYOUT_HORZ_REMAINLINE -4

// �ȸߣ���ʽ���֣�һ���������� ��ֻ�������������
class ListCtrlItemFixHeightFlowLayout : public IListCtrlLayout
{
    virtual void  SetIListCtrlBase(IListCtrlBase* p) { m_pIListCtrlBase = p; }
    virtual void  Arrange(IListItemBase* pStartToArrange, SIZE* pSizeContent);
    virtual void  Measure(SIZE* pSize);
    virtual void  Release() { delete this; }

protected:
    IListCtrlBase*  m_pIListCtrlBase;
};



// ����������ͬʱ���֣������޹������������й�����
// ��listview��icon��ͼģʽ
class ListCtrlItemVariableHeightFlowLayout : public IListCtrlLayout
{
public:
    virtual void  SetIListCtrlBase(IListCtrlBase* p) { m_pIListCtrlBase = p; }
    virtual void  Arrange(IListItemBase* pStartToArrange, SIZE* pSizeContent);
    virtual void  Measure(SIZE* pSize);
    virtual void  Release() { delete this; }      
protected:
    IListCtrlBase*  m_pIListCtrlBase;

private:
    // ��ǰ�е��б���ϡ��ڻ���ʱ�����ܼ��������б����ʵ��λ�á�
    // ����ÿ��item�ĸ߶ȿ��ܲ�һ����Ҫȡ�����߶Ȳ��С�
    class CurrentLineItems
    {
    public:
        CurrentLineItems() 
        {
            m_nMaxHeight = 0; m_nContentWidth = 0; m_nContentHeight = 0;
            m_hSpace = m_vSpace = 0;
            m_nCtrlWidth = 0;
            m_nxCursor = 0;
            m_nyCursor = 0;
        }
        ~CurrentLineItems() 
        {
            UIASSERT(m_vecItems.empty());
        }
        void  AddItem(ListItemBase*, int w, int h);
        void  AddSingleLineItem(ListItemBase*, int h);
        void  CommitLine();
        void  GetContentSize(SIZE* p)
        {
            if (p) { p->cx = m_nContentWidth; p->cy = m_nContentHeight; }
        }
        void  SetHSpace(int n) { m_hSpace = n; }
        void  SetVSpace(int n) { m_vSpace = n; }
        void  SetCtrlWidth(int n) { m_nCtrlWidth = m_nContentWidth = n; }
        void  SetXYCursor(int x, int y) { m_nxCursor = x; m_nyCursor = y; }
    private:
        struct ListItemData
        {
            ListItemBase* pItem;
            int x;
            int w;
        };
        vector<ListItemData>  m_vecItems;
        int  m_nMaxHeight;
        int  m_nxCursor;  // ��ǰx��λ���α�
        int  m_nyCursor;  // ��ǰy��λ���α�

        int  m_hSpace;
        int  m_vSpace;
        int  m_nCtrlWidth;

        // �������ݵĴ�С��߶�
        int  m_nContentWidth;
        int  m_nContentHeight;
    };

};

IListCtrlLayout*  CreateListCtrlLayout(int nType, IListCtrlBase* p);
}
