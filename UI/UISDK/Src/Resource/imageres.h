#pragma once

namespace UI
{
	interface IImageResItem;
	interface IRenderBitmap;
	interface IImageRes;
class ImageData;
class GDIRenderBitmap;
class GdiplusRenderBitmap;
class SkinRes;

//
//	image�е�һ����Ϣ����
//
class ImageResItem
{
public:
	ImageResItem();
	~ImageResItem();

	bool  ModifyImage(LPCTSTR  szPath);
    IMAGE_ITEM_TYPE  GetImageItemType();
    void  SetImageItemType(IMAGE_ITEM_TYPE e);

	DECLARE_STRING_SETGET(Id);
	DECLARE_STRING_SETGET(Path);
	DECLARE_bool_SETGET(UseSkinHLS);
	DECLARE_bool_SETGET(NeedAntiAliasing)

    bool  NeedDpiAdapt();
	void  SetFileDpiScale(int n){
			m_nFileDpiScale = (byte)n;
	}

public:
	bool  ModifyHLS(short h, short l, short s, int nFlag);
	bool  ModifyHLS(IRenderBitmap* pBitmap, short h, short l, short s, int nFlag);
	bool  ModifyAlpha(byte nAlphaPercent);
	IRenderBitmap*  GetImage(SkinRes* pSkinRes, GRAPHICS_RENDER_LIBRARY_TYPE eRenderType = GRAPHICS_RENDER_LIBRARY_TYPE_GDI, bool* pbFirstTimeCreate=NULL);
    IRenderBitmap*  GetImage_gdi(SkinRes* pSkinRes, bool* pbFirstTimeCreate=NULL);

	bool  IsMyRenderBitmap(IRenderBitmap* pRenderBitmap);

    // ����Ixxx�ӿ�
    virtual IImageResItem*  GetIImageResItem(); 
    // ��ȡxml���ԣ���imagelist��layout��count
	virtual void  SetAttribute(IMapAttribute* pMapAttrib);
    // �ڴ�����RenderBitmap֮���ɸ�������ȥΪ��������չ����
    virtual void  SetRenderBitmapAttribute(IRenderBitmap* pRenderBitmap);

protected:
	IImageResItem*  m_pIImageResItem;

    String    m_strId;      // image id
    String    m_strPath;    // image path
	byte   m_nFileDpiScale;  // ͼƬdpi�Ŵ�ϵ����@2x @3x ...

    bool  m_bUseSkinHLS;                   // ��ͼƬ�Ƿ����Ƥ��ɫ���ı� 
    bool  m_bNeedAntiAliasing;             // ��Ҫ֧�ֿ����(gdi��alphablend���Ų�֧��SetStretchBltMode��HALFTONE)
    bool  m_bMustHasAlphaChannel;          // ��ͼƬ�Ƿ���Ҫ����alpha channel��1. ��alpha channel��ͼƬһ�ʲ���gdi����������alphablend���ơ�2. û��alpha channel��gdiͼƬ���޷��ڷֲ㴰���������
    bool  m_bDpiAdapt;                     // �Ƿ�����ӦDPI����

    IMapAttribute*        m_pMapAttrib;    // Ϊ��ͼƬ���õ����ԣ�����imagelist��count��icon��width height
    IMAGE_ITEM_TYPE       m_eType;         // ͼƬ����

    ImageData*            m_pOriginImageData;    // ��ͼƬ��ԭʼ���ݣ��ı�ͼƬɫ��ʱʹ�ã�
    GDIRenderBitmap*      m_pGdiBitmap;          // �ⲿ���ã��ó�Ա���������ü���

    //  ע��Ϊ����߻�ͼЧ�ʣ�ͬʱҲ����һ��ͼƬ���ض�����ʽ������ʹ��GDI(AlphaBlend)����ͼ��
    //	    ��Ҫע����ǣ������Ҫʹ��alphaͨ��ʱ������ǿ�ƴ���һ��32λ��GDI BITMAP
    // GdiplusRenderBitmap*  m_pGdiplusBitmap;      // �ⲿ����
};

class ImageListResItem : public ImageResItem
{
public:
    ImageListResItem();

    virtual IImageResItem*  GetIImageResItem(); 
    virtual void  SetAttribute(IMapAttribute* pMapAttrib);
    virtual void  SetRenderBitmapAttribute(IRenderBitmap* pRenderBitmap);

    IMAGELIST_LAYOUT_TYPE  GetLayoutType();
    int  GetItemCount();
    void  SetLayoutType(IMAGELIST_LAYOUT_TYPE);
    void  SetItemCount(int);

private:
    IMAGELIST_LAYOUT_TYPE   m_eLayoutType;
    int  m_nCount;
};

class ImageIconResItem : public ImageResItem
{
public:
    ImageIconResItem();

    virtual IImageResItem*  GetIImageResItem(); 
    virtual void  SetAttribute(IMapAttribute* pMapAttrib);
    virtual void  SetRenderBitmapAttribute(IRenderBitmap* pRenderBitmap);

private:
    SIZE  m_sizeDraw;
};

//
//	image�б�
//
class ImageRes
{
public:
	ImageRes(SkinRes*  pSkinRes);
	~ImageRes();

	IImageRes*  GetIImageRes();

	long  GetImageCount();
	IImageResItem*  GetImageResItem(long lIndex);
	IImageResItem*  GetImageResItem(LPCTSTR szID);
	bool ModifyImage(LPCTSTR szId, LPCTSTR szPath);

	HBITMAP  LoadBitmap(LPCTSTR szId);
	bool  GetBitmap(LPCTSTR szImageID, GRAPHICS_RENDER_LIBRARY_TYPE eRenderType, __out IRenderBitmap** pRenderBitmap);

	LPCTSTR  GetRenderBitmapId(IRenderBitmap* pBitmap);

public:
	ImageResItem*  LoadItem(LPCTSTR szType, IMapAttribute* pMapAttrib, LPCTSTR szFullPath);

	ImageResItem*  GetImageItem2(int nIndex);
	ImageResItem*  GetImageItem2(LPCTSTR szId);

	ImageResItem*  InsertImage(IMAGE_ITEM_TYPE eType, LPCTSTR szId, LPCTSTR szPath);
	bool RemoveImage(LPCTSTR szId);
    bool RemoveImage(IImageResItem* pItem);
	bool Clear();
	bool ChangeSkinHLS(short h, short l, short s, int nFlag);
	bool ModifyImageItemAlpha(const String& strID, byte nAlphaPercent);

private: 
	IImageRes*  m_pIImageRes;
	SkinRes*  m_pSkinRes;

    typedef  map<String,  ImageResItem*>::iterator  _MyIter;
    map<String,  ImageResItem*>   m_mapImages;
    //vector<ImageResItem*>   m_vImages;   // vector�Ĳ���Ч��̫���ˣ���˻���map
};

}