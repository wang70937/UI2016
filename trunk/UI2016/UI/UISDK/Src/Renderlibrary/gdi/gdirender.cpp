#include "stdafx.h"
#include "gdirender.h"
#include "../renderbuffer.h"
#include "Src/UIEffect/CacheBitmap/cachebitmap.h"

GdiRenderTarget::GdiRenderTarget(bool bNeedAlphaChannel)
{
    m_bNeedAlphaChannel = bNeedAlphaChannel;
    m_hBindDC = NULL;
	m_lDrawingRef = 0;
    m_pRenderBuffer = NULL;
    m_ptOffset.x = m_ptOffset.y = 0;
}
GdiRenderTarget::~GdiRenderTarget()
{
    SAFE_DELETE(m_pRenderBuffer);
 	m_hBindDC = NULL;
}

void  GdiRenderTarget::Release()
{
    delete this;
}

// ��HDC����Ҫ�ͷ� 
HDC GdiRenderTarget::GetBindHDC()
{
	return m_hBindDC;
}

#if 0
HRGN GdiRenderTarget::GetClipRgn()
{
	HRGN hRgn = ::CreateRectRgn(0,0,0,0);
	if( 1 != ::GetClipRgn(GetHDC(), hRgn) )  // �ջ���ʧ��
	{
		::DeleteObject(hRgn);
		hRgn = NULL;
	}
	return hRgn;
}
int GdiRenderTarget::SelectClipRgn( HRGN hRgn, int nMode )
{
	if (RGN_COPY == nMode/* && NULL != hRgn*/)
	{
		return ::SelectClipRgn(GetHDC(), hRgn);
	}

	int nRet = ExtSelectClipRgn(GetHDC(), hRgn, nMode);
	return nRet;
}

int  GdiRenderTarget::SelectClipRect( RECT* prc, uint nrcCount, int nMode)
{
	if (0 == nrcCount || NULL == prc)
	{
		return SelectClipRect(NULL, nMode);
	}
	if (1 == nrcCount)
	{
		HRGN  hRgn = CreateRectRgnIndirect(prc);
		int   nRet = SelectClipRgn(hRgn);
		DeleteObject(hRgn);
		return nRet;
	}

	RGNDATA*   pRgnData      = (RGNDATA*)new BYTE[ sizeof(RGNDATAHEADER) + nrcCount*sizeof(RECT) ];
	pRgnData->rdh.dwSize     = sizeof(RGNDATAHEADER);
	pRgnData->rdh.iType      = RDH_RECTANGLES;
	pRgnData->rdh.nCount     = nrcCount;
	pRgnData->rdh.nRgnSize   = nrcCount*sizeof(RECT);
	for (uint k = 0; k < nrcCount; k++)
	{
		RECT* prcBuffer = (RECT*)pRgnData->Buffer;
		prcBuffer[k] = prc[k];
	}
	HRGN hRgn = ::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + nrcCount*sizeof(RECT), pRgnData);
	SAFE_ARRAY_DELETE(pRgnData);
	int   nRet = SelectClipRgn(hRgn);
	DeleteObject(hRgn);
	return nRet;
}

BOOL GdiRenderTarget::GetViewportOrgEx( LPPOINT lpPoint )
{
	if (NULL == lpPoint)
	{
		return FALSE;
	}

	::GetViewportOrgEx(GetHDC(), lpPoint );
	return TRUE;
}
BOOL GdiRenderTarget::SetViewportOrgEx( int x, int y, LPPOINT lpPoint ) 
{
	BOOL bRet1 = ::SetViewportOrgEx(GetHDC(), x, y, lpPoint);

#ifdef _DEBUG
    ::GetViewportOrgEx(GetHDC(), &m_ptOffset);
#endif
	return bRet1;
}
BOOL GdiRenderTarget::OffsetViewportOrgEx(int x, int y, LPPOINT lpPoint)
{
	BOOL bRet1 = ::OffsetViewportOrgEx(GetHDC(), x, y, lpPoint );

#ifdef _DEBUG
    ::GetViewportOrgEx(GetHDC(), &m_ptOffset);
#endif
	return bRet1;
}
#endif

void  GdiRenderTarget::update_clip_rgn()
{
    if (m_stackClipRect.empty())
    {
        HRGN hRgn = m_arrayMetaClipRegion.CreateRgn();
        ::SelectClipRgn(GetHDC(), hRgn);
        DeleteObject(hRgn);
        return;
    }

    RECT rcIntersect = {0, 0, 1, 1};
    
    deque<RECT>::const_iterator iter = m_stackClipRect._Get_container().begin();
    CopyRect(&rcIntersect, &(*iter));
    iter++;
    for (; iter != m_stackClipRect._Get_container().end(); ++iter)
    {
        ::IntersectRect(&rcIntersect, &rcIntersect, &(*iter));
    }

    HRGN hRgn = NULL;
    if (!m_arrayMetaClipRegion.GetCount())
    {
        hRgn = CreateRectRgnIndirect(&rcIntersect);        
    }
    else
    {
        RectArray array = m_arrayMetaClipRegion;
        if (!array.IntersectRect(&rcIntersect))
        {
            RECT rcIntersect = {0, 0, 1, 1};
            hRgn = CreateRectRgnIndirect(&rcIntersect);           
        }
        else
        {
            hRgn = array.CreateRgn();
        }
    }
    
    ::SelectClipRgn(GetHDC(), hRgn);
    DeleteObject(hRgn);
}

void  GdiRenderTarget::SetMetaClipRegion(LPRECT prc, uint nrcCount)
{
    while (!m_stackClipRect.empty())
        m_stackClipRect.pop();

    m_arrayMetaClipRegion.CopyFromArray(prc, nrcCount);

   update_clip_rgn();
}

void  GdiRenderTarget::PushRelativeClipRect(LPCRECT prc)
{
    RECT rc = {0};
    if (prc)
    {
        CopyRect(&rc, prc);
        OffsetRect(&rc, m_ptOffset.x, m_ptOffset.y);
    }

    m_stackClipRect.push(rc);

    update_clip_rgn();
}

void  GdiRenderTarget::PopRelativeClipRect() 
{
    assert (!m_stackClipRect.empty());
    m_stackClipRect.pop();

    update_clip_rgn();
}

