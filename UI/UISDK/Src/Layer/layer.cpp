#include "stdafx.h"
#include "layer.h"
#include "compositor.h"
#include "Src\Base\Application\uiapplication.h"
#include "..\Base\Object\object_layer.h"
#include "..\Base\Object\object.h"

using namespace UI;

Layer::Layer(): m_iLayer(this)
{
    m_pCompositor = NULL;

	m_pParent = NULL;
	m_pFirstChild = NULL;
	m_pNext = NULL;
	m_pPrev = NULL;

    m_size.cx = m_size.cy = 0;
    m_pLayerContent = NULL;
    m_bClipLayerInParentObj = true;
 	m_bAutoAnimate = 0;
//     m_pAnimateFinishCallback = NULL;
//     m_pAnimateFinishCallbackUserData = NULL;

	m_nOpacity = m_nOpacity_Render = 255;
	m_fyRotate = 0;

    m_xTranslate = 0;
    m_yTranslate = 0;
    m_zTranslate = 0;

// 	m_transfrom3d.perspective(1000);
// 	m_transfrom3d.set_transform_rotate_origin(
// 		TRANSFORM_ROTATE_ORIGIN_LEFT, 0,
// 		TRANSFORM_ROTATE_ORIGIN_CENTER, 0, 0);
}

Layer::~Layer()
{
// 	UIA::IAnimateManager* pAni = m_pCompositor->
// 		GetUIApplication()->GetAnimateMgr();
// 	if (pAni)
// 	{
// 		pAni->ClearStoryboardByNotify(
// 			static_cast<UIA::IAnimateEventCallback*>(this));
// 	}

	if (m_pParent && m_pParent->m_pFirstChild == this)
	{
		m_pParent->m_pFirstChild = m_pNext;
	}

    // ��ÿ�� object �Լ�ȥ�����Լ���layer
//     while (m_pFirstChild)
//     {
//         Layer* p = m_pFirstChild;
//         m_pFirstChild = m_pFirstChild->m_pNext;
//         delete p;
//     }

	if (m_pCompositor)
	{
		if (m_pCompositor->GetRootLayer() == this)
		{
			m_pCompositor->SetRootLayer(0);
		}
	}
}

ILayer*  Layer::GetILayer()
{
    return &m_iLayer;
}

void  Layer::SetCompositorPtr(Compositor* p)
{
    m_pCompositor = p;
}

void  Layer::Invalidate(LPCRECT prcDirtyInLayer, uint nCount)
{
	for (uint i = 0; i < nCount; i++)
		Invalidate(prcDirtyInLayer+i);
}

void  Layer::Invalidate(LPCRECT prcDirtyInLayer)
{
    RECT rcDirty = {0};

    if (!prcDirtyInLayer)
    {
        m_dirtyRectangles.Destroy();
        
        SetRect(&rcDirty, 0, 0, m_size.cx, m_size.cy);
        m_dirtyRectangles.AddRect(&rcDirty);
    }
    else
    {
	    if (IsRectEmpty(prcDirtyInLayer))
		    return;

        CopyRect(&rcDirty, prcDirtyInLayer);
	    m_dirtyRectangles.UnionDirtyRect(prcDirtyInLayer);
    }

    // ����������Ⱦ������ð��
    if (m_pParent && GetType() == Layer_Software)
    {
        // rcDirty ת���� ��layerλ�ã����ɸ�layerȥ����ϳ�
        RECT rcParent = {0};
        m_pParent->m_pLayerContent->GetParentWindowRect(&rcParent);

        RECT rcSelf = {0};
        m_pLayerContent->GetWindowRect(&rcSelf);

        OffsetRect(&rcDirty, 
            rcSelf.left-rcParent.left, 
            rcSelf.top-rcParent.top);

        m_pParent->Invalidate(&rcDirty);
    }
    else
    {
    	if (m_pCompositor)
	    	m_pCompositor->RequestInvalidate();
    }
}

bool Layer::AddSubLayer(Layer* pLayer, Layer* pInsertBeforeChild)
{
	UIASSERT(pLayer);
    UIASSERT (!pLayer->m_pParent);

    if (pLayer->m_pParent)
        return false;

    if (!m_pFirstChild)
    {
        m_pFirstChild = pLayer;
    }
    else
    {
        // ���������
        if (!pInsertBeforeChild)
        {
            Layer* p = m_pFirstChild;
            while (p && p->m_pNext)
            {
                p = p->m_pNext;
            }


            p->m_pNext = pLayer;
            pLayer->m_pPrev = p;
        }
        else
        {
            if (pInsertBeforeChild->m_pPrev)
                pInsertBeforeChild->m_pPrev->m_pNext = pLayer;
            pLayer->m_pPrev = pInsertBeforeChild->m_pPrev;

            pInsertBeforeChild->m_pPrev = pLayer;
            pLayer->m_pNext = pInsertBeforeChild;
        }
    }

    pLayer->m_pParent = this;

    on_layer_tree_changed();
    return true;
}

