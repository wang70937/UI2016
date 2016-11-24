#pragma once

namespace UI
{
	class SkinRes;
	interface IStyleRes;

// 1. �ؼ�����һ��class��ʽ��
//    styleclass="imagebtn;aaa" 
// 
// 2. Tag ��ʽ
//       <CustomWindow font="normal" transparentrgn.type="4corner" transparentrgn.9region="5"/>
// 
// 3. Id ��ʽ 
//       <id id="imagebtn" xxx="yyy"/>
//
// 4. class ��ʽ
//       <class id="imagebtn" xxx="yyy"/>
//

interface IStyleResItem;
interface UIElement;

class StyleResItem
{
public:
	StyleResItem();
	~StyleResItem();
    IStyleResItem*  GetIStyleResItem();

    LPCTSTR  GetId() { 
        return m_strID.c_str();
    }
    void  SetId(LPCTSTR szId){
        if (szId)
            m_strID = szId;
        else
            m_strID.clear();
    }

	STYLE_SELECTOR_TYPE  GetSelectorType() { return m_eSelectorType; }
	void  SetSelectorType(STYLE_SELECTOR_TYPE type) { m_eSelectorType = type; }
	
	LPCTSTR  GetInherits() { return m_strInherits.c_str(); }
	void  SetInherits(LPCTSTR sz);
	int   GetInheritCount();
	bool  GetInheritItem(int i, String& str);
	bool  RemoveInheritItem(LPCTSTR sz);

	void  SetXmlElement(UIElement* p);
	UIElement*  GetXmlElement();
	bool  GetXmlElement2(UIElement** pp);

    void  SetAttributeMap(IMapAttribute* pMapAttrib);
	void  GetAttributeMap(IMapAttribute** ppMapAttrib);
	void  SetAttribute( LPCTSTR key, LPCTSTR value);
	LPCTSTR  GetAttribute(LPCTSTR key);
	bool  ModifyAttribute(LPCTSTR key, LPCTSTR value);
	bool  RemoveAttribute(LPCTSTR key);

	bool  InheritAttribute(LPCTSTR key, LPCTSTR value);
	bool  InheritMyAttributesToAnother(StyleResItem* pChild);
	bool  Apply(IMapAttribute* pMapAttrib, bool bOverwrite);

private:
    IStyleResItem*  m_pIStyleResItem;

	STYLE_SELECTOR_TYPE  m_eSelectorType;
	String  m_strID;          // ��ѡ������ID
	String  m_strInherits;    // �̳��б�
    IMapAttribute*  m_pMapAttrib;  // ��ѡ���������Լ�

	// �ǳ־�����
	vector<String>  m_vInheritList;   // �̳��б������н���m_strInherits

	// editor����
	UIElement*  m_pXmlElement;
};

class StyleRes
{
public:
    StyleRes(SkinRes* p);
	~StyleRes();
    IStyleRes&  GetIStyleRes();
	void  Clear();

public:
	StyleResItem*  Add(STYLE_SELECTOR_TYPE type, LPCTSTR szId, LPCTSTR szInherit);
	StyleResItem*  Insert(STYLE_SELECTOR_TYPE type, LPCTSTR szId, long lPos);
	bool  Modify(StyleResItem* pItem, STYLE_SELECTOR_TYPE type, LPCTSTR szId, LPCTSTR szInherit);
	bool  Remove(STYLE_SELECTOR_TYPE type, LPCTSTR szId);

	bool  AddAttribute(StyleResItem* pItem, LPCTSTR szKey, LPCTSTR szValue);
	bool  ModifyAttribute(StyleResItem* pItem, LPCTSTR szKey, LPCTSTR szValue);
	bool  RemoveAttribute(StyleResItem* pItem, LPCTSTR szKey);

	bool  Add(StyleResItem* pNewItem);
	bool  Remove(StyleResItem* pNewItem);

    long  GetCount();
	StyleResItem* GetItem(long nIndex);
	long  GetItemPos(StyleResItem* p);
	StyleResItem* GetItem(STYLE_SELECTOR_TYPE type, LPCTSTR szID);

	bool  LoadStyle(LPCTSTR szTagName, LPCTSTR szStyleClass, LPCTSTR szID, IMapAttribute* pMapAttribute);
    bool  UnloadStyle(LPCTSTR szTagName, LPCTSTR szStyleClass, LPCTSTR szID, IListAttribute* pListAttribte);

private:
    IStyleRes*  m_pIStyleRes;

	SkinRes*  m_pSkinRes;
	vector<StyleResItem*>  m_vStyles;
};
}