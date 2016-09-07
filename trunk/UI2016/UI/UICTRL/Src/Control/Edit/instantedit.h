#pragma once
#include "edit.h"
#include "..\UISDK\Inc\Interface\iuiinterface.h"

/*
    ��ʱ�༭��
    
    ����ͨedit������
    1. hoverʱ����������������û����һ��
    2. �Ǳ༭״̬�£��ı��ɲ���edit���ƣ�����instantedit���ƣ���֧����ʾ...
    3. �༭״̬�£��س�����ʧ���ᴥ��acccept notify
    4. �༭״̬�£�Esc���ᴥ��cancel notify

    ʹ�ó�����
    1. �б�ؼ����б��������޸�
    2. im����е���������
*/

namespace UI
{

class InstantEdit :  public Edit, public IPreTranslateMessage
{
public:    
    InstantEdit(IInstantEdit* p);
    ~InstantEdit();

	UI_BEGIN_MSG_MAP()
        UIMSG_PAINT(OnPaint)
        MSG_WM_KEYDOWN(OnKeyDown)
        UIMSG_HANDLER_EX(WM_LBUTTONDOWN, OnLButtonDown)
		UIMSG_HANDLER_EX(WM_LBUTTONDBLCLK, OnLButtonDown)
		UIMSG_HANDLER_EX(WM_RBUTTONDOWN, OnRButtonDown)
		UIMSG_HANDLER_EX(WM_SETCURSOR, OnSetCursor)
        MSG_WM_KILLFOCUS(OnKillFocus)
        MSG_WM_SETFOCUS(OnSetFocus)
		UIMSG_SERIALIZE(OnSerialize)
    UI_END_MSG_MAP_CHAIN_PARENT(Edit)


private:
    LRESULT  OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT  OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT  OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void  OnSetFocus(HWND wndFocus);
    void  OnKillFocus(HWND wndFocus);
    void  OnPaint(IRenderTarget* pRenderTarget);
    void  OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void  OnSerialize(SERIALIZEDATA* pData);

	virtual BOOL PreTranslateMessage(MSG* pMsg) override;
private:
	bool  beginEdit();
	void  cancelEdit();
	void  acceptEdit();
	void  endEdit();

private:
    IInstantEdit*  m_pIInstantEdit;
	Color*   m_pColorQuiet; // δ�༭ʱ����ɫ

    bool  m_bEditing;
	String  m_strTextBeforeEdit;
};


}