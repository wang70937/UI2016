#include "stdafx.h"
#include "doc.h"
#include "font.h"
#include "paragraph.h"
#include "page.h"
#include "message.h"
#include "resource.h"
#include "line.h"
#include "link.h"
#include "element/imageelement.h"
#include "element/textelement.h"
#include "../richtext.h"

using namespace UI;
using namespace UI::RT;

Doc::Doc() : m_selection(*this), m_mousekey(*this)
{
	memset(&m_defaultcf, 0, sizeof(m_defaultcf));
	memset(&m_defaultcf_link, 0, sizeof(m_defaultcf_link));

    SetRectEmpty(&m_rcInvalidInDoc);
    m_eInvalidState = InvalidStateEmpty;
    FontCache::Instance().AddRef();

    m_pRichText = NULL;

    m_imageLeftBubble.LoadFromResource(
        g_hInstance, IDB_PNG_RICHTEXT_BUBBLE_DEFAULT_LEFT, L"PNG");
    m_imageRightBubble.LoadFromResource(
        g_hInstance, IDB_PNG_RICHTEXT_BUBBLE_DEFAULT_RIGHT, L"PNG");
}

Doc::~Doc()
{
	Release();
    FontCache::Instance().Release();
}

void  Doc::Init(RichText* pRichText)
{
    m_pRichText = pRichText;
    m_mousekey.Init(pRichText);

	initBlank();
}

// ��ʼ���հ�����, Ĭ��һ��para
void  Doc::initBlank()
{
	UIASSERT(!GetChildNode());
// 	Paragraph* p = new Paragraph;
// 	this->AddChildNode(p);
// 	m_selection.SetAnchor(p, 0);
// 	m_selection.SetFocus(p, 0);	

    m_selection.SetAnchorFocus(this, CaretPos(0));
    CreateDefaultFont();
}

void  Doc::Release()
{
    m_selection.Clear();
    m_mousekey.Clear();
}

PageMode  Doc::GetPageMode()
{
    return m_ePageMode;
}   

void  Doc::SetPageMode(PageMode e)
{
    m_ePageMode = e;
}

RichText*  Doc::GetControl()
{
    return m_pRichText;
}

void  Doc::Render2DC(HDC hDC)
{
    // �ȶ���ģ���������һ�Σ�����doc������message����
    Node::PreDraw(hDC);

    // �پ���Ļ�ÿһ�У�ÿһ���е��ı��������
	Page* p = m_pFirstPage;
	while (p)
	{
		p->Draw(hDC);
		p = p->GetNextPage();
	}
}


void  Doc::CopyDefaultCharFormat(CharFormat* pcf)
{
    if (!pcf)
        return;

    memcpy(pcf, GetDefaultCharFormat(), sizeof(m_defaultcf));

	// ��Щ�ֶβ�����ȥ���ⲿ���øú���ʱ��������Ϊ�˱��ڴ���һ����������
	pcf->hFont = NULL;
	pcf->nFontHeight = 0;
	pcf->nFontDescent = 0;
}
void  Doc::CopyDefaultLinkCharFormat(CharFormat* pcf)
{
	if (!pcf)
		return;

	memcpy(pcf, GetDefaultLinkCharFormat(), sizeof(m_defaultcf_link));

	// ��Щ�ֶβ�����ȥ���ⲿ���øú���ʱ��������Ϊ�˱��ڴ���һ����������
	pcf->hFont = NULL;
	pcf->nFontHeight = 0;
	pcf->nFontDescent = 0;
}

const CharFormat*  Doc::GetDefaultCharFormat()
{
	if (!m_defaultcf.hFont)
		CreateDefaultFont();
	
	return &m_defaultcf;
}
const CharFormat*  Doc::GetDefaultLinkCharFormat()
{
	if (!m_defaultcf_link.hFont)
		CreateDefaultLinkFont();

	return &m_defaultcf_link;
}

void  Doc::ReplaceText(LPCTSTR szText)
{
    ReplaceTextEx(szText, nullptr);
}

