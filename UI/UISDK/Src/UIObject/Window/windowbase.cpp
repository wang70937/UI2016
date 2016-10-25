#include "stdafx.h"
#include "windowbase.h"
#include <Uxtheme.h>

#include "Inc\Interface\ixmlwrap.h"
#include "Inc\Interface\iuires.h"
#include "Inc\Interface\imapattr.h"
#include "Src\Atl\image.h"
#include "Src\Helper\layout\layout.h"
#include "Src\Helper\topwindow\topwindowmanager.h"
#include "Src\Resource\skinres.h"
#include "Src\Util\dwm\dwmhelper.h"
#include "Src\Base\Message\message.h"
#include "Src\Base\Attribute\attribute.h"
#include "Src\Base\Attribute\long_attribute.h"
#include "Src\Base\Attribute\string_attribute.h"
#include "Src\Base\Application\uiapplication.h"
#include "Src\Util\DPI\dpihelper.h"
#include "Src\Layer\compositor.h"
#include "Inc\Interface\renderlibrary.h"
#include "Inc\Interface\iuiapplication.h"

using namespace UI;

WindowBase::WindowBase(IWindowBase* p): Panel(p),
m_oWindowRender(this),
m_oMouseManager(*this)
{
    m_pIWindowBase = p;
	this->m_hWnd = NULL;
	this->m_oldWndProc = NULL;
	this->m_pDefaultFont = NULL;

	m_bFirsetEraseBkgnd = true;
	this->m_bDoModal = false;
	this->m_bEndModal = false;
	this->m_lDoModalReturn = 0;

	m_lMinWidth = m_lMinHeight = NDEF;
	m_lMaxWidth = m_lMaxHeight = NDEF;
	memset(&m_windowStyle, 0, sizeof(m_windowStyle));


	// ȡ��Panel�����е�͸������
	OBJSTYLE s = {0};
	s.default_transparent = 1;
	ModifyObjectStyle(0, &s);

	m_pCallbackProxy = nullptr;
// 	m_bFirstTimePaintWindow = true;
// 	m_oWindowRender.SetCanCommit(false);
}

WindowBase::~WindowBase()
{
	UIASSERT( !IsWindow(m_hWnd) );          // ȷ�������ѱ�����
	UIASSERT( NULL == this->m_oldWndProc ); // ȷ���Ѿ�ȡ�����໯
	m_hWnd = NULL;

	SAFE_RELEASE(m_pDefaultFont);
	SAFE_DELETE(m_pCallbackProxy);
}

HRESULT  WindowBase::FinalConstruct(ISkinRes* p)
{
    DO_PARENT_PROCESS(IWindowBase, IPanel);

    this->m_oMouseManager.SetUIApplication(p->GetUIApplication()->GetImpl());
    this->m_oDragDropManager.SetWindowBase(this);

    m_windowStyle.hard_composite = p->GetUIApplication()->IsGpuCompositeEnable();

    return S_OK;
}

void  WindowBase::OnSerialize(SERIALIZEDATA* pData)
{
	// ������ǰ�棬���ú�Graphics Render Library
	m_oWindowRender.OnSerialize(pData);
	Panel::OnSerialize(pData);
	
	AttributeSerializer s(pData, TEXT("WindowBase"));

	s.AddLong(XML_WINDOW_MAX_WIDTH, this,
            memfun_cast<pfnLongSetter>(&WindowBase::load_maxwidth),
            memfun_cast<pfnLongGetter>(&WindowBase::save_maxwidth)
        )->SetDefault(NDEF);
	s.AddLong(XML_WINDOW_MAX_HEIGHT, this,
            memfun_cast<pfnLongSetter>(&WindowBase::load_maxheight),
            memfun_cast<pfnLongGetter>(&WindowBase::save_maxheight)
        )->SetDefault(NDEF);
	s.AddLong(XML_WINDOW_MIN_WIDHT, this,
            memfun_cast<pfnLongSetter>(&WindowBase::load_minwidth),
            memfun_cast<pfnLongGetter>(&WindowBase::save_minwidth)
        )->SetDefault(NDEF);
    s.AddLong(XML_WINDOW_MIN_HEIGHT, this,
            memfun_cast<pfnLongSetter>(&WindowBase::load_minheight),
            memfun_cast<pfnLongGetter>(&WindowBase::save_minheight)
        )->SetDefault(NDEF);

	s.AddString(XML_FONT, this, 
		    memfun_cast<pfnStringSetter>(&WindowBase::SetDefaultRenderFont),
		    memfun_cast<pfnStringGetter>(&WindowBase::GetDefaultRenderFontId))
        ->SetDefault(NULL);

    s.AddString(XML_TEXT, m_strConfigWindowText);   
}


//	[public] bool Attach( HWND hWnd )
//
//	Ŀ�ģ���һ�����еĴ��ڽ��й������Ըô��ڽ������໯����
//
//	������
//		ID
//			[in]	UI������xml�е�ID�����ڼ����ӿؼ�
//
//		hWnd
//			[in]	Ҫ���й����Ĵ��ھ��
//	
//	���أ��ɹ�����true��ʧ�ܷ���false
//
//	��ע�����øú���������ǣ���һ�����ڣ�����������CDialog��������һ��UI��window�ࣩ��
//		  ����ȴ��ʹ�����ǵ�UI�ؼ������ʱ����Ҫ�������
//
bool WindowBase::CreateUI(LPCTSTR szId)
{
	if (!m_pSkinRes)
	{
		UI_LOG_ERROR(TEXT("δ��ʼ��Ƥ��"));
		return false;
	}
	LayoutManager& layoutmanager = m_pSkinRes->GetLayoutManager();

	if (szId && _tcslen(szId)>0)   
	{
		//	�����ӿؼ�
        LPCTSTR  szName = L"";
        if  (m_pDescription)
            szName = m_pDescription->GetTagName();

		UIElementProxy  pUIElement = layoutmanager.
                FindWindowElement(szName, szId);
        if (pUIElement)
		{
			// �Լ�������
			this->LoadAttributeFromXml(pUIElement.get(), false);

			// �����Ӷ���
			layoutmanager.ParseChildElement(
                    pUIElement.get(), m_pIWindowBase);
		}
		else
		{
			UI_LOG_FATAL(_T("��ȡ���ڶ�Ӧ��xml���ʧ�ܣ�name=%s, id=%s"),
				szName, szId);

			return false;
		}

		m_strId = szId;   // ��ǰ��id��ֵ��������־���
	}

	//
	//	Ϊ�˽��xp��win7�����һ�����ԣ�ֻ���ڰ���ALT��������TAB��֮�󣬲Ż���ʾ�ؼ���focus rect
	//	�ڳ�ʼ���������������������
	//	
	// ::PostMessage(m_hWnd, WM_CHANGEUISTATE, MAKEWPARAM(UIS_CLEAR, UISF_HIDEACCEL|UISF_HIDEFOCUS), 0);
	
	// Ϊ��ʵ���б�����ؼ���theme���ƣ����򻭳�����Ч������ȷ������һ���ڱ�
	// http://msdn.microsoft.com/zh-cn/library/windows/desktop/bb759827(v=vs.85).aspx
	// http://www.codeproject.com/Articles/18858/Fully-themed-Windows-Vista-Controls
	// http://stackoverflow.com/questions/14039984/which-class-part-and-state-is-used-to-draw-selection

	// The following example code gives a list-view control the appearance of a Windows Explorer list: 
	// SetWindowTheme(m_hWnd, L"explorer", NULL);


    // �ֲ�
    
    SetRenderLayer(true);
    
	return true;
}

//
//	����һ���ؼ���֪ͨ��Ϊ�Լ�����������ؼ����¼�����ʱ�����¼�֪ͨ��
//
//	Parameter
//		idPath
//			[in]	Ҫ���õĿؼ�ID����window/panel/loginbtn����ôidPathӦΪ panel/loginbtn
//		nNotifyMapID
//			[in]	֪ͨ�¼��Ĵ���ID����ProcessMessage�ĵڶ�������
//
// void WindowBase::Control_NotifyMe( const String&  idPath, int nNotifyMapID )
// {
// 	this->Control_NotifyMe( __super::GetChildObject( idPath ), nNotifyMapID );
// }
void WindowBase::Control_NotifyMe( Object*  pObj, int nNotifyMapID )
{
	if( NULL == pObj )
	{
		UI_LOG_ERROR( _T("Window::Control_NotifyMe, ��Ч�Ĳ��� pObj ") );
		return ;
	}

	pObj->SetNotify(this->GetIMessage(), nNotifyMapID);
}

WindowBase::operator HWND() const
{
	return this->m_hWnd;
}

HWND  WindowBase::GetHWND() 
{
    return m_hWnd; 
}

void  WindowBase::PaintWindow(HDC hDC, RECT* prcInvalidArray, uint nCount)
{
#ifdef TRACE_DRAW_PROCESS
	if (prcInvalidArray)
	{
		ATL::CString strOutput(L"---------- InvalidRect:");
		for (uint i = 0; i < nCount; i++)
		{
			strOutput.AppendFormat(
				TEXT("%d,%d [%d,%d]  "),
				prcInvalidArray[i].left,
				prcInvalidArray[i].top, 
				RECTW(prcInvalidArray[i]),
				RECTH(prcInvalidArray[i]));
		}
		strOutput.Append(TEXT(" ----------"));
		UI_LOG_DEBUG(strOutput);
	}
	else
	{
		UI_LOG_DEBUG(_T("InvalidRect: NULL-----------------------"));
	}   
#endif
}

