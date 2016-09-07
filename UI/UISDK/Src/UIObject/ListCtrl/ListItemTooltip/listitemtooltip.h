#pragma once

namespace UI
{
class  ListItemBase;

enum LISTITEMTOOLTIPMODE
{
    LITM_NONE,    // ������
    LITM_SINGLE,  // ������ʾ
    LITM_MULTI,   // ��������ʾ
    LITM_CUSTOM,  // �Զ��塣�Լ�����tooltip��Ϣ
};

struct ListITemToolTipInfo
{
    long  lId;
    RECT  region;

    String*  pTextRef;
};

class ListITemToolTipMgr
{
public:
    ListITemToolTipMgr();
    ~ListITemToolTipMgr();
    void  SetListItem(ListItemBase* pItem);

    BOOL  ProcessMessage(UIMSG* pMsg, int nMsgMapId, bool bDoHook);

    bool  AddToolTip(ListITemToolTipInfo* pInfo);
    bool  FindToolTip(long lId, __out_opt int* pIndex = NULL);
    bool  RemoveToolTip(long lId);
    bool  ModifyToolTip(ListITemToolTipInfo* pInfo);

    void  SetToolTip(LPCTSTR szText);
    LPCTSTR  GetToolTip();
    void  ShowSingleToolTip();
    void  SetToolTipMode(LISTITEMTOOLTIPMODE eMode);

protected:
    LRESULT  OnGetToolTipInfo(WPARAM wParam, LPARAM lParam);
    
    void  OnMouseMove(WPARAM wParam, LPARAM lParam);
    void  OnMouseLeave();

protected:
    int  HitTest(POINT ptClient);

private:
    ListItemBase*  m_pListItem;
    IUIApplication*  m_pUIApp;

    int   m_nActiveIndex;

    LISTITEMTOOLTIPMODE  m_eMode;

    // ��ģʽ
    String  m_strToolTip;

    // ����ģʽ
    typedef vector<ListITemToolTipInfo>::iterator _MyIter;
    vector<ListITemToolTipInfo>  m_vecToolTips;
};

}