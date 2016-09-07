#pragma once
#include "Inc\Interface\iedit.h"
#include "Src\Control\Caret\caret.h"
#include "Inc\Interface\irichedit.h"

#include <usp10.h>
#pragma comment(lib, "usp10.lib")

#ifndef _UNICODE 
	#error "usp10 requirs a unicode build."
#endif

#pragma comment(lib, "imm32.lib")

namespace UI
{
class Edit;

//
//	��װ�༭�������
//
class EditData
{
public:
	EditData();
	~EditData();

public:
	void  BindToEdit(Edit* pEdit);

	void  SetText(LPCTSTR, bool bFilter, bool& bUpdate);
	void  ReplaceChar(const TCHAR& c, bool& bUpdate);
	void  ReplaceString(const String& str, bool& bUpdate);
	void  Delete(bool& bUpdate);
	void  BackSpace(bool& bUpdate);
    void  DeleteSelectionText(bool& bUpdate);
	void  SetCaret(int nCaret, bool bSetSelStart, bool& bUpdate);

	void  CutToClipboard();
	void  CopyToClipboard();
	void  PasteFromClipboard();
	void  GetPriorItemPos(int nCP, int* pPrior);
	void  GetNextItemPos(int nCP, int* pNext);

	void  SetMaxChar(long nMax);
    long  GetMaxChar();
	void  SetMaxChar2(int nMax, bool& bNeedUpdate);
	void  SetInsertMode(bool bInsertOrOverride);
	bool  GetInsertMode() { return m_bInsertMode; }

	void  GetText(String& str) { str = m_strText; }
	const String&  GetTextRef() { return m_strText; }
	LPCTSTR  GetText() { return m_strText.c_str(); }
	int   GetTextLength() { return (int)m_strText.length(); }
	int   GetCaretIndex() { return m_nCaret; }
	int   GetSelectionLength() { return abs(m_nCaret - m_nSelStart); }
	int   GetTextWidth()  { return m_nTextWidth; }
	void  GetSelectionInfo(int& nLeft, int& nRight) const;
	void  SetSelectionInfo(int nStart, int nEnd, bool& bUpdate);
	void  GetSelectionText(String& str);
	bool  IsSelectionExist();
    bool  Clear();

	bool  CP2X(int nCP, int* pX);
	bool  X2CP(int nX, int* pnCP, int* pbTrailOrLead);
	void  DestroyStringAnalysis();

	void  SetTextFilterDigit();
	void  ClearTextFilter();

protected:
	void  DeleteSelectionText();   // �ú����������ڲ�����,�������ݽ��д�����ɾ���ı�

	void  Fire_Text_Changed(bool bInvokeSetText);
	TextFilterResult  FilterString(LPCTSTR szSrc, String& strDest);
	TextFilterResult  FilterChar(const TCHAR& cSrc, TCHAR& cDest);

	bool  StringAnalysis();

private:
	String  m_strText;              // �༭�������
	long  m_nMaxChar;             // �������������ַ�����-1��ʾ������

	int   m_nSelStart;            // ѡ����ַ���㣬��û��ѡ��ʱ	m_nSelStart==m_nCaret	
	int   m_nCaret;               // ��ǰ����λ�ã�Ҳ��־��ѡ����End pos

	bool  m_bInsertMode;          // ����/��дģʽ

	// UniScribe��ر���
	SCRIPT_CONTROL	m_ScriptControl;		// For uniscribe
	SCRIPT_STATE	m_ScriptState;			// For uniscribe

	SCRIPT_STRING_ANALYSIS	m_Analysis;     // For cp2x, x2cp...
	int   m_nTextWidth;           // ��ǰ�ַ����ܳ���

	ITextFilter*  m_pTextFilter; // ���ֹ��ˡ�����ʵ��������С�������

	Edit*  m_pEdit;
};

class Edit : public MessageProxy
{
public:
	Edit(IEdit* p);
	~Edit();

	UI_BEGIN_MSG_MAP()
 		UIMSG_PAINT(OnPaint)
		UIMSG_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_SETCURSOR(OnSetCursor)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_CHAR(OnChar)
		MSG_WM_KEYDOWN(OnKeyDown)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		MSG_WM_RBUTTONUP(OnRButtonUp)
		UIMSG_SETFOCUS(OnSetFocus)
		UIMSG_KILLFOCUS(OnKillFocus)
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
		UIMSG_VISIBLE_CHANGED(OnVisibleChanged)
		MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_GETDLGCODE(OnGetDlgCode)
        UIMSG_HANDLER_EX(WM_IME_STARTCOMPOSITION, OnStartComposition)
		UIMSG_STATECHANGED(OnStateChanged)
		UIMSG_CREATE_ACCESSIBLE(OnCreateAccessible)
        UIMSG_INITIALIZE(OnInitialize)
        UIMSG_GETDESIREDSIZE(GetDesiredSize)
        UIMSG_QUERYINTERFACE(Edit)
        UIMSG_SERIALIZE(OnSerialize)
//     UIALT_MSG_MAP(UIALT_CALLLESS)
//         UIMSG_WM_CREATEBYEDITOR(OnCreateByEditor)
	UI_END_MSG_MAP_CHAIN_PARENT_Ixxx(Edit, IControl)

    IEdit*  GetIEdit() { return m_pIEdit; }