bool  GdiRenderTarget::IsRelativeRectInClip(LPCRECT prc) 
{
    assert (prc);
    if (!prc)
        return false;

    if (m_stackClipRect.empty() && m_arrayMetaClipRegion.GetCount() == 0)
        return true;

    CRect rcTest(prc);
    rcTest.OffsetRect(m_ptOffset.x, m_ptOffset.y);

    if (!m_arrayMetaClipRegion.IntersectRect(&rcTest, true))
        return false;

    if (m_stackClipRect.empty())
        return true;

    RECT rcIntersect = {0, 0, 1, 1};

    deque<RECT>::const_iterator iter = m_stackClipRect._Get_container().begin();
    CopyRect(&rcIntersect, &(*iter));
    iter++;
    for (; iter != m_stackClipRect._Get_container().end(); ++iter)
    {
        ::IntersectRect(&rcIntersect, &rcIntersect, &(*iter));
    }
    
    if (!IntersectRect(&rcIntersect, &rcIntersect, &rcTest))
        return false;

    return true;
}

void  GdiRenderTarget::SetOrigin(int x, int y) 
{
    m_ptOffset.x = x;
    m_ptOffset.y = y;

    POINT pt = {0};
    ::SetViewportOrgEx(GetHDC(), m_ptOffset.x, m_ptOffset.y, &pt);
}

void  GdiRenderTarget::OffsetOrigin(int x, int y) 
{
    m_ptOffset.x += x;
    m_ptOffset.y += y;

    POINT pt = {0};
    ::SetViewportOrgEx(GetHDC(), m_ptOffset.x, m_ptOffset.y, &pt);
}
void  GdiRenderTarget::GetOrigin(int* px, int* py) 
{
    if (px)
        *px = m_ptOffset.x;

    if (py)
        *py = m_ptOffset.y;
}

void GdiRenderTarget::BindHDC(HDC hDC)
{
	if (NULL == hDC)
		return;

    if (m_pRenderBuffer)
    {
        SAFE_DELETE(m_pRenderBuffer);
    }
	m_hBindDC = hDC;
}

bool  GdiRenderTarget::CreateRenderBuffer(IRenderTarget*  pSrcRT)
{
    if (m_pRenderBuffer)
        return false;

    m_pRenderBuffer = new RenderBuffer;
    m_pRenderBuffer->m_eGraphicsType = GRAPHICS_RENDER_LIBRARY_TYPE_GDI;
    m_hBindDC = NULL;

    return true;
}

bool  GdiRenderTarget::ResizeRenderBuffer(unsigned int nWidth, unsigned int nHeight)
{
    if (!m_pRenderBuffer)
        CreateRenderBuffer(NULL);

    m_pRenderBuffer->Resize(nWidth, nHeight);
    return true;
}

void  GdiRenderTarget::GetRenderBufferData(ImageData*  pData)
{
	if (!m_pRenderBuffer)
		return;
	m_pRenderBuffer->GetImageData(pData);
}

HDC   GdiRenderTarget::GetHDC()
{
    if (m_hBindDC)
        return m_hBindDC;
    
    if (m_pRenderBuffer)
        return m_pRenderBuffer->m_hDC;

    UIASSERT(0);
    return NULL;
}
void  GdiRenderTarget::ReleaseHDC(HDC hDC)
{

}

//
// �����Ҫͬʱ��������item�������ṩ����RECT���вü�
//
// bClear��Ҫ�����ڷֲ㴰���е�͸������
//
bool GdiRenderTarget::BeginDraw()
{
    UIASSERT (0 == m_lDrawingRef);
    m_lDrawingRef ++;
    return true;
}

void GdiRenderTarget::EndDraw()
{
    -- m_lDrawingRef;
	if (m_lDrawingRef==0)
	{
		this->SetOrigin(0,0);
		this->SetMetaClipRegion(NULL, 0);
	}
}

void GdiRenderTarget::Clear(RECT* prc)
{
    HDC hDC = GetHDC();

    HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    if (prc)
    {
        ::FillRect(hDC, prc, hBrush);
    }
    else
    {
        BITMAP  bm;
        HBITMAP hBitmap = (HBITMAP)::GetCurrentObject(hDC, OBJ_BITMAP);
        if (NULL == hBitmap)
            return;

        ::GetObject(hBitmap, sizeof(bm), &bm);
        RECT  rc = {0,0, bm.bmWidth, bm.bmHeight};
        ::FillRect(hDC, &rc, hBrush);
    }
}

void  GdiRenderTarget::DrawString(IRenderFont* pRenderFont, DRAWTEXTPARAM* pParam)
{
    if (NULL == pParam || NULL == pRenderFont)
        return;

	if (NULL == pParam->prc || RECTW(pParam->prc) <= 0 || RECTH(pParam->prc) <= 0)
		return;

    CRect rcText(pParam->prc);

    // ���еĴ�ֱ���У�DrawText��֧�֣������������λ��
    if ((pParam->nFormatFlag & DT_VCENTER))
    {
		if ((pParam->nFormatFlag & DT_WORDBREAK))
		{
			SIZE  sizeText = pRenderFont->MeasureString(pParam->szText, rcText.Width());

			if (sizeText.cy < rcText.Height())
			{
				rcText.top = rcText.top + (rcText.Height()-sizeText.cy)/2;
				rcText.bottom = rcText.top + sizeText.cy;
			}
		}
		else
		{
			pParam->nFormatFlag |= DT_SINGLELINE; // vcenter������singlineһ��ʹ��
		}
    }

    GRAPHICS_RENDER_LIBRARY_TYPE  eType = pRenderFont->GetGraphicsRenderLibraryType();
	if (eType != GRAPHICS_RENDER_LIBRARY_TYPE_GDI)
	{
        if (eType == GRAPHICS_RENDER_LIBRARY_TYPE_GDIPLUS)
        {
           // GdiplusRenderTarget::DrawStringEx(GetHDC(), pRenderFont, pParam);
            return;
        }
        else
        {
		    UI_LOG_WARN(_T("GdiRenderTarget::DrawString pRenderFont render type != GRAPHICS_RENDER_LIBRARY_TYPE_GDI"));
	    	return;
        }
	}
    HFONT hFont = ((GDIRenderFont*)pRenderFont)->GetHFONT();

    switch (pParam->nEffectFlag)
    {
    case TEXT_EFFECT_NONE:
        draw_string_normal(hFont, &rcText, pParam);
        break;

    case TEXT_EFFECT_HALO:
        draw_string_halo(hFont, rcText, pParam);
        break;

    case TEXT_EFFECT_ENDALPHAMASK:
        draw_string_endalphamask(hFont, &rcText, pParam);
        break;
    }
}

