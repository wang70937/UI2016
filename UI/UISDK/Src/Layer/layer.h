#pragma once
#include "Src\Util\RectArray\rectarray.h"
#include "Inc\Interface\irenderlayer.h"

namespace UI
{
    class Object;
    class Compositor;
}

// 2016.6.4 ��д��Ⱦ����
// Ĭ����֧��Ӳ������Ϊ������Ȼ����������Ⱦ
//
// ÿ��layer����Ӧһ���Դ��ϵ������������Ӳ���ϳ�
//
// layer��������object�����Ե������ڡ�object����Ⱦ��ֱ�ӹ�ϵ��ֻ������
// �ṩlayer�������Լ���������Ϣ��

// http://ariya.ofilabs.com/2013/06/optimizing-css3-for-gpu-compositing.html
// To minimize the amount of texture uploads, you can only animate or
// transition the following properties: opacity, transform, and filter.
// Anything else may trigger a layer update. 
// ʲô���Ķ�������Ҫ�ϴ����ݵ�gpu: �޸�͸���ȡ��任���˾�??
// �任������ɫ�ǲ��еģ���Ҫ�����ϴ�����ͼ������http://codepen.io/ariya/full/xuwgy
// ���ֶ����ͻ�ܿ������ֶ�������ͨ����������ɫ���������ƣ������ϸı�����һ����͸����

// https://wiki.mozilla.org/Gecko:Layers
// Scrolling
// What should we do to scroll?
// Bas: Use a tile cache.
//
// ���⣺layerҲҪ�����ڸ�object���������ƣ�����listitem��layer��ԶҪ��listctrl���棬
//       ��listctrl��һ��������layer�����Խ�layer��Ϊһ����Ⱦ�����������⡣
// �� Compositor ��ʱ�������Ҫ���������layer������Ҫ��parent object rect������clip
//
//
//
// ����. root layer����������
// ÿ����һ��layerʱ����ȥ����������layer tree�ĵ�һ�������Ϊroot layer
//

namespace UI
{

class Object;

interface ILayerContent
{
    virtual bool  IsChildOf(Object*) = 0;
    virtual bool  IsSelfVisible() = 0;
    virtual void  Draw(UI::IRenderTarget*) = 0;
    virtual void  GetWindowRect(RECT* prcOut) = 0;
    virtual void  GetParentWindowRect(RECT* prcOut) = 0;
};

enum LayerType
{
    Layer_Software,
    Layer_Hardware,
};

class Layer : public UIA::IAnimateEventCallback
{
protected:
	enum {
		ANIMATE_DURATION = 250,
	};

public:
	Layer();
    virtual ~Layer();
	
    ILayer*  GetILayer();
    void  SetCompositorPtr(Compositor*);
   
    bool  AddSubLayer(Layer*, Layer* pInsertBefore);
    void  MoveLayer2NewParentEnd(Layer* pOldParent, Layer* pNewParent);
    void  RemoveMeInTheTree();

    void  PostCompositorRequest();
	void  Invalidate(LPCRECT prcDirtyInLayer);
	void  Invalidate(LPCRECT prcArray, uint nCount);
    void  CopyDirtyRect(RectArray& arr);

    void  SetContent(ILayerContent*);
    ILayerContent*  GetContent();

    Layer*  GetNext();
    Layer*  GetParent() {
               return m_pParent; }
    Layer*  GetFirstChild();

	void  OnSize(uint nWidth, uint nHeight);

// 	void  SetColor(color);
// 	void  SetTransform(matrix);

    // property
	byte  GetOpacity();
	void  SetOpacity(byte, LayerAnimateParam*);
    
    void  SetYRotate(float);
    float  GetYRotate();
    void  SetTranslate(float x, float y, float z);
    float  GetXTranslate();
    float  GetYTranslate();
    float  GetZTranslate();

    virtual void  UpdateDirty() {}
	virtual void  MapView2Layer(POINT* pPoint){};
    virtual LayerType  GetType() = 0;

    bool  IsAutoAnimate();
    void  EnableAutoAnimate(bool);
//     void  SetAnimateFinishCallback(pfnLayerAnimateFinish, long*);

protected:
	virtual UIA::E_ANIMATE_TICK_RESULT  OnAnimateTick(UIA::IStoryboard*) override;
    virtual void  OnAnimateEnd(UIA::IStoryboard*, UIA::E_ANIMATE_END_REASON e) override;
    virtual void  virtualOnSize(uint nWidth, uint nHeight) = 0;

private:
    bool  do_add_sub_layer(Layer*);
    bool  do_insert_sub_layer(Layer* p, Layer* pInsertAfter);
    void  on_layer_tree_changed();

protected:
    ILayer  m_iLayer;
    Compositor*  m_pCompositor;

	// Layer Tree
	Layer*  m_pParent;
	Layer*  m_pFirstChild;
	Layer*  m_pNext;
	Layer*  m_pPrev;

	// 
    SIZE  m_size;
	RectArray  m_dirtyRectangles;
	
    ILayerContent*  m_pLayerContent;

    // ��layer���õ���������(ע�����Ǹ�layer��)
    bool  m_bClipLayerInParentObj;

 	long  m_bAutoAnimate;
//     pfnLayerAnimateFinish  m_pAnimateFinishCallback;
//     long*  m_pAnimateFinishCallbackUserData;

	// ����
	byte  m_nOpacity;         // ���õ�ֵ
	byte  m_nOpacity_Render;  // ���������е�ֵ

// 	Transform3D  m_transfrom3d;  // ���������е�ֵ
 	float  m_fyRotate;   // ���õ�ֵ
    float  m_xTranslate; // ���õ�ֵ
    float  m_yTranslate; // ���õ�ֵ
    float  m_zTranslate; // ���õ�ֵ
};
}