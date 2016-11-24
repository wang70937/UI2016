#include "stdafx.h"
#include "htmlnode_br.h"

HtmlNode*  HtmlNode_BR::CreateInstance(HtmlParser* pParser)
{
	HtmlNode_BR* p = new HtmlNode_BR(); 
	p->SetHtmlParser(pParser);
	p->AddRef();
	return p;
}

// <BR> </BR> <BR/> ��Ĭ��Ϊ��һ�������Ļس�
void  HtmlNode_BR::ParseTag(PARSETAGDATA* pData)
{
	m_pHtmlParser->AddText(TEXT("\r\n"));
}