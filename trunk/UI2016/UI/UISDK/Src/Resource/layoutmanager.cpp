#include "StdAfx.h"
#include "layoutmanager.h"

#include "Src\Resource\uiresource.h"
#include "Src\Resource\skinres.h"
#include "Inc\Interface\ixmlwrap.h"
#include "Src\Base\Object\object.h"
#include "Src\Resource\stylemanager.h"
#include "Src\SkinParse\xml\xmlwrap.h"
#include "Inc\Interface\iuires.h"
#include "..\SkinParse\skinparseengine.h"
#include "Inc\Interface\iuiapplication.h"
#include "..\Base\Application\uiapplication.h"
#include "..\UIObject\Window\windowbase.h"

LayoutManager::LayoutManager(SkinRes* p)
{
	m_pSkinRes = p;
    m_pILayoutManager = NULL;
}

LayoutManager::~LayoutManager(void)
{
	m_listUIElement.RELEASE_ALL();
    m_listLayoutConfig.DELETE_ALL();
    SAFE_DELETE(m_pILayoutManager);
}

ILayoutManager*  LayoutManager::GetILayoutManager()
{
    if (NULL == m_pILayoutManager)
        m_pILayoutManager = new ILayoutManager(this);

    return m_pILayoutManager;
}


// ��0��ʼ������һ��������
// ����szTagNameΪNULL����ʱֻƥ��szId
IObject*  LayoutManager::LoadLayout(LPCTSTR szWndName, LPCTSTR szWndId)
{
	UIElementProxy objUIElement = this->FindWindowElement(szWndName, szWndId);

	if (!objUIElement)
	{
		UI_LOG_FATAL(_T("δ�ҵ�Ҫ���صĶ���name=%s, id=%s"), szWndName, szWndId);
		return NULL;
	}    
	IObject* pObject = this->ParseElement(objUIElement.get(), NULL);
	return pObject;
}

// �ⲿֻ֪��һ������id�����õ��ô��ڵ�tagname�õ���uiapplication->createinstancebyname
// bool  LayoutManager::GetWindowNameById(LPCTSTR szId, BSTR* bstrName)
// {
//     UIElement* pElement = NULL;
//     if (!FindWindowElement(NULL, szId, &pElement))
//         return false;
// 
//     pElement->GetTagName(bstrName);
//     return true;
// }

// 1. �ӻ���m_listUIElement�в��ҽ�㣬ֱ��������skin.xml��
// 2. ��szId.xml��Ƥ����Ŀ¼�в��ң��������maindlg����ƥ��maindlg.xml
// 3. ��layoutconfig�е����ü��أ�����<layoutconfig> <item id="" path=""/> </layoutconfig>
UIElementProxy  LayoutManager::FindWindowElement(
	LPCTSTR szTagName, LPCTSTR szId)
{
	UIElementProxy proxy;

	if (!m_pSkinRes || !szId)
		return proxy;

	// 1. �ӻ���m_listUIElement�в��ҽ��
	proxy = find_element_from_cache(szTagName, szId);
	if (proxy)
		return proxy;

	// UIEditor����֧��������ʽ������ȥ����Ŀ¼���Ȳ�֧����
#if 0
	// 2. ��szId.xml��Ƥ����Ŀ¼�в���
	if (loadWindowById(szTagName, szId, ppElem))
		return true;
#endif

	// 3. �������м���ָ������id
	proxy = load_element_from_layout_config(szTagName, szId);
	if (proxy)
		return proxy;

	SkinRes* pParentRes = m_pSkinRes->GetParentSkinRes();
	if (pParentRes)
	{
		return pParentRes->GetLayoutManager().FindWindowElement(szTagName, szId);
	}
	return proxy;
}

