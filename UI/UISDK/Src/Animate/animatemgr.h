#pragma once

namespace UIA
{
// http://msdn.microsoft.com/zh-cn/magazine/cc163397.aspx  ��չ WPF ������
// http://www.cnblogs.com/Jax/archive/2008/03/26/1123778.html ��Programming WPF������ ��8�� ����

// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.doubleanimationbase_members(v=vs.85).aspx  DoubleAnimationBase Members

// http://gamebabyrocksun.blog.163.com/blog/static/571534632010219544464/  ��Ϸ����ѭ������

/*  motion blur<<�˶�ģ��>>

�� ��Ϸ��Ϊʲô��Ҫģ����������һ�㲻�Ǹ�����

	Ϊʲôÿ��24֡���ٶȶ��ڵ�Ӱ��˵�Ѿ����Ի�ú��������Ӿ�Ч������
	���ڵ�����Ϸ��˵ȴ���ԵÿĿ������أ�ԭ��ܼ򵥣�������ڹ�����ʱ
	�򲢷�һ֡һ֡��ֹ�����㣬�������µ�ÿһ֡�Ѿ�������1/24�����ڵ�
	�����Ӿ���Ϣ�����������λ�ơ�����ڿ�¼���ʱ������ͣ��������
	�������Ĳ�����һ�������ľ�ֹ���棬����һ��ģ����ͼ��ԭ�������
	�ˡ�������������һ�㣬��Ϸ���ÿһ֡����һ����ֹ���棬���������
	���Ĺ�����ץһ��ͼƬ�������õ��Ŀ϶���һ�������ľ�̬ͼ���˶�ģ��
	������Ŀ�ľ�������ǿ�����ƶ���������ʵ�У���һ��������������֮֡
	���������λ����Ϣ�����ǽ���ǰ֡ͬǰһ֡�����һ������õ�һ��
	Ч����
*/

// 2015.10.18
// WaitableTimer + SleepEx/WaitForSingleObject + while(1)���ʱ������
// FPS��ʱ���ֻ��60֡����,����ʱ���ܹ�������
// ����Է��֣���chrome�����ʱ��fps����ȥ���ر�chromeʱ��fpsֻ����60
// 2016.3.10
// ʹ��timeBeginPeriod�ɻָ����ȡ�

class Storyboard;
interface IAnimateManager;
interface IAnimateEventCallback;
interface IAnimateTimerCallback;

class AnimateManager
{
public:
	AnimateManager();
	~AnimateManager();

	void  AddStoryboard(Storyboard* p);
    void  AddStoryboardBlock(Storyboard* p);
	void  ClearStoryboardOfNotify(IAnimateEventCallback*);
    void  RemoveStoryboardByNotityAndId(IAnimateEventCallback* pCallback, int nId);
	void  RemoveStoryboard(Storyboard* p);
    void  CancelStoryboard(Storyboard* p);
	
    Storyboard*  CreateStoryboard(
            IAnimateEventCallback* pNotify, 
            int nId = 0, WPARAM wParam = 0, LPARAM lParam = 0);

    IAnimateManager*  GetIAnimateManager();
    void  SetTimerCallback(IAnimateTimerCallback*);

public:
	int     GetFps() { return m_nFps; }
	int     SetFps(int n);

	void    OnTick();

protected:
    void  Destroy();

	void  SetTimer();
	void  KillTimer();

protected:
    IAnimateManager*  m_pIAnimateManager;
    IAnimateTimerCallback*  m_pITimerCallback;

	LARGE_INTEGER     m_liPerFreq;     // ����֡������

	HANDLE m_hTimer;
	int    m_nFps;
    bool   m_bTimerStart;
	bool   m_bHandlingTimerCallback;   // �����ڱ���stl�������ⲿ��ɾ��һ�����󣬵��±���

	list<Storyboard*>  m_listStoryboard;
    HMODULE  m_hModuleWinmm;
};
}

