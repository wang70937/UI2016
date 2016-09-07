#ifndef _UI_IGIFIMAGE_H_
#define _UI_IGIFIMAGE_H_

#include "iimage.h"

namespace UI
{
interface  IGifImageRender;

// Gif�������Ƶķ�ʽ
enum Gif_Timer_Notify_Type
{
    Gif_Timer_Notify_Direct_Hwnd,
    Gif_Timer_Notify_Send_Msg,
};
struct Gif_Timer_Notify
{
    Gif_Timer_Notify_Type  eType;
    union
    {
        struct  // ֱ����ʾ�ڴ��ڵ�ĳ��λ��(δ���ǲü�)
        {
            HWND   hWnd;
            HDC    hDC;
            int    x;
            int    y;
        }notify_hwnd;

        struct  // ������Ϣ�������̣߳�֪ͨ�����Լ�����
        {
            IMessage*  pNotifyMsgObj;
            LPARAM     lParam;

        }notify_ui_msg;
    };
};

class GifImageBase;
interface UISDKAPI IGifImage
{
public:
    IGifImage(GifImageBase* p);
    IGifImage(IUIApplication* pUIApp);
    ~IGifImage();

    void  CreateGifImpl(IUIApplication* pUIApp);
    void  CreatePnglistGifImpl();
    GifImageBase*  GetImpl();
    void  SetImpl(GifImageBase* p);

public:
    bool  Load(const TCHAR* szPath, IMapAttribute* pMapAttrib=NULL);
    bool  Destroy();

    IGifImageRender*  AddRender(Gif_Timer_Notify* pNotify, IUIApplication*  pUIApp, int* pIndex = NULL);
    bool  ModifyRender(Gif_Timer_Notify* pNotify, int nIndex=-1);
    bool  DeleteRender(int nIndex=-1);

    bool  SetTransparentColor(COLORREF colTransparent = GetSysColor(COLOR_BTNFACE));
	ImageWrap*  GetFrameIImage( int nIndex );

private:
    GifImageBase*  m_pImpl;
    bool  m_bCreateGifImageImpl;
};

//
//	GIF����״̬
//	
enum GIF_DRAW_STATUS
{
    GIF_DRAW_STATUS_START,
    GIF_DRAW_STATUS_STOP,
    GIF_DRAW_STATUS_PAUSE
};

interface  IGifImageRender
{
    virtual void  Release() = 0;

    virtual void  Start() = 0;
    virtual void  Pause() = 0;
    virtual void  Stop() = 0;

    virtual int  GetWidth() = 0;
    virtual int  GetHeight() = 0;
    virtual void  OnPaint(HDC hDC, int x, int y) = 0;
    virtual void  OnAlphaPaint(HDC hDC, int x, int y) = 0;
    virtual GIF_DRAW_STATUS GetStatus() = 0;
	virtual IGifImage*  GetIGifImage() = 0;
};

}

#endif 