void  GdiRenderTarget::draw_string_halo(HFONT hFont, const CRect& rcText, DRAWTEXTPARAM* pParam)
{
	UIASSERT(0);
#if 0
	// ��Ӱռ�õĿռ䣬����ı�����һЩ�����Ҫ��������һ��Ļ���ͼƬ
	int  sizeShadow = pParam->wParam;
	if (sizeShadow < 0 || sizeShadow > 0xff)
		return;

    HDC hDC = GetHDC();
    HDC hMemDC = CreateCompatibleDC(NULL);
    SetBkMode(hMemDC, TRANSPARENT);

    int nWidth = rcText.Width();
    int nHeight = rcText.Height();

    CRect rcMem( 0, 0, nWidth + sizeShadow*2, nHeight + sizeShadow*2 );
	CRect rcMemText(sizeShadow, sizeShadow, nWidth+sizeShadow, nHeight+sizeShadow);

    HBITMAP hMemBmp = CacheBitmap::GetInstance()->Create(rcMem.Width(), rcMem.Height());

    HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, hMemBmp);
    HFONT hOldFont = (HFONT)::SelectObject(hMemDC, hFont);
    COLORREF colorShadow = pParam->bkcolor.GetGDIValue();
    COLORREF oldColor = ::SetTextColor(hMemDC, colorShadow);

    // ���ֻὫ255->0�������inverseһ�Σ������õ�ֻ�����ֵ��ڴ�ͼƬ
    FillAlpha255(hMemDC, &rcMemText);
    // ��Ӱ
    ::DrawText(hMemDC, pParam->szText, _tcslen(pParam->szText), 
        (RECT*)&rcMemText, pParam->nFormatFlag);
    InverseAlpha(hMemDC, &rcMemText);

    // ģ����ShadowBlurֻ����alpha channel��
    ShadowBlur(hMemBmp, pParam->bkcolor.GetGDIValue(), &rcMem, pParam->wParam);
    // recursive_blur<> blur;
    // blur.blur(hMemBmp, (double)pParam->wParam, &rc, 0);

    BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
    ::AlphaBlend(hDC,
        rcText.left - sizeShadow,
		rcText.top - sizeShadow,
        rcMem.Width(), rcMem.Height(), 
        hMemDC, 
		0, 0, rcMem.Width(), rcMem.Height(),
		bf);

    ::SetTextColor(hMemDC, oldColor);
    ::SelectObject(hMemDC, hOldFont);
    ::SelectObject(hMemDC, hOldBmp);
    ::DeleteDC(hMemDC);

    // ʵ������
    COLORREF color = pParam->color.GetGDIValue();
    oldColor = ::SetTextColor(hDC, color);
    hOldFont = (HFONT)::SelectObject(hDC, hFont);

    ::DrawText(hDC, pParam->szText, _tcslen(pParam->szText),
        (LPRECT)&rcText, pParam->nFormatFlag);

    ::SetTextColor(hDC, oldColor);
    ::SelectObject(hDC, hOldFont);

    if (m_bNeedAlphaChannel)
        FixAlpha0To255(hDC, (RECT*)&rcText);
#endif
}

void  GdiRenderTarget::draw_string_endalphamask(
        HFONT hFont, LPRECT prcText, DRAWTEXTPARAM* pParam)
{
	UIASSERT(0);
#if 0
    // ֻ֧�ֵ����ı�
    pParam->nFormatFlag |= DT_SINGLELINE;

    // ��������ʾ...�ˣ����黯����
    pParam->nFormatFlag &= ~DT_END_ELLIPSIS;

    int nWidth = prcText->right-prcText->left;
    int nHeight = prcText->bottom-prcText->top;
    uint nTextLength = _tcslen(pParam->szText);
    HDC hDC = GetHDC();

    // ���ָ�����������ܹ���ʾ����Щ�ı���ֱ�ӻ��Ƽ���
    {
        HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);
        SIZE s = {0};
        GetTextExtentPoint32(hDC, pParam->szText, nTextLength, &s);
        ::SelectObject(hDC, hOldFont);

        if (s.cx <= nWidth)
        {
            return draw_string_normal(hFont, prcText, pParam);
        }
    }

    HDC hMemDC = CreateCompatibleDC(NULL);
    SetBkMode(hMemDC, TRANSPARENT);
    COLORREF color = pParam->color.GetGDIValue();

    CRect rcMem(0, 0, nWidth, nHeight);
    HBITMAP hMemBmp = CacheBitmap::GetInstance()->Create(rcMem.Width(), rcMem.Height());

    HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, hMemBmp);
    HFONT hOldFont = (HFONT)::SelectObject(hMemDC, hFont);
    COLORREF oldColor = ::SetTextColor(hMemDC, color);
    
    // clear type������Ҫ�������л�ϣ���ԭ���ݿ�������
    ::BitBlt(hMemDC, 0, 0, nWidth, nHeight, hDC, pParam->prc->left, pParam->prc->top, SRCCOPY);

    // ���ֻὫ255->0�������inverseһ�Σ������õ�ֻ�����ֵ��ڴ�ͼƬ
    FillAlpha255(hMemDC, &rcMem);
    ::DrawText(
        hMemDC, 
        (LPTSTR)pParam->szText, 
        nTextLength, 
        (RECT*)&rcMem,
        pParam->nFormatFlag);
    InverseAlpha(hMemDC, &rcMem);

    // ��д��Ϊ30px��������
    int end_alpha_mask_range = 30;
    if (end_alpha_mask_range > rcMem.Width())
        end_alpha_mask_range = rcMem.Width();

    // ��ĩβ�����䴦��
    if (end_alpha_mask_range > 0)
    {
        RECT rc = rcMem;
        rc.left = rc.right-1;
        int lAlphaDet = 255 / end_alpha_mask_range;
        for (int i = 0, lAlpha = 0; 
            i < end_alpha_mask_range; 
            i++, lAlpha += lAlphaDet)
        {
            _FixAlpha(
                hMemDC, 
                &rc, 
                Util::SET_ALPHA_VALUE_IF_ALPHA_IS_255, 
                lAlpha);

            rc.left--;
            rc.right--;
        }
    }

    // alpha blendҪ��Ԥ��
    Util::PreMultiAlpha(hMemBmp, &rcMem,
        CacheBitmap::GetInstance()->IsTopdowmBitmap());

    BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
    ::AlphaBlend(hDC,
        prcText->left,
        prcText->top,
        rcMem.Width(), 
        rcMem.Height(), 
        hMemDC, 
        0, 0, 
        rcMem.Width(), 
        rcMem.Height(),
        bf);

    ::SetTextColor(hMemDC, oldColor);
    ::SelectObject(hMemDC, hOldFont);
    ::SelectObject(hMemDC, hOldBmp);
    ::DeleteDC(hMemDC);
