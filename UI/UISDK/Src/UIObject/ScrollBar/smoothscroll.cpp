#include "stdafx.h"
#include "smoothscroll.h"
#include "Inc\Interface\ianimate.h"
#include <math.h>
#include "Src\Base\Application\uiapplication.h"

#define BOUNCE_EDGE_DURATION  200
#define SCROLL_DURATION  600

SmoothScroll::SmoothScroll()
{
    m_pCallback = NULL;
    m_pUIApplication = NULL;

    m_bEnable = true;
    m_eMouseWheelDir = MOUSEWHEEL_DOWN;
    m_pAnimateStoryboard = NULL;

	m_nStep = 200;
	m_nMaxBounceEdge = 50;
}
SmoothScroll::~SmoothScroll()
{
    StopScroll();
}

void  SmoothScroll::SetUIApplication(UIApplication* p)
{
    m_pUIApplication = p;
}
void  SmoothScroll::SetCallback(ISmoothScrollCallback* pCallback)
{
    m_pCallback = pCallback;
}

bool  SmoothScroll::IsEnable()
{
    return m_bEnable;
}
void  SmoothScroll::SetEnable(bool b)
{
    m_bEnable = b;
}

bool  SmoothScroll::IsScrolling()
{
    return m_pAnimateStoryboard == NULL ? false:true;
}

// nViewPage: ��ǰҳ��ķ�Χ��һ��mousewheel������Χ��ò������һ����
// ����ᵼ�¼����б������ǹ�����
void  SmoothScroll::AddPower(int zDelta, int nViewPage)
{
    if (0 == zDelta) 
        return;

	MOUSEWHEEL_DIR eDir = zDelta < 0 ? MOUSEWHEEL_DOWN:MOUSEWHEEL_UP;
    if (m_eMouseWheelDir != eDir && IsScrolling())
        StopScroll();

    m_eMouseWheelDir = eDir;
    zDelta = abs(zDelta);

    int nMouseWheelDelta = zDelta/WHEEL_DELTA;
    int nStep = m_nStep;
    if (nViewPage > 0)
        nStep = min(m_nStep, nViewPage);

	// �ϴ�ʣ��ļ��ϱ����¼ӵ�
    int nTotalDistance = nMouseWheelDelta * nStep;
	if (m_pAnimateStoryboard && 
		m_pAnimateStoryboard->GetId() == ScrollAnimate)
	{
		int nPrevTotalDistance = m_pAnimateStoryboard->GetWParam();
		int nPrevScrolledDistance = m_pAnimateStoryboard->GetLParam();
		nTotalDistance += nPrevTotalDistance - nPrevScrolledDistance;
	}

	bool bIsScrolling = IsScrolling();
	if (m_pAnimateStoryboard && 
		m_pAnimateStoryboard->GetId() == BouncyEdgeAnimate_Out)
	{
		CreateBouncyEdgeOutAnimate(nTotalDistance);
	}
	else if (m_pAnimateStoryboard && 
		m_pAnimateStoryboard->GetId() == BouncyEdgeAnimate_Back)
	{
		CreateBouncyEdgeOutAnimate(nTotalDistance);
	}
	else
	{
		CreateScrollAnimate(nTotalDistance);
	}

	// �ǵ�һ�ο�ʼ����
    if (!bIsScrolling && m_pCallback)
    {
        m_pCallback->SmoothScroll_Start();
    }
}

