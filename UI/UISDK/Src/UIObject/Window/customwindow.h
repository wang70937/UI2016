#pragma once
#include "Inc\Interface\icustomwindow.h"
#include "Src\UIObject\Window\window.h"
#include "Inc\Interface\iwndtransmode.h"

/*
 �ǿͻ����ػ��һЩ������

. WM_NCACTIVATE:
  ��������������Ϣ��aero transparent���ڼ���ʱ���Ὣ��Ӱ�޸�Ϊ����Ӱ
  ����ſ������Ϣ���ھ����������棬����ʧ����ˢ��������

. WM_NCCALCSIZE
  �����޸Ŀͻ���λ�á�ֱ��return TRUE;�������ε��ǿͻ���

. WM_NCHITTEST
  �����WM_NCHITTEST�������HTCLOSE,HTMAXBUTTON,HTMINBUTTON��ϵͳ��ǿ��ˢ����������ť����Ҫ���ε�

. WM_NCPAINT
  ������θ���Ϣ����aero trans��������ʾ��Ӱ�ˡ�
  ����ſ������Ϣ���ھ����������洰�ڵõ�����ʱ��ˢ����������Ŀǰֻ��antiģʽ�·����и����⣬mask color/mask alpha���д���֤��

. ����ģʽ�����WM_SETTEXT,WM_SETICON (WM_NCLBUTTONDOWN??)
  ���ø�api��ϵͳ��ǿ��ˢһ�±���������ν��?
  http://blog.sina.com.cn/s/blog_437fd95e01013lrk.html
  Chrome: ui\views\widget\native_widget_win.cc  void NativeWidgetWin::LockUpdates();
  ������WS_VISIBLE����ȥ����Ȼ���ټӻ���

  ���⻹�У�WM_NCLBUTTONDOWN��EnableMenuItem(called from our WM_INITMENU handler)

. ����û���ص���Ϣ�� ��Ŀǰ���Ի�û�������ǵ�����֮����
  WM_NCUAHDRAWCAPTION
  WM_NCUAHDRAWFRAME

*/
// These two messages aren't defined in winuser.h, but they are sent to windows  
// with captions. They appear to paint the window caption and frame.  
// Unfortunately if you override the standard non-client rendering as we do  
// with CustomFrameWindow, sometimes Windows (not deterministically  
// reproducibly but definitely frequently) will send these messages to the  
// window and paint the standard caption/title over the top of the custom one.  
// So we need to handle these messages in CustomFrameWindow to prevent this  
// from happening.  
#define WM_NCUAHDRAWCAPTION 0xAE
#define WM_NCUAHDRAWFRAME   0xAF  


namespace UI
{

class LayeredWindowWrap;
class AeroWindowWrap;

//
// �Զ��崰���࣬�޷ǿͻ�����
//
class CustomWindow : public Window
{
public:
	CustomWindow(ICustomWindow*);
	~CustomWindow(void);

	VIRTUAL_BEGIN_MSG_MAP(CustomWindow)
        MESSAGE_HANDLER(WM_NCCALCSIZE, _OnNcCalcSize)
        CHAIN_MSG_MAP_MEMBER_P(m_pWindowTransparent)
//		MESSAGE_HANDLER(WM_NCPAINT, _OnNcPaint)
        MESSAGE_HANDLER(WM_NCHITTEST, _OnNcHitTest)
		MESSAGE_HANDLER(WM_NCACTIVATE, _OnNcActivate)
        MESSAGE_HANDLER(WM_SETTEXT, _OnSetTextIcon)
        MESSAGE_HANDLER(WM_SETICON, _OnSetTextIcon)
        MESSAGE_HANDLER(WM_WINDOWPOSCHANGING, _OnWindowPosChanging)
        MESSAGE_HANDLER(WM_GETMINMAXINFO, _OnGetMinMaxInfo)
        MESSAGE_HANDLER(WM_NCUAHDRAWCAPTION, _OnNCUAHDrawCaption)
        MESSAGE_HANDLER(WM_NCUAHDRAWFRAME, _OnNCUAHDrawFrame)
#if(_WIN32_WINNT >= 0x0600)
        MESSAGE_HANDLER(WM_DWMCOMPOSITIONCHANGED, _OnDwmCompositionChanged)
#endif
		CHAIN_MSG_MAP(Window)
	END_MSG_MAP()