#endif
}

void  GdiRenderTarget::draw_string_normal(
        HFONT hFont, LPRECT prcText, DRAWTEXTPARAM* pParam)
{
    HDC hDC = GetHDC();
    HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);

    COLORREF col = pParam->color.GetGDIValue();
    COLORREF oldCol = ::SetTextColor(hDC, col);

    ::DrawText(hDC, pParam->szText, _tcslen(pParam->szText), prcText, pParam->nFormatFlag);

    ::SetTextColor(hDC,oldCol);
    ::SelectObject(hDC, hOldFont);

    if (m_bNeedAlphaChannel)
        FixAlpha0To255(hDC, prcText);
}

void GdiRenderTarget::FillRgn( HRGN hRgn, UI::Color* pColor)
{
    if (NULL == pColor)
        return;

	HBRUSH hBrush = ::CreateSolidBrush(RGB(pColor->r, pColor->g, pColor->b));
	::FillRgn(GetHDC(), hRgn, hBrush);
	::DeleteObject(hBrush);
}

void GdiRenderTarget::DrawRect(LPRECT lprc, UI::Color* pColor)
{
    if (NULL == pColor)
        return;

    if (0 == pColor->a)
        return;

    HDC hDC = GetHDC();

    // Ԥ�˺����ɫ
    COLORREF gdicolor = pColor->GetGDIValue();

    if (pColor->a == 255)
    {
	    HBRUSH hBrush = ::CreateSolidBrush(gdicolor);
	    ::FillRect(hDC, lprc, hBrush);
	    ::DeleteObject(hBrush);

        FillAlphaValue(hDC, lprc, 255);

        // GetGDIValue���Ѿ�Ԥ�˹���
    //     if (pColor->a != 255)
    //     {
    //         HBITMAP hBitmap = (HBITMAP)GetCurrentObject(hDC, OBJ_BITMAP);
    //         Util::PreMultiAlpha(
    //                 hBitmap, lprc, RenderBuffer::IsTopDownBitmap());
    //     }
    }
    else
    {
        CacheBitmap* pCache = CacheBitmap::GetInstance();
        pCache->Create(1,1);
        HDC hMemDC = pCache->GetMemDC();
        pCache->SetPixel(0, 0, (gdicolor | (pColor->a << 24)));

        BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
        AlphaBlend(hDC, 
                lprc->left, lprc->top,
                RECTW(lprc), RECTH(lprc),
                hMemDC, 
                0, 0, 1, 1, 
                bf);

        pCache->ReleaseMemDC(hMemDC);
    }
}

void GdiRenderTarget::TileRect(LPRECT lprc, IRenderBitmap* pRenderBitmap)
{
	if (NULL == pRenderBitmap)
		return;

	IRenderBitmap* p = (IRenderBitmap*)pRenderBitmap;
	if (p->GetGraphicsRenderLibraryType() != GRAPHICS_RENDER_LIBRARY_TYPE_GDI)
		return;

	GDIRenderBitmap* pBitmap = (GDIRenderBitmap*)p;
	HBRUSH hBrush = ::CreatePatternBrush(pBitmap->GetBitmap()->operator HBITMAP());
	::FillRect(GetHDC(), lprc, hBrush);
	::DeleteObject(hBrush);
}

//
// ע�����ڿؼ�����ʱ�Ѿ����ü��������⽫����ʹ��pen���Ʊ߿�ʱ����һ���ֻ��Ƴ�����
//     �߿�λ�ڿؼ�����������á�
//
void GdiRenderTarget::Rectangle(LPRECT lprc, UI::Color* pColBorder, UI::Color* pColBack, int nBorder, bool bNullBack)
{
	COLORREF gdicolBorder = 0;
	COLORREF gdicolBack = 0;
	
    if (!pColBorder)
        return;

    if (0 == pColBorder->a)
        return;

    gdicolBorder = pColBorder->GetGDIValue();

	if (!bNullBack && pColBack && 0 != pColBack->a)
	{
        gdicolBack = pColBack->GetGDIValue();
	}

	HPEN hPen = ::CreatePen(PS_SOLID, nBorder, gdicolBorder);
	HBRUSH hBrush = NULL;
	if (bNullBack)
	{
		hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	else
	{
		hBrush = ::CreateSolidBrush(gdicolBack);
	}

	HDC hDC = GetHDC();

	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);

	::Rectangle(hDC, lprc->left, lprc->top, lprc->right, lprc->bottom);

    if (m_bNeedAlphaChannel)
    {
        if (bNullBack)
            FixAlpha0To255(hDC, lprc);
        else
            FillAlpha255(hDC, lprc);
    }

	::SelectObject(hDC, hOldPen);
	::SelectObject(hDC, hOldBrush);
	::DeleteObject(hPen);
	::DeleteObject(hBrush);
}

void GdiRenderTarget::DrawFocusRect(LPRECT lprc )
{
	::DrawFocusRect(GetHDC(), lprc);
}
void GdiRenderTarget::DrawLine(int x1, int y1, int x2, int y2, IRenderPen* pPen)
{
	if (NULL == pPen)
		return;

	if (pPen->GetGraphicsRenderLibraryType() != GRAPHICS_RENDER_LIBRARY_TYPE_GDI)
		return;

    HDC hDC = GetHDC();
	GdiPen* pGdiPen = (GdiPen*)pPen;
	HPEN hOldPen = (HPEN)::SelectObject(hDC, pGdiPen->m_hPen);

	::MoveToEx(hDC, x1, y1, NULL);
	::LineTo(hDC, x2, y2);

	::SelectObject(hDC, hOldPen);
}
void GdiRenderTarget::DrawPolyline(POINT* lppt, int nCount, IRenderPen* pPen)
{
	if (NULL == pPen || NULL == lppt)
		return;

	if (pPen->GetGraphicsRenderLibraryType() != GRAPHICS_RENDER_LIBRARY_TYPE_GDI)
		return;

    HDC hDC = GetHDC();

	GdiPen* pGdiPen = (GdiPen*)pPen;
	HPEN hOldPen = (HPEN)::SelectObject(hDC, pGdiPen->m_hPen);
	::Polyline(hDC, lppt, nCount);
	::SelectObject(hDC, hOldPen);
}

