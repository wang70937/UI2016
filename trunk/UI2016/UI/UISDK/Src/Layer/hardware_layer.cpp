#include "stdafx.h"
#if 0
#include "hardware_layer.h"
#include "hardware_compositor.h"
using namespace UI;

HardwareLayer::HardwareLayer()
{
    m_pRenderTarget = NULL;
    m_pGpuTexture = NULL;
}

HardwareLayer::~HardwareLayer()
{
    SAFE_RELEASE(m_pRenderTarget);
    SAFE_RELEASE(m_pGpuTexture);
}


// void  HardwareLayer::DrawFull()
// {
//     if (!m_pLayerContent)
//         return;
// 
//     IRenderTarget* pRenderTarget = GetRenderTarget();
//     pRenderTarget->Clear(NULL);
// 
//     pRenderTarget->BeginDraw();
//     m_pLayerContent->Draw(pRenderTarget);
//     pRenderTarget->EndDraw();
// 
// 	m_dirtyRectangles.Destroy();
//     upload_2_gpu();
// }

void  HardwareLayer::UpdateDirty()
{
    if (!m_pLayerContent)
        return;

	if (!m_dirtyRectangles.GetCount())
		return;

	IRenderTarget* pRenderTarget = GetRenderTarget();
    uint nCount = m_dirtyRectangles.GetCount();
    for (uint i = 0; i < nCount; i++)
        pRenderTarget->Clear(m_dirtyRectangles.GetRectPtrAt(i));

	pRenderTarget->BeginDraw();

    pRenderTarget->SetMetaClipRegion(
        m_dirtyRectangles.GetArrayPtr(), 
        m_dirtyRectangles.GetCount());

	m_pLayerContent->Draw(pRenderTarget);
	pRenderTarget->EndDraw();

	m_dirtyRectangles.Destroy();
    upload_2_gpu();
}

// Ӳ���ϳ�ʱ��ÿһ��������Ƕ���ġ�
// void HardwareLayer::Draw()
// {
// 	this->draw_layer();
// 
// 	Layer* pChild = m_pFirstChild;
// 	while (pChild)
// 	{
// 		HardwareLayer* p = static_cast<HardwareLayer*>(pChild);
// 
// 		p->draw_layer();
// 		pChild = p->m_pNext;
// 	}
// }
// 
// 
// void  HardwareLayer::draw_layer()
// {
// 	if (m_dirtyRectangles.GetCount() == 0)
// 		return;
// 		
// 	// ��������layer����
// 
// 	// �ύlayer
// 	upload_2_gpu();
// 
// 	// ���������
// 	m_dirtyRectangles.Destroy();
// }

void HardwareLayer::upload_2_gpu()
{
	if (!m_pRenderTarget)
        return;

    if (!m_pGpuTexture)
    {
        m_pGpuTexture = static_cast<UI::HardwareCompositor*>(m_pCompositor)
            ->CreateGpuLayerTexture();
        if (m_pGpuTexture)
        {
        	m_pGpuTexture->Resize(m_size.cx, m_size.cy);
        }
    }

    CRect rc (0, 0, m_size.cx, m_size.cy);
    m_pRenderTarget->Upload2Gpu(m_pGpuTexture, &rc, 1);
}


IRenderTarget*  HardwareLayer::GetRenderTarget()
{
    if (!m_pRenderTarget)        
    {
        if (!m_pCompositor)
            return NULL;

        m_pCompositor->CreateRenderTarget(&m_pRenderTarget);
        if (!m_pRenderTarget)
            return NULL;

        m_pRenderTarget->CreateRenderBuffer(NULL);
    }

    return m_pRenderTarget;
}

void  HardwareLayer::virtualOnSize(uint nWidth, uint nHeight)
{
    if (!m_pRenderTarget)
    {
        GetRenderTarget();
    }
    m_pRenderTarget->ResizeRenderBuffer(nWidth, nHeight);
   
    if (m_pGpuTexture)
    {
        m_pGpuTexture->Resize(nWidth, nHeight);
    }
    m_transfrom3d.set_size(nWidth, nHeight);
}

void  HardwareLayer::Commit(GpuLayerCommitContext* pContext)
{
    if (!m_pGpuTexture)
        return;
    if (!m_pLayerContent)
        return;

    RECT rcWnd;
    m_pLayerContent->GetWindowRect(&rcWnd);
    pContext->SetOffset(rcWnd.left, rcWnd.top);

    RECT rcParentWnd = {0}; 
	if (m_bClipLayerInParentObj && m_pCompositor->GetRootLayer() != this)
    {
		m_pLayerContent->GetParentWindowRect(&rcParentWnd);
    }
    else
    {
        ::GetClientRect(m_pCompositor->GetHWND(), &rcParentWnd);
    }
	pContext->SetClipRect(&rcParentWnd);

    if (m_nOpacity_Render != 255)
	{
        pContext->MultiAlpha(m_nOpacity_Render);
	}

	// ������������תʱ����Ҫ֪�������������Ļ�е�λ�ã�Ȼ����ܼ������������ת����
	// ���ÿ��ʹ��ǰ����һ�Ρ�
	m_transfrom3d.set_pos(rcWnd.left, rcWnd.top);
	if (!m_transfrom3d.is_identity())
	{
		MATRIX44 mat;
		m_transfrom3d.get_matrix(&mat);
		m_pGpuTexture->Compositor(pContext, (float*)&mat);
	}
	else
	{
		m_pGpuTexture->Compositor(pContext, NULL);
	}
}

void  HardwareLayer::MapView2Layer(POINT* pPoint)
{
	if (!m_transfrom3d.is_identity())
	{
		m_transfrom3d.mappoint_view_2_layer(pPoint);
	}
}
#endif