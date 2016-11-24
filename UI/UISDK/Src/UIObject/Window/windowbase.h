#pragma once

#include "Src\UIObject\Panel\panel.h"
#include "Src\UIObject\Window\sync_window.h"
#include "Inc\Interface\iwindow.h"
#include "Src\Helper\MouseKeyboard\mousemanager.h"
#include "Src\Helper\dragdrop\windowdragdropmgr.h"
#include "Src\Util\RectArray\rectarray.h"
#include "Src\Layer\windowrender.h"

namespace UI
{
class WindowRender;
class SkinRes;
interface IRenderFont;

// 
//	�����ڴ������ڵ�ʱ�����ص�һ��������Ϣ�����������໯
//
class CREATE_WND_DATA  // ��ͬ�� ATL �е�_ATL_WIN_MODULE70
{
public:
	CREATE_WND_DATA();
	~CREATE_WND_DATA();

public:
	void  AddCreateWndData(_AtlCreateWndData* pData, void* pThis);
	void* ExtractCreateWndData();

private:
	CRITICAL_SECTION   m_cs;
	_AtlCreateWndData* m_pCreateWndList;
};

// �ⲿ��Ϣ�ص���װ��ͨ��hook�ķ�ʽʵ��
class CWindowMessageHookProxy : public IMessage
{
public:
	CWindowMessageHookProxy(IWindowDelegate* p) : IMessage(CREATE_IMPL_TRUE)
	{
		m_pCallback = p;
	}
	BOOL  OnWindowMessage(UINT msg, WPARAM w, LPARAM l, LRESULT& lResult)
	{
		lResult = 0;
		if (m_pCallback)
		{
			return m_pCallback->OnWindowMessage(msg, w, l, lResult);
		}
		return FALSE;
	}
    void  OnWindowClose(bool& bCanClose)
    {
        if (m_pCallback)
        {
            m_pCallback->OnWindowClose(bCanClose);
        }
    }
    void  OnWindowDestroy()
    {
        if (m_pCallback)
        {
            m_pCallback->OnWindowDestroy();
        }
    }
    void  OnWindowInit()
    {
        if (m_pCallback)
        {
            m_pCallback->OnWindowInitialize();
        }
    }

protected:
	virtual BOOL  virtualProcessMessage(
                UIMSG* pMsg, int nMsgMapID, bool bDoHook)
	{
		if (m_pCallback)
			return m_pCallback->OnWindowUIMessage(pMsg);
		return FALSE;
	}
private:
	IWindowDelegate*  m_pCallback;
};

class WindowBase : public Panel
{
public:
    WindowBase(IWindowBase* p);
    ~WindowBase();

    // WndProc��ԭʼ��Ϣ����   // ����virtual��չ��
    VIRTUAL_BEGIN_MSG_MAP(WindowBase) 
        MESSAGE_HANDLER( WM_ERASEBKGND,           _OnEraseBkgnd )
        MESSAGE_HANDLER( WM_PAINT,                _OnPaint )
        MESSAGE_HANDLER( WM_SETCURSOR,            _OnSetCursor )
        MESSAGE_HANDLER( WM_NCHITTEST,            _OnNcHitTest )
		MESSAGE_HANDLER( UI_MSG_POSTMESSAGE,       _OnPostMessage )
        
        MESSAGE_HANDLER( WM_MOUSEMOVE,            _OnHandleMouseMessage )
        MESSAGE_HANDLER( WM_MOUSELEAVE,           _OnHandleMouseMessage )
        MESSAGE_HANDLER( WM_LBUTTONDOWN,          _OnHandleMouseMessage )
        MESSAGE_HANDLER( WM_LBUTTONUP,            _OnHandleMouseMessage )
        MESSAGE_HANDLER( WM_RBUTTONDOWN,          _OnHandleMouseMessage )
        MESSAGE_HANDLER( WM_RBUTTONUP,            _OnHandleMouseMessage )
        MESSAGE_HANDLER( WM_LBUTTONDBLCLK,        _OnHandleMouseMessage )
        MESSAGE_HANDLER( WM_MOUSEWHEEL,           _OnHandleMouseMessage )
        MESSAGE_HANDLER( WM_CANCELMODE,           _OnHandleMouseMessage )

        MESSAGE_HANDLER( WM_CHAR,                 _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_KEYDOWN,              _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_KEYUP,                _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_SYSKEYDOWN,           _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_SYSKEYUP,             _OnHandleKeyBoardMessage )

#if(WINVER >= 0x0601)
        MESSAGE_HANDLER( /*WM_TOUCH*/0x0240,      _OnHandleTouchMessage )
        MESSAGE_HANDLER( /*WM_GESTURE*/0x0119,    _OnHandleTouchMessage )
        MESSAGE_HANDLER( /*WM_GESTURENOTIFY*/0x011A, _OnHandleTouchMessage )
#endif

