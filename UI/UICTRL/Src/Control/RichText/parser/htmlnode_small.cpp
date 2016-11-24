#include "stdafx.h"
#include "HtmlNode_SMALL.h"

HtmlNode* HtmlNode_SMALL::CreateInstance(HtmlParser* pParser)
{
	HtmlNode_SMALL* p = new HtmlNode_SMALL(); 
	p->SetHtmlParser(pParser);
	p->AddRef();
	return p;
}

void  HtmlNode_SMALL::ParseTag(PARSETAGDATA* pData)
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

bool  HtmlNode_SMALL::FillCharFormat(CharFormat* pcf)
{
	int g_fontSize[] = {8,9,10,11,12,14,16,18,20,22,24,26,28,36,48,72};
	int nSize = sizeof(g_fontSize)/sizeof(int);
	for (int i = 0; i < nSize; i++)
	{
		if (g_fontSize[i] < pcf->lFontSize)
			continue;

		if (i==0)
			pcf->lFontSize = g_fontSize[0];
		else
			pcf->lFontSize = g_fontSize[i-1];

		break;
	}
	return true;
}