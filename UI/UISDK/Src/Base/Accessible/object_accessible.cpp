#include "stdafx.h"
#include "object_accessible.h"
#include "Src\Base\Object\object.h"
#include "Src\UIObject\Window\windowbase.h"

using namespace UI;


ObjectAccessible::ObjectAccessible(Object& o) : m_refObject(o)
{
	
}

ObjectAccessible::~ObjectAccessible()
{

}

HWND  ObjectAccessible::GetHWND()
{
	return m_refObject.GetHWND();
}

HRESULT  ObjectAccessible::get_accParent( 
		IDispatch **ppdispParent, BOOL& bHandled)
{
	bHandled = TRUE;

	if (!ppdispParent)
		return E_INVALIDARG;
	*ppdispParent = NULL;

	IAccessible* pAccessible = NULL;

	if (!m_refObject.GetParentObject())
	{
// 		if (m_refObject.GetObjectType() == OBJ_WINDOW)
// 		{
// 			HWND hWnd = GetHWND();
// 			HWND hwndParent = ::GetParent(hWnd);
// 			if (!hwndParent)
// 			{
// 				hwndParent = ::GetWindow(hWnd, GW_OWNER);
// 				if (!hwndParent)
// 				{
// 					hwndParent = GetDesktopWindow();
// 					if (!hwndParent)
// 						return S_FALSE;
// 				}
// 			}
// 
// 			return AccessibleObjectFromWindow(hwndParent, 
// 					(DWORD)CHILDID_SELF, IID_IDispatch,
// 					(void **)ppdispParent);
// 		}
		return S_OK;
	}

	m_refObject.GetParentObject()->CreateAccesible(&pAccessible);
	if (!pAccessible)
		return S_FALSE;

	*ppdispParent = static_cast<IDispatch*>(pAccessible);
	return S_OK;
}

HRESULT  ObjectAccessible::get_accChildCount( 
		long *pcountChildren, BOOL& bHandled)
{
	bHandled = TRUE;

	if (pcountChildren)
		*pcountChildren = m_refObject.GetChildCount();
	return S_OK;
}

HRESULT  ObjectAccessible::get_accChild( 
		VARIANT varChild,
		IDispatch **ppdispChild,
		BOOL& bHandled)
{
	bHandled = TRUE;

	if (!ppdispChild)
		return E_INVALIDARG;
	*ppdispChild = NULL;

	if (varChild.vt != VT_I4)
		return E_INVALIDARG;

	Object* pChild = m_refObject.GetChildObjectByIndex(varChild.lVal-1);
	if (!pChild)
		return E_FAIL;

	IAccessible* pAccessible = NULL;
	pChild->CreateAccesible(&pAccessible);
	if (!pAccessible)
		return S_FALSE;

	*ppdispChild = static_cast<IDispatch*>(pAccessible);
	return S_OK;
}

HRESULT  ObjectAccessible::get_accName(VARIANT varChild, 
		BSTR *pszName, BOOL& bHandled)
{
	bHandled = TRUE;

	if (!pszName)
		return E_INVALIDARG;
		 
	if (varChild.vt != VT_I4)
	{
		*pszName = NULL;
		return E_INVALIDARG;
	}

	if (varChild.lVal == CHILDID_SELF)
	{
		*pszName = SysAllocString(m_refObject.GetId());
	}
	else
	{
		Object* pChild = m_refObject.GetChildObjectByIndex(varChild.lVal-1);
		if (pChild)
		{
			*pszName = SysAllocString(pChild->GetId());
		}
	}
	return (*pszName != NULL) ? S_OK : S_FALSE;
}

HRESULT  ObjectAccessible::accNavigate( 
		long navDir,
		VARIANT varStart,
		VARIANT *pvarEndUpAt,
		BOOL& bHandled)
{
	bHandled = TRUE;
	// Default value.
	pvarEndUpAt->vt = VT_EMPTY;
	if (varStart.vt != VT_I4)
		return E_INVALIDARG;

	Object* pObj = &m_refObject;
	if (varStart.lVal != CHILDID_SELF)
		pObj = m_refObject.GetChildObjectByIndex(varStart.lVal-1);
	if (!pObj)
		return E_INVALIDARG;

	Object* pObjGet = NULL;

	switch (navDir)
	{
	case NAVDIR_NEXT:
	case NAVDIR_RIGHT:
		pObjGet = pObj->GetNextObject();
		break;

	case NAVDIR_PREVIOUS:
	case NAVDIR_LEFT:
		pObjGet = pObj->GetPrevObject();
		break;

	case NAVDIR_UP:
		pObjGet = pObj->GetParentObject();
		break;

	case NAVDIR_DOWN:
	case NAVDIR_FIRSTCHILD:
		pObjGet = pObj->GetChildObject();
		break;

	case NAVDIR_LASTCHILD:
		pObjGet = pObj->GetLastChildObject();
		break;
	}

	if (!pObjGet)
		return S_FALSE;

	IAccessible* p = NULL;
	pObjGet->CreateAccesible(&p);
	if (!p)
		return S_FALSE;

	pvarEndUpAt->vt = VT_DISPATCH;
	pvarEndUpAt->pdispVal = static_cast<IDispatch*>(p);
	return S_OK;
}

