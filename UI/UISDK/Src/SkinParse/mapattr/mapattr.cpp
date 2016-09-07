#include "stdafx.h"
#include "mapattr.h"
#include "Src\Renderbase\renderbase\renderbase.h"
#include "Src\Renderbase\textrenderbase\textrender.h"
#include "Inc\Interface\iuires.h"
#include "Inc\Interface\iuiapplication.h"

namespace UI
{

CMapAttribute::CMapAttribute()
{
    m_lRef = 0;
    m_iterEnum = m_mapAttr.end();
}
CMapAttribute::~CMapAttribute()
{

}
CMapAttribute::CMapAttribute(const CMapAttribute& o)
{
    m_mapAttr = o.m_mapAttr;
    m_strTemp.clear();
}

void  CMapAttribute::SetTag(LPCTSTR szKey)
{
	if (szKey)
		m_strTag = szKey;
	else
		m_strTag.clear();
}
LPCTSTR  CMapAttribute::GetTag()
{
	return m_strTag.c_str();
}
bool  CMapAttribute::HasAttrib(LPCTSTR szKey)
{
    if (NULL == szKey)
        return false;

    ATTRMAP::iterator iter = m_mapAttr.find(szKey);
    if (iter == m_mapAttr.end())
        return false;

    return true;
}
LPCTSTR CMapAttribute::GetAttr(LPCTSTR szKey, bool bErase)
{
    if (NULL == szKey)
        return NULL;

    ATTRMAP::iterator iter = m_mapAttr.find(szKey);
    if (iter == m_mapAttr.end())
        return NULL;

    if (bErase)
    {
        m_strTemp = iter->second;
        m_mapAttr.erase(iter);
        return m_strTemp.c_str();
    }
    return iter->second.c_str();
}

LPCTSTR CMapAttribute::GetAttr(const String& strKey, bool bErase)
{
    ATTRMAP::iterator iter = m_mapAttr.find(strKey);
    if (iter == m_mapAttr.end())
        return NULL;

    if (bErase)
    {
        m_strTemp = iter->second;
        m_mapAttr.erase(iter);
        return m_strTemp.c_str();
    }
    return iter->second.c_str();
}
LPCTSTR CMapAttribute::GetAttr(LPCTSTR szPrefix, LPCTSTR szKey, bool bErase)
{
    String strKey;
    if (szPrefix)
        strKey = szPrefix;
    strKey.append(szKey);

    return GetAttr(strKey.c_str(), bErase);
}

LRESULT CMapAttribute::GetAttr_bool(LPCTSTR szPrefix, LPCTSTR szKey, bool bErase, bool* pbGet)
{
    String strKey;
    if (szPrefix)
        strKey = szPrefix;
    strKey.append(szKey);

    return GetAttr_bool(strKey.c_str(), bErase, pbGet);
}
LRESULT CMapAttribute::GetAttr_bool(LPCTSTR szKey, bool bErase, bool* pbGet)
{
    if (NULL == szKey || NULL == pbGet)
        return MAPATTR_RET_ERROR;

    ATTRMAP::iterator iter = m_mapAttr.find(szKey);
    if (iter == m_mapAttr.end())
        return MAPATTR_RET_NOT_EXIST;
    
    LRESULT lRet = MAPATTR_RET_OK;

    LPCTSTR szValue = iter->second.c_str();
    if (0 == _tcscmp(szValue, XML_BOOL_VALUE_TRUE)  || 
        0 == _tcscmp(szValue, XML_BOOL_VALUE_1)     ||
        0 == _tcscmp(szValue, XML_BOOL_VALUE_YES))
    {
        *pbGet = true; 
    }
    else if (0 == _tcscmp(szValue, XML_BOOL_VALUE_FALSE) ||
        0 == _tcscmp(szValue, XML_BOOL_VALUE_0)      ||
        0 == _tcscmp(szValue, XML_BOOL_VALUE_NO))
    {
        *pbGet = false;
    }
    else 
    {
        lRet = MAPATTR_RET_INVLID_VALUE;
    }

    if (bErase)
        m_mapAttr.erase(iter);

    return lRet;
}

long CMapAttribute::GetAttr_int(LPCTSTR szPrefix, LPCTSTR szKey, bool bErase, int* pnGet)
{
    String strKey;
    if (szPrefix)
        strKey = szPrefix;
    strKey.append(szKey);

    return GetAttr_int(strKey.c_str(), bErase, pnGet);
}
long CMapAttribute::GetAttr_int(LPCTSTR szKey, bool bErase, int* pnGet)
{
    if (NULL == szKey || NULL == pnGet)
        return MAPATTR_RET_ERROR;

    ATTRMAP::iterator iter = m_mapAttr.find(szKey);
    if (iter == m_mapAttr.end())
        return MAPATTR_RET_NOT_EXIST;

    *pnGet = _ttoi(iter->second.c_str());

    if (bErase)
        m_mapAttr.erase(iter);

    return MAPATTR_RET_OK;
}



long  CMapAttribute::GetAttr_intarray(LPCTSTR szPrefix, LPCTSTR szKey, bool bErase, int* pIntArray, unsigned int nSize)
{
    String strKey;
    if (szPrefix)
        strKey = szPrefix;
    strKey.append(szKey);

    return GetAttr_intarray(strKey.c_str(), bErase, pIntArray, nSize);
}
long  CMapAttribute::GetAttr_intarray(LPCTSTR szKey, bool bErase, int* pIntArray, unsigned int nSize)
{
    if (NULL == szKey || NULL == pIntArray)
        return MAPATTR_RET_ERROR;

    ATTRMAP::iterator iter = m_mapAttr.find(szKey);
    if (iter == m_mapAttr.end())
        return MAPATTR_RET_NOT_EXIST;

    Util::ISplitStringEnum* pEnum = NULL;
    unsigned int nCount = Util::SplitString(iter->second.c_str(), XML_SEPARATOR, &pEnum);
    if (nCount != nSize)
    {
        SAFE_RELEASE(pEnum);
        return MAPATTR_RET_INVLID_VALUE;
    }
    for (unsigned int i = 0; i < nCount; i++)
    {
        pIntArray[i] = _ttoi(pEnum->GetText(i));
    }

    SAFE_RELEASE(pEnum);
    if (bErase)
        m_mapAttr.erase(iter);

    return MAPATTR_RET_OK;
}

long CMapAttribute::GetAttr_REGION4(LPCTSTR szPrefix, LPCTSTR szKey, bool bErase, REGION4* prcGet)
{
    if (NULL == szKey || NULL == prcGet)
        return MAPATTR_RET_ERROR;

    String strKey;
    if (szPrefix)
        strKey = szPrefix;
    strKey.append(szKey);

    ATTRMAP::iterator iter = m_mapAttr.find(strKey.c_str());
    if (iter == m_mapAttr.end())
        return MAPATTR_RET_NOT_EXIST;

    LRESULT lRet = MAPATTR_RET_OK;
    if (false == Util::TranslateRECT(iter->second.c_str(), prcGet))
        lRet = MAPATTR_RET_INVLID_VALUE;

    if (bErase)
        m_mapAttr.erase(iter);

    return lRet;
}

long  CMapAttribute::GetAttr_Image9Region(LPCTSTR szPrefix, LPCTSTR szKey, bool bErase, C9Region* pRegion)
{
    if (NULL == szKey || NULL == pRegion)
        return MAPATTR_RET_ERROR;

    String strKey;
    if (szPrefix)
        strKey = szPrefix;
    strKey.append(szKey);

    ATTRMAP::iterator iter = m_mapAttr.find(strKey.c_str());
    if (iter == m_mapAttr.end())
        return MAPATTR_RET_NOT_EXIST;

    LRESULT lRet = MAPATTR_RET_OK;
    if (false == Util::TranslateImage9Region(iter->second.c_str(), pRegion))
        lRet = MAPATTR_RET_INVLID_VALUE;

    if (bErase)
        m_mapAttr.erase(iter);

    return lRet;
}

long CMapAttribute::GetAttr_RenderBase(
    LPCTSTR szPrefix,
    LPCTSTR szKey, 
    bool bErase,
    IUIApplication* pUIApp, 
    IObject* pBindObj, 
    IRenderBase** ppGet)
{
    if (NULL == szKey || NULL == ppGet || NULL == pUIApp)
        return MAPATTR_RET_ERROR;

    LRESULT lRet = MAPATTR_RET_OK;
    String strKey;
    if (szPrefix)
        strKey = szPrefix;
    strKey.append(szKey);

    ATTRMAP::iterator iter = m_mapAttr.find(strKey);
    if (iter == m_mapAttr.end())
        return MAPATTR_RET_NOT_EXIST;

    SAFE_RELEASE(*ppGet);

    pUIApp->CreateRenderBaseByName(iter->second.c_str(), pBindObj, ppGet);
    if (*ppGet)
    {
        SERIALIZEDATA data = {0};
        data.pUIApplication = pUIApp;
		data.pSkinRes = pBindObj?pBindObj->GetSkinRes():NULL;
        data.pMapAttrib = static_cast<IMapAttribute*>(this);
        data.szPrefix = szPrefix;
        data.nFlags = SERIALIZEFLAG_LOAD;
        if (bErase)
            data.nFlags |= SERIALIZEFLAG_LOAD_ERASEATTR;
        (*ppGet)->Serialize(&data);
    }
    else
    {
        lRet = MAPATTR_RET_INVLID_VALUE;
    }

    if (bErase)
        m_mapAttr.erase(iter);
    return lRet;
}

long CMapAttribute::GetAttr_TextRenderBase(
    LPCTSTR szPrefix, 
    LPCTSTR szKey, 
    bool bErase, 
    IUIApplication* pUIApp,
    IObject* pBindObj, 
    ITextRenderBase** ppGet)
{
    if (NULL == szKey || NULL == ppGet)
        return MAPATTR_RET_ERROR;

    LRESULT lRet = MAPATTR_RET_OK;
    String strKey;
    if (szPrefix)
        strKey = szPrefix;
    strKey.append(szKey);

    ATTRMAP::iterator iter = m_mapAttr.find(strKey.c_str());
    if (iter == m_mapAttr.end())
        return MAPATTR_RET_NOT_EXIST;

    SAFE_RELEASE(*ppGet);

    pUIApp->CreateTextRenderBaseByName(
            (BSTR)iter->second.c_str(), pBindObj, ppGet);
    if (*ppGet)
    {
        SERIALIZEDATA data = {0};
        data.pUIApplication = pUIApp;
		data.pSkinRes = pBindObj ? pBindObj->GetSkinRes() : NULL;
        data.pMapAttrib = static_cast<IMapAttribute*>(this);
        data.szPrefix = szPrefix;
        data.nFlags = SERIALIZEFLAG_LOAD;
        if (bErase)
            data.nFlags |= SERIALIZEFLAG_LOAD_ERASEATTR;
        (*ppGet)->Serialize(&data);
    }
    else
    {
        lRet = MAPATTR_RET_INVLID_VALUE;
    }

    if (bErase)
        m_mapAttr.erase(iter);
    return lRet;
}

long  CMapAttribute::GetAttr_Color(
        LPCTSTR szPrefix,
        LPCTSTR szKey, 
        bool bErase, 
        IUIApplication* pUIApp,
        Color** ppColor)
{
    if (NULL == szKey || NULL == ppColor)
        return MAPATTR_RET_ERROR;

    LRESULT lRet = MAPATTR_RET_OK;
    String strKey;
    if (szPrefix)
        strKey = szPrefix;
    strKey.append(szKey);

    ATTRMAP::iterator iter = m_mapAttr.find(strKey.c_str());
    if (iter == m_mapAttr.end())
        return MAPATTR_RET_NOT_EXIST;

    SAFE_RELEASE(*ppColor);

    IColorRes* pColorRes = pUIApp->GetActiveSkinColorRes();
    pColorRes->GetColor(iter->second.c_str(), ppColor);

    if (bErase)
        m_mapAttr.erase(iter);

    if (!(*ppColor))
        lRet = MAPATTR_RET_ERROR;

    return lRet;
}

bool CMapAttribute::AddAttr(LPCTSTR szKey, LPCTSTR szValue) 
{
    if (NULL == szKey)
        return false;

	if (szValue)
	{
		m_mapAttr[szKey] = szValue;
	}
	else
	{
		ATTRMAP::iterator iter = m_mapAttr.find(szKey);
		if (iter != m_mapAttr.end())
			iter->second.clear();
	}
    return true;
}
void CMapAttribute::AddAttr(const String& strKey, const String& strValue)
{
    m_mapAttr[strKey] = strValue;
}

bool  CMapAttribute::AddAttr_bool(LPCTSTR szKey, bool bValue)
{
	if (NULL == szKey)
		return false;

	m_mapAttr[szKey] = bValue ? _T("1"):_T("0");
	return true;
}

bool  CMapAttribute::AddAttr_REGION4(LPCTSTR szKey, REGION4* pr)
{
	if (NULL == szKey)
		return false;

	TCHAR szText[32] = {0};
	_stprintf(szText, TEXT("%d,%d,%d,%d"), 
		pr->left, pr->top, pr->right, pr->bottom);

	m_mapAttr[szKey] = szText;
	return true;
}

bool  CMapAttribute::AddAttr_int(LPCTSTR szKey, int nValue)
{
	if (NULL == szKey)
		return false;

	TCHAR szText[8] = {0};
	_stprintf(szText, TEXT("%d"), nValue);

	m_mapAttr[szKey] = szText;
	return true;
}

// ʹ��ǰ׺Prefix���ӵ�ǰ�����г�ȡ����Ӧ�����Լ�����ppMapAttribute�з��ظ��ⲿ
// �ⲿ������֮����Ҫʹ��ppMapAttribute->Release�ͷ��ڴ�
bool  CMapAttribute::ExtractMapAttrByPrefix(LPCTSTR szPrefix, bool bErase, /*out*/IMapAttribute** ppMapAttribute)
{
    if (NULL == ppMapAttribute)
        return FALSE;

    if (NULL == szPrefix || 0 == _tcslen(szPrefix))
    {
//         *ppMapAttribute = static_cast<IMapAttribute*>(this);
//         this->AddRef();
//         return true;
        return false;
    }

    ATTRMAP::iterator  iter = m_mapAttr.begin();
    ATTRMAP::iterator  iterEnd = m_mapAttr.end();

    int nPrifixLength = _tcslen(szPrefix);

    IMapAttribute*  pSubMapAttrib = NULL;
    UICreateIMapAttribute(&pSubMapAttrib);

    for ( ; iter != iterEnd; )
    {
        TCHAR*  szKey = const_cast<TCHAR*>(iter->first.c_str());
        if (_tcsstr(szKey, szPrefix) == szKey)
        {
            pSubMapAttrib->AddAttr(szKey + nPrifixLength, iter->second.c_str());

            if (bErase)
                m_mapAttr.erase(iter++);
            else
                ++iter;
        }
        else
        {
            ++iter;
        }
    }

    *ppMapAttribute = pSubMapAttrib;
    return true;
}

long  CMapAttribute::Release()
{
    UIASSERT(m_lRef > 0);
    --m_lRef;
    if (0 == m_lRef)
    {
        delete this;
        return 0;
    }
    return m_lRef;
}
long  CMapAttribute::AddRef()
{
    return ++ m_lRef;
}

void  CMapAttribute::CreateCopy(IMapAttribute** ppNewCopy)
{
    if (NULL == ppNewCopy)
        return;

    CMapAttribute* p = new CMapAttribute;
    p->AddRef();
    *ppNewCopy = static_cast<IMapAttribute*>(p);
    p->m_mapAttr = m_mapAttr;
	p->m_strTag = m_strTag;
}

// ���Լ������Կ�����pDestMapAttrib�����pDestMapAttrib���Ѿ����ڣ���ťbOverride�����ж��Ƿ񸲸�
void  CMapAttribute::CopyTo(IMapAttribute* pDestMapAttrib, bool bOverride)
{
    if (NULL == pDestMapAttrib)
        return;

    ATTRMAP::iterator  iter = m_mapAttr.begin();
    if (bOverride)
    {
        for (; iter != m_mapAttr.end(); iter++)
        {
            pDestMapAttrib->AddAttr(iter->first.c_str(), iter->second.c_str());
        }
    }
    else
    {
        for (; iter != m_mapAttr.end(); iter++)
        {
            if (pDestMapAttrib->HasAttrib(iter->first.c_str()))
                continue;

            pDestMapAttrib->AddAttr(iter->first.c_str(), iter->second.c_str());
        }
    }
}


void  CMapAttribute::BeginEnum()
{
    m_iterEnum = m_mapAttr.begin();
}
bool  CMapAttribute::EnumNext(const TCHAR** szKey, const TCHAR** szValue)
{
    if (NULL == szKey || NULL == szValue)
        return false;

    if (m_iterEnum == m_mapAttr.end())
        return false;

    *szKey = m_iterEnum->first.c_str();
    *szValue = m_iterEnum->second.c_str();

    m_iterEnum++;
    return true;
}
void  CMapAttribute::EndEnum()
{
    m_iterEnum = m_mapAttr.end();
}

HRESULT  UICreateIMapAttribute(IMapAttribute** ppOut)
{
    if (NULL == ppOut)
        return E_INVALIDARG;

    CMapAttribute* p = new CMapAttribute;
    *ppOut = static_cast<IMapAttribute*>(p);
    p->AddRef();

    return S_OK;
}

HRESULT  UICreateIListAttribute(IListAttribute** ppOut)
{
    if (NULL == ppOut)
        return E_INVALIDARG;

    CListAttribute* p = new CListAttribute;
    *ppOut = static_cast<IListAttribute*>(p);
    p->AddRef();

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////

CListAttribute::CListAttribute()
{
    m_lRef = 0;
    m_pFirstItem = NULL;
    m_pLastItem = NULL;
    m_pEnum = NULL;
}
CListAttribute::~CListAttribute()
{
    ListAttrItem* pItem = m_pFirstItem;
    while (pItem)
    {
        ListAttrItem*  pNext = pItem->pNext;
        SAFE_DELETE(pItem);
        pItem = pNext;
    }
    m_pFirstItem = NULL;
    m_pLastItem = NULL;
}


bool  CListAttribute::AddAttr(LPCTSTR szKey, LPCTSTR szValue)
{
    ListAttrItem* pItem = new ListAttrItem;
    if (szKey)
        pItem->strKey = szKey;
    if (szValue)
        pItem->strValue = szValue;

    if (NULL == m_pLastItem)
    {
        m_pFirstItem = m_pLastItem = pItem;
    }
    else
    {
        m_pLastItem->pNext = pItem;
        pItem->pPrev = m_pLastItem;

        m_pLastItem = pItem;
    }

    return true;
}

LPCTSTR  CListAttribute::GetAttr(LPCTSTR szKey)
{
    ListAttrItem* pItem = FindItem(szKey);
    if (pItem)
        return pItem->strValue.c_str();

    return NULL;
}

ListAttrItem*  CListAttribute::FindItem(LPCTSTR szKey)
{
    if (!szKey)
        return NULL;

    ListAttrItem* pItem = m_pFirstItem;
    while (pItem)
    {
        if (pItem->strKey == szKey)
        {
            return pItem;
        }
        pItem = pItem->pNext;
    }

    return NULL;
}


bool  CListAttribute::EraseAttr(LPCTSTR szKey)
{
    ListAttrItem* pItem = FindItem(szKey);
    if (!pItem)
        return false;

    if (pItem == m_pEnum)
    {
        m_pEnum = m_pEnum->pNext;
    }
    if (m_pFirstItem == pItem)
    {
        m_pFirstItem = pItem->pNext;
    }
    if (m_pLastItem == pItem)
    {
        m_pLastItem = pItem->pPrev;
    }

    if (pItem->pNext)
        pItem->pNext->pPrev = pItem->pPrev;
    if (pItem->pPrev)
        pItem->pPrev->pNext = pItem->pNext;

	SAFE_DELETE(pItem);
    return true;
}

void  CListAttribute::SetTag(LPCTSTR szKey)
{
	if (szKey)
		m_strTag = szKey;
	else
		m_strTag.clear();
}

LPCTSTR  CListAttribute::GetTag()
{
    return m_strTag.c_str();
}

void  CListAttribute::BeginEnum()
{
    m_pEnum = m_pFirstItem;
}
bool  CListAttribute::EnumNext(const TCHAR** szKey, const TCHAR** szValue)
{
    if (NULL == m_pEnum)
        return false;

    if (szKey)
        *szKey = m_pEnum->strKey.c_str();
    if (szValue)
        *szValue = m_pEnum->strValue.c_str();

    m_pEnum = m_pEnum->pNext;
    return true;
}
void  CListAttribute::EndEnum()
{
    m_pEnum = NULL;
}

long  CListAttribute::Release()
{
    UIASSERT(m_lRef > 0);
    --m_lRef;
    if (0 == m_lRef)
    {
        delete this;
        return 0;
    }
    return m_lRef;
}
long  CListAttribute::AddRef()
{
    return ++ m_lRef;
}

}


