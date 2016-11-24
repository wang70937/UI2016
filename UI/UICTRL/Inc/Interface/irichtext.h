#ifndef _UI_IRICHTEXT_H_
#define _UI_IRICHTEXT_H_
#include "..\..\..\UISDK\Inc\Interface\icontrol.h"

namespace UI
{

struct CharFormat
{
    // HFONT����
    TCHAR  szFontName[LF_FACESIZE];
    long  lFontSize;
    bool  bBold;
    bool  bItalic;
    bool  bUnderline;
	bool  bDeleteline;

    // ��������
    long  textColor;
    long  backColor;
    bool  bTextColor;  // �Ƿ���������ɫtextColor
    bool  bBackColor;  // �Ƿ����ñ���ɫbackColor

    // �ڲ�ʹ��
    HFONT  hFont;
    unsigned int  nFontHeight;
    unsigned int  nFontDescent;  // �����»��ߵĻ����������
};



enum class PageMode
{
    singleline,
    multiline,
    multipage,
    error,
};

namespace RT
{
// Ŀǰֻʵ���˵���ģʽ
enum class VALIGN : int
{
    top,
    bottom,
    center,
};

enum class HALIGN : int
{
    left,
    right,         // ÿһ�����ݶ��Ҷ���
    center,        // ÿһ�����ݶ����ж���
    paraRight,     // ���������Ҷ��룬�����е��л��������
    paraCenter,    // ����������ж��룬�����е��л��������
};

interface IParagraph
{
    virtual void  SetAlign(RT::HALIGN align) = 0;
    virtual void  SetLeftIndent(int indent) = 0;
    virtual void  SetRightIndent(int indent) = 0;
    virtual void  SetTopSpace(int space) = 0;
    virtual void  SetBottomSpace(int space) = 0;
    virtual void  SetCharformat(CharFormat*) = 0;
    virtual void  ShowBubbleBackgournd(bool b) = 0;
};

interface IChatMessage
{
    virtual void  SetAlign(RT::HALIGN align) = 0;
    virtual HANDLE  EnterBubble() = 0;
    virtual void  LeaveBubble(HANDLE) = 0;
};

// interface INode
// {
//     virtual IParagraph*  GetParagraphNode() = 0;
// };

// interface ISelection
// {
//     virtual INode*  GetAnchorNode() = 0;
//     virtual INode*  GetFocusNode() = 0;
// };


interface ILink
{
    virtual LPCTSTR  GetId() = 0;
    virtual void  SetId(LPCTSTR) = 0;

    virtual LPCTSTR  GetToolTip() = 0;
    virtual void  SetToolTip(LPCTSTR) = 0;

    virtual LPCTSTR  GetReference() = 0;
    virtual void  SetReference(LPCTSTR) = 0;
};
}


#ifdef ReplaceText
#undef ReplaceText
#endif

class RichText;
interface UICTRL_API_UUID(F1B67364-BA72-44CD-BA63-53DA6B23CB4A) IRichText : public IControl
{
    void  Clear();
    void  SetText(LPCTSTR szText);
    void  GetDefaultCharFormat(CharFormat* pcf);
	void  GetDefaultLinkCharFormat(CharFormat* pcf);
    
	void  LoadHtml(LPCTSTR szHtml);

    void  ReplaceText(LPCTSTR szText);
    void  ReplaceTextEx(LPCTSTR szText, CharFormat* pcf);
    void  AppendImage(LPCTSTR szPath);
    RT::ILink*  ReplaceLink(LPCTSTR);
    RT::ILink*  ReplaceLinkEx(LPCTSTR, CharFormat* pcf);
    RT::IChatMessage*  BeginInsertMessage();
    void  EndInsertMessage(RT::IChatMessage*);

    void  SetPageMode(PageMode e);
    PageMode  GetPageMode();

    //RT::ISelection*  GetSelection();
    RT::IParagraph*  GetSelectionAnchorParagraph();

    void  SetStyle_PageContentVAlign(RT::VALIGN);
    void  SetStyle_PageContentHAlign(RT::HALIGN);
    void  SetStyle_DrawLinkUnderline(bool b);

    signal<IRichText*, RT::ILink*>&  LickClickedEvent();
	UI_DECLARE_INTERFACE_ACROSSMODULE(RichText);
};


namespace RT
{
	class LinkUnit;

    // �༭ģʽ
    enum class EDITABLE
    {
        Disable,   // ��֧�ֱ༭������ʾ��꣬��֧�ָ���
        Readonly,  // ��ʾ��֧꣬�ָ��ƣ�δʵ�֣�
        Enable,    // ֧�ֱ༭��δʵ�֣�
    };
}

}

#endif // _UI_IRICHTEXT_H_