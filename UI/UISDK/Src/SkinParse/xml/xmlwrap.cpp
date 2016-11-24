#include "stdafx.h"
#include "xmlwrap.h"

UIElementProxy::UIElementProxy()
{
	m_ptr = NULL;
}
UIElementProxy::UIElementProxy(UIElement* pNew) 
{
	if (pNew)
	{
		m_ptr = pNew;
		m_ptr->AddRef();
	}
	else
	{
		m_ptr = NULL;
	}
}
UIElementProxy::~UIElementProxy()
{
	release();
}

void  UIElementProxy::release()
{
	if (m_ptr)
	{
        m_ptr->Release();
	}
}

UIElementProxy::UIElementProxy(const UIElementProxy& o)
{
	m_ptr = o.m_ptr;
	if (m_ptr)
		m_ptr->AddRef();
}
UIElementProxy& UIElementProxy::operator=(const UIElementProxy& o)
{
	release();

	m_ptr = o.m_ptr;
	if (m_ptr)
		m_ptr->AddRef();

	return *this;
}

UIElement* UIElementProxy::operator->()
{
	return m_ptr;
}

UIElementProxy::operator bool() const
{
	if (!m_ptr)
		return false;

	return true;
}

UIElement*  UIElementProxy::get()
{
	return m_ptr;
}

UIElement* UIElementProxy::detach()
{
    UIElement* p = m_ptr;
    m_ptr = NULL;
    return p;
}
