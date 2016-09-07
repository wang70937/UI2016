#pragma once
#include "animatemgr.h"
#include "movealgo.h"
#include "Src\Util\Stopwatch\stopwatch.h"

// 2014.2.20  TODO: ����repeat delay���ܡ�ÿһ��ѭ��ʱ������ʱһ��ʱ��
// 2015.10.13 
//  Reverse��ת��ʵ��ԭ��
//  ��תʱ��from/to���öԵ���ֻ��Ҫ����ǰ��ʱ������޸�Ϊ 1-time progress
//  ���ɡ�

namespace UIA
{
#define TIMELINE_FLAG_FINISH       0x0004   // ָʾһ��timeline�Ƿ��Ѿ����ڽ�����
#define TIMELINE_FLAG_NEED_RESET   0x0008   // ��ʾһ��timelineһ��ѭ���������´ο�ʼǰ��Ҫ����
#define TIMELINE_FLAG_REVERSING    0x0010   // ��ʾ���ڷ�����ж���
#define TIMELINE_FLAG_AUTO_REVERSE  0x0020  // �ö����Զ���ת

class Timeline
{
public:
    Timeline();
    virtual ~Timeline();

public:
    ITimeline*  GetITimeline();
    void  SetITimeline(ITimeline* p);
    void  SetAnimateMgr(AnimateManager* p) { m_pAnimateMgr = p; }

    void  SetId(int nId) { m_nId = nId; }
    int   GetId() { return m_nId; }

	void  SetUserData(LPARAM lParam) { m_pParam = lParam;}
	LPARAM  GetUserData() { return m_pParam; }

	void  SetRepeateCount(int n) { m_nRepeatTimes = n; }
    void  SetTimeType(TIMELINE_TIME_TYPE eType) { m_eTimeType = eType; }
    void  SetAutoReverse(bool b);
	bool  IsAutoReverse();

    bool  IsFinish();
    void  SetFinish();
    bool  IsReversing();
    void  SetReversing(bool);

	float  GetCurrentValue() { return m_fCurrentValue; }

    void  OnAnimateStart();

protected:
	void  reset_time();

public:
	virtual bool  OnTick();  // ע�������麯������Ϊ����idle timeline ֱ�ӷ���false;
	
	// ��������麯��
	// virtual void  Init(){};
	// ����ʹ�õ�ǰʱ����ȣ��������ǰֵ
    virtual float  OnTick(float fTimePercent) = 0;

protected:
	AnimateManager*  m_pAnimateMgr;

    ITimeline*  m_pITimeline;
    bool  m_bCreateITimeline;

    float   m_duration;
	float   m_fCurrentValue;

	TIMELINE_TIME_TYPE  m_eTimeType;
    StopWatch  m_stopWatch; // ��ʱ ��¼������ʼʱ��time tick����ʱ�䵽��m_nBeginTime+m_nDuretionʱ������������
    int   m_nFrameElapse; // ������֡��

	int   m_nId;
    int   m_nRepeatTimes; // 1��ʾ����һ�Σ�С�ڵ���0��ʾһֱ���ţ���--m_nRepeatTimes==0ʱֹͣ. 
    int   m_nFlags;
	LPARAM  m_pParam;
};
	
class  FromToTimeline : public Timeline
{
public:
	FromToTimeline();
	~FromToTimeline();
	IFromToTimeline*  GetIFromToTimeline();

public:
	void  SetParam(float from, float to, float t);
    void  SetEaseType(EaseType eType);
    void  SetCustomBezierEase(double p1x, double p1y, double p2x, double p2y);
    void  SetCustomTimingFuction(pfnTimingFunction f);

	virtual float OnTick(float fTimePercent) override;

private:
	IFromToTimeline*  m_pIFromToTimeline;
	
	float  m_from;
	float  m_to;

    // ���δָ����function����Ĭ�ϴ���ease_out����
	ITimingFunction*  m_pTimingFunction;
};

class KeyFrame
{
public:
	KeyFrame();
	~KeyFrame();

	float OnTick(float from, float fThisFrameTimePercent);

public:
	float  percent;
	float  to; 
	ITimingFunction*  pTimingFunction;
};

class KeyFrameTimeline : public Timeline
{
public:
	KeyFrameTimeline();
	~KeyFrameTimeline();

    IKeyFrameTimeline*  GetIKeyFrameTimeline();

	void  SetParam(float from, float t);
	void  AddKeyFrame(float percent, float to, EaseType eType);

    virtual float  OnTick(float fTimePercent) override;

private:
	IKeyFrameTimeline*  m_pIKeyFrameTimeline;

	float  m_from;
	vector<KeyFrame*>  m_keyFrames;  // ����û��ȥ��������
};


class StepTimeline : public Timeline
{
public:

};

// ʲôҲ������ֻ��Ϊ��ά�ֶ�����ʱ��
class IdleTimeline : public Timeline
{
public:
	virtual void  Init() {};
	virtual bool  OnTick() { return false; }
	virtual float  OnTick(float fTimePercent) { return 0; }
};

}
