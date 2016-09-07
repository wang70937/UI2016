#ifndef _UI_MSG_H_
#define _UI_MSG_H_


//
// Ϊ�˿�������Ϣ��Ӧ������ֱ�ӷ��ʵ�ǰ��Ϣ�ṹ����m_pCurMsg��Ϊ��Ա�������з��ʻ�����
// Ϊ�˷�ֹ�ڴ���һ����ϢA�Ĺ����У������ٷ�������һ����ϢB������A��m_pCurMsg��B�۸ģ�
// ����������һ����ʱ�������浱ǰ��Ϣm_pCurMsg��������B�������m_pCurMsg�ָ���A
// Ϊ��ʵ�ֻ�ԭm_pCurMsg�����е���Ϣ����ֱ�ӷ��أ����ǽ�m_pCurMsg��ԭ֮����return
//
// fix bug: 20120831
//    Ϊ�˽�������ཫ��ϢCHAIN������ʱ��������UI_BEGIN_MSG_MAP�ֻ�������Ӧһ��DoHook��
//    ����Hook���յ�������Ϣ���������һ��bDoHook����
//
// ps: 20130320
//    ��Ixxx��Σ��ṩIMessage::virtualProcessMessage�麯����nvProcessMessage���麯����
//    ��xxx��Σ�Ҳ�ṩvirtualProcessMessage(��Ҫ���ڷ�IMessage������,IMessageInnerProxy����)��nvProcessMessage���麯��
//    ͬʱ�ⲿ�ؼ���ֱ�Ӽ̳���MessageProxy�ṩ��Ϣӳ��
//

#define UI_BEGIN_MSG_MAP()                            \
    virtual BOOL virtualProcessMessage(UI::UIMSG* pMsg, int nMsgMapID=0, bool bDoHook=false) \
    {                                                 \
        return this->nvProcessMessage(pMsg, nMsgMapID, bDoHook); \
    }                                                 \
    BOOL nvProcessMessage(UI::UIMSG* pMsg, int nMsgMapID, bool bDoHook) \
    {                                                 \
        /*ȡ��wParam,lParam���Ա����Window��Ϣ�����е�wParam,lParam����*/\
        WPARAM wParam = pMsg->wParam;                 \
        LPARAM lParam = pMsg->lParam;                 \
        UINT   uMsg   = pMsg->message;                \
        UINT   code   = pMsg->nCode;                  \
        UI::IMessage* pMsgFrom = pMsg->pMsgFrom;      \
        LRESULT& lResult = pMsg->lRet; /* ����wtl */   \
                                                      \
		(code);   /*δ���ñ��� C4189*/                  \
		(pMsgFrom);                                   \
		(lResult);                                    \
		(wParam);(lParam);(uMsg);                     \
                                                      \
        this->SetCurMsg(pMsg);                        \
                                                      \
        /*��ϢHOOK����*/                                \
        if (bDoHook && this->DoHook(pMsg, nMsgMapID)) \
            return TRUE;                              \
                                                      \
        switch (nMsgMapID)                            \
        {                                             \
        case 0:                                       


#define UI_END_MSG_MAP()                              \
            break;                                    \
        }                                             \
        return FALSE;                                 \
    }

#define UI_END_MSG_MAP_CHAIN_PARENT(baseclass)        \
            break;                                    \
        }                                             \
        if (baseclass::nvProcessMessage(pMsg, nMsgMapID, false)) \
            return TRUE;                              \
        return FALSE;                                 \
    }

