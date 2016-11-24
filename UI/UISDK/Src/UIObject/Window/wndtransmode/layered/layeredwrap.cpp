#include "stdafx.h"
#include "layeredwrap.h"
#include "Src/UIObject/Window/customwindow.h"
#include "Src/Layer/layer.h"
#include "Src/Layer/software_layer.h"

// WHERE_NOONE_CAN_SEE_ME
#define WINDOWS_MINIMIZED_POINT  -32000  // ������С����window���ݸ����ǵ�λ��
namespace UI
{


LayeredWindowWrap::LayeredWindowWrap()
{
    m_nHitTestFlag = 0;
    m_ptStartSizeMove.x = 0;
    m_ptStartSizeMove.y = 0;
    m_ptWindowOld.x = 0;
    m_ptWindowOld.y = 0;
    m_sizeWindowOld.cx = 0;
    m_sizeWindowOld.cy = 0;
    m_ptWindow.x = NDEF;
    m_ptWindow.y = NDEF;
    m_sizeWindow.cx = NDEF;
    m_sizeWindow.cy = NDEF;
    m_pILayeredWindowWrap = NULL;
}
LayeredWindowWrap::~LayeredWindowWrap()
{
    SAFE_DELETE(m_pILayeredWindowWrap);
}

void  LayeredWindowWrap::GetWindowPos()
{
	HWND hWnd = GetHWND();

    CRect rc;
    ::GetClientRect(hWnd, &rc);
    MapWindowPoints(hWnd, NULL, (LPPOINT)&rc, 2);

    m_sizeWindow.cx = rc.Width();
    m_sizeWindow.cy = rc.Height();
    m_ptWindow.x = rc.left;
    m_ptWindow.y = rc.top;
}


// ģ����ק�����������
void  LayeredWindowWrap::OnLButtonDown(UINT nHitTest)
{
    OnEnterSizeMove(nHitTest);
}
void  LayeredWindowWrap::OnLButtonUp(UINT nFlags, POINT point)
{
    OnExitSizeMove();
}
void  LayeredWindowWrap::OnMouseMove(UINT nFlags, POINT point)
{
    if (0 == m_nHitTestFlag)
        return;

    POINT ptCursor;
    GetCursorPos(&ptCursor);

    int nxMoveDiff = 0;
    int nyMoveDiff = 0;

    int oldCX = m_sizeWindow.cx;
    int oldCY = m_sizeWindow.cy;

    // ���㴰�ڵ������� (ע����������/������ʱ���������������/�ߣ����ڼ�������ʱ��Ҫ��������/�������㣬���������λ��)
    switch(m_nHitTestFlag)
    {
    case HTLEFT:
        nxMoveDiff = m_ptStartSizeMove.x - ptCursor.x;
        m_sizeWindow.cx = m_sizeWindowOld.cx + nxMoveDiff;

        if (m_pWindow->m_lMaxWidth != NDEF && m_sizeWindow.cx > m_pWindow->m_lMaxWidth)
            m_sizeWindow.cx = m_pWindow->m_lMaxWidth;
        if (m_pWindow->m_lMinWidth != NDEF && m_sizeWindow.cx < m_pWindow->m_lMinWidth)
            m_sizeWindow.cx = m_pWindow->m_lMinWidth;

        m_ptWindow.x = m_ptWindowOld.x + m_sizeWindowOld.cx - m_sizeWindow.cx;

        break;

    case HTRIGHT:
        nxMoveDiff = ptCursor.x - m_ptStartSizeMove.x;
        m_sizeWindow.cx = m_sizeWindowOld.cx + nxMoveDiff;
        break;

    case HTTOP:
        nyMoveDiff = m_ptStartSizeMove.y - ptCursor.y;
        m_sizeWindow.cy = m_sizeWindowOld.cy + nyMoveDiff;

        if (m_pWindow->m_lMaxHeight != NDEF && m_sizeWindow.cy > m_pWindow->m_lMaxHeight)
            m_sizeWindow.cy = m_pWindow->m_lMaxHeight;
        if (m_pWindow->m_lMinHeight != NDEF && m_sizeWindow.cy < m_pWindow->m_lMinHeight)
            m_sizeWindow.cy = m_pWindow->m_lMinHeight;

        m_ptWindow.y = m_ptWindowOld.y+m_sizeWindowOld.cy - m_sizeWindow.cy;

        break;

    case HTBOTTOM:
        nyMoveDiff = ptCursor.y - m_ptStartSizeMove.y;
        m_sizeWindow.cy = m_sizeWindowOld.cy + nyMoveDiff;
        break;

    case HTTOPLEFT:
        nxMoveDiff = m_ptStartSizeMove.x - ptCursor.x;
        m_sizeWindow.cx = m_sizeWindowOld.cx + nxMoveDiff;

        nyMoveDiff = m_ptStartSizeMove.y - ptCursor.y;
        m_sizeWindow.cy = m_sizeWindowOld.cy + nyMoveDiff;

        if (m_pWindow->m_lMaxWidth != NDEF && m_sizeWindow.cx > m_pWindow->m_lMaxWidth)
            m_sizeWindow.cx = m_pWindow->m_lMaxWidth;
        if (m_pWindow->m_lMaxHeight != NDEF && m_sizeWindow.cy > m_pWindow->m_lMaxHeight)
            m_sizeWindow.cy = m_pWindow->m_lMaxHeight;
        if (m_pWindow->m_lMinWidth != NDEF && m_sizeWindow.cx < m_pWindow->m_lMinWidth)
            m_sizeWindow.cx = m_pWindow->m_lMinWidth;
        if (m_pWindow->m_lMinHeight != NDEF && m_sizeWindow.cy < m_pWindow->m_lMinHeight)
            m_sizeWindow.cy = m_pWindow->m_lMinHeight;

        m_ptWindow.y = m_ptWindowOld.y+m_sizeWindowOld.cy - m_sizeWindow.cy;
        m_ptWindow.x = m_ptWindowOld.x + m_sizeWindowOld.cx - m_sizeWindow.cx;

        break;

    case HTTOPRIGHT:
        nxMoveDiff = ptCursor.x - m_ptStartSizeMove.x;
        m_sizeWindow.cx = m_sizeWindowOld.cx + nxMoveDiff;

        nyMoveDiff = m_ptStartSizeMove.y - ptCursor.y;
        m_sizeWindow.cy = m_sizeWindowOld.cy + nyMoveDiff;

        if (m_pWindow->m_lMaxHeight != NDEF && m_sizeWindow.cy > m_pWindow->m_lMaxHeight)
            m_sizeWindow.cy = m_pWindow->m_lMaxHeight;
        if (m_pWindow->m_lMinHeight != NDEF && m_sizeWindow.cy < m_pWindow->m_lMinHeight)
            m_sizeWindow.cy = m_pWindow->m_lMinHeight;

        m_ptWindow.y = m_ptWindowOld.y+m_sizeWindowOld.cy - m_sizeWindow.cy;
        break;

    case HTBOTTOMLEFT:
        nxMoveDiff = m_ptStartSizeMove.x - ptCursor.x;
        m_sizeWindow.cx = m_sizeWindowOld.cx + nxMoveDiff;

        if (m_pWindow->m_lMaxWidth != NDEF && m_sizeWindow.cx > m_pWindow->m_lMaxWidth)
            m_sizeWindow.cx = m_pWindow->m_lMaxWidth;
        if (m_pWindow->m_lMinWidth != NDEF && m_sizeWindow.cx < m_pWindow->m_lMinWidth)
            m_sizeWindow.cx = m_pWindow->m_lMinWidth;

        m_ptWindow.x = m_ptWindowOld.x + m_sizeWindowOld.cx - m_sizeWindow.cx;

        nyMoveDiff = ptCursor.y - m_ptStartSizeMove.y;
        m_sizeWindow.cy = m_sizeWindowOld.cy + nyMoveDiff;
        break;

    case HTBOTTOMRIGHT:
        nxMoveDiff = ptCursor.x - m_ptStartSizeMove.x;
        m_sizeWindow.cx = m_sizeWindowOld.cx + nxMoveDiff;

        nyMoveDiff = ptCursor.y - m_ptStartSizeMove.y;
        m_sizeWindow.cy = m_sizeWindowOld.cy + nyMoveDiff;
        break;
    }

    // ���ƴ��ڴ�С
    if (m_pWindow->m_lMaxWidth != NDEF && m_sizeWindow.cx > m_pWindow->m_lMaxWidth)
        m_sizeWindow.cx = m_pWindow->m_lMaxWidth;
    if (m_pWindow->m_lMaxHeight != NDEF && m_sizeWindow.cy > m_pWindow->m_lMaxHeight)
        m_sizeWindow.cy = m_pWindow->m_lMaxHeight;
    if (m_pWindow->m_lMinWidth != NDEF && m_sizeWindow.cx < m_pWindow->m_lMinWidth)
        m_sizeWindow.cx = m_pWindow->m_lMinWidth;
    if (m_pWindow->m_lMinHeight != NDEF && m_sizeWindow.cy < m_pWindow->m_lMinHeight)
        m_sizeWindow.cy = m_pWindow->m_lMinHeight;

    if (oldCX == m_sizeWindow.cx && oldCY == m_sizeWindow.cy)
    {
        return;
    }


    //m_pWindow->SetCanRedraw(false);  // ���汻��գ���ֹ���ڼ���object redraw,�ύ������ʱ�����ݲ�������
    //m_pWindow->CreateDoubleBuffer(m_sizeWindow.cx, m_sizeWindow.cy);
    m_pWindow->notify_WM_SIZE(0, m_sizeWindow.cx, m_sizeWindow.cy);

    // ע�⣺m_rcParent�ĸ���ǧ����ʹ��GetWindowRect����Ϊ���ڵĴ�С���ھ�û�б�
    //       ��������Ҳ��û�в���SendMessage(WM_SIZE)�ķ���
    SetRect(&m_pWindow->m_rcParent, 0,0, m_sizeWindow.cx, m_sizeWindow.cy);

    m_pWindow->SetConfigWidth(m_sizeWindow.cx);
    m_pWindow->SetConfigHeight(m_sizeWindow.cy);

    m_pWindow->UpdateLayout();

    //m_pWindow->SetCanRedraw(true);
    m_pWindow->Invalidate();

    // ģ�ⴰ�ڴ�С�ı���Ϣ
    // PS: 20130108 17:07 ����Ϣ���͵�ʱ���Ƶ�OnSize֮��
    //     ��Ҫ����Ϊ��OnSize�У��ᴥ��richedit��setcaretpos֪ͨ��Ȼ���ʱ���ڵĴ�С��û�������ı䣬
    //     �����CCaretWindow::SetCaretPos�е���MapWindowPoints��ȡ���Ĺ����Ļλ�û��Ǿɵģ����¹��
    //     ���洰���ƶ�ʧ�ܡ���˽�CCaretWindow::OnSyncWindowPosChanging�Ӻ�������������⡣
    {
        MSG  msg;
        msg.hwnd = GetHWND();
        msg.message = WM_WINDOWPOSCHANGING;
        msg.wParam = 0;

        WINDOWPOS  wndpos;
        memset(&wndpos, 0, sizeof(&wndpos));
        wndpos.hwnd = msg.hwnd;
        wndpos.flags = SWP_LAYEREDWINDOW_SIZEMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSENDCHANGING;
        wndpos.x = m_ptWindow.x;
        wndpos.y = m_ptWindow.y;
        wndpos.cx = m_sizeWindow.cx;
        wndpos.cy = m_sizeWindow.cy;
        msg.lParam = (LPARAM)&wndpos;
        ::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
    }
    UpdateLayeredCaptionWindowRgn();
}
void  LayeredWindowWrap::OnEnterSizeMove(UINT nHitTest)
{
	HWND hWnd = GetHWND();

    SendMessage(hWnd, WM_ENTERSIZEMOVE, 0, 0);
    SetCapture(hWnd);
    m_nHitTestFlag = nHitTest;

    POINT ptWindow;
    GetCursorPos(&ptWindow);

    m_ptStartSizeMove.x = ptWindow.x;
    m_ptStartSizeMove.y = ptWindow.y;

    m_ptWindowOld.x = m_ptWindow.x;
    m_ptWindowOld.y = m_ptWindow.y;
    m_sizeWindowOld.cx = m_sizeWindow.cx;
    m_sizeWindowOld.cy = m_sizeWindow.cy;
}

void  LayeredWindowWrap::OnExitSizeMove()
{
    if (0 == m_nHitTestFlag)
        return;

    if (GetHWND() == GetCapture())
        ReleaseCapture();

    m_nHitTestFlag = 0;
    m_ptStartSizeMove.x = 0;
    m_ptStartSizeMove.y = 0;

    m_ptWindowOld.x = 0;
    m_ptWindowOld.y = 0;
    m_sizeWindowOld.cx = 0;
    m_sizeWindowOld.cy = 0;
    SendMessage(GetHWND(), WM_EXITSIZEMOVE, 0, 0);
}

// ��������С���˵�ʱ������ٴ���ԭpoint/size, Commit���ᵼ�´����ָֻ�����С��֮ǰ��λ��
// �������һ����־�����������С���ˣ����޸Ĵ���λ�ã�����ԭ���ڴ�С�����ύ
bool  LayeredWindowWrap::IsMinimized()
{
    if (m_ptWindow.x == WINDOWS_MINIMIZED_POINT && m_ptWindow.y == WINDOWS_MINIMIZED_POINT)
        return true;

    return false;
};

bool LayeredWindowWrap::Commit()
{
    if (!m_pWindow)
        return false;

	Layer*  pLayer = m_pWindow->GetLayer();
	IRenderTarget* pRenderTarget = pLayer->GetRenderTarget();

	// 	RECT  rcOffset;
	// 	pLayer->GetRectDrawInBuffer(&rcOffset);

	// ��Ҫ��Ϊ�˷�ֹ�ڷֲ㴰�ڴ�С�ı�ʱ����Ҫ���´������棬
	// �ڻ�������������һ��֮ǰ��ֹ�ύ��������
	if (!m_pWindow->CanRedraw())
		return true;

	// TBD: ���ڻ����ɼ���ComboBox��listbox������������invalidate�������ᵼ��update layered windowʧ��
	// ��������ô����
	if (m_sizeWindow.cx <= 0 ||
		m_sizeWindow.cy <= 0)
		return true;

	//POINT ptMemDC  = {rcOffset.left, rcOffset.top};
	POINT ptMemDC = { 0 };

    int   nFlag = ULW_ALPHA/*ULW_OPAQUE*/;
    DWORD dwColorMask = 0;

    BLENDFUNCTION bf;
    bf.BlendOp     = AC_SRC_OVER ;
    bf.AlphaFormat = AC_SRC_ALPHA;        // AC_SRC_ALPHA �ᵼ�´��ڱ�§��,AC_SRC_OVER��ʹ��͸��
    bf.BlendFlags  = 0;                   // Must be zero. 
	bf.SourceConstantAlpha = (byte)m_pWindow->GetLayer()->GetOpacity();         // 0~255 ͸����
    // 
    // 	if (bf.SourceConstantAlpha != 255)
    // 	{
    // 		nFlag &= ~ULW_OPAQUE;
    // 		nFlag |= ULW_ALPHA;
    // 	}
    // [ע]����xp��win7��ʹ���������������£�COLOR�������⣬��˽���ȫ��ʹ��PNG��ʵ��͸��
    // 	else if (m_pWindow->m_eCurrentTransparentMode & WINDOW_TRANSPARENT_MODE_MASKCOLOR) 
    // 	{
    // 		if (m_pWindow->m_pColMask)
    // 		{
    // 			dwColorMask = m_pWindow->m_pColMask->m_col;
    // 			nFlag = ULW_COLORKEY;
    // 		}
    // 	}

    // 1. ��С����ҲҪˢ�£������ԭ֮��ᷢ�ֻ��治�����ˡ���Ϊ����ʾ��������һ˲�䣬������С��ǰ�Ļ���
    // 2. ��m_sizeWindow����memDC��Сʱ���ᵼ�»�ͼʧ�ܣ�����ֱ���Ե�ǰ��������һ���ز�0 alpha����ƽ�̣�ƽ�����ΪmemDC��С��������ΪsizeWindow��С
    BOOL  bRet = ::UpdateLayeredWindow(
        GetHWND(),
		NULL, 
        IsMinimized() ? NULL : &m_ptWindow, 
        &m_sizeWindow, 
        pRenderTarget->GetHDC(), // TODO: ����d2d�ύ��ʽ
        &ptMemDC, dwColorMask, &bf, nFlag); 
	UIASSERT(bRet);

    return true;
}


//
// ���ڷֲ㴰�ڣ�����ʾ��ʱ����Ҫ���¸���һ�Ρ���Ϊ���ֲ㴰�����ص�ʱ���ڴ�λͼû�еõ����£�invisible false��ֱ��return��
//
// ע��������û��ʹ����ӦWM_SHOWWINDOW��ͨ���ֲ㴰��ˢ�£���Ϊ����ӦWM_SHOWWINDOW��ʱ��IsWindowVisible����FALSE
//     ��˸���OnWindowPosChanged���õ�������ʾ��ʱ����ͨ���ֲ㴰��ˢ��
//   
// LRESULT LayeredWindowWrap::_OnWindowPosChanging( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
// {
//     bHandled = FALSE;
//     LPWINDOWPOS lpWndPos = (LPWINDOWPOS)lParam;
// 
//     if (m_pLayeredWindowWrap)
//     {
//         m_pLayeredWindowWrap->OnWindowPosChanging(lpWndPos);
//     }
//     return 0;
// }
// void  LayeredWindowWrap::OnWindowPosChanging(LPWINDOWPOS lpWndPos)
// {
//     // 	if (lpWndPos->flags & SWP_SHOWWINDOW)  // ������ʾ����������ʱ��DrawObject��ʧ�ܣ�
//     // 	{
//     // 		this->RedrawObject(m_pWindow, TRUE);
//     // 	}
// }

//
//	֪ͨ�ֲ㴰���µ�λ�úʹ�С
//
LRESULT LayeredWindowWrap::_OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	LPWINDOWPOS lpWndPos = (LPWINDOWPOS)lParam;
	OnWindowPosChanged(lpWndPos);

