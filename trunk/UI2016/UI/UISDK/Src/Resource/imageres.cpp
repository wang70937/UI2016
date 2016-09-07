#include "stdafx.h"
#include "imageres.h"
#include "Src\Renderlibrary\gdi\gdibitmap.h"
#include "Src\Renderlibrary\gdiplus\gdiplusbitmap.h"
#include "Src\Resource\skinres.h"
#include "Inc\Util\iimage.h"
#include "Inc\Interface\iuires.h"
#include "Src\SkinParse\datasource\skindatasource.h"
#include "Src\Util\DPI\dpihelper.h"
#include "Inc\Interface\imapattr.h"
#include "uiresource.h"
#include "Src\Base\Application\uiapplication.h"
#include <math.h>

using namespace UI;

ImageResItem::ImageResItem()
{
	m_pMapAttrib = NULL;
	m_pGdiBitmap = NULL;
	//m_pGdiplusBitmap = NULL;
	m_pOriginImageData = NULL;
	m_pIImageResItem = NULL;

    m_bDpiAdapt = true;
	m_bUseSkinHLS = false;
	m_bNeedAntiAliasing = false;
	m_bMustHasAlphaChannel = true;  // Ĭ�϶�����32λalpha channel�����ڴ󲿷�gdi renderҲ֧��fix alpha channel��
	m_eType = IMAGE_ITEM_TYPE_IMAGE;
	m_nFileDpiScale = 1;
}

ImageResItem::~ImageResItem()
{
    // ��ֱ��ɾ�������пؼ����������ͼƬ������UIEditor��ɾ��һ��ͼƬ�����������߸����ͷ������õ�ͼƬָ��
    if (m_pGdiBitmap)
    {
        m_pGdiBitmap->Destroy();
		m_pGdiBitmap->SetOutRef(NULL); // ע��nnd��������һ��ŷ���û���ÿյ��µ�Ī������ı������Լ��������ˣ���֪ͨ��ë���������Ź�
    }
//     if (m_pGdiplusBitmap)
//     {
//         m_pGdiplusBitmap->Destroy();
// 		m_pGdiplusBitmap->SetOutRef(NULL);
//     }
	SAFE_DELETE(m_pOriginImageData);
	SAFE_RELEASE(m_pMapAttrib);
	SAFE_DELETE(m_pIImageResItem);
}

IImageResItem*  ImageResItem::GetIImageResItem()
{
	if (NULL == m_pIImageResItem)
		m_pIImageResItem = new IImageResItem(this);

	return m_pIImageResItem;
}

IMAGE_ITEM_TYPE  ImageResItem::GetImageItemType()
{
    return m_eType;
}
void  ImageResItem::SetImageItemType(IMAGE_ITEM_TYPE e)
{
    if (e == IMAGE_ITEM_TYPE_IMAGE ||
        e == IMAGE_ITEM_TYPE_ICON || 
        e == IMAGE_ITEM_TYPE_IMAGE_LIST)
        m_eType = e;
}

bool  ImageResItem::IsMyRenderBitmap(IRenderBitmap* pRenderBitmap)
{
	if (m_pGdiBitmap == pRenderBitmap) 
	{
		return true;
	}  
	return false;
}

IRenderBitmap* ImageResItem::GetImage(SkinRes* pSkinRes, GRAPHICS_RENDER_LIBRARY_TYPE eRenderType, bool* pbFirstTimeCreate)
{
	if (NULL == pSkinRes)
		return NULL;

	switch (eRenderType)
	{
	case GRAPHICS_RENDER_LIBRARY_TYPE_GDI:
		 return GetImage_gdi(pSkinRes, pbFirstTimeCreate);	

// 	case GRAPHICS_RENDER_LIBRARY_TYPE_GDIPLUS:
//         return GetImage_gdiplus(pSkinRes, pbFirstTimeCreate);	
// 
//     case GRAPHICS_RENDER_LIBRARY_TYPE_DIRECT2D:
//         return GetImage_d2d(pSkinRes, pbFirstTimeCreate);
// 
//     case GRAPHICS_RENDER_LIBRARY_TYPE_DIRECT3D:
//         return GetImage_d3d(pSkinRes, pbFirstTimeCreate);
	}

	return NULL;
}

