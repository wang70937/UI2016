#pragma once
#include "run.h"

namespace UI
{
namespace RT
{
class Page;
class Paragraph;

class Line 
{
public:
	Line(Page* pPage, Paragraph& para, int nDefaultHeight);
	~Line();

	void  Draw(HDC hDC, RECT* prcPage);
    void  AppendRun(Run*, uint height);
    void  AddContentWidth(uint width);
	Run*  GetFirstRun();
	Run*  GetLastRun();
	bool  HasRun();
	Line*  GetNextLine();
	Line*  GetPrevLine();
	void  SetNextLine(Line*);
	void  SetPrevLine(Line*);

    Paragraph&  GetParagraph();
    Node*  GetChatMessage();
    Node*  GetChatMessageBubble();

	Run*  RunHitTest(POINT ptInLine, __out POINT* ptInRun);
	void  GetContentRectInPage(RECT* prc);
    void  GetFullRectInPage(RECT* prc);
    void  GetContentRectInDoc(RECT* prc);

    void  SetPosition(uint xInPage, uint yInPage);
    void  SetPositionXInPage(uint);
    void  SetPositionYInPage(uint);
    uint  GetPositionXInPage();
    uint  GetPositionYInPage();
    
    void  SetHeight(uint nHeight);
    uint  GetHeight();
    uint  GetRunsWidth();
	
	void  SetFirstParaLine(bool b);

private:
	Page*  m_pPage;

    // ���������ĸ��Σ�ͨ��model tree�����ϱ�����ȱ�ݡ���һ�����У�û��first runʱ����������
    Paragraph&  m_paragraph;

	Line*  m_pNextLine = nullptr;
    Line*  m_pPrevLine = nullptr;
    Run*   m_pFirstRun = nullptr;

    // ��page content rect�е�λ��
    int   m_xInPageContent = 0;     
    int   m_yInPageContent= 0;  

    // ������������Ҫ�ĸ߶ȡ��������м��
    uint  m_nHeight = 0;     
    // ����������Ŀ�ȣ�������page������ʱ����
    uint  m_nLimitWidth = -1; // δʹ��
    // ��������runs���ܿ��
	uint  m_nRunsWidth = 0;  

	bool  m_bFirstParaLine = false; // ������
};

}
}