void  WindowBase::DestroyWindow()
{
	if (m_hWnd)
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
	m_strId.clear();
}

bool WindowBase::Create(LPCTSTR szId, HWND hWndParent, RECT* prc, long lStyle, long lExStyle)
{
	if (false == this->CreateUI(szId))
	{
		UI_LOG_ERROR(_T("CreateUI failed. id=%s"), szId);
		return false;
	}

	//	�������ھ��
	CREATESTRUCT cs;
	::ZeroMemory(&cs, sizeof(CREATESTRUCT));

    cs.hwndParent = hWndParent;
    if (lStyle)
        cs.style = lStyle;
    else
	    cs.style = WS_OVERLAPPEDWINDOW|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CLIPCHILDREN;

    if (lExStyle)
        cs.dwExStyle = lExStyle;
    else
	    cs.dwExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_WINDOWEDGE;

	cs.lpszClass = WND_CLASS_NAME;
	cs.lpszName  = _T("");
    if (prc)
    {
        cs.x = prc->left;
        cs.y = prc->top;
        cs.cx = prc->right - prc->left;
        cs.cy = prc->bottom - prc->top;
        m_windowStyle.setcreaterect = 1;
    }
    else
    {
	    cs.x = cs.y  = 0;
	    cs.cx = cs.cy = CW_USEDEFAULT;  //500; ���ﲻ��ֱ��дһ��ֵ���п��ܴ������õ�Ҳ�������С���������ղ���WM_SIZE��Ϣ������ʧ��
    }

    UISendMessage(this, UI_MSG_PRECREATEWINDOW, (WPARAM)&cs);

	s_create_wnd_data.AddCreateWndData(&m_thunk.cd, this);
	this->m_hWnd = ::CreateWindowEx( 
		cs.dwExStyle,
		cs.lpszClass, cs.lpszName, cs.style,
		cs.x, cs.y, cs.cx, cs.cy,
		cs.hwndParent, 0, NULL, NULL );

	if (NULL == m_hWnd)
	{
		UI_LOG_FATAL( _T("%CreateWindowExʧ��"));
		return false;
	}
	return true;
}

bool WindowBase::Attach(HWND hWnd, LPCTSTR szID)
{
	if (m_hWnd)
		return false;

	m_hWnd = hWnd;

	if (false == this->CreateUI(szID))
	{
		m_hWnd = NULL;
		UI_LOG_ERROR(_T("failed. id=%s"), m_strId.c_str());
		return false;
	}

	m_windowStyle.attach = 1;
	this->m_thunk.Init( &WindowBase::ThunkWndProc, this );
	WNDPROC pProc = this->m_thunk.GetWNDPROC();

	this->m_oldWndProc = (WNDPROC)::SetWindowLong(m_hWnd, GWLP_WNDPROC, (LONG)(LONG_PTR)pProc);

	BOOL bHandled = FALSE;
	this->_OnCreate(0,0,0,bHandled);

	return true;
}

void  WindowBase::Detach()
{
	if (NULL == m_hWnd)
		return;

	BOOL bHandled = FALSE;
	_OnNcDestroy(WM_NCDESTROY, 0, 0, bHandled);
	m_hWnd = NULL;
}

void WindowBase::EndDialog(INT_PTR nResult)
{
	UIASSERT( this->m_bDoModal );

	// ��ModalWindow::DoModal�����Ϣѭ���ܹ��˳�
	this->m_lDoModalReturn = (long)nResult ;
	this->m_bEndModal = true;
	::PostMessage( this->m_hWnd, WM_NULL, 0,0 );
}
long WindowBase::DoModal(HINSTANCE hResInst, UINT nResID, LPCTSTR szID, HWND hWndParent )
{
#if 0
	UIASSERT( NULL == m_hWnd );

	UI_AddCreateWndData(&m_thunk.cd, (void*)this);
	m_strID = ID;

	return DialogBox(hResInst, MAKEINTRESOURCE(nResID), hWndParent, (DLGPROC)WindowBase::StartDialogProc);
#endif


	HWND hWnd = this->DoModeless(hResInst, nResID, szID, hWndParent);
	if( NULL == hWnd )
	{
		return -1;
	}

	return this->ModalLoop(hWndParent);
}


//
// ����һ���յ�ģ̬�Ի���
//
long WindowBase::DoModal(LPCTSTR szID, HWND hWndParent, bool canResize)
{

#if 0
	UIASSERT( NULL == m_hWnd );

	CREATESTRUCT cs;
	cs.style        = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION;
	cs.lpszClass    = WND_CLASS_NAME;
	cs.lpszName     = _T("");
	cs.x  = cs.y    = 0;
	cs.cx = cs.cy   = 100;//CW_USEDEFAULT;
	DWORD dwStyleEx = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_WINDOWEDGE;
	this->PreCreateWindow( cs );

	HGLOBAL hgbl = GlobalAlloc(GMEM_ZEROINIT, 1024);
	if (!hgbl)
		return -1;
	LPDLGTEMPLATE lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);

	lpdt->cdit = 0;  // Number of controls
	lpdt->style = cs.style;
	lpdt->dwExtendedStyle = dwStyleEx;
	lpdt->x = cs.x;
	lpdt->y = cs.y;
	lpdt->cx = cs.cx;
	lpdt->cy = cs.cy;

	LPWORD lpw = (LPWORD)(lpdt + 1);
	*lpw++ = 0;             // No menu
	*lpw++ = 0;             // Predefined dialog box class (by default)

	LPWSTR lpwsz = (LPWSTR)lpw;
	int nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "", -1, lpwsz, 50);  // ���ڵı�������
	lpw += nchar;
	GlobalUnlock(hgbl); 

	UI_AddCreateWndData(&m_thunk.cd, this);
	m_strID = ID;

	long lRet = DialogBoxIndirect(/*m_pUIApplication->GetModuleInstance()*/g_hInstance, 
		(LPDLGTEMPLATE)hgbl, 
		hWndParent, 
		(DLGPROC)WindowBase::StartDialogProc); 

	GlobalFree(hgbl); 
	return lRet;
#endif
	HWND hWnd = this->DoModeless(szID, hWndParent, canResize);
	if( NULL == hWnd )
	{
		return -1;
	}

	return this->ModalLoop(hWndParent);
}

long WindowBase::ModalLoop(HWND hWndParent)
{
    // ���ⲿһ������ģ̬������ʾ�Ļ���
//     UIMSG  msg;
//     msg.message = UI_WM_SHOWMODALWINDOW;
//     msg.pMsgFrom = msg.pMsgTo = m_pIWindowBase;
//     LRESULT lRet = UISendMessage(&msg);
// 
//     if(msg.bHandled && lRet)
//     {
// 
//     }
//     else
    {
    	this->ShowWindow();
	    ::UpdateWindow(this->m_hWnd);
    }
	
	this->m_bDoModal = true;
	bool bEnableWindow = false;
	if (hWndParent && GetDesktopWindow() != hWndParent && IsWindowEnabled(hWndParent))
	{
		::EnableWindow( hWndParent, FALSE );
		bEnableWindow = true;
	}

#if 0
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (FALSE == m_pUIApplication->IsDialogMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (this->m_bEndModal)
		{
			this->m_bEndModal = false;
			break;
		}
	}
#else
	GetUIApplication()->MsgHandleLoop(&m_bEndModal);
	m_bEndModal = false;  // ��ԭ�������´�DoModal

#endif
	// hide the window before enabling the parent, etc.
	if (m_hWnd != NULL)
	{
		SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
	}

	if (bEnableWindow)
	{
		::EnableWindow( hWndParent, TRUE );
	}
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
	{
		::SetActiveWindow(hWndParent);  // ��������øú����������¸������ܵ���ĻZ����ĺ���ȥ��
	}
	::DestroyWindow(this->m_hWnd);    // ���ٴ���

	this->m_bDoModal = false;
	return this->m_lDoModalReturn;
}

BOOL WindowBase::PreTranslateMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pRet)
{
	return FALSE;
}