// ���ֻ��ֱ�ӵ���UICHAIN_MSG_MAP_POINT_MEMBER(static_cast<baseinterface*>(m_pI##classname))��ֻ����ӦnMapIDΪ0�������
// �������UI_END_MSG_MAP_CHAIN_PARENT_Ixxx�����ڽ���Ϣ���������� 
#define UI_END_MSG_MAP_CHAIN_PARENT_Ixxx(classname, baseinterface) \
            break;                                    \
        }                                             \
        {                                             \
            if (m_pI##classname)                      \
                if (static_cast<baseinterface*>(m_pI##classname)->nvProcessMessage(pMsg, nMsgMapID, false)) \
                    return TRUE;                      \
        }                                             \
        return FALSE;                                 \
    }

#define UI_BEGIN_MSG_MAP_Ixxx(iclassname) \
	static iclassname* CreateInstance(UI::ISkinRes* pSkinRes)        \
	{ return UI::ObjectNoImplCreator<iclassname>::CreateInstance(pSkinRes); } \
    UI_BEGIN_MSG_MAP()

//
// ��Ϣ������
//
//	�ú��и�ȱ�㣬�����ܽ��յ���ǰ�ķ�֧��Ϣ�����ܴ���������֧��Ϣ����ʹ��UI_BEGIN_CHAIN_ALL_MSG_MAP�Ľ���

#define UICHAIN_MSG_MAP(theChainClass)                \
    if (theChainClass::nvProcessMessage(pMsg, nMsgMapID, false)) \
        return TRUE;                     

#define UICHAIN_MSG_MAP_MEMBER(theChainMember)        \
    if (theChainMember.ProcessMessage(pMsg, nMsgMapID, false)) \
        return TRUE;

#define UICHAIN_MSG_MAP_POINT_MEMBER(pTheChainMember)  \
    if (pTheChainMember)                               \
        if (pTheChainMember->ProcessMessage(pMsg, nMsgMapID, false)) \
            return TRUE;                               

// 
//  ǿ�н�����ǰ��Ϣ��֧��������Ϣȫ�����ݸ���Ҫ�Ķ���
//
//	TODO: �ú�ȱ�㣬ǿ�н����󣬲�֪�����ĸ���֧�ٽ��п�ʼ������Ĭ��ȡΪ0�����齫�ú������ǰ����������
//
#define UI_BEGIN_CHAIN_ALL_MSG_MAP			          \
            break;                                    \
        }

#define UI_END_CHAIN_ALL_MSG_MAP                      \
        switch (nMsgMapID)                            \
        {                                             \
            case 0: 

#define UIALT_MSG_MAP(msgMapID)                       \
        break;                                        \
    case msgMapID:                                    


#define CHAIN_MSG_MAP_MEMBER_P(theChainMember) \
    { \
    if (theChainMember) \
        if(theChainMember->ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult)) \
            return TRUE; \
    }

#define VIRTUAL_BEGIN_MSG_MAP(theClass) \
        public: \
        virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0) \
        { \
            BOOL bHandled = TRUE; \
            (hWnd); \
            (uMsg); \
            (wParam); \
            (lParam); \
            (lResult); \
            (bHandled); \
            switch(dwMsgMapID) \
            { \
            case 0:



#define  UI_MSG_NOTIFY  168272329

//
//  ��UICreateInstanceʱ���ã�������һ���ڹ����г�ʼ�����󲢷��سɹ�ʧ�ܵķ���
//  
//  message : UI_WM_FINALCONSTRUCT
//  code : NA
//  wparam : ISkinRes*,����������Դ��
//
#define UI_MSG_FINALCONSTRUCT  168252120
// HRESULT  FinalConstruct(ISkinRes* p);
#define UIMSG_FINALCONSTRUCT(func)                    \
    if (uMsg == UI_MSG_FINALCONSTRUCT)                \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        lResult = func((UI::ISkinRes*)wParam);        \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }
//
//  ��~UIObjCreator�е��ã�������������ǰ�����麯���ķ���
//  
//  message : UI_WM_FINALRELEASE
//  code : NA
//  wparam : NA
//  lparam : NA
//
#define UI_MSG_FINALRELEASE   168252121
// void  FinalRelease();
#define UIMSG_FINALRELEASE(func)                      \
    if (uMsg == UI_MSG_FINALRELEASE)                  \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func();                                       \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }



