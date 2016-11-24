#pragma once
#include "Inc\Interface\ibutton.h"

#define BUTTON_VK_SPACE_MOUSECAPTURE_NOTIFY_ID  138162231
namespace UI
{

//
//    ��ť�ؼ��ĳ�����࣬��CONTROL�Ļ������ٷ�װ�˰�״̬�����麯����CLICK�¼���Ӧ
//
class ButtonBase : public MessageProxy
{
public:
	ButtonBase(IButtonBase* p);

	UI_BEGIN_MSG_MAP()
		UIMSG_STATECHANGED(OnStateChanged2)
		UIMSG_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_RBUTTONUP(OnRButtonUp)
		MSG_WM_KEYDOWN(OnKeyDown)
		MSG_WM_KEYUP(OnKeyUp)
		UIMSG_KILLFOCUS(OnKillFocus)
		UIMSG_SETFOCUS(OnSetFocus)
    UIALT_MSG_MAP( BUTTON_VK_SPACE_MOUSECAPTURE_NOTIFY_ID )
		UIMSG_HANDLER_EX(WM_ENTERSIZEMOVE, OnEnterSizeMove)
		else
        {
            return TRUE;
        }
	UI_END_MSG_MAP_CHAIN_PARENT_Ixxx(ButtonBase, IControl)   // �ɵ�ǰ��Ϣ����ཫ��Ϣ���ݸ�IControl����


	// ��Ϣ����
protected:
	void  OnStateChanged2(UINT nMask);
	void  OnEraseBkgnd(IRenderTarget*);
// 	void  OnMouseMove(UINT nFlags, POINT point);
// 	void  OnMouseLeave();
 	void  OnLButtonDown(UINT nFlags, POINT point);
	void  OnLButtonUp(UINT nFlags, POINT point);
	void  OnRButtonUp(UINT nFlags, POINT point);
	void  OnLButtonDblClk(UINT nFlags, POINT point);
	void  OnSetFocus(IObject*);
	void  OnKillFocus(IObject*);
	void  OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags );
	void  OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags );
	LRESULT  OnEnterSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void  DrawFocus(IRenderTarget*);
	void  OnClicked(POINT* pt);         // �Լ�������ˣ������������������õ�ǰcheck״̬��

protected:
	virtual void virtualOnClicked() {}

public:
	// �ⲿ�ӿ�
	void  SetCheck(int nCheckState);
    void  SetCheck(bool bCheck);
	void  SetChecked();
	void  SetUnChecked();
	int   GetCheck();
	bool  IsChecked();
	BUTTON_TYPE  GetButtonType();
	void  SetButtonType(BUTTON_TYPE n);
	void  ModifyButtonStyle(BUTTONSTYLE* add, BUTTONSTYLE* remove);
	bool  TestButtonStyle(const BUTTONSTYLE& test);

protected:
    IButtonBase* m_pIButtonBase;
	BUTTON_TYPE  m_eButtonType;
	int   m_nCheckState;       // BST_UNCHECKED, BST_CHECKED
	bool  m_bSpaceKeyDown;     // �Ƿ����˿ո��

	BUTTONSTYLE  m_buttonStyle;
};


class Button : public ButtonBase
{
public:
    Button(IButton* p);
    ~Button();

	UI_BEGIN_MSG_MAP()
        UIMSG_PAINT(OnPaint)
//         UIMSG_WM_NOTIFY(UI_BN_DRAWFOCUS, DrawFocus)
//         UIMSG_WM_GETDLGCODE(OnGetDlgCode)
        UIMSG_GETDESIREDSIZE(GetDesiredSize)
        UIMSG_QUERYINTERFACE(Button)
        UIMSG_INITIALIZE(OnInitialize)
        UIMSG_SERIALIZE(OnSerialize)
		UIMSG_HANDLER_EX(WM_SETCURSOR, OnSetCursor)
        UIMSG_VISIBLE_CHANGED(OnVisibleChanged)
        UIMSG_HANDLER_EX(UI_MSG_DEFAULTBUTTON_VKRETURN_EVENT, OnDefaultEvent)
	UIALT_MSG_MAP(UIALT_CALLLESS)
		UIMSG_CREATEBYEDITOR(OnCreateByEditor)
    UI_END_MSG_MAP_CHAIN_PARENT(ButtonBase)

public:
    IButton*  GetIButton() { return m_pIButton; }

    void  SetTextAndUpdate(LPCTSTR  szText);
    void  SetText(LPCTSTR  szText);
    LPCTSTR  GetText();

	void  Click();

    void  SetIconFromFile(LPCTSTR szIconPath);
    void  SetIconFromHBITMAP(HBITMAP hBitmap);
    void  SetIconFromImageId(LPCTSTR szImageId);

    void  SetDrawFocusType(BUTTON_RENDER_DRAW_FOCUS_TYPE eType);
    void  SetAutoSizeType(BUTTON_AUTO_SIZE_TYPE eType);
    void  SetDefaultPushButtonStyle(bool);
    bool  HasDefaultPushButtonStyle();
	void  SetHandCursorStyle(bool);
	bool  HasHandCursorStyle();

protected:
    void  OnSerialize(SERIALIZEDATA*);
	void  OnCreateByEditor(CREATEBYEDITORDATA*);
    void  OnInitialize();

	void  GetDesiredSize(SIZE* pSize);
    UINT  OnGetDlgCode(LPMSG lpMsg);
    void  OnPaint(IRenderTarget*);
    void  OnVisibleChanged(BOOL bVisible, IObject* pObjChanged);
    LRESULT  DrawFocus(WPARAM, LPARAM);
	LRESULT  OnSetCursor(UINT, WPARAM, LPARAM);
    LRESULT  OnDefaultEvent(UINT, WPARAM, LPARAM);

	virtual void virtualOnClicked() override;

private:
    void  calc_icontext_rect(IRenderTarget*, CRect& rcIcon, CRect& rcText);
    void  set_as_window_default_button();

protected:
    IButton*  m_pIButton;
    String    m_strText;

    BUTTON_AUTO_SIZE_TYPE  m_eAutoSizeType;             // ��ť�Զ�������С����
    ALIGN_TYPE  m_eIconAlignFlag;                       // ͼ�����ı���λ�ù�ϵ
    BUTTON_RENDER_DRAW_FOCUS_TYPE  m_eDrawFocusType;    // FOCUS����
    long  m_lIconMarginText;                            // ͼ������ı��Ĵ�С. ps: icon���밴ť��Ե�ľ����ͨ��padding��ʵ��

public:
    // �¼�
    signal<IButton*>  clicked;
};

} 

