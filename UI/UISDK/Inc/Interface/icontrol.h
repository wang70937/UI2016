#ifndef _UI_ICONTROL_H_
#define _UI_ICONTROL_H_

#include "iobject.h"

namespace UI
{
	interface ITextRenderBase;

// control style
// #define CONTROL_STYLE_GROUP             0x00002000    // �Ƿ���һ������Ŀ�ʼ
// #define CONTROL_STYLE_OWNERDRAW         0x00004000    // �Ƿ��Ի�
typedef struct tagControlStyle
{
	bool  group : 1;
	bool  ownerdraw : 1;
}ControlStyle;

class Control;
struct UIAPI IControl// UIAPI_UUID(78EC44D9-137E-46A1-8ACA-0E12A209FF4F)
         : public IObject
{
   void  ModifyControlStyle(ControlStyle* add, ControlStyle* remove);
   bool  TestControlStyle(ControlStyle* test);
   bool  IsGroup();
   void  SetGroup(bool);

   LPCTSTR  GetToolTipText();
   void  SetToolTipText(LPCTSTR szText);

   ITextRenderBase*  CreateDefaultTextRender();
   ITextRenderBase*  GetTextRenderDefault();

   UI_DECLARE_INTERFACE(Control);
};

}

#endif  // _UI_ICONTROL_H_