//
//	������ʾ��֪ͨ
//		message:UI_WM_SHOW_TOOLTIP
//		code:   0
//		wparam:  
//		lParam: 
//		pMsgFrom: pWindow
//
//	������󲻴��������Ϣ�򷵻�FALSE���򽫵���Ĭ�ϵ���ʾ������ʾtooltip�������ݡ�
//
#define  UI_MSG_SHOW_TOOLTIP  149150933

//
//	��itemdata��Ϊ��ʱ����ؼ���ȡitem����ʾ��Ϣ
//		message:UI_WM_GET_TOOLTIPINFO
//		code:   
//		wparam:  TOOLTIPITEM*
//		lParam:  IToolTipUI*
//		pMsgFrom: 
//
//	����0��ʾʧ�ܣ�����ʾ��ʾ��������1��������ʾ��ʾ��
//
#define UI_MSG_GET_TOOLTIPINFO 149150934
// BOOL  OnGetToolTipInfo(TOOLTIPITEM* pItem, IToolTipUI* pUI);
#define UIMSG_GET_TOOLTIPINFO(func)                   \
    if (uMsg == UI_MSG_GET_TOOLTIPINFO)               \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        lResult = func((TOOLTIPITEM*)wParam, (IToolTipUI*)lParam); \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

// wParam: SERIALIZEDATA
// lParam:
// return: 
#define  UI_MSG_SERIALIZE 139281928
// void  OnSerialize(SERIALIZEDATA* pData);
#define UIMSG_SERIALIZE(func)                         \
	if (uMsg == UI_MSG_SERIALIZE)                     \
	{                                                 \
		SetMsgHandled(TRUE);                          \
		func((SERIALIZEDATA*)wParam);                 \
		if (IsMsgHandled())                           \
			return TRUE;                              \
	}

//
//  �ӿڲ�ѯ
//
//  message : UI_WM_QUERYINTERFACE
//  code : NA
//  wparam : GUID*   pguid
//  lparam : void**  ppOut
//
#define UI_MSG_QUERYINTERFACE 165312200

