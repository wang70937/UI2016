#include "stdafx.h"
#include "colorwrap.h"
#include "Src/UIObject/Window/customwindow.h"
#include "Inc\Interface\iuires.h"
#include "Inc\Interface\imapattr.h"
#include "Src\Base\Application\uiapplication.h"
#include "Src\Resource\colorres.h"

namespace UI
{

ColorMaskWindowWrap::ColorMaskWindowWrap()
{
    m_pColMask = NULL;
}
ColorMaskWindowWrap::~ColorMaskWindowWrap()
{
    SAFE_RELEASE(m_pColMask);
}

void  ColorMaskWindowWrap::Init(ICustomWindow* pWnd)
{
    __super::Init(pWnd);

    IMapAttribute* pMapAttrib = NULL;
    m_pWindow->GetMapAttribute(&pMapAttrib);
    if (pMapAttrib)
    {
        const TCHAR* szText = pMapAttrib->GetAttr(XML_WINDOW_TRANSPARENT_MASKCOLOR_9REGION, false);
        Util::TranslateImage9Region(szText, &m_9region, XML_SEPARATOR);

        szText = pMapAttrib->GetAttr(XML_WINDOW_TRANSPARENT_MASKCOLOR_VALUE, false);
        if (szText)
        {
            UIApplication* pUIApp = m_pWindow->GetUIApplication();
            if (pUIApp)
            {
                ColorRes* pColorRes = pUIApp->GetActiveSkinColorRes();
                if (pColorRes)
                {
                    SAFE_RELEASE(m_pColMask);
                    pColorRes->GetColor(szText, &m_pColMask);
                }
            }
        }

        SAFE_RELEASE(pMapAttrib);
    }
}

void  ColorMaskWindowWrap::Enable(bool b)
{
	if (false == b)  // ȡ����ǰ����
	{
		SetWindowRgn(GetHWND(), NULL, FALSE);
	}
}

void  ColorMaskWindowWrap::UpdateRgn()
{
    if (m_9region.IsAll_1())
        return;

	HWND hWnd = GetHWND();
    byte* pScan0 = NULL;
    int  nPitch = 0;
    GetWindowBitmapInfo(&pScan0, nPitch);

    vector<POINT> vecPt;
    RECT rc = {0};

    CRect rcWnd;
    m_pWindow->GetParentRect(&rcWnd);

    if (m_9region.IsAll_0())
    {
        // �������ڹ���
        GetRgnData(pScan0, nPitch, &rcWnd, vecPt);
    }
    else
    {
        int nTop = abs(m_9region.top);
        int nBottom = abs(m_9region.bottom);

        if (nTop > 0)
        {
            if (m_9region.topleft)
            {
                SetRect(&rc, 0, 0, m_9region.topleft, nTop);
                GetRgnData(pScan0, nPitch, &rc, vecPt);
            }

            if (m_9region.topright)
            {
                SetRect(&rc, rcWnd.right-m_9region.topright, 0, rcWnd.right, nTop);
                GetRgnData(pScan0, nPitch, &rc, vecPt);
            }

            if (m_9region.top > 0)
            {
                SetRect(&rc, m_9region.topleft, 0, rcWnd.right-m_9region.topright, nTop);
                GetRgnData(pScan0, nPitch, &rc, vecPt);
            }
        }

        if (m_9region.left > 0)
        {
            SetRect(&rc, 0, nTop, m_9region.left, rcWnd.bottom-nBottom);
            GetRgnData(pScan0, nPitch, &rc, vecPt);
        }
        if (m_9region.right > 0)
        {
            SetRect(&rc, rcWnd.right-m_9region.right, nTop, rcWnd.right, rcWnd.bottom-nBottom);
            GetRgnData(pScan0, nPitch, &rc, vecPt);
        }

        if (nBottom > 0)
        {
            if (m_9region.bottomleft)
            {
                SetRect(&rc, 0, rcWnd.bottom-nBottom, m_9region.bottomleft, rcWnd.bottom);
                GetRgnData(pScan0, nPitch, &rc, vecPt);
            }
			if (m_9region.bottomright)
			{
				SetRect(&rc, rcWnd.right-m_9region.bottomright, rcWnd.bottom-nBottom, rcWnd.right, rcWnd.bottom);
				GetRgnData(pScan0, nPitch, &rc, vecPt);
			}

            if (m_9region.bottom > 0)
            {
				SetRect(&rc, m_9region.bottomleft, rcWnd.bottom-nBottom, rcWnd.right-m_9region.bottomright, rcWnd.bottom);
				GetRgnData(pScan0, nPitch, &rc, vecPt);
            }
        }
    }

    HRGN hRgnExclude = CreateRgnByVectorPoint(vecPt);
    if (!hRgnExclude)
        return;

    HRGN hRgnWindow = ::CreateRectRgn(0,0,m_pWindow->GetWidth(), m_pWindow->GetHeight());
    ::CombineRgn(hRgnWindow, hRgnWindow, hRgnExclude,  RGN_DIFF);

    // �������Ĳ�����ȥˢ�µĻ����ͻ�����Ļ�ϲ�������Щ���õ���region����
    // MSDN: Typically, you set bRedraw to TRUE if the window is visible. 
    SetWindowRgn(hWnd, hRgnWindow, ::IsWindowVisible(hWnd));
    SAFE_DELETE_GDIOBJECT(hRgnExclude);
//    SAFE_DELETE_GDIOBJECT(hRgnWindow);  // TODO: MSDN�ϱ�ʾ����Ҫ���ٸö���
}


void  ColorMaskWindowWrap::GetRgnData(BYTE* pScan0, int nPitch, RECT* prc, vector<POINT>& vecPt)
{
    if(!pScan0 || !prc || !m_pColMask)
        return;

    if (::IsRectEmpty(prc))
        return;

    BYTE* p = pScan0;

    BYTE b = 0;
    BYTE g = 0;
    BYTE r = 0;
    BYTE bMask = m_pColMask->GetB();
    BYTE gMask = m_pColMask->GetG();
    BYTE rMask = m_pColMask->GetR();

    POINT pt = {0, 0};

    for (LONG i = prc->top; i < prc->bottom; i++ )   
    {
        p = pScan0 + nPitch*i + prc->left*4;
        for (LONG j = prc->left; j < prc->right; j++)      
        {
            b = p[0];
            g = p[1];
            r = p[2];
            p += 4;

            if (r == rMask && g == gMask && b == bMask)
            {
                pt.x = j;
                pt.y = i;
                vecPt.push_back(pt);
            }
        }
    }
}

}