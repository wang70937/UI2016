#include "stdafx.h"
#include "image.h"
#include "..\..\Inc\Util\struct.h"
namespace UI
{

Image::CInitGDIPlus		Image::s_initGDIPlus;
CDCCache			    Image::s_cache;


// ��һ��ͼƬ�ı�Ե���仯�������޸�һ��Ҫ��Ϊ����Ƥ����ͼƬ��ʹ��Ե�����뱳����ɫ�ں�
void  Image::AlphaEdge(int nEdge, byte nSpeed)
{
    // ���Լ�ת�ɴ�alpha channel�ĸ�ʽ
    if (m_nBPP != 32)
    {
        Image alphaimage;
        alphaimage.Create(m_nWidth, m_nHeight, 32, createAlphaChannel);
        HDC hDC = alphaimage.GetDC();
        this->BitBlt(hDC,0,0);
        alphaimage.ReleaseDC();
        alphaimage.SetAlpha(255);
        this->Destroy();
        *this = alphaimage;
        alphaimage.Detach();
    }

    if (nEdge & ImageAlphaEdge_Bottom)
    {
        byte* p = (byte*)m_pBits + (m_nHeight-1)*m_nPitch;
        int  bBytesPerline = abs(m_nPitch);

        int alpha = 0;
        for (int y = m_nHeight; y > 0 && alpha <= 255; y--, alpha+=nSpeed)
        {
            for (int x = 0; x < bBytesPerline; x++)
            {
                p[x] = p[x]*(byte)alpha >> 8;
            }

            p -= m_nPitch;
        }
    }
    if (nEdge & ImageAlphaEdge_Top)
    {
        byte* p = (byte*)m_pBits;
        int  bBytesPerline = abs(m_nPitch);

        int alpha = 0;
        for (int y = 0; y < m_nHeight && alpha <= 255; y++, alpha+=nSpeed)
        {
            for (int x = 0; x < bBytesPerline; x++)
            {
                p[x] = p[x]*(byte)alpha >> 8;
            }

            p += m_nPitch;
        }
    }

    if (nEdge & ImageAlphaEdge_Left)
    {
        byte* p = (byte*)m_pBits;
        int  bBytesPerline = abs(m_nPitch);

        for (int y = 0; y < m_nHeight; y++)
        {
            int alpha = 0;
            for (int x = 0; x < bBytesPerline && alpha <= 255; alpha+=nSpeed)
            {
                p[x] = p[x]*(byte)alpha >> 8; x++;
                p[x] = p[x]*(byte)alpha >> 8; x++;
                p[x] = p[x]*(byte)alpha >> 8; x++;
                p[x] = p[x]*(byte)alpha >> 8; x++;
            }

            p += m_nPitch;
        }
    }

    if (nEdge & ImageAlphaEdge_Right)
    {
        byte* p = (byte*)m_pBits;
        int  bBytesPerline = abs(m_nPitch);

        for (int y = 0; y < m_nHeight; y++)
        {
            int alpha = 0;
            for (int x = bBytesPerline-1; x > 0 && alpha <= 255; alpha+=nSpeed)
            {
                p[x] = p[x]*(byte)alpha >> 8;  x--;
                p[x] = p[x]*(byte)alpha >> 8;  x--;
                p[x] = p[x]*(byte)alpha >> 8;  x--;
                p[x] = p[x]*(byte)alpha >> 8;  x--;
            }

            p += m_nPitch;
        }
    }
}



// ע���ѱ�Util::FixGdiAlphaȡ�������ú�����Ȼ����ʹ��
//
// ��nAlphaIsZero==1��λ����������alpha=0������SetAlphaʹ�ã�
// ������λ��alpha����Ϊ255�� ����richedit�ڷֲ㴰���ϵĻ���
bool Image::FixGDIAlphaChannel(int nAlphaIsZero)
{
    if (m_nBPP != 32)
        return false;

    BYTE* pImageBits = (BYTE*)m_pBits;
    for (int row = 0; row < m_nHeight; row ++)
    {
        for (int i = 0; i < m_nWidth; i ++)
        {
            int nAlphaIndex = 4*i+3;
            int nAlpha = pImageBits[nAlphaIndex];
            if (nAlphaIsZero == nAlpha)
            {
                pImageBits[nAlphaIndex] = 0;
            }
            else if (0 == nAlpha)
            {
                pImageBits[nAlphaIndex] = 255;
            }
        }
        pImageBits+= m_nPitch;
    }

    return true;
}

DWORD  Image::GetAverageColor(RECT* prc)
{
    if (NULL == prc)
        return 0;
    if (m_nBPP != 24 && m_nBPP != 32)   // ��֧��24��32λ��ͼƬ
        return 0;

    if (NULL == m_hBitmap)
        return 0;

    int nWidth = prc->right-prc->left;
    int nHeight = prc->bottom-prc->top;

    int nCount = nWidth*nHeight;
    if (0 == nCount)
        return 0;

    BYTE* pTemp = (BYTE*)m_pBits + prc->top*m_nPitch;

    int   bytesperpx    = m_nBPP>>3;
 //   int   bytesperline   = bytesperpx*nWidth;  /*abs(m_nPitch);*/ // ע������λ��һ��Ҫ����4��λ�ã����ܵ��µ����Ϊ������������Ჹ�����õ�λ��������ﲻ��ֱ����m_nPitch
    bool  bHaveAlphaChannel = GetBPP() == 32;

    LONGLONG  a = 0, g = 0, b = 0, r = 0;
    if (bHaveAlphaChannel)
    {
        for (int row = prc->top; row < prc->bottom; row ++)
        {
            int nLeftPos = prc->left*bytesperpx;
            int nRightPos = prc->right*bytesperpx;
            for( int i = nLeftPos; i < nRightPos; i += bytesperpx)
            {
                b += pTemp[i];
                g += pTemp[i+1];
                r += pTemp[i+2];
                a += pTemp[i+3];
            }
            pTemp += m_nPitch;
        }
    }
    else
    {
        for (int row = prc->top; row < prc->bottom; row ++ )
        {
            int nLeftPos = prc->left*bytesperpx;
            int nRightPos = prc->right*bytesperpx;
            for( int i = nLeftPos; i < nRightPos; i += bytesperpx)
            {
                b += pTemp[i];
                g += pTemp[i+1];
                r += pTemp[i+2];
            }
            pTemp += m_nPitch;
        }
    }

    a = a/nCount;
    r = r/nCount;
    g = g/nCount;
    b = b/nCount;

    return ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)|(((DWORD)(BYTE)(a))<<24)));
}

