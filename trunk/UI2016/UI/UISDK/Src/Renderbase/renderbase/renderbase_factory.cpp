#include "stdafx.h"
#include "renderbase_factory.h"
#include "Src\Renderbase\colorrender\colorrender.h"
#include "Src\Renderbase\imagerender\imagerender.h"
#include "Src\Base\Application\uiapplication.h"

RenderBaseFactory::RenderBaseFactory(UIApplication& app) : m_app(app)
{

}

RenderBaseFactory::~RenderBaseFactory()
{
	Clear();
}

void RenderBaseFactory::Init()
{
	#define REGISTER_UI_RENDERBASE2(classname)  \
        this->RegisterUIRenderBaseCreateData(   \
                classname::GetXmlName(),        \
                classname::GetType(),           \
				(UI::pfnUICreateRenderBasePtr)ObjectCreator<I##classname>::CreateInstance2);

	//REGISTER_UI_RENDERBASE(this, NullRender) -- 2015.4.1���ڣ�����ʹ��
	REGISTER_UI_RENDERBASE2(ColorRender);
	REGISTER_UI_RENDERBASE2(SysColorRender);
	REGISTER_UI_RENDERBASE2(GradientRender);
	REGISTER_UI_RENDERBASE2(ColorListRender);
	REGISTER_UI_RENDERBASE2(ImageRender);
	REGISTER_UI_RENDERBASE2(ImageListItemRender);
	REGISTER_UI_RENDERBASE2(ImageListRender);
}

void RenderBaseFactory::Clear()
{
	UIRENDERBASE_CREATE_DATA::iterator iter = m_vecUIRenderBaseCreateData.begin();
	for ( ; iter != m_vecUIRenderBaseCreateData.end(); iter++ )   
	{
		SAFE_DELETE(*iter);               
	}
	m_vecUIRenderBaseCreateData.clear();     
}


bool  RenderBaseFactory::RegisterUIRenderBaseCreateData(
        LPCTSTR bstrName, 
        int nType, 
        pfnUICreateRenderBasePtr pfunc)
{
	if (NULL == bstrName || NULL == pfunc)
		return false;

	String strName(bstrName);

	UIRENDERBASE_CREATE_INFO* pInfo = new UIRENDERBASE_CREATE_INFO;
	pInfo->m_func = pfunc;
	pInfo->m_nRenderType = nType;
	pInfo->m_strName = strName;
	m_vecUIRenderBaseCreateData.push_back(pInfo);

	// UI_LOG_DEBUG(_T("%s, type=%d, ctrl=%d @ 0x%08X"), bstrName, nType, pfunc);
	return true;
}

bool  RenderBaseFactory::CreateRenderBaseByName(
        ISkinRes* pSkinRes, 
        LPCTSTR strName, 
        IObject* pObject, 
        IRenderBase** ppOut)
{
	if (!strName || !strName[0] || !pObject || !ppOut)
		return false;

	UIRENDERBASE_CREATE_DATA::iterator iter = m_vecUIRenderBaseCreateData.begin();
	for (; iter != m_vecUIRenderBaseCreateData.end(); ++iter)
	{
		UIRENDERBASE_CREATE_INFO* pData = *iter;
		if (!pData)
			continue;

		if (pData->m_strName != strName)
			continue;

		// ����
		//      if (-1 != pData->m_nControlType)
		//      {
		//          if (pObject->GetObjectExtentType() != pData->m_nControlType)
		//              continue;
		//      }
		// 		
		//      if (-1 != pData->m_nControlSubType)
		//      {
		//          int  nStylyEx = pObject->GetStyleEx();
		//          if (GETCONTROLSUBTYPE(nStylyEx) != pData->m_nControlSubType)
		//              continue;
		//      }

		HRESULT hr = pData->m_func(pSkinRes, (void**)ppOut);
		if (SUCCEEDED(hr) && NULL != *ppOut)
		{
            //(*ppOut)->AddRef();
			(*ppOut)->SetObject(m_app.GetIUIApplication(), pObject);
			(*ppOut)->Init();
			(*ppOut)->SetType((RENDER_TYPE)pData->m_nRenderType);
			return true;
		}

		return false;
	}

	UI_LOG_WARN(_T("Create Failed. Name=%s"), strName);
	return false;
}
bool  RenderBaseFactory::CreateRenderBase(
        ISkinRes* pSkinRes, 
        int nType,
        IObject* pObject,
        IRenderBase** ppOut)
{
	if (NULL == ppOut)
		return false;

	UIRENDERBASE_CREATE_DATA::iterator iter = m_vecUIRenderBaseCreateData.begin();
	for ( ; iter != m_vecUIRenderBaseCreateData.end(); iter++ )
	{
		UIRENDERBASE_CREATE_INFO* pData = *iter;
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
			(*ppOut)->SetType((RENDER_TYPE)nType);
			return true;
		}

		return false;
	}

	UI_LOG_WARN(_T("Create Failed. Type=%d"), nType);
	return false;
}

// �������ͻ�ȡ��Ӧ��xml name
LPCTSTR  RenderBaseFactory::GetRenderBaseName(int nType)
{
	UIRENDERBASE_CREATE_DATA::iterator iter = m_vecUIRenderBaseCreateData.begin();
	for ( ; iter != m_vecUIRenderBaseCreateData.end(); ++iter )
	{
		UIRENDERBASE_CREATE_INFO* pData = *iter;
		if (NULL == pData)
			continue;

		if (pData->m_nRenderType != nType)
			continue;

		return pData->m_strName.c_str();
	}

	return NULL;
}

void  RenderBaseFactory::EnumRenderBaseName(pfnEnumRenderBaseNameCallback callback, WPARAM wParam, LPARAM lParam)
{
	UIRENDERBASE_CREATE_DATA::iterator iter = m_vecUIRenderBaseCreateData.begin();
	for ( ; iter != m_vecUIRenderBaseCreateData.end(); ++iter )
	{
		UIRENDERBASE_CREATE_INFO* pData = *iter;
		if (NULL == pData)
			continue;

		callback(pData->m_strName.c_str(), wParam, lParam);
	}
}