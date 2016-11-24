#pragma once
#include "Inc\Interface\irichtext.h"

namespace UI
{
namespace RT
{
class Doc;
class Page;
class Line;
class Element;
class Run;
class Paragraph;

struct SingleLineLayoutContext
{
public:
    Doc*   pDoc;
    Page*  pPage;
    Line*  pLine;
    uint   xLineCursor;  // �ڲ��ֹ����У���Line���ֵ�������

    void  PushElement(Element*);
};

// �����Ű�ʱ��������
struct MultiLineLayoutContext
{
public: 
    Doc*   pDoc;
    Page*  pPage;             // ��ǰҳ
    Line*  pLine;             // ��ǰ��
    Paragraph*  pParagraph;
    Element*  pElement;
    Run*   pRun;

    uint  nPageContentWidth;   // ҳ�����ݴ�С
    uint  nPageContentHeight;

    int   nLineSpace;          // �м��
    int   nLeftIndent;         // ���ֹ����е������������message/paragraph����̬����
    int   nRightIndent;        // ���ֹ����е������������message/paragraph����̬����

    uint  yLine;               // ���е�����ʼλ��
    uint  xLineOffset;         // ����run������ʼλ�ã�������left indent

public:
    void  BeginLayoutElement(Element*);
    void  EndLayoutElement(Element*);
    void  PushChar(uint Index, int width);

private:
    void  checkNewLine();

public:
    void  AppendNewLine();
    void  SetLineFinish();
    int   GetLineContentWidth();
    int   getLineRemainWidth();
};

// ���β���ʱ�Ĳ���
struct MultiLineLayoutContext2nd
{
    uint nPageWidth;
    uint nPageLeftIndent;
    uint nPageRightIndent;
};

}
}