#include "StdAfx.h"
#include "stylemanager.h"
#include "Inc\Interface\iuires.h"
#include "Src\SkinParse\xml\xmlwrap.h"
#include "Inc\Interface\imapattr.h"
#include "Inc\Interface\ixmlwrap.h"
#include "skinres.h"
#include "..\Base\Application\uiapplication.h"

#define MAX_STRING_LEN    256

StyleManager::StyleManager(SkinRes* p) : m_StyleRes(p)
{
    m_pIStyleManager = NULL;
	m_pSkinRes = p;
}

StyleManager::~StyleManager(void)
{
	this->Clear();
    SAFE_DELETE(m_pIStyleManager);

	_MyListItem* pItem = m_listUIElement.GetFirst();
	while (pItem)
	{
		(**pItem).pXmlElement->Release();
		pItem = pItem->GetNext();
	}
	m_listUIElement.Clear();
}

IStyleManager&  StyleManager::GetIStyleManager()
{
    if (!m_pIStyleManager)
        m_pIStyleManager = new IStyleManager(this);

    return *m_pIStyleManager;
}


void StyleManager::Clear()
{
	m_StyleRes.Clear();
}

int StyleManager::GetStyleCount( )
{
	return m_StyleRes.GetCount();
}


StyleRes&  StyleManager::GetStyleRes()
{
	return m_StyleRes;
}

//
// ������ֻ����LoadStyle���������ӿ�
//
// ��xml�н�һ�������������Զ�ȡ�����󣬽�����styleclass����
// bool  StyleManager::ParseStyle(IMapAttribute* pMapAttrib)
// {
//     if (!pMapAttrib)
//         return false;
// 
//     // ����pMapAttrib->GetAttr������ʱ������
//     String strId, strStyleClass, strName;  
//     {
//         LPCTSTR  szId = pMapAttrib->GetAttr(XML_ID, false);
//         if (szId)
//             strId = szId;
// 
//         LPCTSTR  szStyleClass = pMapAttrib->GetAttr(XML_STYLECLASS, false);
//         if (szStyleClass)
//             strStyleClass = szStyleClass;
// 
// 		LPCTSTR  szName = pMapAttrib->GetTag();
// 		if (szName)
// 			strName = szName;
//     }
// 
//     return this->LoadStyle(
//                 strName.c_str(), 
//                 strStyleClass.c_str(), 
//                 strId.c_str(),
//                 pMapAttrib); 
// }

// ��pListAttrib��������style������ɾ���������ڱ��浽xml��
// bool  StyleManager::ReverseParseStyle(IListAttribute* pListAttrib)
// {
//     if (!pListAttrib)
//         return false;
// 
//     String strId, strStyleClass, strName;  // ����pMapAttrib->GetAttr������ʱ������
//     {
//         LPCTSTR  szId = pListAttrib->GetAttr(XML_ID);
//         if (szId)
//             strId = szId;
// 
//         LPCTSTR  szStyleClass = pListAttrib->GetAttr(XML_STYLECLASS);
//         if (szStyleClass)
//             strStyleClass = szStyleClass;
// 
//         LPCTSTR  szName = pListAttrib->GetTag();
//         if (szName)
//             strName = szName;
//     }
// 
//     return m_StyleRes.UnloadStyle(
//         strId.c_str(), strStyleClass.c_str(), strName.c_str(), pListAttrib);
// }

//
// ����һ���̳��ַ�����������ʽ���ͣ��罫#button����Ϊ idѡ�����ͣ�id=button
//
bool  StyleManager::ParseInheritString(
        const String& strInherit,
        STYLE_SELECTOR_TYPE& eStyletype, 
        TCHAR* szStyleName)
{
    if (strInherit.length() <= 0 || NULL == szStyleName )
        return false;

    if (strInherit[0] == STYLE_ID_PREFIX )
    {
        eStyletype = STYLE_SELECTOR_TYPE_ID;
        _tcsncpy( szStyleName, strInherit.substr( 1, strInherit.length()-1 ).c_str(), MAX_STRING_LEN-1 );
    }
    else if (strInherit[0] == STYLE_CLASS_PREFIX )
    {
        eStyletype = STYLE_SELECTOR_TYPE_CLASS;
        _tcsncpy( szStyleName, strInherit.substr( 1, strInherit.length()-1 ).c_str(), MAX_STRING_LEN-1 );
    }
    else
    {
        eStyletype = STYLE_SELECTOR_TYPE_TAG;
        _tcsncpy( szStyleName, strInherit.c_str(), MAX_STRING_LEN-1 );
    }
    return true;
}
bool  StyleManager::MakeInheritString(
    const STYLE_SELECTOR_TYPE& eStyletype, 
    const String& strStypeName, 
    TCHAR* szInherit)
{
    if (strStypeName.length() <= 0 || NULL == szInherit )
        return false;

    if (eStyletype == STYLE_SELECTOR_TYPE_ID )
    {
        _tcscpy( szInherit, _T(" ") );
        szInherit[0] = STYLE_ID_PREFIX ;
        _tcscat( szInherit, strStypeName.c_str() );
    }
    else if (eStyletype == STYLE_SELECTOR_TYPE_CLASS )
    {
        _tcscpy( szInherit, _T(" ") );
        szInherit[0] = STYLE_CLASS_PREFIX ;
        _tcscat( szInherit, strStypeName.c_str() );
    }
    else
    {
        _tcscpy( szInherit, strStypeName.c_str() );
    }
    return true;
}

