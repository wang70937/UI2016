#ifndef _UI_ICUSTOMWINDOW_H_
#define _UI_ICUSTOMWINDOW_H_

#include "iwindow.h"

namespace UI
{
class CustomWindow;
interface IWindowTransparent;

interface __declspec(uuid("AB69FBED-6311-4980-8444-B5D880568B10"))
UISDKAPI ICustomWindow : public IWindow
{
	ICustomWindow();  // �ڲ�ֱ�ӵ���ICustomWindow(CREATE_IMPL_TRUE)

    void  SetWindowResizeType(UINT nType);
    void  SetWindowTransparentType(WINDOW_TRANSPARENT_TYPE eMode);
    WINDOW_TRANSPARENT_TYPE  GetWindowTransparentType();
	IWindowTransparent*  GetWindowTransparent();

    UI_DECLARE_INTERFACE(CustomWindow);
};
typedef ObjectCreator<ICustomWindow>  CCustomWindow;

}

#endif // _UI_ICUSTOMWINDOW_H_