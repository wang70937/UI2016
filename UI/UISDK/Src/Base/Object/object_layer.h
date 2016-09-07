#pragma once
#include "Src/Layer/layer.h"

namespace UI
{
class Object;
class Layer;

class ObjectLayer : public ILayerContent
{
public:
    ObjectLayer(Object& o);
    ~ObjectLayer();

    Layer*  GetLayer() {
                return m_pLayer; }
    Object&  GetObjet() {
                return m_obj;  }

    void  OnSize(uint nWidth, uint nHeight);
    void  OnObjPosInTreeChanged();

protected:
    virtual bool  IsChildOf(Object*) override;
    virtual bool  IsSelfVisible() override;
    virtual void  Draw(UI::IRenderTarget*) override;
    virtual void  GetWindowRect(RECT* prcOut) override;
    virtual void  GetParentWindowRect(RECT* prcOut) override;

private:
    Object&  m_obj;
    Layer*  m_pLayer;
};

}