//
//	�ݹ����һ��CPojo_StyleItem�ļ̳�����
//
//	Parameter
//		pTreeItem
//			[in]	Ҫ������CStyleItem��pTreeItem�������ļ̳���������������Ҷ�ӽ�㡣
//					����Լ��ظ�������������У���ʾ�����������̳У��޷�������
//
//		pStyleRes
//			[in]	��ǰ���е�StyleItem�б�
//
//	Return
//		�����ɹ�����TRUE��ʧ�ܷ���FALSE��ֻҪ������һ������FALSE�������ȫ������FALSE��
//
bool  StyleManager::parse_inherit(
        tree<StyleResItem*>* pTreeItem, StyleRes* pStyleRes)
{
    if (NULL == pTreeItem || NULL == pTreeItem->data )
        return false;

    StyleResItem* pStyleItem = pTreeItem->data;

    // �ж�pTreeItem�Ƿ��м̳й�ϵ
    int nSize = pStyleItem->GetInheritCount();
    if (0 == nSize)
        return true;

    // �жϵ�ǰ���pTreeItem�����������ǲ����ظ������ˣ�����ظ����������ʾ��ѭ����
    {
        tree<StyleResItem*>* pParentItem = pTreeItem;
        do
        {
            pParentItem = pParentItem->parent;
            if (pParentItem && pParentItem->data == pStyleItem )
                return false;
        }
        while(pParentItem != NULL);
    }


    for( int i = nSize-1; i >= 0; i-- )
    {
        String strInherit;
        if (false == pStyleItem->GetInheritItem(i, strInherit) )
            continue;

        STYLE_SELECTOR_TYPE type = STYLE_SELECTOR_TYPE_TAG;
        TCHAR szStyleName[MAX_STRING_LEN] = _T("");
        ParseInheritString(strInherit, type, szStyleName);

        StyleResItem* pInheritItem = pStyleRes->GetItem(type, szStyleName);
        if (NULL == pInheritItem )
        {
            UI_LOG_WARN(_T("get parent style item failed. type=%d, name=%s"), type, szStyleName);
            continue;
        }

        tree<StyleResItem*> t;
        t.parent = pTreeItem;
        t.data   = pInheritItem;

        if (false == this->parse_inherit( &t, pStyleRes) )
        {
            return false;
        }

        // �����ɹ����̳���������
        pInheritItem->InheritMyAttributesToAnother(pStyleItem);
        pStyleItem->RemoveInheritItem(strInherit.c_str());
    }

    return true;
}

HRESULT  StyleManager::UIParseStyleTagCallback(
            IUIElement* pElem, ISkinRes* pSkinRes)
{
    IStyleManager&  pStyleMgr = pSkinRes->GetStyleManager();
    return pStyleMgr.GetImpl()->ParseNewElement(pElem->GetImpl());
}

HRESULT  StyleManager::ParseNewElement(UIElement* pElem)
{
    if (!pElem)
        return E_INVALIDARG;

	StyleTagElementInfo info;
	info.pXmlElement = pElem;
	info.pXmlElement->AddRef();

	CComBSTR bstrId;
	if (pElem->GetAttrib(XML_ID, &bstrId))
	{
		info.strId = bstrId;
	}

	m_listUIElement.Add(info);

    // ��������Ԫ��
    UIElementProxy childElement = pElem->FirstChild();
    while (childElement)
    {
        this->OnNewChild(childElement.get());
        childElement = childElement->NextElement();
    }

    // �����е�ITEM������󣬿�ʼ����inherit��ϵ
    // TODO: 2014.4.16,�ڳ�ʼ���оͽ����̳��б����������ʱ�޸ĸ���ʽ���ԣ�����ʽ�޷����°ɡ��Ժ�������ô�޸�

    int nSize = m_StyleRes.GetCount();
    for (int i = 0; i < nSize; i++)
    {
        StyleResItem* pItem = m_StyleRes.GetItem(i);
        tree<StyleResItem*>  t;
        t.data = pItem;

        if (false == this->parse_inherit(&t, &m_StyleRes) )
        {
            UI_LOG_WARN(_T("CXmlStyleParse::load_from_file parse_inherit failed. style item=%s"), pItem->GetId());
        }
    }

    return S_OK;
}