// 2014.4.15
// ע����win7 aero���棬��WS_THICKFRAME��ʽ��Dialog����10px,10px
//     Ϊ�˽�������⣬Ϊÿһ��Dialog����WS_THICKFRAME��ͬʱ�޸�WM_NCHITTEST��Ϣ��ɾ��SysMenu�е�SC_SIZE.
///    ����ⲿ�ڵ��øú���ʱ����Ҫ����һ��canResize��־
//
HWND WindowBase::DoModeless(LPCTSTR szId, HWND hWndOnwer, bool canResize)
{
	UIASSERT(NULL == m_hWnd);

	if (false == this->CreateUI(szId))
	{
		UI_LOG_ERROR(_T("CreateUI failed. id=%s"), szId);
		return false;
	}

	CREATESTRUCT cs;
	memset(&cs, 0, sizeof(CREATESTRUCT));
	cs.style        = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION | WS_THICKFRAME;
	cs.lpszClass    = WND_CLASS_NAME;
	cs.lpszName     = _T("");
	cs.x  = cs.y    = 0;
	cs.cx = cs.cy   = 0;
	cs.dwExStyle    = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_WINDOWEDGE;
    UISendMessage(this, UI_MSG_PRECREATEWINDOW, (WPARAM)&cs);

	HGLOBAL hgbl = GlobalAlloc(GMEM_ZEROINIT, 1024);
	if (!hgbl)
		return NULL;
	LPDLGTEMPLATE lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);

	lpdt->cdit = 0;  // Number of controls
	lpdt->style = cs.style;
	lpdt->dwExtendedStyle = cs.dwExStyle;
	lpdt->x = (short)cs.x;
	lpdt->y = (short)cs.y;
	lpdt->cx = (short)cs.cx;
	lpdt->cy = (short)cs.cy;

	LPWORD lpw = (LPWORD)(lpdt + 1);
	*lpw++ = 0;             // No menu
	*lpw++ = 0;             // Predefined dialog box class (by default)

	LPWSTR lpwsz = (LPWSTR)lpw;
	int nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "", -1, lpwsz, 50);  // ���ڵı�������
	lpw += nchar;
	GlobalUnlock(hgbl); 

	s_create_wnd_data.AddCreateWndData(&m_thunk.cd, this);
	{
		m_hWnd = CreateDialogIndirect(
					/*UIApplication::GetModuleInstance()*/g_hInstance, 
					(LPDLGTEMPLATE)hgbl, 
					hWndOnwer, 
					(DLGPROC)WindowBase::StartDialogProc); 
	}
	if (m_hWnd)
	{
		// fixĬ�ϵ�#32770���ڹ����ڴ�С�ı�ʱ��ˢ�µ�����
		// TODO:?? Ϊʲô��THICKFRAME�Ĵ��ھ����Լ�ˢ��
		SetClassLong(m_hWnd, GCL_STYLE, CS_HREDRAW | CS_VREDRAW |CS_DBLCLKS);

        // ɾ�����
        if (!canResize)
        {
            HMENU hMenu = GetSystemMenu(m_hWnd, FALSE);
            if (hMenu)
            {
                DeleteMenu(hMenu, SC_SIZE, 0);
            }
			m_windowStyle.dialog_noresize = 1;
        }
	}
	GlobalFree(hgbl); 
	return m_hWnd;
}
HWND WindowBase::DoModeless(HINSTANCE hResInst, UINT nResID, LPCTSTR szId, HWND hWndOnwer)
{
	UIASSERT( NULL == m_hWnd );

	if (false == this->CreateUI(szId))
	{
		UI_LOG_ERROR(_T("CreateUI failed. id=%s"), szId);
		return false;
	}

	s_create_wnd_data.AddCreateWndData(&m_thunk.cd, (void*)this);

    // ע�� hWndParent����Owner��������parent
	m_hWnd = CreateDialog(hResInst, MAKEINTRESOURCE(nResID), hWndOnwer, (DLGPROC)WindowBase::StartDialogProc);

    long lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
    if (!(lStyle & WS_CHILD))
    {
        if (! (lStyle&WS_THICKFRAME))
        {
            CRect  rcWindow;
            ::GetWindowRect(m_hWnd, &rcWindow);

            SetWindowLong(m_hWnd, GWL_STYLE, lStyle|WS_THICKFRAME);

            HMENU hMenu = GetSystemMenu(m_hWnd, FALSE);
            if (hMenu)
            {
               DeleteMenu(hMenu, SC_SIZE, 0);
            }
			m_windowStyle.dialog_noresize = 1;

            // �����޸Ĵ���Ϊ��ȷ�Ĵ�С
            SetWindowPos(m_hWnd, 0, rcWindow.left, rcWindow.top, rcWindow.Width(), rcWindow.Height(), SWP_NOZORDER|SWP_NOACTIVATE|SWP_FRAMECHANGED);
        }
    }
	return m_hWnd;
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                               ��Ϣӳ��                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WindowBase::StartWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��ȡthisָ��
	WindowBase* pThis = (WindowBase*)s_create_wnd_data.ExtractCreateWndData();
	UIASSERT(NULL != pThis);
	
	if (NULL == pThis)
		return 0;

	return pThis->StartProc(hwnd,uMsg,wParam,lParam, true);
}
//
//	[static] Dialog���ʹ��ڵĵ�һ��������Ϣ���õĴ��ڹ���
//
LRESULT CALLBACK WindowBase::StartDialogProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// ��ȡthisָ��
	WindowBase* pThis = (WindowBase*)s_create_wnd_data.ExtractCreateWndData();
	UIASSERT(NULL != pThis);

	if( NULL == pThis )
		return 0;

	return pThis->StartProc(hwnd,uMsg,wParam,lParam, false);
//	return (UINT_PTR)FALSE;
}
//
//	��StartWindowProc/StartDialogProc���ã������ڹ���ת��Ϊ������һ������
//
LRESULT WindowBase::StartProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool bWindowOrDialog )
{
	// ���໯
	this->m_hWnd = hwnd;

	this->m_thunk.Init( &WindowBase::ThunkWndProc, this );
	WNDPROC pProc = this->m_thunk.GetWNDPROC();

	if (bWindowOrDialog)
	{
		this->m_oldWndProc = ::DefWindowProc;
		::SetWindowLong( m_hWnd, GWLP_WNDPROC, (LONG)(LONG_PTR)pProc);
	}
	else
	{
		this->m_oldWndProc = NULL;
		::SetWindowLong( m_hWnd, DWLP_DLGPROC, (LONG)(LONG_PTR)pProc);  
	}

	// �����µĴ��ڹ��� ThunkWndProc
	return pProc(hwnd, uMsg, wParam, lParam);
}

//
//	[static] LRESULT CALLBACK ThunkWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//	��ATL��thunk�滻���Ĵ��ڹ���
//
//	Parameter��
//		hwnd
//			[in]	�������ڱ��滻���ˣ������hwnd��thisָ��
//
//		uMsg,wParam,lParam
//			[in]	��Ϣ��Ϣ
//
LRESULT  WindowBase::ThunkWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	WindowBase* pThis = (WindowBase*)hwnd;
	LRESULT lRet = 0;

	if (FALSE == pThis->PreTranslateMessage(uMsg, wParam, lParam, &lRet))
		lRet = pThis->WndProc( uMsg, wParam, lParam );

	return lRet;
}

LRESULT WindowBase::DefWindowProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (m_oldWndProc)
	{
		return ::CallWindowProc( m_oldWndProc, m_hWnd, uMsg, wParam, lParam );
	}
	return 0;
}

//
//	[private] LRESULT WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
//
//	���ڱ����໯��֮��Ĵ��ڹ���
//
LRESULT	WindowBase::WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	LRESULT lRes;
	UIMSG*  pOldMsg = m_pCurMsg;

	// �ⲿԤ����
	if (m_pCallbackProxy)
	{
		LRESULT lRes = 0;
		BOOL bHandled = m_pCallbackProxy->OnWindowMessage(uMsg, wParam, lParam, lRes);
		if (bHandled)
		{
			return WndProc_GetRetValue(uMsg, wParam, lParam, bHandled, lRes);
		}
	}

	// �ڲ�����
	BOOL bHandled = this->ProcessWindowMessage(m_hWnd, uMsg, wParam, lParam, lRes, 0);  // ����BEGIN_MSG_MAP��Ϣӳ���б�
	if (bHandled)
	{
		return WndProc_GetRetValue(uMsg, wParam, lParam, bHandled, lRes);
	}
    
    // ֱ�ӷ�����ǰ���ڴ���
    UIMSG  msg;
	msg.message = uMsg;
	msg.pMsgTo = this->GetIMessage();
	msg.wParam = wParam;
	msg.lParam = lParam;

	// ��������Ϣ��������ˣ�ֱ�ӷ��أ������ٵ��þɵĴ��ڹ�����
	//if (static_cast<IMessage*>(this)->ProcessMessage(&msg, 0))
    UISendMessage(&msg);
    lRes = WndProc_GetRetValue(uMsg, wParam, lParam,  msg.bHandled, msg.lRet);

	if (uMsg == WM_NCDESTROY)
	{
		// ע��Ϊʲô��������ֱ�ӵ���OnFinalMessage��ȴ��Ҫ�ټ�һ��״̬λ��
		// ��ΪWM_NCDESTROY������DestroyWindow api��������DestroyWindow api
		// ����λ���κ�һ����ǰ���ڵ���Ϣ��Ӧ�У���˵�pOldMsg==NULLʱ������ʾ
		// û����ϢǶ���ˣ��ڼ��һ��WINDOW_STYLE_DESTROYED��־���ɡ�
		m_windowStyle.destroyed = 1;
	}
	if (m_windowStyle.destroyed && pOldMsg == NULL)
	{
		m_windowStyle.destroyed = 0;
	}

	return lRes;
}

