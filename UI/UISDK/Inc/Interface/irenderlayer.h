#ifndef _UI_IRENDERLAYER_H_
#define _UI_IRENDERLAYER_H_

namespace UI
{
interface IRenderTarget;
class WindowRender;

// ����UIEditor���ڱ༭ʱʵʱˢ��
interface IWindowCommitListener
{
    virtual void PreCommit(LPCRECT dirtyarray, int count) = 0;
    virtual void PostCommit(LPCRECT dirtyarray, int count) = 0;
};

interface UIAPI IWindowRender
{
public:
    IWindowRender(WindowRender* p);
    ~IWindowRender();

    WindowRender*  GetImpl();
    void  SetCanCommit(bool b);
    //void  Commit(HDC hDC, RECT* prc, int nCount=1);
    void  InvalidateNow();

    GRAPHICS_RENDER_LIBRARY_TYPE  GetGraphicsRenderType();
	void  SetGraphicsRenderType(GRAPHICS_RENDER_LIBRARY_TYPE  eTpye);
    bool  GetRequireAlphaChannel();
	//void  SetNeedRebuildCompositingLayerTree();
    
    void  SetCommitListener(IWindowCommitListener*);
    IWindowCommitListener*  GetCommitListener();

private:
    WindowRender*  m_pWindowRenderImpl;
};


enum LAYER_ANIMATE_TYPE
{
    STORYBOARD_ID_OPACITY = 1,
    STORYBOARD_ID_YROTATE = 2,
    STORYBOARD_ID_TRANSLATE = 3,
};

interface ILayer;
struct LayerAnimateFinishInfo
{
    ILayer* pLayer;
    LAYER_ANIMATE_TYPE eAnimateType;
    long* pUserData;
};
typedef void(*pfnLayerAnimateFinish)(LayerAnimateFinishInfo*);

struct LayerAnimateParam
{
	bool  bBlock;
	pfnLayerAnimateFinish* pCallback;
	long* pUserData;
};


class Layer;
interface UIAPI ILayer
{
    ILayer(Layer*);
    IRenderTarget*  GetRenderTarget();

    bool  IsAutoAnimate();
    void  EnableAutoAnimate(bool);
    void  SetAnimateFinishCallback(pfnLayerAnimateFinish, long*);

	void SetOpacity(unsigned char, LayerAnimateParam*);
	unsigned char GetOpacity();

    void  SetYRotate(float);
    float  GetYRotate();

    void  SetTranslate(float x, float y, float z);
    float  GetXTranslate();
    float  GetYTranslate();
    float  GetZTranslate();

private:
    Layer* m_pImpl;
};


enum TRANSFORM_TYPE
{
	TRANSFORM_2D,
	TRANSFORM_SOFT3D,
	TRANSFORM_HARD3D,
};

// ��ת����λ��
// Ĭ��ֵ��(CENTER, CENTER, 0)
enum TRANSFORM_ROTATE_ORIGIN
{
	TRANSFORM_ROTATE_ORIGIN_ABSOLUTE = 0,
	TRANSFORM_ROTATE_ORIGIN_TOP = 1,
	TRANSFORM_ROTATE_ORIGIN_LEFT = 1,
	TRANSFORM_ROTATE_ORIGIN_CENTER = 2,
	TRANSFORM_ROTATE_ORIGIN_RIGHT = 3,
	TRANSFORM_ROTATE_ORIGIN_BOTTOM = 3,
	TRANSFORM_ROTATE_ORIGIN_PERCENT = 4,
};
typedef TRANSFORM_ROTATE_ORIGIN ROTATE_CENTER_TYPE;

interface IRenderLayerTransform2
{
	virtual void  Release() = 0;
	virtual TRANSFORM_TYPE  get_type() = 0;

	// anchorPoint
	virtual void  set_transform_rotate_origin(
		TRANSFORM_ROTATE_ORIGIN eX, float fX,
		TRANSFORM_ROTATE_ORIGIN eY, float fY,
		float fZ) = 0;
	virtual void  set_size(int w, int h) = 0;
	virtual void  set_pos(int x, int y) = 0;

	virtual void  mappoint_layer_2_view(__inout POINT* ptInLayer) = 0;
	virtual void  mappoint_view_2_layer(__inout POINT* ptInLayer) = 0;
// 	virtual void  maprect_layer_2_view(
// 		__in RECT* rcInLayer, __out QUAD* pqInView) = 0;
};

// interface IRenderLayerTransform2D : public IRenderLayerTransform2
// {
// 	virtual void  get_matrix(LPMATRIX33 pMatrix) = 0;
// 
// 	virtual void  translate(float x, float y) = 0;
// 	virtual void  scale(float x, float y) = 0;
// 	virtual void  rotate(float angle) = 0;
// };

interface IRenderLayerTransform3D : public IRenderLayerTransform2
{
	virtual void  set_backface_visibility(bool b) = 0;
	virtual bool  get_backface_visibility() = 0;
	virtual bool  is_visible() = 0;

	//virtual void  get_matrix(MATRIX44* pMatrix) = 0;

	// Defines a 3D translation
	virtual void  translate3d(float x, float y, float z) = 0;

	// Defines a 3D translation, using only the value for the X-axis
	virtual void  translateX(float x) = 0;

	// Defines a 3D translation, using only the value for the Y-axis
	virtual void  translateY(float y) = 0;

	// Defines a 3D translation, using only the value for the Z-axis
	virtual void  translateZ(float z) = 0;

	// Defines a 3D scale transformation
	virtual void  scale3d(float x, float y, float z) = 0;

	// Defines a 3D scale transformation by giving a value for the X-axis
	virtual void  scaleX(float x) = 0;

	// Defines a 3D scale transformation by giving a value for the Y-axis
	virtual void  scaleY(float y) = 0;

	// Defines a 3D scale transformation by giving a value for the Z-axis
	virtual void  scaleZ(float z) = 0;

	// Defines a 3D rotation
	virtual void  rotate3d(float x, float y, float z) = 0;

	// Defines a 3D rotation along the X-axis
	virtual void  rotateX(float angle) = 0;

	// Defines a 3D rotation along the Y-axis
	virtual void  rotateY(float angle) = 0;

	// Defines a 3D rotation along the Z-axis
	virtual void  rotateZ(float angle) = 0;

	// Defines a perspective view for a 3D transformed element
	virtual void  perspective(float n) = 0;

};
}

#endif // _UI_IRENDERLAYER_H_