#ifndef IWINDWOBASE_H_45FADAE6_FB0B_49e0_8119_6F6D58FD5700
#define IWINDWOBASE_H_45FADAE6_FB0B_49e0_8119_6F6D58FD5700

#include "irenderlayer.h"
#include "ipanel.h"

namespace UI
{
interface IWindowMouseMgr;
interface IWindowKeyboardMgr;

#define ANCHOR_NONE        0
#define ANCHOR_LEFT        0x0001
#define ANCHOR_RIGHT       0x0002
#define ANCHOR_TOP         0x0004
#define ANCHOR_BOTTOM      0x0008
#define ANCHOR_OUT_LEFT    0x0010
#define ANCHOR_OUT_RIGHT   0x0020
#define ANCHOR_OUT_TOP     0x0040
#define ANCHOR_OUT_BOTTOM  0x0080
#define ANCHOR_CUSTOM      0x0100   // ����UI_WM_SYNC_WINDOWPOSCHANGING��Ϣ�����ڣ��ɴ������о�������ƶ��Լ�


enum SYNC_WINDOW_EVENT_TYPE
{
    ADD_SYNC_WINDOW,            // ֪ͨHOST, ����һ��������.LPARAM: SyncWindowData*
    MODIFY_SYNC_WINDOW,         // ֪ͨHOST, �޸�һ����������Ϣ.LPARAM: SyncWindowData*
    REMOVE_SYNC_WINDOW,         // ֪ͨHOST, �Ƴ�һ��������.LPARAM: SyncWindowData*

    ADD_SYNC_WINDOW_RESULT,     // ֪ͨSITE����ӳɹ�.LPARAM: host HWND
    MODIFY_SYNC_WINDOW_RESULT,  // ֪ͨSITE���޸ĳɹ�.LPARAM: host HWND
    REMOVE_SYNC_WINDOW_RESULT,  // ֪ͨSITE��ɾ���ɹ�.LPARAM: host HWND

    HOST_WINDOW_DESTROYED,      // Ҫ�����HOST�����ˣ�����Լ���HOST���. LPARAM: ��
    HOST_WINDOW_POSCHANGING,    // ���͸�SITE���ڣ�������һ����Ӧ��WindowPosChaning��Ϣ����ΪHOST�ƶ�������
};

struct  AnchorData
{
    int  xOffset;
    int  yOffset;

    union{
        int xOffset2;
        int Width;         // -1 ��ʾ���޸Ĵ��ڿ��
    };
    union{
        int yOffset2;
        int Height;       // -1 ��ʾ���޸Ĵ��ڸ߶�
    };

    AnchorData()
    {
        xOffset = yOffset = xOffset2 = yOffset2 = 0;
    }
};

#define SWDS_MASK_ANCHORTYPE 0x0001
#define SWDS_MASK_ANCHORDATA 0x0002
#define SWDS_MASK_ANCHORON   0x0004
#define SWDS_MASK_ALL   (SWDS_MASK_ANCHORTYPE|SWDS_MASK_ANCHORDATA|SWDS_MASK_ANCHORON)
struct  SyncWindowData
{
    HWND         m_hWnd;           
    UINT         m_nMask;
    UINT         m_nAnchorType;   
    AnchorData   m_rcAnchorData;  // ��m_nAnchorType��Ӧλ��ֵʱ��m_rcAnchorData�ĸ�λ��Ч
    bool         m_bAnchorOn;     // �����Ƿ������á�������ʱ�������Ƴ�HOST���ڣ������и��棬Ȼ��������HOST�������ٴο������档
	bool         m_bSyncVisible;  // �Ƿ�ͬ������

    SyncWindowData()
    {
        m_hWnd = NULL;
        m_nAnchorType = ANCHOR_NONE;
        m_bAnchorOn = true;
		m_bSyncVisible = false;

        m_nMask = 0;
    }
};

// window style
typedef struct tagWindowStyle
{
	bool  destroyed : 1;       // ��ʾ�ô����Ѿ���������(WM_NCDESTROY)�����ڴ���OnFinalMessage
	bool  attach : 1;          // ��ʾ�ô�����attach�ģ��������������ⲿ������
	bool  setcreaterect : 1;   // ��������ʱָ���˴��ڴ�С�������ٽ��д��ڲ�����
	bool  dialog_noresize : 1; // ���ڽ��win7����Dialog��ʾ��С��GetWindowRect��һ�µ�����
	bool  hard_composite: 1;   // ������ʹ��Ӳ���ϳ� 
}WindowStyle;

// �����ⲿҵ��ʵ���ߴ�������Ϣ��
// �ⲿ����Ҫ���IWindowBase������ֻ��Ҫʵ��IWindowMessageCallback�ӿڼ��ɡ�
interface IWindowMessageCallback
{
	virtual BOOL  OnWindowMessage(UINT, WPARAM, LPARAM, LRESULT& lResult) = 0;
	virtual BOOL  OnWindowUIMessage(UIMSG* pMsg) = 0;
};

class WindowBase;
interface __declspec(uuid("1C7CED21-3CF6-49C9-9E52-72522C8A1CF6"))
UISDKAPI IWindowBase : public IPanel
{
    HWND  GetHWND();
    IWindowRender*  GetIWindowRender();
	ISkinRes*  GetSkinRes();

    BOOL  IsChildWindow();
	BOOL  IsWindowVisible();
    void  ShowWindow();
    void  HideWindow();
    bool  IsDoModal();
    void  CenterWindow(HWND hWndCenter = NULL);
    void  CenterWindow(HMONITOR hMonitor = NULL);

    void  SetFocusObject(IObject* pObj);
	void  SetPressObject(IObject* pObj);
    IObject*  GetHoverObject();
    IObject*  GetPressObject();
    IObject*  GetObjectByPos(IObject* pObjParent, POINT* pt, bool bSkinBuilderInvoke=false);

    bool  Create(LPCTSTR szID, HWND hWndParent=NULL, RECT* prc=NULL, long lStyle = 0, long lExStyle = 0);
    bool  Attach(HWND hWnd, LPCTSTR szID);
    void  Detach();
    long  DoModal(LPCTSTR szID, HWND hWndParent, bool canResize);
    long  DoModal(HINSTANCE hResInst, UINT nResID, LPCTSTR szID, HWND hWndParent);
    HWND  DoModeless(LPCTSTR szID, HWND hWndParent, bool canResize);
    HWND  DoModeless(HINSTANCE hResInst, UINT nResID, LPCTSTR szID, HWND hWndParent);
    void  EndDialog(INT_PTR);
	void  DestroyWindow();

    void  UpdateWindow(HDC hDC, RECT* prcInvalid);
    void  SaveMemBitmap(TCHAR* szFile);
    void  DrawMemBitmap(HDC hDC, RECT* prc, bool bAlphaBlend);
    void  EnableDwmTransition(bool b);
	void  EnableGpuComposite(bool b);
	bool  IsGpuComposite();
	void  DirectComposite();
	void  SetWindowMessageCallback(IWindowMessageCallback*);

    void  CalcWindowSizeByClientSize( SIZE sizeClient, SIZE* pSizeWindow );
    void  CalcClientRectByWindowRect( RECT* rcWindow, RECT* rcClient );

    bool  AddAnchorItem(const SyncWindowData& data);
    void  HideAllAnchorItem();

    HRESULT  SetDroppable(bool b);

    UI_DECLARE_INTERFACE(WindowBase);
};

class Window;
interface __declspec(uuid("5C36801E-5929-4512-A998-F9719DCC6903"))
UISDKAPI IWindow : public IWindowBase
{
    UI_DECLARE_INTERFACE(Window)
};

}

#endif  // IWINDWOBASE_H_45FADAE6_FB0B_49e0_8119_6F6D58FD5700