UIElementProxy  LayoutManager::FindListItemElement(LPCTSTR szId)
{
    UIElementProxy proxy;

    if (!m_pSkinRes || !szId)
        return proxy;

    // 1. �ӻ���m_listUIElement�в��ҽ��
    proxy = find_element_from_cache(XML_LISTITEM_, szId);
    if (proxy)
        return proxy;

    // 2. �������м���ָ������id
    proxy = load_element_from_layout_config(XML_LISTITEM_, szId);
    if (proxy)
        return proxy;

    return proxy;
}

UIElementProxy  LayoutManager::load_window_by_id(LPCTSTR szTagName, LPCTSTR szId)
{
    UIElementProxy proxy;
	SkinDataSource* pDataSource = m_pSkinRes->GetDataSource();
	if (!pDataSource)
		return proxy;

	String  strFile(szId);
	strFile.append(TEXT(".") XML_XML);

	SkinParseEngine engine(m_pSkinRes);
	if (engine.Parse(pDataSource, strFile.c_str()))
	{
		// TODO: parseֻ�ǽ��ļ���ӵ��б��У��������´��б����õ�xml���
		// �д��Ż�
		proxy = find_element_from_cache(szTagName, szId);
        if (proxy)
			return proxy;
	}

	return proxy;
}

// ��<layout>�������ָ����tag + id������UIElement
UIElementProxy  LayoutManager::find_element_from_cache(
                        LPCTSTR szTagName, LPCTSTR szId)
{
    // ����szTagNameΪ�գ�ֻƥ��szId
    if (!szId)
        return UIElementProxy();

    UIListItem<UIElement*>* pUIElementItem = m_listUIElement.GetFirst();
    while (pUIElementItem)
	{
		UIElementProxy childElem =(**pUIElementItem)->FirstChild();
		while (childElem)
		{
			if (testUIElement(childElem.get(), szTagName, szId))
            {
				return childElem;
            }

            childElem = childElem->NextElement();
		}
		pUIElementItem = pUIElementItem->GetNext();
	}

    return UIElementProxy();
}

UIElementProxy  LayoutManager::load_element_from_layout_config(
					LPCTSTR szTagName, LPCTSTR szId)
{
	if (!szId)
		return UIElementProxy();

    SkinDataSource* pDataSource = m_pSkinRes->GetDataSource();
    if (!pDataSource)
        return UIElementProxy();

	UIListItem<LayoutConfigItem*>* pListItem = m_listLayoutConfig.GetFirst();
	while (pListItem)
	{
        LayoutConfigItem* pConfigItem = **pListItem;
		if (!pConfigItem->Find(szTagName, szId))
        {
            pListItem = pListItem->GetNext();
            continue;
        }

		SkinParseEngine engine(m_pSkinRes);
		if (engine.Parse(pDataSource, pConfigItem->GetPath()))
		{
			// TODO: parseֻ�ǽ��ļ���ӵ��б��У��������´���
			// �����õ�xml��㡣�д��Ż�
            UIElementProxy proxy = find_element_from_cache(szTagName, szId);
            if (proxy)
			{
				m_listLayoutConfig.Remove(pConfigItem);
                SAFE_DELETE(pConfigItem);
				return proxy;
			}
		}

		pListItem = pListItem->GetNext();
	}

	return UIElementProxy();
}

// ����һ������tag��id  
// ���Բ�ָ��szTagName
bool  LayoutManager::testUIElement(
        UIElement* pElem, LPCTSTR szTagName, LPCTSTR szId)
{
	if (!pElem || !szId)
		return false;

	CComBSTR  bstrId;

	LPCTSTR szTagName2 = pElem->GetTagName();
    if (szTagName && szTagName2 && 0 != _tcscmp(szTagName2, szTagName))
        return false;

	pElem->GetAttrib(XML_ID, &bstrId);
	if (bstrId != szId)
		return false;

	return true;
}


// �ݹ飬���������Ӷ����Ӷ��������
void  LayoutManager::ParseChildElement(
            UIElement* pParentElement, IObject* pIObjParent)
{
	if (!pIObjParent || !pParentElement)
		return;

    UIElementProxy child = pParentElement->FirstChild();
    while (child) 
    {
        ParseElement(child.get(), pIObjParent);        
        child = child->NextElement();
    } 
}