// ��ȡͼ���ƽ��ɫֵ
DWORD Image::GetAverageColor()
{
    RECT  rc = {0, 0, m_nWidth, m_nHeight};
    return GetAverageColor(&rc);

//     if (m_nBPP != 24 && m_nBPP != 32)   // ��֧��24��32λ��ͼƬ
//         return 0;
// 
//     if (NULL == m_hBitmap)
//         return 0;
// 
//     int nCount = m_nWidth*m_nHeight;
//     if (0 == nCount)
//         return 0;
// 
//     BYTE* pTemp = (BYTE*)m_pBits;
//     int   bytesperpx    = m_nBPP>>3;
//     int   bytesperline   = bytesperpx*m_nWidth;  /*abs(m_nPitch);*/ // ע������λ��һ��Ҫ����4��λ�ã����ܵ��µ����Ϊ������������Ჹ�����õ�λ��������ﲻ��ֱ����m_nPitch
//     bool  bHaveAlphaChannel = GetBPP() == 32;
// 
//     LONGLONG  a = 0, g = 0, b = 0, r = 0;
//     if (bHaveAlphaChannel)
//     {
//         for (int row = 0; row < m_nHeight; row ++ )
//         {
//             for( int i = 0; i < bytesperline; i += bytesperpx )
//             {
//                 b += pTemp[i];
//                 g += pTemp[i+1];
//                 r += pTemp[i+2];
//                 a += pTemp[i+3];
//             }
//             pTemp += m_nPitch;
//         }
//     }
//     else
//     {
//         for (int row = 0; row < m_nHeight; row ++ )
//         {
//             for( int i = 0; i < bytesperline; i += bytesperpx )
//             {
//                 b += pTemp[i];
//                 g += pTemp[i+1];
//                 r += pTemp[i+2];
//             }
//             pTemp += m_nPitch;
//         }
//     }
// 
//     a = a/nCount;
//     r = r/nCount;
//     g = g/nCount;
//     b = b/nCount;
// 
//     return ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)|(((DWORD)(BYTE)(a))<<24)));
}


