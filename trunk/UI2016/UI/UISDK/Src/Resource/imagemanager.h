#pragma once
#include "imageres.h"
#include "cursorres.h"
#include "gifres.h"
#include "Src\Util\DataStruct\list.h"

namespace UI
{
	interface UIElement;
	interface IUIElement;
	interface IImageManager;

class  ImageTagElementInfo
{
public:
	String  strId;  // ģ����
	UIElement*  pXmlElement;  // xml���
};

class ImageManager
{
public:
	ImageManager(SkinRes* pSkinRes);
	~ImageManager(void);

    IImageManager*  GetIImageManager();

	ImageRes&    GetImageRes();
	CursorRes&   GetCursorRes();
	GifRes&      GetGifRes();
    UIElement*  GetImageXmlElem(LPCTSTR szId);

	IImageResItem*  InsertImageItem(IMAGE_ITEM_TYPE eType, LPCTSTR szID, LPCTSTR szPath);
	bool  ModifyImageItem(LPCTSTR szID, LPCTSTR szPath);
	bool  RemoveImageItem(LPCTSTR szID);

	bool  ModifyImageItemInRunTime(LPCTSTR szID, LPCTSTR szPath);
	bool  ModifyImageItemAlpha(LPCTSTR szID, byte nAlphaPercent);

	void  Clear();
	bool  ChangeSkinHLS(short h, short l, short s, int nFlag);

	int  GetImageCount( );
	IImageResItem*  GetImageItemInfo(int nIndex);

	static HRESULT  UIParseImageTagCallback(IUIElement*, ISkinRes* pSkinRes);

private:
    // ISkinTagParse
    HRESULT  ParseNewElement(UIElement* pElem);
    void  OnNewChild(UIElement* pElem);

private:
	// ���ݳ־ò�
    IImageManager*  m_pIImageManager;

	typedef  UIList<ImageTagElementInfo>  _MyList;
	typedef  UIListItem<ImageTagElementInfo>  _MyListItem;
	_MyList  m_listUIElement;

	// ��������
	ImageRes        m_resImage;    // ����ͼƬ
	CursorRes       m_resCursor;   // �����ʽͼƬ
	GifRes          m_resGif;      // ����

	SkinRes*        m_pSkinRes;
};

} 