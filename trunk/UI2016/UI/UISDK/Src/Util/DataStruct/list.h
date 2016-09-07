#pragma once

// ps: stl�е�list����һ�����⣺�ڱ����Ĺ����У����ȥɾ��һ������������
//     �������дһ��list���ڱ�����ɾ��Ҳû������
template <class T>
struct UIListItem
{
	UIListItem() 
	{
		m_pPrev = m_pNext = NULL;
	}
	UIListItem*  GetPrev()
	{
		return m_pPrev;
	}
	UIListItem*  GetNext()
	{ 
		return m_pNext; 
	}
	void  SetPrev(UIListItem* p) 
	{ 
		m_pPrev = p;
	}
	void  SetNext(UIListItem* p)
	{
		m_pNext = p;
	}

	T& operator*()
	{
		return m_data;
	}

	T  m_data;
private:
	UIListItem*  m_pPrev;
	UIListItem*  m_pNext;
};
template <class T>
class UIList
{
public:
	UIList()
	{
		m_pFirst = NULL;
		m_pLast = NULL;
		m_nCount = 0;
	}
	~UIList()
	{
		UIListItem<T>* p = m_pFirst;
		UIListItem<T>* pNext = NULL;

		while (p)
		{
			pNext = p->GetNext();    
			delete p;
			p = pNext;
		}
		m_nCount = 0;
		m_pFirst = m_pLast = NULL;
	}

	UIListItem<T>*  Find(T t)
	{
		UIListItem<T>* pItem = m_pFirst;
		while (pItem)
		{
			if (pItem->m_data == t)
				return pItem;

			pItem = pItem->GetNext();
		}
		return NULL;
	}

	T  At(int nPos)
	{
		if (nPos < 0 || nPos >= m_nCount)
			return NULL;

		UIListItem<T>* pItem = m_pFirst;
		int i = 0;
		while (pItem)
		{
			if (nPos == i++)
				return pItem->m_data;

			pItem = pItem->GetNext();
		}
		return (T)NULL;
	}

	void  Remove(T t)
	{
		UIListItem<T>* pItem = Find(t);
		if (pItem)
		{
			if (pItem->GetPrev())
				pItem->GetPrev()->SetNext(pItem->GetNext());
			else
				m_pFirst = pItem->GetNext();

			if (pItem->GetNext())
				pItem->GetNext()->SetPrev(pItem->GetPrev());
			else
				m_pLast = pItem->GetPrev();

			delete pItem;
			m_nCount--;
		}
	}

	void  Add(T t)
	{
		UIListItem<T>* p = new UIListItem<T>;
		p->m_data = t;

		if (m_pLast)
		{
			m_pLast->SetNext(p);
			p->SetPrev(m_pLast);
		}
		m_pLast = p;
		if (NULL == m_pFirst)
			m_pFirst = p;

		m_nCount++;
	}

	int  GetCount() { return m_nCount; }

	void  DELETE_ALL()
	{
		UIListItem<T>* p = m_pFirst;
		UIListItem<T>* pNext = NULL;

		while (p)
		{
			pNext = p->GetNext();
			SAFE_DELETE(p->m_data);
			delete p;
			p = pNext;
		}
		m_nCount = 0;
		m_pFirst = m_pLast = NULL;
	}
	void  RELEASE_ALL()
	{
		UIListItem<T>* p = m_pFirst;
		UIListItem<T>* pNext = NULL;

		while (p)
		{
			pNext = p->GetNext();
			SAFE_RELEASE(p->m_data);
			delete p;
			p = pNext;
		}
		m_nCount = 0;
		m_pFirst = m_pLast = NULL;
	}
	void  Clear()
	{
		UIListItem<T>* p = m_pFirst;
		UIListItem<T>* pNext = NULL;

		while (p)
		{
			pNext = p->GetNext();
			delete p;
			p = pNext;
		}
		m_nCount = 0;
		m_pFirst = m_pLast = NULL;
	}

	UIListItem<T>*  GetFirst()
	{
		return m_pFirst;
	}
	UIListItem<T>*  GetLast()
	{
		return m_pLast;
	}

private:
	UIListItem<T>*  m_pFirst;
	UIListItem<T>*  m_pLast;
	int  m_nCount;
};

