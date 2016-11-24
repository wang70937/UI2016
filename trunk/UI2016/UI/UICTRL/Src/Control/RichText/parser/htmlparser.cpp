#include "stdafx.h"
#include "htmlparser.h"
#include "..\model\doc.h"
#include "htmlconv.h"
#include "htmlnode_a.h"
#include "htmlnode_b.h"
#include "htmlnode_i.h"
#include "htmlnode_u.h"
#include "htmlnode_br.h"
#include "htmlnode_img.h"
#include "htmlnode_font.h"
#include "htmlnode_big.h"
#include "htmlnode_small.h"
#include "htmlnode_del.h"

using namespace UI;
using namespace UI::RT;

HtmlParser::HtmlParser(RT::Doc& doc) : m_refDoc(doc)
{
	RegiterTag(TAG_A,     TEXT("a"),      HtmlNode_A::CreateInstance);
	RegiterTag(TAG_B,     TEXT("b"),      HtmlNode_B::CreateInstance);
	RegiterTag(TAG_B,     TEXT("i"),      HtmlNode_I::CreateInstance);
	RegiterTag(TAG_U,     TEXT("u"),      HtmlNode_U::CreateInstance);
	RegiterTag(TAG_U,     TEXT("ins"),    HtmlNode_U::CreateInstance);
	RegiterTag(TAG_DEL,   TEXT("s"),      HtmlNode_DEL::CreateInstance);
	RegiterTag(TAG_DEL,   TEXT("del"),    HtmlNode_DEL::CreateInstance);
	RegiterTag(TAG_BR,    TEXT("br"),     HtmlNode_BR::CreateInstance);
	RegiterTag(TAG_IMG,   TEXT("img"),    HtmlNode_IMG::CreateInstance);
	RegiterTag(TAG_FONT,  TEXT("font"),   HtmlNode_FONT::CreateInstance);
	RegiterTag(TAG_IMG,   TEXT("img"),    HtmlNode_IMG::CreateInstance);
	RegiterTag(TAG_BIG,   TEXT("big"),    HtmlNode_BIG::CreateInstance);
	RegiterTag(TAG_SMALL, TEXT("small"),  HtmlNode_SMALL::CreateInstance);
}
HtmlParser::~HtmlParser()
{
	list<HtmlNode*>::iterator iter = m_listParentNode.begin();
	for (; iter != m_listParentNode.end(); ++iter)
	{
		(*iter)->Release();
	}
	m_listParentNode.clear();
}

// TODO: ʹ��״̬�������ý�����������д
bool  HtmlParser::Parse(LPCTSTR szText)
{
	if (!szText)
		return false;

	enum PARSE_STATE
	{
		DATA,
		TAG_ENTER,
	};
	PARSE_STATE eParseState = DATA;


	LPCTSTR p = szText;
	LPCTSTR p2 = p;

	while (*p)
	{
		if (DATA == eParseState)
		{
			p2 = ReadText(p);
			if (p2 != p)
			{
				AddText(p, p2);
				p = p2;
			}
			
			if (!*p)
				break;

			if ('<' == *p)
			{
				eParseState = TAG_ENTER;
				p++;
				continue;
			}
			else
			{
				UIASSERT(0);
				p++;
				continue;
			}
		}
		else if (TAG_ENTER == eParseState)
		{
			p = ParseTag(p);

			eParseState = DATA;
			continue;
		}
		else
		{
            UIASSERT(0);
			p++;
		}
	}
	return true;
}

