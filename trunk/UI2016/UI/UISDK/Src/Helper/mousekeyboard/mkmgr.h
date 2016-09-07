#pragma once

// mouse keyboard manager
namespace UI
{
class Object;
class UIApplication;
class WindowBase;

    template <class T>
    void _SetHoverObject(Object* pNewHoverObj, T* pThis)
    {
        if (NULL == pThis->m_pUIApplication)
            return;

        Object* pOldHoverObj = pThis->m_pObjHover;
        if (pThis->m_pObjHover == pNewHoverObj)
            return;

        pThis->m_pUIApplication->HideToolTip();
        if (NULL != pNewHoverObj)
        {
            TOOLTIPITEM  item = {0};
            item.pNotifyObj = pNewHoverObj->GetIObject();
            pThis->m_pUIApplication->ShowToolTip(&item);
        }

        
        // ע�⣺ �ȸı����ж����״̬���ٷ�����Ϣ�������ڵ�һ��������Ӧ��Ϣ��ʱ���ò����ڶ��������״̬
        if (pThis->m_pObjHover)
        {
            pThis->m_pObjHover->SetHover(false, false);
        }

        // diable��object�����Ե���tooltip����������Ϊhover
        if (pNewHoverObj && pNewHoverObj->IsEnable())
        {
            pNewHoverObj->SetHover(true, false);
        }

        pThis->m_pObjHover = pNewHoverObj;

        UIMSG  msg;
        msg.message = UI_MSG_STATECHANGED;
        msg.wParam = OSB_HOVER;
        if (pOldHoverObj)
        {
            msg.pMsgTo = pOldHoverObj->GetIMessage();
            UISendMessage(&msg);
        }
        if (pNewHoverObj)
        {
            msg.pMsgTo = pNewHoverObj->GetIMessage();
            UISendMessage(&msg);
        }
    }

    template <class T>
    void _SetPressObject(Object* pNewPressObj, T* pThis)
    {
        if (pThis->m_pObjPress == pNewPressObj)
            return;

        Object* pOldPressObj = pThis->m_pObjPress;
        if (pThis->m_pObjPress)
        {
            pThis->m_pObjPress->SetPress(false, false);
        }
        if (pNewPressObj)
        {
            pNewPressObj->SetPress(true, false);
        }

        pThis->m_pObjPress = pNewPressObj;
        pThis->m_pUIApplication->HideToolTip();

        UIMSG  msg;
        msg.message = UI_MSG_STATECHANGED;
        msg.wParam = OSB_PRESS;
        if (pOldPressObj)
        {
            msg.pMsgTo = pOldPressObj->GetIMessage();
            UISendMessage(&msg);
        }
        if (pNewPressObj)
        {
            msg.pMsgTo = pNewPressObj->GetIMessage();
            UISendMessage(&msg);
        }
    }

    template <class T>
    void _OnMouseMove(Object* pObj, WPARAM wParam, LPARAM lParam, T* pThis)
    {
        // �����е�������г��������ж�

        if (NULL == pThis->m_pObjPress)
        {
            if (NULL == pThis->m_pObjHover)
            {
                // ��껹���������ƶ�
                if (pObj == pThis->m_pObjHover)   
                {
                }
                // ����ƶ�����pObj��
                else                              
                {
                    pThis->SetHoverObject(pObj);
                    ::UISendMessage(pThis->m_pObjHover, WM_MOUSEMOVE, wParam, lParam);
                }
            }
            else
            {
                // �����A�����ƶ�
                if (pObj == pThis->m_pObjHover)   
                {
                    ::UISendMessage(pThis->m_pObjHover, WM_MOUSEMOVE, wParam, lParam);
                }
                // ���ղŷ���A���棬���������Ƴ�ȥ�ˣ������Ƶ�B���棬Ҳ����û���ƶ��κζ�����
                else                              
                {
                    Object* pSaveHover = pThis->m_pObjHover;
                    pThis->SetHoverObject(pObj); // ������״̬���ٷ�����Ϣ

                    ::UISendMessage(pSaveHover, WM_MOUSELEAVE, (WPARAM) 0, 0);
                    if (pObj)
                    {
                        ::UISendMessage(pObj, WM_MOUSEMOVE, wParam, lParam);
                    }
                }
            }
        }
        else
        {
            if (NULL == pThis->m_pObjHover)
            {
                // ��갴����A�����ϣ��������A���档�������ƶ�������
                if (pObj == pThis->m_pObjPress)  
                {
                    pThis->SetHoverObject(pThis->m_pObjPress);
                    ::UISendMessage(pThis->m_pObjPress, WM_MOUSEMOVE, wParam, lParam);

                }	
                // �����A�����ϰ����ˣ���������ڲ���A����
                else                    
                {
                    ::UISendMessage(pThis->m_pObjPress, WM_MOUSEMOVE, wParam, lParam);
                }
            }
            else
            {
                // ��갴����A�����ϣ�������A���ƶ�
                if (pObj == pThis->m_pObjPress)  
                {
                    UIASSERT(pThis->m_pObjPress == pThis->m_pObjHover);
                    UIASSERT(pThis->m_pObjHover == pObj);

                    ::UISendMessage(pThis->m_pObjPress, WM_MOUSEMOVE, wParam, lParam);
                }
                // ���ղŰ�����A�����ϣ�����������Ƴ����ˡ�
                else  
                {
                    pThis->SetHoverObject(NULL);
                    ::UISendMessage(pThis->m_pObjPress, WM_MOUSEMOVE, wParam, lParam);
                }
            }
        }
    }



