#include "stdafx.h"
#include "htmlnode_b.h"

HtmlNode* HtmlNode_B::CreateInstance(HtmlParser* pParser)
{
	HtmlNode_B* p = new HtmlNode_B(); 
	p->SetHtmlParser(pParser);
	p->AddRef();
	return p;
}

void  HtmlNode_B::ParseTag(PARSETAGDATA* pData)
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

bool  HtmlNode_B::FillCharFormat(CharFormat* pcf)
{
	pcf->bBold = true;
	return true;
}