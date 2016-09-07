#include "stdafx.h"
#include "animatemgr.h"
#include "storyboard.h"

//#include "UISDK\Project\VSync\inc\vsync_inc.h"
//#pragma comment(lib, "vsync.lib")
//#include "UISDK\Kernel\Src\Util\Stopwatch\stopwatch.h"

using namespace UIA;

AnimateManager::AnimateManager()
{
    m_pIAnimateManager = NULL;
	m_hTimer = NULL;
    m_pITimerCallback = NULL;

    m_bTimerStart = false;
	m_bHandlingTimerCallback = false;
	m_nFps = 60;

	::QueryPerformanceFrequency(&m_liPerFreq);
    m_hModuleWinmm = NULL;
}

AnimateManager::~AnimateManager()
{
	// 注：close timer handle不能在UnCoInitialize后调用
    Destroy();

    if (m_pIAnimateManager)
    {
        delete m_pIAnimateManager;
        m_pIAnimateManager = NULL;
    }

    if (m_hModuleWinmm)
    {
        FreeLibrary(m_hModuleWinmm);
        m_hModuleWinmm = NULL;
    }
}

IAnimateManager*  AnimateManager::GetIAnimateManager()
{
    if (NULL == m_pIAnimateManager)
        m_pIAnimateManager = new IAnimateManager(this);

    return m_pIAnimateManager;
}

void  AnimateManager::SetTimerCallback(IAnimateTimerCallback* p)
{
    m_pITimerCallback = p;
}

int  AnimateManager::SetFps(int n) 
{ 
    int nOld = m_nFps;
    m_nFps = n; 
    if (m_nFps > 1000)
        m_nFps = 1000;
    return nOld;
}
void AnimateManager::Destroy()
{
	KillTimer();

    if (m_hTimer)
    {
        CloseHandle(m_hTimer);
        m_hTimer = NULL;
    }

    list<Storyboard*>::iterator iter = m_listStoryboard.begin();
    for (; iter != m_listStoryboard.end(); ++iter)
    {
		(*(iter))->NotifyDiscard();
		delete (*iter);
	}
	m_listStoryboard.clear();
}

Storyboard*  AnimateManager::CreateStoryboard(
                IAnimateEventCallback* pNotify, 
                int nId, 
                WPARAM wParam, 
                LPARAM lParam)
{
	Storyboard* pStoryboard = new Storyboard;
    pStoryboard->SetAnimateMgr(this);
	pStoryboard->SetEventListener(pNotify);
	pStoryboard->SetId(nId);
	pStoryboard->SetWParam(wParam);
	pStoryboard->SetLParam(lParam);

    return pStoryboard;
}

void  AnimateManager::CancelStoryboard(Storyboard* p)
{
    RemoveStoryboard(p);
}
void  AnimateManager::RemoveStoryboard(Storyboard* p)
{
	if (NULL == p)
		return;

	// 如果正在对m_listObjTimeline进行遍历中，则不能在这里对list做erase操作

    list<Storyboard*>::iterator iter = std::find(
        m_listStoryboard.begin(), m_listStoryboard.end(), p);

    if (iter == m_listStoryboard.end())
    {
        SAFE_DELETE(p);
        return;
    }

	p->NotifyCancel();

    if (m_bHandlingTimerCallback)  
    {
        (*iter) = NULL;
        SAFE_DELETE(p);
    }
    else
    {
        m_listStoryboard.erase(iter);
		SAFE_DELETE(p);

		if (0 == m_listStoryboard.size())
		{
			KillTimer();
		}
    }
}

void AnimateManager::ClearStoryboardOfNotify(IAnimateEventCallback* pNotify)
{
	if (NULL == pNotify)
		return;

	list<Storyboard*>::iterator iter = m_listStoryboard.begin();
	for (; iter != m_listStoryboard.end(); )
	{
		Storyboard* pStoryboard = *iter;

		if (pStoryboard->GetEventListener() != pNotify)
		{
			++iter;
			continue;
		}

		if (m_bHandlingTimerCallback)  
		{
			pStoryboard->SetFinish();
			++iter;
		}
		else
		{
			iter = m_listStoryboard.erase(iter);
			SAFE_DELETE(pStoryboard);
		}
	}

	if (0 == m_listStoryboard.size())
	{
		KillTimer();
	}
}

