#ifndef _UI_IANIMATE_H_
#define _UI_IANIMATE_H_

namespace UIA
{
#define ANIMATEBASE_API UIAPI

enum TIMELINE_TIME_TYPE
{
	TT_BY_MS,
	TT_BY_SECOND,
	TT_BY_FRAME,
};

enum EaseType
{
	ease_unknown = -1,

	// cubic
	ease = 0,     // default, �𽥱���
	linear,       // ����
	ease_in,      // ����
	ease_out,     // ����
	ease_in_out,  // ����Ȼ�����

	// sine
	ease_in_sine,
	ease_out_sine,
	ease_in_out_sine, 

	// quad
	ease_in_quad,
	ease_out_quad,
	ease_in_out_quad,

	// quart
	ease_in_quart,
	ease_out_quart,
	ease_in_out_quart,

	// quint
	ease_in_quint,
	ease_out_quint,
	ease_in_out_quint,

	// expo
	ease_in_expo,
	ease_out_expo,
	ease_in_out_expo,

	// circ
	ease_in_circ,
	ease_out_circ,
	ease_in_out_circ,

	// back
	ease_in_back,      // �Ȼ��ˣ��ټ���
	ease_out_back,     // �ȼ��ٳ������ٻ���
	ease_in_out_back,  // �³��� + �ϳ��� 

	ease_bezier_support = ease_in_out_back,   // Ŀǰbezierֻ��֧�ֵ��������Ĳ���easing����ģ��

	// elastic
	ease_in_elastic,
	ease_out_elastic,
	ease_in_out_elastic,

	// bounce
	ease_in_bounce,
	ease_out_bounce,
	ease_in_out_bounce,

	// bezier_custom
	ease_bezier_custom,

	// function custom
	ease_function_custom,

	ease_end,
};

// enum SLIDE_ANIMATE_FLAG
// {
// 	SLIDE_LEFT2RIGHT,
// 	SLIDE_TOP2BOTTOM,
// 	SLIDE_RIGHT2LEFT,
// 	SLIDE_BOTTOM2TOP,
// };
// enum E_WINDOW_ANIMATE_TYPE
// {
// 	E_WINDOW_ANIMATE_3D_ROTATE,
// 	E_WINDOW_ANIMATE_UPDOWNALPHASHOW,
// 	E_WINDOW_ANIMATE_ALPHASHOW,
// };
// enum E_CONTROL_ANIMATE_TYPE
// {
// 	E_CONTROL_ANIMATE_SLIDE,
// };


//////////////////////////////////////////////////////////////////////////
//
//     �����ӿ�
//
//////////////////////////////////////////////////////////////////////////


class Timeline;
interface ANIMATEBASE_API ITimeline
{
public:
	ITimeline(Timeline* p);

	void  SetRepeateCount(int n);
	void  SetTimeType(TIMELINE_TIME_TYPE eType);
	void  SetAutoReverse(bool b);
	bool  IsFinish();
	void  SetFinish();
	float  GetCurrentValue();
	int   GetCurrentIntValue();
	void  SetId(int nID);
	int   GetId();
	void  SetUserData(LPARAM lParam);
	LPARAM  GetUserData();
	void  SetCustomBezierArgs(double, double, double, double);

private:
	Timeline*  m_pTimelineImpl;
};

interface ITimingFunction 
{
	// in: ��ǰʱ�����
	// out: ��ǰλ�ƽ���
	virtual float OnTick(float fTimePercent) = 0;
};
typedef float (__stdcall *pfnTimingFunction)(float fTimePercent);


class FromToTimeline;
interface ANIMATEBASE_API IFromToTimeline : public ITimeline
{
	IFromToTimeline(FromToTimeline* p);

	IFromToTimeline*  SetParam(float from, float to, float t);