// ���öԻ����DialogProc����ֵ����MSDN�ж�DialogProc����ֵ��˵��
LRESULT  WindowBase::WndProc_GetRetValue(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled, LRESULT lRet)
{
    LRESULT lResult = 0; 

    if (NULL == m_oldWndProc)  // Dialog���ڹ���
    {
        switch(uMsg)
        {
        case WM_INITDIALOG: 
            if (GetMouseMgr()->GetFocusObject())
                lResult = FALSE;  // ��ʹ���佹������
            else
                lResult = lRet;
            break;

        case WM_CHARTOITEM:
        case WM_COMPAREITEM:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLORDLG:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORSCROLLBAR:
        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORMSGBOX:
        case WM_QUERYDRAGICON:
        case WM_VKEYTOITEM:
            // return directly���⼸����Ϣ����DialogProc����ֵ�涨��
            lResult = lRet;
            break;

        default:
            SetWindowLong(m_hWnd, DWL_MSGRESULT, lRet) ;
            lResult = bHandled ? 1:0;
            break;
        }
    }
    else
    {
        if (bHandled)
        {
            lResult = lRet;
        }
        else
        {
            lResult = m_oldWndProc/*DefWindowProc*/(m_hWnd, uMsg, wParam, lParam);
        }
    }
    return lResult;
}

LRESULT WindowBase::_OnSetCursor( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = TRUE;

	if (0 == HIWORD(lParam))  // is menu popup
	{ 
		::SetCursor( ::LoadCursor(NULL, IDC_ARROW) );
		return TRUE;
	}

	HWND hWnd = (HWND)wParam;
	if (0 != wParam && hWnd != m_hWnd)
	{
		return DefWindowProc(uMsg,wParam,lParam);  // ���λ�ڴ����ϵ�����windows�ؼ����棬������
	}  
	else if (hWnd == m_hWnd)  
	{
		// TODO: ������Ҫ����window/dialog��
        UINT nHitTest = LOWORD(lParam);
        if (m_oldWndProc)
        {
            if (nHitTest != HTCLIENT)   
            {
                SetCursorByHitTest(nHitTest);
                return TRUE;
            }
        }
        else
        {
            // �������λ��system window�ı�Եʱ��Ӧ�õ���Ĭ�ϵĴ�����̡�
            // �������Լ�����һ��ģ̬�򣬷���HTERRORʱ
		    if (nHitTest != HTCLIENT)    
		    {
			    //return DefWindowProc(uMsg,wParam,lParam);  // 2014.4.10 dialogֱ��return 0��Ȼû��Ӧ���������hittest������ꡣ���ֱ�ӵ���defwindowproc
                return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		    }
        }

//      Removed 20120824 -- ��ʵ��windowless richeditʱ�����ܲ�����ʱ����setcursor�İ취��������
//      ѡ���ı�ʱ�����ʽһֱ��˸������޸ķ���Ϊ���ȷ��ͣ�Ȼ����WM_MOUSEMOVE�м�����hover obj
//      �����˱仯�Ļ��������ٷ���һ��WM_SETCURSOR
	}
	if (TRUE == m_oMouseManager.HandleMessage(uMsg, wParam, lParam, &bHandled)) // ��������ǰhover object
	{
		return TRUE;
	}

	// �ô���ȥ���������Ϣ
	UIMSG msg;
	msg.message = uMsg;
	msg.pMsgTo = this->GetIMessage();
	msg.wParam  = wParam;
	msg.lParam  = lParam;
	if ((this->GetIMessage())->ProcessMessage(&msg, 0)) 
		return msg.lRet;

	// ����û���˴���
	::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	return TRUE;
}

void  WindowBase::SetCursorByHitTest(UINT nHitTest)
{
    static HCURSOR _arrow = ::LoadCursor(NULL, IDC_ARROW);

    switch(nHitTest)
    {
    case HTTOPLEFT:
    case HTBOTTOMRIGHT:
        {
            static HCURSOR _cursor = ::LoadCursor(NULL, IDC_SIZENWSE);
            ::SetCursor(_cursor);
        }
        break;

    case HTTOP:
    case HTBOTTOM:
        {
            static HCURSOR _cursor = ::LoadCursor(NULL, IDC_SIZENS);
            ::SetCursor(_cursor);
        }
        break;

    case HTLEFT:
    case HTRIGHT:
        {
            static HCURSOR _cursor = ::LoadCursor(NULL, IDC_SIZEWE);
            ::SetCursor(_cursor);
        }
        break;

    case HTTOPRIGHT:
    case HTBOTTOMLEFT:
        {
            static HCURSOR _cursor = ::LoadCursor(NULL, IDC_SIZENESW);
            ::SetCursor(_cursor);
        }
        break;

    case HTCAPTION:
        ::SetCursor(_arrow);
        break;

    default:
        ::SetCursor(_arrow);
        break;	
    }
}

LRESULT  WindowBase::_OnNcHitTest( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    bHandled = FALSE;
    if (!m_oldWndProc)
    {
        if (m_windowStyle.dialog_noresize)
        {
            LRESULT lr = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
            if (lr >= HTLEFT && lr <= HTBORDER)  // ��HTTOPRIGHT���������⣬�ǵ���HTBORDER��Ϊʲô
            {
                lr = HTBORDER;
                bHandled = TRUE;
                return lr;
            }
        }
    }
    return 0;
}

// ����Ϣ����WM_PAINT�зַ���
// Ϊ��ʵ������˸�Ļ��ƣ����뽫���еĻ��ƴ������һ���ط�����Ҫô��WM_ERASEBKGND��Ҫô��WM_PAINT
// ��WM_ERASEBKGND����ÿ�ζ��ᴥ���ģ��ڴ������δ���ʱ���ָ����⣩����˿��ǽ����ƴ��붼����WM_PAINT�д���
// 
// ��� lParam == 0�����ʾ��ϵͳ�Լ�����������Ϣ������ϵͳ��Ϣ�ᵼ�º�WM_PAINT DC��һ�£��Ӷ�������˸
// ��˽�WM_ERASEBKGND��Ϣ����WM_PAINT���������Լ����������ҽ�lParam��Ϊ��0
LRESULT WindowBase::_OnEraseBkgnd( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	if (0 == lParam)
	{
		bHandled = TRUE;
		if (m_bFirsetEraseBkgnd)
		{
			m_bFirsetEraseBkgnd = false;

			m_oWindowRender.OnInvalidate();
            
			// ���ʲôҲ�������ᵼ�´��ڵ�һ����ʾʱ����������ʾһ�κ�ɫ������combobox.listbox/menu
			// ���ֱ�ӵ���DefWindowProc�ᵼ�´�����ʾ��ɫ����������ʾ�Ľ��治һ�����ǰ�ɫ�ģ�Ҳ�ᵼ����˸
            // �������������һ��ȫ������
			// DefWindowProc(uMsg, wParam, lParam); 

            // Bug 2014.3.7 �ڷֲ㴰���У���һ�δ����ú���ʱ�����������:
            //   ShowWindow(SW_SHOW);
            //   SetWidowPos(x, y, cx, cy);
            // �����SetWindowPos�д�����һ��erasebkgnd������û���ߵ�windowposchanging�У�����
            // layeredwindow::commit��ʹ�þɵĴ���λ���ύ�ֲ㴰�ڣ��µĴ���λ����Ч�����ֲ㴰����
            // �������Ȼ��x,y,��һ��ˢ�²��ܸ��µ���ȷλ��)����˽�ValidateRect(NULL)ע�������������������
            // 
		}
		
		return 1;   // ����Dialog���ͣ���Ҫ����1����ֹϵͳĬ�ϻ���
	}
	return 0;
}

LRESULT WindowBase::_OnPostMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	if (wParam == MSG_INVALIDATE)
	{
		bHandled = TRUE;
		
		m_oWindowRender.OnInvalidate();
	}
	return 0;
}

//
// [ע]�ֲ㴰�ڵ�InvalidateRect���õ���ps.rcPaint��Զ�ǿյ�
//
LRESULT WindowBase::_OnPaint( UINT uMsg, WPARAM wParam,LPARAM lParam, BOOL& bHandled )
{
	PAINTSTRUCT ps;
	HDC  hDC = NULL;
	RECT rcInvalid = {0};

	if (NULL == wParam)
	{
		hDC = ::BeginPaint(this->m_hWnd ,&ps);
		rcInvalid = ps.rcPaint;

		if (IsRectEmpty(&rcInvalid) && 
			GetWindowLong(m_hWnd, GWL_EXSTYLE)&WS_EX_LAYERED)
		{
			GetClientRect(m_hWnd, &rcInvalid);
		}
	}
	else
	{
		GetClipBox(hDC, &rcInvalid);
        if (IsRectEmpty(&rcInvalid))
        {
            GetClientRect(m_hWnd, &rcInvalid);
        }
	}

	if (!IsRectEmpty(&rcInvalid))
	{
// 		if (m_bFirstTimePaintWindow)
// 		{
// 			m_bFirstTimePaintWindow = false;
// 			m_oWindowRender.SetCanCommit(true);
// 		}

        // 2016.3.12 ����WM_PAINTһ�㶼�ɲ���ϵͳ�����������͸�������µ�ˢ�¡�
        // UISDK����������WM_PAINT�������������ӦWM_PAINTʱ��ֱ���ύ���棬��
        // ��ˢ��
        m_oWindowRender.OnPaint(hDC, &rcInvalid);
	}
	
	if(NULL == wParam)
	{
		EndPaint(m_hWnd,&ps);
	}
	
	return 1;  //  ��_OnPaint�з���0���ᵼ��dialog���͵Ĵ��ڣ����������ڸ��Ǻ��Ƴ���ˢ���쳣!!!
}

