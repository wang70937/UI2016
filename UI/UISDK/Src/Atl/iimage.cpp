#include "stdafx.h"
#include "image.h"
#include "Inc\Util\iimage.h"



ImageWrap::ImageWrap()
{
    m_pImageImpl = new Image;
	m_bCreateImage = TRUE;
}

ImageWrap::~ImageWrap()
{
	if (m_bCreateImage)
		SAFE_DELETE(m_pImageImpl);
}

ImageWrap::ImageWrap(Image* p)
{
	m_pImageImpl = p;
	m_bCreateImage = FALSE;
}

Image*  ImageWrap::GetImpl()
{
	return m_pImageImpl; 
}

bool  ImageWrap::Load(const TCHAR*  szPath, bool bForceAlpha) 
{ 
	if (bForceAlpha)
		m_pImageImpl->ForceUseAlpha();

    if (FAILED(m_pImageImpl->Load(szPath)))
        return false;
    if (m_pImageImpl->IsNull())
        return false;

    return true;
}

bool  ImageWrap::LoadFromResource( HINSTANCE hInstance, UINT nIDResource, TCHAR* szResourceType )
{
    m_pImageImpl->LoadFromResource(hInstance, nIDResource, szResourceType);
    if (m_pImageImpl->IsNull())
        return false;
    return true;
}
void  ImageWrap::Attach(HBITMAP hBitmap, bool bHasAlphaChannel)
{ 
    m_pImageImpl->Attach(hBitmap);
    if (bHasAlphaChannel)
    {
        m_pImageImpl->ForceUseAlpha();
    }
}
HBITMAP  ImageWrap::Detach()   
{ 
	return m_pImageImpl->Detach();
}

BOOL  ImageWrap::Create(int nWidth, int nHeight, int nBPP, DWORD dwFlags) 
{ 
	return m_pImageImpl->Create(nWidth, nHeight, nBPP, dwFlags); 
}
void  ImageWrap::Destroy() 
{ 
	m_pImageImpl->Destroy(); 
}
bool  ImageWrap::IsNull()  
{
	return m_pImageImpl->IsNull(); 
}

int   ImageWrap::GetWidth() 
{
	return m_pImageImpl->GetWidth();
}
int   ImageWrap::GetHeight()
{ 
	return m_pImageImpl->GetHeight();
}
int   ImageWrap::GetPitch() 
{ 
	return m_pImageImpl->GetPitch();
}
byte* ImageWrap::GetBits()  
{
	return (byte*)m_pImageImpl->GetBits();
}
int   ImageWrap::GetBPP()   
{ 
	return m_pImageImpl->GetBPP(); 
}
HBITMAP  ImageWrap::GetHBITMAP()  
{
	return m_pImageImpl->GetHBITMAP(); 
}

HDC   ImageWrap::GetDC()       
{
	return m_pImageImpl->GetDC();
}
void  ImageWrap::ReleaseDC()         
{
	return m_pImageImpl->ReleaseDC();
}
BOOL  ImageWrap::BitBlt(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, DWORD dwROP)
{ 
    return m_pImageImpl->BitBlt(hDestDC, xDest, yDest, nDestWidth, nDestHeight, xSrc, ySrc, dwROP); 
}
void  ImageWrap::Draw(HDC hDestDC, int x, int y)
{
    m_pImageImpl->Draw(hDestDC, x, y);
}
void  ImageWrap::Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight)
{
    m_pImageImpl->Draw(hDestDC, xDest, yDest, nDestWidth, nDestHeight, xSrc, ySrc, nSrcWidth, nSrcHeight);
}
BOOL  ImageWrap::Draw( 
		HDC hDestDC, int xDest, int yDest, int nDestWidth, int nDestHeight, 
		int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, 
		C9Region* pImage9Region, bool bDrawCenter, byte bAlpha)
{
    return m_pImageImpl->Draw(hDestDC, xDest, yDest, nDestWidth, nDestHeight, xSrc, ySrc, nSrcWidth, nSrcHeight, pImage9Region, bDrawCenter, bAlpha);
}

void  ImageWrap::AlphaTopEdge(byte nSpeed)
{
    m_pImageImpl->AlphaEdge(Image::ImageAlphaEdge_Top, nSpeed); 
}
void  ImageWrap::AlphaBottomEdge(byte nSpeed) 
{ 
    m_pImageImpl->AlphaEdge(Image::ImageAlphaEdge_Bottom, nSpeed); 
}
void  ImageWrap::AlphaLeftEdge(byte nSpeed)
{
    m_pImageImpl->AlphaEdge(Image::ImageAlphaEdge_Left, nSpeed); 
}
void  ImageWrap::AlphaRightEdge(byte nSpeed)
{
    m_pImageImpl->AlphaEdge(Image::ImageAlphaEdge_Right, nSpeed); 
}
void  ImageWrap::SaveAsPng(const TCHAR* szPath) 
{ 
    if (NULL == szPath)
        return;

    m_pImageImpl->Save(szPath, Gdiplus::ImageFormatPNG); 
}
void  ImageWrap::SaveAsBmp(const TCHAR* szPath)
{
	if (NULL == szPath)
		return;

	m_pImageImpl->Save(szPath, Gdiplus::ImageFormatBMP); 
}

COLORREF  ImageWrap::GetPixel(int x, int y) 
{
	return m_pImageImpl->GetPixel(x, y); 
}
DWORD  ImageWrap::GetAverageColor() 
{ 
	return m_pImageImpl->GetAverageColor();
}

bool  ImageWrap::ChangeHSL(short h, short s, short l, int nFlag)
{
    return m_pImageImpl->ChangeHSL(NULL, h, s, l, nFlag);
}
