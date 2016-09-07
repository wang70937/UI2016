#ifndef _UI_IIMAGE_H_
#define _UI_IIMAGE_H_

namespace UI
{
class Image;
interface UISDKAPI ImageWrap
{
    ImageWrap();
	ImageWrap(Image* p);
    ~ImageWrap();
    Image*  GetImpl();

    static const DWORD createAlphaChannel = 0x01;

    void  Attach(HBITMAP hBitmap, bool bHasAlphaChannel);
    HBITMAP  Detach();
    bool  Load(const TCHAR*  szPath, bool bForceAlpha=false);
    bool  LoadFromResource( HINSTANCE hInstance, UINT nIDResource, TCHAR* szResourceType );

    BOOL  Create(int nWidth, int nHeight, int nBPP, DWORD dwFlags = 0);
    void  Destroy();
    bool  IsNull();

    int   GetWidth();
    int   GetHeight();
    int   GetPitch();
    byte* GetBits();
    int   GetBPP();
	HBITMAP  GetHBITMAP();

    HDC   GetDC();
    void  ReleaseDC();
    BOOL  BitBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, DWORD dwROP = SRCCOPY);
    void  Draw(HDC hDestDC, int x, int y);
    void  Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight);
    BOOL  Draw( HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, C9Region* pImage9Region, bool bDrawCenter=true, byte bAlpha=0xFF );

    void  AlphaBottomEdge(byte nSpeed=2);
    void  AlphaTopEdge(byte nSpeed=2);
    void  AlphaLeftEdge(byte nSpeed=2);
    void  AlphaRightEdge(byte nSpeed=2);

    bool  ChangeHSL(short h, short s , short l, int nFlag);

    DWORD  GetAverageColor();
    COLORREF  GetPixel(int x, int y);
    void  SaveAsPng(const TCHAR* szPath);
	void  SaveAsBmp(const TCHAR* szPath);

private:
    Image*  m_pImageImpl;
	BOOL    m_bCreateImage;
};
}

#endif // _UI_IIMAGE_H_