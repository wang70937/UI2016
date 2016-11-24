#pragma once
#include "Src\SkinParse\xml\xmlwrap.h"
#include "..\Util\DataStruct\list.h"

namespace UI
{
class Object;
interface UIElement;
interface ILayoutWindowNodeList;


//  �ӳټ��صĲ�������
//  һ��xml�����У��������ö��������Ĵ��ڡ��Զ����б���
class LayoutConfigItem
{
public:
    void  SetPath(LPCTSTR szPath);
    LPCTSTR  GetPath();

    void  AddWindow(LPCTSTR szName);
    bool  FindWindow(LPCTSTR szName);

    void  AddListItem(LPCTSTR szName);
    bool  FindListItem(LPCTSTR szName);

    bool  Find(LPCTSTR szTagName, LPCTSTR szName);

    UINT  GetWindowCount();
    LPCTSTR  GetWindowName(UINT index);

private:
	String  m_strPath;
    vector<String>  m_vecWindow;
    vector<String>  m_vecListItem;
};

// �¼��ص��ӿؼ���notify����Ϊ˭
#define NOTIFY_TARGET_ROOT  (IMessage*)1  // ���ڣ�������
#define NOTIFY_TARGET_NULL  (IMessage*)0  // ��ָ��

class LayoutManager
{
public:
	LayoutManager(SkinRes*);
	~LayoutManager(void);
	
    ILayoutManager&  GetILayoutManager();

    Object*  LoadControlLayout(
                LPCTSTR szId, 
                Object* pNewParemt, 
                IMessage* pNotifyTarget = NOTIFY_TARGET_ROOT);

	UIElementProxy  FindWindowElement(LPCTSTR szTagName, LPCTSTR szId);
    UIElementProxy  FindListItemElement(LPCTSTR szId);

    Object*  ParseElement(
                UIElement* pUIElement, 
                Object* pParent, 
                IMessage* pNotifyTarget = NOTIFY_TARGET_ROOT);
    Object*  ParseChildElement(
                UIElement* pParentElement, 
                Object* pParent,
                IMessage* pNotifyTarget = NOTIFY_TARGET_ROOT);

	bool  ReLoadLayout(
                Object* pRootObj, 
                list<Object*>& listAllChild );
	void  ReloadChildObjects(
                Object* pObjParent,
                UIElement* pObjElement,
                list<Object*>& listAllChild);

    static HRESULT  UIParseLayoutTagCallback(
                IUIElement*, ISkinRes* pSkinRes);
	static HRESULT  UIParseLayoutConfigTagCallback(
                IUIElement*, ISkinRes* pSkinRes);
	
	// �༭��ר�ú���
	bool  LoadWindowNodeList(ILayoutWindowNodeList** pp);

private:
    void  ParseNewElement(UIElement* pElem);
	void  ParseLayoutConfigTag(UIElement* pElem);
    bool  ParseLayoutConfigFileTag(UIElement* pElem);

    UIElementProxy  find_element_from_cache(LPCTSTR szTagName, LPCTSTR szId);
	UIElementProxy  load_window_by_id(LPCTSTR szTagName, LPCTSTR szId);
	UIElementProxy  load_element_from_layout_config(LPCTSTR szTagName, LPCTSTR szId);
	bool  testUIElement(UIElement* pParentElem, LPCTSTR szTagName, LPCTSTR szId);

private:
    ILayoutManager*  m_pILayoutManager;
    UIList<UIElement*>  m_listUIElement;  // �Ѽ��صĲ�����<layout>����б�
	UIList<LayoutConfigItem*>  m_listLayoutConfig;  // ��δ���صĲ�����

	// ��������
	SkinRes*         m_pSkinRes;
};

}