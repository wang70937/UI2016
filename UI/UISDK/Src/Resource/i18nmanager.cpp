#include "stdafx.h"
#include "i18nmanager.h"
#include "skinres.h"
#include "Inc\Interface\ixmlwrap.h"
#include "skinmanager.h"

I18nManager::~I18nManager()
{
    for (auto iter : m_listElement)
    {
        iter->Release();
    }
    m_listElement.clear();
}

HRESULT  I18nManager::UIParseI18nTagCallback(IUIElement* pElem, ISkinRes* pSkinRes)
{
	I18nManager& mgr = pSkinRes->GetImpl()->GetI18nManager();
	return mgr.ParseNewElement(pElem->GetImpl());
}

/*
  <i18n>
    <string key="ok">
      <string id="ok" zh_cn="ȷ��" en_us="OK"/>
    </string>
  </i18n>
*/
LRESULT  I18nManager::ParseNewElement(UIElement* p)
{
    UIASSERT(p);
    p->AddRef();
    m_listElement.push_back(p);

    parse(p);

    return S_OK;
}

void  I18nManager::parse(UIElement* p)
{
	LPCTSTR szLanguage = m_pSkinRes->GetSkinMgr().GetCurrentLanguage();

	UIElementProxy xml = p->FirstChild();
	while (xml)
	{
		CComBSTR strId, strValue;
		xml->GetAttrib(XML_ID, &strId);
		xml->GetAttrib(szLanguage, &strValue);

		m_i18nRes.Add(strId, strValue);
		xml = xml->NextElement();
	}
}

void  I18nManager::Reload()
{
    m_i18nRes.Clear();

    for (auto iter : m_listElement)
    {
        parse(iter);
    }
}