// ����һ�α�ǩ <> �����ر�ǩ������λ��
// < tag  arg="xx">
//  |              |
//  p            ����ֵ 
//
LPCTSTR  HtmlParser::ParseTag(LPCTSTR p)
{
	p = SkipWhiteSpace(p);
	if (!*p) return p;

	PARSETAGDATA  data;
	data.bStartBacklash = false;
	data.bEndBacklash = false;

	if (*p == TEXT('/'))
	{
		p++;
		p = SkipWhiteSpace(p);
		if (!*p) return p;

		data.bStartBacklash = true;
	}

	// ��ȡ��ǩ��
	LPCTSTR p2 = ReadNextWord(p);
	if (p == p2)
		return SkipOutOfTag(p);

	data.strTagName.assign(p, p2-p);
	p = p2;

	HTMLTAG* pInfo = GetTagInfo(data.strTagName.c_str());
	if (!pInfo)
		return SkipOutOfTag(p2);

	// ������ǩ��
	HtmlNode* pNode = pInfo->pCreator(this);
	if (!pNode)
		return SkipOutOfTag(p2);

	// һ�������ı�ǩ����������
	if (!data.bStartBacklash)
	{
		enum PARSE_TAG_PROP_STATE
		{
			PARSE_KEY,
			PARSE_VALUE,
		};
		PARSE_TAG_PROP_STATE eState = PARSE_KEY;
		String strKey;
		String strValue;

		// ��ȡ��ǩ����
		while (*p && *p != TEXT('>') && *p != TEXT('/'))
		{
			if (eState == PARSE_KEY)
			{
				p = SkipWhiteSpace(p);
				if (!*p) break;

				p2 = ReadNextWord(p);
				strKey.assign(p, p2-p);
				
				// ������һ���Ⱥ�

				p = SkipWhiteSpace(p2);
				if (!*p) break;
		
				if (*p == TEXT('='))
				{
					p++;
					eState = PARSE_VALUE;
				}
			}
			else if (eState == PARSE_VALUE)
			{
				p = SkipWhiteSpace(p);
				if (!*p)
					break;

				// �ж�����ֵ����������
				TCHAR c = 0;
				if (*p == TEXT('\"'))
				{
					p++;
					c = TEXT('\"');
				}
				else if (*p == TEXT('\''))
				{
					p++;
					c = TEXT('\'');
				}
				else 
				{
					// û�����Ű�Χ��Ĭ��Ϊ�����ո����
					// �Ȳ����������ո���TAB
					c = TEXT(' '); 
				}

				p2 = p;
				while (*p && *p != c && *p != TEXT('>'))
					p++;

				strValue.assign(p2, p-p2);


				// ��������
				if (*p && *p == c)
					p++;

				eState = PARSE_KEY;

				String strDecode;
				XmlUnEscape(strValue.c_str(), strDecode);
				data.mapProp[strKey] = strDecode;
				strKey.clear();
				strValue.clear();
			}
		}

		if (*p == TEXT('/'))
			data.bEndBacklash = true;
	}
	p = SkipOutOfTag(p);

	// ����
	pNode->ParseTag(&data);
	pNode->Release();
	return p;
}

bool  IsWhiteSpace(TCHAR c)
{
	if (c == TEXT(' ') || 
		c == TEXT('\r') ||
		c == TEXT('\n') ||
		c == TEXT('\t'))
		return true;

	return false;
}

bool  IsTagControlChar(TCHAR c)
{
	if (c == TEXT('<') ||
		c == TEXT('>') ||
		c == TEXT('/'))
		return true;

	return false;
}

bool  IsPropControlChar(TCHAR c)
{
	if (c == TEXT('=') ||
		c == TEXT('"') ||
		c == TEXT('\''))
		return true;

	return false;
}
bool  IsControlChar(TCHAR c)
{
	if (IsTagControlChar(c) || IsPropControlChar(c))
		return true;

	return false;
}

bool  IsText(TCHAR c)
{
	if (IsWhiteSpace(c) || IsControlChar(c))
		return false;

	return true;
}

LPCTSTR  ReadNextWord(LPCTSTR p)
{
	while (*p && IsText(*p))
	{ p++; }

	return p;
}

LPCTSTR  ReadText(LPCTSTR p)
{
	while (*p && !IsControlChar(*p))
	{ p++; }

	return p;
}

LPCTSTR  SkipWhiteSpace(LPCTSTR p)
{
	while (*p && IsWhiteSpace(*p))
	{ p++; }

	return p;
}

