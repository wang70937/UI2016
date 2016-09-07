#include "stdafx.h"
#include "dragfeedback.h"
#include <shlobj.h>

//
// ϵͳ�Դ�����קͼ��ȱ�ݽ϶࣬�Լ�ʵ��һ��
// 1. �Ƴ����������أ����봰��ʱ��ʾ�������˸
// 2. ͸��ɫ���ƣ�������һЩĪ�����������
// 3. ��С���ƣ�С��ĳ��ֵʱ������ʾ
// 4. ��win7����Ч
//

namespace UI
{
CDragFeedback*  g_pCurDragFeedback = NULL;

CDragFeedback::CDragFeedback()
{
    g_pCurDragFeedback = this;
    m_hHook = NULL;
    m_hImageWnd = NULL;
    m_hTextWnd = NULL;
    m_ptOffset.x = m_ptOffset.y = 0;
    m_hFont = NULL;
	m_hIcon = NULL;
}
CDragFeedback::~CDragFeedback()
{
    g_pCurDragFeedback = NULL;
    if (m_hFont)
    {
        DeleteObject(m_hFont);
        m_hFont = NULL;
    }
}

// ��drop target���ã����õ�ǰ��ʾ�ı���
// ÿ�������ק�ƶ�֪ͨ�ⲿ��Ҫ��������
// �ڽ�����������GiveFeedback����Ⱦ���ı�
void  CDragFeedback::SetDescription(HBITMAP hIcon, LPCWSTR szText)
{
    if (szText)
        m_strDescription = szText;
    else
        m_strDescription.clear();

	m_hIcon = hIcon;
}

// ��drop source���ã���ʾ�����ʽ����ʱ���������ı�
HRESULT  CDragFeedback::GiveFeedback(DWORD dwEffect)
{
    if (m_strPrevDescription != m_strDescription)
    {
        POINT pt;
        ::GetCursorPos(&pt);
        UpdateTextWnd(m_hIcon, m_strDescription.c_str(), pt.x+14, pt.y+16);
    }

    // ����֮��������գ���һ���������Ƶ��հ״�
    m_strPrevDescription = m_strDescription;
    m_strDescription.clear();
	m_hIcon = NULL;

    if (dwEffect == DROPEFFECT_NONE ||
        dwEffect == DROPEFFECT_SCROLL)
    {
        return DRAGDROP_S_USEDEFAULTCURSORS;
    }
    else if (dwEffect == DROPEFFECT_NOTREADY)
    {
        static HCURSOR hCursorWait = LoadCursor(NULL, IDC_WAIT);
        SetCursor(hCursorWait);
        return S_OK;
    }
    else if (m_strPrevDescription.length())
    {
        // ��ͨ��� �� �ı�����
        static HCURSOR hCursorArrow = LoadCursor(NULL, IDC_ARROW);
        SetCursor(hCursorArrow);
        return S_OK;
    }
    else
    {
        return DRAGDROP_S_USEDEFAULTCURSORS;
    }
}

void  CDragFeedback::Create(HBITMAP hDragImage, POINT ptOffset)
{
    POINT pt = {0};
    GetCursorPos(&pt);
    pt.x -= ptOffset.x;
    pt.y -= ptOffset.y;

    m_ptOffset = ptOffset;
    m_hImageWnd = CreateDragImageWnd(hDragImage, pt.x, pt.y);
    InstallDragMouseHook();
}

void  CDragFeedback::Destroy()
{
    UninstallDragMouseHook();
    if (m_hTextWnd)
    {
        DestroyWindow(m_hTextWnd);
        m_hTextWnd = NULL;
    }
    DestroyWindow(m_hImageWnd);
    m_hImageWnd = NULL;
}

void  CDragFeedback::UpdateTextWnd(
		HBITMAP hIcon, LPCWSTR szText, int x, int y)
{
    if (!szText || !szText[0])
    {
        if (m_hTextWnd)
        {
            ShowWindow(m_hTextWnd, SW_HIDE);
            return;
        }
        return;
    }

    // ��������
    if (!m_hTextWnd)
    {
        if (!m_hImageWnd)
            return;

        m_hTextWnd = CreateWindowEx(
            WS_EX_LAYERED|WS_EX_TRANSPARENT|WS_EX_TOPMOST|WS_EX_TOOLWINDOW, 
            WND_DRAGBITMAPWND_CLASS_NAME,
            NULL, 
            WS_POPUP,
            0, 
            0,
            1, 
            1,
            m_hImageWnd,
            NULL, NULL, NULL);

        if (!m_hTextWnd)
            return;
    }

    if (!m_hFont)
    {
        m_hFont = CreateFont(-12, 0, 0, 0, FW_NORMAL,0,0,0,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS,
				TEXT("΢���ź�"));
        if (!m_hFont)
            return;
    }

	if (m_imageBkg.IsNull())
	{
		HMODULE  hModule = GetModuleHandle(TEXT("UISDK.dll"));
		if (hModule)
		{
#ifndef IDB_PNG_TOOLTIP_BKG  
#define IDB_PNG_TOOLTIP_BKG  167
#endif
			m_imageBkg.LoadFromResource(hModule, 
					IDB_PNG_TOOLTIP_BKG, TEXT("PNG"));
		}
	}
    // ����
    HDC hDC = CreateCompatibleDC(NULL);
    HFONT hOldFont = (HFONT)SelectObject(hDC, m_hFont);
    SIZE sizeText = {0};
    GetTextExtentPoint32(hDC, szText, wcslen(szText), &sizeText);
    SelectObject(hDC, hOldFont);
    DeleteDC(hDC);

	SIZE sizeWnd = sizeText;

	const int icon_text_margin = 3;
	UINT nIconW = 0;
	UINT nIconH = 0;
	if (hIcon)
	{	
		BITMAP  bm = {0};
		GetObject(hIcon, sizeof(bm), &bm);
		nIconW = bm.bmWidth;
		nIconH = bm.bmHeight;

		sizeWnd.cx += nIconW + icon_text_margin;

// 		if (sizeWnd.cy < nIconH)
// 			sizeWnd.cy = nIconH;
	}

    
    REGION4  padding = {6,6,10,6};
    sizeWnd.cx += padding.left + padding.right;
    sizeWnd.cy += padding.top + padding.bottom;
    
    const UINT maxWidth = 300;
    if (sizeWnd.cx > maxWidth)
        sizeWnd.cx = maxWidth;

    UI::Image image;
    image.Create(sizeWnd.cx, -sizeWnd.cy, 32, 
		UI::Image::createAlphaChannel);
    hDC = image.GetDC();
    
    // ����
	if (m_imageBkg.IsNull())
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(118,118,118));
		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen); 
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC,
				(HBRUSH)GetStockObject(WHITE_BRUSH));
		Rectangle(hDC, 0, 0, sizeWnd.cx, sizeWnd.cy);
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);
		DeleteObject(hPen);
	}
	else
	{
		C9Region r;
		r.Set(
			(short)padding.left,
			(short)padding.top,
			(short)padding.right,
			(short)padding.bottom);

		m_imageBkg.Draw(hDC, 0, 0, sizeWnd.cx, sizeWnd.cy,
			0, 0, m_imageBkg.GetWidth(), m_imageBkg.GetHeight(),
			&r, true, 255);
	}

    // ���� 
    SetBkMode(hDC, TRANSPARENT);
    RECT rcText = {0, 0, sizeWnd.cx, sizeWnd.cy};
    rcText.left += padding.left;
    rcText.top += padding.top;
    rcText.right -= padding.right;
    rcText.bottom -= padding.bottom;

	if (hIcon)
	{
		int x = padding.left;
		int y = rcText.top + (rcText.bottom-rcText.top - (int)nIconH)/2;

		HDC hDCMem = CreateCompatibleDC(hDC);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDCMem, hIcon);
		BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		AlphaBlend(hDC, x, y, nIconW, nIconH,
			hDCMem, 0, 0, nIconW, nIconH, bf);
		SelectObject(hDCMem, hOldBitmap);
		DeleteDC(hDCMem);
		
		rcText.left += nIconW + icon_text_margin;
	}

    hOldFont = (HFONT)SelectObject(hDC, m_hFont);
    ::SetTextColor(hDC, RGB(36,36,36));
    ::DrawText(hDC, szText, -1, &rcText, 
			DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
    SelectObject(hDC, hOldFont);

    // ��alpha --> 255
    byte* pBits = (byte*)image.GetBits() + rcText.top * image.GetPitch();
    for (int h = rcText.top; h < rcText.bottom; h++)
    {
        for (int x = rcText.left; x < rcText.right; x++)
            pBits[x*4 +3] = 255;

        pBits += image.GetPitch();
    }

    // �ύ����Ļ
    POINT ptSrc = {0, 0};
    POINT ptDst = {x, y};
    BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
    UpdateLayeredWindow(m_hTextWnd, NULL, 
			&ptDst, &sizeWnd, hDC,
			&ptSrc, 0, &bf, ULW_ALPHA);
    
    image.ReleaseDC();
    ShowWindow(m_hTextWnd, SW_SHOWNOACTIVATE);
}

