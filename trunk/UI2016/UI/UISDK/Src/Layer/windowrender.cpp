#include "stdafx.h"
#include "windowrender.h"
#include "Src\Base\Object\object.h"
#include "Src\UIObject\Window\windowbase.h"
#include "Inc\Interface\iwndtransmode.h"
#include "Inc\Interface\irenderlayer.h"
#include "Src\Base\Attribute\attribute.h"
#include "Src\Base\Attribute\enum_attribute.h"

#include "Src\Layer\hardware_compositor.h"
#include "Src\Layer\software_compositor.h"

using namespace UI;

WindowRender::WindowRender(WindowBase*  p)
{
    m_eGRL = GRAPHICS_RENDER_LIBRARY_TYPE_GDI;
    m_pWindow = p;
    m_lRefCanCommit = 0;
    
    m_pIWindowRender = NULL;
//	m_pHardwareComposition = NULL;
//    m_bNeedRebuildGpuLayerTree = true;
    m_bNeedAlphaChannel = true;

	m_pCompositor = NULL;
}

WindowRender::~WindowRender()
{
    SAFE_DELETE(m_pCompositor);
    SAFE_DELETE(m_pIWindowRender);
//	SAFE_RELEASE(m_pHardwareComposition);
}

IWindowRender*  WindowRender::GetIWindowRender()
{
    if (!m_pIWindowRender)
        m_pIWindowRender = new IWindowRender(this);

    return m_pIWindowRender;
}

void  WindowRender::OnSerialize(SERIALIZEDATA* pData)
{
	AttributeSerializer s(pData, TEXT("WindowRender"));

	s.AddEnum(XML_WINDOW_GRAPHICS_RENDER_LIBRARY, *(long*)&m_eGRL)
		->FillGraphicsRenderLibraryData()
		->SetDefault(GRAPHICS_RENDER_LIBRARY_TYPE_GDI);

    s.AddBool(XML_WINDOW_NEED_ALPHACHANNEL, m_bNeedAlphaChannel)
		->SetDefault(true);
}

// IGpuRenderLayer*  WindowRender::CreateGpuLayerTexture(RenderLayer* p)
// {
// 	if (NULL == m_pHardwareComposition)
// 	{
// 		if (m_pWindow->IsGpuComposite())
// 		{
// 			m_pHardwareComposition = CreateHardwareComposition(m_pWindow->m_hWnd);
// 		}
// 	}
// 
// 	IGpuRenderLayer* pGpuTexture = NULL;
// 	if (m_pHardwareComposition)
// 	{
// 		pGpuTexture = m_pHardwareComposition->CreateLayerTexture();
// 		if (p->GetCreateObject() == m_pWindow)
// 			m_pHardwareComposition->SetRootLayerSurface(pGpuTexture);
// 
// 		m_bNeedRebuildGpuLayerTree = true;
// 	}
// 
// 	return pGpuTexture;
// }
//
// void  WindowRender::OnRenderLayerDestroy(RenderLayer* p)
// {
//     if (m_pHardwareComposition)
//     {
//         m_bNeedRebuildGpuLayerTree = true;
//     }
// }   

// IRenderTarget* û�����ü�������
// ����Ȼ����Release�����ͷţ�delete�� 
bool  WindowRender::CreateRenderTarget(IRenderTarget** pp)
{
    if (!pp)
        return false;

    *pp = UICreateRenderTarget(m_pWindow->GetIUIApplication(), m_eGRL, m_bNeedAlphaChannel);
    return true;
}

// void  WindowRender::UpdateWindow(HDC hDC, RECT* prcDamageArray, uint nCount)
// {
//     CRect rcClient;
//     if (!prcDamageArray)
//     {
// 		::GetClientRect(m_pWindow->m_hWnd, &rcClient);
// 		prcDamageArray = &rcClient;
// 		nCount = 1;
// 	}
// 
// 	RenderLayer* pRenderLayer = m_pWindow->GetRenderLayer2();
// 	if (!pRenderLayer)
// 		return;
// 
// 	pRenderLayer->UpdateLayer(prcDamageArray, nCount);
// 	Commit(hDC, prcDamageArray, nCount);
// }

void WindowRender::OnWindowSize(UINT nWidth, UINT nHeight)
{
//     if (m_pHardwareComposition)
//         m_pHardwareComposition->Resize(nWidth, nHeight);

    if (m_pCompositor)
        m_pCompositor->Resize(nWidth, nHeight);
}

void  WindowRender::SetCanCommit(bool b)
{
    if (b)
        m_lRefCanCommit --;
    else
        m_lRefCanCommit ++;
}

bool  WindowRender::CanCommit() 
{ 
    return 0 == m_lRefCanCommit; 
}

