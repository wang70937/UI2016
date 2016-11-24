#include "stdafx.h"
#include "htmlnode_img.h"

HtmlNode* HtmlNode_IMG::CreateInstance(HtmlParser* pParser)
{
	HtmlNode_IMG* p = new HtmlNode_IMG(); 
	p->SetHtmlParser(pParser);
	p->AddRef();
	return p;
}

// �涨�� <img/> �����Ǹ������ı�ǩ��ĩβ���Ͻ�����
void  HtmlNode_IMG::ParseTag(PARSETAGDATA* pData)
{
	// </img> ��Ч
	if (pData->bStartBacklash)
		return;

	// <img> ��Ч
	if (!pData->bEndBacklash)
		return;

	String strPath;
	String strAlt;

	PARSETAGDATA::_Iter iter = pData->mapProp.find(TEXT("src"));
	if (iter == pData->mapProp.end())
		return;

	strPath = iter->second;
	iter = pData->mapProp.find(TEXT("alt"));
	if (iter != pData->mapProp.end())
		strAlt = iter->second;

	m_pHtmlParser->AddImg(strPath.c_str());
}