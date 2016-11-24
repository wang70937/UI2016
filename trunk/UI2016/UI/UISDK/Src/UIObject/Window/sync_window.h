#pragma once
#include "Inc\Interface\iwindow.h"

//////////////////////////////////////////////////////////////////////////
//
//                          �����ƶ��Ĵ���
//
//////////////////////////////////////////////////////////////////////////

// TODO: 
//		1. ���ڶ���Ļ��Ե��ճ��
//		2. ���ڶ�����site window��ճ�������ճ��
//		3. host �����ƶ���site window��ճ��



//
//  ��UI��Ϣ������::SendMessage����
//	���������ƶ�ʱ����SYNC_CUMTOM���͵Ĵ��ڷ���һ����Ϣȥͬ���Լ���λ��
//		wparam:  HDWP
//		lparam:  RECT* 
//  return:  HWDP
#define UI_WM_SYNC_WINDOWPOSCHANING  (WM_USER+828)

//
//	ͨ����������WM_WINDOWPOSCHANGING��Ϣ�ķ���������ϵͳ�������Լ���ȥ����һ��DeferWindowPos
//
//	�ŵ�:
//		1. �򵥣�ֻ��Ҫ����һ����Ϣ
//		2. �ܴ�����ִ���λ�á���С�仯�ĳ���
//
//	ȱ��:
//		1. ���������ײ�����ѭ��
//
class SyncWindow
{
public:
    SyncWindow();
    void  SetHwnd(HWND hWnd);
    void  _OnShowWindow(WPARAM wParam, LPARAM lParam);
	void  _OnWindowPosChanging(LPWINDOWPOS lpWndPos, BOOL& bHandled);
	void   OnHostWindowPosChanging(LPWINDOWPOS lpWndPos);
	bool   OnSiteWindowPosChanging(LPWINDOWPOS lpWndPos);
	bool   CheckRectAnchor(const CRect* prcHost, CRect* prcSite, bool bChangeSize, SyncWindowData* pData);
	void   GetRectByWNDPOS(LPWINDOWPOS lpWndPos, CRect* prc);
	HDWP   HandleWindowAnchor(HDWP& hdwp, const CRect& rcSrcWindow, const SyncWindowData& rData);

	void   _OnNcDestroy();
	HWND   GetMyHWND();
	void   _OnEnterSizeMove();
	void   _OnExitSizeMove();
public:
	bool   AddAnchorItem(const SyncWindowData& data);
	bool   RemoveAnchorItem(HWND hWnd);
	bool   ModifyAnchorItem(const SyncWindowData& data);
    void   OnAddAnchorItem(HWND hHostWnd);     // �Լ���Ϊ������;
    void   OnModifyAnchorItem(HWND hHostWnd);  // �Լ���Ϊ������;
    void   OnRemoveAnchorItem(HWND hHostWnd);  // �Լ���Ϊ������;
    
    void   SyncNow();

	// Host Window�ƶ�ʱ��ͬ��site Windowλ��ʱ����site window����Ϣ
	void   OnHostWindowPosChanging();
	bool   ClearAnchorItem();
	int    FindAnchorItem(HWND hWnd);
    int    GetAvailableHWNDCount();  // �����ж��Ƿ�ɼ�����ʱ���ɼ�Ҳ��Ҫ������Ҫ��Ȼ����ʾʱ��λ�þͲ���ȷ��;

	void   HideAllAnchorItem();
    bool   IsSizeMoveIng();

protected:
	vector<SyncWindowData>   m_vecAnchorItems;
    HWND   m_hWnd;
	HWND   m_hHostWnd;                 // ��ʾ�Լ���ǰҪ�����ĸ����ڽ����ƶ���
                                       // ���ΪNULL���ʾ�Լ���������������
	bool   m_bSendByDeferWindowPos;    // WM_WINDOWPOSCHANGING��Ϣ�ķ�����
	byte   m_nAnchorMagnetCapability;  // ͣ���Ĵ�����������С�ڶ���pxʱ�Զ�ճ��
	bool   m_bAnchorToScreenEdge;      // �Ƿ����ͣ������Ļ��Ե
	bool   m_bAnchorToOtherSize;       // �Ƿ��������������������
	bool   m_bSizeMove;                // ������ǰ�Ƿ�������ק����
};