bool IsTrue(LPCTSTR szValue)
{
	if (0 == _tcscmp(szValue, XML_BOOL_VALUE_TRUE)  || 
		0 == _tcscmp(szValue, XML_BOOL_VALUE_1)     ||
		0 == _tcscmp(szValue, XML_BOOL_VALUE_YES))
	{
		return true;
	}
	return false;
}

// �õ�һ��Ԫ��ָ�룬������Ԫ�ؼ����ӽ�㣬�����ض���ָ��
IObject*  LayoutManager::ParseElement(UIElement* pUIElement, IObject* pParent)
{
    UIApplication*  pUIApp = m_pSkinRes->GetUIApplication();

    Object*  pObj = NULL;
    CComBSTR  bstrTagName = pUIElement->GetTagName();

    // ����ǩ��ת��Ϊ��
	PARSE_CONTROL_RETURN eParseRet = ParseControl_CreateObject;

	// �����ֶΣ�������
	if (0 == wcscmp(XML_PROP, bstrTagName))
		return NULL;

    IObject* pIObject = pUIApp->CreateUIObjectByName(bstrTagName, m_pSkinRes->GetISkinRes());
    if (NULL == pIObject)
    {
        // ����Ѱ�Ҹ�Tag�Ƿ�ע����
        pfnParseControlTag  func = NULL;
        pUIApp->GetControlTagParseFunc(bstrTagName, &func);
        if (func)
        {
            eParseRet = func(
                    pUIElement->GetIUIElement(), 
                    m_pSkinRes->GetISkinRes(), 
                    pParent, 
                    &pIObject);

			if (eParseRet == ParseControl_Failed)
            {
                UI_LOG_ERROR(_T("Parse Object:  %s Failed."), (BSTR)bstrTagName);    
				return NULL;
            }
        }
        else
        {
			UI_LOG_ERROR(_T("CreateObject Failed. name=%s"), (BSTR)bstrTagName);
			return NULL;
        }
    }

    if (NULL == pIObject)
        return NULL;

    pObj = pIObject->GetImpl();

	if (eParseRet < ParseControl_LoadObject)
	{
        // �ȵ����������¸���������font/bitmap����ʱ����ȡ��window��graphics type
        pObj->SetParentObjectDirect(pParent->GetImpl());  
        {
            // �Լ�������
		    pObj->LoadAttributeFromXml(pUIElement, false);
        }
        pObj->SetParentObjectDirect(NULL);
		
		// �ȼ������ԣ���ȡ��zorder��ncchild����֮����addchild
		if (pParent)
		{
			OBJSTYLE s = {0};
			s.ncobject = 1;
			if (pObj->TestObjectStyle(s))
			{
				pParent->GetImpl()->AddNcChild(pObj);
			}
			else
			{
				pParent->GetImpl()->AddChild(pObj);
			}
		}
	}

	if (eParseRet < ParseControl_LoadDescendants)
	{
		// �����Ӷ���
		ParseChildElement(pUIElement, pIObject);
	}

    // Ĭ�Ͻ��ÿؼ����һ��notify objectΪ���ڶ��󣬲���msgmapid����Ϊ0
    WindowBase* pWindowBase = pObj->GetWindowObject(); 
    if (pObj != pWindowBase && pWindowBase)
        pObj->SetNotify(pWindowBase->GetIMessage(), 0);

    return pIObject;
}



//	����ָ���Ķ���Ĳ������ݣ���xml�н�����pRootObj��object name��id������ԭ�ж���������ԺͲ���
bool  LayoutManager::ReLoadLayout(Object* pRootObj, list<Object*>& listAllChild )
{
    LPCTSTR  szObjName = L"";
    if (pRootObj->GetDescription())
        szObjName = pRootObj->GetDescription()->GetTagName();

    UIElementProxy objUIElem = this->FindWindowElement(
            szObjName, pRootObj->GetId());

    if (!objUIElem)
    {
        UI_LOG_FATAL(_T("δ�ҵ�Ҫ���صĶ���name=%s, id=%s"),  szObjName, pRootObj->GetId());
        return false;
    }    

	pRootObj->LoadAttributeFromXml(objUIElem.get(), true);
    ReloadChildObjects(pRootObj, objUIElem.get(), listAllChild);

    return true;
}