	return 0;
}

LRESULT  LayeredWindowWrap::_OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	UpdateLayeredCaptionWindowRgn();
    return 0;
}

void  LayeredWindowWrap::OnWindowPosChanged(LPWINDOWPOS lpWndPos)
{
    // ��С���ˣ�������һ��m_ptWindow�����ں����ж��Ƿ�����С�������޸�m_sizeWindow�����ô���
    if (lpWndPos->x == WINDOWS_MINIMIZED_POINT && lpWndPos->y == WINDOWS_MINIMIZED_POINT)
    {
        m_ptWindow.x = WINDOWS_MINIMIZED_POINT;
        m_ptWindow.y = WINDOWS_MINIMIZED_POINT;
    }
    else
    {
        // �����С�Ǵ��ڵĴ�С�����ǿͻ����Ĵ�С����˱������µ���getclientrect����ȡ
//         if (!(lpWndPos->flags & SWP_NOMOVE))
//         {
//             m_ptWindow.x = lpWndPos->x;
//             m_ptWindow.y = lpWndPos->y;
//         }
//         if (!(lpWndPos->flags & SWP_NOSIZE))
//         {
//             m_sizeWindow.cx = lpWndPos->cx;
//             m_sizeWindow.cy = lpWndPos->cy;
//         }

        if ((!(lpWndPos->flags & SWP_NOMOVE)) || !(lpWndPos->flags & SWP_NOSIZE))
        {
            GetWindowPos();
        }

        if (lpWndPos->flags & SWP_SHOWWINDOW)  // ������ʾ����������ʱ��DrawObject��ʧ�ܣ�
        {
            ::InvalidateRect(GetHWND(), NULL, TRUE);
        }
    }
}