void GdiRenderTarget::GradientFillH(LPRECT lprc, COLORREF colFrom, COLORREF colTo )
{
//	Util::GradientFillH(GetHDC(), lprc, colFrom, colTo );
}
void GdiRenderTarget::GradientFillV(LPRECT lprc, COLORREF colFrom, COLORREF colTo )
{
//	Util::GradientFillV(GetHDC(), lprc, colFrom, colTo );
}

void GdiRenderTarget::BitBlt(int xDest, int yDest, int wDest, int hDest, IRenderTarget* pSrcHDC, int xSrc, int ySrc, DWORD dwRop)
{
	if (NULL == pSrcHDC)
		return;
	if (pSrcHDC->GetGraphicsRenderLibraryType() != GRAPHICS_RENDER_LIBRARY_TYPE_GDI)
		return;

    GdiRenderTarget*  pSrcRT = static_cast<GdiRenderTarget*>(pSrcHDC);
	::BitBlt(GetHDC(), xDest,yDest,wDest,hDest, pSrcRT->GetHDC(),xSrc,ySrc,dwRop);
}

void GdiRenderTarget::DrawBitmap( IRenderBitmap* hBitmap, int x, int y)
{
	if (NULL == hBitmap)
		return;

	IRenderBitmap* p = (IRenderBitmap*)hBitmap;
	if (p->GetGraphicsRenderLibraryType() != GRAPHICS_RENDER_LIBRARY_TYPE_GDI)
		return;

	GDIRenderBitmap* pBitmap = (GDIRenderBitmap*)p;
	if (NULL == pBitmap)
		return;

	Image* pImage = pBitmap->GetBitmap();
	if (NULL == pImage)
		return;

	pImage->Draw(GetHDC(), x,y);
}
void GdiRenderTarget::DrawBitmap(IRenderBitmap* pBitmap, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc)
{
	if (NULL == pBitmap)
		return;

	if (pBitmap->GetGraphicsRenderLibraryType() != GRAPHICS_RENDER_LIBRARY_TYPE_GDI)
		return;

	GDIRenderBitmap* pRenderBitmap = static_cast<GDIRenderBitmap*>(pBitmap);
	if (NULL == pBitmap)
		return;

	Image* pImage = pRenderBitmap->GetBitmap();
	if (NULL == pImage)
		return;

	pImage->Draw(GetHDC(), xDest,yDest, wDest,hDest, xSrc, ySrc, wDest, hDest);
}
void GdiRenderTarget::DrawBitmap( IRenderBitmap* hBitmap, int xDest, int yDest, int nDestWidth, 
							int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight )
{
	if( NULL == hBitmap )
		return;

	IRenderBitmap* p = (IRenderBitmap*)hBitmap;
	if( p->GetGraphicsRenderLibraryType() != GRAPHICS_RENDER_LIBRARY_TYPE_GDI )
		return;

	GDIRenderBitmap* pBitmap = (GDIRenderBitmap*)p;
	if (NULL == pBitmap)
		return;

	Image* pImage = pBitmap->GetBitmap();
	if (NULL == pImage)
		return;

	pImage->Draw(GetHDC(), xDest, yDest, nDestWidth, nDestHeight, xSrc, ySrc, nSrcWidth, nSrcHeight );
}

void GdiRenderTarget::DrawBitmap( IRenderBitmap* hBitmap, int xDest, int yDest, int nDestWidth, 
					int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
					C9Region* p9Region )
{
	if( NULL == hBitmap )
		return;

	if( NULL == p9Region )
		return this->DrawBitmap( hBitmap, xDest, yDest, nDestWidth, nDestHeight, xSrc, ySrc, nSrcWidth, nSrcHeight );

	IRenderBitmap* p = (IRenderBitmap*)hBitmap;
	if( p->GetGraphicsRenderLibraryType() != GRAPHICS_RENDER_LIBRARY_TYPE_GDI )
		return;

	GDIRenderBitmap* pBitmap = (GDIRenderBitmap*)p;
	if (NULL == pBitmap)
		return;

	Image* pImage = pBitmap->GetBitmap();
	if (NULL == pImage)
		return;

	pImage->Draw(GetHDC(), xDest, yDest, nDestWidth, nDestHeight, xSrc, ySrc, nSrcWidth, nSrcHeight, p9Region );
}

void GdiRenderTarget::ImageList_Draw( IRenderBitmap* hBitmap, int x, int y, int col, int row, int cx, int cy )
{
	if( NULL == hBitmap )
		return;

	IRenderBitmap* p = (IRenderBitmap*)hBitmap;
	if( p->GetGraphicsRenderLibraryType() != GRAPHICS_RENDER_LIBRARY_TYPE_GDI )
		return;

	GDIRenderBitmap* pBitmap = (GDIRenderBitmap*)p;
	if (NULL == pBitmap)
		return;

	Image* pImage = pBitmap->GetBitmap();
	if (NULL == pImage)
		return;

	pImage->ImageList_Draw(GetHDC(), x,y,col,row,cx,cy);
}

