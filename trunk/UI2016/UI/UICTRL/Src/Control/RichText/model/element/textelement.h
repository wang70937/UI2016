#pragma once
#include "element.h"
#include "unit.h"
#include <usp10.h>

namespace UI
{
namespace RT
{

enum TextElementType
{
	TET_TEXT = 0,
	TET_LINK = 1
};
class TextUnit;
class TextElement : public Element
{
public:
    TextElement();
	~TextElement();

    virtual NodeType  GetNodeType() { return NodeType::Text; }

    virtual void  Draw(HDC hDC, Run* pRun, RECT* prcRun) override;
	virtual uint  GetWidth() override;
	virtual uint  GetHeight() override;
	virtual uint  GetCharacters() override;
    virtual void  ToString(
             CaretPos start, 
             CaretPos end, 
             int format,
             String& out
         ) override;

    virtual void LayoutMultiLine(MultiLineLayoutContext*) override;

    virtual const CharFormat*  GetCharFormat() override;

public:
	void  SetText(LPCTSTR szText, int nLength=-1);
    void  InsertTextAt(uint pos, LPCTSTR szText, int nLength = -1);
    void  SplitTwoElement(uint pos);

	uint  GetWidthByFrom(uint nFrom);
    uint  GetWidthByTo(uint nTo);
    uint  GetWidthByFromTo(uint nFrom, uint nTo);
	bool  GetRangeToByWidthFrom(
            uint nFrom, 
            __inout uint* pWidth, 
            __out uint* pnTo);
    CaretPos  SelectionHitTest(uint nFrom, int pos);
    
    const CharFormat*  GetOriginCharFormat();
    void  SetCharFormat(const CharFormat* pcf);

private:
    void  analyze();
    void  DrawText(
        HDC hDC, RECT* prc, LPCTSTR szText,
        ULONG nLen, const CharFormat* pcf);

protected:
	String            m_strText;
	vector<int>       m_advances;   // ÿ���ַ��Ŀ��
	uint              m_nABCWidth;  // �ı��ܿ��

    bool  m_bNeedAnalyze;
	// ΪNULL��ʾʹ��unit������
	CharFormat*  m_pcf;  
};

}
}