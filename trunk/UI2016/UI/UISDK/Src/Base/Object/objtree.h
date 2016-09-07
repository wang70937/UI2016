#pragma once
#include "Src/Base/Message/message.h"

//
//	ʹ��Tree�ṹʵ��Object֮��Ĳ�ι�ϵ
//
namespace UI
{
class Object;

class ObjTree : public Message
{
public:
	ObjTree(IMessage* p) : Message(p)
		{ m_pParent = m_pChild = m_pNcChild = m_pNext = m_pPrev = NULL;	}
	~ObjTree()
		{}

public:
    virtual bool IsNcObject() { return false; }  // �ɼ̳�������չʵ��
	virtual void SetAsNcObject(bool b) {}
	virtual bool CanTabstop() { return false; }
	virtual long GetZorder() { return 0;  }

	void  AddChild(Object* pObj);
    void  InsertChild(Object* pObj, Object* pInsertAfter);
	void  AddNcChild(Object* pObj);
    void  InsertAfter(Object* pInsertAfter);
    void  InsertBefore(Object* pInsertBefore);
	bool  IsMyChild(Object* pChild, bool bFindInGrand);
	bool  RemoveChildInTree(Object* pChild);

	Object* EnumChildObject(Object* pObj);
	Object* REnumChildObject(Object* pObj);
	Object* EnumNcChildObject(Object* pObj);
	Object* REnumNcChildObject(Object* pObj);
	Object* EnumAllChildObject(Object* pObj);
	Object* EnumParentObject(Object* pObj);
	Object* REnumParentObject(Object* pObj);

	Object* GetParentObject();
	Object* GetChildObject();
	Object* GetNcChildObject();
	Object* GetLastChildObject();
	Object* GetLastNcChildObject();
	Object* GetNextObject();
	Object* GetPrevObject();
	Object* GetRootObject();

    void SetParentObjectDirect(Object* p);
    void SetChildObjectDirect(Object* p);
    void SetNcChildObjectDirect(Object* p);
    void SetNextObjectDirect(Object* p);
    void SetPrevObjectDirect(Object* p);

	Object* GetNextTreeItemObject();
	Object* GetPrevTreeItemObject();
	Object* GetNextTreeTabstopItemObject();
	Object* GetPrevTreeTabstopItemObject();

	Object* GetNextTabObject()
    {
        return GetNextTreeTabstopItemObject(); 
    }
	Object* GetPrevTabObject()
    {
        return GetPrevTreeTabstopItemObject(); 
    }

	void RemoveMeInTheTree();

    void MoveToAsFirstChild();
    void MoveToAsLastChild();
    bool SwapObject(Object* pObj1, Object* pObj2);

protected:
    void DestroyChildObject();

protected:
	Object*  m_pParent;
	Object*  m_pChild;
	Object*  m_pNcChild;     // �ǿͻ������Ӷ�����Ҫ����ʵ�ֹ���ʱ��������ƫ��
	Object*  m_pNext;
	Object*  m_pPrev;
};

}