void  LayoutManager::ReloadChildObjects(
            Object* pObjParent, 
            UIElement* pObjElement, 
            list<Object*>& listAllChild)
{
    UIApplication*  pUIApp = m_pSkinRes->GetUIApplication();
    UIElementProxy childElem = pObjElement->FirstChild();

    // ���������Ӷ���
    while (childElem)
    {
        CComBSTR  bstrId;

        Object*  pObj = NULL;
        LPCTSTR szTagName = childElem->GetTagName();
        if (!szTagName || !szTagName[0])
        {
            UI_LOG_WARN(_T("xml invalid tag name."));

            childElem = childElem->NextElement();
            continue;
        }

        childElem->GetAttrib(XML_ID, &bstrId);
        if (0 != bstrId.Length())
        {
            // ���� tagName + id ��listAllChild�в��Ҹö���
            list<Object*>::iterator  iter = listAllChild.begin();
            list<Object*>::iterator  iterEnd = listAllChild.end();

            for (; iter != iterEnd; iter++)
            {
                Object* pTempObj = *iter;
                LPCTSTR szObjName = L"";
                if (pTempObj->GetDescription())
                    szObjName = pTempObj->GetDescription()->GetTagName();

                if (0 == _tcscmp(szTagName, szObjName) &&
                    0 == _tcscmp((BSTR)bstrId, pTempObj->GetId()))
                {
                    pObj = pTempObj;
                    break;
                }
            }
        }

        if (NULL == pObj)
        {
            // û���ҵ�
            // ֱ������������
            // ����ǩ��ת��Ϊ��

            IObject* pIObject = pUIApp->CreateUIObjectByName(szTagName, m_pSkinRes->GetISkinRes());
            if (NULL == pIObject)
            {
                UI_LOG_ERROR(_T("CreateObject Failed. name=%s"), szTagName);

                childElem = childElem->NextElement();
                continue;
            }

            pObj = pIObject->GetImpl();

            // ����
            pObjParent->AddChild(pObj);

            // ��������
			pObj->LoadAttributeFromXml(childElem.get(), false);

            // Ĭ�Ͻ��ÿؼ������һ��notify objectΪ���ڶ��󣬲���msgmapid����Ϊ0
            WindowBase* pWindowBase = pObj->GetWindowObject();
            if (pWindowBase)
            {
                pObj->SetNotify(pWindowBase->GetIMessage(), 0);
            }

            // �ݹ飬��������Ӷ�����Ӷ���
            this->ReloadChildObjects(pObj, childElem.get(), listAllChild);
        }
        else
        { 
            // ����
            pObjParent->AddChild(pObj);

            // ��������
			pObj->LoadAttributeFromXml(childElem.get(), true);

            // �ݹ飬��������Ӷ�����Ӷ���
            this->ReloadChildObjects(pObj, childElem.get(), listAllChild);

            // ���б����Ƴ�
            list<Object*>::iterator iter = listAllChild.begin();
            list<Object*>::iterator iterEnd = listAllChild.end();
            for (; iter != iterEnd; iter++)
            {
                if (pObj == (*iter))
                {
                    listAllChild.erase(iter);
                    break;
                }
            }
        }

        childElem = childElem->NextElement();
    }
}


//////////////////////////////////////////////////////////////////////////

