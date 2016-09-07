#ifndef _UI_IRENDERBASE_H_
#define _UI_IRENDERBASE_H_

#include <uxtheme.h>

namespace UI
{
    // ע��libo 20120927
    //     ÿ�ι��render����һ����ֵ�������޷�ֻ���������ֵ��ͼƬ����������ƥ�䣬�޷���������ж�
    //     ��˾�����չDrawState��������λΪͼƬ����ֵ����λΪ״ֵ̬

#define RENDER_STATE_MASK     0xFFFF0000
#define RENDER_STATE_NORMAL   0x00010000
#define RENDER_STATE_HOVER    0x00020000
#define RENDER_STATE_PRESS    0x00040000
#define RENDER_STATE_DISABLE  0x00080000
#define RENDER_STATE_SELECTED 0x00100000
#define RENDER_STATE_DEFAULT  0x00200000
#define RENDER_STATE_READONLY 0x00400000
#define RENDER_STATE_NOTFOCUS 0x00800000  // ��ѡ���ˣ���ʧȥ�˽���


const UINT  LISTCTRLITEM_FOREGND_RENDER_STATE_NORMAL  = RENDER_STATE_NORMAL|0;
const UINT  LISTCTRLITEM_FOREGND_RENDER_STATE_HOVER   = RENDER_STATE_HOVER|1;
const UINT  LISTCTRLITEM_FOREGND_RENDER_STATE_PRESS   = RENDER_STATE_PRESS|2;
const UINT  LISTCTRLITEM_FOREGND_RENDER_STATE_DISABLE = RENDER_STATE_DISABLE|3;
const UINT  LISTCTRLITEM_FOREGND_RENDER_STATE_SELECTED_NORMAL = RENDER_STATE_NORMAL|RENDER_STATE_SELECTED|4;
const UINT  LISTCTRLITEM_FOREGND_RENDER_STATE_SELECTED_HOVER  = RENDER_STATE_HOVER|RENDER_STATE_SELECTED|5;
const UINT  LISTCTRLITEM_FOREGND_RENDER_STATE_SELECTED_PRESS  = RENDER_STATE_PRESS|RENDER_STATE_SELECTED|6;
const UINT  LISTCTRLITEM_FOREGND_RENDER_STATE_SELECTED_DISABLE = RENDER_STATE_DISABLE|RENDER_STATE_SELECTED|7;
const UINT  LISTCTRLITEM_FOREGND_RENDER_STATE_SELECTED_NOT_FOCUS = RENDER_STATE_SELECTED|RENDER_STATE_NOTFOCUS|8;

const UINT  LISTCTRLITEM_FOCUS_RENDER_STATE_NORMAL = RENDER_STATE_NORMAL|0;
const UINT  LISTCTRLITEM_FOCUS_RENDER_STATE_CTRLNOFOCUS = RENDER_STATE_NOTFOCUS|1;


enum RENDER_TYPE
{
    RENDER_TYPE_NULL,             
    RENDER_TYPE_COLOR,       
    RENDER_TYPE_THEME_SYSCOLOR,
    RENDER_TYPE_GRADIENT,    
    RENDER_TYPE_COLORLIST,

    RENDER_TYPE_IMAGE,  
    RENDER_TYPE_IMAGELIST,
    RENDER_TYPE_IMAGELISTITEM,

    //	RENDER_TYPE_NOTHEME,    // ǿ��ʹ��ϵͳ��������ʽ����ʽ��win2000��ʽ����Ŀǰ��GroupBox֧��
    RENDER_TYPE_THEME_FIRST = 1000,
    //	RENDER_TYPE_THEME,
    RENDER_TYPE_THEME_TOOLTIP_WINDOW_BKGND,

    RENDER_TYPE_THEME_LAST = 9999,
};

class RenderBase;
interface UISDKAPI IRenderBase : public IMessage
{
    void  CheckThemeChanged();
    bool  IsThemeRender();

    void  SetObject(IUIApplication*  pUIApp, IObject* pObject);
    IObject*  GetObject();

    void  SetType(RENDER_TYPE nType);
    RENDER_TYPE  GetType();

    IColorRes*  GetSkinColorRes();
    IImageRes*  GetSkinImageRes();

    void  Serialize(SERIALIZEDATA* pData);
    void  DrawState(IRenderTarget*, LPCRECT prc, int nState);
    SIZE  GetDesiredSize();
    void  Init();

	UI_DECLARE_INTERFACE(RenderBase);
};


// -- 2015.4.1���ڣ�����ʹ��
class NullRender;
interface __declspec(uuid("4A0A8C42-CA22-4BD4-8875-C58FB8FC2788"))
INullRender : public IRenderBase
{
    UI_DECLARE_INTERFACE(NullRender);
};
//////////////////////////////////////////////////////////////////////////

}

#endif // _UI_IRENDERBASE_H_