LRESULT WindowBase::_OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	if (SIZE_MINIMIZED == wParam)
        return 0;
	
	RECT rcWindow;
	::GetWindowRect(m_hWnd, &rcWindow);
	::GetClientRect(m_hWnd, &m_rcParent);

	if (GetConfigWidth() > 0)
		SetConfigWidth(rcWindow.right-rcWindow.left);
	if (GetConfigHeight() > 0)
		SetConfigHeight(rcWindow.bottom-rcWindow.top);

	bHandled = TRUE;
	notify_WM_SIZE(wParam, LOWORD(lParam), HIWORD(lParam));

    m_oWindowRender.OnWindowSize(LOWORD(lParam), HIWORD(lParam));

	// ע�����ﲻҪ����InvalidateRect������CustomWindow��������ʱ����ǰ���ڵ�RGN�����ϵ����ã�
	//     ���½��յ�WM_PAINTʱ��ps.rcPaintҲ������RGN���ã����ˢ��������������
	//     Ҳ���½�������CustomWindow::UpdateWindowRgn�õ��Ĵ��ڱ������������������
	//     �������
	// ������ﻹ��ֱ�ӵ����˴��ڵ�UpdateObject
	//::InvalidateRect(m_hWnd, NULL, FALSE);
	
	if (m_oWindowRender.CanCommit())
	{
		// ���!cancommit���п����Ǵ��ڸմ���ʱ�ĳ�ʼ����ֱ����WM_PAINT��Ϣ
		// Ȼ����windowrender���cancommit����

		this->Invalidate();
		ValidateRect(m_hWnd, NULL);
	}
	else
	{
		InvalidateRect(m_hWnd, NULL, TRUE);
	}
	
	return 0;
}

LRESULT WindowBase::_OnCreate(
        UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

    m_oWindowRender.BindHWND(m_hWnd);

    // ���⴦��
    // ���ڽ�D2D RenderTarget�󶨵���������.D2D��GDI/GDIPlus
    // ��һ����������Ҫ��Ūһ��˫���壬ֱ��begindraw/enddraw����
// 	if (m_pRenderLayer)
// 	{
// 		IRenderTarget*  pRT = m_pRenderLayer->GetRenderTarget();
// 		if (pRT)
// 		{
// 			pRT->BindHWND(m_hWnd);
// 		}
// 	}

	//
	//  �п���m_strIDΪ�գ���������Դ��������ʱ��popupconotrolwindow��
	//	���û�н�AddTopWindowObject��OnInitWindow����CreateUI��ִ��
	//
	// if (!IsChildWindow())--�Ӵ���Ҳ��һ��UI���ڣ�Ҳά������
	{
		TopWindowManager* pTopWndMgr = 
            GetUIApplication()->GetTopWindowMgr();
		if (pTopWndMgr)
			pTopWndMgr->AddTopWindowObject(this);
	}

	// ����
	if (m_windowStyle.attach)  // attach�Ĵ���ֱ��ʹ���ⲿ�Ĵ�С
	{
		::GetClientRect(m_hWnd, &m_rcParent);

        // �����ʱ����GetDesiredSize��ȥ�������ڴ�С�ˣ�
        // ���´��ڱ��޸�Ϊ����Ӧ��С 
		CRect rcWindow;               
		::GetWindowRect(m_hWnd, &rcWindow);
		SetConfigWidth(rcWindow.Width());
		SetConfigHeight(rcWindow.Height());

        // ��ΪAttach���Ĵ��ڳ�ʼ��ʱ�Ѿ��ղ���WM_SIZE�ˣ�
        // ����Լ��ٷ�һ�Σ�
        // ֪ͨ����RenderTarget����������һЩˢ�½�ʧ��
		notify_WM_SIZE(0, m_rcParent.Width(), m_rcParent.Height());
        this->UpdateLayout();
	}
    else
    {
        if (m_windowStyle.setcreaterect)
        {
            // �����ʱ����GetDesiredSize��ȥ�������ڴ�С�ˣ�
            // ���´��ڱ��޸�Ϊ����Ӧ��С 
            CRect rcWindow;    
            ::GetWindowRect(m_hWnd, &rcWindow);
            SetConfigWidth(rcWindow.Width());
            SetConfigHeight(rcWindow.Height());

            ::GetClientRect(m_hWnd, &m_rcParent);
            this->UpdateLayout();
        }
        else
        {
            // ���ܷ��� OnInitialize ���档
            // ��Ϊ�п���OnInitialize���Ѿ����ù� SetWindowPos
            DesktopLayout dl;  
            dl.Arrange(this);
        }
    }

    if (!m_strConfigWindowText.empty())
        ::SetWindowText(m_hWnd, m_strConfigWindowText.c_str());

    // ����Ĭ������
    if (!m_pDefaultFont)
        SetDefaultRenderFont(L"");

    // ������һ����ʼ���Ļ��� (virtual)��
    // �����������/��ԭ��ť��״̬
    this->virtualInnerInitWindow();

    // ��ֹ��ʵ����ʾ����ʱ������ʾ��һЩ��ʼ����ˢ�µ����ݡ�
    // ע������ֻ����һ��layer
    m_oWindowRender.SetCanCommit(false); 
    {
        UISendMessage(m_pIMessage, UI_MSG_INITIALIZE);

        UIMSG msg;
        UIMSG msg2;
        msg.message = UI_MSG_INITIALIZE;
		msg2.message = UI_MSG_INITIALIZE2;
        ForwardMessageToChildObject2(this, &msg, &msg2);

		UISendMessage(m_pIMessage, UI_MSG_INITIALIZE2);
    }
	m_oWindowRender.SetCanCommit(true);

	// ����Ĭ�϶���
	m_oMouseManager.SetDefaultObject(
            m_oMouseManager.GetOriginDefaultObject(),
            false);

    if (m_windowStyle.attach) // ��������ˢ��
    {
        Invalidate();
    }
	return 0;
}

// �ݹ�֪ͨ�Ӷ���������
void  WindowBase::recursion_on_load_notify(Object* pParent)
{
    pParent->virtualOnLoad();

    Object* pChild = pParent->GetChildObject();
    while (pChild)
    {
        recursion_on_load_notify(pChild);
        pChild = pChild->GetNextObject();
    }
    pChild = pParent->GetNcChildObject();
    while (pChild)
    {
        recursion_on_load_notify(pChild);
        pChild = pChild->GetNextObject();
    }
}

void  WindowBase::virtualInnerInitWindow()
{
    // ���ڼ��ؼ�����ɣ����ڲ���������
    recursion_on_load_notify(this);
}

LRESULT WindowBase::_OnNcDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	SyncWindowHelper<WindowBase>::_OnNcDestroy();

//	if (!IsChildWindow())
	{
		TopWindowManager* pTopWndMgr = GetUIApplication()->GetTopWindowMgr();
		if (pTopWndMgr)
			pTopWndMgr->RemoveTopWindowObject(this);
	}

    this->m_oDragDropManager.OnWindowDestroy();
	m_oMouseManager.HandleMessage( uMsg, wParam, lParam, &bHandled);

	if (m_oldWndProc)
	{
		::SetWindowLong( m_hWnd, GWLP_WNDPROC, (LONG)(LONG_PTR)m_oldWndProc);
		m_oldWndProc = NULL;
	}
	m_hWnd = NULL;
	
	this->DestroyChildObject();   // ɾ���Ӷ���
	return 0;
}

LRESULT WindowBase::_OnHandleMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_bSizeMove)  // ��ק�����в�����
	{
		bHandled = FALSE;
		return 0;
	}

	bHandled = FALSE;
	LRESULT lRet = m_oMouseManager.HandleMouseMessage(uMsg, wParam, lParam, &bHandled);

    if (bHandled)
        return lRet;

	if (m_oMouseManager.GetHoverObject() || NULL != m_oMouseManager.GetPressObject())
	{
		bHandled = TRUE;
	}
	return 0;
}
LRESULT  WindowBase::_OnHandleKeyBoardMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    return m_oMouseManager.HandleKeyboardMessage( uMsg, wParam, lParam, &bHandled );
}

LRESULT  WindowBase::_OnHandleTouchMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    return m_oMouseManager.HandleTouchMessage( uMsg, wParam, lParam, bHandled);
}


LRESULT WindowBase::_OnSetFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = TRUE;


	m_oMouseManager.HandleMessage( uMsg, wParam, lParam, &bHandled);

	UIMSG msg;
	msg.message = uMsg;
	msg.pMsgTo = this->GetIMessage();
	msg.wParam  = wParam;
	msg.lParam  = lParam;
    UISendMessage(&msg);

	return 1;    // ע������DialogĬ�ϵ�WM_SETFOCUS�Ὣ���������õ��Լ��ĵ�һ���ؼ����棬������������return 0����bHandled = FALSE
	             //     ������::SetFocus(m_hWnd)�Ľ��㵽����������ֱ������Լ����õ��ؼ����棬����SetFocus�ﲻ�����ǵı���
}
LRESULT WindowBase::_OnKillFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = TRUE;

	m_oMouseManager.HandleMessage( uMsg, wParam, lParam, &bHandled);

	UIMSG msg;
	msg.message = uMsg;
	msg.pMsgTo = this->GetIMessage();
	msg.wParam = wParam;
	msg.lParam = lParam;
//	this->ProcessMessage( &msg );
    UISendMessage(&msg);

	return 1 ;
}

