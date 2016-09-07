#include "StdAfx.h"
#include "skinres.h"
#include "Src\SkinParse\skinparseengine.h"
#include "Src\SkinParse\xml\xmlwrap.h"
#include "Src\Resource\skinmanager.h"
#include "Src\SkinParse\datasource\skindatasource.h"
#include "Src\Util\Stopwatch\stopwatch.h"
#include "Inc\Interface\iuires.h"
#include "uiresource.h"

SkinRes::SkinRes(SkinManager& o):
    m_mgrSkinRef(o),
    m_mgrLayout(this),
    m_mgrStyle(this),
    m_mgrFont(this),
    m_mgrColor(this),
    m_mgrImage(this),
	m_mgrI18n(this)
    
{	
    m_pISkinRes = NULL;
    m_eLoadState = SKIN_RES_LOAD_STATE_NONE;
    m_pDataSource = NULL;
    m_pHLSInfo = NULL;
	m_pParentSkinRes = NULL;
}
SkinRes::~SkinRes()
{
    Unload();
    SAFE_DELETE(m_pISkinRes);
    SAFE_DELETE(m_pHLSInfo);
    SAFE_RELEASE(m_pDataSource);
}

ISkinRes*  SkinRes::GetISkinRes()
{
    if (NULL == m_pISkinRes)
    {
        m_pISkinRes = new ISkinRes(this);
    }
    return m_pISkinRes; 
}

void  SkinRes::SetParam(SKIN_PACKET_TYPE eType, LPCTSTR szName, LPCTSTR szPath)
{
	m_strSkinResName.clear();
    SAFE_DELETE(m_pDataSource);

    CreateDataSourceInstance(eType, &m_pDataSource);
    m_pDataSource->SetPath(szPath);
	if (szName)
		m_strSkinResName = szName;
}

LPCTSTR  SkinRes::GetName()
{ 
    return m_strSkinResName.c_str();
}

LPCTSTR  SkinRes::GetPath() 
{ 
    if (NULL == m_pDataSource)
        return NULL;

    return m_pDataSource->GetPath(); 
}

bool  SkinRes::Load()
{
    if (m_eLoadState == SKIN_RES_LOAD_STATE_LOADED)
        return true;

    // �����ٶȲ���
	StopWatch watch;
	watch.Start();

	SkinParseEngine parse(this);
    if (parse.Parse(m_pDataSource, XML_SKIN_XML))
    {
		m_eLoadState = SKIN_RES_LOAD_STATE_LOADED;
		
		__int64 lCost = watch.Now();
		TCHAR szText[128] = {0};
		_stprintf(szText, _T("Load Skin File Cost: %d ms\r\n"), lCost);
		::OutputDebugString(szText);

		UI_LOG_DEBUG(_T("Load Skin File Cost: %d ms"), lCost);
		return true;
    }   
	else
	{
		Unload();
		return false;
	}
}

bool  SkinRes::Unload()
{
    _DocList::iterator iter = m_listDoc.begin();
    for (; iter != m_listDoc.end(); iter++)
    {
        UIDocument* po = *iter;
        SAFE_RELEASE(po);
    }
    m_listDoc.clear();

    m_eLoadState = SKIN_RES_LOAD_STATE_NONE;
    return true;
}

void  SkinRes::SetParentSkinRes(SkinRes* p)
{
	m_pParentSkinRes = p;
}

SkinRes*  SkinRes::GetParentSkinRes()
{
	return m_pParentSkinRes;
}

// parse.ProcessFile�ڽ��������ļ������У��������һ���µ��ĵ���
// ���ص��ú�������֪ͨ
void  SkinRes::OnNewUIDocument(UIDocument* pDoc)
{
	if (!pDoc)
		return;

	pDoc->AddRef();
    m_listDoc.push_back(pDoc);
}

bool SkinRes::Save()
{
    _DocList::iterator iter = m_listDoc.begin();
    for (; iter != m_listDoc.end(); iter++)
    {
        UIDocument* po = *iter;

        if (!po->IsDirty())
            continue;

        if (!po->Save())
        {
            UI_LOG_ERROR(_T("%s Save Xml Doc Failed. path=%s"), 
					po->GetSkinPath());
        }
    }

	return true;
}


//
//	����ָ���Ķ���Ĳ������ݣ���xml�н�����pRootObj��object name��id������ԭ�ж���������ԺͲ���
//
bool SkinRes::ReLoadLayout(Object* pRootObj, list<Object*>& listAllChild )
{
	return m_mgrLayout.ReLoadLayout(pRootObj, listAllChild);
}

IImageResItem* SkinRes::GetImageItemInfo(UINT nIndex)
{
	return m_mgrImage.GetImageItemInfo(nIndex);
}

IColorResItem* SkinRes::GetColorItemInfo(UINT nIndex)
{
	return m_mgrColor.GetColorItemInfo(nIndex);
}

IFontResItem* SkinRes::GetFontItemInfo(UINT nIndex)
{
	return m_mgrFont.GetFontItemInfo(nIndex);
}

bool SkinRes::ChangeSkinHLS(short h, short l, short s, int nFlag)
{
    if (NULL == m_pHLSInfo)
    {
        m_pHLSInfo = new SKIN_HLS_INFO;
    }
    m_pHLSInfo->h = h;
    m_pHLSInfo->l = l;
    m_pHLSInfo->s = s;
    m_pHLSInfo->nFlag = nFlag;

	if (false == m_mgrImage.ChangeSkinHLS(h,l,s,nFlag))
		return false;
	if (false == m_mgrColor.ChangeSkinHLS(h,l,s,nFlag))
		return false;

#if 0 // -- �ܹ�����	
	// ����� HLS ����
	SkinConfigRes* pSkinConfigRes = m_pSkinConfigMgr->GetSkinConfigRes();
	if (NULL == pSkinConfigRes)
		return false;

	pSkinConfigRes->SetHLS((char)h,(char)l,(char)s,nFlag);
#endif
	return true;
}


UIDocument*  SkinRes::GetXmlDoc(UINT nIndex)
{
    if (nIndex >= GetXmlDocCount())
        return NULL;

    return m_listDoc[nIndex];
}

UIDocument* SkinRes::GetXmlDocByName(LPCTSTR szName)
{
	if (!szName)
		return false;

	_DocList::iterator iter = m_listDoc.begin();
	for (; iter != m_listDoc.end(); iter++)
	{
		UIDocument* po = *iter;
		if (0 == _tcscmp(szName, po->GetSkinPath()))
		{
			return po;
		}
	}
	return NULL;
}

UIApplication*  SkinRes::GetUIApplication()  
{ 
	return m_mgrSkinRef.GetUIApplication();
}
ISkinManager*  SkinRes::GetISkinManager()   
{ 
	return m_mgrSkinRef.GetISkinManager(); 
}

ImageRes&  SkinRes::GetImageRes()
{
	return m_mgrImage.GetImageRes();
}
ColorRes&  SkinRes::GetColorRes()
{
	return m_mgrColor.GetColorRes();
}
FontRes&  SkinRes::GetFontRes()
{
	return m_mgrFont.GetFontRes();
}

I18nRes&  SkinRes::GetI18nRes()
{
	return m_mgrI18n.GetI18nRes();
}