
// MFCDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "UI\UISDK\Inc\Interface\iwindow.h"


// CMFCDemoDlg �Ի���
class CMFCDemoDlg : public CDialogEx, public UI::IWindowMessageCallback
{
// ����
public:
	CMFCDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��


// �Ի�������
	enum { IDD = IDD_MFCDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

    virtual BOOL  OnWindowMessage(UINT msg, WPARAM, LPARAM, LRESULT& lResult)
    {
        if (msg == WM_DESTROY)
        {
            m_pUIWindow->Detach();
        }
        return FALSE;
    };
    virtual BOOL  OnWindowUIMessage(UI::UIMSG* pMsg) {
        return FALSE;
    }

    void OnNcDestroy()
    {
        __super::OnNcDestroy();
        if (m_pUIWindow)
        {
            m_pUIWindow->Release();
            m_pUIWindow = nullptr;
        }
    }

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    UI::IWindow* m_pUIWindow = nullptr;
};