// ����λ�� < ֮�������м�����ݣ�ֱ���ҵ� / ���� >
// TODO: ����������� "a/b" ���� "a>b" ��ô�죿
LPCTSTR  SkipTagContent(LPCTSTR p)
{
	while (*p && (*p != TEXT('/')) && (*p != TEXT('>')))
	{
		p++;
	}
	return p;
}
LPCTSTR  SkipOutOfTag(LPCTSTR p)
{
	while (*p && *p != TEXT('>'))
	{
		p++;
	}

	if (*p == TEXT('>')) // �����>�����������ݽ���
		p++;

	return p;
}

void  HtmlParser::AddText(LPCTSTR pStart, LPCTSTR pEnd)
{
	if (pEnd == pStart)
		return;

	String  str;
	str.append(pStart, pEnd-pStart);
	AddText(str.c_str());
}
void  HtmlParser::AddText(LPCTSTR szText)
{
	String strDecode;
	XmlUnEscape(szText, strDecode);

	HtmlNode_A* pLastA = GetLastLinkContext();
	if (pLastA)
	{
		CharFormat cf = {0};
		m_refDoc.CopyDefaultLinkCharFormat(&cf);
		GetCharFormatOnStack(&cf);

		LinkUnit* pUnit = pLastA->GetLinkUnit();
		if (!pUnit)
		{
			UI::RichTextLinkInfo info = {0};
			info.szText = strDecode.c_str();
			pLastA->SetLinkUnit(m_refDoc.AppendLink2(&info, &cf));
		}
		else
		{
			// link��Ƕ����������ʽ�������ڸ�link���������
			m_refDoc.AppendLink3(pUnit, strDecode.c_str(), &cf);
		}
	}
	else
	{
		CharFormat cf = {0};
		m_refDoc.CopyDefaultCharFormat(&cf);
		GetCharFormatOnStack(&cf);

		m_refDoc.AppendTextEx(strDecode.c_str(), &cf);
	}
}

bool  HtmlParser::AddImg(LPCTSTR szImgPath)
{
	if (!szImgPath)
		return false;

	if (!m_refDoc.AppendImage(szImgPath))
		return false;

	return true;
}

HtmlNode_A*  HtmlParser::GetLastLinkContext()
{
	list<HtmlNode*>::iterator iter = m_listParentNode.begin();
	for (; iter != m_listParentNode.end(); ++iter)
	{
		if ((*iter)->GetTagType() == TAG_A)
			return static_cast<HtmlNode_A*>(*iter);;
	}
	return NULL;
}

//  ��ȡ��ǰ��������ʽ
void  HtmlParser::GetCharFormatOnStack(CharFormat* pcf)
{
	list<HtmlNode*>::reverse_iterator iter = m_listParentNode.rbegin();
	for (; iter != m_listParentNode.rend(); ++iter)
	{
		(*iter)->FillCharFormat(pcf);
	}
}

void  HtmlParser::RegiterTag(HTMLTAGTYPE eType, LPCTSTR szTag, pfnCreateHtmlNode p)
{
	if (!p || !szTag)
		return;

	HTMLTAG info;
	info.eType = eType;
	info.strName = szTag;
	info.pCreator = p;
	m_listTagInfo.push_back(info);
}

HTMLTAG*  HtmlParser::GetTagInfo(LPCTSTR szTag)
{
	if (!szTag)
		return NULL;

	list<HTMLTAG>::iterator iter = m_listTagInfo.begin();
	for (; iter != m_listTagInfo.end(); ++iter)
	{
		if (0 == _tcsicmp(iter->strName.c_str(), szTag))
			return &(*iter);
	}

	return NULL;
}

void  HtmlParser::AddNode(HtmlNode* pNode)
{
	if (!pNode)
		return;

	// ��ǰ�壬����ɾ��ʱ��ʹ��������� erase()��֧��reserve_iterator??
	m_listParentNode.push_front(pNode);
	pNode->AddRef();
}

void  HtmlParser::RemoveLastTagNode(HTMLTAGTYPE eType)
{
	list<HtmlNode*>::iterator iter = m_listParentNode.begin();
	for (; iter != m_listParentNode.end(); ++iter)
	{
		if ((*iter)->GetTagType() == eType)
		{
			(*iter)->Release();
			m_listParentNode.erase(iter);
			break;
		}
	}
}