void  SmoothScroll::CreateScrollAnimate(uint nPower)
{
	if (!m_pUIApplication)
		return;

	// ʹ��in out���ͣ��ᵼ�¶����������ٴ���mousewheel��ǰ�󶯻�������
	// ease_out���о��������ٽ׶λ��Ǽ���̫����Щ
	// ease_out_sine ease_out_quad����

#define EASETYPE  UIA::ease_out_sine

	UIA::IFromToTimeline*  pTimeline = NULL;
    if (m_pAnimateStoryboard)
	{ 
		// ��ǰ���ڹ����У�ֱ�����ù�������
        pTimeline = static_cast<UIA::IFromToTimeline*>(
				m_pAnimateStoryboard->FindTimeline(0));
	}
	else
	{
        UIA::IAnimateManager*  pAnimateMgr = m_pUIApplication->GetAnimateMgr();
		m_pAnimateStoryboard = pAnimateMgr->CreateStoryboard(
				this, ScrollAnimate);

		pTimeline = m_pAnimateStoryboard->CreateTimeline(0);
		m_pAnimateStoryboard->Begin();
	}

	// ��¼
	m_pAnimateStoryboard->SetWParam(nPower);
	m_pAnimateStoryboard->SetLParam(0);
	pTimeline->SetParam(
		0,
		(float)nPower, 
		SCROLL_DURATION)
        ->SetEaseType(EASETYPE);
}

// m_eMouseWheelDir�а����˷���
void  SmoothScroll::CreateBouncyEdgeOutAnimate(uint nPower)
{
	int bounce_from = 0;

	int bounce_height = nPower/10;
	if (bounce_height > m_nMaxBounceEdge)
		bounce_height = m_nMaxBounceEdge;

	if (bounce_height <= 0)
		return;

	UIA::IFromToTimeline* pTimeline = NULL;
	int nDuration = BOUNCE_EDGE_DURATION;

	if (m_pAnimateStoryboard)
	{ 
		m_pAnimateStoryboard->SetId(BouncyEdgeAnimate_Out);
		pTimeline = static_cast<UIA::IFromToTimeline*>(
			m_pAnimateStoryboard->FindTimeline(0));

		// ����ӵ�ǰλ�õ���ߵ㣬����Ҫ�Ķ���ʱ����
		// dpercent = sin(tpercent)
		// tpercent = asin(dpercent)
		// tpercent = asin(bounce_from/bounce_height)
		// duration = (1-tpercent)*full_duration
		if (m_pCallback)
		{
			bounce_from = abs(
				m_pCallback->SmoothScroll_GetScrolledBounceHeight());
		}
		nDuration = _round(
			(1 - asin((float)bounce_from/(float)bounce_height))*nDuration);
	}
	else
	{
        UIA::IAnimateManager*  pAnimateMgr = m_pUIApplication->GetAnimateMgr();
		m_pAnimateStoryboard = pAnimateMgr->CreateStoryboard(
				this, BouncyEdgeAnimate_Out);

		pTimeline = m_pAnimateStoryboard->CreateTimeline(0);
		m_pAnimateStoryboard->Begin();
	}

	pTimeline->SetParam(
		(float)bounce_from,
		(float)bounce_height,
		(float)nDuration); 
}

void  SmoothScroll::CreateBouncyEdgeBackAnimate()
{
	DestroyAnimate();

    UIA::IAnimateManager*  pAnimateMgr = m_pUIApplication->GetAnimateMgr();
	m_pAnimateStoryboard = pAnimateMgr->CreateStoryboard(
		    this, BouncyEdgeAnimate_Back);

	UIA::IFromToTimeline* pTimeline = m_pAnimateStoryboard->CreateTimeline(0);

	int bouncy_height = m_nMaxBounceEdge;
	if (m_pCallback)
	{
		bouncy_height = m_pCallback->
			SmoothScroll_GetScrolledBounceHeight();

		if (bouncy_height < 0)
		{
			UIASSERT(m_eMouseWheelDir == MOUSEWHEEL_UP);
			bouncy_height = -bouncy_height;
		}
	}

	pTimeline->SetParam(
		(float)bouncy_height,
		0,
		BOUNCE_EDGE_DURATION); 
	m_pAnimateStoryboard->Begin();
}