HWND  CDragFeedback::CreateDragImageWnd(HBITMAP hBitmap, int x, int y)
{
	if (!hBitmap)
		return NULL;

    RegisterDragBitmapWndClass();

	BITMAP bm = {sizeof(bm)};
	GetObject(hBitmap, sizeof(bm), &bm);

	POINT ptDst = {x, y};
	SIZE  sizeDst = {bm.bmWidth, bm.bmHeight};

	HWND hWnd = CreateWindowEx(
		WS_EX_LAYERED|WS_EX_TRANSPARENT|WS_EX_TOPMOST|WS_EX_TOOLWINDOW, 
		WND_DRAGBITMAPWND_CLASS_NAME,
		NULL, 
		WS_POPUP,
		ptDst.x, 
		ptDst.y,
		sizeDst.cx, 
		sizeDst.cy,
		NULL, NULL, NULL, NULL);
	if (!hWnd)
		return NULL;

	ShowWindow(hWnd, SW_SHOWNOACTIVATE);

	POINT ptSrc = {0, 0};
	HDC hDC = CreateCompatibleDC(NULL);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDC, hBitmap);
	BLENDFUNCTION bf = {AC_SRC_OVER, 0, 180, AC_SRC_ALPHA};
	UpdateLayeredWindow(hWnd, NULL, 
			&ptDst, &sizeDst, hDC, 
			&ptSrc, 0, &bf, ULW_ALPHA);
	SelectObject(hDC, hOldBitmap);
	DeleteDC(hDC);

	
	return hWnd;
}