HRESULT  LayoutManager::UIParseLayoutTagCallback(IUIElement* pElem, ISkinRes* pSkinRes)
{
    ILayoutManager*  pLayoutMgr = pSkinRes->GetLayoutManager();
    if (NULL == pLayoutMgr)
        return E_FAIL;

    pLayoutMgr->GetImpl()->ParseNewElement(pElem->GetImpl());
	return true;
}
HRESULT  LayoutManager::UIParseLayoutConfigTagCallback(IUIElement* pElem, ISkinRes* pSkinRes)
{
	ILayoutManager*  pLayoutMgr = pSkinRes->GetLayoutManager();
	if (NULL == pLayoutMgr)
		return E_FAIL;

	pLayoutMgr->GetImpl()->ParseLayoutConfigTag(pElem->GetImpl());
	return S_OK;
}

void  LayoutManager::ParseNewElement(UIElement* pElem)
{
    pElem->AddRef();
    m_listUIElement.Add(pElem);

    // �ɴ����Լ�����LoadLayout�ٽ���
}

//
// ����
//
//  <layoutconfig>
//      <file path="login_dlg.xml">
//        <Window name="login.dlg"/>
//        <Window name="login.userlist"/>
//        <ListItem name="login.userlist.item"/>
//      </file>
//  </layoutconfig>
//
void  LayoutManager::ParseLayoutConfigTag(UIElement* pElem)
{
	if (!pElem)
		return;

	// ��������Ԫ��

    CComBSTR bstrTagName;

    UIElementProxy childElement = pElem->FirstChild();
    while (childElement)
    {
        bstrTagName = childElement->GetTagName();
        if (bstrTagName == XML_FILE)
        {
            ParseLayoutConfigFileTag(childElement.get());
        }
        
        childElement = childElement->NextElement();
    }
}

bool  LayoutManager::ParseLayoutConfigFileTag(UIElement* pElem)
{
    if (!pElem)
        return false;

    CComBSTR bstrPath;
    if (!pElem->GetAttrib(XML_PATH, &bstrPath))
        return false;

    LayoutConfigItem*  pFileitem = new LayoutConfigItem;
    pFileitem->SetPath(bstrPath);

    UIElementProxy childElement = pElem->FirstChild();
    while (childElement)
    {
        CComBSTR bstrWindowName;
        LPCTSTR szTagName = childElement->GetTagName();

        if (0 == _tcscmp(szTagName, XML_WINDOW))
        {
            if (childElement->GetAttrib(XML_NAME, &bstrWindowName))
            {
                pFileitem->AddWindow(bstrWindowName);
            }
        }
        else if (0 == _tcscmp(szTagName, XML_LISTITEM))
        {
            if (childElement->GetAttrib(XML_NAME, &bstrWindowName))
            {
                pFileitem->AddListItem(szTagName);
            }
        }

        childElement = childElement->NextElement();
    }

    m_listLayoutConfig.Add(pFileitem);
    return true;
}

//////////////////////////////////////////////////////////////////////////

void  LayoutConfigItem::AddWindow(LPCTSTR szName)
{
    if (szName)
    {
        m_vecWindow.push_back(String(szName));
    }
}
bool  LayoutConfigItem::FindWindow(LPCTSTR szName)
{
    if (!szName)
        return false;

    vector<String>::iterator iter = std::find(
                m_vecWindow.begin(), 
                m_vecWindow.end(), 
                String(szName));    

    if (iter == m_vecWindow.end())
        return false;

    return true;
}


void  LayoutConfigItem::AddListItem(LPCTSTR szName)
{
    if (szName)
    {
        m_vecListItem.push_back(String(szName));
    }
}
bool  LayoutConfigItem::FindListItem(LPCTSTR szName)
{
    if (!szName)
        return false;

    vector<String>::iterator iter = std::find(
        m_vecListItem.begin(), 
        m_vecListItem.end(), 
        String(szName));    

    if (iter == m_vecListItem.end())
        return false;

    return true;
}

// Ŀǰֻ������������
bool  LayoutConfigItem::Find(LPCTSTR szTagName, LPCTSTR szName)
{
    if (!szTagName || !szName)
        return false;

    if (0 == _tcscmp(szTagName, XML_LISTITEM_))
        return FindListItem(szName);
    else
        return FindWindow(szName);
}