HBITMAP Image::CopyImageByRect(RECT* prc)
{
    if (NULL == prc)
        return NULL;

    int nWidth = prc->right - prc->left;
    int nHight = prc->bottom - prc->top;

    Image image;
    image.Create(nWidth, nHight, 32, Image::createAlphaChannel);

    HDC hDC = image.GetDC();
    this->BitBlt(hDC, 0,0, nWidth, nHight, prc->left, prc->top);
    image.ReleaseDC();

    HBITMAP hRetValue = image.Detach();
    return hRetValue;
}

// ������ȫ����0
void  Image::Clear()
{
    assert(m_hBitmap);

    if ( m_nPitch < 0)  // ����
    {
        int nBitsSize = (m_nPitch * m_nHeight);
        byte* pDstBits = (byte*)m_pBits  + (nBitsSize - m_nPitch);
        ZeroMemory(pDstBits, -nBitsSize);
    }
    else
    {
        ZeroMemory(m_pBits, m_nPitch * m_nHeight);
    }
}


// ���ø�ͼƬ������λ�õ�alphaֵ����ͼһ����һ����ʱͼƬ�������ڷֲ㴰����ʹ��HDC���л���
bool  Image::SetAlpha(byte nAlpha)
{
    assert(m_hBitmap);
    if (m_nBPP != 32)
        return false;

    BYTE* pImageBits = (BYTE*)m_pBits;
    for (int row = 0; row < m_nHeight; row ++)
    {
        for (int i = 0; i < m_nWidth; i ++)
        {
            pImageBits[4*i+3] = nAlpha;
        }
        pImageBits+= m_nPitch;
    }

    return true;
}


// �޸�ͼƬ��͸����
bool Image::ModifyAlpha(ImageData* pOriginImageData, byte nAlphaPercent)
{
    assert(m_hBitmap);
    if (m_nBPP != 32)
        return false;

    if (NULL == pOriginImageData)
        return false;

    BYTE* pTemp = pOriginImageData->m_pScan0;
    if (NULL == pTemp)
        return false;

    BYTE* pNewImageBits = (BYTE*)m_pBits;
    int   bytesperpx    = m_nBPP>>3;
    int   bytesperline   = bytesperpx*m_nWidth;  /*abs(m_nPitch);*/ // ע������λ��һ��Ҫ����4��λ�ã����ܵ��µ����Ϊ������������Ჹ�����õ�λ��������ﲻ��ֱ����m_nPitch

    for (int row = 0; row < m_nHeight; row ++)
    {
        for (int i = 0; i < bytesperline; i += bytesperpx)
        {
            BYTE  a = pTemp[i+3];
            a = a * nAlphaPercent / 100;

            pNewImageBits[i+3] = a;
            pNewImageBits[i]   = pTemp[i]  *a/255;
            pNewImageBits[i+1] = pTemp[i+1]*a/255;
            pNewImageBits[i+2] = pTemp[i+2]*a/255;
        }

        pNewImageBits += m_nPitch;
        pTemp += pOriginImageData->m_nStride;
    }
    return true;
}


// libo add 20120401 ����ͼ���б���Ʒ���
bool Image::ImageList_Draw(HDC hDestDC, int x, int y, int col, int row, int cx, int cy )
{
    assert(m_hBitmap);
    assert( col >= 0 );
    assert( row >= 0 );
    assert( cx >= 0 );
    assert( cy >= 0 );

    int xSrc = col * cx;
    int ySrc = row * cy;

    if( xSrc > m_nWidth )
        return false;
    if( ySrc > m_nHeight )
        return false;

    this->Draw(hDestDC, x, y, cx, cy, xSrc, ySrc, cx, cy );

    return true;
}


// libo 20120318 ��DIBת����DDB
// hMemDC
//		[in]	Ҫ���ڼ���DDB��DC
//
bool Image::ChangeDIB2DDB(HDC hMemDC)
{
    assert( NULL != m_hBitmap );
    assert( NULL != hMemDC );

    if( false == m_bIsDIBSection )
        return true;

    HBITMAP hDDB = ::CreateCompatibleBitmap(hMemDC,this->GetWidth(),this->GetHeight());
    HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hDDB);

    this->BitBlt(hMemDC, 0,0 );
    ::SelectObject(hMemDC, hOldBitmap);

    this->Destroy();
    this->Attach(hDDB);

    return true;
}



