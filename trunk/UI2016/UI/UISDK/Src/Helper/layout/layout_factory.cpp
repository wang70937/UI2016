#include "stdafx.h"
#include "layout_factory.h"

#include "Src\Helper\layout\layout.h"
#include "Src\Helper\layout\horzlayout.h"
#include "Src\Helper\layout\vertlayout.h"
#include "Src\Helper\layout\canvaslayout.h"
#include "Src\Helper\layout\cardlayout.h"
#include "Src\Helper\layout\gridlayout.h"
#include "Src\Helper\layout\averagelayout.h"


LayoutFactory::LayoutFactory()
{
	
}

LayoutFactory::~LayoutFactory()
{
	Clear();
}

void  LayoutFactory::Init()
{
	Register(XML_LAYOUT_CANVAS, LAYOUT_TYPE_CANVAS, CanvasLayout::CreateInstance);
	Register(XML_LAYOUT_HORZ,   LAYOUT_TYPE_HORZ, HorzLayout::CreateInstance);
	Register(XML_LAYOUT_VERT,   LAYOUT_TYPE_VERT, VertLayout::CreateInstance);
	Register(XML_LAYOUT_CARD,   LAYOUT_TYPE_CARD, CardLayout::CreateInstance);
	Register(XML_LAYOUT_GRID,   LAYOUT_TYPE_GRID, GridLayout::CreateInstance);
	Register(XML_LAYOUT_AVERAGE, LAYOUT_TYPE_AVERAGE, AverageLayout::CreateInstance);
}

bool  LayoutFactory::Register(LPCTSTR szName, int nType, pfnUICreateLayoutPtr pfun)
{
	if (NULL == pfun || NULL == szName)
	{
		UI_LOG_WARN(_T("invalid arg. name=%s, pfun=%x"), szName, pfun);
		return false;
	}

	int nSize = (int)m_vecData.size();
	for (int i = 0; i < nSize; i++)
	{
		UILAYOUT_CREATE_INFO* pData = m_vecData[i];
		if (pData && (pData->m_strName == szName || pData->m_nLayoutType == nType))
		{
			UI_LOG_WARN(_T("register duplicate. name=%s, type=%d, pfun=%x"), szName, nType, pfun);
			return false;
		}
	}

	UILAYOUT_CREATE_INFO* pData = new UILAYOUT_CREATE_INFO;
	pData->m_strName = szName;
	pData->m_nLayoutType = nType;
	pData->m_func = pfun;

	m_vecData.push_back(pData);
	return true;
}


bool  LayoutFactory::Create(int nType, IObject* pObject, ILayout** ppOut)
{
	if (NULL == ppOut)
		return false;

	pfnUICreateLayoutPtr  pFunc = NULL;
	UILAYOUT_CREATE_DATA::iterator iter = m_vecData.begin();
	for ( ; iter != m_vecData.end(); iter++ )
	{
		UILAYOUT_CREATE_INFO* pData = *iter;
		if (pData && pData->m_nLayoutType == nType)
		{
			pFunc = pData->m_func;
			break;
		}
	}

	if (NULL == pFunc)
		return false;

	pFunc(pObject, ppOut);
	return true;
}
bool  LayoutFactory::CreateByName(LPCTSTR szName, IObject* pObject, bool bCreateDefault, ILayout** ppOut)
{
	if (NULL == ppOut)
		return false;

	pfnUICreateLayoutPtr  pFunc = NULL;
	if (szName)  // ΪNULL�п�����û������layout���ԣ�Ĭ�ϴ���һ��canvas����
	{
		UILAYOUT_CREATE_DATA::iterator iter = m_vecData.begin();
		for ( ; iter != m_vecData.end(); iter++ )
		{
			UILAYOUT_CREATE_INFO* pData = *iter;
			if (pData && pData->m_strName == szName)
			{
				pFunc = pData->m_func;
				break;
			}
		}
	}

	if (NULL == pFunc && bCreateDefault && m_vecData.size() > 0)
		pFunc = m_vecData[0]->m_func;

	if (NULL == pFunc)
		return false;

	pFunc(pObject, ppOut);
	return true;
}

void  LayoutFactory::EnumLayoutType(pfnEnumLayoutTypeCallback callback, WPARAM wParam, LPARAM lParam)
{
	if (!callback)
		return;

	UILAYOUT_CREATE_DATA::iterator iter = m_vecData.begin();
	for ( ; iter != m_vecData.end(); iter++ )
	{
		if (!callback((*iter)->m_nLayoutType, (*iter)->m_strName.c_str(), wParam, lParam))
			break;
	}
}

void  LayoutFactory::Clear()
{
	UILAYOUT_CREATE_DATA::iterator iter = m_vecData.begin();
	for ( ; iter != m_vecData.end(); iter++ )   
	{
		SAFE_DELETE(*iter);               
	}
	m_vecData.clear();                          
}
