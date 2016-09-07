#include "stdafx.h"
#include "aerowrap.h"
#include "Src\UIObject\Window\customwindow.h"
#include "Inc\Interface\imapattr.h"

namespace UI
{
AeroWindowWrap::AeroWindowWrap()
{
    m_pDWM = DwmHelper::GetInstance();
    m_pAeroDisableMode = NULL;
    m_pIAeroWindowWrap = NULL;
    m_eAeroDisableMode = WINDOW_TRANSPARENT_TYPE_LAYERED;
    m_eMode = AERO_MODE_BLUR;
    ::SetRect(&blur.m_regionBlur, -1, -1, -1, -1);
    blur.m_hrgnBlurRgn = NULL;
    blur.m_bAutoClipChildCtrl = false;
}
AeroWindowWrap::~AeroWindowWrap()
{
    m_pDWM = NULL;
    SAFE_DELETE(m_pIAeroWindowWrap);
    SAFE_RELEASE(m_pAeroDisableMode);
    SAFE_DELETE_GDIOBJECT(blur.m_hrgnBlurRgn);
}

void  AeroWindowWrap::Init(ICustomWindow* pWnd)
{
    __super::Init(pWnd);

    IMapAttribute* pMapAttrib = m_pWindow->m_pIMapAttributeRemain;
    if (pMapAttrib)
    {
        const TCHAR* szText = m_pWindow->m_pIMapAttributeRemain->GetAttr(XML_WINDOW_TRANSPARENT_TYPE_AERO_DISABLE, true);
        if (szText)
            m_eAeroDisableMode = GetTransparentModeTypeFromAttr(szText);

        szText = pMapAttrib->GetAttr(XML_WINDOW_TRANSPARENT_AERO_MODE, false);
        if (szText)
        {
            if (0 == _tcscmp(szText, XML_WINDOW_TRANSPARENT_AERO_MODE_BLUR))
            {
                m_eMode = AERO_MODE_BLUR;
            }
            else if (0 == _tcscmp(szText, XML_WINDOW_TRANSPARENT_AERO_MODE_TRANS))
            {
                m_eMode = AERO_MODE_TRANSPARENT;
                UIASSERT(GetWindowLong(GetHWND(), GWL_STYLE)&WS_CAPTION);
            }
            else
            {
                m_eMode = AERO_MODE_BLUR;
            }
        }

        if (AERO_MODE_TRANSPARENT == m_eMode)
        {
            szText = pMapAttrib->GetAttr(XML_WINDOW_TRANSPARENT_AERO_TRANS_MARGINS, false);
            if (szText)
            {
				// ע�� ������ֵ��С���ᵼ�´����ĸ�Բ�Ǳ��Σ���������Ϊ5? 7?
                RECT rc;
                if (Util::TranslateRECT(szText, &rc, XML_SEPARATOR))
                {
                    trans.m_margins.cxLeftWidth = rc.left;
                    trans.m_margins.cxRightWidth = rc.right;
                    trans.m_margins.cyTopHeight = rc.top;
                    trans.m_margins.cyBottomHeight = rc.bottom;
#ifdef _DEBUG
					UIASSERT(rc.left == -1 || rc.left >= 5);
					UIASSERT(rc.right == -1 || rc.right >= 5);
					UIASSERT(rc.top == -1 || rc.top >= 5);
					UIASSERT(rc.bottom == -1 || rc.bottom >= 5);
#endif
                }
            }
        }
        else
        {
            szText = pMapAttrib->GetAttr(XML_WINDOW_TRANSPARENT_AERO_BLUR_REGION, false);
            if (szText)
            {
                Util::TranslateRECT(szText, &blur.m_regionBlur, XML_SEPARATOR);
            }
            pMapAttrib->GetAttr_bool(XML_WINDOW_TRANSPARENT_AERO_BLUR_AUTOCLIPCHILDCTRL, false, &blur.m_bAutoClipChildCtrl);
        }
    }
}

LRESULT  AeroWindowWrap::_OnDwmCompositionChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    Enable(true);
    m_pWindow->m_bNeedToSetWindowRgn = true;
    m_pWindow->Invalidate();

    bHandled = FALSE;
    return 0;
}

IAeroWindowWrap*  AeroWindowWrap::GetIAeroWndTransMode()
{
    if (!m_pIAeroWindowWrap)
        m_pIAeroWindowWrap = new IAeroWindowWrap(this);

    return m_pIAeroWindowWrap;
}

