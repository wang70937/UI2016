#include "stdafx.h"
#include "textrender_factory.h"
#include "Src\Renderbase\textrenderbase\textrender.h"
#include "Src\Base\Application\uiapplication.h"

TextRenderFactory::TextRenderFactory(UIApplication& app):m_app(app)
{

}
TextRenderFactory::~TextRenderFactory()
{
	Clear();
}

void  TextRenderFactory::Init()
{
	
#define REGISTER_UI_TEXTRENDERBASE2(classname) \
        this->RegisterUITextRenderBaseCreateData( \
                classname::GetXmlName(),          \
                classname::GetType(),             \
				(UI::pfnUICreateTextRenderBasePtr)ObjectCreator<I##classname>::CreateInstance2);

	REGISTER_UI_TEXTRENDERBASE2(SimpleTextRender)
	REGISTER_UI_TEXTRENDERBASE2(ColorListTextRender)
	REGISTER_UI_TEXTRENDERBASE2(FontColorListTextRender)
	REGISTER_UI_TEXTRENDERBASE2(ContrastColorTextRender)
	REGISTER_UI_TEXTRENDERBASE2(ContrastColorListTextRender)
}
void  TextRenderFactory::Clear()
{
	UITEXTRENDERBASE_CREATE_DATA::iterator iter = m_vecUITextRenderBaseCreateData.begin();
	for ( ; iter != m_vecUITextRenderBaseCreateData.end(); iter++ )   
	{
		SAFE_DELETE(*iter);               
	}
	m_vecUITextRenderBaseCreateData.clear();     
}


bool  TextRenderFactory::RegisterUITextRenderBaseCreateData(
        LPCTSTR szName, 
        int nType, 
        pfnUICreateTextRenderBasePtr pfunc)
{
	if (NULL == szName || NULL == pfunc)
		return false;

	String strName(szName);

	UITEXTRENDERBASE_CREATE_INFO* pInfo = new UITEXTRENDERBASE_CREATE_INFO;
	pInfo->m_func = pfunc;
	pInfo->m_nRenderType = nType;
	pInfo->m_strName = strName;
	m_vecUITextRenderBaseCreateData.push_back(pInfo);

	UI_LOG_DEBUG(_T("%s   @ 0x%08X"), szName,  pfunc);
	return true;
}

bool  TextRenderFactory::CreateTextRenderBaseByName(
        ISkinRes* pSkinRes, 
        LPCTSTR bstrName, 
        IObject* pObject, 
        ITextRenderBase** ppOut)
{
	if (!bstrName || !bstrName[0] || !pObject || !ppOut)
		return false;

	UITEXTRENDERBASE_CREATE_DATA::iterator iter = m_vecUITextRenderBaseCreateData.begin();
	for ( ; iter != m_vecUITextRenderBaseCreateData.end(); iter++ )
	{
		UITEXTRENDERBASE_CREATE_INFO* pData = *iter;
		if (NULL == pData)
			continue;

		if (pData->m_strName != bstrName)
			continue;

		// ����
		//      if (-1 != pData->m_nControlType)
		//      {
		//          if (pObject->GetObjectExtentType() != pData->m_nControlType)
		//              continue;
		//      }
		//      if (-1 != pData->m_nControlSubType)
		//      {
		//          int  nStylyEx = pObject->GetStyleEx();
		//          if (GETCONTROLSUBTYPE(nStylyEx) != pData->m_nControlSubType)
		//              continue;
		//      }

		HRESULT hr = pData->m_func(pSkinRes, (void**)ppOut);
		if (SUCCEEDED(hr) && NULL != *ppOut)
		{
            // (*ppOut)->AddRef();
			(*ppOut)->SetObject(m_app.GetIUIApplication(), pObject);
			(*ppOut)->Init();
			(*ppOut)->SetType((TEXTRENDER_TYPE)pData->m_nRenderType);
			return true;
		}

		return false;
	}

	UI_LOG_WARN(_T("Create Failed. Name=%s"), bstrName);
	return false;
}
bool  TextRenderFactory::CreateTextRender(
        ISkinRes* pSkinRes, 
        int nType, 
        IObject* pObject, 
        ITextRenderBase** ppOut)
{
	if (/*NULL == pObject ||*/ NULL == ppOut)
		return false;

	UITEXTRENDERBASE_CREATE_DATA::iterator iter = m_vecUITextRenderBaseCreateData.begin();
	for ( ; iter != m_vecUITextRenderBaseCreateData.end(); iter++ )
	{
		UITEXTRENDERBASE_CREATE_INFO* pData = *iter;
		if (NULL == pData)
			continue;

		if (pData->m_nRenderType != nType)
			continue;

		HRESULT hr = pData->m_func(pSkinRes, (void**)ppOut);
		if (SUCCEEDED(hr) && NULL != *ppOut)
		{
            //(*ppOut)->AddRef();
			(*ppOut)->SetObject(m_app.GetIUIApplication(), pObject);
			(*ppOut)->Init();
			(*ppOut)->SetType((TEXTRENDER_TYPE)nType);
			return true;
		}

		return false;
	}

	UI_LOG_WARN(_T("Create Failed. Type=%d"), nType);
	return false;
}

LPCTSTR  TextRenderFactory::GetTextRenderBaseName(int nType)
{
	UITEXTRENDERBASE_CREATE_DATA::iterator iter = m_vecUITextRenderBaseCreateData.begin();
	for ( ; iter != m_vecUITextRenderBaseCreateData.end(); iter++ )
	{
		UITEXTRENDERBASE_CREATE_INFO* pData = *iter;
		if (NULL == pData)
			continue;

		if (pData->m_nRenderType != nType)
			continue;

		return pData->m_strName.c_str();
	}

	return NULL;
}

void  TextRenderFactory::EnumTextRenderBaseName(
        pfnEnumTextRenderBaseNameCallback callback,
        WPARAM wParam, 
        LPARAM lParam)
{
	UITEXTRENDERBASE_CREATE_DATA::iterator iter = m_vecUITextRenderBaseCreateData.begin();
	for ( ; iter != m_vecUITextRenderBaseCreateData.end(); iter++ )
	{
		UITEXTRENDERBASE_CREATE_INFO* pData = *iter;
		if (NULL == pData)
			continue;

		callback(pData->m_strName.c_str(), wParam, lParam);
	}
}