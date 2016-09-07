#include "stdafx.h"
#include "giftimermanager.h"
#include "Src\Base\Application\uiapplication.h"

using namespace UI;
GifTimerManager::GifTimerManager()
{
	m_pUIApplication = NULL;
	m_hTimer = NULL;
}

GifTimerManager::~GifTimerManager()
{
	if (m_hTimer)
	{
		EndTimer();
	}
}

void GifTimerManager::Init(UIApplication* pUIApp)
{
	m_pUIApplication = pUIApp;
}

void  GifTimerManager::StartTimer()
{
	if (m_hTimer)
		return;

	int nPeriod = 30;  // ��ʱ������

	m_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);

	IWaitForHandlesMgr* pMgr = m_pUIApplication->GetWaitForHandlesMgr();
	if (pMgr)
		pMgr->AddHandle(m_hTimer, static_cast<IWaitForHandleCallback*>(this), 0);

	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -1000*10*nPeriod;   // ��һ����Ӧ�ӳ�ʱ�䡣��ֵ��ʾһ����Ե�ʱ�䣬������100����Ϊ��λ�����ʱ�䣬������������5ms��������Ϊ-50000��
	SetWaitableTimer(m_hTimer, &liDueTime, nPeriod, NULL, NULL, 0);
}

void  GifTimerManager::EndTimer()
{
	if (m_hTimer)
	{
		IWaitForHandlesMgr* pMgr = m_pUIApplication->GetWaitForHandlesMgr();
		if (pMgr)
			pMgr->RemoveHandle(m_hTimer);

		// CancelWaitableTimer 
		CloseHandle(m_hTimer);
		m_hTimer = NULL;
	}
}

void GifTimerManager::OnWaitForHandleObjectCallback( HANDLE, LPARAM )
{
	check_timer();
}

// ����б��е�timer item�����������Ӧʱ��㣬�򴥷�on_timer
void GifTimerManager::check_timer()
{
	_MyIter iter = m_listTimerItem.begin();
	for (; iter != m_listTimerItem.end();)
	{
		Gif_TimerItem* pItem = *iter;
		if (pItem->get_remain() <= 0)
		{
			this->on_item_timer(pItem);

			if (pItem->nRepeat != -1 && 0 == --pItem->nRepeat)
			{
				// ������ѭ��������ɾ��
				delete pItem;
				pItem = NULL;

				iter = m_listTimerItem.erase(iter);
				continue;	
			}
			else
			{
				// ���¼�����һ�ε���Ӧʱ��
				pItem->update_repeat();    // gifimage��on_timer��Ӧ��ֻ�޸���waitֵ���������startֵ�����£�
			}
		}
		iter++;
	}
}


void  GifTimerManager::on_item_timer(Gif_TimerItem* pItem)
{
	GifImageRender* pImageDrawItem = (GifImageRender*)pItem->pData;
	if (NULL == pImageDrawItem)
		return;

	pImageDrawItem->on_tick(pItem);
}

void  GifTimerManager::AddItem(Gif_TimerItem*  pItem)
{
	if (!pItem)
		return;

	_MyIter iter = m_listTimerItem.begin();
	_MyIter iterEnd = m_listTimerItem.end();
	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->nId == pItem->nId)
			return;
	}

	Gif_TimerItem* pNewItem = new Gif_TimerItem;
	memcpy(pNewItem, pItem, sizeof(Gif_TimerItem));
	m_listTimerItem.push_back(pNewItem);

//	update_wait_time();
	if (!m_hTimer)
	{
		StartTimer();
	}
	on_item_timer(pNewItem);  // ����ˢ�µ�һ֡�����������ʾһ�¿հף� ��ճ��gifʱ
}


//
//	�������̣߳����̣߳����ã�ת����gif�����߳������� --> 
//
void GifTimerManager::RemoveItem(int id)
{
	_MyIter iter = m_listTimerItem.begin();
	for (; iter != m_listTimerItem.end();)
	{
		Gif_TimerItem* pItem = (*iter);
		if (NULL == pItem)
		{
			iter ++;
			continue;
		}

		if (pItem->nId == id)
		{
			delete pItem;
			pItem = NULL;
			iter = m_listTimerItem.erase(iter);

			if (0 == m_listTimerItem.size())
			{
				EndTimer();
			}
			break;
		}
		else
		{
			iter ++ ;
		}
	}
}

