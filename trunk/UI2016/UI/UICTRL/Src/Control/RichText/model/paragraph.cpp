#include "stdafx.h"
#include "paragraph.h"
#include "line.h"
#include "page.h"
#include "font.h"

using namespace RT;


Paragraph::Paragraph()
{
}

Paragraph::~Paragraph()
{
    // SAFE_DELETE(m_pTail);
    SAFE_DELETE(m_pcf);
}

void Paragraph::PreLayoutMultiLine(MultiLineLayoutContext* context)
{
    context->pParagraph = this;
    context->yLine += m_nTopSpace;
    context->AppendNewLine();

    // ��message��װ���򲻴���
    if (GetParentNode() &&
        GetParentNode()->GetNodeType() == NodeType::ChatMessage)
    {
        return;
    }
    
    context->nLeftIndent += m_nLeftIndent;
    context->nRightIndent += m_nRightIndent;
    
}

void Paragraph::PostLayoutMultiLine(MultiLineLayoutContext* context)
{
    context->yLine += m_nBottomSpace;
    context->pParagraph = nullptr;

    // ��message��װ���򲻴���
    if (GetAncestorNodeByType(NodeType::ChatMessageBubble))
    {
        return;
    }

    int nLineMaxWidth = 0;
    if (m_hAlign == HALIGN::paraRight ||
        m_hAlign == HALIGN::paraCenter)
    {
        // �����ֶ��뷽ʽ��Ҫͳ�����ݵ������
        Line* pLine = context->pLine;
        UIASSERT(pLine);

        while (pLine)
        {
            if (&pLine->GetParagraph() != this)
                break;

            int width = pLine->GetRunsWidth();
            if (width > nLineMaxWidth)
                nLineMaxWidth = width;

            pLine = pLine->GetPrevLine();
        }
    }

    // ���к;��ҵ��龰�£�ÿһ�����ݵ���࿪ʼλ�ö���һ������������
    if (m_hAlign == HALIGN::center)
    {
        Line* pLine = context->pLine;
        UIASSERT(pLine);
        while (pLine)
        {
            if (&pLine->GetParagraph() != this)
                break;

            int nLineW = pLine->GetRunsWidth();
            if (nLineW < context->GetLineContentWidth())
            {
                int leftIndex = context->nLeftIndent + (context->GetLineContentWidth() - nLineW) / 2;
                pLine->SetPositionXInPage(leftIndex);
            }

            pLine = pLine->GetPrevLine();
        }
    }
    else if (m_hAlign == HALIGN::right)
    {
        Line* pLine = context->pLine;
        UIASSERT(pLine);
        while (pLine)
        {
            if (&pLine->GetParagraph() != this)
                break;

            int nLineW = pLine->GetRunsWidth();
            int leftIndex =
                    context->nPageContentWidth - context->nRightIndent - nLineW;
            pLine->SetPositionXInPage(leftIndex);

            pLine = pLine->GetPrevLine();
        }
    }
    else
    {
        Line* pLine = context->pLine;
        UIASSERT(pLine);

        // ����ÿһ���Լ�����
        int leftIndex = 0;
        switch (m_hAlign)
        {
        case HALIGN::left:
        {
            leftIndex = context->nLeftIndent;
        }
        break;

        case HALIGN::paraCenter:
        {
            if (nLineMaxWidth < context->GetLineContentWidth())
            {
                leftIndex =
                    context->nLeftIndent +
                    (context->GetLineContentWidth() - context->nLeftIndent -
                    context->nRightIndent) / 2;
            }
        }
        break;

        case HALIGN::paraRight:
        {
            leftIndex =
                context->nPageContentWidth - context->nRightIndent - nLineMaxWidth;
        }
        break;

        }

        while (pLine)
        {
            if (&pLine->GetParagraph() != this)
                break;

            pLine->SetPositionXInPage(leftIndex);

            pLine = pLine->GetPrevLine();
        }
    }

    context->nLeftIndent -= m_nLeftIndent;
    context->nRightIndent -= m_nRightIndent;
}

void Paragraph::SetAlign(RT::HALIGN align)
{
    m_hAlign = align;

//     switch (m_hAlign)
//     {
//     case RT::HALIGN::left:
//     {
//         m_rcMsgIndent.left = 10;
//         m_rcMsgIndent.right = 100;
//     }
//     break;
// 
//     case RT::HALIGN::right:
//     {
//         m_rcMsgIndent.left = 100;
//         m_rcMsgIndent.right = 10;
//     }
//     break;
// 
//     case RT::HALIGN::center:
//     {
//         m_rcMsgIndent.left = 50;
//         m_rcMsgIndent.right = 50;
//     }
//     break;
//     }
}

void  Paragraph::SetLeftIndent(int indent)
{
    m_nLeftIndent = indent;
}
void  Paragraph::SetRightIndent(int indent)
{
    m_nRightIndent = indent;
}
void  Paragraph::SetTopSpace(int space)
{
    m_nTopSpace = space;
}
void  Paragraph::SetBottomSpace(int space)
{
    m_nBottomSpace = space;
}

const CharFormat*  Paragraph::GetCharFormat()
{
    if (m_pcf)
        return m_pcf;

    return Node::GetCharFormat();
}

void  Paragraph::SetCharformat(CharFormat* pcf)
{
    if (!pcf)
    {
        SAFE_DELETE(m_pcf);
        return;
    }

    if (!m_pcf)
        m_pcf = new CharFormat;

    memcpy(m_pcf, pcf, sizeof(CharFormat));
    
    // ���������
    FontCache::Instance().CacheFont(m_pcf);

    // TODO: ���ӽ�����¸��£����������С
}

void  Paragraph::ToString(
        CaretPos start, CaretPos end, int format, String& out)
{
    // ���⴦����һ��para���ӻس�������Ĳ���Ҫ�ӻس�
    // TBD: ��������߼����ܲ�����
    bool bFirstParagraph = true;
    Node* p = this;
    while (p)
    {
        if (p->GetPrevNode())
        {
            bFirstParagraph = false;
            break;
        }
        p = p->GetParentNode();
    }

    if (!bFirstParagraph)
        out.append(TEXT("\r\n"));
}