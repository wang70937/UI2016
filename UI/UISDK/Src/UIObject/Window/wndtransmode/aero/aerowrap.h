#pragma once
#include "../wndtransmodebase.h"
#include "Src\Util\dwm\dwmhelper.h"

//
// 2014.5.22 16:12 
//
// ������û��WS_CAPTION��ʽ������£�ʹ��TRANSPARENTģʽ��aero͸������Ҫ�������¼��������޷����:
// 1. ���ʱ��͸��Ч����ʧ���������ȫ��
// 2. �����϶����ڴ�Сʱ�������л��ʳ��ֺ�ɫ�����������˸
// 3. �л�ϵͳ��ʽ����ʱ�ᵼ�´��ڳ��ֺ�Ũ����Ӱ�޷���ʧ��
//
// (���������⣬����WS_CAPTION��ʽʱ������; 
//  ������û��WS_CAPTION����WM_THICKFRAME����������WM_NCCALCSIZE��Ϣ�������Ҳ�Ǻõģ�
//  �����ޱ߿�Ĵ��ڻ�����blurģʽȥ�����)
//  

namespace UI
{

class AeroWindowWrap: public WndTransModeBase
{
public:
    AeroWindowWrap();
    ~AeroWindowWrap();

    VIRTUAL_BEGIN_MSG_MAP(LayeredWindowWrap)
#if(_WIN32_WINNT >= 0x0600)
        MESSAGE_HANDLER(WM_DWMCOMPOSITIONCHANGED, _OnDwmCompositionChanged)
#endif
        CHAIN_MSG_MAP_MEMBER_P(m_pAeroDisableMode);
    END_MSG_MAP()

	UI_BEGIN_MSG_MAP()
        UICHAIN_MSG_MAP_POINT_MEMBER(m_pAeroDisableMode)
        UIMSG_QUERYINTERFACE(AeroWindowWrap)
	UI_END_MSG_MAP()


    LRESULT  _OnDwmCompositionChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//	LRESULT  _OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    virtual WINDOW_TRANSPARENT_TYPE  GetType();
    virtual bool  RequireAlphaChannel();
    virtual void  UpdateRgn();
    virtual void  Enable(bool b);
    virtual bool  Commit();
    virtual void  Init(ICustomWindow* pWnd);

    IAeroWindowWrap*  GetIAeroWndTransMode();
    AERO_MODE  GetAeroMode();
    void  GetBlurRegion(CRegion4* pregion);
	void  SetTransparentMargins(RECT* prcMargin);

public:

    AERO_MODE  m_eMode;
    union
    {
        struct TransparentMode
        {
            MARGINS  m_margins;
        }trans;

        struct BlurMode
        {
            RECT  m_regionBlur;  // ָ�����ܲ�����blur������
            HRGN  m_hrgnBlurRgn;  // <-- �ⲿ����
            bool  m_bAutoClipChildCtrl;  // �Զ����õ��ӿؼ�
        }blur;
    };    

    DwmHelper*  m_pDWM;
    WINDOW_TRANSPARENT_TYPE  m_eAeroDisableMode;
    IWindowTransparent*  m_pAeroDisableMode;

    IAeroWindowWrap*  m_pIAeroWindowWrap;
};


}