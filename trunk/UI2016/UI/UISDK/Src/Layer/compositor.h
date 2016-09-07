#pragma once
#include "layer.h"
namespace UI
{
    class UIApplication;
}

#define MSG_INVALIDATE  161311307

namespace UI
{
class Compositor
{
public:
	Compositor();
	virtual ~Compositor();

    void  SetUIApplication(UIApplication*);
    void  SetWindowRender(WindowRender*);
    UIApplication*  GetUIApplication();
    bool  CreateRenderTarget(IRenderTarget** pp);

	Layer*  CreateLayer();
    void  SetRootLayer(Layer* pChanged);
	Layer*  GetRootLayer();
    void  BindHWND(HWND);
    HWND  GetHWND();
    void  RequestInvalidate();
    void  DoInvalidate();

    virtual void  UpdateDirty(__out_opt  RectArray& arrDirtyInWindow) = 0;
    virtual void  Commit(const RectArray& arrDirtyInWindow) = 0;
    virtual void  Resize(uint nWidth, uint nSize) = 0;

protected:
    virtual Layer*  virtualCreateLayer() = 0;
    virtual void  virtualBindHWND(HWND) = 0;

protected:
    UI::UIApplication*  m_pUIApp;
    WindowRender*  m_pWindowRender;

	Layer*  m_pRootLayer;

    HWND  m_hWnd;
private:
    // ����ˢ��ʱpostmessage�Ĵ���������Ѿ�post��һ�����Ͳ���post
    long  m_lPostInvalidateMsgRef;  
};

}