// void*  QueryInterface(const IID* pIID);
#define UIMSG_QUERYINTERFACE(className)               \
    if (uMsg == UI_MSG_QUERYINTERFACE)                \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        if (IsEqualIID(*(const IID*)wParam, __uuidof(I##className)))  \
        {                                             \
            SetMsgHandled(TRUE);                      \
            lResult = (LRESULT)(void*)m_pI##className;\
            return TRUE;                              \
        }                                             \
    }

//
//	����һ��������ָ���Ķ��������λ��
//
//		message: UI_WM_HITTEST
//		code:
//		wparam:  [in]  POINT*  ptInParent
//		lparam:  [out,optional] POINT*  ptInChild (�����ȥ�Լ������Ͻ�ƫ��)
//		pMsgFrom: 
//
//	Return
//
//		HTNOWHERE��ʾ���ڸö�������
//
#define UI_MSG_HITTEST  168261616
// UINT  OnHitTest(POINT* ptInParent, __out POINT* ptInChild)
#define UIMSG_HITTEST(func)                           \
    if (uMsg == UI_MSG_HITTEST)                       \
    {                                                 \
        POINT ptInParent = *(POINT*)wParam;           \
        SetMsgHandled(TRUE);                          \
        lResult = (long)func(&ptInParent, (POINT*)lParam); \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }


//	�桢�������еĿɼ�״̬�����˸ı䣨��Ҫ�������ڸ���������ʱ�����Զ���HwndObj����Ҳ���أ�
//		message: UI_WM_PARENT_VISIBLE_CHANGED
//		code:    
//		wparam:  BOOL bVisible, (������WM_SHOWWINDOWһ��)
//		lparam:  Object*
#define UI_MSG_VISIBLE_CHANGED  168261620

// void  OnVisibleChanged(BOOL bVisible, IObject* pObjChanged)
#define UIMSG_VISIBLE_CHANGED(func)                   \
    if (uMsg == UI_MSG_VISIBLE_CHANGED)               \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func(wParam?TRUE:FALSE, (UI::IObject*)lParam);\
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }


//
//	MouseManager֪ͨһ���������ĵ�ǰ״̬�����ı�(hover,press...)
//  ר������ˢ�´�����Ҫ�������Ϣ����������߼�������Ϊ���ܻᱻHOOK��������״̬����ȷ
//
//    UI_WM_STATECHANGED,

// wParam: nChangeMask
#define UI_MSG_STATECHANGED 168261626
// void  OnStateChanged(UINT nMask);
#define UIMSG_STATECHANGED(func)                      \
    if (uMsg == UI_MSG_STATECHANGED)                  \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((UINT)wParam);                           \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

// void  OnPaint(IRenderTarget* pRenderTarget)
#define UIMSG_PAINT(func)                             \
    if (uMsg == WM_PAINT)                             \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((UI::IRenderTarget*)wParam);             \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

// void  OnEraseBkgnd(UI::IRenderTarget* pRenderTarget)
//	remark
//		����HDC�����Ѿ�������˫���崦���ˣ�����ֱ��ʹ�á�ԭʼ��WM_ERASEBKGND��Ϣ�ѱ�������
//		����ȥ���˷���ֵ���Է��������Լ�Ҳ��֪�����׸÷���ʲô����ʵ����ķ���ֵû����˼��
#define UIMSG_ERASEBKGND(func)                     \
    if (uMsg == WM_ERASEBKGND)                        \
    {                                                 \
        SetMsgHandled(TRUE);                          \
		func((UI::IRenderTarget*)wParam);             \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

//
//  ����������֧�ֲ��֣����ز��ֶ���ָ�롣��OBJ_WINDOW,OBJ_PANEL,OBJ_COMPOUND_CONTROL
//
//		message: UI_WM_GETLAYOUT
//		code:
//		wparam: 
//		lparam
//
//	Return:  ILayout*
//
#define UI_MSG_GETLAYOUT 168261637


//
//	��ȡ��ǰ���Ƶ�ƫ����������OnDraw
//
//		wparam:  [out] int* xOffset
//		lparam:  [out] int* yOffset 
//
//	Return
//
#define UI_MSG_GETSCROLLOFFSET  168261657
// void  OnGetScrollOffset(int* pxOffset, int* pyOffset);
#define UIMSG_GETSCROLLOFFSET(func)                   \
    if (uMsg == UI_MSG_GETSCROLLOFFSET)               \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((int*)wParam, (int*)lParam);             \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

//
//	��ȡ��ǰ������Χ
//
//		wparam:  [out] int* xRange
//		lparam:  [out] int* yRange
//
//	Return
//
#define UI_MSG_GETSCROLLRANGE  168261658
// void  OnGetScrollRange(int* pxRange, int* pyRange);
#define UIMSG_GETSCROLLRANGE(func)                    \
    if (uMsg == UI_MSG_GETSCROLLRANGE)                \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((int*)wParam, (int*)lParam);             \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }


//
//  ��������Ҫ��������non client region(������padding,border)����ʱ����
//  non client child object���͸���Ϣ
//       message: UI_WM_CALC_PARENT_NONCLIENTRECT
//       wparam:  RECT*  prcNonClient.  ������������Ҳ�ʱ��prcNonClient.right+=20;
//
#define UI_MSG_CALC_PARENT_NONCLIENTRECT 168261659

// void OnCalcParentNonClientRect(CRegion4* prcNonClientRegion)
#define UIMSG_CALCPARENTNONCLIENTRECT(func)               \
    if (uMsg == UI_MSG_CALC_PARENT_NONCLIENTRECT)         \
    {                                                     \
        SetMsgHandled(TRUE);                              \
        func((CRegion4*)wParam);                          \
        if (IsMsgHandled())                               \
            return TRUE;                                  \
    } 


// void  OnPaint(IRenderTarget* pRenderTarget)
#define UIMSG_PAINT(func)                             \
    if (uMsg == WM_PAINT)                             \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((UI::IRenderTarget*)wParam);             \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }


// �ؼ����Լ����Ӷ�����֮���ٽ���һ����Ϣ���ں���
// ��Ҫ�ؼ���post_paint��ʽ
#define UI_MSG_POSTPAINT        147281159
// void OnPostPaint(IRenderTarget* pRenderTarget)
#define UIMSG_POSTPAINT(func) \
	if (uMsg == UI_MSG_POSTPAINT) \
	{ \
		SetMsgHandled(TRUE); \
        func((UI::IRenderTarget*)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

// void  OnEraseBkgnd(UI::IRenderTarget* pRenderTarget)
#define UIMSG_ERASEBKGND(func)                        \
    if (uMsg == WM_ERASEBKGND)                        \
    {                                                 \
        SetMsgHandled(TRUE);                          \
		func((UI::IRenderTarget*)wParam);             \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }



//
//	��ui��ʵ��post message���Ժ�����Ӧ��
//	��CForwardPostMessageWindow
//
#define UI_MSG_POSTMESSAGE  (WM_USER+829)



// LRESULT OnMessageHandlerEX(UINT uMsg, WPARAM wParam, LPARAM lParam)
#define UIMSG_HANDLER_EX(msg, func) \
	if(uMsg == msg) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = func(uMsg, wParam, lParam); \
		if(IsMsgHandled()) \
			return TRUE; \
	}



struct GETDESIREDSIZEINFO
{
    LPCTSTR szText;
    int  nLimitWidth;
};

//
//  ��ȡ����������С�������������Margin������Ҫ�Լ�ȥ����padding/border��
//
//  wparam : [out] SIZE*
//  lparam : GETDESIREDSIZEINFO* (Ŀǰ����ITextRenderBaseֵ��ΪNULL)
//
#define UI_MSG_GETDESIREDSIZE  168261803

// void  GetDesiredSize(SIZE* pSize);
#define UIMSG_GETDESIREDSIZE(func)                    \
    if (uMsg == UI_MSG_GETDESIREDSIZE)                \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((SIZE*)wParam);                          \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

// void GetDesiredSize(SIZE* pSize, GETDESIREDSIZEINFO* pInfo);
#define UIMSG_GETDESIREDSIZE2(func)                   \
    if (uMsg == UI_MSG_GETDESIREDSIZE)                \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((SIZE*)wParam, (GETDESIREDSIZEINFO*)lParam); \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }


//
//  RenderBase������Ϣ
//    
//  message : UI_WM_RENDERBASE_DRAWSTATE
//  code : NA
//  wparam : 
//  lparam : RENDERBASE_DRAWSTATE*
//
#define UI_MSG_RENDERBASE_DRAWSTATE 168261825

// void DrawState(RENDERBASE_DRAWSTATE* pDrawStruct);
#define UIMSG_RENDERBASE_DRAWSTATE(func)              \
    if (uMsg == UI_MSG_RENDERBASE_DRAWSTATE)          \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((RENDERBASE_DRAWSTATE*)wParam);          \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

// void OnDrawState(TEXTRENDERBASE_DRAWSTATE* pDrawStruct);
#define UIMSG_TEXTRENDERBASE_DRAWSTATE(func)          \
    if (uMsg == UI_MSG_RENDERBASE_DRAWSTATE)          \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((TEXTRENDERBASE_DRAWSTATE*)wParam);      \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }


// ���ô����ϵ�Ĭ�ϰ�ť (����Ϣ������ˢ�¶���)
// message:  UI_DM_SETDEFID  
// wParam:   IObject*
#define UI_DM_SETDEFID  138011443

// void OnSetDefId(IObject* pButton);
#define UIMSG_DM_SETDEFID(func)                       \
    if (uMsg == UI_DM_SETDEFID)                       \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((UI::IObject*)wParam);                   \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

