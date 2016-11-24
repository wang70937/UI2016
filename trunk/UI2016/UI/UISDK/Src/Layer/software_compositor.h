#pragma once
#include "compositor.h"
namespace UI
{

// ����ϳ���Ӳ���ϳɵ�����
// 1. Ӳ���ϳ��У�ÿ����������Ƕ���ģ������֮�䲻Ӱ�죬�������gpuͳһ�ϳɡ�
//    �����ȫ�У�һ�����ϵ����ݰ����������������������ݡ��ӽ��ˢ�£���Ӱ�����е����Ƚ�㡣
//
// 2. ������������ʱ�������Ⱦ�����Ͻ���ð�ݣ�������layer����������Ӧ��������
//    �����ڴ������»���ʱ�򣬾��ܸ��ݵ�ǰlayer�Ƿ����������������Ż��ˡ�û��������Ĳ㲻��Ҫ�ػ�
//
class SoftwareCompositor : public Compositor
{
public:
    virtual Layer*  virtualCreateLayer() override;
	virtual void  virtualBindHWND(HWND) override;
    virtual void  virtualCommit(const RectArray& arrDirtyInWindow) override;
	virtual void  UpdateDirty(__out_opt  RectArray& arrDirtyInWindow) override;
	virtual void  Resize(uint nWidth, uint nSize) override;

private:
    void  update_dirty_recursion(Layer* p);
    void  commit_recursion(Layer* p);
};
}