HRESULT  ObjectAccessible::accHitTest( 
		long xLeft, long yTop,
		VARIANT *pvarChild, BOOL& bHandled)
{
	bHandled = TRUE;

	if (!pvarChild)
		return E_INVALIDARG;
	pvarChild->vt = VT_EMPTY;

	POINT pt = {xLeft, yTop};
	HWND hWnd = GetHWND();
	::MapWindowPoints(NULL, hWnd, &pt, 1);

	// Not in our window.
// 	if (WindowFromPoint(pt) != hWnd)
// 	{
// 		pvarChild->vt = VT_EMPTY;
// 		return S_FALSE;
// 	}

	CRect rc;
	m_refObject.GetWindowRect(&rc);
	if (!PtInRect(&rc, pt))	
		return S_FALSE;

	pvarChild->vt = VT_I4;
	pvarChild->lVal = CHILDID_SELF;

	pt.x -= rc.left;
	pt.y -= rc.top;

	GetObjectByPosExData  data = {0};
	data.pObjParent = &m_refObject;
	data.ptParent = &pt;
	data.bTestDecendant = false;
	data.bTestRejectMouseMsg = false;
	data.bTestDisable = false;
	Object* pObjChildHit = WindowMouseMgr::GetObjectByPosEx(&data);

	if (!pObjChildHit)
		return S_OK;

#if 0
	unsigned long lIndex = m_refObject.GetChildObjectIndex(pObjChildHit);
	pvarChild->vt = VT_I4;
	pvarChild->lVal = lIndex+1;
#else
	IAccessible* pAccessible = NULL;
	pObjChildHit->CreateAccesible(&pAccessible);
	if (!pAccessible)
		return S_OK;

	pvarChild->vt = VT_DISPATCH;
	pvarChild->pdispVal = static_cast<IDispatch*>(pAccessible);
#endif
	return S_OK;
}

HRESULT  ObjectAccessible::accLocation( 
	long *pxLeft,
	long *pyTop,
	long *pcxWidth,
	long *pcyHeight,
	VARIANT varChild,
	BOOL& bHandled)
{
	bHandled = TRUE;
	if (varChild.vt != VT_I4)
		return E_INVALIDARG;

	CRect rc;
	rc.SetRectEmpty();

	Object* pObj = &m_refObject;
	if (varChild.lVal != CHILDID_SELF)
		pObj = m_refObject.GetChildObjectByIndex(varChild.lVal-1);

	if (pObj)
	{
		pObj->GetWindowRect(&rc);
		::MapWindowPoints(GetHWND(), NULL, (LPPOINT)&rc, 2);
	}
	
	if (pxLeft)
		*pxLeft = rc.left;
	if (pyTop)
		*pyTop = rc.top;
	if (pcxWidth)
		*pcxWidth = rc.Width();
	if (pcyHeight)
		*pcyHeight = rc.Height();

	return S_OK;
}

HRESULT  ObjectAccessible::get_accDescription(VARIANT varChild,
		BSTR *pszDescription, BOOL& bHandled)
{
	bHandled = TRUE;

	if (!pszDescription)
		return E_INVALIDARG;
	*pszDescription = NULL;

	if (varChild.vt != VT_I4)
	{
		*pszDescription = NULL;
		return E_INVALIDARG;
	}
	Object* pObj = &m_refObject;
	if (varChild.lVal == CHILDID_SELF)
	{
	}
	else
	{
		pObj = m_refObject.GetChildObjectByIndex(varChild.lVal-1);
	}

	if (!pObj)
		return S_FALSE;

	// TODO:
// 	LPCTSTR szToolTips = pObj->GetToolTipText();
// 	if (szToolTips && szToolTips[0])
// 	{
// 		*pszDescription = SysAllocString(szToolTips);
// 	}
	return S_OK;
}

