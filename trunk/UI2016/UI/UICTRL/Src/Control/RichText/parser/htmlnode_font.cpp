#include "stdafx.h"
#include "htmlnode_font.h"
#include "..\UISDK\Inc\Util\util.h"

HtmlNode_FONT::HtmlNode_FONT()
{
	m_bDefineColor = false;
	m_bDefineHeight = false;
	m_bDefineFace = false;

	m_color = 0;
	m_size = 9;
}

HtmlNode* HtmlNode_FONT::CreateInstance(HtmlParser* pParser)
{
	HtmlNode_FONT* p = new HtmlNode_FONT(); 
	p->SetHtmlParser(pParser);
	p->AddRef();
	return p;
}

//
// ���ԣ�size="" face="" color=""
// �����size������html�е�h1---h7������font height
//
void  HtmlNode_FONT::ParseTag(PARSETAGDATA* pData)
{
	if (pData->bStartBacklash)
	{
		// ȡ����ջ�ϵ�һ��<font>
		m_pHtmlParser->RemoveLastTagNode(GetTagType());
	}
	else
	{
		// û�бպϣ������ջ�У��ȴ���һ��</font>
		if (!pData->bEndBacklash)
		{
			m_pHtmlParser->AddNode(this);

			PARSETAGDATA::_Iter iter;

			iter = pData->mapProp.find(TEXT("face"));
			if (iter != pData->mapProp.end())
			{
				m_bDefineFace = true;
				m_face = iter->second;
			}
			iter = pData->mapProp.find(TEXT("size"));
			if (iter != pData->mapProp.end())
			{
				m_bDefineHeight = true;
				m_size = _ttoi(iter->second.c_str());
			}
			iter = pData->mapProp.find(TEXT("color"));
			if (iter != pData->mapProp.end())
			{
				m_bDefineColor = true;
				m_color = Util::TranslateColor(iter->second.c_str());
			}
		}
	}
}

bool  HtmlNode_FONT::FillCharFormat(CharFormat* pcf)
{
	if (m_bDefineColor)
	{
		pcf->bTextColor = true;
		pcf->textColor = m_color;
	}
	if (m_bDefineFace)
	{
		_tcsncpy(pcf->szFontName, m_face.c_str(), LF_FACESIZE-1);
	}
	if (m_bDefineHeight)
	{
		pcf->lFontSize = m_size;
	}
	return true;
}