IRenderBitmap*  ImageResItem::GetImage_gdi(SkinRes* pSkinRes, bool* pbFirstTimeCreate)
{
    SkinDataSource* pDataSource = pSkinRes->GetDataSource();
    if (NULL == pDataSource)
        return NULL;

	if (m_bNeedAntiAliasing)
	{
		// TODO:
		UIASSERT(0);
		//return GetImage_gdiplus(pSkinRes, pbFirstTimeCreate);
	}

    if (m_pGdiBitmap )
    {
        m_pGdiBitmap->AddRef();
        return (IRenderBitmap*)m_pGdiBitmap;
    }    

    RenderBitmapFactory::CreateInstance(
        pSkinRes->GetUIApplication()->GetIUIApplication(),
        GRAPHICS_RENDER_LIBRARY_TYPE_GDI,
        m_eType, 
        (IRenderBitmap**)&m_pGdiBitmap);

    if (!m_pGdiBitmap)
        return NULL;
    
    m_pGdiBitmap->SetOutRef((IRenderResource **)&m_pGdiBitmap);
    SetRenderBitmapAttribute(m_pGdiBitmap);

    /*if (false == m_pGdiBitmap->LoadFromFile(m_strPath.c_str(), m_bMustHasAlphaChannel))*/
	// 2015.2.11 gdiҲ��������͸�������ˡ��ɴཫ����ͼƬ������Ϊ32λ�ģ�ʡ��gdi��jpgͼƬ�޷���͸����������ʾ
    long flags = RENDER_BITMAP_LOAD_CREATE_ALPHA_CHANNEL;
	if (NeedDpiAdapt())
	{
		flags |= RENDER_BITMAP_LOAD_DPI_ADAPT;
		flags |= (m_nFileDpiScale) << 24;
	}

    if (false == pDataSource->Load_RenderBitmap(m_pGdiBitmap, 
                    m_strPath.c_str(), 
                    (RENDER_BITMAP_LOAD_FLAG)flags))  
    {
        SAFE_RELEASE(m_pGdiBitmap);
        UI_LOG_ERROR(_T("Load gdi bitmap from file failed. path=%s"), 
            m_strPath.c_str());
        return NULL;
    }

    if (pbFirstTimeCreate)
        *pbFirstTimeCreate = true;

    return (IRenderBitmap*)m_pGdiBitmap;
}


bool  ImageResItem::ModifyHLS(short h, short l, short s, int nFlag)
{
	if (false == m_bUseSkinHLS)
		return true;

	ModifyHLS(m_pGdiBitmap, h,l,s,nFlag);

	// 	if (m_pGdiBitmap)
	// 		m_pGdiBitmap->GetBitmap()->Save(_T("c:\\aaa.png"),Gdiplus::ImageFormatPNG);
	return true;
}
bool ImageResItem::ModifyHLS(IRenderBitmap* pBitmap, short h, short l, short s, int nFlag)
{
	if (false == m_bUseSkinHLS)
		return true;

	if (pBitmap)
	{
		if (NULL == m_pOriginImageData)
		{
			m_pOriginImageData = new ImageData;
			if (false == pBitmap->SaveBits(m_pOriginImageData))
			{
				UI_LOG_WARN(_T("not support this image to change hue. id=%s"), m_strId.c_str() );
				m_bUseSkinHLS = false;
				SAFE_DELETE(m_pOriginImageData);
			}
		}
		return pBitmap->ChangeHSL(m_pOriginImageData, h,s,l,nFlag);
	}
	return false;
}

bool ImageResItem::ModifyAlpha(byte nAlphaPercent)
{
	if (m_pGdiBitmap)
	{
		if (NULL == m_pOriginImageData)
		{
			m_pOriginImageData = new ImageData;
			if (false == m_pGdiBitmap->SaveBits(m_pOriginImageData))
			{
				UI_LOG_WARN(_T("not support this image to change alpha. id=%s"), m_strId.c_str() );
				SAFE_DELETE(m_pOriginImageData);
			}
		}
		return m_pGdiBitmap->GetBitmap()->ModifyAlpha(m_pOriginImageData, nAlphaPercent);
	}
	return false;
}

