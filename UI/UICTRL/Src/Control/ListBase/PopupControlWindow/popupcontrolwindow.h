#pragma once
#include "..\UISDK\Inc\Interface\icustomwindow.h"
#include "..\UISDK\Inc\Interface\iuiinterface.h"

namespace UI
{

class PopupControlWindow : 
            public ICustomWindow, 
            public IPreTranslateMessage
{
public:
    PopupControlWindow();
    virtual ~PopupControlWindow();

    UI_BEGIN_MSG_MAP_Ixxx(PopupControlWindow)
        UIMSG_KILLFOCUS(OnKillFocus)
        MSG_WM_MOUSEACTIVATE(OnMouseActivate)
        MSG_WM_ACTIVATEAPP(OnActivateApp)
        MSG_WM_DESTROY(OnDestroy)
        //UIMSG_WM_SKINCHANGING(OnSkinChanging)
        UIMSG_INITIALIZE(OnInitialize)
        UIMSG_PRECREATEWINDOW(PreCreateWindow)
   UI_END_MSG_MAP_CHAIN_PARENT(ICustomWindow)

protected:
    virtual BOOL  PreTranslateMessage(MSG* pMsg);

    void  OnInitialize();
    BOOL  PreCreateWindow(CREATESTRUCT* pcs);
    void  OnSkinChanging(BOOL* pbChange);
    BOOL  OnEraseBkgnd(IRenderTarget* pRenderTarget);
    void  OnDestroy();
    int   OnMouseActivate(HWND wndTopLevel, UINT nHitTest, UINT message);
    void  OnActivateApp(BOOL bActive, DWORD dwThreadID);
    void  OnKillFocus(IObject* pNewFocusObj);

public:
    //void  Create(IObject*  pBindObj, IObject* pContentObj, LPCTSTR szId, HWND hParentWnd = NULL);
	virtual void  SetBindObject(IObject* pBindObj);
	virtual void  SetContentObject(IObject* pContentObj);
    void  Show(POINT pt, SIZE size, BOOL bDoModal, long lShowType/*=SW_SHOWNOACTIVATE*/, BOOL bDesignMode=FALSE);
    void  Hide();
    void  Destroy();
	void  SetPopupFromInfo(HWND hWnd, RECT* prcClickInWnd);

protected:
    void  destroy_popup_window();
	void  start_show_animate();

protected:
    IObject*  m_pBindObject;// ��Menu* ComboboxBase*������֪ͨ����ȡ������Դ����ҪΪ��ʵ�ֽ�popupwindow�����������combobox��������listbox
	IObject*  m_pContentObj;// ����������װ�صĶ���ָ�룬��ListBox* Menu* 

    bool      m_bExitLoop;  // ���յ�WM_EXITPOPUPLOOP֮�󣬸ñ���Ϊtrue
    bool      m_bMouseIn;   // ����ڴ����ϣ����ڴ���WM_MOUSELEAVE

	// ���ڽ���ڵ����˵�������ť���˵������غ�����ʾ����������
	HWND  m_hWndClickFrom; // ΪNULLʱ��ʾ���ж�
	CRect  m_rcClickFrom;  // ��������
};

class PopupListBoxWindow : public PopupControlWindow
{
public:
    PopupListBoxWindow();
   
    UI_BEGIN_MSG_MAP_Ixxx(PopupListBoxWindow)
        MSG_WM_KEYDOWN(OnKeyDown)
        UIMSG_INITIALIZE(OnInitialize)
    UIALT_MSG_MAP(1)
    UI_END_MSG_MAP_CHAIN_PARENT(PopupControlWindow)
 
protected:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    void  OnInitialize();
    void  OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class Menu;
class PopupMenuWindow : public PopupControlWindow
{
public:
    PopupMenuWindow();

    UI_BEGIN_MSG_MAP_Ixxx(PopupMenuWindow)
		UIMSG_PRECREATEWINDOW(PreCreateWindow)
    UI_END_MSG_MAP_CHAIN_PARENT(PopupControlWindow)

public:
	virtual void  SetContentObject(IObject* pContentObj) override;

protected:
    virtual BOOL  PreTranslateMessage(MSG* pMsg);
	BOOL  PreCreateWindow(CREATESTRUCT* pcs);

protected:
    Menu*    m_pMenu;
    POINT    m_ptLastMousePos; // �����������Ӳ˵��������ƶ������ص���ϵͳ��������WM_MOUSEMOVE��Ϣ��
    Menu*    m_pLastHoverMenu; // TODO: ͻȻ���ֵ����⣬����Ƴ��˵��ղ���WM_MOUSELEAVE��Ϣ��
                               // Ŀǰ�Ʋ�����Ϊ���յ�һ������Ƴ���WM_MOUSEMOVE��ֱ��RETURN TRUE�ˣ�����
                               // TrackMouseEventû������MOUSELEAVE�����û�и����ڷ�����Ϣ��
                               // Ϊ�˽�������⣬�������¼һ����һ������µĲ˵���������Ƴ����ֶ�����
                               // WM_MOUSELEAVE
};
}