AERO_MODE  AeroWindowWrap::GetAeroMode()
{
    return m_eMode;
}   
void  AeroWindowWrap::GetBlurRegion(CRegion4* pregion)
{
    if (pregion && m_eMode == AERO_MODE_BLUR)
    {
        pregion->CopyRect(&blur.m_regionBlur);
    }
}

void  AeroWindowWrap::SetTransparentMargins(RECT* prcMargin)
{
	if (m_eMode != AERO_MODE_TRANSPARENT)
		return;

	trans.m_margins.cxLeftWidth = prcMargin->left;
	trans.m_margins.cxRightWidth = prcMargin->right;
	trans.m_margins.cyTopHeight = prcMargin->top;
	trans.m_margins.cyBottomHeight = prcMargin->bottom;
}

void  AeroWindowWrap::Enable(bool b)
{
	HWND hWnd = GetHWND();
	UIASSERT(hWnd);

    if (m_pDWM->IsEnable())
    {
        if (b)
        {
            if (m_pAeroDisableMode)
            {
                m_pAeroDisableMode->Enable(false);
                SAFE_RELEASE(m_pAeroDisableMode);
            }

			if (m_eMode == AERO_MODE_TRANSPARENT)
			{
				// ע��ʹ�����ѵ�ͼƬ��Ӱ+aero͸���ᵼ��һ��������˵����⣺���ڸı��Сʱ���ֺڱ����⣬����ק�����ʱ
				//     Ϊ�˹��������⣬ֻ�ܲ���aero�Դ�����Ӱ����˵����һ��WS_THICKFRAME�����Կ�����Ӱ���ټ���WS_NCCALCSIZE/WS_NCACTIVATE��Ϣ
				//     ���α߿����ʾ
				long lStyle = GetWindowLong(hWnd, GWL_STYLE);
				SetWindowLong(hWnd, GWL_STYLE, lStyle|WS_THICKFRAME);
				SetWindowPos(hWnd, 0,0,0,0,0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_FRAMECHANGED);
			}
            return;
            // ���ⲿ����UpdateRgn����
        }
        else
        {
            if (m_eMode == AERO_MODE_BLUR)
            {
                DwmHelper* pDwm = DwmHelper::GetInstance();
                if (pDwm->pDwmEnableBlurBehindWindow)
                {
                    DWM_BLURBEHIND blurbehind = {0};
                    blurbehind.dwFlags = DWM_BB_ENABLE;
                    blurbehind.fEnable = FALSE;
                    pDwm->pDwmEnableBlurBehindWindow(hWnd, &blurbehind);
                }
            }
            else
            {
                if (m_pDWM->pDwmExtendFrameIntoClientArea)
                {
                    MARGINS m = {0};
                    m_pDWM->pDwmExtendFrameIntoClientArea(hWnd, &m);
                }

				if (m_eMode == AERO_MODE_TRANSPARENT)
				{
					long lStyle = GetWindowLong(hWnd, GWL_STYLE);
					if (lStyle & WS_THICKFRAME)
					{
						SetWindowLong(hWnd, GWL_STYLE, lStyle & ~WS_THICKFRAME);
						SetWindowPos(hWnd, 0,0,0,0,0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_FRAMECHANGED);
					}
				}
            }
        }
    }
    
    do 
    {
        if (m_pAeroDisableMode)
        {
            m_pAeroDisableMode->Enable(true);
            break;
        }

        m_pAeroDisableMode = CreateTransparentModeByType(m_eAeroDisableMode);
        if (!m_pAeroDisableMode)
            break;

        m_pAeroDisableMode->Init(m_pWindow->m_pICustomWindow);
        m_pAeroDisableMode->Enable(true);

    } while (0);
}


struct EnumChildWindowAeroRgnData
{
    int nCount;
    vector<CRect>  m_vec;
    HWND  m_hParent;
};

BOOL CALLBACK EnumChildWindowAeroRgn(HWND hWnd, LPARAM lParam)
{
    EnumChildWindowAeroRgnData*  pData = (EnumChildWindowAeroRgnData*)lParam;
    if (IsWindowVisible(hWnd))
    {
        if (GetParent(hWnd) == pData->m_hParent)
        {
            pData->nCount ++;
            CRect rc;
            GetWindowRect(hWnd, &rc);
            ::MapWindowPoints(NULL, pData->m_hParent, (LPPOINT)&rc, 2);

            pData->m_vec.push_back(rc);
        }
    }
    return TRUE;
}

