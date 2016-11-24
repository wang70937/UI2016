#include "stdafx.h"
#include "HtmlNode_I.h"

HtmlNode* HtmlNode_I::CreateInstance(HtmlParser* pParser)
{
	HtmlNode_I* p = new HtmlNode_I(); 
	p->SetHtmlParser(pParser);
	p->AddRef();
	return p;
}

void  HtmlNode_I::ParseTag(PARSETAGDATA* pData)
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

bool  HtmlNode_I::FillCharFormat(CharFormat* pcf)
{
	pcf->bItalic = true;
	return true;
}