#include "stdafx.h"
#include "popupcontrolwindow.h"
#include "Inc\Interface\imenu.h"
#include "Src\Control\Menu\menu.h"
#include "..\UISDK\Inc\Interface\iuiinterface.h"
#include "..\UISDK\Inc\Interface\ilayout.h"

namespace UI
{

PopupControlWindow::PopupControlWindow():ICustomWindow(CREATE_IMPL_TRUE)
{
	m_pBindObject = NULL;
	m_pContentObj = NULL;
	m_bExitLoop = false;
    m_bMouseIn = false;
	m_hWndClickFrom = NULL;
	m_rcClickFrom.SetRectEmpty();
}

PopupControlWindow::~PopupControlWindow()
{
}

//(WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE);
// ע������Լ��ĸ����ڲ������棬�򲻻���������������ʾͼ��
BOOL PopupControlWindow::PreCreateWindow(CREATESTRUCT* pcs)
{
    if (NULL == pcs->hwndParent)
    {
    	pcs->dwExStyle |= WS_EX_TOPMOST|WS_EX_NOACTIVATE|WS_EX_TOOLWINDOW;
    }
    else
    {
        pcs->dwExStyle |= WS_EX_NOACTIVATE;
    }
	pcs->lpszClass = WND_POPUP_CONTROL_NAME/*WND_POPUP_CONTROL_SHADOW_NAME*/;

	// �˵����б��ڿ��ܻ�Ƚ�С�������WS_CAPTION�����´�������С�߶�����
	pcs->style = WS_POPUP;
    return TRUE;
}

// void  PopupControlWindow::Create(
// 	IObject*  pBindObj, 
// 	IObject* pContentObj, 
// 	LPCTSTR szId, 
// 	HWND hParentWnd)
// {
//     m_pBindObject = pBindObj;
// 	m_pContentObj = pContentObj;
//     __super::Create(_T(""), hParentWnd);  // �ڴ�����panel��layer֮���ٴ������ڣ������һ����Ӧ_OnSize��û��layout
// 
//     IMapAttribute*  pMapAttrib = NULL;
//     pBindObj->GetMapAttribute(&pMapAttrib);
//     if (pMapAttrib)
//     {
// 		if (szId)
// 			this->SetID(szId);
// 
// 		SetAttributeByPrefix(XML_POPUPWINDOW_PREFIX, pMapAttrib, false, true);
// 		SAFE_RELEASE(pMapAttrib);
// 	}
// }
 
void  PopupControlWindow::SetBindObject(IObject* pBindObj)
{
	m_pBindObject = pBindObj;
}
void  PopupControlWindow::SetContentObject(IObject* pContentObj)
{
	m_pContentObj = pContentObj;
}

void  PopupControlWindow::SetPopupFromInfo(HWND hWnd, RECT* prcClickInWnd)
{
	m_hWndClickFrom = hWnd;
	if (prcClickInWnd)
		m_rcClickFrom.CopyRect(prcClickInWnd);
	else
		m_rcClickFrom.SetRectEmpty();
}

// 2014.4.18  libo
// ����һ��bDesignMode������֧����UIEditor������ʾ�˵� 
void  PopupControlWindow::Show(
        POINT pt, 
        SIZE size, 
        BOOL bDoModal, 
        long lShowType, 
        BOOL bDesignMode)
{
    if (NULL == m_pBindObject || NULL == m_pContentObj)
        return;

    m_bExitLoop = false;

    CRect rcWorkArea;
    MONITORINFO mi = {sizeof(MONITORINFO), 0};
    if (GetMonitorInfo(MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST), &mi))
    {
        rcWorkArea = mi.rcWork;
    }
    else
    {
        ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
    }

    if (size.cx > rcWorkArea.Width())
        size.cx = rcWorkArea.Width();
    if (size.cy > rcWorkArea.Height())
        size.cy = rcWorkArea.Height();

    if (pt.x < rcWorkArea.left)
        pt.x = rcWorkArea.left;
    if (pt.x + size.cx > rcWorkArea.right)
        pt.x = rcWorkArea.right - size.cx;
    if (pt.y < rcWorkArea.top)
        pt.y = rcWorkArea.top;