//
// �޸�ͼƬ
//
// ע�⣺1. �������Ͳ���֪��image�Ѿ������´�������ʵ����Ϊû��ָ�뱻����
//       2. ������һ�����⣬�����һ�μ��ص���32λ��alpha��ͼƬ�����س�ȥ�Ľ���GdiBitmap��
//          ��ʱ�����modifyΪһ����32λ��λͼ��������GdiBitmapû��alphaͨ�����ڷֲ㴰����
//          ��ʾʧ��
//
bool ImageResItem::ModifyImage(LPCTSTR szPath)
{
	if (NULL == szPath)
		return false;

	m_strPath = szPath;

	SAFE_DELETE(m_pOriginImageData);

	if (m_pGdiBitmap)
	{
        long flags = 0;
        if (m_bMustHasAlphaChannel)
            flags |= RENDER_BITMAP_LOAD_CREATE_ALPHA_CHANNEL;
        if (NeedDpiAdapt())
            flags |= RENDER_BITMAP_LOAD_DPI_ADAPT;
		if (!m_pGdiBitmap->LoadFromFile(m_strPath.c_str(), (RENDER_BITMAP_LOAD_FLAG)flags))
		{
			UI_LOG_ERROR(_T("load from file failed 1:%s"), m_strPath.c_str());
		}
	}

//     long flags = RENDER_BITMAP_LOAD_CREATE_ALPHA_CHANNEL;
//     if (NeedDpiAdapt())
//         flags |= RENDER_BITMAP_LOAD_DPI_ADAPT;
// 	if (m_pGdiplusBitmap)
// 	{
// 		if (!m_pGdiplusBitmap->LoadFromFile(m_strPath.c_str(), (RENDER_BITMAP_LOAD_FLAG)flags))
// 		{
// 			UI_LOG_ERROR(_T("load from file failed 2:%s"), m_strPath.c_str());
// 		}
// 	}

	return true;
}

bool  ImageResItem::NeedDpiAdapt()
{
    if (!m_bDpiAdapt)
        return false;

    if (GetDpi() == DEFAULT_SCREEN_DPI)
        return false;

    return true;
}
void ImageResItem::SetAttribute(IMapAttribute* pMapAttrib) 
{ 
	m_pMapAttrib = pMapAttrib;
	m_pMapAttrib->AddRef();

	bool  bUseSkinHLS = false;
	bool  bNeedAntiAliasing = false;
    bool  bDpiAdapt = true;

	pMapAttrib->GetAttr_bool(XML_IMAGE_THEMEHLS, true, &bUseSkinHLS);
	this->SetUseSkinHLS(bUseSkinHLS);

	pMapAttrib->GetAttr_bool(XML_IMAGE_NEED_ANTIALIASING, true, &bNeedAntiAliasing);
	this->SetNeedAntiAliasing(bNeedAntiAliasing);

    pMapAttrib->GetAttr_bool(XML_IMAGE_DPI_ADAPT, true, &bDpiAdapt);
    m_bDpiAdapt = bDpiAdapt;
}

// TODO: �ú����Ժ󽫱��ɵ�������һ��iconû���ع�
void  ImageResItem::SetRenderBitmapAttribute(IRenderBitmap* pRenderBitmap)
{
}
//////////////////////////////////////////////////////////////////////////
ImageListResItem::ImageListResItem()
{
    m_eType = IMAGE_ITEM_TYPE_IMAGE_LIST;
    m_eLayoutType = IMAGELIST_LAYOUT_TYPE_H;
    m_nCount = 1;
}

IImageResItem*  ImageListResItem::GetIImageResItem()
{
    if (NULL == m_pIImageResItem)
        m_pIImageResItem = new IImageListResItem(this);

    return m_pIImageResItem;   
}
IMAGELIST_LAYOUT_TYPE  ImageListResItem::GetLayoutType()
{
    return m_eLayoutType;
}

int  ImageListResItem::GetItemCount()
{
    return m_nCount;
}
void  ImageListResItem::SetLayoutType(IMAGELIST_LAYOUT_TYPE e)
{
    m_eLayoutType = e;
}
void  ImageListResItem::SetItemCount(int n)
{   
    m_nCount = n;
}
void  ImageListResItem::SetAttribute(IMapAttribute* pMapAttrib)
{
    __super::SetAttribute(pMapAttrib);

    LPCTSTR szText = pMapAttrib->GetAttr(XML_IMAGE_IMAGELIST_LAYOUT, true);
    if (szText)
    {
        if (0 == _tcscmp(szText, XML_IMAGE_IMAGELIST_LAYOUT_V))
            m_eLayoutType = IMAGELIST_LAYOUT_TYPE_V;
        else
            m_eLayoutType = IMAGELIST_LAYOUT_TYPE_H;
    }

    pMapAttrib->GetAttr_int(XML_IMAGE_IMAGELIST_COUNT, true, &m_nCount);
}