void  Layer::RemoveMeInTheTree()
{
    if (m_pPrev)
    {
        m_pPrev->m_pNext = m_pNext;
        if (m_pNext)
            m_pNext->m_pPrev = m_pPrev;
    }
    else
    {
        if (m_pParent)
            m_pParent->m_pFirstChild = m_pNext;
    }

    m_pParent = NULL;
    m_pPrev = NULL;
    m_pNext = NULL;
}

void  Layer::MoveLayer2NewParentEnd(Layer* pOldParent, Layer* pNewParent)
{
	UIASSERT(pOldParent && pNewParent);

    RemoveMeInTheTree();
    pNewParent->AddSubLayer(this, NULL);
}

void  Layer::SetContent(ILayerContent* p)
{
    m_pLayerContent = p;
}
ILayerContent*  Layer::GetContent()
{
    return m_pLayerContent;
}

void  Layer::on_layer_tree_changed()
{
	UIASSERT(m_pCompositor);

    Layer* pRootLayer = this;

    while (pRootLayer && pRootLayer->m_pParent)
    {
        pRootLayer = pRootLayer->m_pParent;
    }
    while (pRootLayer && pRootLayer->m_pPrev)
    {
        pRootLayer = pRootLayer->m_pPrev;
    }
    m_pCompositor->SetRootLayer(pRootLayer);
}

Layer* Layer::GetNext()
{
    return m_pNext;
}

Layer*  Layer::GetFirstChild()
{
    return m_pFirstChild;
}

void  Layer::OnSize( uint nWidth, uint nHeight )
{
    m_size.cx = nWidth;
    m_size.cy = nHeight;
    Invalidate(NULL);

    if (!m_pRenderTarget)
    {
        GetRenderTarget();
    }
    m_pRenderTarget->ResizeRenderBuffer(nWidth, nHeight);

    virtualOnSize(nWidth, nHeight);
}

void  Layer::PostCompositorRequest()
{
   m_pCompositor->RequestInvalidate();
} 


void Layer::SetOpacity(byte b, LayerAnimateParam* pParam)
{
	if (m_nOpacity == b)
		return;

	m_nOpacity = b;

	// ������ʽ����
	if (pParam)
	{
		UIA::IAnimateManager* pAni = m_pCompositor->
			GetUIApplication()->GetAnimateMgr();

		pAni->RemoveStoryboardByNotityAndId(
			static_cast<UIA::IAnimateEventCallback*>(this), 
			STORYBOARD_ID_OPACITY);

		UIA::IStoryboard* pStoryboard = pAni->CreateStoryboard(
			static_cast<UIA::IAnimateEventCallback*>(this), 
			STORYBOARD_ID_OPACITY);

		pStoryboard->CreateTimeline(0)->SetParam(
			m_nOpacity_Render, 
			m_nOpacity, 
			ANIMATE_DURATION);

		pStoryboard->SetWParam((WPARAM)pParam->pCallback);
		pStoryboard->SetLParam((LPARAM)pParam->pUserData);

		if (pParam->bBlock)
		{
			pStoryboard->BeginBlock();
		}
		else
		{
			pStoryboard->Begin();
		}
	}
	else
	{
		m_nOpacity_Render = m_nOpacity;
		m_pCompositor->RequestInvalidate();
	}
}

byte Layer::GetOpacity()
{
	return m_nOpacity_Render;
}
void  Layer::SetYRotate(float f)
{
	if (m_fyRotate == f)
		return;

	m_fyRotate = f;

	// ������ʽ����
// 	if (IsAutoAnimate())
// 	{
// 		UIA::IAnimateManager* pAni = m_pCompositor->
// 			GetUIApplication()->GetAnimateMgr();
// 
// 		pAni->RemoveStoryboardByNotityAndId(
// 			static_cast<UIA::IAnimateEventCallback*>(this), 
// 			STORYBOARD_ID_YROTATE);
// 
// 		UIA::IStoryboard* pStoryboard = pAni->CreateStoryboard(
// 			static_cast<UIA::IAnimateEventCallback*>(this), 
// 			STORYBOARD_ID_YROTATE);
// 
// 		pStoryboard->CreateTimeline(0)->SetParam(
// 			m_transfrom3d.get_rotateY(), 
// 			m_fyRotate, 
// 			ANIMATE_DURATION);
// 
// 		pStoryboard->Begin();
// 	}
// 	else
// 	{
// 		m_transfrom3d.rotateY(f);
// 	}
}
float  Layer::GetYRotate() 
{
	return m_fyRotate;
}