void GdiRenderTarget::DrawBitmapEx(HDC hDC, IRenderBitmap* pBitmap, DRAWBITMAPPARAM* pParam)
{
	IRenderBitmap* p = (IRenderBitmap*)pBitmap;
	if (p->GetGraphicsRenderLibraryType() != GRAPHICS_RENDER_LIBRARY_TYPE_GDI)
	{
		if (p->GetGraphicsRenderLibraryType() == GRAPHICS_RENDER_LIBRARY_TYPE_GDIPLUS) // ��Ҫ����ݵ����
		{
			UIASSERT(0);
			//GdiplusRenderTarget::DrawBitmapEx(hDC, p, pParam);
		}
		return;
	}

	Image* pImage =  ((GDIRenderBitmap*)p)->GetBitmap();
	Image  imageDisable;
	if (pParam->nFlag & DRAW_BITMAP_DISABLE)
	{
		imageDisable.CopyGrayImageFrom(pImage);
		pImage = &imageDisable;
	}

	if (pParam->nFlag & DRAW_BITMAP_STRETCH)
	{
		if (pParam->pRegion|| 
		   (pParam->wSrc == pParam->wDest && pParam->hSrc == pParam->hDest) )
		{
			pImage->Draw(hDC,
				pParam->xDest,
				pParam->yDest,
				pParam->wDest,
				pParam->hDest,
				pParam->xSrc,
				pParam->ySrc,
				pParam->wSrc,
				pParam->hSrc,
				pParam->pRegion,
				true,
				pParam->nAlpha);
		}
		else
		{
			// gdi��������Ʋ�֧�ֿ���ݣ���gdiplus��ʵ��
			Gdiplus::Graphics g(hDC);
			Gdiplus::Bitmap bmp(
				pImage->GetWidth(),
				pImage->GetHeight(),
				pImage->GetPitch(),
				PixelFormat32bppARGB,
				(BYTE*)pImage->GetBits());

			g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

			Gdiplus::Rect rcDst;
			rcDst.X = pParam->xDest;
			rcDst.Y = pParam->yDest;
			rcDst.Width = pParam->wDest;
			rcDst.Height = pParam->hDest;
			g.DrawImage(&bmp, rcDst,
				pParam->xSrc, pParam->ySrc, pParam->wSrc, pParam->hSrc, Gdiplus::UnitPixel);
		}
        if (pParam->prcRealDraw)
        {
			::SetRect(pParam->prcRealDraw,
                pParam->xDest, 
                pParam->yDest,
                pParam->xDest+pParam->wDest,
                pParam->yDest+pParam->hDest);
        }
	}
    else if (pParam->nFlag & DRAW_BITMAP_BITBLT)
    {
        int nW = min(pParam->wDest, pParam->wSrc);
        int nH = min(pParam->hDest, pParam->hSrc);
        pImage->Draw(hDC, 
            pParam->xDest, 
            pParam->yDest,
            nW, nH, 
            pParam->xSrc, 
            pParam->ySrc, 
            nW, nH, 
            pParam->nAlpha);

        if (pParam->prcRealDraw)
        {
			::SetRect(pParam->prcRealDraw,
                pParam->xDest, 
                pParam->yDest, 
                pParam->xDest+nW, 
                pParam->yDest+nH);
        }
    }
	else if (pParam->nFlag & DRAW_BITMAP_STRETCH_BORDER)
	{
		pImage->Draw(hDC, 
            pParam->xDest, 
            pParam->yDest, 
            pParam->wDest, 
            pParam->hDest,
            pParam->xSrc,
            pParam->ySrc,
            pParam->wSrc,
            pParam->hSrc, 
            pParam->pRegion, 
            false, 
            pParam->nAlpha);

        if (pParam->prcRealDraw)
        {
			::SetRect(pParam->prcRealDraw,
                pParam->xDest,
                pParam->yDest,
                pParam->xDest+pParam->wDest,
                pParam->yDest+pParam->hDest);
        }
	}
	else if (pParam->nFlag & DRAW_BITMAP_TILE)
	{
		if (NULL == pBitmap)
			return;

		RECT rc = {
            pParam->xDest, 
            pParam->yDest, 
            pParam->xDest+pParam->wDest,
            pParam->yDest+pParam->hDest
        };
		HBRUSH hBrush = ::CreatePatternBrush(pImage->operator HBITMAP());
		::FillRect(hDC, &rc, hBrush);
		::DeleteObject(hBrush);

        if (pParam->prcRealDraw)
        {
			::SetRect(pParam->prcRealDraw,
                pParam->xDest, 
                pParam->yDest, 
                pParam->xDest+pParam->wDest,
                pParam->yDest+pParam->hDest);
        }
	}
	else if (pParam->nFlag & DRAW_BITMAP_CENTER)
	{
        long wDest = pParam->wSrc;
        long hDesst = pParam->hSrc;

		long x = pParam->xDest + (pParam->wDest - wDest)/2;
		long y = pParam->yDest + (pParam->hDest - hDesst)/2;

		pImage->Draw(
            hDC, x,y, 
            pParam->wSrc, 
            pParam->hSrc, 
            pParam->xSrc,
            pParam->ySrc,
            pParam->wSrc,
            pParam->hSrc,
            pParam->nAlpha);

        if (pParam->prcRealDraw)
        {
			::SetRect(pParam->prcRealDraw,
                x, y, 
                x+pParam->wSrc,
                y+pParam->hSrc);
        }
	}
	else if (pParam->nFlag & DRAW_BITMAP_ADAPT)
	{
		if (pParam->wSrc == 0 || pParam->hSrc == 0)
			return;

		if (pParam->wDest == 0 || pParam->hDest == 0)
			return;

		bool bNeedToStretch = false;
		int  wImage = pParam->wSrc;
		int  hImage = pParam->hSrc;

		if (pParam->wDest < pParam->wSrc || pParam->hDest < pParam->hSrc)
		{
			bNeedToStretch = true;

			double tan_x_y_image = (double)pParam->wSrc / (double)pParam->hSrc;
			double tan_x_y_dest = (double)pParam->wDest / (double)pParam->hDest;

			if( tan_x_y_image > tan_x_y_dest ) // ����ռ��
			{
				wImage = pParam->wDest;
				hImage = (int)((double)wImage/tan_x_y_image);
			}
			else   // ����ռ��
			{
				hImage = pParam->hDest;
				wImage = (int)(hImage*tan_x_y_image);
			}
		}

		// ����ͼƬ��ʾλ��
		int xDisplayPos = pParam->xDest + (pParam->wDest-wImage)/2;
		int yDisplayPos = pParam->yDest + (pParam->hDest-hImage)/2;

		if (bNeedToStretch)
		{
			pImage->Draw(
                hDC, xDisplayPos, yDisplayPos,
                wImage, hImage,
                pParam->xSrc, pParam->ySrc, 
                pParam->wSrc, pParam->hSrc,
                pParam->pRegion, true,
                pParam->nAlpha);
		}
		else
		{
			pImage->Draw(hDC, 
                xDisplayPos, yDisplayPos,
                wImage, hImage, pParam->xSrc,
                pParam->ySrc, wImage, hImage, 
                pParam->nAlpha);
		}

        if (pParam->prcRealDraw)
        {
			::SetRect(pParam->prcRealDraw,
                xDisplayPos, yDisplayPos, 
                xDisplayPos+wImage, yDisplayPos+hImage);
        }
	}
    else if (pParam->nFlag & DRAW_BITMAP_BITBLT_RIGHTTOP)
    {
        int nW = min(pParam->wDest, pParam->wSrc);
        int nH = min(pParam->hDest, pParam->hSrc);

        int xDest = pParam->xDest + (pParam->wDest-nW);
        int xSrc = pParam->xSrc+ (pParam->wSrc-nW);

        pImage->Draw(hDC, xDest, 
            pParam->yDest,
            nW, nH, 
            xSrc, pParam->ySrc, 
            nW, nH, 
            pParam->nAlpha);

        if (pParam->prcRealDraw)
        {
			::SetRect(pParam->prcRealDraw,
                xDest, 
                pParam->yDest, 
                xDest+nW,
                pParam->yDest+nH);
        }
    }
}
void GdiRenderTarget::DrawBitmap(IRenderBitmap* pRenderBitmap, 
                                 DRAWBITMAPPARAM* pParam)
{
	if (NULL == pRenderBitmap || NULL == pParam)
		return;

	GdiRenderTarget::DrawBitmapEx(GetHDC(), pRenderBitmap, pParam);
}

