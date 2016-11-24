#include "stdafx.h"
#include "object.h"
#include "Inc\Interface\ilayout.h"

#include "Src\Base\Attribute\attribute.h"
#include "Src\Base\Attribute\bool_attribute.h"
#include "Src\Base\Attribute\enum_attribute.h"
#include "Src\Base\Attribute\stringselect_attribute.h"
#include "Inc\Interface\irenderbase.h"
#include "Inc\Interface\itextrenderbase.h"
#include "Inc\Interface\imapattr.h"
#include "Inc\Interface\iuiapplication.h"

void* memfun_cast2(void*& p)
{
    return p;
}

// ��xml�õ�����ת�ɶ�������ԣ�ע�⣬�������ظú���ʱ�������ȵ��ø���ĸ÷���
// bReload��ʾΪ����ʱ���ã���ʱ�����ٸ�ĳЩ���Ը�ֵ������text����
void  Object::OnSerialize(SERIALIZEDATA* pData)
{
    IMapAttribute*  pMapAttrib = pData->pMapAttrib;
	if (pData->IsReload())
	{
		SAFE_RELEASE(m_pLayoutParam);
		SAFE_RELEASE(m_pBkgndRender);
		SAFE_RELEASE(m_pForegndRender);
		SAFE_RELEASE(m_pTextRender);
		//SAFE_RELEASE(m_pCursor);
	}

    if (pData->IsLoad())
    {
        // �������ԣ�������չ��
        SAFE_RELEASE(m_pIMapAttributeRemain);
        m_pIMapAttributeRemain = pMapAttrib;
        if (m_pIMapAttributeRemain)
            m_pIMapAttributeRemain->AddRef();

        if (NULL == pMapAttrib)
        {
            UI_LOG_ERROR(_T("Invalid Argument."));
            return;
        }
    }

    {
		AttributeSerializer s(pData, TEXT("Object"));
        s.AddString(XML_ID, m_strId)->AsData();

#ifdef EDITOR_MODE
        s.AddString(XML_STYLECLASS,
            [this](LPCTSTR t){ if (t){m_strStyle = t;}else {m_strStyle.clear();} },
            [this]()->LPCTSTR{ return m_strStyle.c_str(); })
            ->ReloadOnChanged();
#endif

		// styelclass���޸�ʱ��Ӧ�����½�����������
        // s.AddString(XML_STYLECLASS, m_strStyleClass)->ReloadOnChanged();
        // s.AddString(XML_TOOLTIP, m_strTooltip);

        s.AddRect(XML_MARGIN, this,
            memfun_cast<pfnRectSetter>(&Object::LoadMargin),
            memfun_cast<pfnRectGetter>(&Object::SaveMargin));
        s.AddRect(XML_PADDING, this,
            memfun_cast<pfnRectSetter>(&Object::LoadPadding),
            memfun_cast<pfnRectGetter>(&Object::SavePadding));
	    s.AddRect(XML_BORDER, this,
            memfun_cast<pfnRectSetter>(&Object::LoadBorder),
            memfun_cast<pfnRectGetter>(&Object::SaveBorder));

        s.AddEnum(XML_VISIBLE, this,
            memfun_cast<pfnLongSetter>(&Object::LoadVisibleEx), 
            memfun_cast<pfnLongGetter>(&Object::SaveVisibleEx))
			->AddOption(VISIBILITY_COLLAPSED, XML_VISIBILITY_COLLAPSED)
			->AddOption(VISIBILITY_VISIBLE, XML_VISIBILITY_VISIBLE)
			->AddOption(VISIBILITY_HIDDEN, XML_VISIBILITY_HIDDEN)
		    ->SetDefault(VISIBILITY_VISIBLE)->AsData();

        s.AddBool(XML_DISABLE, this, 
            memfun_cast<pfnBoolSetter>(&Object::SetDisableDirect), 
            memfun_cast<pfnBoolGetter>(&Object::IsSelfDisable))
            ->AsData();

	    s.AddBool(XML_BACKGND_IS_TRANSPARENT, this, 
		    memfun_cast<pfnBoolSetter>(&Object::SetTransparent), 
		    memfun_cast<pfnBoolGetter>(&Object::IsTransparent))
            ->SetDefault(m_objStyle.default_transparent);

		s.AddLong(XML_ZORDER, m_lzOrder);  // z�� (ע��Ĭ���ڴ�xml����֮��AddChild֮ǰ���Ƚ���һ��)
	    s.AddBool(XML_NO_CLIP_DRAW, this, 
		    memfun_cast<pfnBoolSetter>(&Object::SetNoClip), 
		    memfun_cast<pfnBoolGetter>(&Object::IsNoClip));

        s.AddBool(XML_REJEST_MOUSE_MSG, this, 
            memfun_cast<pfnBoolSetter>(&Object::SetRejectMouseMsgAll), 
            memfun_cast<pfnBoolGetter>(&Object::IsRejectMouseMsgAll))
            ->SetDefault(m_objStyle.default_reject_all_mouse_msg);

		s.AddBool(XML_ISNCCHILD, this,
			memfun_cast<pfnBoolSetter>(&Object::SetAsNcObject), 
			memfun_cast<pfnBoolGetter>(&Object::IsNcObject))
			->SetDefault(m_objStyle.default_ncobject);

        s.AddBool(XML_LAYER, this, 
            memfun_cast<pfnBoolSetter>(&Object::load_layer_config), 
            memfun_cast<pfnBoolGetter>(&Object::HasLayer));

// 	    s.AddString(XML_CURSOR, this, 
// 		    memfun_cast<pfnStringSetter>(&Object::SetCursorId),
// 		    memfun_cast<pfnStringGetter>(&Object::SaveCursorId));

       s.AddRenderBase(XML_BACKGND_RENDER_PREFIX, this, m_pBkgndRender);  // ���ñ�����Ⱦ��
	   s.AddRenderBase(XML_FOREGND_RENDER_PREFIX, this, m_pForegndRender); // ����ǰ������

//         s.AddStringEnum(XML_BACKGND_RENDER_PREFIX XML_RENDER_TYPE, this,
//             memfun_cast<pfnStringSetter>(&Object::LoadBkgndRender),
//             memfun_cast<pfnStringGetter>(&Object::SaveBkgndRender))
//             ->FillRenderBaseTypeData()
//             ->ReloadOnChanged();
//         s.AddStringEnum(XML_FOREGND_RENDER_PREFIX XML_RENDER_TYPE, this,
//             memfun_cast<pfnStringSetter>(&Object::LoadForegndRender),
//             memfun_cast<pfnStringGetter>(&Object::SaveForegndRender))
//             ->FillRenderBaseTypeData()
//             ->ReloadOnChanged();

	   // ��������
// 	   if (m_pBkgndRender)
// 	   {
// 		   SERIALIZEDATA data(*pData);
// 		   data.szParentKey = XML_BACKGND_RENDER_PREFIX XML_RENDER_TYPE;
// 		   data.szPrefix = XML_BACKGND_RENDER_PREFIX;
// 
// 		   // ��editor�У���̬�޸�render type��Ҫ������ԡ�
// 		   // 1. ���Կ��ܹ���һ��key����render.image=����ʹ����type������Ҳ���Թ���
// 		   // 2. Ҫʵ��undo/redo�����ܶ�������
// 		   if (data.pUIApplication->IsDesignMode())
// 			   data.SetErase(false);
// 
// 		   m_pBkgndRender->Serialize(&data);
// 	   }
// 
// 	   // ǰ������
// 	   if (m_pForegndRender)
// 	   {
// 		   SERIALIZEDATA data(*pData);
// 		   data.szParentKey = XML_FOREGND_RENDER_PREFIX XML_RENDER_TYPE;
// 		   data.szPrefix = XML_FOREGND_RENDER_PREFIX;
// 		   if (data.pUIApplication->IsDesignMode())
// 			   data.SetErase(false);
// 
// 		   m_pForegndRender->Serialize(&data);
// 	   }
    }

	// ��������
	if (m_pLayoutParam)
	{
		m_pLayoutParam->Serialize(pData);
	}
	else if (pData->IsReload())
	{
		CreateLayoutParam();
	}

	
}

void  Object::virtualOnLoad()
{
    // �ȶ���������λ��ȷ���ˣ��������ȷ����֮���ٴ���layer
    update_layer_ptr();
}