LRESULT LayeredWindowWrap::_OnCancelMode( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    bHandled = FALSE;
    OnExitSizeMove();
    return 0;
}

// �ֲ㴰�ڲ�ʹ��NC������ק���ƣ���ƽ��
LRESULT  LayeredWindowWrap::_OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return HTCLIENT;
}
LRESULT  LayeredWindowWrap::_OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    POINT pt;
    ::GetCursorPos(&pt);
    ::MapWindowPoints(NULL, GetHWND(), &pt,1);

    UINT nHitTest = m_pWindow->OnHitTest(&pt, NULL);
    if (nHitTest != HTCLIENT)
    {
        m_pWindow->SetCursorByHitTest(nHitTest);
        return TRUE;
    }
    bHandled = FALSE;
    return 0;
}

//	
//  ����UpdateLayeredWindow֧�ָ�ƽ���Ĵ�������Ч������˶��ڷֲ㴰�ڵ�����������һ���߼�ʵ�ֵ�
//
void LayeredWindowWrap::OnLButtonDown(UINT nFlags, POINT pt)
{
    UINT nHitTest = m_pWindow->OnHitTest(&pt, NULL);

    switch(nHitTest)
    {
    case HTTOPLEFT:
    case HTTOP:
    case HTTOPRIGHT:
    case HTLEFT:
    case HTRIGHT:
    case HTBOTTOMLEFT:
    case HTBOTTOM:
    case HTBOTTOMRIGHT:
        {
            OnLButtonDown(nHitTest);
            SetMsgHandled(TRUE);
        }
        break;

    case HTCAPTION:
        ::PostMessage(GetHWND(), WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pt.x,pt.y) );
        break;

    default:
        {
            SetMsgHandled(FALSE);
        }
        break;
    }
}

