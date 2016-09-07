#pragma once
namespace UI
{
	//
	// �����������������ҵ���Ϣ
	//
	class MsgHook
	{
	public:
		MsgHook()
		{
			pObj = NULL;
			nMsgMapIDToHook = 0;
			nMsgMapIDToNotify = 0;
		}

		IMessage* pObj;                // ��¼˭Ҫ�����ҵ���Ϣ
		int      nMsgMapIDToHook;      // pObjҪHOOK��map id����Ϣ
		int      nMsgMapIDToNotify;    // HOOK������Ϣ��pObjʹ�ø�map idȥ��Ӧ
	};

	class MsgNotify
	{
	public:
		MsgNotify()
		{
			pObj = NULL;
			nMsgMapIDToNotify = 0;
		}

		IMessage*  pObj;
		int       nMsgMapIDToNotify;    // ������Ϣ֪ͨpObjʱ��pObjʹ�ø�idȥ��Ӧ
	};

	// ��Ϣ����,object�Ӹ���̳дӶ�ӵ������Ϣ����
	class Message
	{
	public:
		Message(IMessage*);
		virtual ~Message();

		IMessage*    GetIMessage();

		BOOL         IsMsgHandled()const;
		void         SetMsgHandled(BOOL);
		UIMSG*       GetCurMsg() { return m_pCurMsg; }
		void         SetCurMsg(UIMSG* p) { m_pCurMsg = p; }

		void         ClearNotify();
		void         SetNotify(IMessage* pObj, int nMsgMapID = 0);
		void         CopyNotifyTo(IMessage* pObjCopyTo);
		IMessage*    GetNotifyObj() { return m_objNotify.pObj; }

		void         AddHook(IMessage* pObj, int nMsgMapIDToHook, int nMsgMapIDToNotify);
		void         RemoveHook(IMessage* pObj, int nMsgMapIDToHook, int nMsgMapIDToNotify);
		void         RemoveHook(IMessage* pObj);
		void         ClearHook();

		// ����TRUE����ʾ����Ϣ�ѱ�����FALSE��ʾ����Ϣû������
		BOOL         ProcessMessage(UIMSG* pMsg, int nMsgMapID = 0, bool bDoHook = false);
		virtual BOOL virtualProcessMessage(UIMSG* pMsg, int nMsgMapID = 0, bool bDoHook = false);

		BOOL         DoHook(UIMSG* pMsg, int nMsgMapID);
		long         DoNotify(UIMSG* pMsg);

		void         AddDelayRef(void** pp);
		void         RemoveDelayRef(void** pp);
		void         ResetDelayRef();

	protected:
		list<MsgHook*>    m_lHookMsgMap;      // ����ComboBoxҪhook Combobox�е�������ť���¼�
		MsgNotify         m_objNotify;        // �����¼�ʱ����Ҫ֪ͨ�Ķ���
		list<void**>      m_lDelayRefs;       // ��Ҫ�ӳٵ����Լ���һЩ���ã������Լ�������֮�󻹵���IMessage��һЩ��������uipostmessage, tooltip timer. ȡ��ԭUIApplication�е�AddUIObject���ܣ�Ч��̫��

		UIMSG *           m_pCurMsg;          // ��¼��ǰ���ڴ������Ϣ
		IMessage*         m_pIMessage;
		BOOL              m_bCreateIMessage;
	};

}