    // ע������һ��HwndHost����ʱ������Ƶ�������ʱ������WM_MOUSELEAVE��Ϣ��������MOUSEMOVE
    //     ���HwndHost����û��ʵ��Tooltip;
    //
    template <class T>
    void _OnMouseLeave(T* pThis)
    {
        // Ϊ�˷�ֹ�ڶ����ڴ���WM_LBUTTONUP��ϢʱMouseManager��״̬�����˸ı�,�ȱ���״̬
        Object*  pSaveObjPress = pThis->m_pObjPress;
        Object*  pSaveObjHover = pThis->m_pObjHover;

        pThis->SetHoverObject(NULL);
        pThis->SetPressObject(NULL);

        if (pSaveObjHover)
        {
            UIMSG msg;
            msg.pMsgTo = pSaveObjHover->GetIMessage();
            msg.message = WM_MOUSELEAVE;
            msg.wParam  = 0;
            msg.lParam  = 0;
            ::UISendMessage(&msg);
        }

        if (pSaveObjPress && pSaveObjPress != pSaveObjHover)
        {
            ::UISendMessage(pSaveObjPress, WM_MOUSELEAVE);
        }
    }

    template <class T>
    void _OnLButtonDown(WPARAM wParam, LPARAM lParam, BOOL* pbHandled, T* pThis)
    {
        if (pThis->m_pObjHover && pThis->m_pObjHover->IsEnable())
        {
            pThis->SetPressObject(pThis->m_pObjHover);

            Object* pSave = pThis->m_pObjPress;

            // Ӧ������setfocus����lbuttondown/up
            if (pThis->m_pObjPress && pThis->m_pObjPress->CanTabstop())
            {
                pThis->SetFocusObject(pThis->m_pObjPress);
            }

            if (pThis->m_pObjPress && pSave == pThis->m_pObjPress)
            {
                UIMSG msg;
                msg.pMsgTo = pThis->m_pObjHover->GetIMessage();
                msg.message = WM_LBUTTONDOWN;
                msg.wParam  = wParam;
                msg.lParam  = lParam;
                ::UISendMessage(&msg, 0, pbHandled);      // �п��ܵ���m_pObjPressΪNULL��
            }
        }
    }

    template <class T>
    void _OnLButtonDBClick(WPARAM wParam, LPARAM lParam, BOOL* pbHandled, T* pThis)
    {
        BOOL bHandled = FALSE;
        if (pThis->m_pObjPress)
        {
            UIMSG msg;
            msg.pMsgTo = pThis->m_pObjPress->GetIMessage();
            msg.message = WM_LBUTTONDBLCLK;
            msg.wParam  = wParam;
            msg.lParam  = lParam;

            ::UISendMessage(&msg, 0, pbHandled);
            if (pThis->m_pObjPress && pThis->m_pObjPress->IsTabstop())
            {
                pThis->SetFocusObject(pThis->m_pObjPress);
            }
            bHandled = TRUE;
        }
        else if (pThis->m_pObjHover && pThis->m_pObjHover->IsEnable())
        {
            pThis->SetPressObject(pThis->m_pObjHover);

            UIMSG msg;
            msg.pMsgTo = pThis->m_pObjHover->GetIMessage();
            msg.message = WM_LBUTTONDBLCLK;
            msg.wParam  = wParam;
            msg.lParam  = lParam;

            ::UISendMessage(&msg, 0, pbHandled);
            if (pThis->m_pObjPress && pThis->m_pObjPress->IsTabstop())
            {
                pThis->SetFocusObject(pThis->m_pObjPress);
            }
            bHandled = TRUE;
        }

        if (pbHandled)
            *pbHandled = bHandled;
    }


    template <class T>
    void _OnLButtonUp(WPARAM w, LPARAM l, T* pThis)
    {
        if (pThis->m_pObjPress)
        {
            if (pThis->m_pObjPress)
            {
                Object*  pSavePress = pThis->m_pObjPress;

                UIMSG  msg;
                msg.pMsgTo = pSavePress->GetIMessage();
                msg.message = WM_LBUTTONUP;
                msg.wParam  = w;
                msg.lParam  = l;
                ::UISendMessage(&msg);

				// SetPressObject(NULL)����UISendMessageǰ�棬
				// �ᵼ��checkbutton�����ˢ��
				if (pThis->m_pObjPress == pSavePress)
					pThis->SetPressObject(NULL);
				else
					pSavePress->SetPress(false);
            }
        }
    }

}