	UI_BEGIN_MSG_MAP()
        UICHAIN_MSG_MAP_POINT_MEMBER(m_pWindowTransparent)
		UIMSG_ERASEBKGND(OnEraseBkgnd)
		UIMSG_HITTEST(OnHitTest)
        MSG_WM_SYSCOMMAND(OnSysCommand)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		
//		UIMSG_NCHITTEST(OnNcHitTest)
// 		UIMSG_NCMOUSEMOVE    (OnNcMouseMove)
// 		UIMSG_NCMOUSEHOVER   (OnNcMouseHover)
// 		UIMSG_NCMOUSELEAVE   (OnNcMouseLeave)
// 		UIMSG_NCLBUTTONDOWN  (OnNcLButtonDown)
// 		UIMSG_NCLBUTTONUP    (OnNcLButtonUp)
// 		UIMSG_NCLBUTTONDBLCLK(OnNcLButtonDblClk)
// 		UIMSG_NCRBUTTONDOWN  (OnNcRButtonDown)
// 		UIMSG_NCRBUTTONUP    (OnNcRButtonUp)
// 		UIMSG_NCRBUTTONDBLCLK(OnNcRButtonDblClk)
// 		UIMSG_NCMBUTTONDOWN  (OnNcMButtonDown)
// 		UIMSG_NCMBUTTONUP    (OnNcMButtonUp)
// 		UIMSG_NCMBUTTONDBLCLK(OnNcMButtonDblClk)

        UIMSG_GET_WINDOW_TRANSPARENT_MODE(GetWindowTransparentType)
        UIMSG_QUERYINTERFACE(CustomWindow)
        UIMSG_SERIALIZE(OnSerialize)
        UIMSG_PRECREATEWINDOW(PreCreateWindow)
	UI_END_MSG_MAP_CHAIN_PARENT(Window)

    ICustomWindow* GetICustomWindow() { return m_pICustomWindow; }

	// ��������
public:
	void  SetResizeCapability(long nType);
    long  GetResizeCapability();
	void  UpdateWindowRgn();
	
    bool  IsWindowLayered();
    void  EnableWindowLayered(bool b);
    void  EnableWindowAero(bool b);
    void  SetWindowTransparentType(WINDOW_TRANSPARENT_TYPE l);
    void  SetWindowTransparent(IWindowTransparent* pMode);
    WINDOW_TRANSPARENT_TYPE  GetWindowTransparentType();
    IWindowTransparent*  GetWindowTransparentPtr();

protected:
	void  OnSerialize(SERIALIZEDATA* pData);

private:
	BOOL  PreCreateWindow(CREATESTRUCT* pcs);

	virtual void  virtualInnerInitWindow() override;
	virtual bool  virtualCommitReq() override;
	virtual void  virtualOnPostDrawObjectErasebkgnd() override;

    bool  IsBorderlessWindow();

	// ��Ϣ��Ӧ
protected:
    
    LRESULT  _OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT  _OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT  _OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT  _OnGetMinMaxInfo( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
//	LRESULT  _OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT  _OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT  _OnSetTextIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT  _OnNCUAHDrawCaption(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT  _OnNCUAHDrawFrame(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT  _OnDwmCompositionChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    
	void  OnEraseBkgnd(IRenderTarget* hDC);
    int   OnHitTest(POINT* pt, POINT*  ptInChild);
    void  OnSysCommand(UINT nID, CPoint point);
	void  OnLButtonDblClk(UINT nFlags, POINT point);


    virtual void  virtualOnSize(UINT nType, UINT nWidth, UINT nHeight);

//	int  OnCreate(LPCREATESTRUCT lpCreateStruct);
//	LRESULT  OnNcHitTest( POINT pt );
// 	void OnNcMouseMove( UINT nHitTest, POINT point );
// 	void OnNcMouseHover( UINT nHitTest, POINT point ); 
// 	void OnNcMouseLeave();
// 	void OnNcLButtonDown( UINT nHitTest, POINT point );
// 	void OnNcLButtonUp( UINT nHitTest, POINT point );
// 	void OnNcLButtonDblClk( UINT nHitTest, POINT point );
// 	void OnNcRButtonDown( UINT nHitTest, POINT point );
// 	void OnNcRButtonUp( UINT nHitTest, POINT point );
// 	void OnNcRButtonDblClk( UINT nHitTest, POINT point );
// 	void OnNcMButtonDown( UINT nHitTest, POINT point );
// 	void OnNcMButtonUp( UINT nHitTest, POINT point );
// 	void OnNcMButtonDblClk( UINT nHitTest, POINT point );

protected:
	bool    TestResizeBit(int nBit);

protected:
    ICustomWindow*  m_pICustomWindow;
    
    //
	// ����͸�������������
	//
    IWindowTransparent*  m_pWindowTransparent;
	bool  m_bNeedToSetWindowRgn;   // �Ƿ���Ҫ�������ô�����������
    bool  m_bFirstNcCalcSize;
    bool  m_bRemoveMAXBOX;        // ���ʱȥ��MAXBOX��ʽ
    bool  m_bAeroCaptionAnimate;  // ʹ��aero ws_caption ��������ʾ�����ء����С����С����

    //
    // �߿���ק
    //
	long  m_lResizeCapability;  // ���ڱ�Ե����������־λ
	long  m_lResizeBorder;      // ������ק�ı�Ե�����С
    long  m_lMaximizeBorder;    // ���ʱ��Ҫλ����Ļ��ı߿��С��������resizeborder��һ��

    friend class AeroWindowWrap;
    friend class LayeredWindowWrap;
};

}
