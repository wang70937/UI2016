#ifndef _UIDEFINE_H_
#define _UIDEFINE_H_

// 导入导出定义
#ifdef UISDK_EXPORTS
#define UISDKAPI __declspec(dllexport)
#else
#define UISDKAPI __declspec(dllimport)
#endif

namespace UI
{

	// 特殊含义常量
	enum
	{
		NDEF = -1,    // 没有定义的变量
		AUTO = -2,    // 自动调整（如width/height=auto）
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
	

// 跨模块时，作为本模块的根对象，需要增加成员变量及销毁该变量
#define  UI_DECLARE_INTERFACE_ACROSSMODULE(T)               \
	UI_DECLARE_INTERFACE(T)                                 \
protected:                                                  \
	virtual ~I##T();                                        \
	T*  m_pImpl;


// 这里采用operator new的原因：
//   如果采用m_pImpl = new T(this);
//   那么在T构造函数中调用Ixxx类的方法时，Ixxx的m_pImpl还没有
//   被赋值。因此将该操作分解成两步：malloc + construct
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

	// TODO: 优化调用形式。
#define DO_PARENT_PROCESS(IMyInterface, IParentInterface) \
    static_cast<IParentInterface*>(m_p##IMyInterface)->nvProcessMessage(GetCurMsg(), 0, 0); \
    SetMsgHandled(TRUE)

	// 获取父类是否处理
#define DO_PARENT_PROCESS2(IMyInterface, IParentInterface, bHandled) \
	bHandled = static_cast<IParentInterface*>(m_p##IMyInterface)->nvProcessMessage(GetCurMsg(), 0, 0); \
	SetMsgHandled(TRUE)
    
// 获取返回值
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
//	release版本下断言输出，可以利用debugview.exe看到断言错误
//  remark: __FILE__ ...这些都是char*类型的
//
#ifdef ASSERT
#undef ASSERT  // 避免使用ASSERT，请使用UIASSERT
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

// 是否处理返回值 
enum HANDLED_VALUE
{
	NOT_HANDLED = 0,
	HANDLED = 1,
};

class IRootInterface
{
public:
	virtual ~IRootInterface() = 0 {};  // 确保delete时能调用到派生类的析构函数
};


// 对象序列化消息。用于取代WM_SETATTRIBUTE
enum SERIALIZEFLAG
{
	SERIALIZEFLAG_LOAD = 0x01,
	SERIALIZEFLAG_RELOAD = 0x03,   // reload也是load
	SERIALIZEFLAG_SAVE = 0x04,
	SERIALIZEFLAG_EDITOR = 0x08,  // 获取属性列表和提示信息

	// load 标识
	SERIALIZEFLAG_LOAD_ERASEATTR = 0x0200,  // 获取到属性后，将该属性从mapattr中删除

	// save 标识
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

	IUIApplication*  pUIApplication; // TODO: 废弃该变量，只使用pSkinRes
	ISkinRes*  pSkinRes;
	LPCTSTR  szPrefix;      // 属性前缀
	LPCTSTR  szParentKey;   // 父属性（仅用于editor），如bkg.render.type
	UINT  nFlags;

	bool  IsReload() { return ((nFlags&SERIALIZEFLAG_RELOAD) == SERIALIZEFLAG_RELOAD) ? true : false; }
	bool  IsLoad() { return ((nFlags&SERIALIZEFLAG_LOAD) == SERIALIZEFLAG_LOAD) ? true : false; }
	bool  IsSave() { return ((nFlags&SERIALIZEFLAG_SAVE) == SERIALIZEFLAG_SAVE) ? true : false; }
	bool  IsEditor() { return ((nFlags&SERIALIZEFLAG_EDITOR) == SERIALIZEFLAG_EDITOR) ? true : false; }
	bool  NeedErase() { return (nFlags&SERIALIZEFLAG_LOAD_ERASEATTR) ? true : false; }
	void  SetErase(bool b) { if (b) { nFlags |= SERIALIZEFLAG_LOAD_ERASEATTR; } else { nFlags &= ~SERIALIZEFLAG_LOAD_ERASEATTR; } }
};


// object的state bit，状态位
enum OBJECT_STATE_BIT
{
	OSB_UNVISIBLE = 0x0001,        // 自己是否可见
	OSB_COLLAPSED = 0x0002,        // listitem为收起来，不显示子结点（TODO:以后可用于扩展为隐藏并且不占用布局?）
	OSB_DISABLE = 0x0004,        // 自己是否可用

	OSB_PRESS = 0x0010,
	OSB_HOVER = 0x0020,
	OSB_FORCEPRESS = 0x0040,
	OSB_FORCEHOVER = 0x0080,
	OSB_READONLY = 0x0100,       // 还是由各个控件自己实现更好一些。
	OSB_FOCUS = 0x0200,
	OSB_DEFAULT = 0x0400,
	OSB_SELECTED = 0x0800,        // 被选中
	OSB_CHECKED = 0x1000,
	OSB_RADIOCHECKED = 0x2000,
	OSB_DRAGDROPHOVER = 0x4000,       // 拖拽过程中，位于鼠标下。目前仅listitem使用
	OSB_DRAGING = 0x8000,       // 正在被拖拽

	WSB_ACTIVE = 0x1000,        // 窗口为active
};

#define SWP_LAYEREDWINDOW_SIZEMOVE  0x80000000   // 表示这是个分层窗口模拟的窗口大小改变
#define SWP_NOUPDATELAYOUTPOS       0x40000000   // 调用SetObjectPos时不更新布局属性
#define SWP_FORCESENDSIZEMSG        0x20000000   // 即使大小没有改变，也强制发送一个WM_SIZE消息，用于走通逻辑
#define SWP_FORCEUPDATEOBJECT       0x10000000   // 即使大小没有改变，也强制柠檬素，用于走通逻辑



#define UI_DECLARE_RENDERBASE(className, xml, rendertype)   \
    static LPCTSTR  GetXmlName() { return xml; }       \
    static int  GetType() { return rendertype; }      


// 本宏定义主要是用于theme类型的renderbase，要根据控件类型进行创建
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


// 窗口类名称
#define WND_CLASS_NAME                  _T("UI")
#define WND_ANIMATE_CLASS_NAME          _T("UI_Animate")
#define WND_POPUP_CONTROL_SHADOW_NAME   _T("UI_PopupControlShadow") // 带系统阴影功能
#define WND_POPUP_CONTROL_NAME          _T("UI_PopupControl")
#define WND_DRAGBITMAPWND_CLASS_NAME    _T("UI_DragBitmapWnd")

}


#endif // _UIDEFINE_H_