UINT  LayoutConfigItem::GetWindowCount() 
{
    return m_vecWindow.size(); 
}
LPCTSTR  LayoutConfigItem::GetWindowName(UINT index)
{
    if (index <= m_vecWindow.size())
        return m_vecWindow[index].c_str();

    return NULL;
}

void  LayoutConfigItem::SetPath(LPCTSTR szPath)
{
    SETSTRING(m_strPath, szPath);
}
LPCTSTR  LayoutConfigItem::GetPath()
{
    return m_strPath.c_str();
}

//////////////////////////////////////////////////////////////////////////

#if 0
class LayoutWindowNodeList : public ILayoutWindowNodeList
{
public:
    static LayoutWindowNodeList* Create()
    {
        return new LayoutWindowNodeList;
    }
    virtual void  Release() override
    {
        delete this;
    }
    virtual UINT  GetCount() override
    {
        return (long)m_array.size();
    }
    virtual LPCTSTR  GetWindowName(UINT index)
    {
        if (index >= m_array.size())
            return NULL;

        return m_array[index].strName.c_str();
    }
    virtual LPCTSTR  GetWindowId(UINT index)
    {
        if (index >= m_array.size())
            return NULL;

        return m_array[index].strId.c_str();
    }
    virtual LPCTSTR  GetWindowPath(UINT index)
    {
        if (index >= m_array.size())
            return NULL;

        return m_array[index].strPath.c_str();
    }
public:
    void  AddWindow(LPCTSTR szPath, LPCTSTR szName, LPCTSTR szId)
    {
        if (szId && szPath)
        {
            WindowInfo info;
            SETSTRING(info.strName, szName);
            SETSTRING(info.strId, szId);
            SETSTRING(info.strPath, szPath);
            m_array.push_back(info);
        }
    }

private:
    struct  WindowInfo
    {
        String  strPath;
        String  strName;
        String  strId;
    };
    vector<WindowInfo> m_array;
};
#endif
#if 0
bool  LayoutManager::LoadWindowNodeList(ILayoutWindowNodeList** ppInfo)
{
    if (!ppInfo)
        return false;

    LayoutWindowNodeList* pResult = LayoutWindowNodeList::Create();
    *ppInfo = pResult;

    // �ӻ����ж�ȡ
    UIListItem<UIElement*>* pUIElementItem = m_listUIElement.GetFirst();
    while (pUIElementItem)
    {
        UIElement* pLayoutElem = **pUIElementItem;

        // ��ȡ�ý�����ڵ�xml����
        LPCTSTR szDocName = NULL;
        UIDocument* pDoc = pLayoutElem->GetDocument();
        if (pDoc)
        {
            szDocName = pDoc->GetSkinPath();
        }

        //  ������<layout>����Ĵ��ڽ��
        UIElement*  pChildElem = NULL;
        CComBSTR  bstrTagName;
        CComBSTR  bstrId;

        UIElementProxy childElem = pLayoutElem->FirstChild();
        while (childElem)
        {
            pChildElem->GetAttrib(XML_ID, &bstrId);
            pResult->AddWindow(
                        szDocName, 
                        pChildElem->GetTagName(),
                        bstrId);

            childElem = childElem->NextElement();
        }
        pUIElementItem = pUIElementItem->GetNext();
    }

    // 2. ��layoutconfig�ж�ȡ
    UIListItem<LayoutConfigItem*>* pListItem = m_listLayoutConfig.GetFirst();
    while (pListItem)
    {
        LayoutConfigItem* pConfigItem = **pListItem;

        UINT nWindowCount = pConfigItem->GetWindowCount();
        for (UINT i = 0; i < nWindowCount; i++)
        {
            pResult->AddWindow(pConfigItem->GetPath(), NULL, pConfigItem->GetWindowName(i));
        }
        pListItem = pListItem->GetNext();
    }
    return true;
}
#endif
//////////////////////////////////////////////////////////////////////////