void  Layer::SetTranslate(float x, float y, float z)
{
    if (m_xTranslate == x && 
        m_yTranslate == y &&
        m_zTranslate == z)
        return;

    m_xTranslate = x;
    m_yTranslate = y;
    m_zTranslate = z;
        
    // ������ʽ����
//     if (IsAutoAnimate())
//     {
//         UIA::IAnimateManager* pAni = m_pCompositor->
//             GetUIApplication()->GetAnimateMgr();
// 
//         pAni->RemoveStoryboardByNotityAndId(
//             static_cast<UIA::IAnimateEventCallback*>(this),
//             STORYBOARD_ID_TRANSLATE);
// 
//         UIA::IStoryboard* pStoryboard = pAni->CreateStoryboard(
//             static_cast<UIA::IAnimateEventCallback*>(this),
//             STORYBOARD_ID_TRANSLATE);
// 
//         pStoryboard->CreateTimeline(0)->SetParam(
//             m_transfrom3d.get_translateX(),
//             m_xTranslate,
//             ANIMATE_DURATION);
// 
//         pStoryboard->CreateTimeline(1)->SetParam(
//             m_transfrom3d.get_translateY(),
//             m_yTranslate,
//             ANIMATE_DURATION);
// 
//         pStoryboard->CreateTimeline(2)->SetParam(
//             m_transfrom3d.get_translateZ(),
//             m_zTranslate,
//             ANIMATE_DURATION);
// 
//         pStoryboard->Begin();
//     }
//     else
//     {
//         m_transfrom3d.translate3d(x, y, z);
//     }
}

float  Layer::GetXTranslate()
{
    return m_xTranslate;
}
float  Layer::GetYTranslate()
{
    return m_yTranslate;
}
float  Layer::GetZTranslate()
{
    return m_zTranslate;
}

UIA::E_ANIMATE_TICK_RESULT Layer::OnAnimateTick(UIA::IStoryboard* pStoryboard)
{
	switch (pStoryboard->GetId())
	{
	case STORYBOARD_ID_OPACITY:
		{
			m_nOpacity_Render = (byte)pStoryboard->
				GetTimeline(0)->GetCurrentIntValue();

            static_cast<ObjectLayer*>(m_pLayerContent)->GetObjet().Invalidate();
			// �п�����block��������Ҫ����ˢ��
			m_pCompositor->DoInvalidate();
		}
		break;

// 	case STORYBOARD_ID_YROTATE:
// 		{
// 			m_transfrom3d.rotateY(pStoryboard->
// 				GetTimeline(0)->GetCurrentValue());
// 
// 			m_pCompositor->RequestCompositor();
// 		}
// 		break;
//
//     case STORYBOARD_ID_TRANSLATE:
//         {
//             m_transfrom3d.translate3d(
//                 pStoryboard->GetTimeline(0)->GetCurrentValue(),
//                 pStoryboard->GetTimeline(1)->GetCurrentValue(),
//                 pStoryboard->GetTimeline(2)->GetCurrentValue());
// 
//             m_pCompositor->RequestCompositor();
//         }
//         break;
	}

	return UIA::ANIMATE_TICK_RESULT_CONTINUE;
}

void  Layer::OnAnimateEnd(UIA::IStoryboard* pStoryboard, UIA::E_ANIMATE_END_REASON e)
{
	pfnLayerAnimateFinish pCallback = (pfnLayerAnimateFinish)pStoryboard->GetWParam();
	long* pUserData = (long*)pStoryboard->GetLParam();

	if (!pCallback)
		return;

    LayerAnimateFinishInfo info = { 0 };
    info.pLayer = &m_iLayer;
    info.eAnimateType = (UI::LAYER_ANIMATE_TYPE)pStoryboard->GetId();
    info.pUserData = pUserData;
	pCallback(&info);
}

void  Layer::CopyDirtyRect(RectArray& arr)
{   
    arr = m_dirtyRectangles;
}

bool  Layer::IsAutoAnimate()
{
    return m_bAutoAnimate == 0 ? true : false;
}
void  Layer::EnableAutoAnimate(bool b)
{
    if (b)
        ++m_bAutoAnimate;
    else
        --m_bAutoAnimate;
}

// void  Layer::SetAnimateFinishCallback(pfnLayerAnimateFinish pfn, long* pUserData)
// {
//     m_pAnimateFinishCallback = pfn;
//     m_pAnimateFinishCallbackUserData = pUserData;
// }


IRenderTarget*  Layer::GetRenderTarget()
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