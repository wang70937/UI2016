#pragma once

namespace UI
{
namespace RT
{
class Line;
class Element;
class Doc;

class Page
{
public:
	Page(Doc* pDoc);
	~Page();

	Page*  GetNextPage();
	Page*  GetPrevPage();
	void   AppendLine(Line* pLine);
	Line*  GetFirstLine();
	Line*  GetLastLine();
	Line*  LineHitTest(POINT ptInPage, __out POINT* ptInLine);
    Line*  NearestLineHitTest(POINT ptInDoc);
	void  PageRect2DocRect(RECT* prcPage, RECT* prcDoc);
    void  GetContentRectInDoc(RECT* prc);
    void  GetRectInDoc(LPRECT);

	void  Draw(HDC hDC);
    void  SetSize(int cx, int cy);
	void  SetVisible(bool b);
	bool  IsVisible();
	
private:
    

private:
	Doc*   m_pDoc;
	Page*  m_pNextPage;
	Page*  m_pPrevPage;
	Line*  m_pFirstLine;

    POINT  m_posInDoc;    // ҳ����doc�е�λ��
    SIZE   m_sizePage;    // ҳ���С(px)
	
	bool   m_bVisible;
};

}
}