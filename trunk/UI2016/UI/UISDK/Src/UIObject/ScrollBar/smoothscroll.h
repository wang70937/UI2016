#pragma once
#include "Src/Base/Message/message.h"
#include "Inc/Interface/ianimate.h"

//////////////////////////////////////////////////////////////////////////
//              ���Թ���
//
//  �ɵĹ����߼����ѷ�����
//   1. ��m_nScroll_V0��Ϊ���ٶ��������˶��������ٶȵ���0ֹͣ
//   2. ÿһ��MOUSEWHEELʱ�����ٶ�����Ϊm_nScroll_V0
//   3. ������������Ϊ·��
//   4. ÿһ��MOUSEWHEELʱ���ۼ�����Ҫ��·��
//   5. ʱ�䲻��Ҫ��2as=v^2��û��ʱ�����
//
//  �µĹ����߼���
//   1. ���ü��ٵĻ����㷨
//   2. ÿ�ι���ʱ��̶�
//   3. ��������������ٴδ���mousewheel������¶�����from/to/duration����
//     �����������ٴ�����
//
//////////////////////////////////////////////////////////////////////////

//  1. �ص�ʱ����μ�¼��ǰ�ѻص��ľ��룿Ҫ��Ȼ�ڻص��������ٴ���һ��
//     mouse wheel��ô���ö���������
//     ����callback�ټ�һ���ӿڣ����ڻ�ȡ�����Ĺ���ֵ
//       �ٴδ���ʱ��ʹ��sin���Ǻ������¼���ӵ�ǰλ�õ���ߵ�����Ҫ�Ķ���
//       ʱ��
//
//
//
// TODO: ��ios�߽絯�Թ������� m_bAnimateOverflow
//  2014/8/27 
//  . �ڶ��������е������������ֹͣ��ǰ��������δ���
//
//  2015/9/2
//  . �ڴ�����������UpdatePanningFeedback API ʵ�����������ڵı�Ե�ص���
//    ������ios�Ķ���Ч������û�뵽Ҫ��ôʵ��
//
// bouncy edge
// 
// ��Ƥ���
// Rubber band animation
// ��������
// Bounce animation
//
namespace UI
{

interface  IStoryboard;

enum SmoothScrollResult
{
    INERTIA_SCROLL_STOP = 0,      // ��������
    INERTIA_SCROLL_CONTINUE = 1,  // ��������
    INERTIA_SCROLL_BOUNCE_EDGE = 2,  // �����
};

enum MOUSEWHEEL_DIR 
{
	MOUSEWHEEL_DOWN, 
	MOUSEWHEEL_UP
};
interface ISmoothScrollCallback
{
    virtual  void SmoothScroll_Start() = 0;
    virtual  void SmoothScroll_Stop() = 0;

	// ƽ�������׶Σ�nDeltaPosΪƫ����
    virtual  SmoothScrollResult SmoothScroll_Scroll(
				MOUSEWHEEL_DIR eDir, uint nDeltaPos) = 0;

	// ��Ե�ص��׶Σ�nBounceHeightΪ����ڱ�Ե�ľ���ֵ
	virtual  SmoothScrollResult SmoothScroll_BounceEdge(
				MOUSEWHEEL_DIR eDir, uint nBounceHeight) = 0;

	// ��ȡ��ǰ�����������ĵ��Ը߶ȡ�
	// Ŀ�ģ�
	// 1. ���������ص��л�ȡ�µĿ�ʼλ�� 
	// 2. ���ڻص����ؽ׶λ�ȡfromֵ
	// �������ֵС��0��ʾ���Ϲ����ˡ�����0��ʾ���¹�����
	virtual  int  SmoothScroll_GetScrolledBounceHeight() = 0;
};

class SmoothScroll : public UIA::IAnimateEventCallback
{
public:
    SmoothScroll();
    ~SmoothScroll();


public:
    void  SetUIApplication(UIApplication* p);
    void  SetCallback(ISmoothScrollCallback* pCallback);
    bool  IsEnable();
    void  SetEnable(bool b);
    bool  IsScrolling();
    void  AddPower(int zDelta, int nViewPage);
    void  StopScroll();

protected:
	enum AnimateType
	{
		ScrollAnimate,
		BouncyEdgeAnimate_Out,  // ����ȥ
		BouncyEdgeAnimate_Back, // ������
	};

    virtual UIA::E_ANIMATE_TICK_RESULT  OnAnimateTick(UIA::IStoryboard*) override;
	void  OnTick_ScrollAnimate();
	void  OnTick_BounceEdgeAnimate();

	void  OnScrollStop();
    void  CreateScrollAnimate(uint nPower);
	void  CreateBouncyEdgeOutAnimate(uint nPower);
	void  CreateBouncyEdgeBackAnimate();
    void  DestroyAnimate();

private:
    UIApplication*  m_pUIApplication;
    ISmoothScrollCallback*  m_pCallback;

	// �������¹��������Ǵ�������
	MOUSEWHEEL_DIR  m_eMouseWheelDir;

    bool  m_bEnable;
    UIA::IStoryboard*  m_pAnimateStoryboard;

	// ÿ��mousewheel��������
	int  m_nStep;         

	// ��Ե�ص���С
	int  m_nMaxBounceEdge;
};

}