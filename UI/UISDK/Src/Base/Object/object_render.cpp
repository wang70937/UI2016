#include "stdafx.h"
#include "object.h"
#include "Src\UIObject\Window\windowbase.h"
#include "Src\Atl\image.h"
#include "object_layer.h"
#include "Inc\Interface\irenderbase.h"
#include "Inc\Interface\renderlibrary.h"
#include "Src\Layer\software_layer.h"

// 2016.6.1 ��Ⱦ�߼�����
//
// 1. ����UpdateObject������ͳһ�ĳ�Invalidate����
// 2. ����ListCtrl�е�LF_REDRAW��ʶλ���ⲿ������Ҫ����ˢ��

using namespace UI;

#if 0
HBITMAP  Object::TakeSnapshot()
{
    Image image;
    image.Create(GetWidth(), GetHeight(), 32, Image::createAlphaChannel);
    HDC hDC = image.GetDC();
    SetBkMode(hDC, TRANSPARENT);
    SetStretchBltMode(hDC, HALFTONE);
    
    IRenderTarget*  pRenderTarget = NULL;
    GetWindowRender()->CreateRenderTarget(&pRenderTarget);
    pRenderTarget->BindHDC(hDC);
    pRenderTarget->BeginDraw();
    {
        CRect rcRenderRegion(0, 0, GetWidth(), GetHeight());
        RenderContext  context(&rcRenderRegion, &rcRenderRegion, true);
        this->RealDrawObject(pRenderTarget, context);
    }
    pRenderTarget->EndDraw();

    pRenderTarget->Release();
    image.ReleaseDC();
#ifdef _DEBUGx
    image.Save(L"C:\\aaa.png", Gdiplus::ImageFormatPNG);
#endif
    return image.Detach();
}


// ��֧��zorder�ص���ֱ��ȡ���ȵı���
HBITMAP  Object::TakeBkgndSnapshot()
{
    if (!m_pParent)
        return NULL;

    Image image;
    image.Create(m_rcParent.Width(), m_rcParent.Height(), 32, Image::createAlphaChannel);
    HDC hDC = image.GetDC();
    SetBkMode(hDC, TRANSPARENT);
    SetStretchBltMode(hDC, HALFTONE);

	IRenderTarget*  pRenderTarget = NULL;
    GetWindowRender()->CreateRenderTarget(&pRenderTarget);

    pRenderTarget->BindHDC(hDC);
    pRenderTarget->BeginDraw();
    {
        // ����游��������û�в�͸���ģ��Ӹø�������
        Object*  pObjAncestorFirst2DrawBkgnd = NULL;
        Object*  pChild = this;
        CRect    rcInAncestor(0, 0, GetWidth(), GetHeight());
        while (pObjAncestorFirst2DrawBkgnd = this->EnumParentObject(pObjAncestorFirst2DrawBkgnd))
        {
            ChildRect2ParentRect(pChild, &rcInAncestor, &rcInAncestor);

            // �ҵ���͸��������߲����
            if (!pObjAncestorFirst2DrawBkgnd->IsTransparent())
                break;

            pChild = pObjAncestorFirst2DrawBkgnd;
        }
        if (NULL == pObjAncestorFirst2DrawBkgnd)
            pObjAncestorFirst2DrawBkgnd = GetWindowObject();

        CRect  rcClip(0, 0, GetWidth(), GetHeight());
        CRect  rcCurrentClip(&rcInAncestor);
        rcCurrentClip.OffsetRect(-rcInAncestor.left*2, -rcInAncestor.top*2);
        RenderContext  context(&rcClip, &rcCurrentClip, false);
        context.m_ptOffset.x = rcCurrentClip.left;
        context.m_ptOffset.y = rcCurrentClip.top;

        Object*  pParent = pObjAncestorFirst2DrawBkgnd;
        context.Update(pRenderTarget);
        ::UISendMessage(pParent, WM_ERASEBKGND, (WPARAM)pRenderTarget, (LPARAM)&context);
        while (pChild = REnumParentObject(pParent))
        {
            if (!pChild->IsNcObject())
                context.DrawClient(pParent->m_pIObject);
            context.DrawChild(pChild->m_pIObject);
            context.Update(pRenderTarget);

            ::UISendMessage(pParent, WM_ERASEBKGND, (WPARAM)pRenderTarget, (LPARAM)&context);
            pParent = pChild;
        }
    }
    pRenderTarget->EndDraw();
    pRenderTarget->Release();
    image.ReleaseDC();

#ifdef _DEBUGx
    image.Save(L"C:\\aaa.png", Gdiplus::ImageFormatPNG);
#endif
    return image.Detach();
}