	IFromToTimeline*  SetEaseType(EaseType eType);
	IFromToTimeline*  SetCustomBezierEase(
		    double p1x, double p1y, double p2x, double p2y);
	IFromToTimeline*  SetCustomTimingFuction(pfnTimingFunction f);

private:
	FromToTimeline*  m_pFromToTimelineImpl;
};

class KeyFrameTimeline;
interface ANIMATEBASE_API IKeyFrameTimeline : public ITimeline
{
	IKeyFrameTimeline(KeyFrameTimeline* p);

	IKeyFrameTimeline*  SetParam(float from, float t);
	IKeyFrameTimeline*  AddKeyFrame(float percent, float to, EaseType eType);

private:
	KeyFrameTimeline*  m_pKeyFrameTimelineImpl;
};


interface IAnimateEventCallback;
class Storyboard;
interface ANIMATEBASE_API IStoryboard
{
public:
	IStoryboard(Storyboard* p);
	Storyboard* GetImpl();

	void  SetEventListener(IAnimateEventCallback*);
	void  SetId(int nID);
	int   GetId();
	void  SetWParam(WPARAM wParam);
	WPARAM  GetWParam();
	void  SetLParam(LPARAM lParam);
	LPARAM  GetLParam();

	ITimeline*  CreateIdleTimeline(int nTimelineId);
	IFromToTimeline*  CreateTimeline(int nId);
	IKeyFrameTimeline*  CreateKeyFrameTimeline(int nId);

	ITimeline*  GetTimeline(unsigned int nIndex);
	ITimeline*  FindTimeline(int nTimelineId);

	bool  IsFinish();
    void  Cancel();

	void  Begin();
	void  BeginBlock();
	void  BeginDelay(long lElapse);
private:
	Storyboard*  m_pStoryboardImpl;
};

interface IAnimateEventCallback;
class AnimateManager;

interface ANIMATEBASE_API IAnimateManager
{
	IAnimateManager(AnimateManager* p);
	AnimateManager*  GetImpl();

    void  OnTick();
	int   SetFps(int n);

	void  ClearStoryboardByNotify(IAnimateEventCallback*);
	void  RemoveStoryboard(IStoryboard* p);
    void  RemoveStoryboardByNotityAndId(IAnimateEventCallback*, int nId);

	IStoryboard*  CreateStoryboard(IAnimateEventCallback* pNotify, int nId = 0, WPARAM wParam = 0, LPARAM lParam = 0);

private:
	AnimateManager*  m_pImpl;
}; 

enum E_ANIMATE_END_REASON
{
	ANIMATE_END_NORMAL,  // ��������
	ANIMATE_END_CANCEL,  // ���¶���ȡ����
	ANIMATE_END_DISCARD, // �����˳���ǿ�ƽ���
};
enum E_ANIMATE_TICK_RESULT
{  
    ANIMATE_TICK_RESULT_CONTINUE, // ��������
    ANIMATE_TICK_RESULT_CANCEL,   // ����ȡ������������
};
interface IAnimateEventCallback
{
	virtual void  OnAnimateStart(IStoryboard*) {};
	virtual void  OnAnimateEnd(IStoryboard*, E_ANIMATE_END_REASON e) {};
	virtual E_ANIMATE_TICK_RESULT  OnAnimateTick(IStoryboard*) { return ANIMATE_TICK_RESULT_CONTINUE; };
	virtual void  OnAnimateRepeat(IStoryboard*) {};
	virtual void  OnAnimateReverse(IStoryboard*) {};
};

interface IAnimateTimerCallback
{
	virtual void  OnSetTimer(HANDLE) = 0;
	virtual void  OnKillTimer(HANDLE) = 0;
};

extern "C"
{
    void  ANIMATEBASE_API CreateAnimateManager(
                                IAnimateTimerCallback* p, 
                                IAnimateManager** pp);
    void  ANIMATEBASE_API DestroyAnimateManager(
                                IAnimateManager* p);
}

}

#endif // _UI_IANIMATE_H_