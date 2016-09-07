#ifndef _UIDEFINE_H_
#define _UIDEFINE_H_

// ���뵼������
#ifdef UISDK_EXPORTS
#define UISDKAPI __declspec(dllexport)
#else
#define UISDKAPI __declspec(dllimport)
#endif

namespace UI
{

	// ���⺬�峣��
	enum
	{
		NDEF = -1,    // û�ж���ı���
		AUTO = -2,    // �Զ���������width/height=auto��
	};

	enum E_BOOL_CREATE_IMPL
	{
		CREATE_IMPL_FALSE = 0,
		CREATE_IMPL_TRUE = 1,
	};

#define EMPTYTEXT TEXT("")

#define  __pImpl static_cast<ImplName*>(m_pImpl)

#define  UI_DECLARE_INTERFACE(T)                            \
public:                                                     \
	typedef T ImplName;                                     \
	static I##T* CreateInstance(UI::ISkinRes* pSkinRes);    \
	I##T(E_BOOL_CREATE_IMPL);                               \
    BOOL  nvProcessMessage(UI::UIMSG* pMsg, int nMsgMapID, bool bDoHook); \
	T*  GetImpl();                                          
	

// ��ģ��ʱ����Ϊ��ģ��ĸ�������Ҫ���ӳ�Ա���������ٸñ���
#define  UI_DECLARE_INTERFACE_ACROSSMODULE(T)               \
	UI_DECLARE_INTERFACE(T)                                 \
protected:                                                  \
	virtual ~I##T();                                        \
	T*  m_pImpl;


// �������operator new��ԭ��
//   �������m_pImpl = new T(this);
//   ��ô��T���캯���е���Ixxx��ķ���ʱ��Ixxx��m_pImpl��û��
//   ����ֵ����˽��ò����ֽ��������malloc + construct
#define  UI_IMPLEMENT_INTERFACE(T, SUPER)                   \
	I##T::I##T(E_BOOL_CREATE_IMPL b) : I##SUPER(CREATE_IMPL_FALSE)  \
	{                                                       \
        if (b)                                              \
        {                                                   \
            m_pImpl = (T*)operator new (sizeof(T));         \
            new(m_pImpl) T(this);                           \
        }                                                   \
        else                                                \
        {                                                   \
			m_pImpl = NULL;                                 \
        }                                                   \
	}                                                       \
	T*  I##T::GetImpl()                                     \
	{                                                       \
		return static_cast<T*>(m_pImpl);                    \
	}                                                       \
	I##T* I##T::CreateInstance(ISkinRes* pSkinRes)          \
	{                                                       \
		return ObjectCreator<I##T>::CreateInstance(pSkinRes); \
	}                                                       \
    BOOL  I##T::nvProcessMessage(UI::UIMSG* pMsg, int nMsgMapID, bool bDoHook) \
    {                                                       \
        return __pImpl->nvProcessMessage(pMsg, nMsgMapID, bDoHook); \
    }       


#define  UI_IMPLEMENT_INTERFACE_ACROSSMODULE(T, SUPER)      \
	I##T::~I##T()                                           \
	{                                                       \
		if (m_pImpl)                                        \
        {                                                   \
			delete m_pImpl;                                 \
            m_pImpl = NULL;                                 \
        }                                                   \
	}                                                       \
	I##T::I##T(E_BOOL_CREATE_IMPL b) : I##SUPER(CREATE_IMPL_TRUE)   \
	{                                                       \
        if (b)                                              \
        {                                                   \
            m_pImpl = (T*)operator new (sizeof(T));         \
            new(m_pImpl) T(this);                           \
        }                                                   \
        else                                                \
        {                                                   \
			m_pImpl = NULL;                                 \
        }                                                   \
	}                                                       \
	T*  I##T::GetImpl()                                     \
	{                                                       \
		return static_cast<T*>(m_pImpl);                    \
	}                                                       \
	I##T* I##T::CreateInstance(UI::ISkinRes* pSkinRes)          \
	{                                                       \
		return UI::ObjectCreator<I##T>::CreateInstance(pSkinRes); \
	}                                                       \
    BOOL  I##T::nvProcessMessage(UI::UIMSG* pMsg, int nMsgMapID, bool bDoHook) \
    {                                                       \
        return __pImpl->nvProcessMessage(pMsg, nMsgMapID, bDoHook); \
    }  