void Object::DoPostPaint(IRenderTarget* pRenderTarget, RenderContext context)
{
	if (m_objStyle.post_paint)
	{
		// �ӿؼ����������˼�������������Ȼָ��ٻ�postpaint
		context.m_bUpdateClip = true;
		context.Update(pRenderTarget);
		UI::UISendMessage(m_pIObject,
			UI_WM_POSTPAINT, 
			(WPARAM)pRenderTarget, 
			(LPARAM)&context);
	}
}

#endif

void Object::OnEraseBkgnd(IRenderTarget* pRenderTarget)
{
    if (m_pBkgndRender)
    {
        CRect rc(0,0, this->GetWidth(), this->GetHeight());
        m_pBkgndRender->DrawState(pRenderTarget, &rc, 0);
    }
}

// ���ϵ���ˢ�·�ʽ(����),���Էֲ㴰����Ч��
// �ֲ㴰��BeginPaint֮���õ���ps.rcPaintΪ��
//
// InvalidateObject���Զ�ε��ã���ֻ��/*�������ʱ*/ ��һ����Ϣѭ��ʱ ������ˢ�¡�
//
// 1. ���ڽ��ʵ�����������У�����UpdateObject������
// 2. ����ʵ�ֶ������Ա仯ʱ��ʱˢ�£����Ч�ʡ�
// 3. ���ڽ��ʵ�����̵��У�������ø���ʱ����֪����ǰ�Ƿ���Ҫ����ˢ�º�����
//    �����˿��ܵ���Ч�ʵ��£������˽���û��ˢ��
// 4. ʵ�������У��ܶ࿪����Ա��ˢ�²��˽⣬��֪��ʲôʱ����Ҫ��ˢ��
//
void  Object::Invalidate()
{
	CRect rc(0,0, m_rcParent.Width(), m_rcParent.Height());
	Invalidate(&rc);
}

void  Object::Invalidate(RECT* prcObj)
{
	if (!prcObj)
		return;

	if (!IsVisible() || !CanRedraw())
		return;

	ObjectLayer* pLayer = GetLayerEx();
	if (!pLayer)  // �մ�������δ��ʼ��layer�׶�
		return;

	Object& layerObj = pLayer->GetObjet();
	
	RECT rc = {0};
	if (!CalcRectInAncestor(&layerObj, prcObj, true, &rc))
		return;

	pLayer->GetLayer()->Invalidate(&rc);
}

void  Object::Invalidate(RECT* prcObjArray, int nCount)
{
	for (int i = 0; i < nCount; i++)
	{
		this->Invalidate(prcObjArray+i);
	}
}


//////////////////////////////////////////////////////////////////////////

