#pragma once
#include "Inc\Interface\iskindatasource.h"
#include "colormanager.h"
#include "imagemanager.h"
#include "fontmanager.h"
#include "stylemanager.h"
#include "layoutmanager.h"
#include "i18nmanager.h"

namespace UI
{
class UIApplication;
class Object;
class SkinManager;
interface  UIDocument;
interface  SkinDataSource;
struct SKIN_HLS_INFO;

enum SKIN_RES_LOAD_STATE
{
    SKIN_RES_LOAD_STATE_NONE,     // ��new SkinRes
    SKIN_RES_LOAD_STATE_LOADED,   // �Ѽ���    
};


//
//	һ��Ƥ����Դ����
//	
class SkinRes
{
public:
    SkinRes(SkinManager& o);
	~SkinRes();

	bool  Load();
	bool  Unload();
	bool  Save();
	LPCTSTR  GetName();
	LPCTSTR  GetPath();
	void  SetParam(SKIN_PACKET_TYPE eType, LPCTSTR szName, LPCTSTR szPath);
	bool  ReLoadLayout(Object* pRootObj, list<Object*>& listAllChild);
	bool  ChangeSkinHLS(short h, short l, short s, int nFlag);

	ISkinRes*        GetISkinRes();
    UIApplication*   GetUIApplication();
    ISkinManager*    GetISkinManager();

    IImageManager*   GetIImageManager()   { return m_mgrImage.GetIImageManager(); }
    IColorManager*   GetIColorManager()   { return m_mgrColor.GetIColorManager(); }
    IFontManager*    GetIFontManager()    { return m_mgrFont.GetIFontManager(); }
    IStyleManager*   GetIStyleManager()   { return m_mgrStyle.GetIStyleManager(); }
    ILayoutManager*  GetILayoutManager()  { return m_mgrLayout.GetILayoutManager(); }
    IImageRes*       GetIImageRes()       { return m_mgrImage.GetImageRes().GetIImageRes(); }
    IFontRes*        GetIFontRes()        { return m_mgrFont.GetFontRes().GetIFontRes(); }
    IColorRes*       GetIColorRes()       { return m_mgrColor.GetColorRes().GetIColorRes(); }
    IStyleRes*       GetIStyleRes()       { return m_mgrStyle.GetStyleRes().GetIStyleRes(); }
	

	SkinManager&     GetSkinMgr()         { return m_mgrSkinRef; } //  �ڲ�����
	ImageManager&    GetImageManager()    { return m_mgrImage;  }
	ColorManager&    GetColorManager()    { return m_mgrColor;  }
	FontManager&     GetFontManager()     { return m_mgrFont;   }
	StyleManager&    GetStyleManager()    { return m_mgrStyle;  }
	LayoutManager&   GetLayoutManager()   { return m_mgrLayout; }
	SKIN_HLS_INFO*   GetHLSInfo()         { return m_pHLSInfo; }
	SkinDataSource*  GetDataSource()      { return m_pDataSource; }
	I18nManager&     GetI18nManager()     { return m_mgrI18n; }

	ImageRes&  GetImageRes();
	ColorRes&  GetColorRes();
	FontRes&   GetFontRes();
	I18nRes&   GetI18nRes();

    UINT  GetImageCount()    { return m_mgrImage.GetImageCount(); }
    UINT  GetColorCount()    { return m_mgrColor.GetColorCount(); }
    UINT  GetFontCount()     { return m_mgrFont.GetFontCount(); }
    UINT  GetStyleCount()    { return m_mgrStyle.GetStyleCount(); }
    UINT  GetXmlDocCount()   { return m_listDoc.size(); }

	IImageResItem*  GetImageItemInfo(UINT nIndex);
	IColorResItem*  GetColorItemInfo(UINT nIndex);
	IFontResItem*  GetFontItemInfo(UINT nIndex);
	UIDocument*  GetXmlDoc(UINT nIndex);
	UIDocument*  GetXmlDocByName(LPCTSTR szName);

	void  OnNewUIDocument(UIDocument* pDoc);    
	
	void  SetParentSkinRes(SkinRes*);
	SkinRes*  GetParentSkinRes();

private:
    ISkinRes*  m_pISkinRes;
	SkinManager&  m_mgrSkinRef;

	// ��һ������Դ�������Ҳ���ʱ�����ϲ���
	// ���ڲ����Դ�����ø�һ������Դ
	SkinRes*  m_pParentSkinRes;  

	String  m_strSkinResName;
    SkinDataSource*  m_pDataSource;
	ColorManager   m_mgrColor;
	ImageManager   m_mgrImage;
	FontManager    m_mgrFont;
	StyleManager   m_mgrStyle;
	LayoutManager  m_mgrLayout;
	I18nManager    m_mgrI18n;

	SKIN_RES_LOAD_STATE  m_eLoadState;
    SKIN_HLS_INFO*  m_pHLSInfo;

    typedef vector<UIDocument*> _DocList;
    _DocList  m_listDoc;   // ����xml�ĵ�ָ��
};

}