// ��ȡ�����ϵ�Ĭ�ϰ�ť
// message: UI_DM_GETDEFID  
// return: IObject*
#define UI_DM_GETDEFID  138011444

// IObject* OnGetDefId();
#define UIMSG_DM_GETDEFID(func)                       \
    if (uMsg == UI_DM_GETDEFID)                       \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        lResult = (LRESULT)func();                    \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

//
//  �޸Ĵ�����������
//  
//  message: CREATESTRUCT*
//
#define UI_MSG_PRECREATEWINDOW 168270752

// BOOL  PreCreateWindow(CREATESTRUCT* pcs);
#define UIMSG_PRECREATEWINDOW(func)                   \
    if (uMsg == UI_MSG_PRECREATEWINDOW)               \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        lResult = func((CREATESTRUCT*)wParam);        \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }



//  ��ȡ���ڵ�͸�����ͣ������жϵ�ǰ�����Ƿֲ�ģ�����aero
//  wparam: 
//  lparam:
//  Return: 
//      WINDOW_TRANSPARENT_MODE
//
#define  UI_MSG_GET_WINDOW_TRANSPARENT_MODE  132831133

// UI::WINDOW_TRANSPARENT_MODE  GetTransparentMode()
#define UIMSG_GET_WINDOW_TRANSPARENT_MODE(func)           \
    if (uMsg == UI_MSG_GET_WINDOW_TRANSPARENT_MODE)       \
    {                                                     \
        SetMsgHandled(TRUE);                              \
        lResult = (long)func();                           \
        if (IsMsgHandled())                               \
            return TRUE;                                  \
    } 



//
//  ���������пؼ����ɴ��ڷ���ĳ�ʼ��
//
//  ��ʼ��˳��
//    1. ��ʼ���Լ�UI_WM_INITIALIZE��
//    2. ��ʼ���Լ����Ӷ���
//    3. ���Լ��ٷ���һ��UI_WM_INITIALIZE2��Ϣ��ʾ�Ӷ����ʼ�����
//
//  message : UI_WM_INITIALIZE��UI_WM_INITIALIZE2
//
#define UI_MSG_INITIALIZE  155051209
#define UI_MSG_INITIALIZE2 155051210

// void  OnInitialize();
#define UIMSG_INITIALIZE(func)                        \
    if (uMsg == UI_MSG_INITIALIZE)                    \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func();                                       \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

// void  OnInitialize2();
#define UIMSG_INITIALIZE2(func)                       \
    if (uMsg == UI_MSG_INITIALIZE2)                   \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func();                                       \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

#define  UI_MSG_GETRENDERFONT  168271806
//
// ��ȡRender Font
//
// IRenderFont*  OnGetFont()
#define UIMSG_GETRENDERFONT(func) \
    if (uMsg == UI_MSG_GETRENDERFONT) \
    { \
        SetMsgHandled(TRUE); \
        lResult = (long)(IRenderFont*)func(); \
        if (IsMsgHandled()) \
            return TRUE; \
    }



#pragma region // ����
//
//  �ڴ��ڻ���֮ǰ�������ڷ���һ��֪ͨ�����ڿ�����������ֹ�Լ����뻻��
//		message: UI_WM_SKINCHANGING
//		code:
//		wparam:  BOOL* pbChangeSkin
//		lparam: 
//
//	Return:
//
#define UI_MSG_SKINCHANGING 168271815

//
//	�ڴ��ڻ�����ɺ󣬸����ڷ���һ��֪ͨ
//
//		message: UI_WM_SKINCHANGED
//		code:
//		wparam:  
//		lparam: 
//
//	Return:
//
#define UI_MSG_SKINCHANGED  168271814

//
//	�ڴ��ڵ�HLS�仯�󣬸����ڷ���һ��֪ͨ
//
#define UI_MSG_SKINHLSCHANGED  168271813

//
//	IRenderFont���屻�޸ĺ��listener��������֪ͨ�¼�
//
//		message: UI_WM_FONTMODIFIED
//		code:
//		wparam:  IRenderFont*
//		lparam:  
//
//	Return:
//
//    UI_WM_FONTMODIFIED, -- �������Ĳ��ã�����

// ͼƬ����
#define UI_MSG_SKINTEXTURECHANGED  168271812
#pragma endregion



// ����ֵ�������ﵽ��Ե����Ҫ���б�Ե�ص�����
#define GESTURE_RETURN_NEED_BOUNCE_EDGE  2

// ������Ϣ��ƽ��
// wParam: MAKEWPARAM(xOffset, yOffset)
// lParam: UI::GESTUREINFO*
// return: HANDLED | GESTURE_RETURN_NEED_BOUNCE_EDGE
#define UI_WM_GESTURE_PAN  151221941

// wParam: MAKEWPARAM(xOffset, yOffset)
// lParam: UI::GESTUREINFO*
// return: HANDLED | GESTURE_RETURN_NEED_BOUNCE_EDGE2
#define UI_WM_GESTURE_PRESSANDTAP  151221942

//
// ��ʼ����ǰ���򱻴����Ķ�����������Ϣ�Ƿ���Ҫ���Ƶ���Ϣ��
// ����Լ�����Ҫ���������ö���ĸ������������
//
// wParam: GESTUREINFO*
// return: 0��ʾ����Ҫ��1��ʾ��Ҫ����Ϣ
//
#define UI_WM_GESTURE_BEGIN_REQ  152270927



// windows���յ�mousewheel֮���ȷ��͸�
// press object��ѯ�ʵ�ǰ�Ƿ�����mousewheel
//
// wParam/lParam: WM_MOUSEWHEEL��Ϣ����
//
// return 0 ��ʾpressobjû��������mousewheel
// return 1 ��ʾpressobj�Ѵ���������ַ�mousewheel
//
#define UI_MSG_MOUSEWHEEL_REQ  142841001




//
// ��ΪĿ��������ק�¼�
//  wParam : DROPTARGETEVENT_TYPE
//  lParam : DROPTARGETEVENT_DATA*
//
#define UI_MSG_DROPTARGETEVENT   136041933


//
//  gifˢ�µ�֪ͨ
//  wParam:  GifImageRender*
//  lParam:  notify.lparam
#define UI_MSG_GIFFRAME_TICK  168281226


namespace UI
{
	interface IUIAccessible;
	interface IUIAccessibleCreator
	{
		virtual void  Add(IUIAccessible*) = 0;	
	};
}
// �����ؼ���IAccessible
// WPARAM: IUIAccessibleCreator*
#define UI_WM_CREATE_ACCESSIBLE  155042030

// void  OnCreateAccessible(IUIAccessibleCreator* p)
#define UIMSG_CREATE_ACCESSIBLE(func)                 \
    if (uMsg == UI_WM_CREATE_ACCESSIBLE)              \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((UI::IUIAccessibleCreator*)wParam);      \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }


// void  OnSetFocus(IObject* pOldFocusObj)
#define UIMSG_SETFOCUS(func)                          \
    if (uMsg == WM_SETFOCUS)                          \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((IObject*)wParam);                       \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

// void  OnKillFocus(IObject* pNewFocusObj)
#define UIMSG_KILLFOCUS(func)                         \
    if (uMsg == WM_KILLFOCUS)                         \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((IObject*)wParam);                       \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }



// void  OnHScroll(int nSBCode, int nPos, IMessage* pMsgFrom)
#define UIMSG_HSCROLL(func)                           \
    if (uMsg == WM_HSCROLL)                           \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((int)wParam, (int)lParam, pMsgFrom);     \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

// void  OnVScroll(int nSBCode, int nPos, IMessage* pMsgFrom)
#define UIMSG_VSCROLL(func)                           \
    if (uMsg == WM_VSCROLL)                           \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((int)wParam, (int)lParam, pMsgFrom);     \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