void  stringReplaceR(TCHAR* start, TCHAR* end)
{
    TCHAR* p = start;
    while (p <= end)
    {
        if (*p == TEXT('\r'))
            *p = TEXT(' ');

        p++;
    }
}
void  stringReplaceN(TCHAR* start, TCHAR* end)
{
    TCHAR* p = start;
    while (p <= end)
    {
        if (*p == TEXT('\n'))
            *p = TEXT(' ');

        p++;
    }
}

void  Doc::ReplaceTextEx(LPCTSTR _szText, const CharFormat* pcf)
{
    if (!_szText)
        return;
  
    // �ڲ�Ҫ�޸�����ַ��������¿���һ��
    String strText(_szText);


    // 1. ��ǰѡ�������ݣ���ɾ����ǰѡ��
    if (!m_selection.IsEmpty())
    {
        // todo:
    }

    //  �ֲ�س��ַ�
    int  nLength = strText.length();
    if (0 == nLength)
        return;

    LPTSTR pStart = (LPTSTR)strText.c_str();
    LPTSTR pEnd = pStart + nLength;
    LPTSTR p = NULL;

    // ���β���
    do
    {
        p = _tcschr(pStart, TEXT('\n'));

        int nParaLength = 0;
        if (p)
        {
            nParaLength = p - pStart;

            // \nǰ���\r���˵�
            if (nParaLength > 0 && *(p - 1) == TEXT('\r'))  
                nParaLength--;

            // �ٽ��м��\r�滻Ϊ�ո񣬲�֧�ֵ�����\r���س����س�ֻʶ��\n
            stringReplaceR(pStart, pStart + nParaLength);

            if (nParaLength)
                replaceTextNoCarriageReturn(pStart, nParaLength, pcf);
            
            if (m_ePageMode != PageMode::singleline)
            {
                Paragraph* pThisPara = nullptr;
                Node* pNode = m_selection.GetAnchor();
                if (pNode)
                {
                    pThisPara = pNode->GetParagraphNode();
                }

                // ����һ��para
                Paragraph* para = new Paragraph;
                if (pThisPara)
                    para->InsertAfter(pThisPara);
                else
                    this->AddChildNode(para);

                m_selection.SetAnchorFocus(para, CaretPos(0));
            }

            pStart = p + 1;
        }
        else
        {
            nParaLength = pEnd - pStart;

            stringReplaceR(pStart, pStart + nParaLength);
            replaceTextNoCarriageReturn(pStart, nParaLength, pcf);
        }
    } while (p);
}

// �����ı������ı��в������س�����
void  Doc::replaceTextNoCarriageReturn(
    LPCTSTR szText, uint length, const CharFormat* pcf)
{
    // 2. �жϵ�ǰλ�ã��Ƿ���Ժϲ���һ��textelement��
    Node* cp = m_selection.GetAnchor();
    UIASSERT(cp);

    bool bCanMerge = false;

    // �涨��������ֱ�������������,ʵ����������������.
    if (cp->GetNodeType() == NodeType::Text)
    {
        const CharFormat* pcf_cp =
            static_cast<TextElement*>(cp)->GetOriginCharFormat();

        if (!pcf)
        {
            pcf = pcf_cp;
            bCanMerge = true;
        }
        else if (IsSameFormat(pcf_cp, pcf))
        {
            bCanMerge = true;
        }
    }


    uint selPos = m_selection.GetAnchorOffset().pos;

    if (bCanMerge)
    {
        // 3. ����ܺϲ�������
        static_cast<TextElement*>(cp)->InsertTextAt(selPos, szText, length);

        selPos += length;
        m_selection.SetAnchorFocus(cp, CaretPos(selPos));
    }
    else
    {
        // 3. ������ܺϲ�������һ���µ�
        TextElement* pText = new TextElement();
        if (cp->IsRealNode())
        {
            // ������ı�������ǰ�ı����в�֡��������������ֱ�Ӽӵ�����ȥ
            if (cp->GetNodeType() == NodeType::Text)
            {
                static_cast<TextElement*>(cp)->SplitTwoElement(selPos);
            }

            pText->InsertAfter(cp);
        }
        else
        {
            // �����һ����������棬����message
            // �����ǰ����һ��������ȼ�һ������
            if (selPos == -1)
            {
                Paragraph* para = new Paragraph;
                para->InsertAfter(cp);
                para->AddChildNode(pText);
            }
            else if (!cp->GetParagraphNode())
            {
                Paragraph* para = new Paragraph;
                cp->AddChildNode(para);
                para->AddChildNode(pText);
            }
            else
            {
                cp->AddChildNode(pText);
            }
        }

        pText->SetCharFormat(pcf);
        pText->SetText(szText, length);

        uint pos = length;
        m_selection.SetAnchorFocus(pText, CaretPos(pos));
    }
}

