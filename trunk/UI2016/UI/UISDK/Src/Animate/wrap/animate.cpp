#include "stdafx.h"
#include "animate.h"
#include "Src\Helper\msg\msghelper.h"

AnimateHelper::AnimateHelper()
{
    m_pWaitforHandle = NULL;
    m_pAnimateMgr = NULL;
}

AnimateHelper::~AnimateHelper()
{
    UIASSERT(!m_pAnimateMgr);
}

void  AnimateHelper::Init(WaitForHandlesMgr* p)
{
    m_pWaitforHandle = p;
    UIA::CreateAnimateManager(this, &m_pAnimateMgr);
}

void  AnimateHelper::UnInit()
{
    UIA::DestroyAnimateManager(m_pAnimateMgr);
    m_pAnimateMgr = NULL;
}

void  AnimateHelper::OnSetTimer(HANDLE hHandle)
{
    if (m_pWaitforHandle)
    {
        m_pWaitforHandle->AddHandle(
            hHandle, 
            static_cast<IWaitForHandleCallback*>(this), 
            0);
    }
}

void  AnimateHelper::OnKillTimer(HANDLE hHandle)
{
    if (m_pWaitforHandle)
    {
        m_pWaitforHandle->RemoveHandle(hHandle);
    }
}

void AnimateHelper::OnWaitForHandleObjectCallback(HANDLE, LPARAM)
{
    if (m_pAnimateMgr)
        m_pAnimateMgr->OnTick();
}

UIA::IAnimateManager*  AnimateHelper::GetAnimateManager()
{
    return m_pAnimateMgr;
}
