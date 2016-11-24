#include "stdafx.h"
#include "sync_window.h"

SyncWindow::SyncWindow()
{
    m_bSendByDeferWindowPos = false;
    m_hHostWnd = NULL;
    m_hWnd = NULL;

    m_nAnchorMagnetCapability = 10;
    m_bAnchorToScreenEdge = false;
    m_bAnchorToOtherSize = false;
    m_bSizeMove = false;
}

void SyncWindow::SetHwnd(HWND hWnd)
{
    m_hWnd = hWnd;
}

void  SyncWindow::_OnShowWindow(WPARAM wParam, LPARAM lParam)
{
    int nSize = (int)m_vecAnchorItems.size();
    for (int i = 0; i < nSize; i++)
    {
        HWND hWnd = m_vecAnchorItems[i].m_hWnd;
        if (wParam)
        {
            ::ShowWindow(hWnd, SW_SHOWNOACTIVATE);
        }
        else
        {
            ::ShowWindow(hWnd, SW_HIDE);
        }
    }
}

void SyncWindow::_OnWindowPosChanging(LPWINDOWPOS lpWndPos, BOOL& bHandled)
{
    if (lpWndPos->flags & SWP_SHOWWINDOW)
    {
        if (m_hHostWnd)
        {
            int a = 0;
        }
    }
    if (lpWndPos->flags & SWP_NOMOVE && lpWndPos->flags & SWP_NOSIZE)
    {
        // ������������жϻᵼ��HostWindow�ڴ���С���ָ�ʱ��SiteWindow������һ��ָ�
        bHandled = FALSE;
        return;
    }

    bHandled = TRUE;
    // ���ж��Լ���ǰ�Ƿ�������������ڣ�Ȼ���ٴ����Լ����ƶ���
    if (m_hHostWnd)
    {
        if (this->OnSiteWindowPosChanging(lpWndPos))
            return;
    }

    if (m_vecAnchorItems.size() > 0)
    {
        this->OnHostWindowPosChanging(lpWndPos);
    }
    else
    {
        bHandled = FALSE;
    }
}

// ����ͬ��һ�Σ�ģ��һ�Ѵ��ڴ�С�ı�
void SyncWindow::SyncNow()
{
    WINDOWPOS wndPos = { 0 };
    wndPos.hwnd = m_hWnd;

    CRect rc;
    GetWindowRect(m_hWnd, &rc);

    vector<SyncWindowData>::iterator  iter = m_vecAnchorItems.begin();
    vector<SyncWindowData>::iterator  iterEnd = m_vecAnchorItems.end();
    for (; iter != iterEnd; iter++)
    {
        HWND hWnd = iter->m_hWnd;
        if (hWnd /*&& IsWindowVisible(hWnd)*/) //  ���ɼ�ҲҪ����
        {
            HDWP hdwpNull = NULL;
            HandleWindowAnchor(hdwpNull, rc, *iter);
        }
    }

//     wndPos.x = rc.left;
//     wndPos.y = rc.top;
//     wndPos.cx = rc.Width();
//     wndPos.cy = rc.Height();
//     wndPos.flags = 0;
// 
//     OnHostWindowPosChanging(&wndPos);
}