void  AnimateManager::RemoveStoryboardByNotityAndId(
        IAnimateEventCallback* pCallback, int nId)
{
    if (NULL == pCallback)
        return;

    list<Storyboard*>::iterator iter = m_listStoryboard.begin();
    for (; iter != m_listStoryboard.end(); )
    {
        Storyboard* pStoryboard = *iter;

        if (pStoryboard->GetEventListener() != pCallback)
        {
            ++iter;
            continue;
        }

        if (pStoryboard->GetId() != nId)
        {
            ++iter;
            continue;
        }

        if (m_bHandlingTimerCallback)
        {
            pStoryboard->SetFinish();
            ++iter;
        }
        else
        {
            iter = m_listStoryboard.erase(iter);
            SAFE_DELETE(pStoryboard);
        }
    }

    if (0 == m_listStoryboard.size())
    {
        KillTimer();
    }
}

void AnimateManager::AddStoryboard(Storyboard* p)
{
	if (NULL == p)
		return;

	list<Storyboard*>::iterator iter = std::find(
		m_listStoryboard.begin(), m_listStoryboard.end(), p);

	if (iter != m_listStoryboard.end())
		return;

	m_listStoryboard.push_back(p);

    SetTimer();

    if (!p->IsDelayWaiting())
        p->OnAnimateStart();
}

// 阻塞型动画
void  AnimateManager::AddStoryboardBlock(Storyboard* pStoryboard)
{
    if (!pStoryboard)
        return;

    SetTimer();

    pStoryboard->OnAnimateStart();

    //StopWatch watch;
    bool bFinish = false;
    while (1)
    {
        //watch.Start();
        bFinish = pStoryboard->OnTick();
        //long elapse = watch.Now();
        //assert(elapse < 5);

        if (bFinish)
        {
            pStoryboard->NotifyEnd();

            SAFE_DELETE(pStoryboard);
            if (0 == m_listStoryboard.size())
                KillTimer();

            return;
        }
        else
        {
            // sleep效率太低，换成使用Waitable Timer
            WaitForSingleObject(m_hTimer,INFINITE); 
            //UI::WaitForVerticalBlank();
        }
    }
}


