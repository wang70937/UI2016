#pragma once
#include "button.h"

class SystemButton;
struct ISystemButton : public IButton
{
    UI_DECLARE_INTERFACE(SystemButton);
};

//
//  �������ϵ�ϵͳ��ť������ģ����С������󻯡��رյĹ���
// 
class SystemButton : public Button
{
public:
    SystemButton(ISystemButton*);

    UI_BEGIN_MSG_MAP()
        UIMSG_SERIALIZE(OnSerialize)
        UIMSG_INITIALIZE2(OnInitialize2)
		UIMSG_FINALRELEASE(FinalRelease)
    UI_END_MSG_MAP_CHAIN_PARENT(Button)

protected:
    void  OnSerialize(SERIALIZEDATA* pData);
    void  OnInitialize2();
	void  FinalRelease();

    virtual void virtualOnClicked() override;

	void  on_window_size(long type);

private:
    SYSTEM_BUTTON_TYPE  m_eButtonType;
};