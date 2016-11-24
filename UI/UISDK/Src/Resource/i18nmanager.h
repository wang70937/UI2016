#pragma once
#include "i18nres.h"

namespace UI
{
interface UIElement;
interface IUIElement;

class I18nManager
{
public:
	I18nManager(SkinRes* p) : m_i18nRes(p) {
		m_pSkinRes = p;
	}
    ~I18nManager();

	I18nRes&  GetI18nRes() {
		return m_i18nRes;
	}

	static HRESULT  UIParseI18nTagCallback(IUIElement*, ISkinRes* pSkinRes);
	LRESULT  ParseNewElement(UIElement*);

    void  Reload();
    
private:
    void  parse(UIElement*);

private:
	I18nRes  m_i18nRes;
	SkinRes*  m_pSkinRes;

    // xml����¼�����ڶ�̬�л����ԣ���༭���У�
    list<UIElement*>  m_listElement;
};



}