// ScrollBarMgr֪ͨ�ؼ�ƽ�����������ڿؼ��Լ�ˢ�»�������������ʶ
// message: UI_MSG_INERTIAVSCROLL
// wParam: nOldPos
// wParam: nNewPos
#define UI_MSG_INERTIAVSCROLL  138181247



// LRESULT  OnNotify(WPARAM w, LPARAM l);
#define UIMSG_NOTIFY(_code, func)                     \
    if (uMsg == UI_MSG_NOTIFY && _code == code)       \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        lResult = func(wParam, lParam);               \
        if(IsMsgHandled())                            \
            return TRUE;                              \
    }
// LRESULT  OnNotify(WPARAM w, LPARAM l);
#define UIMSG_NOTIFY_CODE_FROM(_code, _From, func)    \
    if (uMsg == UI_MSG_NOTIFY &&                      \
        _code == code &&                              \
        static_cast<IMessage*>(_From) == pMsgFrom)    \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        lResult = func(wParam, lParam);               \
        if(IsMsgHandled())                            \
            return TRUE;                              \
    }

// void  OnNotify(WPARAM w, LPARAM l);
#define UIMSG_NOTIFY_NORET(_code, func)               \
    if (uMsg == UI_MSG_NOTIFY && _code == code)       \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func(wParam, lParam);                         \
        if(IsMsgHandled())                            \
            return TRUE;                              \
    }
// void  OnNotify(WPARAM w, LPARAM l);
#define UIMSG_NOTIFY_CODE_FROM_NORET(_code, _From, func) \
    if (uMsg == UI_MSG_NOTIFY &&                      \
        _code == code &&                              \
        static_cast<IMessage*>(_From) == pMsgFrom)    \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func(wParam, lParam);                         \
        if(IsMsgHandled())                            \
            return TRUE;                              \
    }


//	void  OnSize(UINT nType, int cx, int cy);
#define UIMSG_SIZE(func)                              \
    if (uMsg == WM_SIZE)                              \
    {                                                 \
        UINT nType = (UINT)wParam;                    \
        int  cx    = LOWORD(lParam);                  \
        int  cy    = HIWORD(lParam);                  \
        SetMsgHandled(TRUE);                          \
        func(nType, cx, cy);                          \
        if (IsMsgHandled())                           \
            return TRUE;                              \
    }

// �ȴ���WM_TIMER����һ��LPARAM
// void  OnTimer(UINT_PTR nIDEvent, LPARAM lParam)
#define UIMSG_TIMER(func)                             \
    if (uMsg == WM_TIMER)                             \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func((UINT_PTR)wParam, lParam);               \
        lResult = 0;                                  \
        if(IsMsgHandled())                            \
            return TRUE;                              \
    }



//
//  PopupControlWindow���͸�LISTBOX��MENU����Ϣ��LISTBOXת����COMBOBOX����Ϣ��COMBOBOX/MENUת�������ڵ���ϢpMsgFrom��ָ����Ϣ���ͷ�
//
#define UI_WM_INITPOPUPCONTROLWINDOW    147301646
#define UI_WM_UNINITPOPUPCONTROLWINDOW  147301647
#define UI_WM_POPUPWINDOWDESTROY        147301648

// void OnContextMenu(HWND hWnd, POINT point)
#define UIMSG_WM_CONTEXTMENU  MSG_WM_CONTEXTMENU

//void OnInitPopupControlWindow()
#define UIMSG_WM_INITPOPUPCONTROLWINDOW(func)         \
    if (uMsg == UI_WM_INITPOPUPCONTROLWINDOW)         \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func();                                       \
        if(IsMsgHandled())                            \
            return TRUE;                              \
    }

//void OnUnInitPopupControlWindow()
#define UIMSG_WM_UNINITPOPUPCONTROLWINDOW(func)       \
    if (uMsg == UI_WM_UNINITPOPUPCONTROLWINDOW)       \
    {                                                 \
        SetMsgHandled(TRUE);                          \
        func();                                       \
        if(IsMsgHandled())                            \
            return TRUE;                              \
    }

#endif