// ���ڽ�����WindowBase��һ���Ӵ��ڣ�����һ����ͨ���㴰���ϣ���ô���WindowBase
// ���޷��õ����㡣�����������ӦMouseActive��Ϣ�����Լ�SetFocus
// ������ϵͳ��ť�ؼ�����δ���ĵ���õ�����Ͳ��ö�֪�ˣ���WS_TABSTOP����ԭ���Թ��ˣ�û�á�
LRESULT WindowBase::_OnMouseActive( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    bHandled = FALSE;
    if (IsChildWindow() && ::GetFocus() != m_hWnd)
    {
        ::SetFocus(m_hWnd);
    }
    return 0;
}

LRESULT WindowBase::_OnThemeChange( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	
    UIMSG  msg;
    msg.message = uMsg;
    msg.wParam = wParam;
    msg.lParam = lParam;

	Object::ForwardMessageToChildObject(this, &msg);
	return 0;
}

HMONITOR  WindowBase::GetPrimaryMonitor()
{
    POINT pt = {0,0};
    return MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY); 
}


// minmax.c xxxInitSendValidateMinMaxInfo
//
// 1. �����󻯵�������ʱ��ptMaxSize, ptMaxPositionָ���������Ĵ�С��
//    For systems with multiple monitors, the ptMaxSize and ptMaxPosition members
//    describe the maximized size and position of the window on the primary monitor, 
//    even if the window ultimately maximizes onto a secondary monitor. 

// 2. ϵͳ�ڲ����ж����pinfo��maxsize�����������ʱ������Ӧ�ĵ���Ϊ�����Ĵ�С��
//    In that case, the window manager adjusts these values to compensate for differences
//    between the primary monitor and the monitor that displays the window. Thus, if 
//    the user leaves ptMaxSize untouched, a window on a monitor larger than the primary
//    monitor maximizes to the size of the larger monitor.
//
// 3. ����!!!����һ������ -- ���Ƿ��б߿��޹ء�
//
//     . �б߿�Ĵ��ڻ�����������border������ptMaxSize����ͱ������ֱ��ʴ�6~8px,�����׳���������⡣
//     . �ޱ߿�Ĵ�����󻯻���ָ������������ȫ�������⣬�����Ҫ��ptMaxSize�������������׳�����������
//       (���Է��֣�ֻ��ͬʱ�߱���WS_CAPTION|WS_MAXIMIZEBOX���ԵĴ�����󻯺󣬲Ų��Ḳ����������
//  
//    ������Ǹ�����ptMaxSize����ptMaxPosition�޹أ�
//    С�������ֱ��ʴ�Сʱ����󻯵������Ĵ��ڲ�������Ӧ,��Ȼ�ǲ����е�ptMaxSize��
//    �����������Ļ����������󻯲���ȫ�����⡣
//
// 4. ptMaxPosition��Windows�ڲ��ǻ���������workarea����������λ�����ʱ,ptMaxPostion.x=1���Զ�������
//    ���������Ҳ�1px��
//
// 5. ptMaxSize��Windows�ڲ�����������monitorarea�Ƚϵġ���ptMaxSize��С������Сʱ��Windows���Զ�
//    �����ڵ���Ϊworkarea��С�������ptMaxSizeС��������Сʱ��Windows��������
//
LRESULT WindowBase::_OnGetMinMaxInfo( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    // ����WindowBase��ֻ�Դ�����Сֵ�����ֵ�Դ���
    // �����߼���CustomWindowȥ����

	bHandled = TRUE;
	MINMAXINFO* pInfo = (MINMAXINFO*)lParam;

	if (NDEF != m_lMaxWidth)
		pInfo->ptMaxSize.x = pInfo->ptMaxTrackSize.x = m_lMaxWidth;

	if (NDEF != m_lMaxHeight)
		pInfo->ptMaxSize.y = pInfo->ptMaxTrackSize.y = m_lMaxHeight;

	if (NDEF != m_lMinWidth)
		pInfo->ptMinTrackSize.x = m_lMinWidth;

	if (NDEF != m_lMinHeight)
		pInfo->ptMinTrackSize.y = m_lMinHeight;

	return 0;
}

LRESULT WindowBase::_OnWindowPosChanging( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	SyncWindowHelper<WindowBase>::_OnWindowPosChanging((LPWINDOWPOS)lParam, bHandled);
	return 0;
}
LRESULT WindowBase::_OnSyncWindow( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	switch(wParam)
	{
	case ADD_SYNC_WINDOW:
		{
			const SyncWindowData& data = *(SyncWindowData*)(lParam);
			this->AddAnchorItem(data);
		}
		break;
	case MODIFY_SYNC_WINDOW:
		{
			const SyncWindowData& data = *(SyncWindowData*)(lParam);
			this->ModifyAnchorItem(data);
		}
		break;
	case REMOVE_SYNC_WINDOW:
		{
			const SyncWindowData& data = *(SyncWindowData*)(lParam);
			this->RemoveAnchorItem(data.m_hWnd);
		}
		break;
	case ADD_SYNC_WINDOW_RESULT:
		{
			this->OnAddAnchorItem((HWND)lParam);
		}
		break;
	case MODIFY_SYNC_WINDOW_RESULT:
		{
			this->OnModifyAnchorItem((HWND)lParam);
		}
		break;
	case REMOVE_SYNC_WINDOW_RESULT:
		{
			this->OnRemoveAnchorItem((HWND)lParam);
		}
		break;
	case HOST_WINDOW_DESTROYED:
		{
			this->OnRemoveAnchorItem((HWND)lParam);
		}
		break;
	case HOST_WINDOW_POSCHANGING:
		{
			this->OnHostWindowPosChanging();
		}
		break;
	}
	return 0;
}
LRESULT WindowBase::_OnEnterSizeMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	SyncWindowHelper<WindowBase>::_OnEnterSizeMove();

	LRESULT lRet = m_oMouseManager.HandleMouseMessage(uMsg, wParam, lParam, &bHandled);
	if (bHandled)
		return lRet;

	return 0;
}
LRESULT WindowBase::_OnExitSizeMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	SyncWindowHelper<WindowBase>::_OnExitSizeMove();

	LRESULT lRet = m_oMouseManager.HandleMouseMessage(uMsg, wParam, lParam, &bHandled);
	if (bHandled)
		return lRet;
	return 0;
}	

LRESULT  WindowBase::_OnGetObject( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (!m_hWnd)
		return 0;

	IAccessible *pAccessible = NULL;
	this->CreateAccesible(&pAccessible);
	if (pAccessible)
	{
		LRESULT lres = LresultFromObject(IID_IAccessible, wParam, pAccessible);
		pAccessible->Release();
		return lres;
	}
	return 0;
}

void  WindowBase::OnEraseBkgnd(IRenderTarget* pRenderTarget)
{
	if (NULL == pRenderTarget)
		return;

	if (m_pBkgndRender || m_pForegndRender)
	{
        // ���m_pBkgndRenderû�д������������ϵͳ����
		__super::OnEraseBkgnd(pRenderTarget);  
	}
	else
	{
#if 0 // ��������xmlȥ���ðɡ���layered�����Ȼ����alphaͨ��������

		if (NULL == m_oldWndProc)   // Dialog���ͣ�ֱ�����ϵͳ����ɫ
		{
			CRect rc;
			::GetClientRect(m_hWnd,&rc);

            Color c(::GetSysColor(COLOR_BTNFACE));
            c.a = 255;
			pRenderTarget->DrawRect(&rc, &c);
		}
		else  // Window���ͣ�ֱ�ӵ���ϵͳ����
		{
			HDC hDC = pRenderTarget->GetHDC();
            if (hDC)
            {
                // ��ԭʼ��Ϣ��������
			    DefWindowProc(WM_ERASEBKGND, (WPARAM)hDC, 1);  
            }
		}

#endif
	}	
}

IMKMgr* WindowBase::virtualGetIMKMgr()
{
    return static_cast<IMKMgr*>(&m_oMouseManager);
}

