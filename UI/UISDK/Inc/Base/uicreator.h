#ifndef _UI_CREATOR_H_
#define _UI_CREATOR_H_

//////////////////////////////////////////////////////////////////////////
//
//    ���󴴽��ߣ�Ϊ��֧���ڶ��󴴽������ٵ�ʱ���ܵ����麯����Ҫ�����ж���
//    ����UICreateInstance���д���������ʼ��������ͷŴ������FinalConstruct
//    /FinalRelease����
//
//////////////////////////////////////////////////////////////////////////

namespace UI
{
interface IUIApplication;
interface ISkinRes;
struct UIMSG;

template<class T>
class ObjectCreator : public T
{
public: 
	// ͨ����
	static void CreateInstance2(ISkinRes* pSkinRes, void** pp)
	{
		*pp = (void*)ObjectCreator<T>::CreateInstance(pSkinRes);
	}
	// ר����
	static T* CreateInstance(ISkinRes* pSkinRes)
	{
        if (!pSkinRes)
        {
#ifdef _DEBUG
            DebugBreak();
#endif
            return NULL;
        }

		T* p = new ObjectCreator<T>;
		
        if (FAILED(UISendMessage(
                    p,
                    UI_MSG_FINALCONSTRUCT, 
                    (WPARAM)pSkinRes)))
        {
            p->Release();
            return NULL;
        }

		return p;
	}
	ObjectCreator() : T(CREATE_IMPL_TRUE)
	{

	}

	// ȷ��delete��new��ͬһģ�飬��IMessage::Destroy����
	virtual void virtual_delete_this() override
	{
		UISendMessage(this, UI_MSG_FINALRELEASE);
		delete this;
	}

	// ����imessage->virtualProcessMessageʱ���ص���������������
	virtual BOOL virtualProcessMessage(UIMSG* pMsg, int nMsgMapID, bool bDoHook) override
	{
		return T::nvProcessMessage(pMsg, nMsgMapID, bDoHook);
	}
};



template<class T>
class ObjectNoImplCreator : public T
{
public: 
	// ͨ����
	static void CreateInstance2(ISkinRes* pSkinRes, void** pp)
	{
		*pp = (void*)ObjectNoImplCreator<T>::CreateInstance(pSkinRes);
	}
	// ר����
	static T* CreateInstance(ISkinRes* pSkinRes)
	{
		T* p = new ObjectNoImplCreator<T>;

		if (FAILED(UISendMessage(
				p,
				UI_MSG_FINALCONSTRUCT, 
				(WPARAM)pSkinRes)))
		{
			delete p;
			return NULL;
		}

		return p;
	}
	ObjectNoImplCreator() : T()
	{

	}

	// ȷ��delete��new��ͬһģ�飬��IMessage::Destroy����
	virtual void virtual_delete_this() override
	{
		UISendMessage(this, UI_MSG_FINALRELEASE);
		delete this;
	}
};


}
#endif // _UI_CREATOR_H_