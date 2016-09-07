#pragma once
#include "Src\Util\Timeout\timeout.h"
#include "Inc\Interface\iuiinterface.h"

namespace UI
{
#define TOOLTIP_DELAY_SHOW_TIME    600
#define TOOLTIP_AUTO_HIDE_TIME    (GetDoubleClickTime()*10)
#define TOOLTIP_MAX_WIDTH  500/*256*/
	class UIApplication;

//////////////////////////////////////////////////////////////////////////
// TimerProc thunk

class ToolTipItem
{
public:
    ToolTipItem();

public:
    void  Set(TOOLTIPITEM* pItem);
    void  Reset();
    
public:
    String  strContent;

    IMessage*  pNotifyObj;   
    void*  pItemData;    

    int  nToolTipFlag; 
};

//
// ��ʾ���Ĺ���
// ���ڹ�����ּ�ʱ������Ϣת������ʵ�ָ����߼���ʹ��IToolTipUI�Ϳؼ�����ȥ����Щ�߼�
//
class ToolTipManager : public ITimeoutCallback
{
public:
	ToolTipManager();
	~ToolTipManager();

	void   Init(UIApplication* p, IToolTipUI* pTooltipUI=NULL);
	void   Release();
	bool   Hide();
	bool   Show(TOOLTIPITEM* pItemInfo);

protected:
    virtual void OnTimeout(long lId, WPARAM wParam, LPARAM lParam) override;

protected:
	IToolTipUI*        m_pToolTipUI;      // ���ǿؼ���tooltip
	ToolTipItem        m_tooltipItem;     // ��ǰ������ʾ�������ڵȴ���ʾ����ʾ����Ϣ
    UIApplication*     m_pUIApplication;

	enum TIMEOUT_TYPE
	{
		TIMEOUT_DELAY_SHOW,  // ��ʱ��ʾ
		TIMEOUT_AUTO_HIDE,   // ��ʾ����ʾʱ�䣬Ȼ���Զ�����
	};
    CTimeout   m_timeout;
};

}