//
//	���ݲ������ṩ��ԭʼ����pSaveBits�����Լ�ƫ��wNewHɫ��
//
bool ChangeColorHue(BYTE& R, BYTE& G, BYTE& B, short h, bool bOffsetOrReplace)
{
    if( R==G && G==B )  // ��ɫϵ���ܸı�����ɫ������ԶΪ160
        return false;

    Color c(R,G,B,255);

    HSL hsl;
    c.GetHSL(hsl);

    if (bOffsetOrReplace)
        hsl.hue += h;
    else
        hsl.hue = h;

    while(hsl.hue < MIN_HUE_VALUE)
        hsl.hue += MAX_HUE_VALUE;
    while (hsl.hue >= MAX_HUE_VALUE)
        hsl.hue -= MAX_HUE_VALUE;
    
    c.SetHSL(hsl);

    R = c.r;
    G = c.g;
    B = c.b;

    return true;
}

void ChangeColorLuminance(BYTE& R, BYTE& G, BYTE& B, short l, float dL)  // dL = l/100;
{
    if (l > 0)  // �൱�����ڱ���ͼ�������һ��ȫ�׵�ͼƬ��alphaΪdL)
    {  
        R = R + (BYTE)((255 - R) * dL);  
        G = G + (BYTE)((255 - G) * dL);  
        B = B + (BYTE)((255 - B) * dL);  
    }  
    else if (l < 0)  // �൱�����ڱ���ͼ�������һ��ȫ�ڵ�ͼƬ(alphaΪdL)
    {  
        R = R + (BYTE)(R * dL);  
        G = G + (BYTE)(G * dL);   
        B = B + (BYTE)(B * dL);  
    }  
#define CHECK_RGB_RANGE(x)  \
    if (x>255) x = 255; \
    if (x<0)   x = 0;

    CHECK_RGB_RANGE(R);
    CHECK_RGB_RANGE(G);
    CHECK_RGB_RANGE(B);
}

bool ChangeColorHueAndSaturation(BYTE& R, BYTE& G, BYTE& B, short h, bool bOffsetOrReplace, short s, float dS)
{
    if( R==G && G==B )  // ��ɫϵ���ܸı�����ɫ������ԶΪ160
        return false;

    Color c(R,G,B,255);

    HSL hsl;
    c.GetHSL(hsl);

    // hue
    if (bOffsetOrReplace)
        hsl.hue += h;
    else
        hsl.hue = h;

    while(hsl.hue < MIN_HUE_VALUE)
        hsl.hue += MAX_HUE_VALUE;
    while (hsl.hue >= MAX_HUE_VALUE)
        hsl.hue -= MAX_HUE_VALUE;

    // saturation
    if (s > 0)
        hsl.saturation = (hsl.saturation / (1-dS));
    else
        hsl.saturation = (hsl.saturation * (1+dS));

    if(hsl.saturation <= MIN_SATURATION_VALUE)
        hsl.saturation = 0, hsl.hue = 0;  // ��ɫϵ��

    if (hsl.saturation > MAX_SATURATION_VALUE)
        hsl.saturation = MAX_SATURATION_VALUE;


    c.SetHSL(hsl);
    R = c.r;
    G = c.g;
    B = c.b;

    return true;
}
bool ChangeColorSaturation(BYTE& R, BYTE& G, BYTE& B, short s, float dS)
{
    if( R==G && G==B )  // ��ɫϵ���ܸı�����ɫ������ԶΪ160
        return false;

    Color c(R,G,B,255);

    HSL hsl;
    c.GetHSL(hsl);

    // saturation
    if (s > 0)
        hsl.saturation = (hsl.saturation / (1-dS));
    else
        hsl.saturation = (hsl.saturation * (1+dS));

    if(hsl.saturation <= MIN_SATURATION_VALUE)
        hsl.saturation = 0, hsl.hue = 0;  // ��ɫϵ��

    if (hsl.saturation > MAX_SATURATION_VALUE)
        hsl.saturation = MAX_SATURATION_VALUE;

    c.SetHSL(hsl);
    R = c.r;
    G = c.g;
    B = c.b;

    return true;
}