	// TODO: �Ż�������ʽ��
#define DO_PARENT_PROCESS(IMyInterface, IParentInterface) \
    static_cast<IParentInterface*>(m_p##IMyInterface)->nvProcessMessage(GetCurMsg(), 0, 0); \
    SetMsgHandled(TRUE)

	// ��ȡ�����Ƿ���
#define DO_PARENT_PROCESS2(IMyInterface, IParentInterface, bHandled) \
	bHandled = static_cast<IParentInterface*>(m_p##IMyInterface)->nvProcessMessage(GetCurMsg(), 0, 0); \
	SetMsgHandled(TRUE)
    
// ��ȡ����ֵ
#define DO_PARENT_PROCESS3(IMyInterface, IParentInterface) \
	(static_cast<IParentInterface*>(m_p##IMyInterface)->nvProcessMessage(GetCurMsg(), 0, 0), \
	SetMsgHandled(TRUE), GetCurMsg()->lRet);

#define DO_PARENT_PROCESS_MAPID(IMyInterface, IParentInterface, MsgMapId) \
    static_cast<IParentInterface*>(m_p##IMyInterface)->nvProcessMessage(GetCurMsg(), MsgMapId, 0); \
    SetMsgHandled(TRUE)

#ifndef interface
#define interface struct
#endif

typedef RECT  REGION4;
inline  int RECTW(LPCRECT prc) { return prc->right - prc->left; }
inline  int RECTH(LPCRECT prc) { return prc->bottom - prc->top; }
inline  int RECTW(RECT& rc)  { return rc.right - rc.left; }
inline  int RECTH(RECT& rc)  { return rc.bottom - rc.top; }

#define RGBA(r,g,b,a)  (((BYTE)(r))|((WORD)(((BYTE)(g))<<8))|(DWORD(((BYTE)(b))<<16))|((DWORD)(((BYTE)(a))<<24)))


//
//	release�汾�¶����������������debugview.exe�������Դ���
//  remark: __FILE__ ...��Щ����char*���͵�
//
#ifdef ASSERT
#undef ASSERT  // ����ʹ��ASSERT����ʹ��UIASSERT
#endif

#ifdef _DEBUG
#define UIASSERT(x)   \
	if(!(x))                          \
	{                                 \
		char szInfo[2048] = "";       \
		sprintf_s(szInfo, 2047, "UIASSERT expr(%s) %s(L%d) ## %s\r\n", #x, __FILE__, __LINE__, __FUNCTION__ );  \
		::OutputDebugStringA(szInfo); \
		DebugBreak(); \
	} 
#else
#define UIASSERT(x)    
#endif

// �Ƿ�����ֵ 
enum HANDLED_VALUE
{
	NOT_HANDLED = 0,
	HANDLED = 1,
};

class IRootInterface
{
public:
	virtual ~IRootInterface() = 0 {};  // ȷ��deleteʱ�ܵ��õ����������������
};


// �������л���Ϣ������ȡ��WM_SETATTRIBUTE
enum SERIALIZEFLAG
{
	SERIALIZEFLAG_LOAD = 0x01,
	SERIALIZEFLAG_RELOAD = 0x03,   // reloadҲ��load
	SERIALIZEFLAG_SAVE = 0x04,
	SERIALIZEFLAG_EDITOR = 0x08,  // ��ȡ�����б����ʾ��Ϣ

	// load ��ʶ
	SERIALIZEFLAG_LOAD_ERASEATTR = 0x0200,  // ��ȡ�����Ժ󣬽������Դ�mapattr��ɾ��

	// save ��ʶ
};
interface IMapAttribute;
interface IListAttribute;
interface IAttributeEditorProxy;
interface IUIApplication;
interface ISkinRes;

struct SERIALIZEDATA
{
	union
	{
		IMapAttribute*  pMapAttrib;    // load [in] / getlist [out]
		IListAttribute*  pListAttrib;  // save [out]
		IAttributeEditorProxy*  pAttributeEditorProxy;  // editor [in]
	};

	IUIApplication*  pUIApplication; // TODO: �����ñ�����ֻʹ��pSkinRes
	ISkinRes*  pSkinRes;
	LPCTSTR  szPrefix;      // ����ǰ׺
	LPCTSTR  szParentKey;   // �����ԣ�������editor������bkg.render.type
	UINT  nFlags;

	bool  IsReload() { return ((nFlags&SERIALIZEFLAG_RELOAD) == SERIALIZEFLAG_RELOAD) ? true : false; }
	bool  IsLoad() { return ((nFlags&SERIALIZEFLAG_LOAD) == SERIALIZEFLAG_LOAD) ? true : false; }
	bool  IsSave() { return ((nFlags&SERIALIZEFLAG_SAVE) == SERIALIZEFLAG_SAVE) ? true : false; }
	bool  IsEditor() { return ((nFlags&SERIALIZEFLAG_EDITOR) == SERIALIZEFLAG_EDITOR) ? true : false; }
	bool  NeedErase() { return (nFlags&SERIALIZEFLAG_LOAD_ERASEATTR) ? true : false; }
	void  SetErase(bool b) { if (b) { nFlags |= SERIALIZEFLAG_LOAD_ERASEATTR; } else { nFlags &= ~SERIALIZEFLAG_LOAD_ERASEATTR; } }
};


// object��state bit��״̬λ
enum OBJECT_STATE_BIT
{
	OSB_UNVISIBLE = 0x0001,        // �Լ��Ƿ�ɼ�
	OSB_COLLAPSED = 0x0002,        // listitemΪ������������ʾ�ӽ�㣨TODO:�Ժ��������չΪ���ز��Ҳ�ռ�ò���?��
	OSB_DISABLE = 0x0004,        // �Լ��Ƿ����

	OSB_PRESS = 0x0010,
	OSB_HOVER = 0x0020,
	OSB_FORCEPRESS = 0x0040,
	OSB_FORCEHOVER = 0x0080,
	OSB_READONLY = 0x0100,       // �����ɸ����ؼ��Լ�ʵ�ָ���һЩ��
	OSB_FOCUS = 0x0200,
	OSB_DEFAULT = 0x0400,
	OSB_SELECTED = 0x0800,        // ��ѡ��
	OSB_CHECKED = 0x1000,
	OSB_RADIOCHECKED = 0x2000,
	OSB_DRAGDROPHOVER = 0x4000,       // ��ק�����У�λ������¡�Ŀǰ��listitemʹ��
	OSB_DRAGING = 0x8000,       // ���ڱ���ק

	WSB_ACTIVE = 0x1000,        // ����Ϊactive
};

#define SWP_LAYEREDWINDOW_SIZEMOVE  0x80000000   // ��ʾ���Ǹ��ֲ㴰��ģ��Ĵ��ڴ�С�ı�
#define SWP_NOUPDATELAYOUTPOS       0x40000000   // ����SetObjectPosʱ�����²�������
#define SWP_FORCESENDSIZEMSG        0x20000000   // ��ʹ��Сû�иı䣬Ҳǿ�Ʒ���һ��WM_SIZE��Ϣ��������ͨ�߼�
#define SWP_FORCEUPDATEOBJECT       0x10000000   // ��ʹ��Сû�иı䣬Ҳǿ�������أ�������ͨ�߼�



#define UI_DECLARE_RENDERBASE(className, xml, rendertype)   \
    static LPCTSTR  GetXmlName() { return xml; }       \
    static int  GetType() { return rendertype; }      


// ���궨����Ҫ������theme���͵�renderbase��Ҫ���ݿؼ����ͽ��д���
#define UI_DECLARE_RENDERBASE2(className, xml, rendertype) \
    static LPCTSTR  GetXmlName() { return xml; }       \
    static int  GetType() { return rendertype; }      \


#define UI_DECLARE_TEXTRENDERBASE(className, xml, rendertype) \
    static LPCTSTR  GetXmlName() { return xml; }       \
    static int  GetType() { return rendertype; }      

#define UI_DECLARE_TEXTRENDERBASE2(className, xml, rendertype) \
    static LPCTSTR  GetXmlName() { return xml; }       \
    static int  GetType() { return rendertype; }      \



#define TESTWNDSTYLE(hWnd, flag) \
    GetWindowLong(hWnd, GWL_STYLE)&flag
#define ADDWNDSTYLE(hWnd, flag) \
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE)|flag)
#define REMOVEWNDSTYLE(hWnd, flag) \
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE)&~flag)


// ����������
#define WND_CLASS_NAME                  _T("UI")
#define WND_ANIMATE_CLASS_NAME          _T("UI_Animate")
#define WND_POPUP_CONTROL_SHADOW_NAME   _T("UI_PopupControlShadow") // ��ϵͳ��Ӱ����
#define WND_POPUP_CONTROL_NAME          _T("UI_PopupControl")
#define WND_DRAGBITMAPWND_CLASS_NAME    _T("UI_DragBitmapWnd")

}


#endif // _UIDEFINE_H_