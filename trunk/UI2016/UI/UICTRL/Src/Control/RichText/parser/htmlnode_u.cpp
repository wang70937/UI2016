#include "stdafx.h"
#include "HtmlNode_U.h"

HtmlNode* HtmlNode_U::CreateInstance(HtmlParser* pParser)
{
	HtmlNode_U* p = new HtmlNode_U(); 
	p->SetHtmlParser(pParser);
	p->AddRef();
	return p;
}

void  HtmlNode_U::ParseTag(PARSETAGDATA* pData)
{
	if (pData->bStartBacklash)
	{
		// ȡ����ջ�ϵ�һ��<a>
		m_pHtmlParser->RemoveLastTagNode(GetTagType());
	}
	else
	{
		// û�бպϣ������ջ�У��ȴ���һ��</a>
		if (!pData->bEndBacklash)
			m_pHtmlParser->AddNode(this);
	}
}

bool  HtmlNode_U::FillCharFormat(CharFormat* pcf)
{
	pcf->bUnderline = true;
	return true;
}