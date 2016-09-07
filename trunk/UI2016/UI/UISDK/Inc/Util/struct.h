#ifndef _UI_STRUCT_H_
#define _UI_STRUCT_H_

interface IDataObject;

namespace UI
{
//
// 9������������
//
class  UISDKAPI C9Region
{
public:
    C9Region(){ Set(0); }
    C9Region(const C9Region& o)
    {
		this->Copy(o);
    }
	bool IsEqual( const C9Region& r );
	void Copy( const C9Region& r );

	void Set( short w );
	void Set( short wLeftRight, short wTopBottom );
	void Set( short wLeft, short wTop, short wRight, short wBottom );
    bool IsAll_0();
    bool IsAll_1();
	bool IsSimpleValue();
	bool IsSimpleRect();
	
    
	short topleft;
	short top;
	short topright;
	short left;
	short right;
	short bottomleft;
	short bottom;
	short bottomright;
};

interface IRenderTarget;
struct RENDERBASE_DRAWSTATE
{
    IRenderTarget*  pRenderTarget;
    RECT  rc;
    int  nState;
};

struct TEXTRENDERBASE_DRAWSTATE
{
    RENDERBASE_DRAWSTATE  ds_renderbase;
    LPCTSTR  szText;
    int  nDrawTextFlag;
};

enum
{
    TOOLTIP_ACTION_FLAG_NORMAL = 0,              // ����ϵͳ��ʾ���������ؼ�ʱ����ʱ��ʾ���ƿ��ؼ�ʱ����������
};

interface IMessage;
// ShowToolTip�Ĳ���
struct TOOLTIPITEM
{
    // Ҫ��ʾ�����ݣ������ֵ��Ч�������pNotifyObj/pItemData��
    LPCTSTR  szToolTipContent;

    // Ҫ��ʾ��ʾ���Ŀؼ����Լ���Ϣ֪ͨ�Ķ���
    IMessage*  pNotifyObj;   

    // �����Ҫ��ʾ�ؼ��ڲ�һ��item����ʾ��Ϣ���ɽ���ֵ����Ϊ���ʵ�ֵ��
    // Ȼ��mgr����pNotifyObj����UI_WM_GET_TOOLTIPINFO��Ϣ
    void*  pItemData;    

    // ��ʾ������Ϊ��־�� ��20120928 ����ֻ֧��normal��
    int   nToolTipFlag; 
};


class TimerItem;
typedef void  (*UITimerProc)(UINT_PTR nTimerID, TimerItem* pTimerItem);

interface IMessage;
class TimerItem
{
public:
    int  nRepeatCount;   // ��Ӧ����,-1��ʾ����
    IMessage* pNotify;    // ��ӦWM_TIMER�Ķ���
    UITimerProc  pProc;
    int  nId;
    WPARAM   wParam;
    LPARAM   lParam; 

    TimerItem()
    {
        nRepeatCount = -1;
        pNotify = NULL;
        pProc = NULL;
        wParam = 0;
        lParam = 0;
        nId = 0;
    }
};


// UI_WM_DROPTARGETEVENT ��Ϣ����
enum DROPTARGETEVENT_TYPE
{
	_DragEnter,
	_DragOver,
	_DragLeave,
	_Drop,
};
interface IDragFeedback;
struct DROPTARGETEVENT_DATA
{
	IDataObject*  pDataObj;
	DWORD         grfKeyState;
	POINTL        pt;
	DWORD*        pdwEffect;
	IDragFeedback*  pDragFeedback;
};

}
#endif // _UI_STRUCT_H_