long  MapControlType(CONTROL_TYPE eCtrlType)
{
	switch (eCtrlType)
	{
	case CONTROL_BUTTON:
		return ROLE_SYSTEM_PUSHBUTTON; 

	case CONTROL_CHECKBUTTON:
		return ROLE_SYSTEM_CHECKBUTTON;

	case CONTROL_RADIOBUTTON:
		return ROLE_SYSTEM_RADIOBUTTON;

	case CONTROL_LABEL:
		return ROLE_SYSTEM_STATICTEXT;

	case CONTROL_HYPERLINK:
		return ROLE_SYSTEM_LINK;

	case CONTROL_HSCROLLBAR:
	case CONTROL_VSCROLLBAR:
		return ROLE_SYSTEM_SCROLLBAR;

	case CONTROL_GROUPBOX:
		return ROLE_SYSTEM_GROUPING;

	case CONTROL_COMBOBOX:
		return ROLE_SYSTEM_COMBOBOX;
		//return ROLE_SYSTEM_DROPLIST;

	case CONTROL_PROGRESSBAR:
		return ROLE_SYSTEM_PROGRESSBAR;

	case CONTROL_SLIDERCTRL:
		return ROLE_SYSTEM_SLIDER;

	case CONTROL_LISTBOX:
	case CONTROL_LISTVIEWCTRL:
	case CONTROL_TREEVIEWCTRL:
	case CONTROL_LISTTREEVIEWCTRL:
		return ROLE_SYSTEM_LIST;

	case CONTROL_EDIT:
	case CONTROL_RICHEDIT:
		return ROLE_SYSTEM_TEXT;

	case CONTROL_PICTURE:
	case CONTROL_GIFCTRL:
		return ROLE_SYSTEM_GRAPHIC;

	default:
		return ROLE_SYSTEM_GRAPHIC;
	}
}

HRESULT  ObjectAccessible::get_accRole(VARIANT varChild,
		VARIANT *pvarRole, BOOL& bHandled)
{
	bHandled = TRUE;

	if (!pvarRole)
		return E_INVALIDARG;
	pvarRole->vt = VT_EMPTY;

	if (varChild.vt != VT_I4)
		return E_INVALIDARG;

	pvarRole->vt = VT_I4;

	Object* pObj = &m_refObject;
	if (varChild.lVal != CHILDID_SELF)
		pObj = m_refObject.GetChildObjectByIndex(varChild.lVal-1);
	if (!pObj)
		return S_FALSE;

	OBJ_TYPE eObjType = OBJ_UNKNOWN;
    if (pObj->GetDescription())
        eObjType = pObj->GetDescription()->GetMajorType();

	if (OBJ_WINDOW == eObjType || OBJ_HWNDHOST == eObjType)
	{
		pvarRole->vt = VT_I4;
		pvarRole->lVal = ROLE_SYSTEM_WINDOW;
	}
	else if (OBJ_PANEL == eObjType)
	{
		// û��panel����container���ͣ���pane�����������
		pvarRole->vt = VT_I4;
		pvarRole->lVal = ROLE_SYSTEM_PANE;  
	}
	else if (OBJ_CONTROL == eObjType)
	{
		pvarRole->vt = VT_I4;
		CONTROL_TYPE eCtrlType = CONTROL_UNKNOWN;
		if (pObj->GetDescription())
			eCtrlType = (CONTROL_TYPE)pObj->GetDescription()->GetMinorType();
		pvarRole->lVal = MapControlType(eCtrlType);
	}
	return S_OK;
}

HRESULT  ObjectAccessible::get_accState(VARIANT varChild, 
		VARIANT *pvarState, BOOL& bHandled)
{
	bHandled = TRUE;
	if (!pvarState)
		return 0;

	if (varChild.vt != VT_I4)
	{
		pvarState->vt = VT_EMPTY;
		return E_INVALIDARG;
	}

	Object* pObj = &m_refObject;
	if (varChild.lVal != CHILDID_SELF)
		pObj = m_refObject.GetChildObjectByIndex(varChild.lVal-1);
	if (!pObj)
		return S_FALSE;

	pvarState->vt = VT_I4;
	pvarState->lVal = 0;

	if (pObj->IsSelfDisable())
		pvarState->lVal |= STATE_SYSTEM_UNAVAILABLE;
	if (!pObj->IsSelfVisible())
		pvarState->lVal |= STATE_SYSTEM_INVISIBLE;
	
	if (pObj->m_objState.checked || pObj->m_objState.radio_checked)
		pvarState->lVal |= STATE_SYSTEM_CHECKED;

	if (pObj->IsTabstop())
	{
		pvarState->lVal |= STATE_SYSTEM_FOCUSABLE;
		if (pObj->IsFocus())
			pvarState->lVal |= STATE_SYSTEM_FOCUSED;
	}
	
	if (pObj->IsDefault())
		pvarState->lVal |= STATE_SYSTEM_DEFAULT;

	if (pObj->IsPress())
		pvarState->lVal |= STATE_SYSTEM_PRESSED;

	if (pObj->m_objState.readonly)
		pvarState->lVal |= STATE_SYSTEM_READONLY;

	return S_OK;

}