#pragma once
#include "fontres.h"
#include "Src\Util\DataStruct\list.h"

namespace UI
{
	interface IFontManager;
	interface UIElement;
	interface IUIElement;

class  FontTagElementInfo
{
public:
	String  strId;  // ģ����
	UIElement*  pXmlElement;  // xml���
};

class FontManager
{
public:
	FontManager(SkinRes* p);
	~FontManager(void);
   
    IFontManager*  GetIFontManager();
	FontRes&  GetFontRes();

public:
	void       Clear();
	int        GetFontCount();
	IFontResItem*  GetFontItemInfo(int nIndex);
	
    static HRESULT  UIParseFontTagCallback(IUIElement*, ISkinRes* pSkinRes);
private:
    HRESULT  ParseNewElement(UIElement* pElem);
    void  OnNewChild(UIElement* pElem);

private:
	// ���ݳ־ò�
    IFontManager*  m_pIFontManager;
   
	typedef  UIList<FontTagElementInfo>  _MyList;
	typedef  UIListItem<FontTagElementInfo>  _MyListItem;
	_MyList  m_listUIElement;

	// ��������
	FontRes   m_resFont;
	SkinRes*  m_pSkinRes;
};

}