void  ImageListResItem::SetRenderBitmapAttribute(IRenderBitmap* pRenderBitmap)
{
    if (!pRenderBitmap)
        return;

    if (pRenderBitmap->GetImageType() != IMAGE_ITEM_TYPE_IMAGE_LIST)
        return;

    IImageListRenderBitmap* pImageListBmp = static_cast<IImageListRenderBitmap*>(pRenderBitmap);
    pImageListBmp->SetItemCount(m_nCount);
    pImageListBmp->SetLayoutType(m_eLayoutType);
}


//////////////////////////////////////////////////////////////////////////

ImageIconResItem::ImageIconResItem()
{
    m_eType = IMAGE_ITEM_TYPE_ICON;
    m_sizeDraw.cx = 16;
    m_sizeDraw.cx = 16;
}

IImageResItem*  ImageIconResItem::GetIImageResItem()
{
    if (NULL == m_pIImageResItem)
        m_pIImageResItem = new IImageIconResItem(this);

    return m_pIImageResItem;   
}
void  ImageIconResItem::SetAttribute(IMapAttribute* pMapAttrib)
{
    __super::SetAttribute(pMapAttrib);

    pMapAttrib->GetAttr_int(XML_IMAGE_ICON_WIDTH,  true, (int*)&m_sizeDraw.cx);
    pMapAttrib->GetAttr_int(XML_IMAGE_ICON_HEIGHT, true, (int*)&m_sizeDraw.cy);
}

void  ImageIconResItem::SetRenderBitmapAttribute(IRenderBitmap* pRenderBitmap)
{
    if (!pRenderBitmap)
        return;

    if (pRenderBitmap->GetImageType() != IMAGE_ITEM_TYPE_ICON)
        return;

    IImageIconRenderBitmap* pIcon = static_cast<IImageIconRenderBitmap*>(pRenderBitmap);
    pIcon->SetDrawSize(&m_sizeDraw);
}

//////////////////////////////////////////////////////////////////////////
//
//	ImageRes
//

ImageRes::ImageRes(SkinRes*  pSkinRes)
{
	m_pSkinRes = pSkinRes; 
	m_pIImageRes = NULL;
}

ImageRes::~ImageRes()
{	
	this->Clear(); 
	SAFE_DELETE(m_pIImageRes);
}

IImageRes*  ImageRes::GetIImageRes()
{
	if (NULL == m_pIImageRes)
		m_pIImageRes = new IImageRes(this);

	return m_pIImageRes;
}

//
// ���ļ��м���һ��(��CXmlImageParse::load_from_file�е���)
//
ImageResItem* ImageRes::LoadItem(LPCTSTR szType, IMapAttribute* pMapAttrib, LPCTSTR szFullPath)
{
    if (!szType || !pMapAttrib || !szFullPath)
        return NULL;

    LPCTSTR szId = pMapAttrib->GetAttr(XML_ID, true);
    if (!szId)
        return NULL;

    IMAGE_ITEM_TYPE eType = IMAGE_ITEM_TYPE_IMAGE;
    if (0 == _tcscmp(szType, XML_IMAGE_ITEM_TYPE_ICON))
    {
        eType = IMAGE_ITEM_TYPE_ICON;
    }
    else if (0 == _tcscmp(szType, XML_IMAGE_ITEM_TYPE_IMAGELIST))
    {
        eType = IMAGE_ITEM_TYPE_IMAGE_LIST;
    }
    else 
    {
        int lLength = _tcslen(szFullPath);
        if (lLength > 4 && 0 == _tcsicmp((szFullPath+lLength-4), _T(".ico")))
        {
            eType = IMAGE_ITEM_TYPE_ICON;
        }
        else
        {
            eType = IMAGE_ITEM_TYPE_IMAGE;
        }
    }

	ImageResItem* pItem = this->InsertImage(eType, szId, szFullPath);
	if (!pItem)
	{
        UI_LOG_WARN( _T("insert image m_strID=%s, path=%s failed."), szId, szFullPath);
        return NULL;
	}
	
    pItem->SetAttribute(pMapAttrib);
    return pItem;
}

long ImageRes::GetImageCount() 
{
    return (long)m_mapImages.size();
}