void  Doc::LayoutAll(uint nControlWidth, uint nControlHeight)
{
#if 0
    if (nControlHeight == m_nControlHeight &&
        nControlWidth == nControlWidth)
        return;

    m_nControlHeight = nControlHeight;

    // ������û�䣬���ò��֣������о��е������
    // if (m_nControlWidth == nControlWidth)
    //    return;

    m_nControlWidth = nControlWidth;
#endif

    // TODO: �������򵥴���
    SetNeedUpdateElementIndex();

 	PreLayoutAll();
 
    if (nControlWidth == 0 || nControlHeight == 0)
         return;
 
 	switch (m_ePageMode)
 	{
    case PageMode::singleline:
 		{
 			layoutAllSingleLine(nControlWidth, nControlHeight);
 		}
 		break;

    case PageMode::multiline:
 		{
 			layoutAllMultiLine(nControlWidth, nControlHeight);
 		}
 		break;
#if 0
    case PageMode::multipage:
 		{
 			layoutAllMultiPage(nControlWidth, nControlHeight);
 		}
 		break;
#endif		
 	}
}

// ����ǰ���ͷŲ��ֶ���:Page��Line��Run
void  Doc::PreLayoutAll()
{
	releasePage();

    if (m_bNeedUpdateElementIndex)
    {
        UpdateElementIndex();
    }
}

void  Doc::releasePage()
{
	Page* p = m_pFirstPage;
	Page* pNext = NULL;
	while (p)
	{
		pNext = p->GetNextPage();
		delete p;
		p = pNext;
	}
	m_pFirstPage = NULL;
}

void  Doc::layoutAllSingleLine(uint nControlWidth, uint nControlHeight)
{
    m_pFirstPage = new Page(this);
    m_pFirstPage->SetSize(nControlWidth, nControlHeight);

    Node* pParagraph = this->GetChildNode();
    UIASSERT(pParagraph && pParagraph->GetNodeType() == NodeType::Paragraph);

    Line* pSingleLine = new Line(
        m_pFirstPage, 
        *static_cast<Paragraph*>(pParagraph),
        m_defaultcf.nFontHeight);

    m_pFirstPage->AppendLine(pSingleLine);

    SingleLineLayoutContext context;
    context.pDoc = this;
    context.pPage = m_pFirstPage;
    context.pLine = pSingleLine; // Ψһ��
    context.xLineCursor = 0;  // ����run������ʼλ��

    this->LayoutSingleLine(&context);
}


