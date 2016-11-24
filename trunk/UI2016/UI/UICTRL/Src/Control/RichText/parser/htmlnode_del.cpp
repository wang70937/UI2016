#include "stdafx.h"
#include "HtmlNode_DEL.h"

HtmlNode* HtmlNode_DEL::CreateInstance(HtmlParser* pParser)
{
	HtmlNode_DEL* p = new HtmlNode_DEL(); 
	p->SetHtmlParser(pParser);
	p->AddRef();
	return p;
}

void  HtmlNode_DEL::ParseTag(PARSETAGDATA* pData)
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

bool  HtmlNode_DEL::FillCharFormat(CharFormat* pcf)
{
	pcf->bDeleteline = true;
	return true;
}