#if 0
//
//	��˫���������ύ������DC��
//
void WindowBase::CommitDoubleBuffet2Window(HDC hDCWnd, RECT* prcCommit, int nRectCount)
{
	IRenderTarget* pRenderTarget = m_pRenderLayer->GetRenderTarget();

	POINT  ptSrc = {0, 0};
    m_pRenderLayer->GetOffsetDrawInBuffer(&ptSrc);

	HDC hDC = hDCWnd;
	if (NULL == hDC)
    {
        if (!m_hWnd)  // �����������ˡ�
            return;

		hDC = GetDC(m_hWnd);
    }
    
    HRGN hRgn = NULL;
    CRect rcCommit;

    
	if (NULL == prcCommit)
	{
		::GetClientRect(m_hWnd, &rcCommit);
		nRectCount = 1;
	}
    else if (nRectCount == 1)
    {
        CopyRect(&rcCommit, prcCommit);
        OffsetRect(&rcCommit, ptSrc.x, ptSrc.y);
    }
    // �������ʱ��ͬʱ�ύ��������˸
    else //if (nRectCount > 1)
    {
        RECT rc;
        CopyRect(&rc, prcCommit);
        OffsetRect(&rc, ptSrc.x, ptSrc.y);
        UnionRect(&rcCommit, &rcCommit, &rc);

        hRgn = CreateRectRgnIndirect(&rc);
        for (int i = 1; i < nRectCount; ++i)
        {
            if (::IsRectEmpty(&prcCommit[i]))
                continue;

            CopyRect(&rc, &prcCommit[i]);
            OffsetRect(&rc, ptSrc.x, ptSrc.y);
            UnionRect(&rcCommit, &rcCommit, &rc);

            HRGN hRgn2 = CreateRectRgnIndirect(&rc);
            CombineRgn(hRgn, hRgn, hRgn2, RGN_OR);
            DeleteObject(hRgn2);
        }
    }

    if (hRgn)
    {
        SaveDC(hDC);
        ExtSelectClipRgn(hDC, hRgn, RGN_AND);
    }

    Render2TargetParam  param = {0};
    param.xDst = rcCommit.left;
    param.yDst = rcCommit.top;
    param.wDst = rcCommit.Width();
    param.hDst = rcCommit.Height();
    param.xSrc = rcCommit.left;
    param.ySrc = rcCommit.top;
    param.wSrc = rcCommit.Width();
    param.hSrc = rcCommit.Height();
    param.bAlphaBlend = false;
    param.opacity = 255;

    pRenderTarget->Render2DC(hDC, &param);

#ifdef TRACE_DRAW_PROCESS
    UI_LOG_DEBUG(_T("Commit2Wnd: %d,%d [%d,%d]"),
        rcCommit.left, rcCommit.top,
        RECTW(rcCommit), RECTH(rcCommit));
#endif

   if (hRgn)
   {
       RestoreDC(hDC, -1);
       DeleteObject(hRgn);
   }

	if (NULL == hDCWnd)
	{
		ReleaseDC(m_hWnd, hDC);
	}
}
#endif

void WindowBase::OnGetDesiredSize(SIZE* pSize)
{
    if (NULL == pSize)
        return;

	Panel::OnGetDesiredSize(pSize);

    // �����ȡ������client��С����Ҫת����window��С
    this->CalcWindowSizeByClientSize(*pSize, pSize);
}

//
//	�����֪һ�����ڵ�client�����С����������ڵ�window�����С
//
//	����
//		sizeClient
//			[in]	�ô��ڵĿͻ�����С
//		pSizeWindow
//			[out]	���ش��ڵ�window��С
//
void WindowBase::CalcWindowSizeByClientSize( SIZE sizeClient, SIZE* pSizeWindow )
{
	if( NULL == pSizeWindow )
		return;

	CRect  tempRcWindow, tempRcClient;

	NCCALCSIZE_PARAMS     np;
	WINDOWPOS             wp;

	// ͨ�� WM_NCCALCSIZE ��Ϣ����ϵͳ��������һ��window rect ��Ӧ��client rect�Ƕ���
	np.lppos = &wp;
	::GetWindowRect( this->m_hWnd, &np.rgrc[0] );
	tempRcWindow.CopyRect(&np.rgrc[0]);
	::SendMessage( this->m_hWnd, WM_NCCALCSIZE, (WPARAM)FALSE, (LPARAM)&np );
	tempRcClient.CopyRect(&np.rgrc[0] );

	pSizeWindow->cx = sizeClient.cx + tempRcWindow.Width() - tempRcClient.Width();
	pSizeWindow->cy = sizeClient.cy + tempRcWindow.Height() - tempRcClient.Height();
}

//
//	�����֪һ�����ڵ�window�����С����������ڵ�client�����С
//
//	����
//		rcWindow
//			[in]	�ô��ڵĿʹ�С
//		rcClient
//			[out]	���ش��ڵ�client��С��Ĭ��left=0,top=0����ֻ���ش��ڴ�С��
//
void WindowBase::CalcClientRectByWindowRect( RECT* rcWindow, RECT* rcClient )
{
	NCCALCSIZE_PARAMS     np;
	WINDOWPOS             wp;

	// ͨ�� WM_NCCALCSIZE ��Ϣ����ϵͳ��������һ��window rect ��Ӧ��client rect�Ƕ���
	np.lppos = &wp;
	::CopyRect( &np.rgrc[0], rcWindow );
	::SendMessage( this->m_hWnd, WM_NCCALCSIZE, (WPARAM)FALSE, (LPARAM)&np );

	rcClient->left   = rcClient->top = 0;
	rcClient->right  = np.rgrc[0].right - np.rgrc[0].left;
	rcClient->bottom = np.rgrc[0].bottom - np.rgrc[0].top;
}

BOOL WindowBase::IsChildWindow()
{
	LONG lStyle = ::GetWindowLong(m_hWnd, GWL_STYLE );
	if( lStyle & WS_CHILD )
		return TRUE;
	else
		return FALSE;
}

BOOL  WindowBase::IsWindowVisible()
{
	if (!m_hWnd)
		return FALSE;

	return ::IsWindowVisible(m_hWnd);
}
// CommitWindowBufferListenerMgr&  WindowBase::GetCommitWindowBufferListenerMgr()
// {
// 	return m_MgrCWBL;
// }
// void  WindowBase::AddCommitWindowBufferListener(ICommitWindowBufferListener* p) 
// { 
//     m_MgrCWBL.AddListener(p); 
// }
// void  WindowBase::RemoveCommitWindowBufferListener(ICommitWindowBufferListener* p) 
// { 
//     m_MgrCWBL.RemoveListener(p); 
// }

Object* WindowBase::GetHoverObject()
{
	return m_oMouseManager.GetHoverObject();
}
Object* WindowBase::GetPressObject()
{
	return m_oMouseManager.GetPressObject();
}
void  WindowBase::SetPressObject(Object* p)
{
	m_oMouseManager.SetHoverObject(p);
	m_oMouseManager.SetPressObject(p);
}

void WindowBase::ShowWindow()
{
	::ShowWindow( m_hWnd, SW_SHOW );
}
void WindowBase::HideWindow()
{
	::ShowWindow( m_hWnd, SW_HIDE );
}

void  WindowBase::CenterWindow(HWND hWndCenter)
{
    // Copy  CWindowImpl::CenterWindow()
    CWindow wnd;
    wnd.Attach(m_hWnd);
    wnd.CenterWindow(hWndCenter);
    wnd.Detach();
}

