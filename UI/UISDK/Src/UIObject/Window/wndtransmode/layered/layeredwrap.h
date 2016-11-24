#pragma once
#include "..\wndtransmodebase.h"
namespace UI
{

//
//	�ֲ㴰��ʵ�ִ���
//
class LayeredWindowWrap : public WndTransModeBase
{
public:
    LayeredWindowWrap();
    ~LayeredWindowWrap();

    VIRTUAL_BEGIN_MSG_MAP(LayeredWindowWrap)
		MESSAGE_HANDLER(WM_SIZE, _OnSize)
        MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, _OnWindowPosChanged)
        MESSAGE_HANDLER(WM_CANCELMODE, _OnCancelMode)
        MESSAGE_HANDLER(WM_NCHITTEST, _OnNcHitTest)
        MESSAGE_HANDLER(WM_SETCURSOR, _OnSetCursor)
    END_MSG_MAP()


	UI_BEGIN_MSG_MAP()
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_LBUTTONUP(OnLButtonUp)
        MSG_WM_MOUSEMOVE(OnMouseMove)
        UIMSG_QUERYINTERFACE(LayeredWindowWrap)
	UI_END_MSG_MAP()

    LRESULT  _OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT  _OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT  _OnCancelMode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT  _OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT  _OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    void  OnLButtonDown(UINT nFlags, POINT point);
    void  OnLButtonUp(UINT nFlags, POINT point);
    void  OnMouseMove(UINT nFlags, POINT point);

    ILayeredWindowWrap*  GetILayeredWindowWrap();
    byte  GetAlpha();
    void  SetAlpha(byte b, bool bUpdate);

    virtual WINDOW_TRANSPARENT_TYPE  GetType()
    {
        return WINDOW_TRANSPARENT_TYPE_LAYERED;
    }
    virtual bool  RequireAlphaChannel()
    {
        return true;
    }
    virtual void  Init(ICustomWindow* pWnd)
    {
        __super::Init(pWnd);
        GetWindowPos();
    }
    virtual void  UpdateRgn(); 
    bool  UpdateLayeredCaptionWindowRgn();
    virtual bool  Commit();
    virtual void  Enable(bool b);

    void   GetWindowPos();
    void   OnWindowPosChanged(LPWINDOWPOS lpWndPos);

    void   OnLButtonDown(UINT nHitTest);
    void   OnEnterSizeMove(UINT nHitTest);
    void   OnExitSizeMove();

protected:
    bool   IsMinimized();

protected:
    friend    class CustomWindow;
    ILayeredWindowWrap*  m_pILayeredWindowWrap;

    // TODO: ���ֲ㴰�ڴ�THICKFRAME��ʽʱ����󻯺󴰿�������ͻ�����һ�£�
    //       �� m_ptWindowӦ�ñ�ʾ�������Ͻǣ����ǿͻ��������Ͻǣ�
    //      (���ʱ�ᵼ�´�������ƫ�ƺü���)

    POINT     m_ptWindow;          // �ֲ㴰�ڵ�����
    SIZE      m_sizeWindow;        // �ֲ㴰�ڵĴ�С(�ͻ�����С�����Ǵ��ڴ�С����buffer�Ĵ�С����һ�£�������Ⱦʧ��)

    // �ֲ㴰������ʱʹ�õ��м����
    UINT      m_nHitTestFlag;      // �����ʶ
    POINT     m_ptStartSizeMove;   // ��ʼ����ʱ������λ�ã����ڼ������ƫ��
    POINT     m_ptWindowOld;       // ��ʼ����ʱ�Ĵ������꣬���ں�ƫ��һ�������λ��
    SIZE      m_sizeWindowOld;     // ��ʼ����ʱ�Ĵ��ڴ�С�����ں�ƫ��һ������´�С
};

}