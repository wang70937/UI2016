#pragma once
#include "..\wndtransmodebase.h"

namespace UI
{

class AlphaMaskWindowWrap : public WndTransModeBase
{
public:
    AlphaMaskWindowWrap();
    ~AlphaMaskWindowWrap();

    virtual WINDOW_TRANSPARENT_TYPE  GetType()
    {
        return WINDOW_TRANSPARENT_TYPE_MASKALPHA;
    }
    virtual void  UpdateRgn();
    virtual void  Init(ICustomWindow* pWnd);
	virtual void  Enable(bool b);

public:
    void   GetRgnData(BYTE* pScan0, int nPitch, RECT* prc, vector<POINT>& vPt);

private:
    C9Region  m_9region;
    byte  m_nAlphaMask;    //  ��Ҫ����͸�������alphaֵ��С�ڸ�alphaֵ�����򶼽����ٵ�
};

}