    if (pt.y + size.cy > rcWorkArea.bottom)
	{
		if (!m_rcClickFrom.IsRectEmpty())
		{
			// ���б���ʾ��COMBOBOX���棬�����Ǹ���COMBOBOX��
			POINT ptTop = {m_rcClickFrom.left, m_rcClickFrom.top};
			if (m_hWndClickFrom)
				::MapWindowPoints(m_hWndClickFrom, NULL, &ptTop, 1);
			pt.y = ptTop.y - size.cy;
		}
		else
		{
			pt.y = rcWorkArea.bottom - size.cy;
		}
	}

    // ע������ô�����һ�εĴ�С����һ����ʾ�Ĵ�Сһ�£��ᵼ�´����ղ���OnSize֪ͨ�������ڲ��ؼ��޷�����
    HWND hPopupWnd = GetHWND();
    CRect rcOld, rcNew;

    ::GetClientRect(hPopupWnd, &rcOld);
    ::SetWindowPos(hPopupWnd, NULL, pt.x, pt.y, size.cx, size.cy, SWP_NOZORDER|SWP_NOACTIVATE);
    ::GetClientRect(hPopupWnd, &rcNew);

    if (rcNew.Width() == rcOld.Width() && rcNew.Height() == rcOld.Height())
    {
        if (GetLayout())
            GetLayout()->Arrange(NULL);
    }

    UISendMessage(m_pContentObj, UI_WM_INITPOPUPCONTROLWINDOW, 0,0,0, this);
    if (m_pBindObject && m_pBindObject != m_pContentObj)
    {
        UISendMessage(m_pBindObject, UI_WM_INITPOPUPCONTROLWINDOW, 0,0,0, this);
    }

	if (!bDesignMode)
	{
        if (lShowType != SW_HIDE)
        {
            // ���showwindow������ֱ����ʾ��alpha 255������
            GetLayer()->SetOpacity(10, nullptr);
            GetWindowRender()->InvalidateNow();

            ::ShowWindow(hPopupWnd, lShowType);
            start_show_animate();
        }
        else
        {
            ::ShowWindow(hPopupWnd, lShowType);
        }

		IMessageFilterMgr* pMgr = GetUIApplication()->GetMessageFilterMgr();
		pMgr->AddMessageFilter(static_cast<IPreTranslateMessage*>(this));

		if (TRUE == bDoModal)
			GetUIApplication()->MsgHandleLoop(&m_bExitLoop);
	}
}

void  PopupControlWindow::Hide()
{
    m_bExitLoop = true;

    if (!IsWindowVisible())  // �Ѿ�������
        return ;

	HideWindow();
    if (m_pContentObj)
    {
        UISendMessage(m_pContentObj, UI_WM_UNINITPOPUPCONTROLWINDOW, 0,0,0, this);  // ������Ϣ�������﷢�ͣ���֤ÿһ���Ӳ˵���������ʱ�����յ�����Ϣ
    }
	if (m_pBindObject && m_pBindObject != m_pContentObj)
	{
		UISendMessage(m_pBindObject, UI_WM_UNINITPOPUPCONTROLWINDOW, 0,0,0, this);
	}

    // Ҳ��listbox��������ʾ��
    if (m_pContentObj)
        m_pContentObj->GetUIApplication()->HideToolTip();

    // �������б�����ʱ���������٣�����ʱҪ�ɵ�pretranslate
    IMessageFilterMgr* pMgr = GetUIApplication()->GetMessageFilterMgr();
    if (pMgr)
        pMgr->RemoveMessageFilter(static_cast<IPreTranslateMessage*>(this));
}

void  PopupControlWindow::Destroy()
{
    ::DestroyWindow(GetHWND());
}

void PopupControlWindow::OnInitialize()
{
    __super::nvProcessMessage(GetCurMsg(), 0, 0);
    SetWindowResizeType(WRSB_NONE);
	EnableDwmTransition(false);

//     if (m_pContentObj)
//     {
// 	    m_pContentObj->AddHook(this, 0, 1);
//     }
}

