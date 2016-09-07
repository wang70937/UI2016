#include "stdafx.h"
#include "skinparseengine.h"
#include "Inc\Interface\ixmlwrap.h"
#include "Inc\Interface\iskindatasource.h"
#include "Src\Resource\skinres.h"
#include "Inc\Interface\iuires.h"
#include "xml\pugixmlwrap\pugixmlwrap.h"
#include "datasource\skindatasource.h"
#include "..\Base\Application\uiapplication.h"

using namespace UI;

SkinParseEngine::SkinParseEngine(SkinRes* pSkinRes)
{   
	UIASSERT(pSkinRes);
    m_pSkinRes = pSkinRes;
	m_pUIApplication = m_pSkinRes->GetUIApplication();
}

SkinParseEngine::~SkinParseEngine()
{
}

bool  SkinParseEngine::Parse(
			SkinDataSource* pDataSource, 
			LPCTSTR szXmlFile)
{
#if 0
	1. ���� <skin> root element�����û���ҵ���ֱ�ӷ��أ���ʾ�ⲻ��һ���Ϸ��ļ�
		2. ���� <skin> ��child element

		2.1 ��ȡtagName������image
		2.2 ��ȡimage��ǩ��Ӧ�Ľ����� IImageParse
		2.3 ����IImageParse��NewElement
		2.4 IImageParse�Լ�ȥ��������ӽ��
#endif
	if (!pDataSource || !szXmlFile)
		return false;

    UIDocument*  pUIDocument = NULL;
    CreateXmlDocument(XML_ENGINE_DEFAULT, &pUIDocument);
	if (!pDataSource->Load_UIDocument(pUIDocument, szXmlFile))
	{
		SAFE_RELEASE(pUIDocument);
		return false;
	}
	pUIDocument->SetSkinPath(szXmlFile);	

	do 
	{
		UIElementProxy rootElem = pUIDocument->FindElem(XML_SKIN);
		if (!rootElem)
		{
			UI_LOG_ERROR(_T("Cannot find root element: %s"), XML_SKIN);
			break;
		}

		m_pSkinRes->OnNewUIDocument(pUIDocument);

		UIElementProxy childElem = rootElem->FirstChild();
		while (childElem)
		{
			this->NewChild(childElem.get());
			childElem = childElem->NextElement();
		}
	}
	while (0);

	SAFE_RELEASE(pUIDocument);	
	return true;
}

void  SkinParseEngine::NewChild(UIElement* pElement)
{
	if (!pElement)
		return;

    pfnParseSkinTag func;
    if (!m_pUIApplication->GetSkinTagParseFunc(
				(LPCWSTR)pElement->GetTagName(), &func))
	{
        return;
	}

    func(pElement->GetIUIElement(), m_pSkinRes->GetISkinRes());
}


HRESULT  SkinParseEngine::UIParseIncludeTagCallback(IUIElement* pElement, ISkinRes* pSkinRes)
{
    if (NULL == pElement || NULL == pSkinRes)
        return E_FAIL;

    LPCTSTR szData = pElement->GetData();
    if (!szData)
		return E_FAIL;

    SkinParseEngine  parse(pSkinRes->GetImpl());
	SkinDataSource*  pDataSource = pSkinRes->GetImpl()->GetDataSource();

    if (!parse.Parse(pDataSource, szData))
        return E_FAIL;

    return S_OK;
}

namespace UI
{
bool  CreateXmlDocument(XML_ENGINE e, UIDocument** pp)
{
	if (!pp)
		return false;

	*pp = NULL;

    switch (e)
    {
//     case MSXML:
//         {
//             return MsXmlDocument::CreateInstance(pp);
//         }
//         break;

    case PUGIXML:
    case XML_ENGINE_DEFAULT:
        {
            UIDocument* p = new PugiXmlDocument;
			p->AddRef();
			*pp = p;
			return true;
        }
        break;
    }

    return false;
}
}