void  LayeredWindowWrap::Enable(bool b)
{
	HWND hWnd = GetHWND();
	UIASSERT(hWnd);

    if (b)
    {
        // ʹ�÷ֲ㴰�ڵĻ����Ͳ�Ҫ����ʾ������Ӱ�ˡ����������˵�����Ĭ���Ǵ�SHADOW�ģ�ʹ�÷ֲ㴰��ʱ�Ͳ���Ҫ�����Ӱ�ˣ��Լ�ģ�⼴�ɣ�
        SetClassLong(hWnd, GCL_STYLE, GetClassLong(hWnd, GCL_STYLE)&~CS_DROPSHADOW);
        SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        UpdateLayeredCaptionWindowRgn();
        // SetWindowRgn(hWnd, NULL, FALSE);   // ȡ�����ڵ����Σ��ɷֲ㴰���Լ��������ֲ㴰�ڽ����յ�һ��WINDOWPOSCHANGED��Ϣ����SWP_NOSIZE��
        // ��˻���Ҫ���ⷢ��һ�����ڴ�С����Ϣ���߷ֲ㴰�ڵ�ǰ��С
    }
    else
    {
        // ע��ֱ��ȥ��WS_EX_LAYERED���ԣ��ᵼ�´����ȱ�ڣ�Ȼ����ˢ��һ�Ρ�
        //     ���������ȥ���ֲ�����֮��ֱ�ӽ�mem bitmap�������Ȼ��������������������⡣
        //     ͬʱΪ�˽�����������ϵ�������rgn����Ĳ��֣���ȥ���ֲ�����֮ǰ���ȼ���
        //     ���ڵ���״

        m_pWindow->UpdateWindowRgn();
        SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);

        HDC hDC = ::GetDC(hWnd);
        RECT  rc = {0,0, m_pWindow->GetWidth(), m_pWindow->GetHeight()};
        m_pWindow->DrawMemBitmap(hDC, &rc, false);
        ReleaseDC(hWnd, hDC);
    }

	// ȡ����������  TEST IT!
	if (b)
	{
		SetWindowRgn(hWnd, NULL, FALSE);
	}
}


