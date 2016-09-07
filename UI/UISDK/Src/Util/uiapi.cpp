#include "stdafx.h"
#include "Inc\Base\uiapi.h"
#include "Src\Base\Object\object.h"
#include "Src\Base\Application\uiapplication.h"
#include "Src\Renderlibrary\gdi\gdifont.h"
#include "Src\ATL\image.h"
#include "Inc\Util\util.h"
#include "Inc\Interface\iuiapplication.h"

// �ڲ�ȫ�ַ���
namespace UI
{
 
bool  CreateUIApplication(IUIApplication** pp)
{
    if (!pp)
        return false;

    IUIApplication* p = new IUIApplication;
    p->GetImpl()->x_Init();

    *pp = p;
    return true;
}

/*
void  UI_AttachFont(IRenderFont** pOut, HFONT hFont, GRAPHICS_RENDER_LIBRARY_TYPE eRenderType )
{
    if( NULL == hFont || NULL == pOut)
        return;

    switch (eRenderType)
    {
    case GRAPHICS_RENDER_LIBRARY_TYPE_GDI:
        {
            GDIRenderFont::CreateInstance(pOut);
            IRenderFont* pRenderFont = (IRenderFont*)*pOut;
            pRenderFont->Attach(hFont);
        }
        break;

    case GRAPHICS_RENDER_LIBRARY_TYPE_GDIPLUS:
        {
            GdiplusRenderFont::CreateInstance(pOut);
            IRenderFont* pRenderFont = (IRenderFont*)*pOut;
            pRenderFont->Attach(hFont);
        }
        break;

#ifdef UI_D2D_RENDER
    case GRAPHICS_RENDER_LIBRARY_TYPE_DIRECT2D:
        {
            Direct2DRenderFont::CreateInstance(pOut);
            IRenderFont* pRenderFont = (IRenderFont*)*pOut;
            pRenderFont->Attach(hFont);
        }
        break;
#endif

    default: 
        return ;
    }

    return ;
}
*/
void  UI_ExtendPath(String& strPath)
{
    if (FALSE == Util::IsFullPath(strPath.c_str()))
    {
        TCHAR szModulePath[MAX_PATH] = _T("");

        GetModuleFileName(/*UIApplication::GetModuleInstance()*/g_hInstance, szModulePath, MAX_PATH);
        TCHAR* szTemp = _tcsrchr(szModulePath, _T('\\'));
        if (szTemp)
            *(szTemp+1) = 0;

        String strTemp = szModulePath;
        strPath = strTemp;
        strPath.append(strPath);
    }
}


void  UI_AttachFont(IRenderFont** pOut, HFONT hFont, GRAPHICS_RENDER_LIBRARY_TYPE eRenderType)
{
	if (NULL == hFont || NULL == pOut)
		return;

	switch (eRenderType)
	{
	case GRAPHICS_RENDER_LIBRARY_TYPE_GDI:
	{
		GDIRenderFont::CreateInstance(pOut);
		IRenderFont* pRenderFont = (IRenderFont*)*pOut;
		pRenderFont->Attach(hFont);
	}
	break;

	default:
		return;
	}

	return;
}

//
// �ַ������
//
//	Remark:
//		����DLL֮��ʹ��stl��Ϊ�������д��ݣ��ᵼ���ڴ��ͷ�ʱ�ı������������û��ֱ��ȥ����UI_Split����
//		����ֱ����ULDLL����д��ôһ������
//
void  UI_Split(const String& str, TCHAR szSep, vector<String>& vRet)
{
    int nIndex = 0;

    while (true)
    {
        int nResult = (int)str.find( szSep, nIndex );
        if (-1 == nResult)
        {
            vRet.push_back( str.substr( nIndex, str.length()-nIndex ) );
            break;
        }
        else
        {
            vRet.push_back( str.substr( nIndex, nResult-nIndex ) );
            nIndex = ++nResult;
        }
    }
}

// ����֧��Getʱ����һ��LPCTSTR��ʱ����
#define GlobalTempBufferSize  256
TCHAR   g_szGlobalTempBuffer[GlobalTempBufferSize];
String  g_strGlobalTempBuffer;

TCHAR*  GetTempBuffer(int nMaxSize)
{
	UIASSERT(nMaxSize < GlobalTempBufferSize);
	memset(g_szGlobalTempBuffer, 0, sizeof(g_szGlobalTempBuffer));
	return g_szGlobalTempBuffer;
}
String&  GetTempBufferString()
{
	g_strGlobalTempBuffer.clear();
	return g_strGlobalTempBuffer;
}

// ���ڽ�һ��RECT��Сһ��PADDING/MARGIN�Ĵ�С
void DeflatRect(RECT* pfc, LPCRECT pDeflatRc)
{
	pfc->left += pDeflatRc->left;
	pfc->top += pDeflatRc->top;
	pfc->right -= pDeflatRc->right;
	pfc->bottom -= pDeflatRc->bottom;
}


HBITMAP CreateMemBitmap(int nWidth, int nHeight, int* pnPitch, byte** ppBits)
{
    Image image;
    image.Create(nWidth, nHeight, 32, Image::createAlphaChannel);

    if (ppBits)
        *ppBits = (BYTE*)image.GetBits();

    if (pnPitch)
        *pnPitch = image.GetPitch();
    
    return image.Detach();
}

// void  UILoadXmlDocument(const TCHAR* szXmlPath, IUIDocument** ppDoc)
// {
//     if (NULL == szXmlPath || NULL == ppDoc)
//         return;
// 
//     if (!PathFileExists(szXmlPath))
//         return;
// 
//     IUIDocument* p = NULL;
//     if (!CreateXmlDocument(XML_ENGINE_DEFAULT, &p))
//         return;
// 
//     if (!p->LoadFile(szXmlPath))
//     {
//         SAFE_RELEASE(p);
//         return;
//     }
// 
//     *ppDoc = static_cast<IUIDocument*>(p);
// }

// void  CreateTCHARBuffer(IBuffer** pBuffer)
// {
//     if (!pBuffer)
//         return;
// 
//     CBuffer* p = new CBuffer;
//     *pBuffer = p;
// }

} // namespace
