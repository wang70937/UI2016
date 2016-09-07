#pragma once
// #include "UISDK\Project\UICompositor\inc\ihardwarecompositor.h"
// #pragma comment(lib, "UICompositor.lib")


////
// ���𴰿ڵ���Ⱦ�������ǲ��������ʽ������Ӳ���ϳ�
//
// 2014.3.10
//
// ͳһGraphics Render Library��ʹ�ã�һ�����ڲ��ٻ��ʹ�ã�ֻ��ʹ��һ��Graphics Render Library
// ���򴦴�Ҫ���Ǵ���ʲô���͵���Ⱦ�⣬��Ĵ�����ң��߼�Ҳ��
//
namespace UI
{
class Object;
class WindowBase;
interface IWindowRender;
class Compositor;
class Layer;

class WindowRender
{
public:
    WindowRender(WindowBase*  p);
    ~WindowRender();

    IWindowRender*  GetIWindowRender();

public:
    void  BindHWND(HWND hWnd);   
	void  OnSerialize(SERIALIZEDATA* pData);
    void  OnWindowSize(UINT nWidth, UINT nHeight);

    bool  CreateRenderTarget(IRenderTarget** pp);
    
    bool  GetRequireAlphaChannel();
    void  SetGraphicsRenderType(GRAPHICS_RENDER_LIBRARY_TYPE  eTpye);
    GRAPHICS_RENDER_LIBRARY_TYPE  GetGraphicsRenderType();
    void  SetCanCommit(bool b);
    bool  CanCommit();

    void  OnInvalidate();
    void  OnPaint(HDC hDC, RECT* prcInvalid);

	Layer*  CreateLayer(Object*);

private:
    Compositor*  get_create_compositor();

public:
    IWindowRender*  m_pIWindowRender;
    GRAPHICS_RENDER_LIBRARY_TYPE  m_eGRL;
    WindowBase*    m_pWindow;

    long  m_lRefCanCommit;

    // �ô��ڵ���Ⱦ�Ƿ���Ҫalphaͨ�������ٸ���graphics render lib type�����������û��Լ�����
    bool  m_bNeedAlphaChannel;       

private:
    Compositor*  m_pCompositor;
};

}