#pragma once
#include "Inc\Interface\iobjectdescription.h"
#include "Inc\Interface\ipanel.h"

class PanelDescription : public IObjectDescription
{
public:
	static IObjectDescription*  Get()
	{
		static PanelDescription s;
		return &s;
	}

    virtual void  CreateInstance(UI::ISkinRes* p, void** pp) override
    {
        *pp = IPanel::CreateInstance(p);
    }

	virtual OBJ_TYPE  GetMajorType() override
	{
		return OBJ_PANEL;
	}
	virtual long  GetMinorType() override
	{
		return PANEL_SIMPLE;
	}
	virtual const wchar_t*  GetCategory() override
	{
		return CATEGORY_CONTAINER; 
	}
	virtual const wchar_t*  GetTagName() override
	{
		return L"Panel";
	}
	virtual const GUID&  GetGUID() override
	{
		return __uuidof(IPanel);
	}

    virtual void  GetDefaultSize(long& w, long h) override
    {
        w = 100;
        h = 100;
    }
    virtual HBITMAP  LoadIcon() override 
    {
        return NULL;
    }
};