#if 0
//
//
// ����ͼƬ������תnDegree�Ȼ���ͼƬ
// 
//
// ʹ��SetWorldTransformҲ����ʵ����ת��Ч��������MSDN����Example
//
// The SetWorldTransform function sets a two-dimensional linear transformation between 
// world space and page space for the specified device context. This transformation can 
// be used to scale, rotate, shear, or translate graphics output.
//
// ����ת����ʽ 
//  newx = x * eM11 + y * eM21 + eDx 
//  newy = x * eM12 + y * eM22 + eDy 
//  ����eM11��eM22�ǽǶȵ�����ֵ��eM21�ǽǶȵ����ң�eM12��eM21�ĸ�ֵ��
//
// [ע1] ���hDC��һ��MemDC�Ļ���������ڻ�����֮�����ModifyWorldTransform(hMemDC, NULL, MWT_IDENTITY);
//       �����memdc�ύ������DC�ϵ����ݽ�����ȷ
// [ע2] Ĭ����Щ��ת���ǻ���(0,0)����ģ����ú���Ҫʵ�ֵ�Ŀ���ǻ�������������ת
//
void GdiRenderTarget::DrawRotateBitmap(IRenderBitmap* pBitmap, int nDegree, DRAWBITMAPPARAM* pParam)
{

	IRenderBitmap* p = (IRenderBitmap*)pBitmap;
	if (p->GetGraphicsRenderLibraryType() != GRAPHICS_RENDER_LIBRARY_TYPE_GDI)
		return;

#define M_PI 3.14159265358979323846

	Image* pImage =  ((GDIRenderBitmap*)p)->GetBitmap();
	if (pImage)
	{
		POINT ptCenter = {pParam->xDest+pParam->wDest/2, pParam->yDest+pParam->hDest/2};  // ͼƬĿ����������ģ��Ƹõ���ת��������Ĭ���ǣ�0��0��������ת

		double fAngle =  M_PI * nDegree / 180; // (2*pi *nDegree)/360 ; 
		float cosAngle = (float)cos(fAngle);
		float sinAngle = (float)sin(fAngle);

#if 0  // ʹ��PlgBlt���Ƶ�ͼƬ���޷�ʹ��Alpha����˷���

		POINT ptOri[3] = // Ŀ��������꣨δ��תǰ��
		{
			{pParam->xDest, pParam->yDest}, 
			{pParam->xDest+pParam->wDest, pParam->yDest}, 
			{pParam->xDest, pParam->yDest+pParam->hDest}
		};
		POINT ptOriToCenter[3] = // �����ͼƬ����λ��
		{
			{ptOri[0].x - ptCenter.x, ptOri[0].y - ptCenter.y},
			{ptOri[1].x - ptCenter.x, ptOri[1].y - ptCenter.y},
			{ptOri[2].x - ptCenter.x, ptOri[2].y - ptCenter.y}
		};

		POINT ptArg[3] = {0};  // ������ת���ƽ���ı��е�ǰ��������λ��
		for (int i = 0; i < 3; i++)
		{
			ptArg[i].x = (int)(ptOriToCenter[i].x*cosAngle + ptOriToCenter[i].y*sinAngle); 
			ptArg[i].y = (int)(ptOriToCenter[i].y*cosAngle - ptOriToCenter[i].x*sinAngle);

			// �ָ�Ϊ����ڣ�0��0������
			ptArg[i].x += ptCenter.x;
			ptArg[i].y += ptCenter.y;
		}

		pImage->PlgBlt(m_hDC, ptArg, pParam->xSrc, pParam->ySrc, pParam->wSrc, pParam->hSrc, NULL, 0, 0);

#else
        HDC hDC = GetHDC();

 		int nOldGraphicsMode = ::GetGraphicsMode(m_hDC);
 		::SetGraphicsMode(hDC, GM_ADVANCED);
  		XFORM xForm = {0};
		xForm.eM11 = cosAngle;
		xForm.eM12 = sinAngle;
		xForm.eM21 = -sinAngle;
		xForm.eM22 = cosAngle;
		xForm.eDx = ptCenter.x - cosAngle*ptCenter.x + sinAngle*ptCenter.y;
		xForm.eDy = ptCenter.y - cosAngle*ptCenter.y - sinAngle*ptCenter.x;
		SetWorldTransform(hDC, &xForm);
 		DrawBitmapEx(hDC, pBitmap, pParam);

		ModifyWorldTransform(hDC, NULL, MWT_IDENTITY);
 		::SetGraphicsMode(hDC, nOldGraphicsMode);
#endif
	}
}
#endif


IRenderPen* GdiRenderTarget::CreateSolidPen(int nWidth, Color* pColor)
{
	IRenderPen* p = NULL;
	GdiPen::CreateInstance(&p);

	if (p)
	{
		p->CreateSolidPen(nWidth, pColor);
	}
	return p;
}
IRenderPen* GdiRenderTarget::CreateDotPen(int nWidth, Color* pColor)
{
	IRenderPen* p = NULL;
	GdiPen::CreateInstance(&p);

	if (p)
	{
		p->CreateDotPen(nWidth, pColor);
	}
	return p;
}
IRenderBrush*  GdiRenderTarget::CreateSolidBrush(Color* pColor)
{
	IRenderBrush* p = NULL;
	GdiBrush::CreateInstance(&p);

	if (p)
	{
		p->CreateSolidBrush(pColor);
	}
	return p;
}