IImageResItem* ImageRes::GetImageResItem(long lIndex)
{
	ImageResItem* pItem = this->GetImageItem2(lIndex);
	if (NULL == pItem)
		return NULL;

	return pItem->GetIImageResItem();
}
IImageResItem*  ImageRes::GetImageResItem(LPCTSTR szId)
{
	if (NULL == szId)
		return NULL;

    ImageResItem* p = GetImageItem2(szId);
    if (p)
        return p->GetIImageResItem();
    return NULL;
}

ImageResItem* ImageRes::GetImageItem2(int nIndex)
{
	if (nIndex < 0)
		return NULL;

    if (nIndex >= (int)m_mapImages.size())
        return NULL;

    _MyIter iter = m_mapImages.begin();
    for (int i = 0; i < nIndex; i++)
    {
        iter++;
    }

    return iter->second;
}

ImageResItem* ImageRes::GetImageItem2(LPCTSTR szId)
{
    String strId(szId);
    _MyIter iter = m_mapImages.find(strId);
    if (iter == m_mapImages.end())
        return NULL;

    return  iter->second;
}

ImageResItem*  ImageRes::InsertImage(IMAGE_ITEM_TYPE eType, LPCTSTR szId, LPCTSTR szPath)
{
    if (!szId || !szPath)
        return NULL;

    ImageResItem* pItem = this->GetImageItem2(szId);
    if (pItem)
    {
        UI_LOG_WARN(_T("failed, insert item=%s, path=%s"), szId, szPath);
        return NULL;
    }

    ImageResItem*  pImageItem = NULL;
    switch (eType)
    {
    case IMAGE_ITEM_TYPE_IMAGE_LIST:
        pImageItem = new ImageListResItem;
        break;

    case IMAGE_ITEM_TYPE_ICON:
        pImageItem = new ImageIconResItem;
        break;

    default:
        pImageItem = new ImageResItem;
        break;
    }

    if (!pImageItem)
        return NULL;


	pImageItem->SetId(szId);
	pImageItem->SetPath(szPath);

	// DPI���䣬�����ļ��Ƿ���@2x,@3xͼƬ
	do 
	{
		if (UI::GetDpi() == DEFAULT_SCREEN_DPI)
			break;

		SkinDataSource* pDataSource = m_pSkinRes->GetDataSource();
		if (!pDataSource)
			break;

		float fScale = GetDpiScale();
		int nScale = (int)ceil(fScale);

		// ���ļ�·��������������װ
		LPCTSTR p = _tcsrchr(szPath, TEXT('.'));
		if (!p)
		{
			UI_LOG_WARN(_T("invalid path: %s"), szPath);
			break;
		}

		String str(szPath);
		String::size_type pos = str.find_last_of(TEXT('.'));
		if (pos == String::npos)
		{
			UI_LOG_WARN(_T("invalid path: %s"), szPath);
			break;
		}

			
		TCHAR szInsert[16] = { 0 };
		for (int i = nScale; i > 1; i--)
		{
			_stprintf(szInsert, TEXT("@%dx"), i);
			String strTest(str);
			strTest.insert(pos, szInsert);

			if (pDataSource->FileExist(strTest.c_str()))
			{
				pImageItem->SetPath(strTest);
				pImageItem->SetFileDpiScale(i);

				break;
			}
		}
	} while (0);
	

	String  strId(szId);
    m_mapImages[strId] = pImageItem;
	return pImageItem;
}

