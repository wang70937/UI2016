#ifndef _UI_IEDIT_H_
#define _UI_IEDIT_H_
#include "..\..\..\UISDK\Inc\Interface\icontrol.h"

namespace UI
{
const UINT  EDIT_TEXTREDNER_STATE_NORMAL    = TEXTRENDER_STATE_NORMAL|0;
const UINT  EDIT_TEXTRENDER_STATE_SELECTED  = TEXTRENDER_STATE_SELECTED|1;

const UINT  EDIT_BKGND_RENDER_STATE_NORMAL  = RENDER_STATE_NORMAL|0;
const UINT  EDIT_BKGND_RENDER_STATE_HOVER   = RENDER_STATE_HOVER|1;
const UINT  EDIT_BKGND_RENDER_STATE_PRESS   = RENDER_STATE_PRESS|2;
const UINT  EDIT_BKGND_RENDER_STATE_DISABLE = RENDER_STATE_DISABLE|3;

// �ڱ༭���а�����ĳ����
// wParam: RETURN/ESC/TAB
// return: 1�Ѵ���0δ����
#define UI_EN_KEYDOWN  136181700

// �ڱ༭���е��ı������˱仯
// message: UI_WM_NOTIFY
// code:    UI_EN_CHANGE
// wParam:  0��ʾ���û����봥����1��ʾ�ɵ���api(settext��)���� bInvokeBySetText
#define UI_EN_CHANGE   136221429


struct EDITSTYLE
{
	bool  combobox : 1;       // ��Ͽ���ʹ�õı༭��
	bool  want_tab : 1;       // ����TAB����Ϣ
	bool  read_only : 1;      // ֻ��

};

class Edit;
interface __declspec(uuid("9EFA8C1A-21A6-415A-9C6C-C759ACAD2966"))
UICTRLAPI IEdit : public IControl
{
    void  SetText(LPCTSTR szText);
    void  SetTextLong(long lNumber);
	void  SetTextNoFilter(LPCTSTR szText);
    LPCTSTR  GetText();
    void  Clear();

    void  SetSel(int nStartChar, int nEndChar);
    void  GetSel(int& nStartChar,int& nEndChar) const;

    bool  IsReadOnly();
    void  SetReadOnly(bool b);
	
	void  SetTextFilterDigit();
	void  ClearTextFilter();

	UI_DECLARE_INTERFACE_ACROSSMODULE(Edit);
};

enum ACCEPT_EDIT_RESULT
{
	ACCEPT_EDIT_ACCEPT,  // ����
	ACCEPT_EDIT_REJECT,  // ������
	ACCEPT_EDIT_REJECT_AND_CONTINUE_EDIT,  // �����ܣ������༭
};

enum START_EDIT_RESULT
{
	START_EDIT_OK = 0,     // ����
	START_EDIT_CANCEL = 1, // ȡ����������༭����
};

// return: START_EDIT_RESULT
#define UI_EN_STARTEDIT  148042202
// return: ACCEPT_EDIT_RESULT
#define UI_EN_ACCEPTEDIT 148042203
#define UI_EN_CANCELEDIT 148042204
#define UI_EN_ENDEDIT    148042205

class InstantEdit;
interface __declspec(uuid("227EB0DD-D4EC-4A3A-83ED-25FEA28E123E"))
UICTRLAPI IInstantEdit : public IEdit
{
    UI_DECLARE_INTERFACE(InstantEdit);
};


// ��ǰ��Сд�����䷢��ֻ���н���ʱ֪ͨ��
// wParam: IPasswordEdit
// lParam: 0/1 �Ƿ�����
// return: 1��ʾ�Ѵ���0��ʾδ����
#define UI_PASSWORDEDIT_NM_CAPSLOCK_CHANGED  151081409

// ��������ͳ��
#define UI_PASSWORDEDIT_NM_INPUT_STATS  167200939





// ������������࣬������
// �����ṩ��ȡԭʼ����Ľӿڣ��ⲿֻ��ͨ������Ϣ���
class PasswordEdit;
interface __declspec(uuid("5A7915E7-5F89-4660-A50C-CF9ED142AC4A"))
IPasswordEdit : public IEdit
{
	UI_DECLARE_INTERFACE(PasswordEdit);
};

// wParam: BSTR*
// lParam: bool* 
#define UI_PASSWORDEDIT_MSG_GETREALPASSWORD  151081651
inline void PasswordEdit_GetRealPassword(IMessage* pPasswordCtrl, void/*BSTR*/* pOut)
{
	UIASSERT(pPasswordCtrl && pOut);
	UISendMessage(pPasswordCtrl, UI_PASSWORDEDIT_MSG_GETREALPASSWORD, (WPARAM)pOut);
}

// wParam: LPCTSTR
#define UI_PASSWORDEDIT_MSG_SETREALPASSWORD  164152027
inline void PasswordEdit_SetRealPassword(IMessage* pPasswordCtrl, LPCTSTR pText)
{
	UIASSERT(pPasswordCtrl && pText);
	UISendMessage(pPasswordCtrl, UI_PASSWORDEDIT_MSG_SETREALPASSWORD, (WPARAM)pText);
}

// ԭ�ġ����� ģʽ�л�
// wParam: PASSWORDEDIT_MODE
enum PASSWORDEDIT_MODE
{
	PASSWORDEDIT_MODE_PASSWORD,
	PASSWORDEDIT_MODE_PLAIN,
};
#define UI_PASSWORDEDIT_MSG_SWITCHMODE  169061750
inline void PasswordEdit_SwitchMode(IMessage* pPasswordCtrl, PASSWORDEDIT_MODE eMode)
{
	UIASSERT(pPasswordCtrl);
	UISendMessage(pPasswordCtrl, UI_PASSWORDEDIT_MSG_SWITCHMODE, (WPARAM)eMode);
}

}

#endif // _UI_IEDIT_H_