void  Object::DrawToLayer__(IRenderTarget* pRenderTarget)
{
    // 1. �ǿͻ���������padding scrollӰ��
    ::UISendMessage(this, WM_ERASEBKGND, (WPARAM)pRenderTarget); 
    this->virtualOnPostDrawObjectErasebkgnd();

    // 2. �ͻ��������²ü���ƫ��
    CRect rcObj(0, 0, m_rcParent.Width(), m_rcParent.Height());
    CRect rcClient;
    this->GetClientRectInObject(&rcClient);

    int xOffset = 0, yOffset = 0;
    GetScrollOffset(&xOffset, &yOffset);
    xOffset = -xOffset + rcClient.left;
    yOffset = -yOffset + rcClient.top;

    bool bNeedClip = true;
    if (rcClient == rcObj)
        bNeedClip = false;

    if (bNeedClip)
        pRenderTarget->PushRelativeClipRect(&rcClient);
    {
        pRenderTarget->OffsetOrigin(xOffset, yOffset);
        {
            ::UISendMessage(this, WM_PAINT, (WPARAM)pRenderTarget);    

            if (m_pChild)
            {
                this->DrawChildObject__(pRenderTarget, m_pChild);
            }

        }
        pRenderTarget->OffsetOrigin(-xOffset, -yOffset);
    }
    if (bNeedClip)
        pRenderTarget->PopRelativeClipRect();
    
    // 3. �ǿͻ����Ӷ���
    if (m_pNcChild)
    {
        this->DrawChildObject__(pRenderTarget, m_pNcChild);
    }

    if (m_objStyle.post_paint)
    {
            UI::UISendMessage(m_pIObject,
                UI_MSG_POSTPAINT,
                (WPARAM)pRenderTarget);
    }

#ifdef _DEBUG
    static bool bDebug = false;
    if (bDebug)
        pRenderTarget->Save(0);
#endif 
}

// ʹ���������첽ˢ�·�������Ȼ��֧��z����ƣ�ֻҪ���ص�����ͻᱻ����
void  Object::DrawChildObject__(IRenderTarget* pRenderTarget, Object* pChildStart)
{
    Object*  pChild = pChildStart;
    while (pChild)
    {
        if (!pChild->IsSelfVisible())
        {
            pChild = pChild->m_pNext;
            continue;
        }

        if (pChild->m_pLayer)
        {
            Layer* pChildLayer = pChild->m_pLayer->GetLayer();
            // Ӳ���ϳɣ�ÿ���㵥�����ơ�
            if (pChildLayer->GetType() == Layer_Hardware)
            {
                pChild = pChild->m_pNext;
                continue;
            }
        }

        if (!pRenderTarget->IsRelativeRectInClip(pChild->m_rcParent))
        {
            pChild = pChild->m_pNext;
            continue;
        }

        // �����������Ҫ���ã���ô�Լ�Ҳ������
        bool bChildNeedClip = pChild->NeedClip();

        if (bChildNeedClip)
            pRenderTarget->PushRelativeClipRect(&pChild->m_rcParent);

        pRenderTarget->OffsetOrigin(
            pChild->m_rcParent.left, 
            pChild->m_rcParent.top);

        if (pChild->m_pLayer)
        {
            // �����Ⱦģʽ���棬�Ӳ�Ҫ���ڸ�������
            Layer* pChildLayer = pChild->m_pLayer->GetLayer();
            if (pChildLayer/* && pChildLayer->GetType() == Layer_Software*/)
            {
                pChildLayer->UpdateDirty();

                Render2TargetParam param = {0};
                param.xSrc = param.xDst = 0;
                param.ySrc = param.yDst = 0;
                param.wSrc = param.wDst = pChild->GetWidth();
                param.hSrc = param.hDst = pChild->GetHeight();
                param.bAlphaBlend = true;
                param.opacity = pChildLayer->GetOpacity();
                pChildLayer->GetRenderTarget()->Render2Target(pRenderTarget, &param);
            }
        }
        else
        {
            pChild->DrawToLayer__(pRenderTarget);
        }

        pRenderTarget->OffsetOrigin(
            -pChild->m_rcParent.left, 
            -pChild->m_rcParent.top);

       if (bChildNeedClip)
           pRenderTarget->PopRelativeClipRect();

       pChild = pChild->m_pNext;
    }
}