void  StyleManager::OnNewChild(UIElement* pElem)
{
    CComBSTR bstrTagName = pElem->GetTagName();

    //	������������
    IMapAttribute* pMapAttrib = NULL;
    pElem->GetAttribList(&pMapAttrib);

    StyleResItem* pStyleItem = new StyleResItem;
    pStyleItem->SetAttributeMap(pMapAttrib);
	pStyleItem->SetXmlElement(pElem);

    do 
    {
        // ����  SELECTOR_TPYE, ID
        if (bstrTagName == XML_STYLE_SELECTOR_TYPE_ID)
        {
            pStyleItem->SetSelectorType(STYLE_SELECTOR_TYPE_ID);

            String strID;
            LPCTSTR szText = pMapAttrib->GetAttr(XML_ID, true);
            if (NULL == szText)
            {
                UI_LOG_WARN( _T("Can't find the %s attribute of %s"), XML_ID, (BSTR)bstrTagName);
                delete pStyleItem;
                break;
            }
            strID = szText;
            pStyleItem->SetId(strID.c_str());
        }
        else if (bstrTagName == XML_STYLE_SELECTOR_TYPE_CLASS)
        {
            pStyleItem->SetSelectorType(STYLE_SELECTOR_TYPE_CLASS);

            String strID;
            LPCTSTR szText = pMapAttrib->GetAttr(XML_ID, true);
            if (NULL == szText)
            {
                UI_LOG_WARN( _T("Can't find the %s attribute of %s"), XML_ID, (BSTR)bstrTagName);
                delete pStyleItem;
                break;
            }
            strID = szText;
            pStyleItem->SetId(strID.c_str());
        }
        else
        {
            pStyleItem->SetSelectorType(STYLE_SELECTOR_TYPE_TAG);
            pStyleItem->SetId(bstrTagName);
        }

        // ���� inherit ����
        LPCTSTR szText = pMapAttrib->GetAttr(XML_STYLE_INHERIT, true);  // ����һ�����ԣ�������Ҫ����չ�ģ�ɾ����
        if (szText)
            pStyleItem->SetInherits(szText);

        if (false == m_StyleRes.Add(pStyleItem) )
        {
            delete pStyleItem;
            pStyleItem = NULL;
        }
    }
    while (0);

    SAFE_RELEASE(pMapAttrib);
}

UIElement*  StyleManager::GetStyleXmlElem(LPCTSTR szId)
{
    if (0 == m_listUIElement.GetCount())
        return NULL;

    if (!szId)
    {
        return m_listUIElement.GetFirst()->m_data.pXmlElement;
    }

    _MyListItem* pItem = m_listUIElement.GetFirst();
    while (pItem)
    {
        if ((**pItem).strId == szId)
        {
            return (**pItem).pXmlElement;
        }
        pItem = pItem->GetNext();
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
void  StyleManager::OnStyleAdd(StyleResItem* p)
{
	IUIEditor* pUIEditor = m_pSkinRes->GetUIApplication()->GetUIEditorPtr();
	if (pUIEditor)
	{
		pUIEditor->OnStyleChanged(
			m_pSkinRes->GetISkinRes(), p->GetIStyleResItem(), Editor_Add);
	}
}
void  StyleManager::OnStyleRemove(StyleResItem* p)
{
	IUIEditor* pUIEditor = m_pSkinRes->GetUIApplication()->GetUIEditorPtr();
	if (pUIEditor)
	{
		pUIEditor->OnStyleChanged(
			m_pSkinRes->GetISkinRes(), p->GetIStyleResItem(), Editor_Remove);
	}
}
void  StyleManager::OnStlyeModify(StyleResItem* p)
{
	IUIEditor* pUIEditor = m_pSkinRes->GetUIApplication()->GetUIEditorPtr();
	if (pUIEditor)
	{
		pUIEditor->OnStyleChanged(
			m_pSkinRes->GetISkinRes(), p->GetIStyleResItem(), Editor_Modify);
	}
}
void  StyleManager::OnStyleAttributeAdd(StyleResItem* p, LPCTSTR szKey)
{
	IUIEditor* pUIEditor = m_pSkinRes->GetUIApplication()->GetUIEditorPtr();
	if (pUIEditor)
	{
		pUIEditor->OnStyleAttributeChanged(
			m_pSkinRes->GetISkinRes(), p->GetIStyleResItem(), szKey, Editor_Add);
	}
}
void  StyleManager::OnStyleAttributeRemove(StyleResItem* p, LPCTSTR szKey)
{
	IUIEditor* pUIEditor = m_pSkinRes->GetUIApplication()->GetUIEditorPtr();
	if (pUIEditor)
	{
		pUIEditor->OnStyleAttributeChanged(
			m_pSkinRes->GetISkinRes(), p->GetIStyleResItem(), szKey, Editor_Remove);
	}
}
void  StyleManager::OnStyleAttributeModify(StyleResItem* p, LPCTSTR szKey)
{
	IUIEditor* pUIEditor = m_pSkinRes->GetUIApplication()->GetUIEditorPtr();
	if (pUIEditor)
	{
		pUIEditor->OnStyleAttributeChanged(
			m_pSkinRes->GetISkinRes(), p->GetIStyleResItem(), szKey, Editor_Modify);
	}
}