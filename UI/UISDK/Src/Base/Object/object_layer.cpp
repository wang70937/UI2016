#include "stdafx.h"
#include "object_layer.h"
#include "object.h"
#include "Src/Layer/windowrender.h"
#include "Src/UIObject/Window/windowbase.h"

using namespace UI;

ObjectLayer::ObjectLayer(Object& o) : m_obj(o)
{
	WindowRender* pWndRender = NULL;

	WindowBase* pWindow = o.GetWindowObject();
	if (pWindow)
		pWndRender = pWindow->GetWindowRender();

    if (pWndRender)
    {
        m_pLayer = pWndRender->CreateLayer(&m_obj);
        m_pLayer->SetContent(this);
    }
    else
    {
        // ��resize��ʱ�򴴽�
    }
}

ObjectLayer::~ObjectLayer()
{
    SAFE_DELETE(m_pLayer);
}

void  ObjectLayer::Draw(UI::IRenderTarget* pRenderTarget) 
{
    m_obj.DrawToLayer__(pRenderTarget);
}

void  ObjectLayer::GetWindowRect(RECT* prcOut) 
{
    m_obj.GetWindowRect(prcOut);
}

void  ObjectLayer::GetParentWindowRect(RECT* prcOut)
{
    if (m_obj.GetParentObject())
        m_obj.GetParentObject()->GetWindowRect(prcOut);  // TODO: -->> visible part only
}

void  ObjectLayer::OnSize(uint nWidth, uint nHeight)
{
    if (m_pLayer)
    {
        m_pLayer->OnSize(nWidth, nHeight);
    }
}

bool  ObjectLayer::IsChildOf(Object* pParent)
{
    if (!pParent)
        return false;

    if (pParent->IsMyChild(&m_obj, true))
        return true;

    return false;
}

bool  ObjectLayer::IsSelfVisible()
{
    return m_obj.IsSelfVisible();
}

// object�ڶ������е�λ�øı��ˣ�ͬ�����ֲ�������
void  ObjectLayer::OnObjPosInTreeChanged()
{
    UIASSERT (m_pLayer);

    Layer* pParentLayer = m_pLayer->GetParent();
	UIASSERT(pParentLayer);

    m_pLayer->RemoveMeInTheTree();
    pParentLayer->AddSubLayer(m_pLayer, m_obj.FindNextLayer(pParentLayer));
}