	// ��Ϣ����
protected:
    void  OnInitialize();
	void  OnPaint(IRenderTarget* hDC);
	void  OnEraseBkgnd(IRenderTarget* pRenderTarget);
	BOOL  OnSetCursor(HWND hWnd, UINT nHitTest, UINT message);
	void  OnSetFocus(IObject*);
	void  OnKillFocus(IObject*);
	void  OnWindowPosChanged(LPWINDOWPOS);
	void  OnShowWindow(BOOL bShow, UINT nStatus);
    UINT  OnGetDlgCode(LPMSG lpMsg);
	void  OnVisibleChanged(BOOL bVisible, IObject* pParent);
    LRESULT  OnStartComposition(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void  OnStateChanged(UINT nMask);
	void  OnCreateAccessible(IUIAccessibleCreator*);

	void  OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void  OnLButtonDown(UINT nFlags, POINT point);
	void  OnLButtonUp(UINT nFlags, POINT point);
	void  OnMouseMove(UINT nFlags, POINT point);
	void  OnLButtonDblClk(UINT nFlags, POINT point);
	void  OnRButtonDown(UINT nFlags, CPoint point);
    void  OnRButtonUp(UINT nFlags, CPoint point);

	void  OnInputChar(UINT nChar);
	void  OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void  OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void  OnKeyDown_Ctrl_A();
	void  OnKeyDown_Ctrl_X();
	void  OnKeyDown_Ctrl_C();
	void  OnKeyDown_Ctrl_V();
	void  OnKeyDown_Ctrl_Z();
	void  OnKeyDown_Ctrl_Y();
	void  OnKeyDown_Backspace(bool bCtrlDown);
	void  OnKeyDown_Delete(bool bCtrlDown);
	void  OnKeyDown_Left_Top(bool bCtrlDown);
	void  OnKeyDown_Right_Down(bool bCtrlDown);
	void  OnKeyDown_Home(bool bCtrlDown);
	void  OnKeyDown_End(bool bCtrlDown);
	void  OnKeyDown_Insert(bool bCtrlDown);
    void  OnKeyDown_Return();
    void  OnKeydown_Esc();
    void  OnKeydown_Tab();
   
    void  GetDesiredSize(SIZE* pSize); 
    void  OnSerialize(SERIALIZEDATA* pData);
//    void  OnCreateByEditor(CREATEBYEDITORDATA* pData);

	virtual void virtualShowContentMenu(CPoint point);

public:
	int   GetTextLength();
    LPCTSTR  GetText();
	void  SetText(LPCTSTR szText);
	void  SetTextNoFilter(LPCTSTR szText);
	int   GetEditType();
	void  SetEditType(int n);
	long  GetMaxChar();

    void  DeleteSelectionText(bool bUpdate);
    bool  DoCut();
    bool  DoCopy();
    bool  DoPaste();
	void  SetSel(int nStartChar, int nEndChar);
	void  SetSel2(int nStartChar, int nSelLength);
	void  GetSel(int& nStartChar,int& nEndChar) const;
	void  GetSel2(int& nStartChar, int& nSelLength) const;
    void  Clear();

    bool  IsReadOnly();
    void  SetReadOnly(bool b);
	bool  IsRealFocus();
	
	void  SetTextFilterDigit();
	void  ClearTextFilter();

protected:
	void  EnterFocus();
	void  ExitFocus();
    long  GetTextHeight();
	
private:
	void  DrawNormal(IRenderTarget* pRenderTarget);
	void  DrawFocus(IRenderTarget* pRenderTarget);

	void  CalcCaretPos(int nCaretIndex, bool& bUpdate);
	void  UpdateCaretByPos();
    long  GetCaretHeight();

	void  setText(LPCTSTR szText, bool bFilter, bool bUpdate);
    void  loadThemeEditBkgRender();

//     void  LoadColor(LPCTSTR szColor) 
//     {
//         m_pIEdit->LoadColor(szColor, m_pColor); 
//     }
//     LPCTSTR  SaveColor() 
//     {
//         return m_pIEdit->SaveColor(m_pColor);
//     }
//     void  LoadSelectColor(LPCTSTR szColor)
//     {
//         m_pIEdit->LoadColor(szColor, m_pColorSelect); 
//     }
//     LPCTSTR  SaveSelectColor() 
//     {
//         return m_pIEdit->SaveColor(m_pColorSelect);
//     }
//     void  LoadSelectBkgColor(LPCTSTR szColor)
//     {
//         m_pIEdit->LoadColor(szColor, m_pColorSelectBk); 
//     }
//     LPCTSTR  SaveSelectBkgColor()
//     {
//         return m_pIEdit->SaveColor(m_pColorSelectBk);
//     }

	// ����
protected:
	EditData  m_editData;
    IEdit*  m_pIEdit;

	// ����
private:
    // �ؼ��н��㲻����edit���ܱ༭������instantedit��
	bool  m_bRealFocus;             

    // �Ƿ��������������϶�ѡ��
	bool  m_bMouseDrag;			

    // �Ƿ���Ҫ���´�ˢ�µ�ʱ����¹��λ��
	bool  m_bNeedUpdateCaretPos;    

	long  m_nXScroll;	            // �ַ������ĳ���
	long  m_nXCaretPos;				// ����λ�ã�����padding.left

    // ���߶Ȳ�Ӱ���ı�����λ�ã���Ӱ��ѡ�еı����߶�
    // ���־��л��ƣ�m_nTextHeightӰ���ı�����λ��
    long  m_nCaretHeight;			// ���ĸ߶�
    long  m_nTextHeight;            // �����Ӧ���ı��߶ȣ���'1'������

	friend class EditData;

	// ����
protected:
	Color*   m_pColor;
	Color*   m_pColorSelect;
	Color*   m_pColorSelectBk;
	CCaret   m_caret;     // ����ʵ�ֲַ㴰���ϵĹ����ʾ

	long  m_lEditType;   // edit�ؼ����ͣ�����Ƕ����combo�е�edit

	EDITSTYLE  m_editStyle;
};
}