void  AnimateManager::OnTick()
{ 
//    UI::WaitForVerticalBlank();

// 	static long lPrev = 0;
// 	static LARGE_INTEGER liPrev = {0};
// 	static LARGE_INTEGER liNow = {0};
// 	::QueryPerformanceCounter(&liNow);
// 	 int time = (int)((liNow.QuadPart - liPrev.QuadPart)*1000/m_liPerFreq.QuadPart);
// 
// 	char  szText[64] = {0};
// 	sprintf(szText, "%d\n", time);
// 	liPrev = liNow;
// 	::OutputDebugStringA(szText);
	
	m_bHandlingTimerCallback = true;
 
    list<Storyboard*>::iterator iter = m_listStoryboard.begin();
	for (; iter != m_listStoryboard.end();)
	{
		Storyboard* pStoryboard = *iter;

        // 在遍历过程中被销毁了。这里清理掉
        if (!pStoryboard)
        {
            iter = m_listStoryboard.erase(iter);
            continue;
        }

        // 延时动画
		if (pStoryboard->IsDelayWaiting())
		{
			pStoryboard->UpdateDelayWaiting();

			if (pStoryboard->IsDelayWaiting())
			{
				++iter;
				continue;
			}
			else
			{
				pStoryboard->OnAnimateStart();
			}
		}

		pStoryboard->OnTick();
        pStoryboard = *iter;  // 再更新。有可能中途被remove了

		if (pStoryboard && pStoryboard->IsFinish())
		{
			pStoryboard->NotifyEnd(); 
			delete pStoryboard;
			iter = m_listStoryboard.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	m_bHandlingTimerCallback = false;

    if (0 == m_listStoryboard.size())
    {
        KillTimer();
    }
}

VOID CALLBACK TimerAPCProc(
			   LPVOID lpArgToCompletionRoutine,
			   DWORD dwTimerLowValue,
			   DWORD dwTimerHighValue
						   )
{
	//AnimateManager*  pThis = (AnimateManager*)lpArgToCompletionRoutine;
	//pThis->OnTime();
}

#define MINIMUM_TIMER_RESOLUTION 1  // ms
//
// Setting a higher resolution can also reduce overall system performance, 
// because the thread scheduler switches tasks more often.
//
// http://forum.sysinternals.com/bug-in-waitable-timers_topic16229.html
//
static void  improve_timer_resolution(HMODULE& hModuleWinmm)
{

    //#include <mmsystem.h>
    //#pragma comment(lib, "Winmm.lib")
    //timeBeginPeriod(1);

    if (!hModuleWinmm)
        hModuleWinmm = LoadLibrary(TEXT("Winmm.dll"));

    if (hModuleWinmm)
    {
        typedef UINT (__stdcall *pfntimeBeginPeriod)(UINT uPeriod);

        static FARPROC func = NULL;
        if (!func)
            func = GetProcAddress(hModuleWinmm, "timeBeginPeriod");

        if (func)
            ((pfntimeBeginPeriod)func)(MINIMUM_TIMER_RESOLUTION);
    }
}
static void  restore_timer_resolution(HMODULE& hModuleWinmm)
{
    if (!hModuleWinmm)
        return;

    typedef UINT (__stdcall *timeEndPeriod)(UINT uPeriod);

    static FARPROC func = NULL;
    if (!func)
        func = GetProcAddress(hModuleWinmm, "timeEndPeriod");

    if (func)
        ((timeEndPeriod)func)(MINIMUM_TIMER_RESOLUTION);
}

void AnimateManager::SetTimer()
{
	if (m_bTimerStart)
		return;

    if (!m_hTimer)
    {
        // 自动重置，即调用WaitForSingleObject时重置HANDLE
        // 如果是手动的，得再次调用SetWaitableTimer来重置
	    m_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
    }
    
    improve_timer_resolution(m_hModuleWinmm);

    if (m_pITimerCallback)
        m_pITimerCallback->OnSetTimer(m_hTimer);

    if (0 == m_nFps)
        m_nFps = 60;

    int nPeriod = 1000/m_nFps;  // 计时器周期

    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -1000*10*nPeriod;   // 第一次响应延迟时间。负值表示一个相对的时间，代表以100纳秒为单位的相对时间，（如从现在起的5ms，则设置为-50000）

    // 注： 不要使用TimerAPCProc,该方式需要让线程处理alertable状态，
    //      即调用SleepEx(x, TRUE)让线程进入等待状态
    // LONG lPeriod：设置定时器周期性的自我激发，该参数的单位为毫秒。
    // 如果为0，则表示定时器只发出一次信号，大于0时，定时器每隔一段时
    // 间自动重新激活一个计时器，直至取消计时器使用
    if (!SetWaitableTimer(m_hTimer, &liDueTime, nPeriod, NULL, NULL, 0))  
    {
        UIASSERT(0);
    }
    m_bTimerStart = true;

// 	SleepEx
// 	if (!SetWaitableTimer(m_hTimer, &liDueTime, nPeriod, TimerAPCProc, (LPVOID)this, 0))
// 	{
// 		assert(0);
// 	}
}
void AnimateManager::KillTimer()
{
	if (m_hTimer)
	{
        if (m_pITimerCallback)
            m_pITimerCallback->OnKillTimer(m_hTimer);

		CancelWaitableTimer(m_hTimer);
        m_bTimerStart = false;

        restore_timer_resolution(m_hModuleWinmm);
	}
}