bool ChangeColorHLS(BYTE& R, BYTE& G, BYTE& B, short h, short l , short s, int nFlag )
{
    bool bChangeH = nFlag & CHANGE_SKIN_HLS_FLAG_H ? true:false;
    bool bChangeL = nFlag & CHANGE_SKIN_HLS_FLAG_L ? true:false;
    bool bChangeS = nFlag & CHANGE_SKIN_HLS_FLAG_S ? true:false;
    bool bSetHueMode = nFlag & CHANGE_SKIN_HLS_FALG_REPLACE_MODE ? false:true;
    if (l == 0)
        bChangeL = false;
    if (s == 0)
        bChangeS = false;

    if(false == bChangeH && false == bChangeL && false == bChangeS)
        return false;

    float dL = 0, ds = 0;
    if (bChangeL)
        dL = (float)(l/100.0); 
    if (bChangeS)
        ds = (float)(s/100.0);

    if (bChangeL)
        ChangeColorLuminance(R,G,B,l,dL);

    if (bChangeH && bChangeS)
    {
        ChangeColorHueAndSaturation(R,G,B,h,bSetHueMode,s,ds);
    }
    else
    {
        if (bChangeH)
            ChangeColorHue(R,G,B,h,bSetHueMode);
        if (bChangeS)
            ChangeColorSaturation(R,G,B,s,ds);
    }
    return true;
}

bool Image::ChangeHSL(const ImageData* pOriginImageData, short h, short s, short l, int nFlag)
{
    // ���pOriginImageDataΪNULL,��ֱ���޸ĵ�ǰͼƬ
    // ȡ���Լ���imagedata
    ImageData tempImageData;
    if (NULL == pOriginImageData)
    {
        if (!this->SaveBits(&tempImageData, 0))
            return false;
        pOriginImageData = &tempImageData;
    }

    BYTE* pTemp = pOriginImageData->m_pScan0;
    if (NULL == pTemp)
        return false;

    if (m_nBPP != pOriginImageData->m_nbpp)
    {
        assert(0);
        return false;
    }

    bool bChangeH = nFlag & CHANGE_SKIN_HLS_FLAG_H ? true:false;
    bool bChangeL = nFlag & CHANGE_SKIN_HLS_FLAG_L ? true:false;
    bool bChangeS = nFlag & CHANGE_SKIN_HLS_FLAG_S ? true:false;
    bool bSetHueMode = nFlag & CHANGE_SKIN_HLS_FALG_REPLACE_MODE ? true:false;

    if (l == 0)
        bChangeL = false;
    if (s == 0)
        bChangeS = false;
    if (h == 0 && !bSetHueMode)
        bChangeH = false;

    // 		if(false == bChangeH && false == bChangeL && false == bChangeS) // �п����ǻ�ԭ��������˲���ֱ���˳� 
    // 			return false;

    BYTE* pNewImageBits = (BYTE*)m_pBits;
    int   bytesperpx    = m_nBPP>>3;
    int   bytesperline   = bytesperpx*m_nWidth;  /*abs(m_nPitch);*/ // ע������λ��һ��Ҫ����4��λ�ã����ܵ��µ����Ϊ������������Ჹ�����õ�λ��������ﲻ��ֱ����m_nPitch
    bool  bHaveAlphaChannel = GetBPP() == 32;

    float dL = 0, ds = 0;
    if (bChangeL)
        dL = (float)(l/100.0);   // ������ѭ�����ظ������ֵ
    if (bChangeS)
        ds = (float)(s/100.0);

    for (int row = 0; row < m_nHeight; row ++)
    {
        for (int i = 0; i < bytesperline; i += bytesperpx)
        {
            BYTE B = pTemp[i];
            BYTE G = pTemp[i+1];
            BYTE R = pTemp[i+2];
            BYTE A = pTemp[i+3];

            // ����Ҫ��Ԥ�ˣ�������������Ԥ�ˡ�������alphablendʱ�Ų������
            if (A != 0 && A != 255)
            {
                B = (BYTE)pTemp[i]*255/A;
                G = (BYTE)pTemp[i+1]*255/A;
                R = (BYTE)pTemp[i+2]*255/A;
            }

            if (bChangeL)
                ChangeColorLuminance(R,G,B,l,dL);

            if (bChangeH && bChangeS)
            {
                ChangeColorHueAndSaturation(R,G,B,h,!bSetHueMode,s,ds);
            }
            else
            {
                if (bChangeH)
                    ChangeColorHue(R,G,B,h,!bSetHueMode);
                if (bChangeS)
                    ChangeColorSaturation(R,G,B,s,ds);
            }

            if (A != 0 && A != 255)
            {
                // Ԥ��
                B = B*A/255;
                G = G*A/255;
                R = R*A/255;
            }

            pNewImageBits[i]   = B;
            pNewImageBits[i+1] = G;
            pNewImageBits[i+2] = R;

            if (bHaveAlphaChannel)
            {
                pNewImageBits[i+3] = pTemp[i+3];
            }
        }

        pNewImageBits += m_nPitch;
        pTemp += pOriginImageData->m_nStride;
    }

    return true;
}