// void  WindowRender::DrawIncrement()
// {
//     m_pCompositor->UpdateDirty(NULL);
//     
// }
//
// void  WindowRender::Commit(HDC hDC, RECT* prc, int nCount)
// {
//     if (!CanCommit())
//         return;
// 
//     if (m_pHardwareComposition)
//     {
//         HardComposite();
//     }
//     else
//     {
//         m_pWindow->CommitDoubleBuffet2Window(hDC, prc, prc?nCount:0);
//     }
// }
// 
// void  WindowRender::HardComposite()
// {
//     if (m_pHardwareComposition)
//     {
//         if (m_bNeedRebuildGpuLayerTree)
//             this->RebuildCompositingLayerTree();
// 
//         if (m_pHardwareComposition->BeginCommit())
//         {
//             RenderLayer*  pRootLayer = m_pWindow->GetRenderLayer2();
// 
// 			RECT rcClip;
// 			::GetClientRect(m_pWindow->GetHWND(), &rcClip);
// 
// 			GpuLayerCommitContext context;
// 			context.SetClipRect(&rcClip);
//             pRootLayer->Compositor2Gpu(context);
//             m_pHardwareComposition->EndCommit();
//         }
//     }
// }
// 
// void  WindowRender::OnHardCompositeChanged(bool bEnable)
// {
// 	if (m_bNeedRebuildGpuLayerTree)
// 		this->RebuildCompositingLayerTree();
// 
// 	RenderLayer*  pRootLayer = m_pWindow->GetRenderLayer2();
//     if (pRootLayer)
//     {
// 	    pRootLayer->OnHardCompositeChanged(bEnable);
//     }
// }
// 
// void  WindowRender::SetNeedRebuildCompositingLayerTree()
// {
// 	m_bNeedRebuildGpuLayerTree = true;
// }
// 
// // ͨ��renderlayer��������gpulayer��
// void  WindowRender::RebuildCompositingLayerTree()
// {
//     if (!m_bNeedRebuildGpuLayerTree)
//         return;
// 
//     RenderLayer*  pRootLayer = m_pWindow->GetRenderLayer2();
//     if (!pRootLayer)
//         return;
// 
//     pRootLayer->ClearChildren();
//     rebuildCompositingLayerTree(m_pWindow,pRootLayer);
// 
//     m_bNeedRebuildGpuLayerTree = false;
// }
// void  WindowRender::rebuildCompositingLayerTree(
//         Object* pParent,
//         RenderLayer* pParentLayer)
// {
//     if (!pParent || !pParentLayer)
//         return;
// 
//     Object*  pChild = NULL;
//     RenderLayer*  pLayer = NULL;
// 
//     while (pChild = pParent->EnumAllChildObject(pChild))
//     {
//         pLayer = pChild->GetSelfRenderLayer2();
//         if (pLayer)
//         {
// //             IGpuRenderLayer*  pGpuLayerTexture = pLayer->GetGpuTexture();
// //             UIASSERT(pGpuLayerTexture);
// 
// //             pGpuLayerTexture->ClearChildren();
// //             pParentLayer->AddChild(pGpuLayerTexture);
// // #ifdef _DEBUG
// //             POINT pt = pChild->GetWindowPoint();
// //             pGpuLayerTexture->SetWindowPos(pt.x, pt.y);
// // #endif            
// //             rebuildCompositingLayerTree(pChild, pGpuLayerTexture);
// 
//             pLayer->ClearChildren();
//             pParentLayer->AddChild(pLayer);
//             rebuildCompositingLayerTree(pChild, pLayer);
//         }
//         else
//         {
//             rebuildCompositingLayerTree(pChild, pParentLayer);
//         }
//     }
// }

void  WindowRender::SetGraphicsRenderType(GRAPHICS_RENDER_LIBRARY_TYPE  eTpye)
{
	// ���ڴ��ڴ���֮ǰ��������
	m_eGRL = eTpye;
}
GRAPHICS_RENDER_LIBRARY_TYPE  WindowRender::GetGraphicsRenderType()
{
	return m_eGRL;
}
bool  WindowRender::GetRequireAlphaChannel()
{
    return m_bNeedAlphaChannel;

	// return m_eGRL==GRAPHICS_RENDER_LIBRARY_TYPE_GDI?false:true;  
}





///-----------------------------

Layer*  WindowRender::CreateLayer(Object* pObj)
{
	Layer* pLayer = get_create_compositor()->CreateLayer();

    // ��ȡ��layer
    Layer* pParentLayer = pObj->GetLayer();
    if (!pParentLayer)
    {
        m_pCompositor->SetRootLayer(pLayer);
        return pLayer;
    }
    
    // �������layerӦ�ò�����layer tree���ĸ�λ�ã���Ҫ��object tree��Ӧ
    UIASSERT (pObj->GetParentObject());
    Layer* pNextLayer = pObj->FindNextLayer(pParentLayer);
    
    pParentLayer->AddSubLayer(pLayer, pNextLayer);

    // �����Ӷ����layer
    Layer* p = pParentLayer->GetFirstChild();
    while (p)
    {
        ILayerContent* pLayerContent = p->GetContent();
        
        // �����Լ����Ӷ����滻������������Ϊ�Լ�����layer
        if (pLayerContent && pLayerContent->IsChildOf(pObj))
        {
            p->MoveLayer2NewParentEnd(pParentLayer, pLayer);
        }
        p = p->GetNext();
    }

	return pLayer;
}

Compositor* UI::WindowRender::get_create_compositor()
{
    if (!m_pCompositor)
    {
//         if (m_pWindow->IsGpuComposite())
//         {
//             m_pCompositor = new ::HardwareCompositor();
//         }
//         else
        {
            m_pCompositor = new SoftwareCompositor;
        }
        m_pCompositor->SetUIApplication(m_pWindow->GetUIApplication());
        m_pCompositor->SetWindowRender(this);
        m_pCompositor->BindHWND(m_pWindow->GetHWND());
    }

    return m_pCompositor;
}

void  WindowRender::OnInvalidate()
{
    if (m_pCompositor)
        m_pCompositor->DoInvalidate();
}

void  WindowRender::OnPaint(HDC hDC, RECT* prcInvalid)
{
    if (m_pCompositor)
    {
        RectArray arr;
        arr.AddRect(prcInvalid);
        m_pCompositor->Commit(arr);
    }
}


void WindowRender::BindHWND(HWND hWnd)
{
    if (m_pCompositor)
        m_pCompositor->BindHWND(hWnd);
}