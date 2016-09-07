#include "stdafx.h"
#include "dropsource.h"

namespace UI
{

void  CreateDropSourceInstance(IDropSource**  pp)
{
    if (NULL == pp)
        return;

    CDropSource* p = new CDropSource;
    p->AddRef();

    *pp = p;
}

CDropSource::CDropSource()
{
    m_lRef = 0;
    m_pDragFeedback = NULL;
}
CDropSource::~CDropSource()
{
    m_pDragFeedback = NULL;
}


HRESULT STDMETHODCALLTYPE CDropSource::QueryInterface(REFIID riid, void **ppvObject)
{ 
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropSource))
    {
        AddRef();
        *ppvObject = static_cast<IDropSource*>(this);
        return S_OK;
    }
    else if (IsEqualIID(riid, IID_IDropSourceEx))
    {
        AddRef();
        *ppvObject = static_cast<IDropSourceEx*>(this);
        return S_OK;
    }

    return E_NOINTERFACE;
}
ULONG STDMETHODCALLTYPE CDropSource::AddRef(void)
{
    return ++m_lRef;
}
ULONG STDMETHODCALLTYPE CDropSource::Release(void)
{
    --m_lRef; 
    if (0 == m_lRef)
    {
        delete this;
        return 0;
    } 
    return m_lRef;
}

//////////////////////////////////////////////////////////////////////////
// 
// bEscapePressed
// States whether the ESC key has been pressed since the last call to COleDropSource::QueryContinueDrag.
// 
// dwKeyState
//  Contains the state of the modifier keys on the keyboard. This 
//  is a combination of any number of the following: MK_CONTROL, 
//  MK_SHIFT, MK_ALT, MK_LBUTTON, MK_MBUTTON, and MK_RBUTTON.
// 
// Return Value
//  DRAGDROP_S_CANCEL if the ESC key or right button is pressed, 
//  or left button is raised before dragging starts. DRAGDROP_S_DROP 
//  if a drop operation should occur. Otherwise S_OK.
//
HRESULT STDMETHODCALLTYPE CDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
    if (fEscapePressed)
        return DRAGDROP_S_CANCEL;

    // ����ק�����У���ʵ�û��ǿ��Ի�������Ҽ��ģ���˵�����������ͷ�ʱ����Ϊdrop
    if (0== (grfKeyState&(MK_LBUTTON|MK_RBUTTON)))
        return DRAGDROP_S_DROP;

    return S_OK;
}

// Return Value
//  Returns DRAGDROP_S_USEDEFAULTCURSORS if dragging is in progress, NOERROR if it is not.
HRESULT STDMETHODCALLTYPE CDropSource::GiveFeedback(DWORD dwEffect)
{
    // ֪ͨ CDragFeedback ���µ�ǰ�ı�
    if (m_pDragFeedback)
    {
        HRESULT hr = m_pDragFeedback->GiveFeedback(dwEffect);
        if (SUCCEEDED(hr))
            return hr;
    }

    return DRAGDROP_S_USEDEFAULTCURSORS;
}

void  CDropSource::SetDragFeedback(IDragFeedback* p) 
{
    m_pDragFeedback = p;
}
IDragFeedback*  CDropSource::GetDragFeedback()
{
    return m_pDragFeedback;
}

}