// ����Ĵ����Ǹ���PHOTOSHOP�������Ľ����
// ������ʵ��������ʱ������Ĵ��벻���������㡣����
// baseThemeColor ->  newThemeColor�� ��Ҫ�ó�һ�� HSL ֵ������ȥ����һ��ԭʼͼƬ�е�
// originPixelColor  ->  newPixelColor��
// AdjustColorHSL����CalcColorDeltaHSLʹ�á�
bool AdjustColorHSL(BYTE& R, BYTE& G, BYTE& B, double h, bool bOffsetOrReplace, float dS, float dL)
{
    if( R==G && G==B )  // ��ɫϵ���ܸı�����ɫ������ԶΪ160
        return false;

    UI::Color c(R,G,B,255);

    UI::HSL hsl;
    c.GetHSL(hsl);

    // hue
    if (bOffsetOrReplace)
        hsl.hue += h;
    else
        hsl.hue = h;

    while(hsl.hue < MIN_HUE_VALUE)
        hsl.hue += MAX_HUE_VALUE;
    while (hsl.hue >= MAX_HUE_VALUE)
        hsl.hue -= MAX_HUE_VALUE;

    // saturation
    if (dS > 0)
        hsl.saturation = (hsl.saturation / (1-dS));
    else
        hsl.saturation = (hsl.saturation * (1+dS));

    // light
    if (dL > 0)
        hsl.lightness = (hsl.lightness / (1-dL));
    else
        hsl.lightness = (hsl.lightness * (1+dL));

    if(hsl.saturation <= MIN_SATURATION_VALUE)
        hsl.saturation = 0, hsl.hue = 0;  // ��ɫϵ��

    if (hsl.saturation > MAX_SATURATION_VALUE)
        hsl.saturation = MAX_SATURATION_VALUE;

    if (hsl.lightness > MAX_LUMINANCE_VALUE)
        hsl.lightness = MAX_LUMINANCE_VALUE;

    c.SetHSL(hsl);
    R = c.r;
    G = c.g;
    B = c.b;

    return true;
}

// ����color��base color֮�䣬��Ҫ��hslƫ����
// AdjustColorHSL����CalcColorDeltaHSLʹ�á�
void  CalcColorDeltaHSL(COLORREF baseColor, COLORREF color, double* pHue, double* pSat, double* pLight)
{
    // ����base theme color�������base theme colorƫ��ȡ
    // ��ǰ��theme color��Ҫ��hsl���ݣ�����ͼƬ����
    UI::Color cDest(color);
    UI::Color cSrc(baseColor);

    UI::HSL hslDest;
    UI::HSL hslSrc;
    cDest.GetHSL(hslDest);
    cSrc.GetHSL(hslSrc);

    double hue = hslDest.hue - hslSrc.hue;
    double sat = 0;
    double light = 0;

    if (hslDest.saturation > hslSrc.saturation)
        sat = (1 - (hslSrc.saturation/hslDest.saturation)) * 100;
    else if (hslSrc.saturation > hslDest.saturation)
        sat = ((hslDest.saturation/hslSrc.saturation) - 1) * 100;

    if (hslDest.lightness > hslSrc.lightness)
        light = (1 - (hslSrc.lightness/hslDest.lightness)) * 100;
    else if (hslSrc.lightness > hslDest.lightness)
        light = ((hslDest.lightness/hslSrc.lightness) - 1) * 100;

    if (*pHue)
        *pHue = hue;
    if (*pSat)
        *pSat = sat;
    if (*pLight)
        *pLight = light;
}


