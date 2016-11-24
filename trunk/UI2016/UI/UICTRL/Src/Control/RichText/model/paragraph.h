#pragma once
#include "element\element.h"
#include "layout_context.h"


//
// ���ʵ��message�����ݽṹ��
//  ����1. message����ά������unitָ�룬begin/end�������м�Ķ����ڸ�message
//  �İ�2. ��message��Ϊһ�������unit��Ҳ���뵽���ݵ��У���������msg unit�м�ģ���
//        ����ͬһ��message
//
//  ����1��Ҫά���Ķ������࣬�����ݸ�������Ƚ��鷳������Ҫ��������������ǰ���������ݾͲ���ʵ��
//  ��������÷���2��ʵ�֣�һ��message��Ϊ����unit�������ݰ�������
//

namespace UI
{
namespace RT{

class ChatDoc;
class Paragraph;
class ParagraphTail;


class Paragraph : public RT::Node, public RT::IParagraph
{
public:
    Paragraph();

    ~Paragraph();

    virtual NodeType  GetNodeType() override { return NodeType::Paragraph; }

    virtual void  SetAlign(RT::HALIGN align) override;
    virtual void  SetLeftIndent(int indent) override;
    virtual void  SetRightIndent(int indent) override;
    virtual void  SetTopSpace(int space) override;
    virtual void  SetBottomSpace(int space) override;
    virtual void  SetCharformat(CharFormat*) override;
    virtual void  ShowBubbleBackgournd(bool b) override{};

    virtual const CharFormat*  GetCharFormat() override;
    virtual void  ToString(
            CaretPos start, CaretPos end, int format, String& out) override;
//     virtual void  LayoutMultiLine(
//         RT::MultiLineLayoutContext* pContext) override;

    //void  CreateDefaultTail();

protected:
    virtual void PreLayoutMultiLine(MultiLineLayoutContext*) override;
    virtual void PostLayoutMultiLine(MultiLineLayoutContext*) override;

private:
    // ������뷽ʽ
    RT::HALIGN  m_hAlign = HALIGN::left;
    // �μ��
    int  m_nTopSpace = 0;
    int  m_nBottomSpace = 0;
    int  m_nLeftIndent = 0;
    int  m_nRightIndent = 0;
    // �м��
    int  m_nLineSpace = 0;
    // �����Ĭ������
    CharFormat*  m_pcf = nullptr;  
};
#if 0
class Paragraph : 
{
public:
    Paragraph(ChatDoc& doc);

    virtual void  SetAlign(RT::HALIGN align) override;
    virtual void  ShowBubbleBackgournd(bool b) override;

    RT::Unit*  CreateHeadUnit();
    RT::Unit*  CreateTailUnit();

    void  PostLayout();
    void  PreDraw(HDC hDC);

    CRegion4  GetMessageIndent();

private:
    RT::Line*  GetHeadLine();
    RT::Line*  GetTailLine();
    
private:
    ChatDoc&  m_doc;

    bool  m_bDrawBubbleBackground = true;
    CRect  m_rcBubble;

    // �������ݵ�����������Ϊ�����õ�λ��
    CRegion4  m_rcMsgIndent;

    // �������ڣ���chatmessage����, doc�������٣�
    ChatMessageHead*  m_pHead = nullptr;
    ChatMessageTail*  m_pTail = nullptr;

    RT::HALIGN  m_hAlign;
};
#endif
}
}