void SyncWindow::OnHostWindowPosChanging(LPWINDOWPOS lpWndPos)
{
    if (m_bSendByDeferWindowPos)
    {
        m_bSendByDeferWindowPos = false;  // DeferWindowPos���ݹ�������Ϣ�����С�
        return;
    }

    // ���㵱ǰ���ڵ��������С
    CRect rc;
    this->GetRectByWNDPOS(lpWndPos, &rc);

    do
    {
        // ע��DeferWindowPos��֧��SWP_NOSENDCHANGING���ᱨ��Ч����������������������һ��
        //     bool ��������ֹ��ѭ��

        // ������Ҫͬ���ƶ��Ĵ������� 
        int nCount = this->GetAvailableHWNDCount();

        // +1 �Լ������m_hWnd.���ֲ㴰���Լ�ά���Լ��Ĵ�С�ı䣩
        bool IsLayeredWindow = lpWndPos->flags & SWP_LAYEREDWINDOW_SIZEMOVE ? true : false;
        if (!IsLayeredWindow)
            nCount += 1;

        HDWP hdwp = BeginDeferWindowPos(nCount);
        if (NULL == hdwp) break;

        if (!IsLayeredWindow)
        {
            UINT nFlag = lpWndPos->flags & ~0x01000000;  // ?? �����˵��ñ�ǻᵼ��DeferWindodwPosʧ�ܣ���֪��Ϊʲô
            hdwp = ::DeferWindowPos(hdwp,
                m_hWnd,
                lpWndPos->hwndInsertAfter,
                lpWndPos->x,
                lpWndPos->y,
                lpWndPos->cx,
                lpWndPos->cy,
                nFlag/*|SWP_NOSENDCHANGING*/);
            if (NULL == hdwp)
            {
                // HOST�������С����ԭ��ʱ���ʧ��
                break;
            }
        }

        bool bLoopResult = true;
        vector<SyncWindowData>::iterator  iter = m_vecAnchorItems.begin();
        vector<SyncWindowData>::iterator  iterEnd = m_vecAnchorItems.end();
        for (; iter != iterEnd; iter++)
        {
            HWND hWnd = iter->m_hWnd;
            if (hWnd /*&& IsWindowVisible(hWnd)*/ && iter->m_bAnchorOn)
            {
                hdwp = HandleWindowAnchor(hdwp, rc, *iter);
                if (NULL == hdwp)
                {
                    bLoopResult = false;
                    break;
                }
            }
        }
        if (false == bLoopResult)  break;

        if (!IsLayeredWindow)
        {
            m_bSendByDeferWindowPos = true;
        }
        BOOL bRet = EndDeferWindowPos(hdwp);
        UIASSERT(bRet);

        // ע�������ﲻ�����SWP_NOZORDER������ᵼ�´�����קʱZ������ȷ
        lpWndPos->flags |= SWP_NOMOVE | SWP_NOSIZE; // �ܾ������޸ģ�ʹ��DeferWindowPos����Ϣ

        return;
    } while (0);

    // ʧ���ˣ�����Ĭ�Ϲ���
    vector<SyncWindowData>::iterator  iter = m_vecAnchorItems.begin();
    vector<SyncWindowData>::iterator  iterEnd = m_vecAnchorItems.end();
    for (; iter != iterEnd; iter++)
    {
        HWND hWnd = iter->m_hWnd;
        if (hWnd /*&& IsWindowVisible(hWnd)*/) //  ���ɼ�ҲҪ����
        {
            HDWP hdwpNull = NULL;
            HandleWindowAnchor(hdwpNull, rc, *iter);
        }
    }
}

void SyncWindow::OnHostWindowPosChanging()
{
    UIASSERT(NULL != m_hHostWnd);
    m_bSendByDeferWindowPos = true;
}

bool SyncWindow::OnSiteWindowPosChanging(LPWINDOWPOS lpWndPos)
{
    if (m_bSendByDeferWindowPos)  // ���ε��ƶ�����ΪHOST����deferwindowpos�����ģ�����
    {
        m_bSendByDeferWindowPos = false;
        return false;
    }

    // �������������������ק���죬�򲻴������細�ڸմ�����
    // ����ֲ㴰������ק�ı��Сʱ��Ŀǰ������ENTERSIZEMOVE��Ϣ������ֱ�Ӵ���
    if (m_bSizeMove || lpWndPos->flags & SWP_LAYEREDWINDOW_SIZEMOVE)
    {
        bool bSize = (lpWndPos->flags & SWP_NOSIZE) ? false : true;   // ���ֶβ���ȷ����ʹֻ�϶����ڣ�Ҳû��SWP_NOSIZE��־
        bool bMove = (lpWndPos->flags & SWP_NOMOVE) ? false : true;
        if (!bSize && !bMove)
            return false;

        SyncWindowData data;
        data.m_hWnd = GetMyHWND();

        CRect rcHost, rcSite, rcSiteOld;
        ::GetWindowRect(m_hHostWnd, &rcHost);
        ::GetWindowRect(data.m_hWnd, &rcSiteOld);
        this->GetRectByWNDPOS(lpWndPos, &rcSite);

        if (rcSiteOld.Width() != lpWndPos->cx || rcSiteOld.Height() != lpWndPos->cy)
        {
            bSize = true;
        }
        else
        {
            bSize = false;
        }
        bool bRet = this->CheckRectAnchor(&rcHost, &rcSite, bSize, &data);

        if (bRet)  // ճ���ˣ��޸Ĵ��ڵ�λ��
        {
            lpWndPos->x = rcSite.left;
            lpWndPos->y = rcSite.top;
            lpWndPos->cx = rcSite.Width();
            lpWndPos->cy = rcSite.Height();
        }

        SendMessage(m_hHostWnd, UI_WM_SYNC_WINDOW, MODIFY_SYNC_WINDOW, (LPARAM)&data);
    }
    return false;
}