//
// ��һ�β��֣�
//   1. �޶�indent
//   2. ��������line����
//
// �ڶ��β��֣�
//   1. ����line���ݵĶ��뷽ʽ����ʼλ��
//   2. ����������post layout���ٴε�������Ự����
//
void  Doc::layoutAllMultiLine(uint nControlWidth, uint nControlHeight)
{
    m_pFirstPage = new Page(this);

    MultiLineLayoutContext  context = { 0 };
    context.pDoc = this;
    context.pPage = m_pFirstPage;
    context.nPageContentWidth =
        nControlWidth -
        m_style.page_padding.left -
        m_style.page_padding.right;
    context.nPageContentHeight =
        nControlHeight -
        m_style.page_padding.top -
        m_style.page_padding.bottom;

    context.nLineSpace = m_style.line_space;

    this->LayoutMultiLine(&context);

    if (nControlHeight >= 0x7FFFFFFF)  // TODO: GetDesiredSizeʹ��
        return;

    int cySize = context.yLine;
    // ���һ�еĸ߶Ȼ�û�мӵ�yLine��
    if (context.pLine)
        cySize += context.pLine->GetHeight();
    m_mousekey.SetContentSize(0, cySize);
    m_mousekey.SetViewSize(0, context.nPageContentHeight);

    m_pFirstPage->SetSize(nControlWidth, cySize);
}

void  Doc::layoutAllMultiPage(uint nControlWidth, uint nControlHeight)
{
}

void  Doc::InvalidRegion(RECT* prcInDoc)
{
    if (!prcInDoc)
    {
        m_eInvalidState = InvalidStateFull;
        SetRectEmpty(&m_rcInvalidInDoc);
        return;
    }

    if (!IsRectEmpty(prcInDoc))
    {
        UnionRect(&m_rcInvalidInDoc, &m_rcInvalidInDoc, prcInDoc);
        m_eInvalidState = InvalidStatePart;
    }
}

void  Doc::ClearInvlidRegion()
{
    m_eInvalidState = InvalidStateEmpty;
    SetRectEmpty(&m_rcInvalidInDoc);
}

void  Doc::Refresh()
{
    if (m_eInvalidState == InvalidStateEmpty)   
        return;

    if (!m_pRichText)
        return;

	if (m_eInvalidState == InvalidStatePart)
        m_pRichText->GetIRichText()->Invalidate(&m_rcInvalidInDoc);
	else
        m_pRichText->GetIRichText()->Invalidate();

    ClearInvlidRegion();
}

void  Doc::SetDefaultFont(CharFormat* pcf)
{
	if (!pcf)
		return;

	memcpy(&m_defaultcf, pcf, sizeof(m_defaultcf));
	m_defaultcf.hFont = 0;
	m_defaultcf.nFontDescent = 0;
	m_defaultcf.nFontHeight = 0;

	// ����Ĭ������
	FontCache::Instance().GetFont(&m_defaultcf);

}
void  Doc::SetDefaultLinkFont(CharFormat* pcf)
{
	if (!pcf)
		return;

	memcpy(&m_defaultcf_link, pcf, sizeof(m_defaultcf_link));
	m_defaultcf_link.hFont = 0;
	m_defaultcf_link.nFontDescent = 0;
	m_defaultcf_link.nFontHeight = 0;

	// ����Ĭ������
	FontCache::Instance().GetFont(&m_defaultcf_link);
}

void  Doc::CreateDefaultFont()
{
	if (m_defaultcf.hFont)
		return;

	memset(&m_defaultcf, 0, sizeof(m_defaultcf));
	_tcscpy(m_defaultcf.szFontName, TEXT("΢���ź�"));
	m_defaultcf.lFontSize = 9;
	

	FontCache::Instance().GetFont(&m_defaultcf);
}
void  Doc::CreateDefaultLinkFont()
{
	if (m_defaultcf_link.hFont)
		return;

	memset(&m_defaultcf_link, 0, sizeof(m_defaultcf_link));
	_tcscpy(m_defaultcf_link.szFontName, TEXT("΢���ź�"));
	m_defaultcf_link.lFontSize = 9;
	m_defaultcf_link.bUnderline = false;  // �»������Լ�����������ഴ��һ������
	m_defaultcf_link.textColor = RGB(0,0,204);
	m_defaultcf_link.bTextColor = true;

	FontCache::Instance().GetFont(&m_defaultcf_link);
}

Style&  Doc::GetStyle()
{
    return m_style;
}

Selection&  Doc::GetSelection()
{
    return m_selection;
}

MouseKey&  Doc::GetMouseKey()
{
    return m_mousekey;
}