void PopupControlWindow::OnDestroy()
{
	SetMsgHandled(FALSE);

    IMessageFilterMgr* pMgr = GetUIApplication()->GetMessageFilterMgr();
    if (pMgr)
        pMgr->RemoveMessageFilter(static_cast<IPreTranslateMessage*>(this));

    if (m_pBindObject && m_pBindObject != m_pContentObj)
    {
        UISendMessage(m_pBindObject, UI_WM_POPUPWINDOWDESTROY);
    }

	// �˵����Լ������������У��Ӵ��ڵ����ṹ���Ƴ��ˡ����Է������ٴ���
}


// ע��Ϊʲô��alpha=10��ʼ������������0��ʼ��
//     �����edit���Ҽ����˵�ǰ�����Ƚ��������Ϊidc_arrow����������һ��ʼ
//     ���ڵ�͸���ȱ�����Ϊ0�����������Ϣ�����ߵ���edit�У�����ֱ��޸�Ϊ
//     idc_ibean����������ʼ��͸��������10.����������Ϊ1���У�
void  PopupControlWindow::start_show_animate()
{
	LayerAnimateParam param = { 0 };
	param.bBlock = false;
	GetLayer()->SetOpacity(10, nullptr);
	GetLayer()->SetOpacity(255, &param);
}

// Popup�ؼ������뻻����Ŀǰ
void  PopupControlWindow::OnSkinChanging(BOOL* pbChange)
{
    *pbChange = FALSE;
}

// ������������ڵ�ʱ�򣬲�Ҫȥ��ռ�����ڵĽ���
// ��������ͬʱ�����������Ϣ�ͼ�����Ϣ��ȡ��������
// WM_NCHITTEST���յ�
int PopupControlWindow::OnMouseActivate(HWND wndTopLevel, UINT nHitTest, UINT message)
{
	return MA_NOACTIVATE;
}
// ��ĳЩʱ��Ҫ��˵���������ģ����絯��һ������̵�CEF���档ֻ���õ��Լ���killfocus��Ϣ
void PopupControlWindow::OnKillFocus(IObject* pNewFocusObj)
{
    SetMsgHandled(FALSE);
    this->Hide();
}

// �����Ϣ��������������������/�������㴰�ڵ�ʱ��
// ���������ص�֪ͨ�����ڵ�����Լ��Ķ��㴰��ȴ������
void PopupControlWindow::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	SetMsgHandled(FALSE);
	if (FALSE == bActive)
	{
		this->Hide();
	}
}



// ע��
//	�����GetMessage��ʵ��ֻ������PostMessage���͹�������Ϣ��
//  ��������SendMessage����Ϣ�ǲ��ܵ�֪�ġ�
//

