#pragma once
#include "..\wndtransmodebase.h"
#include "Src\Atl\image.h"

// TODO: δ������
// 1. ��Ӱ�Ĺ��ɲ���vs2013��ô��Ȼ
// 2. ���ڵ�Բ��δ����
// 3. ���ڵ�����δ����
// 4. ���ڵ������С��δ����

namespace UI
{
	enum GLOW_WINDOW_ALIGN
	{
		GLOW_WINDOW_LEFT,
		GLOW_WINDOW_TOP,
		GLOW_WINDOW_RIGHT,
		GLOW_WINDOW_BOTTOM,
	};

class GlowWindow : public CWindowImpl < GlowWindow >
{
public:
	GlowWindow();

	BEGIN_MSG_MAP(GlowWindow)
		MSG_WM_SIZE(OnSize)
	END_MSG_MAP()

	void  Create(HWND hWnd, GLOW_WINDOW_ALIGN eAlign);
	void  Destroy();
    void render();
	void  commit();

private:
	void OnSize(UINT nType, CSize size);

private:
	Image  m_buffer;
	GLOW_WINDOW_ALIGN  m_eAlign;
	int  m_nGlowSize;
	COLORREF  m_color;
};

class GlowWindowWrap : public WndTransModeBase
{
public:
    GlowWindowWrap();
    ~GlowWindowWrap();

    virtual WINDOW_TRANSPARENT_TYPE  GetType()
    {
        return WINDOW_TRANSPARENT_TYPE_GLOW;
    }

    virtual BOOL  ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0);

    virtual void  UpdateRgn();
    virtual void  Init(ICustomWindow* pWnd);
	virtual void  Enable(bool b);

public:
    void  Show();
    void  Hide();
private:
	GlowWindow  m_topGW;
	GlowWindow  m_leftGW;
	GlowWindow  m_rightGW;
	GlowWindow  m_bottomGW;

};

}