//
// �ж����������ճ�Ϲ�ϵ�����ճ�ϣ����޸�prcSize��ֵ
//
// Return
//    �������������Ƿ�ճ����
//
// Remark
//  Q1. ������ֱ�����MOVE������SIZE����ǰ�������ڵĸߺͿ��Ա�lpWindowPos�еĸߺͿ������������Ƿ�����change size.
//      TODO: ���ܻ᲻׼ȷ��...
//
bool SyncWindow::CheckRectAnchor(const CRect* prcHost, CRect* prcSite, bool bChangeSize, SyncWindowData* pData)
{
    bool bLeftLeft = (abs(prcSite->left - prcHost->left) < m_nAnchorMagnetCapability);
    bool bLeftRight = (abs(prcSite->left - prcHost->right) < m_nAnchorMagnetCapability);
    bool bRightLeft = (abs(prcSite->right - prcHost->left) < m_nAnchorMagnetCapability);
    bool bRightRight = (abs(prcSite->right - prcHost->right) < m_nAnchorMagnetCapability);
    bool bX = bLeftLeft || bLeftRight || bRightRight || bRightLeft ||
        (prcSite->left > prcHost->left && prcSite->left<prcHost->right) ||
        (prcSite->right>prcHost->left && prcSite->right<prcHost->right) ||
        (prcHost->left>prcSite->left && prcHost->left<prcSite->right) ||
        (prcHost->right>prcSite->left && prcHost->right < prcSite->right);

    bool bTopTop = (abs(prcHost->top - prcSite->top) < m_nAnchorMagnetCapability);
    bool bTopBottom = (abs(prcSite->top - prcHost->bottom) < m_nAnchorMagnetCapability);
    bool bBottomTop = (abs(prcSite->bottom - prcHost->top) < m_nAnchorMagnetCapability);
    bool bBottomBottom = (abs(prcSite->bottom - prcHost->bottom) < m_nAnchorMagnetCapability);
    bool bY = bTopTop || bTopBottom || bBottomTop || bBottomBottom ||
        (prcSite->top > prcHost->top && prcSite->top<prcHost->bottom) ||
        (prcSite->bottom>prcHost->top && prcSite->bottom<prcHost->bottom) ||
        (prcHost->top>prcSite->top && prcHost->top<prcSite->bottom) ||
        (prcHost->bottom>prcSite->top && prcHost->bottom < prcSite->bottom);

    bool bXAnchored = false;
    bool bYAnchored = false;
    int  nAnchorType = ANCHOR_NONE;
    if (bY)
    {
        if (bLeftLeft)
        {
            if (bChangeSize)
            {
                prcSite->left = prcHost->left;
            }
            else
            {
                int W = prcSite->Width();
                prcSite->left = prcHost->left;
                prcSite->right = prcSite->left + W;
            }
            bXAnchored = true;
            nAnchorType |= ANCHOR_LEFT;
        }
        else if (bLeftRight)
        {
            if (bChangeSize)
            {
                prcSite->left = prcHost->right;
            }
            else
            {
                int W = prcSite->Width();
                prcSite->left = prcHost->right;
                prcSite->right = prcSite->left + W;
            }
            bXAnchored = true;
            nAnchorType |= ANCHOR_OUT_RIGHT;
        }
        else if (bRightLeft)
        {
            if (bChangeSize)
            {
                prcSite->right = prcHost->left;
            }
            else
            {
                int W = prcSite->Width();
                prcSite->right = prcHost->left;
                prcSite->left = prcSite->right - W;
            }
            bXAnchored = true;
            nAnchorType |= ANCHOR_OUT_LEFT;
        }
        else if (bRightRight)
        {
            if (bChangeSize)
            {
                prcSite->right = prcHost->right;
            }
            else
            {
                int W = prcSite->Width();
                prcSite->right = prcHost->right;
                prcSite->left = prcSite->right - W;
            }
            bXAnchored = true;
            nAnchorType |= ANCHOR_RIGHT;
        }
    }

    if (bX)
    {
        if (bTopTop)
        {
            if (bChangeSize)
            {
                prcSite->top = prcHost->top;
            }
            else
            {
                int H = prcSite->Height();
                prcSite->top = prcHost->top;
                prcSite->bottom = prcSite->top + H;
            }
            bYAnchored = true;
            nAnchorType |= ANCHOR_TOP;
        }
        else if (bTopBottom)
        {
            if (bChangeSize)
            {
                prcSite->top = prcHost->bottom;
            }
            else
            {
                int H = prcSite->Height();
                prcSite->top = prcHost->bottom;
                prcSite->bottom = prcSite->top + H;
            }
            bYAnchored = true;
            nAnchorType |= ANCHOR_OUT_BOTTOM;
        }
        else if (bBottomTop)
        {
            if (bChangeSize)
            {
                prcSite->bottom = prcHost->top;
            }
            else
            {
                int H = prcSite->Height();
                prcSite->bottom = prcHost->top;
                prcSite->top = prcSite->bottom - H;
            }
            bYAnchored = true;
            nAnchorType |= ANCHOR_OUT_TOP;
        }
        else if (bBottomBottom)
        {
            if (bChangeSize)
            {
                prcSite->bottom = prcHost->bottom;
            }
            else
            {
                int H = prcSite->Height();
                prcSite->bottom = prcHost->bottom;
                prcSite->top = prcSite->bottom - H;
            }
            bYAnchored = true;
            nAnchorType |= ANCHOR_BOTTOM;
        }
    }

    // ����nAnchorTypeֻͣ����һ��������һ������û�и�ֵ�����⡣
    if (0 != nAnchorType)
    {
        pData->m_nMask |= SWDS_MASK_ANCHORTYPE | SWDS_MASK_ANCHORON | SWDS_MASK_ANCHORDATA;
        pData->m_nAnchorType = nAnchorType;
        pData->m_bAnchorOn = true;
        pData->m_rcAnchorData.Width = -1;
        pData->m_rcAnchorData.Height = -1;
        if (false == bXAnchored)
        {
            pData->m_nAnchorType |= ANCHOR_LEFT;
            pData->m_rcAnchorData.xOffset = prcSite->left - prcHost->left;
        }
        else if (false == bYAnchored)
        {
            pData->m_nAnchorType |= ANCHOR_TOP;
            pData->m_rcAnchorData.yOffset = prcSite->top - prcHost->top;
        }

        return true;
    }
    else
    {
        pData->m_nMask |= SWDS_MASK_ANCHORON;
        pData->m_bAnchorOn = false;

        return false;
    }

    return false;
}