BOOL PopupControlWindow::PreTranslateMessage(MSG* pMsg)
{
	if ((WM_KEYFIRST <= pMsg->message && WM_KEYLAST >= pMsg->message) || WM_MOUSEWHEEL == pMsg->message)
	{
		if (WM_KEYDOWN == pMsg->message && VK_ESCAPE == pMsg->wParam)
		{
			this->Hide();
			return TRUE;
		}

		BOOL bHandle = FALSE;
		UISendMessage(this->m_pContentObj, pMsg->message, pMsg->wParam, pMsg->lParam, 0, 0, 0, &bHandle);
		//return bHandle;
        return TRUE;
	}
	
	if (WM_MOUSEMOVE == pMsg->message ||
		WM_NCMOUSEMOVE == pMsg->message )
	{
		if (pMsg->hwnd != GetHWND())
		{
            if (m_bMouseIn)
            {
                ::PostMessage(GetHWND(), WM_MOUSELEAVE, 0, 0);
                m_bMouseIn = false;
            }
			return TRUE;  // �����������ƶ��¼�����
		}
		else
		{
            m_bMouseIn = true;
			return FALSE;
		}
	}
	else if (WM_LBUTTONDOWN == pMsg->message || WM_LBUTTONDBLCLK == pMsg->message)
	{
		// ������������������һ���жϡ��п��ܸô��ǵ����˵��ĵط���
		// ֱ�ӹرղ˵��ᵼ�²˵����رպ���������ʾ�����˵� 

        //         RECT rcWindow;
        //         ::GetWindowRect(m_hWnd, &rcWindow);
        // 
        //         DWORD dwPos = GetMessagePos();
        //         POINT pt = {GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos)};
        //         if (!PtInRect(&rcWindow, pt))  
        // ע��popupwindow�����Ƿֲ㴰�ڣ�����͸��������Բ�ǣ���
        // ���Բ���ʹ��PtInRect�����ж�
        if (pMsg->hwnd != GetHWND()) 
		{
			this->Hide();

			// ��ԭ���ڷ���һ������ƶ���Ϣ������hover����
			// ����ᵼ��popupwnd��ʧ��ԭ�������ֱ�ӵ���޷�Ӧ
			// ���ߵ��´��ڽ��յ�lbuttondown֮ǰ����û�и���hover����
			::SendMessage(pMsg->hwnd, WM_MOUSEMOVE, 0, pMsg->lParam);

			// ����ڵ��������������ˣ��رյ�ǰ���ڡ���������˵������İ�ť���棬��ֱ�ӹرգ�����ת����Ϣ
			if (m_hWndClickFrom && m_hWndClickFrom == pMsg->hwnd)
			{
				POINT ptWnd = {GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam)};
				if (PtInRect(&m_rcClickFrom, ptWnd))
					return TRUE;
			}

			return FALSE/*TRUE*/;
		}
	}
	else if (
		WM_RBUTTONDOWN   == pMsg->message ||
		WM_RBUTTONDBLCLK == pMsg->message ||
		WM_MBUTTONDOWN   == pMsg->message ||
		WM_MBUTTONDBLCLK == pMsg->message ||
		WM_XBUTTONDOWN   == pMsg->message ||
		WM_XBUTTONDBLCLK == pMsg->message ||
		WM_NCLBUTTONDOWN == pMsg->message ||
		WM_NCRBUTTONDOWN == pMsg->message ||
        WM_NCRBUTTONDBLCLK == pMsg->message || 
        /*WM_GESTURE*/0x0119 == pMsg->message ||
        /*WM_TOUCH*/0x0240 == pMsg->message)
	{
        if (pMsg->hwnd != GetHWND())
        {
            this->Hide();

            // ��ԭ���ڷ���һ������ƶ���Ϣ������hover����
            // ����ᵼ��popupwnd��ʧ��ԭ�������ֱ�ӵ���޷�Ӧ
			// ���ߵ��´��ڽ��յ�lbuttondown֮ǰ����û�и���hover����
			::SendMessage(pMsg->hwnd, WM_MOUSEMOVE, 0, pMsg->lParam);

            return FALSE/*TRUE*/;
		}
	}
    else if (pMsg->message == WM_CLOSE && pMsg->hwnd != GetHWND())
    {
        // �ر��˸����ڣ��Լ�Ҳ�ùص�����������������Ұָ��
        if (pMsg->hwnd == m_hWndClickFrom ||
            pMsg->hwnd == GetWindow(GetHWND(), GW_OWNER))
        {
            Hide();
        }
    }

	return FALSE;
}

PopupListBoxWindow::PopupListBoxWindow()
{
}

BOOL PopupListBoxWindow::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYFIRST <= pMsg->message && WM_KEYLAST >= pMsg->message)
	{
		if (pMsg->message != WM_KEYDOWN)
			return FALSE;

		UINT nChar = pMsg->wParam;
		if (VK_ESCAPE == nChar)
		{
			this->Hide();
			return TRUE;
		}

		// ֻ����һЩ��������������������Ϣ���Ǵ��ݸ���ǰFOCUS COMBOBOX'S EDIT
		if (IsKeyDown(VK_SHIFT))  // SHIFT + xxx ��EDIT����
			return FALSE;

		if (nChar == VK_UP    ||
			nChar == VK_DOWN  ||
			nChar == VK_HOME  ||
			nChar == VK_END   ||
			nChar == VK_PRIOR ||
			nChar == VK_NEXT  ||
            nChar == VK_RETURN)
		{
			BOOL bHandle = FALSE;
			UISendMessage(this->m_pContentObj, pMsg->message, pMsg->wParam, pMsg->lParam, 0, 0, 0, &bHandle);
			return TRUE;/*bHandle*/; // �п��ܴ����������ؼ�
		}

		return FALSE;
	}

	return __super::PreTranslateMessage(pMsg);
}