void  AeroWindowWrap::UpdateRgn()
{
	HWND hWnd = GetHWND();
	UIASSERT(hWnd);

    if (m_pAeroDisableMode)
    {
        m_pAeroDisableMode->UpdateRgn();
        return;
    }

    DwmHelper*  pDwm = DwmHelper::GetInstance();
    if (pDwm->IsEnable())
    {
        if (m_eMode == AERO_MODE_BLUR)
        {
            if (pDwm->pDwmEnableBlurBehindWindow)
            {
                DWM_BLURBEHIND blurbehind = {0};
                blurbehind.dwFlags = DWM_BB_ENABLE|DWM_BB_BLURREGION|DWM_BB_TRANSITIONONMAXIMIZED;
                blurbehind.fEnable = TRUE;
                blurbehind.fTransitionOnMaximized = TRUE;  // ����ֶε�������ʲô�ģ�

                if (blur.m_hrgnBlurRgn)
                {
                    blurbehind.hRgnBlur = blur.m_hrgnBlurRgn;
                    pDwm->pDwmEnableBlurBehindWindow(hWnd, &blurbehind);
                }
                else if (blur.m_bAutoClipChildCtrl)
                {
                    CRect  rcClient;
                    ::GetClientRect(hWnd, &rcClient);
                    rcClient.DeflateRect(blur.m_regionBlur.left, 
                        blur.m_regionBlur.top, 
                        blur.m_regionBlur.right, 
                        blur.m_regionBlur.bottom);

                    HRGN hRgn = CreateRectRgnIndirect(&rcClient);

                    // �ٵ��Ӵ���
                    EnumChildWindowAeroRgnData  data;
                    data.m_hParent = hWnd;
                    data.nCount = 0;
                    EnumChildWindows(hWnd, EnumChildWindowAeroRgn, (LPARAM)&data);
                    if (data.nCount > 0)
                    {
                        for (int i = 0; i < data.nCount; i++)
                        {
                            HRGN hRgnChild = CreateRectRgnIndirect(&data.m_vec[i]);
                            ::CombineRgn(hRgn, hRgn, hRgnChild, RGN_DIFF);
                            DeleteObject(hRgnChild);
                        }
                    }

                    blurbehind.hRgnBlur = hRgn;
                    pDwm->pDwmEnableBlurBehindWindow(hWnd, &blurbehind);
                    DeleteObject(hRgn);
                }
                else
                {
                    CRect  rcClient;
                    ::GetClientRect(hWnd, &rcClient);
                    rcClient.DeflateRect(blur.m_regionBlur.left, 
                        blur.m_regionBlur.top, 
                        blur.m_regionBlur.right, 
                        blur.m_regionBlur.bottom);

                    HRGN hRgn = CreateRectRgnIndirect(&rcClient);

//                     RECT rc = {10, 10, 200, 500};
//                     HRGN hRgnChild = CreateRectRgnIndirect(&rc);
//                     ::CombineRgn(hRgn, hRgn, hRgnChild, RGN_DIFF);
//                     DeleteObject(hRgnChild);

                    blurbehind.hRgnBlur = hRgn;
                    pDwm->pDwmEnableBlurBehindWindow(hWnd, &blurbehind);
                    DeleteObject(hRgn);
                }
            }
        }
        else if (AERO_MODE_TRANSPARENT == m_eMode)
        {
             if (m_pDWM->pDwmExtendFrameIntoClientArea)
             {
				 m_pDWM->pDwmExtendFrameIntoClientArea(hWnd, &trans.m_margins);
             }
        }
    }
}

WINDOW_TRANSPARENT_TYPE  AeroWindowWrap::GetType()
{
    if (m_pAeroDisableMode)
        return m_pAeroDisableMode->GetType();

    return WINDOW_TRANSPARENT_TYPE_AERO;
}

bool  AeroWindowWrap::RequireAlphaChannel()
{
    if (m_pAeroDisableMode)
        return m_pAeroDisableMode->RequireAlphaChannel();

    return true;
}
bool  AeroWindowWrap::Commit()
{
    if (m_pAeroDisableMode)
    {
        return m_pAeroDisableMode->Commit();
    }

    return false;
}

}