void  WindowBase::CenterWindow(HMONITOR hMonitor)
{
    CRect  rcDlg;
    CRect  rcArea;

    ::GetWindowRect(m_hWnd, &rcDlg);
    
    if (!hMonitor)
    {
        POINT pt = {0};
        hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
    }
    if(hMonitor)
    {
        MONITORINFO  mi = {sizeof(mi)};
        GetMonitorInfo(hMonitor, &mi);
        rcArea.CopyRect(&mi.rcWork);
    }
    else
    {
        ::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
    }

    int DlgWidth = rcDlg.Width();
    int DlgHeight = rcDlg.Height();

    int xLeft = (rcArea.left + rcArea.right) / 2 - DlgWidth / 2;
    int yTop = (rcArea.top + rcArea.bottom) / 2 - DlgHeight / 2;

    // if the dialog is outside the screen, move it inside
    if(xLeft + DlgWidth > rcArea.right)
        xLeft = rcArea.right - DlgWidth;
    if(xLeft < rcArea.left)
        xLeft = rcArea.left;

    if(yTop + DlgHeight > rcArea.bottom)
        yTop = rcArea.bottom - DlgHeight;
    if(yTop < rcArea.top)
        yTop = rcArea.top;

    ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1,
        SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

// ʹ�����沼�����¸��´���λ�ô�С����Ҫ������UIEditor���޸���
// ���ڶ���Ĵ�С����ʹ��Object::UpdateLayout�������²����Ӷ��������
void  WindowBase::UpdateDesktopLayout()
{
    DesktopLayout dl;
    dl.Arrange(this);
}

void WindowBase::SaveMemBitmap(TCHAR* szFile)
{
	UIASSERT(0 && _T("TODO:"));
	return;
#ifdef _DEBUGx
	if (NULL == m_hMemBitmap)
		return;

	::SelectObject(m_hMemDC, m_hOldBitmap);
	Image image;
	image.Attach(m_hMemBitmap);
//	image.ForceUseAlpha();
	image.Save(szFile, Gdiplus::ImageFormatBMP);
	image.Detach();
	::SelectObject(m_hMemDC, m_hMemBitmap);
#endif
}

// ���ڴ�λͼ��ͼ��ָ������
void WindowBase::DrawMemBitmap(HDC hDC, RECT* prc, bool bAlphaBlend)
{
	assert (0 && "��Ҫ��д");
#if 0
    HDC hMemDC = m_pRenderLayer->GetRenderTarget()->GetHDC();
    POINT  ptOffset;
    m_pRenderLayer->GetOffsetDrawInBuffer(&ptOffset);

	if (bAlphaBlend) 
	{
        BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
        AlphaBlend(hDC, prc->left, prc->top, RECTW(prc), RECTH(prc), hMemDC, ptOffset.x, ptOffset.y, RECTW(prc), RECTH(prc), bf);
	}
	else
	{
        HRGN hRgn = CreateRectRgn(0,0,0,0);
		int nRet = ::GetWindowRgn(m_hWnd, hRgn);
        if (ERROR == nRet)
        {
            BitBlt(hDC, prc->left, prc->top, RECTW(prc), RECTH(prc), hMemDC, ptOffset.x, ptOffset.y, SRCCOPY);
        }
        else
        {
		    ::SelectClipRgn(hDC, hRgn);
		    ::OffsetClipRgn(hDC, prc->left, prc->top);
    		
            BitBlt(hDC, prc->left, prc->top, RECTW(prc), RECTH(prc), hMemDC, ptOffset.x, ptOffset.y, SRCCOPY);

		    ::SelectClipRgn(hDC, NULL);
        }
        SAFE_DELETE_GDIOBJECT(hRgn);
	}
#endif
}

void WindowBase::EnableDwmTransition(bool b)
{
    DwmHelper::GetInstance()->EnableTransition(m_hWnd, b);
}

void WindowBase::SetActive(bool b)
{
    m_objState.activate = b;
}
bool WindowBase::IsActive()
{
	return m_objState.activate;
}

#if 1   // CreateData

CREATE_WND_DATA WindowBase::s_create_wnd_data;
CREATE_WND_DATA::CREATE_WND_DATA()
{
	m_pCreateWndList = NULL;
	::InitializeCriticalSection(&m_cs);
}
CREATE_WND_DATA::~CREATE_WND_DATA()
{
	::DeleteCriticalSection(&m_cs);
}

void  CREATE_WND_DATA::AddCreateWndData(_AtlCreateWndData* pData, void* pThis)
{
	UIASSERT(pData != NULL && pThis != NULL);

	pData->m_pThis = pThis;
	pData->m_dwThreadID = ::GetCurrentThreadId();

	EnterCriticalSection(&m_cs);

	pData->m_pNext = this->m_pCreateWndList;
	this->m_pCreateWndList = pData;

	LeaveCriticalSection(&m_cs);
}
void*  CREATE_WND_DATA::ExtractCreateWndData()
{
	void* pv = NULL;
	EnterCriticalSection(&m_cs);

	_AtlCreateWndData* pEntry = this->m_pCreateWndList;
	if(pEntry != NULL)
	{
		DWORD dwThreadID = ::GetCurrentThreadId();
		_AtlCreateWndData* pPrev = NULL;
		while(pEntry != NULL)
		{
			if(pEntry->m_dwThreadID == dwThreadID)
			{
				if(pPrev == NULL)
					this->m_pCreateWndList = pEntry->m_pNext;
				else
					pPrev->m_pNext = pEntry->m_pNext;
				pv = pEntry->m_pThis;
				break;
			}
			pPrev = pEntry;
			pEntry = pEntry->m_pNext;
		}
	}

	LeaveCriticalSection(&m_cs);
	return pv;
}

void  WindowBase::SetFocusObject(Object* pObj)
{
    m_oMouseManager.SetFocusObject(pObj);
}

// ��ȡ��ǰ����µĶ��� 
Object*  WindowBase::GetObjectUnderCursor()
{
    POINT  pt;
    GetCursorPos(&pt);
    ::ScreenToClient(m_hWnd, &pt);

    return m_oMouseManager.GetObjectByPos(this, &pt, NULL);
}
Object*  WindowBase::GetObjectByPos(Object* pObjParent, POINT* pt, bool bSkinBuilderInvoke)
{
    if (bSkinBuilderInvoke)
	{
        return m_oMouseManager.GetObjectByPos_UIEditor(pObjParent, pt, NULL);
	}
    else
	{
        return m_oMouseManager.GetObjectByPos(pObjParent, pt, NULL);
	}
}
#endif

// ������ˢ��
void WindowBase::OnSetDefId(IObject* pButton)
{
    if (pButton)
        m_oMouseManager.SetOriginDefaultObject(pButton->GetImpl());
    else
        m_oMouseManager.SetOriginDefaultObject(NULL);
}

IObject* WindowBase::OnGetDefId()
{
    Object* pObj = m_oMouseManager.GetOriginDefaultObject();
    if (pObj)
    {
        return pObj->GetIObject();
    }
    else
    {
        return NULL;
    }
}

IRenderFont*  WindowBase::GetWindowDefaultRenderFont() 
{
	return m_pDefaultFont; 
}

void  WindowBase::SetDefaultRenderFont(LPCTSTR szFontId)
{
	SAFE_RELEASE(m_pDefaultFont);
    if (!szFontId)
        return;

	FontRes* pFontRes = NULL;
	if (m_pSkinRes)
		pFontRes = &m_pSkinRes->GetFontRes();

	if (NULL == pFontRes)
		return;

    // Ĭ�ϣ����������ַ�������ֱ�Ӵ���Ĭ������
    if (szFontId[0] == TEXT('\0'))
    {
        pFontRes->GetDefaultFont(m_oWindowRender.GetGraphicsRenderType(), &m_pDefaultFont);
    }
    else
    {
	    pFontRes->GetFont(szFontId, m_oWindowRender.GetGraphicsRenderType(), &m_pDefaultFont);
    }

	// ����Ĭ������
	if (NULL == m_pDefaultFont)
	{
		// GDI ��������
		HFONT hFont = (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0,0);
		if (hFont)
		{
			UI_AttachFont(&m_pDefaultFont, hFont, m_oWindowRender.GetGraphicsRenderType());
		}
		else
		{
			// UI Font Res Defualt Font
			pFontRes->GetDefaultFont(m_oWindowRender.GetGraphicsRenderType(), &m_pDefaultFont);
			if (NULL == m_pDefaultFont)
			{
				// System Default Font
				hFont = (HFONT)GetStockObject(SYSTEM_FONT);
				UI_AttachFont(&m_pDefaultFont, hFont, m_oWindowRender.GetGraphicsRenderType());
			}
		}
	}
}

LPCTSTR  WindowBase::GetDefaultRenderFontId()
{
	if (!m_pDefaultFont)
		return NULL;

	if (m_pDefaultFont->IsAttach())
		return NULL;
	
	FontRes* pFontRes = GetUIApplication()->GetActiveSkinFontRes();
	if (!pFontRes)
		return NULL;

	return pFontRes->GetRenderFontId(m_pDefaultFont);
}

bool  WindowBase::IsDoModal() 
{ 
    return m_bDoModal;
}
HRESULT  WindowBase::SetCanDrop(bool b)
{
    return m_oDragDropManager.SetDroppable(b); 
}

void  WindowBase::SetObjectPos( int x, int y, int cx, int cy, int nFlag)
{
	// ���ڴ�����˵���������õ��Ƿǿͻ����Ĵ�С
	::SetWindowPos(m_hWnd, NULL, x, y, cx, cy, SWP_NOZORDER|SWP_NOACTIVATE);
	::GetClientRect(m_hWnd, &m_rcParent);
}

void  WindowBase::virtualSetVisibleEx(VISIBILITY_TYPE eType)
{
	bool bVisibleCompatible = eType==VISIBILITY_VISIBLE ? true:false;
	::ShowWindow(m_hWnd, bVisibleCompatible?SW_SHOW:SW_HIDE);
}

void  WindowBase::virtualSetEnable(bool b)
{
	::EnableWindow(m_hWnd, b ? TRUE:FALSE);
}

bool  WindowBase::IsVisible()
{
	return ::IsWindowVisible(((WindowBase*)this)->m_hWnd)?true:false;
}

bool  WindowBase::IsEnable()
{
	return ::IsWindowEnabled(m_hWnd)?true:false;
}
void  WindowBase::SetGpuComposite(bool b)
{
    assert (!m_hWnd && TEXT("�ú���Ŀǰ��Ҫ�ڴ��ڴ���֮ǰ����"));

	if (b)
	{
		if (!GetUIApplication()->IsGpuCompositeEnable())
			b = false;
	}

	if (b == m_windowStyle.hard_composite)
		return;

	m_windowStyle.hard_composite = b;
	// m_oWindowRender.OnHardCompositeChanged(b);

    // UI_LOG_DEBUG(TEXT("hard composite enable, window=0x%08x"), this);
}

bool  WindowBase::IsGpuComposite()
{
	return m_windowStyle.hard_composite;
}
void  WindowBase::DirectComposite()
{
	assert (0 && "��������Ƿ���Ҫ��������?");
}

void  WindowBase::load_minwidth(long n)
{
    m_lMinWidth = ScaleByDpi_if_gt0(n);
}
long  WindowBase::save_minwidth()
{
    return RestoreByDpi_if_gt0(m_lMinWidth);
}
void  WindowBase::load_maxwidth(long n)
{
    m_lMaxWidth = ScaleByDpi_if_gt0(n);
}
long  WindowBase::save_maxwidth()
{
    return RestoreByDpi_if_gt0(m_lMaxWidth);
}

void  WindowBase::load_minheight(long n)
{
    m_lMinHeight = ScaleByDpi_if_gt0(n);
}
long  WindowBase::save_minheight()
{
    return RestoreByDpi_if_gt0(m_lMinHeight);
}
void  WindowBase::load_maxheight(long n)
{
    m_lMaxHeight = ScaleByDpi_if_gt0(n);
}
long  WindowBase::save_maxheight()
{
    return RestoreByDpi_if_gt0(m_lMaxHeight);
}


void WindowBase::SetWindowMessageCallback(IWindowMessageCallback* p)
{
	if (m_pCallbackProxy)
	{
		this->RemoveHook(m_pCallbackProxy);
		SAFE_DELETE(m_pCallbackProxy);
	}

	if (!p)
		return;

	m_pCallbackProxy = new CWindowMessageHookProxy(p);
	this->AddHook(m_pCallbackProxy, 0, 0);
}