bool ImageRes::ModifyImage(LPCTSTR szId, LPCTSTR szPath)
{
	if (NULL == szId || NULL == szPath)
		return false;

	ImageResItem* p = this->GetImageItem2(szId);
	if (p)
	{
		bool bRet = p->ModifyImage(szPath);
		if (p->GetUseSkinHLS())
		{
			// ��鵱ǰƤ����HLS
			if (m_pSkinRes && m_pSkinRes->GetHLSInfo())
			{
				SKIN_HLS_INFO* pHLSInfo = m_pSkinRes->GetHLSInfo();
				p->ModifyHLS(pHLSInfo->h, pHLSInfo->l, pHLSInfo->s, pHLSInfo->nFlag);
			}
		}

		return bRet;
	}

	UI_LOG_WARN(_T("failed. modify image id=%s, path=%s"), szId, szPath);
	return false;
}
bool ImageRes::RemoveImage(LPCTSTR szId)
{
    if (!szId)
        return false;

    String strId(szId);
    _MyIter iter = m_mapImages.find(strId);
    if (iter == m_mapImages.end())
        return false;

    ImageResItem* p = iter->second;
    delete p;
    m_mapImages.erase(iter);
    return true;
}
bool ImageRes::RemoveImage(IImageResItem* pItem)
{
    if (!pItem)
        return false;

    ImageResItem* pItemImpl = pItem->GetImpl();
    
    _MyIter iter = m_mapImages.begin();
    for (; iter != m_mapImages.end(); iter++)
    {
        if (iter->second == pItemImpl)
        {
            delete pItemImpl;
            m_mapImages.erase(iter);
            return true;
        }
    }

    return false;
}
bool ImageRes::Clear()
{
    _MyIter iter = m_mapImages.begin();
    for (; iter != m_mapImages.end(); iter++)
    {
        ImageResItem* p = iter->second;
        delete p;
    }
    m_mapImages.clear();
	return true;
}

bool ImageRes::ChangeSkinHLS(short h, short l, short s, int nFlag)
{
    _MyIter iter = m_mapImages.begin();
    for (; iter != m_mapImages.end(); iter++)
    {
        ImageResItem* p = iter->second;
        p->ModifyHLS(h,l,s,nFlag);
    }

	return true;
}

bool ImageRes::ModifyImageItemAlpha(const String& strID, byte nAlphaPercent)
{
    _MyIter iter = m_mapImages.find(strID);
    if (iter == m_mapImages.end())
        return false;

    ImageResItem* p = iter->second;
    return p->ModifyAlpha(nAlphaPercent);

	return false;
}

HBITMAP  ImageRes::LoadBitmap(LPCTSTR szId)
{
	if (NULL == szId)
		return NULL;

	ImageResItem* pItem = this->GetImageItem2(szId);
	if (NULL == pItem)
	{
		UI_LOG_ERROR( _T("GetImageItem��%s failed .1"), szId);
		return NULL;
	}

	SkinDataSource* pDataSource = m_pSkinRes->GetDataSource();
	if (NULL == pDataSource)
		return NULL;

	ImageWrap image;
	pDataSource->Load_Image(pItem->GetPath(), &image);    

	if (image.IsNull())
		return NULL;

	return image.Detach();
}

bool  ImageRes::GetBitmap(LPCTSTR szImageID, GRAPHICS_RENDER_LIBRARY_TYPE eRenderType, __out IRenderBitmap** pRenderBitmap)
{
	if (NULL == szImageID || NULL == pRenderBitmap)
		return false;

	ImageResItem* pItem = this->GetImageItem2(szImageID);
	if (NULL == pItem)
	{
		// ��ȡʧ�ܣ���������һ����Դ��ȡ
		if (m_pSkinRes->GetParentSkinRes())
		{
			return m_pSkinRes->GetParentSkinRes()->
				GetImageRes().GetBitmap(szImageID, eRenderType, pRenderBitmap);
		}

		UI_LOG_ERROR( _T("GetImageItem��%s failed .1"), szImageID);
		return false;
	}

	bool bFirstTimeCreate = false;
	IRenderBitmap* pBitmap = pItem->GetImage(m_pSkinRes, eRenderType, &bFirstTimeCreate);
	if (NULL == pBitmap)
	{
		UI_LOG_ERROR( _T("GetImage��%s failed .2"), szImageID);
		return false;
	}

	if (bFirstTimeCreate && pItem->GetUseSkinHLS())
	{
		// ��鵱ǰƤ����HLS
		if (m_pSkinRes && m_pSkinRes->GetHLSInfo())
		{
			SKIN_HLS_INFO* pHLSInfo = m_pSkinRes->GetHLSInfo();
			pItem->ModifyHLS(pBitmap, pHLSInfo->h, pHLSInfo->l, pHLSInfo->s, pHLSInfo->nFlag);
		}
	}

	*pRenderBitmap = pBitmap;
	return true;
}

LPCTSTR  ImageRes::GetRenderBitmapId(IRenderBitmap* pBitmap)
{
	if (!pBitmap)
		return NULL;

	_MyIter iter = m_mapImages.begin();
	for (; iter != m_mapImages.end(); iter++)
	{
		if (iter->second->IsMyRenderBitmap(pBitmap))
		{
			return iter->second->GetId();
		}
	}
	return NULL;
}