        MESSAGE_HANDLER( WM_IME_STARTCOMPOSITION, _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_IME_ENDCOMPOSITION,   _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_IME_COMPOSITION,      _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_IME_SETCONTEXT,       _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_IME_NOTIFY,           _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_IME_CONTROL,          _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_IME_COMPOSITIONFULL,  _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_IME_SELECT,           _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_IME_CHAR,             _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_IME_REQUEST,          _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_IME_KEYDOWN,          _OnHandleKeyBoardMessage )
        MESSAGE_HANDLER( WM_IME_KEYUP,            _OnHandleKeyBoardMessage )
        
        MESSAGE_HANDLER( WM_GETMINMAXINFO,        _OnGetMinMaxInfo )
        MESSAGE_HANDLER(WM_SHOWWINDOW,            _OnShowWindow)
        MESSAGE_HANDLER( WM_WINDOWPOSCHANGING,    _OnWindowPosChanging )
        MESSAGE_HANDLER( UI_WM_SYNC_WINDOW,       _OnSyncWindow )
        MESSAGE_HANDLER( WM_ENTERSIZEMOVE,        _OnEnterSizeMove )
        MESSAGE_HANDLER( WM_EXITSIZEMOVE,         _OnExitSizeMove )
        MESSAGE_HANDLER( WM_SIZE,                 _OnSize )
        MESSAGE_HANDLER( WM_SETFOCUS,             _OnSetFocus )
        MESSAGE_HANDLER( WM_KILLFOCUS,            _OnKillFocus )
        MESSAGE_HANDLER( WM_MOUSEACTIVATE,        _OnMouseActive )
		MESSAGE_HANDLER( WM_GETOBJECT,            _OnGetObject )
        MESSAGE_HANDLER( WM_CREATE,               _OnCreate )
        MESSAGE_HANDLER( WM_INITDIALOG,           _OnCreate )
		MESSAGE_HANDLER( WM_CLOSE,                _OnClose)
        MESSAGE_HANDLER( WM_NCDESTROY,            _OnNcDestroy )
#if(_WIN32_WINNT >= 0x0501)
        MESSAGE_HANDLER( WM_THEMECHANGED,         _OnThemeChange )
#endif
    END_MSG_MAP()

    // UI�¼�����Ϣ����
	UI_BEGIN_MSG_MAP()
        UIMSG_ERASEBKGND( OnEraseBkgnd )
        UIMSG_DM_GETDEFID( OnGetDefId )
        UIMSG_DM_SETDEFID( OnSetDefId )
        UIMSG_GETDESIREDSIZE( OnGetDesiredSize )
        UIMSG_QUERYINTERFACE( WindowBase )
        UIMSG_SERIALIZE( OnSerialize )
        UIMSG_FINALCONSTRUCT( FinalConstruct )
        UIMSG_PRECREATEWINDOW( PreCreateWindow )
     UI_END_MSG_MAP_CHAIN_PARENT( Panel )

    IWindowBase*   GetIWindowBase() { return m_pIWindowBase; }
    WindowRender*  GetWindowRender() { return &m_oWindowRender; }

public:
	operator   HWND() const;
    HWND       GetHWND();

    Object*    GetHoverObject();
    Object*    GetPressObject();
	void  SetPressObject(Object*);

	bool  Create(LPCTSTR szID,  HWND hWndParent = NULL, RECT* prc = NULL, long lStyle = 0, long lExStyle = 0);
	void  DestroyWindow();
	long  DoModal(LPCTSTR szID, HWND hWndParent, bool canResize);
	long  DoModal(HINSTANCE hResInst, UINT nResID, LPCTSTR szID, HWND hWndParent);
	HWND  DoModeless(LPCTSTR szID, HWND hWndOnwer, bool canResize);
	HWND  DoModeless(HINSTANCE hResInst, UINT nResID, LPCTSTR szID, HWND hWndOnwer);
	void  EndDialog(INT_PTR);
	bool  Attach(HWND hWnd, LPCTSTR szID);
	void  Detach();
	void  SetWindowMessageCallback(IWindowDelegate*);

	BOOL  IsChildWindow();
	BOOL  IsWindowVisible();
	void  ShowWindow();
	void  HideWindow();
    void  SetActive(bool b);
    bool  IsActive();
    bool  IsDoModal();
    HRESULT  SetCanDrop(bool b);
    void  CenterWindow(HWND hWndCenter = NULL);
    void  CenterWindow(HMONITOR hMonitor = NULL);
    void  UpdateDesktopLayout();

	void  CalcWindowSizeByClientSize( SIZE sizeClient, SIZE* pSizeWindow );
	void  CalcClientRectByWindowRect( RECT* rcWindow, RECT* rcClient );
	void  Control_NotifyMe(const String&  idPath, int nNotifyMapID);
	void  Control_NotifyMe(Object*  pObj, int nNotifyMapID);

    WindowMouseMgr*  GetMouseMgr() { return &m_oMouseManager; }
    Object*  GetObjectUnderCursor();
    Object*  GetObjectByPos(Object* pObjParent, POINT* pt, bool bSkinBuilderInvoke=false);
    void  SetFocusObject(Object* pObj);

    // ��ȡ�������̣��Ȼ�ȡ�Լ���m_pTextRender�����û��������Լ���m_pWindow��GetRenderFont
    IRenderFont*  GetWindowDefaultRenderFont();
	void  SetDefaultRenderFont(LPCTSTR);
	LPCTSTR  GetDefaultRenderFontId();