// Ϊ�˽���ڴ�����һ��ģ����䣬ȴ������ģ���ͷţ��������޸�Ϊ���ⲿ�Լ������ڴ�
bool Image::SaveBits(ImageData* pImageData, int* pNeedSize)
{
    if (NULL == pImageData && NULL == pNeedSize)
        return false;

    if (m_nBPP != 24 && m_nBPP != 32)  // TODO: �ݲ�֧��������ʽ�ģ�8λ���ǻ��ڵ�ɫ��ģ�
        return false;

    int   bytesperline = abs(m_nPitch);
    int nSize = bytesperline*m_nHeight;
    if (pNeedSize && *pNeedSize != nSize)
    {
        *pNeedSize = nSize;
        return false;
    }

    pImageData->m_nWidth = m_nWidth;
    pImageData->m_nHeight = m_nHeight;
    pImageData->m_nStride = m_nPitch;
    pImageData->m_nbpp = m_nBPP;

    // ֻ��ȡ��������
    if (NULL == pImageData->m_ptr)
    {
        if (pImageData->m_bNeedDeletePtr)
            return false;

        pImageData->m_pScan0 = (byte*)m_pBits;
        pImageData->m_ptr = (byte*)m_pBits;
        if (m_nPitch < 0)
            pImageData->m_ptr += ((m_nHeight-1)*m_nPitch);
    }
    else
    {
        pImageData->m_pScan0 = pImageData->m_ptr;
        if (m_nPitch < 0)
            pImageData->m_pScan0 += ((m_nHeight-1)*bytesperline);

        // �ڴ濽��
        BYTE* pThisBits = (BYTE*)m_pBits;
        BYTE* pTemp = pImageData->m_pScan0;
        for (int row = 0; row < m_nHeight; row ++ )
        {
            memcpy(pTemp, pThisBits, bytesperline);
            pThisBits += m_nPitch;
            pTemp += m_nPitch;
        }
    }
    return true;
}

void Image::RestoreBits( ImageData* pImageData )
{
    // TODO
}

void  Image::SetPixelBit32(uint x, uint y, long lValue)
{
    if (x > (uint)m_nWidth || y > (uint)m_nHeight)   
        return;

#define GetAValue(rgb)      (LOBYTE(((rgb)) >> 24))

    byte* p = ((byte*)m_pBits + y*m_nPitch);
    p[x]   = GetBValue(lValue);
    p[x+1] = GetGValue(lValue);
    p[x+2] = GetRValue(lValue);
    p[x+3] = GetAValue(lValue);
}

void  Image::CreateSpecialSizeFromGdiplusBitmap(Gdiplus::Bitmap* pBmp, long newWidth, long newHeight)
{
    if (!pBmp)
        return;

    this->Destroy();

	long w = newWidth;
	long h = newHeight;

    this->Create(w, h, 32, UI::Image::createAlphaChannel);
    HDC hDC = this->GetDC();
    {
        Gdiplus::Graphics g(hDC); 
        g.SetInterpolationMode(Gdiplus::InterpolationModeBicubic); 
        g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality); 

        Gdiplus::ImageAttributes  attr;
        attr.SetWrapMode(Gdiplus::WrapModeTileFlipXY);

        Gdiplus::RectF rDest(0, 0, (Gdiplus::REAL)w, (Gdiplus::REAL)h);
        g.DrawImage(pBmp, rDest, 
            0, 0, 
            (Gdiplus::REAL)pBmp->GetWidth(), 
            (Gdiplus::REAL)pBmp->GetHeight(),
            Gdiplus::UnitPixel, &attr);
    }
    this->ReleaseDC();
}