ILayeredWindowWrap*  LayeredWindowWrap::GetILayeredWindowWrap()
{
    if (!m_pILayeredWindowWrap)
        m_pILayeredWindowWrap = new ILayeredWindowWrap(this);

    return m_pILayeredWindowWrap;
}

void  LayeredWindowWrap::UpdateRgn()
{
	// �����޸�Ϊ���ڳ�ʼ����ʱ������һ�Ρ�
//     HWND hWnd = GetHWND();
//     RECT rc;
//     GetWindowRect(hWnd, &rc);
//     ::OffsetRect(&rc, -rc.left, -rc.top);
// 
//     HRGN hRgn = CreateRectRgnIndirect(&rc);
//     SetWindowRgn(hWnd, hRgn, TRUE);
}

bool  LayeredWindowWrap::UpdateLayeredCaptionWindowRgn()
{
	HWND hWnd = GetHWND();
    if (GetWindowLong(hWnd, GWL_STYLE) & WS_CAPTION)
    {
        // SetWindowRgn(NULL)�����ã���Ȼ���б�������Բ��
        RECT rc;
        GetWindowRect(hWnd, &rc);
        ::OffsetRect(&rc, -rc.left, -rc.top);

       HRGN hRgn = CreateRectRgnIndirect(&rc);
       SetWindowRgn(hWnd, hRgn, TRUE);
        // SAFE_DELETE_GDIOBJECT(hRgn);   // TODO: MSDN�ϱ�ʾ����Ҫ���ٸö���
       return true;
    }

    return false;
}
}