	void  SetGpuComposite(bool b);
	bool  IsGpuComposite();
	void  DirectComposite();

	void  SaveMemBitmap(TCHAR* szFile);
	void  DrawMemBitmap(HDC hDC, RECT* prc, bool bAlphaBlend);
    void  EnableDwmTransition(bool b);
    void  SetCursorByHitTest(UINT nHitTest);

protected:
	bool  CreateUI(LPCTSTR szID);
	long  ModalLoop(HWND hWndParent);
    HMONITOR  GetPrimaryMonitor();
    
public:
	virtual bool  virtualCommitReq() { return false; }  // ��Ҫ�Ƿֲ㴰�ڵ�ʵ������ͨ���ڲ�һ��

    virtual void  virtualInnerInitWindow();
	
	virtual void  SetObjectPos( int x, int y, int cx, int cy, int nFlag=0) override;
	virtual void  virtualSetVisibleEx(VISIBILITY_TYPE eType) override;
	virtual void  virtualSetEnable(bool b) override;
	virtual bool  IsVisible() override;
	virtual bool  IsEnable() override;
	virtual IMKMgr* virtualGetIMKMgr() override;

protected:
    HRESULT    FinalConstruct(ISkinRes* p);
    BOOL       PreCreateWindow(CREATESTRUCT* pcs) { return TRUE; }
	void       OnSerialize(SERIALIZEDATA* pData);
    void       OnSetDefId(IObject* pButton);
    IObject*   OnGetDefId();

	virtual BOOL  PreTranslateMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pRet);
	
#pragma region // message handle
public:
	static LRESULT CALLBACK  StartWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	static LRESULT CALLBACK  StartDialogProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );\
protected:
	static CREATE_WND_DATA  s_create_wnd_data;    // ��������ʱ�����ص�һ��������Ϣ����HWND->this
	static LRESULT CALLBACK ThunkWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	LRESULT  StartProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool bWindowOrDialog );
	LRESULT  WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	LRESULT  DefWindowProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
    LRESULT  WndProc_GetRetValue(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled, LRESULT lRet);

protected:
	LRESULT  _OnSetCursor( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
    LRESULT  _OnNcHitTest( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnEraseBkgnd( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnPostMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT  _OnNcDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnHandleMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT  _OnHandleKeyBoardMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT  _OnHandleTouchMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT  _OnSetFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnKillFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
    LRESULT  _OnMouseActive( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnThemeChange( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnWindowPosChanging( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
    LRESULT  _OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT  _OnSyncWindow( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnGetMinMaxInfo( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnEnterSizeMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnExitSizeMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT  _OnGetObject( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	void      OnEraseBkgnd(IRenderTarget*);
    void      OnGetDesiredSize(SIZE* pSize);

private:
    void  set_hwnd(HWND);

    void  load_minwidth(long);
    long  save_minwidth();
    void  load_maxwidth(long);
    long  save_maxwidth();

    void  load_minheight(long);
    long  save_minheight();
    void  load_maxheight(long);
    long  save_maxheight();

    void  forward_on_load_notify();
	void  recursion_on_load_notify(Object* pParent);

#pragma endregion


public:
	HWND  m_hWnd;               // ���ھ��

protected:
    IWindowBase*  m_pIWindowBase;
    CWndProcThunk  m_thunk;          // ATL�е�THUNK�����ڽ�һ�����ڹ��������Լ��ĳ�Ա����
	WNDPROC  m_oldWndProc;           // �ô��ڵ�ԭʼ���ڹ���

    SyncWindow       m_syncWindow;
    WindowDragDropMgr      m_oDragDropManager;   // ��ק������
	WindowMouseMgr         m_oMouseManager;      // �����Ϣ������
    WindowRender           m_oWindowRender;      // ���ڵ���Ⱦ��������������棬windowbase��������Ⱦ

	CWindowMessageHookProxy*  m_pCallbackProxy;

	bool  m_bDoModal;                // ģ̬��������
	bool  m_bEndModal;
	INT_PTR  m_lDoModalReturn;

	WindowStyle  m_windowStyle;


	long  m_lMinWidth;               // ���ڴ�С����
	long  m_lMinHeight;
	long  m_lMaxWidth;
	long  m_lMaxHeight;
    String  m_strConfigWindowText;   // Ƥ�������õĴ��ڱ���

	bool  m_bFirsetEraseBkgnd;       // TODO: �����һ����ʾ����ʱ������˸һ�º�ɫ������.

    // ���ƣ�����ؼ�û����������ʱ��Ĭ��ȡ���ڵ����壬��m_pDefaultFont
    //
    // 1. ��xml�������˴�������ʱ��m_hFontΪxml�е����塣
    // 2. ��û����������ʱ��ʹ�ô������塣
    // 3. ����������Ҳûʱ��ʹ��UI Font Res Ĭ������
    // 4. �������ʹ��system default font.
	IRenderFont*  m_pDefaultFont;    
	
public:
	// �¼�
	signal_mc<long>  size_changed;
    signal_mc<bool&> on_close;
};



}