void SyncWindow::GetRectByWNDPOS(LPWINDOWPOS lpWndPos, CRect* prc)
{
    int   x = 0, y = 0, w = 0, h = 0;
    ::GetWindowRect(GetMyHWND(), prc);
    if (lpWndPos->flags & SWP_NOMOVE)
    {
        x = prc->left;
        y = prc->top;
    }
    else
    {
        x = lpWndPos->x;
        y = lpWndPos->y;
    }
    if (lpWndPos->flags & SWP_NOSIZE)
    {
        w = prc->right - prc->left;
        h = prc->bottom - prc->top;
    }
    else
    {
        w = lpWndPos->cx;
        h = lpWndPos->cy;
    }
    prc->SetRect(x, y, x + w, y + h);
}

HDWP SyncWindow::HandleWindowAnchor(HDWP& hdwp, const CRect& _rcSrcWindow, const SyncWindowData& rData)
{
    CRect rcSrcWindow(_rcSrcWindow);
    if (rData.m_nAnchorType == ANCHOR_CUSTOM)
    {
        return (HDWP)::SendMessage(rData.m_hWnd, 
            UI_WM_SYNC_WINDOWPOSCHANING, (WPARAM)hdwp, (LPARAM)&rcSrcWindow);
    }

    if (rData.m_nAnchorType & ANCHOR_CLIENTREGION)
    {
        SendMessage(m_hWnd, WM_NCCALCSIZE, TRUE, (LPARAM)&rcSrcWindow);
    }

    int x = 0, y = 0, w = 0, h = 0;
    int nFlag = SWP_NOZORDER | SWP_NOACTIVATE;

    CRect  rcThisWindow;
    ::GetWindowRect(rData.m_hWnd, &rcThisWindow);
    w = rcThisWindow.Width();
    h = rcThisWindow.Height();

    // �������뷽ʽ
    bool bLeft = rData.m_nAnchorType&ANCHOR_LEFT ? true : false;
    bool bRight = rData.m_nAnchorType&ANCHOR_RIGHT ? true : false;
    bool bTop = rData.m_nAnchorType&ANCHOR_TOP ? true : false;
    bool bBottom = rData.m_nAnchorType&ANCHOR_BOTTOM ? true : false;

    bool bOutLeft = rData.m_nAnchorType&ANCHOR_OUT_LEFT ? true : false;
    bool bOutRight = rData.m_nAnchorType&ANCHOR_OUT_RIGHT ? true : false;
    bool bOutTop = rData.m_nAnchorType&ANCHOR_OUT_TOP ? true : false;
    bool bOutBottom = rData.m_nAnchorType&ANCHOR_OUT_BOTTOM ? true : false;

    if (bLeft && bRight)
    {
        x = rcSrcWindow.left + rData.m_rcAnchorData.xOffset;
        int xRight = rcSrcWindow.right - rData.m_rcAnchorData.xOffset2;
        w = xRight - x;
    }
    else
    {
        if (-1 != rData.m_rcAnchorData.Width)
            w = rData.m_rcAnchorData.Width;

        if (bLeft)
            x = rcSrcWindow.left + rData.m_rcAnchorData.xOffset;
        else if (bRight)
            x = rcSrcWindow.right - rData.m_rcAnchorData.xOffset - w;
        else if (bOutLeft)
            x = rcSrcWindow.left - rData.m_rcAnchorData.xOffset - w;
        else if (bOutRight)
            x = rcSrcWindow.right + rData.m_rcAnchorData.xOffset;
        else
            x = rcThisWindow.left;
    }

    if (bTop && bBottom)
    {
        y = rcSrcWindow.top + rData.m_rcAnchorData.yOffset;
        int yBottom = rcSrcWindow.bottom - rData.m_rcAnchorData.yOffset2;
        h = yBottom - y;
    }
    else
    {
        if (-1 != rData.m_rcAnchorData.Height)
            h = rData.m_rcAnchorData.Height;

        if (bTop)
            y = rcSrcWindow.top + rData.m_rcAnchorData.yOffset;
        else if (bBottom)
            y = rcSrcWindow.bottom - rData.m_rcAnchorData.yOffset - h;
        else if (bOutTop)
            y = rcSrcWindow.top - rData.m_rcAnchorData.yOffset - h;
        else if (bOutBottom)
            y = rcSrcWindow.bottom + rData.m_rcAnchorData.yOffset;
        else
            y = rcThisWindow.top;
    }
    if (NULL == hdwp)
    {
        ::SetWindowPos(rData.m_hWnd, NULL, x, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
        return NULL;
    }
    else
    {
        SendMessage(rData.m_hWnd, UI_WM_SYNC_WINDOW, HOST_WINDOW_POSCHANGING, 0);
        return ::DeferWindowPos(hdwp, rData.m_hWnd, NULL, x, y, w, h, nFlag);
    }
}

void SyncWindow::_OnNcDestroy()
{
    // �Լ�������ʱ�����������
    if (m_hHostWnd)  // ֪ͨ���������Ƴ��Լ�
    {
        SyncWindowData data;
        data.m_hWnd = GetMyHWND();
        ::SendMessage(m_hHostWnd, UI_WM_SYNC_WINDOW, REMOVE_SYNC_WINDOW, (LPARAM)&data);
    }

    // ֪ͨ���������Լ��Ĵ��ڣ���������������
    this->ClearAnchorItem();
}

bool SyncWindow::AddAnchorItem(const SyncWindowData& data)
{
    if (NULL == data.m_hWnd)
        return false;

    if (GetWindowLong(data.m_hWnd, GWL_STYLE) & WS_CHILD)
        return false;

    if (-1 != this->FindAnchorItem(data.m_hWnd))
        return false;

    m_vecAnchorItems.push_back(data);
    ::SendMessage(data.m_hWnd, UI_WM_SYNC_WINDOW, ADD_SYNC_WINDOW_RESULT, (LPARAM)GetMyHWND());
    return true;
}

bool SyncWindow::RemoveAnchorItem(HWND hWnd)
{
    vector<SyncWindowData>::iterator  iter = m_vecAnchorItems.begin();
    vector<SyncWindowData>::iterator  iterEnd = m_vecAnchorItems.end();
    for (; iter != iterEnd; iter++)
    {
        if (iter->m_hWnd == hWnd)
        {
            m_vecAnchorItems.erase(iter);
            return true;
        }
    }
    ::SendMessage(hWnd, UI_WM_SYNC_WINDOW, REMOVE_SYNC_WINDOW_RESULT, (LPARAM)GetMyHWND());
    return false;
}

bool SyncWindow::ModifyAnchorItem(const SyncWindowData& data)
{
    int nIndex = this->FindAnchorItem(data.m_hWnd);
    if (-1 == nIndex)
        return false;

    if (data.m_nMask & SWDS_MASK_ANCHORON)
        m_vecAnchorItems[nIndex].m_bAnchorOn = data.m_bAnchorOn;
    if (data.m_nMask & SWDS_MASK_ANCHORTYPE)
        m_vecAnchorItems[nIndex].m_nAnchorType = data.m_nAnchorType;
    if (data.m_nMask & SWDS_MASK_ANCHORDATA)
        memcpy(&m_vecAnchorItems[nIndex].m_rcAnchorData, &data.m_rcAnchorData, sizeof(AnchorData));

    ::SendMessage(data.m_hWnd, UI_WM_SYNC_WINDOW, MODIFY_SYNC_WINDOW_RESULT, (LPARAM)GetMyHWND());
    return true;
}

void SyncWindow::OnAddAnchorItem(HWND hHostWnd) // �Լ���Ϊ������
{
    UIASSERT(NULL == m_hHostWnd);
    m_hHostWnd = hHostWnd;
}

void SyncWindow::OnRemoveAnchorItem(HWND hHostWnd) // �Լ���Ϊ������
{
    UIASSERT(hHostWnd == m_hHostWnd);
    m_hHostWnd = NULL;
}

void SyncWindow::OnModifyAnchorItem(HWND hHostWnd) // �Լ���Ϊ������
{
    UIASSERT(NULL != m_hHostWnd);
}

bool SyncWindow::ClearAnchorItem()
{
    int nSize = (int)m_vecAnchorItems.size();
    for (int i = 0; i < nSize; i++)
    {
        ::SendMessage(m_vecAnchorItems[i].m_hWnd, UI_WM_SYNC_WINDOW, HOST_WINDOW_DESTROYED, (LPARAM)GetMyHWND());
    }
    m_vecAnchorItems.clear();
    return true;
}

int SyncWindow::FindAnchorItem(HWND hWnd)
{
    int nSize = (int)m_vecAnchorItems.size();
    for (int i = 0; i < nSize; i++)
    {
        if (m_vecAnchorItems[i].m_hWnd == hWnd)
        {
            return i;
        }
    }
    return -1;
}

int SyncWindow::GetAvailableHWNDCount() // �����ж��Ƿ�ɼ�����ʱ���ɼ�Ҳ��Ҫ������Ҫ��Ȼ����ʾʱ��λ�þͲ���ȷ��
{
    int nCount = 0;
    int nSize = (int)m_vecAnchorItems.size();
    for (int i = 0; i < nSize; i++)
    {
        HWND hWnd = m_vecAnchorItems[i].m_hWnd;
        if (hWnd /*&& ::IsWindowVisible(hWnd)*/ && m_vecAnchorItems[i].m_bAnchorOn)
        {
            nCount++;
        }
    }
    return nCount;
}

void SyncWindow::HideAllAnchorItem()
{
    int nSize = (int)m_vecAnchorItems.size() + 1;
    HDWP hdwp = ::BeginDeferWindowPos(nSize);

    hdwp = ::DeferWindowPos(hdwp, m_hWnd, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);

    vector<SyncWindowData>::iterator  iter = m_vecAnchorItems.begin();
    vector<SyncWindowData>::iterator  iterEnd = m_vecAnchorItems.end();
    for (; iter != iterEnd; iter++)
    {
        hdwp = ::DeferWindowPos(hdwp, iter->m_hWnd, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
    }

    ::EndDeferWindowPos(hdwp);
}

bool SyncWindow::IsSizeMoveIng()
{
    return m_bSizeMove;
}

HWND SyncWindow::GetMyHWND()
{
    return m_hWnd;
}

void SyncWindow::_OnExitSizeMove()
{
    m_bSizeMove = false;
}

void SyncWindow::_OnEnterSizeMove()
{
    m_bSizeMove = true;
}