Paragraph*  Doc::GetSelectionAnchorParagraph()
{
    Node* node = m_selection.GetAnchor();
    if (!node)
        return nullptr;

    return node->GetParagraphNode();
}

SIZE  Doc::GetDesiredSize()
{
	
    SIZE s = { 0, 0 };
#if 0	
    if (!m_pFirstPage)
        return s;

    if (m_ePageMode == PAGE_MODE_SINGLELINE)
    {
        Line* pLine = m_pFirstPage->GetFirstLine();
        if (pLine)
        {
            s.cx = pLine->GetRunsWidth();
            s.cy = pLine->GetHeight();
        }
    }
    else if (m_ePageMode == PAGE_MODE_MULTILINE)
    {
        Line* pLine = m_pFirstPage->GetLastLine();
        if (pLine)
        {
            s.cy = pLine->GetHeight() + pLine->GetPositionYInPage();
        }

        pLine = m_pFirstPage->GetFirstLine();
        while (pLine)
        {
            int w = pLine->GetRunsWidth();
            if (w > s.cx)
                s.cx = w;

            pLine = pLine->GetNextLine();
        }
    }
    else if (m_ePageMode == PAGE_MODE_MULTIPAGE)
    {
        // TODO:
        UIASSERT(0);
    }
#endif
    return s;
}

EDITABLE  Doc::GetEditMode()
{
    return m_eEditMode;
}

void Doc::SetEditMode(EDITABLE e)
{
    if (e == EDITABLE::Enable)
    {
        UIASSERT(0 && "Not implement");
    }
    m_eEditMode = e;
}

void Doc::CopySelection()
{
    if (m_eEditMode == EDITABLE::Disable)
        return;

    if (m_selection.IsEmpty())
        return;

    String str;
    m_selection.ToString(str);

    if (OpenClipboard(NULL))
    {
        EmptyClipboard();

        int nSize = sizeof(TCHAR) * (str.length() + 1);
        HGLOBAL hBlock = GlobalAlloc(GMEM_MOVEABLE, nSize);
        if (hBlock)
        {
            void* p = GlobalLock(hBlock);
            memcpy(p, str.c_str(), nSize);
            GlobalUnlock(hBlock);
        }
        SetClipboardData(CF_UNICODETEXT, hBlock);

        CloseClipboard();
        // We must not free the object until CloseClipboard is called.
        if (hBlock)
            GlobalFree(hBlock);
    }
}

void Doc::SelectAll()
{
    if (m_eEditMode == EDITABLE::Disable)
        return;

    m_selection.Clear();

    Node* pLast = this;
    Node* pTemp = nullptr;
    while (pLast)
    {
        pTemp = pLast->GetLastChildNode();
        if (!pTemp)
            break;;
        pLast = pTemp;
    }
    UIASSERT(pLast);

    m_selection.SetAnchor(this, 0);
    m_selection.SetFocus(pLast, -1);

    InvalidRegion(nullptr);
    Refresh();
}

// TODO: ��û�и��õķ������ж����������Ⱥ�˳��
void Doc::SetNeedUpdateElementIndex()
{
    m_bNeedUpdateElementIndex = true;
}

void  Doc::UpdateElementIndex()
{
    UIASSERT(m_bNeedUpdateElementIndex);
    m_bNeedUpdateElementIndex = false;

    int index = 0;
    Node* p = this;
    while (p)
    {
        p->SetIndex(++index);
        p = p->GetNextTreeNode();
    }
}