LRESULT CALLBACK MouseProc(
	int nCode,
    WPARAM wParam,
    LPARAM lParam
)
{
    if (!g_pCurDragFeedback)
        return 0;

    HHOOK hHook = g_pCurDragFeedback->m_hHook;

	if (nCode < 0)
		return CallNextHookEx(hHook, nCode, wParam, lParam);

	if (wParam == WM_MOUSEMOVE || wParam == WM_NCMOUSEMOVE)
	{
		MOUSEHOOKSTRUCT* pStruct = (MOUSEHOOKSTRUCT*)lParam;
        g_pCurDragFeedback->OnMouseMove(pStruct->pt);
	}
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void  CDragFeedback::OnMouseMove(POINT pt)
{
    if (!m_hImageWnd)
        return;

    SetWindowPos(m_hImageWnd, 
        0,
        pt.x - m_ptOffset.x,
        pt.y - m_ptOffset.y,
        0, 0, 
        SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

    if (m_hTextWnd && IsWindowVisible(m_hTextWnd))
    {
        SetWindowPos(m_hTextWnd, 
            0,
            pt.x+14,
            pt.y+16,
            0, 0, 
            SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);
    }
}

BOOL  CDragFeedback::InstallDragMouseHook()
{
	if (m_hHook)
		return TRUE;

	m_hHook = SetWindowsHookEx(
			WH_MOUSE, MouseProc, NULL, GetCurrentThreadId());
	if (!m_hHook)
		return FALSE;

	return TRUE;
}

void  CDragFeedback::UninstallDragMouseHook()
{
	if (m_hHook)
	{
		UnhookWindowsHookEx(m_hHook);
		m_hHook = NULL;
	}
}

void  CDragFeedback::RegisterDragBitmapWndClass()
{
    static bool s_bRegiste = false;
    if (s_bRegiste)
        return;

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_DBLCLKS;  
    wcex.lpfnWndProc	= DefWindowProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= 0;
    wcex.hIcon			= 0;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1); 
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= WND_DRAGBITMAPWND_CLASS_NAME;
    wcex.hIconSm		= 0;
    RegisterClassEx(&wcex);

    s_bRegiste = true;
}

// ��feedback����dataobject���棬������droptarget�Ļص��в���
// ͨ��IDataObject�õ�idragfeedbac�ӿ�
void  CDragFeedback::Bind2DataObject(IDataObject* pDataObject)
{
    if (!pDataObject)
        return;

    IDataObjectEx* pDataObjectEx = NULL;
    pDataObject->QueryInterface(IID_IDataObjectEx, (void**)&pDataObjectEx);
    if (pDataObjectEx)
    {
        pDataObjectEx->SetDragFeedback(this);
        pDataObjectEx->Release();
    }
}
void  CDragFeedback::Unbind2DataObject(IDataObject* pDataObject)
{
    if (!pDataObject)
        return;

    IDataObjectEx* pDataObjectEx = NULL;
    pDataObject->QueryInterface(IID_IDataObjectEx, (void**)&pDataObjectEx);
    if (pDataObjectEx)
    {
        if (pDataObjectEx->GetDragFeedback() == this)
            pDataObjectEx->SetDragFeedback(NULL);

        pDataObjectEx->Release();
    }
}
void  CDragFeedback::Bind2DropSource(IDropSource* pDropSource)
{
    if (!pDropSource)
        return;

    IDropSourceEx* pDropSourceEx = NULL;
    pDropSource->QueryInterface(IID_IDropSourceEx, (void**)&pDropSourceEx);
    if (pDropSourceEx)
    {
        pDropSourceEx->SetDragFeedback(this);
        pDropSourceEx->Release();
    }
}
void  CDragFeedback::Unbind2DropSource(IDropSource* pDropSource)
{
    if (!pDropSource)
        return;

    IDropSourceEx* pDropSourceEx = NULL;
    pDropSource->QueryInterface(IID_IDropSourceEx, (void**)&pDropSourceEx);
    if (pDropSourceEx)
    {
        if (pDropSourceEx->GetDragFeedback() == this)
            pDropSourceEx->SetDragFeedback(NULL);

        pDropSourceEx->Release();
    }
}

LRESULT _UIDoDragDrop(
		IDataObject* pDataObject, 
		IDropSource* pDropSource, 
		DWORD dwOKEffect,         
		DWORD* pdwEffect,        
		HBITMAP hbmpDragImage,
		POINT ptOffset)
{
    CDragFeedback  feedback;
    feedback.Create(hbmpDragImage, ptOffset);
    feedback.Bind2DataObject(pDataObject);
    feedback.Bind2DropSource(pDropSource);
    
    LRESULT lRet = DoDragDrop(
			pDataObject, pDropSource, dwOKEffect, pdwEffect);

    feedback.Unbind2DataObject(pDataObject);
    feedback.Unbind2DropSource(pDropSource);
    feedback.Destroy();
    return lRet;
}
}