BOOL Image::Draw(HDC hDestDC, int xDest, int yDest, int nDestWidth,
    int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
    C9Region* pImage9Region, bool bDrawCenter, byte bAlpha) const throw()
{
    if (NULL == pImage9Region)
    {
        return this->Draw(hDestDC, xDest, yDest, nDestWidth, nDestHeight,
            xSrc, ySrc, nSrcWidth, nSrcHeight, bAlpha);
    }

    int nWDst = 0;
    int nHDst = 0;
    int nWSrc = 0;
    int nHSrc = 0;

    // 1. topleft

    this->Draw(hDestDC,

        xDest,
        yDest,
        pImage9Region->topleft,
        pImage9Region->top,

        xSrc,
        ySrc,
        pImage9Region->topleft,
        pImage9Region->top
        , bAlpha
        );

    // 2. top
    nWDst = nDestWidth - pImage9Region->topleft - pImage9Region->topright;
    nWSrc = nSrcWidth - pImage9Region->topleft - pImage9Region->topright;
    if (nWDst > 0 && nWSrc > 0)
    {
        this->Draw(hDestDC,

            xDest + pImage9Region->topleft,
            yDest,
            nWDst,
            pImage9Region->top,

            xSrc + pImage9Region->topleft,
            ySrc,
            nWSrc,
            pImage9Region->top
            , bAlpha
            );
    }

    // 3. topright

    this->Draw(hDestDC,

        xDest + nDestWidth - pImage9Region->topright,
        yDest,
        pImage9Region->topright,
        pImage9Region->top,

        xSrc + nSrcWidth - pImage9Region->topright,
        ySrc,
        pImage9Region->topright,
        pImage9Region->top
        , bAlpha
        );

    // 4. left
    nHDst = nDestHeight - pImage9Region->top - pImage9Region->bottom;
    nHSrc = nSrcHeight - pImage9Region->top - pImage9Region->bottom;
    if (nHDst > 0 && nHSrc > 0)
    {
        this->Draw(hDestDC,

            xDest,
            yDest + pImage9Region->top,
            pImage9Region->left,
            nHDst,

            xSrc,
            ySrc + pImage9Region->top,
            pImage9Region->left,
            nHSrc
            , bAlpha
            );
    }

    // 5. center
    if (bDrawCenter)
    {
        nWDst = nDestWidth - pImage9Region->left - pImage9Region->right;
        nHDst = nDestHeight - pImage9Region->top - pImage9Region->bottom;
        nWSrc = nSrcWidth - pImage9Region->left - pImage9Region->right;
        nHSrc = nSrcHeight - pImage9Region->top - pImage9Region->bottom;
        if (nHDst > 0 && nHSrc > 0 && nWDst > 0 && nWSrc > 0)
        {
            this->Draw(hDestDC,

                xDest + pImage9Region->left,
                yDest + pImage9Region->top,
                nWDst,
                nHDst,

                xSrc + pImage9Region->left,
                ySrc + pImage9Region->top,
                nWSrc,
                nHSrc
                , bAlpha
                );
        }
    }
    // 6. right

    nHDst = nDestHeight - pImage9Region->top - pImage9Region->bottom;
    nHSrc = nSrcHeight - pImage9Region->top - pImage9Region->bottom;
    if (nHDst > 0 && nHSrc > 0)
    {
        this->Draw(hDestDC,

            xDest + nDestWidth - pImage9Region->right,
            yDest + pImage9Region->top,
            pImage9Region->right,
            nHDst,

            xSrc + nSrcWidth - pImage9Region->right,
            ySrc + pImage9Region->top,
            pImage9Region->right,
            nHSrc
            , bAlpha
            );
    }

    // 7. bottomleft

    this->Draw(hDestDC,

        xDest,
        yDest + nDestHeight - pImage9Region->bottom,
        pImage9Region->bottomleft,
        pImage9Region->bottom,

        xSrc,
        ySrc + nSrcHeight - pImage9Region->bottom,
        pImage9Region->bottomleft,
        pImage9Region->bottom
        , bAlpha
        );

    // 8. bottom
    nWDst = nDestWidth - pImage9Region->bottomleft - pImage9Region->bottomright;
    nWSrc = nSrcWidth - pImage9Region->bottomleft - pImage9Region->bottomright;
    if (nWDst > 0 && nWSrc > 0)
    {
        this->Draw(hDestDC,

            xDest + pImage9Region->bottomleft,
            yDest + nDestHeight - pImage9Region->bottom,
            nWDst,
            pImage9Region->bottom,

            xSrc + pImage9Region->bottomleft,
            ySrc + nSrcHeight - pImage9Region->bottom,
            nWSrc,
            pImage9Region->bottom
            , bAlpha
            );
    }

    // 9. bottomright
    this->Draw(hDestDC,

        xDest + nDestWidth - pImage9Region->bottomright,
        yDest + nDestHeight - pImage9Region->bottom,
        pImage9Region->bottomright,
        pImage9Region->bottom,

        xSrc + nSrcWidth - pImage9Region->bottomright,
        ySrc + nSrcHeight - pImage9Region->bottom,
        pImage9Region->bottomright,
        pImage9Region->bottom
        , bAlpha
        );

    return true;
}

}