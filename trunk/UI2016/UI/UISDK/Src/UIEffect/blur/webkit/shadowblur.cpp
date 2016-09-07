#include "stdafx.h"
#include "shadowblur.h"
#include <math.h>

// Ҫ�㣺
//
// 1. ����box blur����Gaussian blur
// 2. 3��box blur��Ч���ӽ���Gaussian blur
// 3. һ��box blur��ʵ��Ϊһ�� Horz motion blur + Vert motion blur
// 4. 
//
// Box blurs are frequently used to approximate a Gaussian blur.
// By the central limit theorem, if applied 3 times on the same image, 
// a box blur approximates the Gaussian kernel to within about 3%, 
// yielding the same result as a quadratic convolution kernel.
//
//
// Instead doing a 2D box blur on an image, you can first do a 
// horizontal motion blur, and then a vertical motion blur.
// This actually create an image that is equivalent to a box 
// blurred image! 
// This speeds it up from O(m^2*n^2) to O(m^2*2n)!
//

/* http://www.w3.org/TR/SVG/filters.html#feGaussianBlurElement

	15.17 Filter primitive ��feGaussianBlur��

	This filter primitive performs a Gaussian blur on the input image.
	The Gaussian blur kernel is an approximation of the normalized convolution:
	G(x,y) = H(x)I(y)

	where
		H(x) = exp(-x2/ (2s2)) / sqrt(2* pi*s2)
	and
		I(y) = exp(-y2/ (2t2)) / sqrt(2* pi*t2)

	  with 's' being the standard deviation in the x direction and 't' being the 
	standard deviation in the y direction, as specified by ��stdDeviation��.

	  The value of ��stdDeviation�� can be either one or two numbers. If two numbers 
	are provided, the first number represents a standard deviation value along the
	x-axis of the current coordinate system and the second value represents a standard
	deviation in Y. If one number is provided, then that value is used for both X and Y.

	  Even if only one value is provided for ��stdDeviation��, this can be 
	implemented as a separable convolution.

	  For larger values of 's' (s >= 2.0), an approximation can be used: Three successive 
	box-blurs build a piece-wise quadratic convolution kernel, which approximates
	the Gaussian kernel to within roughly 3%.

	  let d = floor(s * 3*sqrt(2*pi)/4 + 0.5)

		... if d is odd, use three box-blurs of size 'd', centered on the output pixel.
		... if d is even, two box-blurs of size 'd' (the first one centered on the pixel 
		    boundary between the output pixel and the one to the left, the second one 
		    centered on the pixel boundary between the output pixel and the one to the
		    right) and one box blur of size 'd+1' centered on the output pixel.
*/
namespace UI
{

enum
{
    leftLobe = 0,
    rightLobe = 1
};


// ���ö����������������ٶȣ���ƽ������
// Instead of integer division, we use 17.15 for fixed-point division.
static const int blurSumShift = 15;

// ����radius��stdDeviation����˹��̫�ֲ��ı�׼�����dֵ���ó�ÿһ��box blur�ķ�Χ��
// ��radius����2ʱ�����Բ�������Box blur��ģ��Gaussian blur��
//
//  d = floor(s * 3*sqrt(2*pi)/4 + 0.5)
//
// Takes a two dimensional array with three rows and two columns for the lobes.
static void calculateLobes(int lobes[][2], float blurRadius, bool shadowsIgnoreTransforms)
{
    int diameter;
    if (shadowsIgnoreTransforms)
	{
        diameter = max(2, static_cast<int>(floorf((2 / 3.f) * blurRadius))); // Canvas shadow. FIXME: we should adjust the blur radius higher up.
	}
    else 
	{
        // http://dev.w3.org/csswg/css3-background/#box-shadow
        // Approximate a Gaussian blur with a standard deviation equal to half the blur radius,
        // which http://www.w3.org/TR/SVG/filters.html#feGaussianBlurElement tell us how to do.
        // However, shadows rendered according to that spec will extend a little further than m_blurRadius,
        // so we apply a fudge factor to bring the radius down slightly.
        float stdDev = blurRadius / 2;
        const float piFloat = 3.14159265358979f;
        const float gaussianKernelFactor = 3 / 4.f * sqrtf(2 * piFloat);
        const float fudgeFactor = 0.88f;
        diameter = max(2, static_cast<int>(floorf(stdDev * gaussianKernelFactor * fudgeFactor + 0.5f)));
    }

	// Ϊ�������� 7 = 3(left) + 1(output) + 3(right)
    if (diameter & 1) 
	{
        // if d is odd, use three box-blurs of size 'd', centered on the output pixel.
        int lobeSize = (diameter - 1) / 2;
        lobes[0][leftLobe] = lobeSize;
        lobes[0][rightLobe] = lobeSize;
        lobes[1][leftLobe] = lobeSize;
        lobes[1][rightLobe] = lobeSize;
        lobes[2][leftLobe] = lobeSize;
        lobes[2][rightLobe] = lobeSize;
    } 
	// Ϊż��������box blur������һ������Ϊƫ��+ƫ��+ƫ����
	else
	{
        // if d is even, two box-blurs of size 'd' (the first one centered on the pixel boundary
        // between the output pixel and the one to the left, the second one centered on the pixel
        // boundary between the output pixel and the one to the right) and one box blur of size 'd+1' centered on the output pixel
        int lobeSize = diameter / 2;
        lobes[0][leftLobe] = lobeSize;
        lobes[0][rightLobe] = lobeSize - 1;
        lobes[1][leftLobe] = lobeSize - 1;
        lobes[1][rightLobe] = lobeSize;
        lobes[2][leftLobe] = lobeSize;
        lobes[2][rightLobe] = lobeSize;
    }
}

void /*ShadowBlur::*/blurLayerImage(byte* imageData, const SIZE& size, int rowStride, RECT* prcBlur, int nBlurRadius)
{
	// ����box blur����Ϊ��
	// Step 1: blur alpha channel and store the result in the blue channel
	// Step 2: blur blue channel and store the result in the green channel
	// Step 3: blur green channel and store the result in the alpha channel
	// ��Ϊʲô���±�һ���ڴ汣����??��
	//
	// ����������alpha channel������ȷ��ֵ����blue��green channel��ֵ����Ⱦ��
	// �����ǿ��Խ�rgb channel����������Ϊ��������Ҫ��shadow color����ҪԤ�ˣ�
    const int channels[4] = { 3, 0, 1, 3 };  // �ֱ����{alpha --> blue -->  green --> alpha}

    int lobes[3][2]; // indexed by pass, and left/right lobe
    calculateLobes(lobes, (float)nBlurRadius, false/*m_shadowsIgnoreTransforms*/);  // m_shadowsIgnoreTransforms��ʲô����?

    // First pass is horizontal.
    int stride = 4;        // 1����(byte)
    int pitch = rowStride; // �м��(byte)

	int jstart = prcBlur->top;
    int jfinal = prcBlur->bottom;

    int dim = prcBlur->right - prcBlur->left;     
	// Box blur �ֽ�Ϊһ��Horz motion blur��һ��Vert motion blur
    // Two stages: horizontal and vertical
    for (int pass = 0; pass < 2; ++pass) 
	{
        unsigned char* pixels = imageData + prcBlur->top*pitch + prcBlur->left*stride;

        if (!pass && !nBlurRadius)
            jfinal = jstart; // Do no work if horizonal blur is zero.

        for (int j = jstart; j < jfinal; ++j, pixels += pitch) 
		{
            // For each step, we blur the alpha in a channel and store the result
            // in another channel for the subsequent step.
            // We use sliding window algorithm to accumulate the alpha values.
            // This is much more efficient than computing the sum of each pixels
            // covered by the box kernel size for each x.
			// ���������㷨:(����d=5)
			//
			//  ������������ڿڿڿ�   //  ���ĸ�������ƽ�� sum4 = p2+p3+p4+p5+p6    , alpha4 = sum4 / 5
			//  1 2 3 4 5 6 7 8 9 10 
			//  �ڿ�����������ڿڿ�   //  �����������ƽ�� sum5 =    p3+p4+p5+p6+p7 , alpha5 = sum5 / 5
			//
			//  ==> avg5 = avg4 - p2 + p7�� ���ֻҪ�������1��sum�����ܺܿ������һ��sum
			//
            for (int step = 0; step < 3; ++step) 
			{
                int side1 = lobes[step][leftLobe];
                int side2 = lobes[step][rightLobe];
                int pixelCount = side1 + 1 + side2;
                int invCount = ((1 << blurSumShift) + pixelCount - 1) / pixelCount;  // ������ת��Ϊ�˷���ƽ��
                int ofs = 1 + side2;
                int firstalpha = pixels[channels[step]];
                int lastalpha = pixels[(dim - 1) * stride + channels[step]];

                unsigned char* ptr = pixels + channels[step + 1];  // output channel
                unsigned char* prev = pixels + stride + channels[step];  
                unsigned char* next = pixels + ofs * stride + channels[step];

				// ���߽紦��һ����side1������
                int i;
                int sum = side1 * firstalpha + firstalpha;   // sum = ��� + �Լ�
                int limit = (dim < side2 + 1) ? dim : side2 + 1;  // �Ҳ�Խ������

                for (i = 1; i < limit; ++i, prev += stride)  // sum += �Ҳ�
                    sum += *prev;

                if (limit <= side2)
                    sum += (side2 - limit + 1) * lastalpha;

                limit = (side1 < dim) ? side1 : dim;
                for (i = 0; i < limit; ptr += stride, next += stride, ++i, ++ofs) 
				{
                    *ptr = (unsigned char)((sum * invCount) >> blurSumShift);  // avg: pixel = sum / count
                    sum += ((ofs < dim) ? *next : lastalpha) - firstalpha;  // nextsum = sum + next - prev
                }
                
				// �м䴦��
                prev = pixels + channels[step];
                for (; ofs < dim; ptr += stride, prev += stride, next += stride, ++i, ++ofs) 
				{
                    *ptr = (unsigned char)((sum * invCount) >> blurSumShift);
                    sum += (*next) - (*prev);
                }
                
				// �Ҳ�߽紦��
                for (; i < dim; ptr += stride, prev += stride, ++i) 
				{
                    *ptr = (unsigned char)((sum * invCount) >> blurSumShift);
                    sum += lastalpha - (*prev);
                }
            }
        }

        // Last pass is vertical.
        stride = rowStride;
        pitch = 4;
		jstart = prcBlur->left;
        jfinal = prcBlur->right;
        dim = prcBlur->bottom - prcBlur->top;
    }
}

#if 0
// Check http://www.w3.org/TR/SVG/filters.html#feGaussianBlur.
// As noted in the SVG filter specification, running box blur 3x
// approximates a real gaussian blur nicely.
void /*ContextShadow::*/blurLayerImage(unsigned char* imageData, const SIZE& size, int rowStride, RECT* prc)
{
    // Instead of integer division, we use 17.15 for fixed-point division.
    static const int BlurSumShift = 15;

    int m_blurDistance = 128;
    int channels[4] =  { 3, 0, 1, 3 };
    int d = max(2, static_cast<int>(floorf((2 / 3.f) * m_blurDistance)));
    int dmax = d >> 1;
    int dmin = dmax - 1 + (d & 1);
    if (dmin < 0)
        dmin = 0;

    // Two stages: horizontal and vertical
    for (int k = 0; k < 2; ++k) 
    {
        unsigned char* pixels = imageData;
        int stride = (!k) ? 4 : rowStride;
        int delta = (!k) ? rowStride : 4;

#ifndef BLUR32_WEBKIT_REGION
        int jfinal = (!k) ? size.cy : size.cx;
        int dim = (!k) ? size.cx : size.cy;
        int jstart = 0;
#else
        // ����blru����Χ
        int jfinal = (!k) ? prc->bottom : prc->right;
        int dim = (!k) ? prc->right : prc->bottom;
        int jstart = (!k) ? prc->top : prc->left;
        int pixelstart = (!k)? prc->left : prc->top;
        pixelstart *= delta;
#endif
        for (int j = jstart; j < jfinal; ++j, pixels += delta) 
        {

#ifdef BLUR32_WEBKIT_REGION
            pixels += pixelstart;
#endif
            // For each step, we blur the alpha in a channel and store the result
            // in another channel for the subsequent step.
            // We use sliding window algorithm to accumulate the alpha values.
            // This is much more efficient than computing the sum of each pixels
            // covered by the box kernel size for each x.

            for (int step = 0; step < 3; ++step) 
            {
                int side1 = (!step) ? dmin : dmax;
                int side2 = (step == 1) ? dmin : dmax;
                int pixelCount = side1 + 1 + side2;
                int invCount = ((1 << BlurSumShift) + pixelCount - 1) / pixelCount;
                int ofs = 1 + side2;
                int alpha1 = pixels[channels[step]];
                int alpha2 = pixels[(dim - 1) * stride + channels[step]];
                unsigned char* ptr = pixels + channels[step + 1];
                unsigned char* prev = pixels + stride + channels[step];
                unsigned char* next = pixels + ofs * stride + channels[step];

                int i;
                int sum = side1 * alpha1 + alpha1;
                int limit = (dim < side2 + 1) ? dim : side2 + 1;
                for (i = 1; i < limit; ++i, prev += stride)
                    sum += *prev;
                if (limit <= side2)
                    sum += (side2 - limit + 1) * alpha2;

                limit = (side1 < dim) ? side1 : dim;
                for (i = 0; i < limit; ptr += stride, next += stride, ++i, ++ofs) {
                    *ptr = (sum * invCount) >> BlurSumShift;
                    sum += ((ofs < dim) ? *next : alpha2) - alpha1;
                }
                prev = pixels + channels[step];
                for (; ofs < dim; ptr += stride, prev += stride, next += stride, ++i, ++ofs) {
                    *ptr = (sum * invCount) >> BlurSumShift;
                    sum += (*next) - (*prev);
                }
                for (; i < dim; ptr += stride, prev += stride, ++i) {
                    *ptr = (sum * invCount) >> BlurSumShift;
                    sum += alpha2 - (*prev);
                }
            }
        }
    }
}
#endif

void  ShadowBlur(HBITMAP hBitmap, COLORREF colorShadow, RECT* prcBlur, int nRadius)
{
	// Ĭ����top down
	RECT rcBlur = {0};
	BITMAP bm = {0};
	GetObject(hBitmap, sizeof(bm), &bm);
	SIZE s = {bm.bmWidth, bm.bmHeight};

	if (bm.bmBitsPixel != 32)
		return;

	if (prcBlur)
	{
		RECT rcBmp = {0, 0, bm.bmWidth, bm.bmHeight};
		if (FALSE == ::IntersectRect(&rcBlur, &rcBmp, prcBlur))
			return;
	}
	else
	{
		::SetRect(&rcBlur, 0, 0, bm.bmWidth, bm.bmHeight);
	}

	// blur
	blurLayerImage((byte*)bm.bmBits, s, bm.bmWidthBytes, &rcBlur, nRadius);


	// �����ɫ������pre-multi������ᵼ��alphablend�쳣

	byte r = GetRValue(colorShadow);
	byte g = GetGValue(colorShadow);
	byte b = GetBValue(colorShadow);
    byte* pLineBit = (byte*)bm.bmBits + rcBlur.top*bm.bmWidthBytes;

	if (RGB(r,g,b) == RGB(255,255,255))  // ���⴦���/��
	{
        for (int y = rcBlur.top; y < rcBlur.bottom; y++)
        {
            byte* pBit = pLineBit + rcBlur.left * 4;
            for (int x = rcBlur.left; x < rcBlur.right; x++, pBit+=4)
            {
                pBit[0] = pBit[3];
                pBit[1] = pBit[3];
                pBit[2] = pBit[3];
            }
            pLineBit += bm.bmWidthBytes;
        }
	}
	else if (RGB(r,g,b) == RGB(0,0,0))
	{
        for (int y = rcBlur.top; y < rcBlur.bottom; y++)
        {
            byte* pBit = pLineBit + rcBlur.left * 4;
            for (int x = rcBlur.left; x < rcBlur.right; x++, pBit+=4)
            {
                *((DWORD*)pBit) = pBit[3]<<24;
//                 pBit[0] = 0;
//                 pBit[1] = 0;
//                 pBit[2] = 0;
            }
            pLineBit += bm.bmWidthBytes;
        }
	}
	else
	{
        for (int y = rcBlur.top; y < rcBlur.bottom; y++)
        {
            byte* pBit = pLineBit + rcBlur.left * 4;
            for (int x = rcBlur.left; x < rcBlur.right; x++, pBit+=4)
            {
                pBit[0] = b * pBit[3] >> 8;  // >>8 �� /255 
                pBit[1] = g * pBit[3] >> 8;
                pBit[2] = r * pBit[3] >> 8;
            }
            pLineBit += bm.bmWidthBytes;
        }
	}
}

}