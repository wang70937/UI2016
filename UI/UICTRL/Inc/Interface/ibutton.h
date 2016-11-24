#ifndef _UI_CTRL_IBUTTON_H_
#define _UI_CTRL_IBUTTON_H_

#include "..\..\..\UISDK\Inc\Interface\icontrol.h"

namespace UI
{
	struct BUTTONSTYLE
	{
		bool  default_push_button : 1;   // ���Գ�ΪĬ�ϰ�ť
		bool  click_on_pushdown : 1;     // ����갴��ʱ�ʹ���click����������굯��ʱ����
		bool  notify_hoverleave : 1;     // ��������Ƴ�ʱ������֪ͨ
		bool  hand_cursor : 1;           // ʹ���������
	};

	enum BUTTON_TYPE
	{
		// button style
		BUTTON_TYPE_HAVE_TEXT_FIRST = 0x00,      // ��־�����ݵİ�ť����Щ��ť����Ҫ��������textrender
		BUTTON_TYPE_PUSHBUTTON = 0x00,           // ��ͨ��ť
		BUTTON_TYPE_RADIOBUTTON = 0x01,          // ��ѡ��ť
		BUTTON_TYPE_CHECKBUTTON = 0x02,          // ��ѡ��ť
		BUTTON_TYPE_HYPERLINK = 0x03,            // �����Ӱ�ť
		BUTTON_TYPE_MENUBUTTON = 0x04,           // �˵���ť
		BUTTON_TYPE_SPLITMENUBUTTON = 0x05,      // ��ť+�˵�
		BUTTON_TYPE_TABCTRLBUTTON = 0x06,        // TAB�ؼ��ϵ�ӭ����
		BUTTON_TYPE_HAVE_TEXT_LAST = 0x80,


		BUTTON_TYPE_COMBOBOX = 0x81,             // ��Ͽ��еİ�ť
		BUTTON_TYPE_SCROLLLINEUP = 0x82,         // ��������ť
		BUTTON_TYPE_SCROLLLINEDOWN = 0x83,
		BUTTON_TYPE_SCROLLLINELEFT = 0x84,
		BUTTON_TYPE_SCROLLLINERIGHT = 0x85,
		BUTTON_TYPE_HSCROLLTHUMB = 0x86,         // ��������ק��ť
		BUTTON_TYPE_VSCROLLTHUMB = 0x87,         // ��������ק��ť
		BUTOTN_TYPE_SLIDERTRACKBUTTON = 0x88,    // ����������İ�ť

        // ϵͳ����
        BUTTON_TYPE_MINIMIZE = 0x100,
        BUTTON_TYPE_MAXIMIZE = 0x101,
        BUTTON_TYPE_RESTORE = 0x102,
        BUTTON_TYPE_CLOSE = 0x103,
	};

class ButtonBase;
struct UICTRL_API IButtonBase : public IControl
{
    void  SetCheck(int nCheckState);
    void  SetCheck(bool bCheck);
    void  SetChecked();
    void  SetUnChecked();
    int   GetCheck();
    bool  IsChecked();
	void  SetButtonType(BUTTON_TYPE n);

	UI_DECLARE_INTERFACE_ACROSSMODULE(ButtonBase)
};

class Button;
interface UICTRL_API_UUID(DEBF57B2-C7C4-480B-A90D-427891E77502) IButton : public IButtonBase
{
    void  SetText(LPCTSTR  szText);
    LPCTSTR GetText();
    
	void  Click();

    void  SetIconFromFile(LPCTSTR szIconPath);
    void  SetIconFromHBITMAP(HBITMAP hBitmap);   // ע����HBITMAP������ڲ�����

    void  SetDrawFocusType(BUTTON_RENDER_DRAW_FOCUS_TYPE eType);
    void  SetAutoSizeType(BUTTON_AUTO_SIZE_TYPE eType);
	
    signal<IButton*>&  ClickedEvent();

	UI_DECLARE_INTERFACE(Button);
};

//
//	��ť����¼�
//
//		message:UI_WM_NOTIFY
//		code:   UI_BN_CLICKED
//		wparam: NA // -- POINT*������window
//		lParam: 
//
#define UI_BN_CLICKED  139222333

// BN_HILITE    
#define UI_BN_HOVER  152811331

// BN_UNHILITE
#define UI_BN_LEAVE  152811332  

}

#endif // IBUTTON_H_787AE538_A2A3_4f45_A366_7AD02BC2EF9A