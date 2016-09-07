#ifndef _UI_IMESSAGE_H_
#define _UI_IMESSAGE_H_

namespace UI
{
interface  IMessage;

// ��Ϣ�ṹ���塣ϵͳ�ؼ���Ȼʹ��MSG����UI�ؼ�ʹ��UIMsg���д���
struct UIMSG : public MSG
{
    UIMSG() { memset(this, 0, sizeof(UIMSG)); }

    IMessage*   pMsgFrom;    // ��Ϣ������
    IMessage*   pMsgTo;      // ��Ϣ������ 
    UINT        nCode;       // ��� WM_COMMAND,WM_NOTIFY
    long        lRet;        // ��Ϣ���������ķ���ֵ
    BOOL        bHandled;    // ����Ϣ�Ƿ��ѱ������
};



class Message;
interface UISDKAPI IMessage
{
    IMessage(E_BOOL_CREATE_IMPL);
    BOOL  ProcessMessage(UIMSG* pMsg, int nMsgMapID=0, bool bDoHook=false);
    void  Release();
	
	BOOL    IsMsgHandled()const;
	void    SetMsgHandled(BOOL b);
	UIMSG*  GetCurMsg();
	void    SetCurMsg(UIMSG* p);
	BOOL    DoHook(UIMSG* pMsg, int nMsgMapID);

protected:
    friend class Message;  
    virtual ~IMessage();  // �麯��1. ��֤��ȷ�ͷ���������
private:
    virtual BOOL  virtualProcessMessage(UIMSG* pMsg, int nMsgMapID, bool bDoHook);  // �麯��2. ��Ϣ����
	virtual void  virtual_delete_this();  // ��UIObjCreator����ʵ��

public:
	Message*  GetImpl();
    void  ClearNotify();
    void  SetNotify(IMessage* pObj, int nMsgMapID);
    long  DoNotify(UIMSG* pMsg);
    IMessage*  GetNotifyObj();
	void  CopyNotifyTo(IMessage* pObjCopyTo);

    void  AddHook(IMessage* pObj, int nMsgMapIDToHook, int nMsgMapIDToNotify );
    void  RemoveHook(IMessage* pObj, int nMsgMapIDToHook, int nMsgMapIDToNotify );
    void  RemoveHook(IMessage* pObj );
    void  ClearHook();

    void  AddDelayRef(void** pp);
    void  RemoveDelayRef(void** pp);

    void*  QueryInterface(REFIID iid);
protected:
    Message*   m_pImpl;
};


// ���ⲿ����̳У�����֧����Ϣӳ��궨��
class UISDKAPI MessageProxy
{
public:
	MessageProxy(IMessage* p);
	virtual ~MessageProxy();

	BOOL    IsMsgHandled()const;
	void    SetMsgHandled(BOOL b);
	UIMSG*  GetCurMsg();
	void    SetCurMsg(UIMSG* p);
	BOOL    DoHook(UIMSG* pMsg, int nMsgMapID);

protected:
	Message*  m_pImpl;
};

}

#endif  _UI_IMESSAGE_H_