#include "stdafx.h"
#include "compositor.h"
#include "windowrender.h"

Compositor::Compositor()
{
	m_pRootLayer = NULL;
    m_pUIApp = NULL;
    m_hWnd = NULL;
    m_pWindowRender = NULL;

    m_lPostInvalidateMsgRef = 0;
}

Compositor::~Compositor()
{
    // ��ÿ��object�Լ�ȥ����
//     while (m_pRootLayer)
//     {
//         Layer* p = m_pRootLayer;
//         m_pRootLayer = m_pRootLayer->GetNext();
// 
//         delete p;
//     }
}

void  Compositor::SetUIApplication(UIApplication* p)
{
    m_pUIApp = p;
}
UIApplication*  Compositor::GetUIApplication()
{
    return m_pUIApp;
}

Layer*  Compositor::CreateLayer()
{
    Layer* pLayer = this->virtualCreateLayer();
    if (pLayer)
        pLayer->SetCompositorPtr(this);

    return pLayer;
}

void  Compositor::SetRootLayer(Layer* pChanged)
{
    m_pRootLayer = pChanged;
}

Layer*  Compositor::GetRootLayer()
{
	return m_pRootLayer;
}

void Compositor::BindHWND(HWND hWnd)
{
    if (m_hWnd == hWnd)
        return;

    m_hWnd = hWnd;
    this->virtualBindHWND(hWnd);
}

HWND  Compositor::GetHWND()
{
    return m_hWnd;
}

void  Compositor::RequestInvalidate()
{
    UIASSERT (m_hWnd);

    if (m_lPostInvalidateMsgRef == 0)
    {
        ::PostMessage(m_hWnd, UI_MSG_POSTMESSAGE, MSG_INVALIDATE, 0);
        m_lPostInvalidateMsgRef++;
    }
}

void Compositor::DoInvalidate()
{
    m_lPostInvalidateMsgRef = 0;

    RectArray arrDirtyInWindow;
    this->UpdateDirty(arrDirtyInWindow);

    if (!m_pWindowRender->CanCommit())
        return;

    this->Commit(arrDirtyInWindow);
}

void  Compositor::Commit(const RectArray& arrDirtyInWindow)
{
    IWindowCommitListener* pListener = m_pWindowRender->GetCommitListener();

    if (pListener)
    {
        pListener->PreCommit(
            arrDirtyInWindow.GetArrayPtr2(),
            arrDirtyInWindow.GetCount());

        this->virtualCommit(arrDirtyInWindow);

        pListener->PostCommit(
            arrDirtyInWindow.GetArrayPtr2(),
            arrDirtyInWindow.GetCount());
    }
    else
    {
        this->virtualCommit(arrDirtyInWindow);
    }
}

bool  Compositor::CreateRenderTarget(IRenderTarget** pp)
{
    return m_pWindowRender->CreateRenderTarget(pp);
}

void Compositor::SetWindowRender( WindowRender* p )
{
    m_pWindowRender = p;
}
