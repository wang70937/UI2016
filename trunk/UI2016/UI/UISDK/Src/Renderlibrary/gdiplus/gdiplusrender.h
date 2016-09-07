#pragma once
#include "gdipluspen.h"
#include "gdiplusbitmap.h"
#include "gdiplusfont.h"
#include "UISDK\Kernel\Src\Util\RectArray\rectarray.h"

//
// 2014.4.10
// Graphics::SetClipЧ�ʷǳ��ͣ�����ҶDemo�У�SetClipֱ�ӽ�CPU������ȥ��SetClip��ָ�Ϊ0
// Ϊ�˽�������⣬����ֻ��ÿ����ҪGraphics����ʱ�Ŵ�hdc����һ����
// ����ʱ�򲻱����ö��󣬱���ÿ�����ü��ú�ƫ��ʱ������ͬ����Graphics��
// ��������ʹ��Gdiplusʱ���󲿷ֵĻ�ͼ�������ǽ�����gdi��alphablend
//
namespace UI
{
class RenderBuffer;

class GdiplusRenderTarget : public IRenderTarget
{
public:
 	GdiplusRenderTarget();
	virtual ~GdiplusRenderTarget();
    virtual void  Release();
	virtual GRAPHICS_RENDER_LIBRARY_TYPE GetGraphicsRenderLibraryType() { return GRAPHICS_RENDER_LIBRARY_TYPE_GDIPLUS; }
    virtual bool  IsRenderAlphaChannel() { return true; }

	virtual void  BindHDC(HDC hDC);
	virtual HDC   GetBindHDC();
    virtual HDC   GetHDC();
    virtual void  ReleaseHDC(HDC hDC);
    virtual bool  CreateRenderBuffer(IRenderTarget*  pSrcRT);
    virtual bool  ResizeRenderBuffer(unsigned int nWidth, unsigned int nHeight);
	virtual void  GetRenderBufferData(ImageData*  pData);
    virtual void  BindHWND(HWND hWnd) { /*UIASSERT(0);*/ /*��֧��*/ }

    virtual void  SetMetaClipRegion(LPRECT prc, uint nrcCount) override;
    virtual void  PushRelativeClipRect(LPCRECT) override;
    virtual void  PopRelativeClipRect() override;
    virtual bool  IsRelativeRectInClip(LPCRECT) override;
    virtual void  SetOrigin(int x, int y) override;
    virtual void  OffsetOrigin(int x, int y) override;
    virtual void  GetOrigin(int* px, int* py) override;

// 	virtual HRGN  GetClipRgn();
// 	virtual int   SelectClipRgn( HRGN hRgn, int nMode = RGN_COPY );
// 	virtual int   SelectClipRect( RECT* prc, uint nrcCount, int nMode = RGN_COPY );
// 	virtual BOOL  GetViewportOrgEx( LPPOINT lpPoint );
// 	virtual BOOL  SetViewportOrgEx( int x, int y, LPPOINT lpPoint = NULL ) ;
// 	virtual BOOL  OffsetViewportOrgEx( int x, int y, LPPOINT lpPoint = NULL );

	virtual bool  BeginDraw();
	virtual void  EndDraw();
	virtual void  Clear(RECT* prc);
	virtual void  Save(const TCHAR* szPath);
    virtual void  Render2DC(HDC hDC, Render2TargetParam* pParam);
    virtual void  Render2Target(IRenderTarget* pDst, Render2TargetParam* pParam);

	virtual void  FillRgn( HRGN hRgn, UI::Color* pColor);
	virtual void  DrawRect(LPRECT lprc, UI::Color* pColor);
	virtual void  TileRect(LPRECT lprc, IRenderBitmap* hBitmap );
	virtual void  Rectangle(LPRECT lprc, UI::Color* pColBorder, UI::Color* pColBack, int nBorder, bool bNullBack );
	virtual void  DrawFocusRect(LPRECT lprc );
	virtual void  DrawLine(int x1, int y1, int x2, int y2, IRenderPen*);
	virtual void  DrawPolyline(POINT* lppt, int nCount, IRenderPen*);
	virtual void  GradientFillH(LPRECT lprc, COLORREF colFrom, COLORREF colTo );
	virtual void  GradientFillV(LPRECT lprc, COLORREF colFrom, COLORREF colTo );
	virtual void  BitBlt( int xDest, int yDest, int wDest, int hDest, IRenderTarget* pSrcHDC, int xSrc, int ySrc, DWORD dwRop );
	virtual void  ImageList_Draw( IRenderBitmap* hBitmap, int x, int y, int col, int row, int cx, int cy );
	
    virtual void  DrawBitmap(IRenderBitmap* hBitmap, DRAWBITMAPPARAM* pParam );
    virtual void  DrawString(IRenderFont* pFont, DRAWTEXTPARAM* pParam);

    virtual IRenderPen*     CreateSolidPen(int nWidth, Color* pColor);
    virtual IRenderPen*     CreateDotPen(int nWidth, Color* pColor); 
    virtual IRenderBrush*   CreateSolidBrush(Color* pColor);

    virtual void  Upload2Gpu(IGpuRenderLayer* p, LPRECT prcArray, int nCount);

public:
	static void  DrawBitmap(Gdiplus::Graphics* pGraphics, IRenderBitmap* hBitmap, int x, int y );
	static void  DrawBitmap(Gdiplus::Graphics* pGraphics, IRenderBitmap* pBitmap, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc);
	static void  DrawBitmap(Gdiplus::Graphics* pGraphics, IRenderBitmap* hBitmap, int xDest, int yDest, int nDestWidth, 
							int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, Gdiplus::ImageAttributes* pAttr=NULL);
	static void  DrawBitmap(Gdiplus::Graphics* pGraphics, IRenderBitmap* hBitmap, int xDest, int yDest, int nDestWidth, 
							int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
                            C9Region* p9Region, bool bDrawCenter=true, Gdiplus::ImageAttributes* pAttr=NULL);		
	static void  DrawBitmapEx(HDC hBindDC, IRenderBitmap* pBitmap, DRAWBITMAPPARAM* pParam);
    static void  DrawStringEx(HDC hBindDC, IRenderFont*  pFont, DRAWTEXTPARAM* pParam);

    static void  GetStringFormatByGdiFormat(UINT, Gdiplus::StringFormat* p);

private:
    void  update_clip_rgn();

protected:
//	Gdiplus::Graphics*  m_pGraphics;
//	Gdiplus::Bitmap*    m_pGdiMemBitmap;
//	HRGN     m_hRgnMeta;   // ����ģ��ʵ��GDI��SetMetaRgn����

    HDC      m_hBindDC;
    RenderBuffer*  m_pRenderBuffer;
    long     m_lDrawingRef; // ��ʶ�ⲿ�����˼���BeginDraw�����Ƕ�׵��ó��ֵ�һЩ����


    // ��������
    RectArray  m_arrayMetaClipRegion;
    stack<RECT>  m_stackClipRect;

    POINT  m_ptOffset;   // ���ڵ���ʱ�鿴��ǰHDCƫ����
};
}