void PopupListBoxWindow::OnInitialize()
{
	__super::OnInitialize();

	// ���ԭ���������hover��press����
	HWND hWnd = GetActiveWindow();
	::PostMessage(hWnd,WM_MOUSELEAVE,0,0);
}

void PopupListBoxWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SetMsgHandled(FALSE);

	if (VK_DOWN == nChar || VK_UP == nChar) 
	{
		SetMsgHandled(TRUE);
		m_pContentObj->ProcessMessage(GetCurMsg(), 0);
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
PopupMenuWindow::PopupMenuWindow()
{
	m_pMenu = NULL;
	m_ptLastMousePos.x = m_ptLastMousePos.y = 0;
    m_pLastHoverMenu = NULL;
}

// �˵�û�и����ڣ�ֱ���ö�
BOOL PopupMenuWindow::PreCreateWindow(CREATESTRUCT* pcs)
{
	__super::PreCreateWindow(pcs);
	pcs->dwExStyle |= WS_EX_TOPMOST;  

	return TRUE;
}

void  PopupMenuWindow::SetContentObject(IObject* pContentObj)
{
	__super::SetContentObject(pContentObj);

    if (m_pContentObj)
    {
        IMenu* p = (IMenu*)m_pContentObj->QueryInterface(__uuidof(IMenu));
        if (p)
            m_pMenu = p->GetImpl();
    }

    if (m_pMenu)
    {
	    // ���ԭ���������hover��press����.����Ҳ��ʹԭ����releasecaption����mouseleave�У�
        // ��˿���ֱ���ڰ�ť��lbuttonup��Ϣ�е����˵���

        // �Ӳ˵�������ʱ,activeWindow�Ǹ��˵�����˲���Ҫ�ٷ���mouseleave��Ϣ
	    if (m_pMenu->GetRootMenu() == m_pMenu) 
	    {
		    HWND hWnd = GetActiveWindow();
		    ::PostMessage(hWnd,WM_MOUSELEAVE,0,0);
	    }
    }
}


//
//	ע��ֻ�и��˵��Ż���MenuLoop���Ӳ˵�û��
//
//  REMARK: ģ̬�˵������ڼ䣬���ڴ���ֱ�ӱ����ٵ��²˵����ر������⣺
//  ��������¿��Բ����ô���������֮ǰ��POSTMESSAGE�����������ﱻ����
//  ����ǰ�رղ˵���
//
BOOL PopupMenuWindow::PreTranslateMessage(MSG* pMsg)
{
    if (NULL == m_pMenu)
        return FALSE;

    HWND hWnd = GetHWND();

	if ((WM_KEYFIRST <= pMsg->message && WM_KEYLAST >= pMsg->message) 
        || WM_MOUSEWHEEL == pMsg->message  // �˵��������㣬������ֹ���ݿؼ�(richedit)����
        )
	{
		if (WM_SYSKEYDOWN == pMsg->message && pMsg->wParam == VK_MENU)
		{
			this->Hide();   // ����ALT��ʱ�����������˵���(ESCֻ����last menu)
			return TRUE;
		}

		BOOL bHandle = FALSE;
        Menu* pMenu = m_pMenu->GetLastMenu();
        if (pMenu)
    		UISendMessage(pMenu->GetIMenu(), pMsg->message, pMsg->wParam, pMsg->lParam, 0, 0, 0, &bHandle);

		return bHandle;
	}

	if (WM_MOUSEMOVE == pMsg->message ||
		WM_NCMOUSEMOVE == pMsg->message )
	{
        // ���β˵���������Ϣ����ϵͳ��,��������뿪�˵���ʱ�򣬵ò���һ��MOUSELEAVE�¼�...(TODO: �ǲ������ĸ����)
        Menu*  pMenuHover = m_pMenu->GetMenuByHWND(pMsg->hwnd);
		if (NULL == pMenuHover)
		{
            if (m_pLastHoverMenu)
            {
                ::PostMessage(m_pLastHoverMenu->GetIMenu()->GetHWND(), WM_MOUSELEAVE, 0, 0);
            }
            m_pLastHoverMenu = NULL;
			return TRUE;  // �����������ƶ��¼�����
		}
		else
		{
            m_pLastHoverMenu = pMenuHover;
			POINT ptNow = {LOWORD(pMsg->lParam), HIWORD(pMsg->lParam)};
			if (ptNow.x == m_ptLastMousePos.x && ptNow.y == m_ptLastMousePos.y)  // �����������Ӳ˵�����ʾ��������MOUSEMOVE������
			{
				//UI_LOG_DEBUG(_T("point repeat"));
				return TRUE;
			}

			m_ptLastMousePos = ptNow;
			return FALSE;
		}
	}
	else if (
        WM_LBUTTONDOWN   == pMsg->message ||
        WM_LBUTTONUP     == pMsg->message ||
		WM_LBUTTONDBLCLK == pMsg->message ||
		WM_RBUTTONDOWN   == pMsg->message ||
        WM_RBUTTONUP     == pMsg->message ||  // TMD�������������Ϣ�����ܵ����ⲿ��ȥ����һ�β˵�������һ�εĲ˵�״̬��û�ɵ�
		WM_RBUTTONDBLCLK == pMsg->message ||
		WM_MBUTTONDOWN   == pMsg->message ||
        WM_MBUTTONUP     == pMsg->message ||
		WM_MBUTTONDBLCLK == pMsg->message ||
		WM_XBUTTONDOWN   == pMsg->message ||
		WM_XBUTTONDBLCLK == pMsg->message ||
		WM_NCLBUTTONDOWN == pMsg->message ||
		WM_NCRBUTTONDOWN == pMsg->message ||
        WM_NCLBUTTONUP   == pMsg->message ||
        WM_NCRBUTTONUP   == pMsg->message ||
		WM_NCRBUTTONDBLCLK == pMsg->message)
	{
		RECT rcWindow;
		::GetWindowRect(GetHWND(), &rcWindow);
        Menu* pHitMenu = m_pMenu->GetMenuByPos(pMsg->pt);

        // ����ڵ��������������ˣ��رյ�ǰ����
		if (NULL == pHitMenu)  
		{
			this->Hide();

			// ��ԭ���ڷ���һ������ƶ���Ϣ������hover����
			// ����ᵼ��popupwnd��ʧ��ԭ�������ֱ�ӵ���޷�Ӧ
			// ���ߵ��´��ڽ��յ�lbuttondown֮ǰ����û�и���hover����
			
			HWND hWnd = m_hWndClickFrom;  // GetActiveWindow(); ������activewindow��activewindow�õ����Ƕ������ڣ���ԭ���ڿ������Ӵ���
			::SendMessage(hWnd, WM_MOUSEMOVE, 0, pMsg->lParam);
            
            // ����ǰ������Ĵ��ڷ���һ�������Ϣ�����ڽ���ڵ����˵���
            // ֱ�ӵ����һ�������ϵİ�ť��û�з�Ӧ
            if (pMsg->hwnd != hWnd)
                ::SendMessage(pMsg->hwnd, WM_MOUSEMOVE, 0, pMsg->lParam);

			// ����ڵ��������������ˣ��رյ�ǰ���ڡ���������˵������İ�ť���棬��ֱ�ӹرգ�����ת����Ϣ
			if (m_hWndClickFrom && m_hWndClickFrom == pMsg->hwnd)
			{
				POINT ptWnd = {GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam)};
				if (PtInRect(&m_rcClickFrom, ptWnd))
					return TRUE;
			}

            return FALSE;
		}
	}

	return FALSE;
}
}