void  SmoothScroll::DestroyAnimate()
{
    if (m_pUIApplication && m_pAnimateStoryboard)
    {
        UIA::IAnimateManager* pAnimateMgr = m_pUIApplication->GetAnimateMgr();

        pAnimateMgr->RemoveStoryboard(m_pAnimateStoryboard);
        m_pAnimateStoryboard = NULL;
    }
}


void  SmoothScroll::StopScroll()
{
    if (!IsScrolling())
        return;
   
    DestroyAnimate();
    OnScrollStop();
}

// 1. ��������StopScroll����
// 2. ������������
void  SmoothScroll::OnScrollStop()
{
	if (m_pCallback)
	{
		m_pCallback->SmoothScroll_Stop();
	}
}

UIA::E_ANIMATE_TICK_RESULT SmoothScroll::OnAnimateTick(UIA::IStoryboard* pStoryboard)
{
    UIASSERT(pStoryboard == m_pAnimateStoryboard);
	long  lAnimateId = m_pAnimateStoryboard->GetId();
	bool  bBouncyEdgeAnimate = 
			(lAnimateId == BouncyEdgeAnimate_Out ||
			lAnimateId == BouncyEdgeAnimate_Back);

	if (bBouncyEdgeAnimate)
		OnTick_BounceEdgeAnimate();
	else
		OnTick_ScrollAnimate();

    return UIA::ANIMATE_TICK_RESULT_CONTINUE;
}

void  SmoothScroll::OnTick_ScrollAnimate()
{
	bool bFinish = m_pAnimateStoryboard->IsFinish();
	do 
	{
		int nCurValue = m_pAnimateStoryboard->GetTimeline(0)
			->GetCurrentIntValue();

		int  nScrolledDistance = m_pAnimateStoryboard->GetLParam();
		m_pAnimateStoryboard->SetLParam(nCurValue);

		// ƫ����
		int nScrollNow = nCurValue - nScrolledDistance;
		if (0 == nScrollNow)
			break;

		SmoothScrollResult lResult = INERTIA_SCROLL_STOP;
		if (m_pCallback)
		{
			lResult = m_pCallback->SmoothScroll_Scroll(
				m_eMouseWheelDir, nScrollNow);
		}

		if (INERTIA_SCROLL_STOP == lResult)
		{
			int nRemainDistance = 0;
			if (m_pAnimateStoryboard && m_pAnimateStoryboard->GetId() == ScrollAnimate)
			{
				int nTotalDistance = m_pAnimateStoryboard->GetWParam();
				int nScrolledDistance = m_pAnimateStoryboard->GetLParam();
				nRemainDistance = nTotalDistance - nScrolledDistance;
			}

			StopScroll();
			if (nRemainDistance > 0)
			{
				// bouncy animate
				CreateBouncyEdgeOutAnimate(nRemainDistance);
			}
			break;
		}
		else if (INERTIA_SCROLL_BOUNCE_EDGE == lResult)
		{
		}
	}
	while (0);

	if (bFinish)
	{
		m_pAnimateStoryboard = NULL;
		OnScrollStop();
	}
}

void  SmoothScroll::OnTick_BounceEdgeAnimate()
{
	bool bFinish = m_pAnimateStoryboard->IsFinish();
	do 
	{
		int nScrollNow = m_pAnimateStoryboard->GetTimeline(0)
			->GetCurrentIntValue();

		// ������Ҫ�����ľ���,����ֵ
		SmoothScrollResult lResult = INERTIA_SCROLL_STOP;
		if (m_pCallback)
		{
			lResult = m_pCallback->SmoothScroll_BounceEdge(
				m_eMouseWheelDir, nScrollNow);
		}
	}
	while (0);

	if (bFinish)
	{
		if (m_pAnimateStoryboard->GetId() == BouncyEdgeAnimate_Out)
		{
			m_pAnimateStoryboard = NULL;
			CreateBouncyEdgeBackAnimate();
		}
		else 
		{
			m_pAnimateStoryboard = NULL;
			OnScrollStop();
		}
	}
}