Page*  Doc::PageHitTest(POINT ptDoc, __out POINT* ptInPage)
{
    if (!m_pFirstPage)
        return NULL;

    if (m_ePageMode == PageMode::singleline || m_ePageMode == PageMode::multiline)
    {
        RECT rcPage;
        m_pFirstPage->GetContentRectInDoc(&rcPage);

        if (PtInRect(&rcPage, ptDoc))
        {
            if (ptInPage)
            {
                ptInPage->x = ptDoc.x - rcPage.left;
                ptInPage->y = ptDoc.y - rcPage.top;
            }
            return m_pFirstPage;
        }

        return NULL;
    }

    if (m_ePageMode == PageMode::multipage)
    {
        Page* p = m_pFirstPage;
        while (p)
        {
            if (p->IsVisible())
            {
                RECT rcPage;
                p->GetContentRectInDoc(&rcPage);

                if (PtInRect(&rcPage, ptDoc))
                {
                    if (ptInPage)
                    {
                        ptInPage->x = ptDoc.x - rcPage.left;
                        ptInPage->y = ptDoc.y - rcPage.top;
                    }
                    return p;
                }
            }
            p = p->GetNextPage();
        }

        return NULL;
    }

    return NULL;
}


Page*  Doc::GetLastPage()
{
    if (!m_pFirstPage)
        return nullptr;

    Page* p = m_pFirstPage;
    while (p)
    {
        if (!m_pFirstPage->GetNextPage())
            return p;

        p = m_pFirstPage->GetNextPage();
    }

    return nullptr;
}
Page*  Doc::GetFirstPage()
{
    return m_pFirstPage;
}

const CharFormat* Doc::GetCharFormat()
{
    return &m_defaultcf;
}

IChatMessage*  Doc::BeginInsertMessage()
{
    ChatMessage* message = new ChatMessage(*this);

    // ��ȡ��ǰλ�õĶ�����㣬���ں��档
    // ĿǰԼ����message���ֻ������doc�������

    // ֱ�������µ��У����ڵ�ǰ������
    Node* p = m_selection.GetAnchor();
    while (p)
    {
        if (p->GetParentNode() == this)
            break;

        p = p->GetParentNode();
    }
    if (p)
    {
        message->InsertAfter(p);
    }
    else
    {
        this->AddChildNode(message);
    }
    

    m_selection.SetAnchorFocus(message, CaretPos(0));
    return static_cast<IChatMessage*>(message);
}

// �����ŵ�message����ȥ
void  Doc::EndInsertMessage(IChatMessage* pMsg)
{
    UIASSERT(pMsg);
    ChatMessage* msg = static_cast<ChatMessage*>(pMsg);
    // ��-1����ʾ�������������棬������������  ???????
    m_selection.SetAnchorFocus(
        static_cast<Node*>(msg), CaretPos(-1));
}

ILink*  Doc::ReplaceLink(LPCTSTR szText)
{
    return ReplaceLinkEx(szText, nullptr);
}
ILink*  Doc::ReplaceLinkEx(LPCTSTR szText, const CharFormat* pcf)
{
    // Լ�������Ӳ�����\r\n��ֻ����һ��

    Link* link = new Link;

    Node* cp = m_selection.GetAnchor();
    UIASSERT(cp);

    uint selPos = m_selection.GetAnchorOffset().pos;

    if (cp->IsRealNode())
    {
        link->InsertAfter(cp);
    }
    else
    {
        if (selPos == -1)
        {
            Paragraph* para = new Paragraph;
            para->InsertAfter(cp);
            para->AddChildNode(link);
        }
        else
        {
            cp->AddChildNode(link);
        }
    }

    m_selection.SetAnchorFocus(link, CaretPos(0));
    // link->SetCharFormat(pcf);
 
    ReplaceTextEx(szText, nullptr);

    return static_cast<ILink*>(link);
}

void  Doc::ReplaceImage(LPCTSTR szPath)
{
    ImageElement* image = new ImageElement;

    Node* cp = m_selection.GetAnchor();
    UIASSERT(cp);

    uint selPos = m_selection.GetAnchorOffset().pos;

    if (cp->IsRealNode())
    {
        image->InsertAfter(cp);
    }
    else
    {
        if (selPos == -1)
        {
            Paragraph* para = new Paragraph;
            para->InsertAfter(cp);
            para->AddChildNode(image);
        }
        else
        {
            cp->AddChildNode(image);
        }
    }

    m_selection.SetAnchorFocus(image, CaretPos(0));

    image->Load(szPath);
}