void  GdiRenderTarget::Render2DC(HDC hDstDC, Render2TargetParam* pParam)
{
    int& xDst = pParam->xDst;
    int& yDst = pParam->yDst; 
    int& wDst = pParam->wDst; 
    int& hDst = pParam->hDst; 
    int& xSrc = pParam->xSrc; 
    int& ySrc = pParam->ySrc; 
    int& wSrc = pParam->wSrc; 
    int& hSrc = pParam->hSrc; 
    bool& bAlphaBlend = pParam->bAlphaBlend;
    byte& opacity = pParam->opacity;

    HDC hDC = GetHDC();
    UIASSERT (hDC != hDstDC);

    int nOldGraphicsMode = 0;
#if 0
    if (pParam->pTransform2d)
    {
        nOldGraphicsMode = ::GetGraphicsMode(hDstDC);
        ::SetGraphicsMode(hDstDC, GM_ADVANCED);

		XFORM xForm = {
			pParam->pTransform2d->m11, pParam->pTransform2d->m12,
			pParam->pTransform2d->m21, pParam->pTransform2d->m22,
			pParam->pTransform2d->m31, pParam->pTransform2d->m32
		};
        ::SetWorldTransform(hDstDC, &xForm);
    }
#endif
    if (bAlphaBlend)
    {
        BLENDFUNCTION bf = {AC_SRC_OVER, 0, opacity, AC_SRC_ALPHA};
        ::AlphaBlend(hDstDC, xDst, yDst, wDst, hDst, hDC, xSrc, ySrc, wSrc, hSrc, bf);
    }
    else
    {
        if (wDst == wSrc && hDst == hSrc)
        {
            ::BitBlt(hDstDC, xDst, yDst, wDst, hDst, hDC, xSrc, ySrc, SRCCOPY);
        }
        else
        {
            ::StretchBlt(hDstDC, xDst, ySrc, wDst, hDst, hDC, xSrc, ySrc, wSrc, hSrc, SRCCOPY);
        }
    }
    
#if 0
    if (pParam->pTransform2d)
    {
        ModifyWorldTransform(hDstDC, NULL, MWT_IDENTITY);
        ::SetGraphicsMode(hDstDC, nOldGraphicsMode);
    }
#endif
}

void  GdiRenderTarget::Render2Target(IRenderTarget* pDst, Render2TargetParam* pParam)
{
    if (!pDst)
        return;

    if (pDst->GetGraphicsRenderLibraryType() != GRAPHICS_RENDER_LIBRARY_TYPE_GDI)
    {
        UI_LOG_WARN(_T("Graphcis Render library is wrong"));
        return;
    }

    GdiRenderTarget* pGdiDst = static_cast<GdiRenderTarget*>(pDst);

#if 0
	if (pParam->pTransform3d)
	{
		QuadPerspectiveTextureMapping textmapping;

		Matrix44  matrix(pParam->pTransform3d);
		RECT rcSrc = {pParam->xSrc, pParam->ySrc, pParam->xSrc+pParam->wSrc, pParam->ySrc+pParam->hSrc};

		// ��������ӳ������
		QUAD quad;
		matrix.MapRect2Quad(&rcSrc, &quad);  // �任
		textmapping.SetQuad(&quad, &rcSrc);

		// ������������
		textmapping.SetTextureRect(&rcSrc);

		// ��������(src)��Ŀ�껺��(dest)
		ImageData  srcBuffer;
		this->GetRenderBufferData(&srcBuffer);
		textmapping.SetSrcBuffer(&srcBuffer);

		ImageData  dstBuffer;
		pGdiDst->GetRenderBufferData(&dstBuffer);
		textmapping.SetDstBuffer(&dstBuffer);

		// ��Ⱦλ�úͼ�������
		POINT  ptOffsetDst;
		pGdiDst->GetOrigin((int*)&ptOffsetDst.x, (int*)&ptOffsetDst.y);
		POINT ptDraw = {ptOffsetDst.x + pParam->xDst, ptOffsetDst.y + pParam->yDst};

		RECT  rcClip = {0};
		GetClipBox(pGdiDst->GetHDC(), &rcClip);
		OffsetRect(&rcClip, ptOffsetDst.x, ptOffsetDst.y);
		
		textmapping.Do(&ptDraw, &rcClip, pParam->opacity);
		return;
	}
#endif
    return Render2DC(pGdiDst->GetHDC(), pParam);
}

void  GdiRenderTarget::Save(const TCHAR*  szPath )
{
    if (m_pRenderBuffer)
    {
        m_pRenderBuffer->Dump();
    }
}

void  GdiRenderTarget::Upload2Gpu(IGpuRenderLayer* p, LPRECT prcArray, int nCount)
{
	UIASSERT(0);
#if 0
    if (m_pRenderBuffer)
    {
        m_pRenderBuffer->Upload2Gpu(p, prcArray, nCount);
    }
#endif
}

// gdi�����ı�����alphaͨ����gdi+�ı������ֱȽ϶ࡣ
// �������ܲ�����gdi+���ı��Ͳ�����
void  GdiRenderTarget::FixAlpha0To255(HDC hDC, LPCRECT lpRect)
{
    _FixAlpha(hDC, lpRect, Util::SET_ALPHA_255_IF_ALPHA_IS_0, 0);
}

void  GdiRenderTarget::FillAlpha255(HDC hDC, LPCRECT lpRect)
{
    _FixAlpha(hDC, lpRect, Util::SET_ALPHA_255, 0);
}
void  GdiRenderTarget::FillAlphaValue(HDC hDC, LPCRECT lpRect, WPARAM w)
{
	_FixAlpha(hDC, lpRect, Util::SET_ALPHA_VALUE, w);
}
void  GdiRenderTarget::InverseAlpha(HDC hDC, LPCRECT lpRect)
{
    _FixAlpha(hDC, lpRect, Util::SET_ALPHA_INVERSE_0_255, 0);
}
void  GdiRenderTarget::_FixAlpha(
	HDC hDC, LPCRECT lpRect, Util::FixAlphaMode e, WPARAM wParam)
{
    RECT rc = *lpRect;

    RECT rcClip;
    GetClipBox(hDC, &rcClip);
    ::IntersectRect(&rc, &rc, &rcClip);

    POINT ptOffset = {0};
    ::GetViewportOrgEx(hDC, &ptOffset);
    ::OffsetRect(&rc, ptOffset.x, ptOffset.y);

    Util::FixAlphaData data = {0};
    data.bTopDownDib = RenderBuffer::IsTopDownBitmap();
    data.hDC = hDC;
    data.lprc = &rc;
    data.eMode = e;
	data.wParam = wParam;
    Util::FixBitmapAlpha(&data);
}