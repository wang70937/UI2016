#pragma once
#include "Inc\Interface\ilayout.h"
#include "Src\Helper\layout\layout.h"

namespace UI{

class Object;

class CanvasLayoutParam : public LayoutParamImpl<ICanvasLayoutParam>
{
public:
    CanvasLayoutParam();
    ~CanvasLayoutParam();

    virtual void  UpdateByRect();
    virtual void  Serialize(SERIALIZEDATA* pData);
    virtual LAYOUTTYPE  GetLayoutType() { return LAYOUT_TYPE_CANVAS; }
    virtual SIZE  CalcDesiredSize();
    virtual bool  IsSizedByContent();

    static int  ParseAlignAttr(LPCTSTR);

public:
    virtual long  GetConfigLeft() override;
    virtual void  SetConfigLeft(long) override;
    void  LoadConfigLeft(long);
    long  SaveConfigLeft();

    virtual long  GetConfigTop() override;
    virtual void  SetConfigTop(long) override;
    void  LoadConfigTop(long);
    long  SaveConfigTop();

    virtual long  GetConfigRight() override;
    virtual void  SetConfigRight(long) override;
    void  LoadConfigRight(long);
    long  SaveConfigRight();

    virtual long  GetConfigBottom() override;
    virtual void  SetConfigBottom(long) override;
    void  LoadConfigBottom(long);
    long  SaveConfigBottom();

    virtual long  GetConfigWidth() override;
    virtual void  SetConfigWidth(long) override;
    void  LoadConfigWidth(long);
    long  SaveConfigWidth();

    virtual long  GetConfigHeight() override;
    virtual void  SetConfigHeight(long) override;
    void  LoadConfigHeight(long);
    long  SaveConfigHeight();

    virtual void SetConfigLayoutFlags(long) override;
    virtual long GetConfigLayoutFlags() override;

private:
    long  m_nConfigWidth;   // ����Ŀ�ȣ���ȡֵ�� ��ֵ | "auto" . ������window����width ��ָclient����Ĵ�С�������������ڵĴ�С��width����padding����������margin��
    long  m_nConfigHeight;  // ����ĸ߶ȣ���ȡֵ�� ��ֵ | "auto" . ������window����height��ָclient����Ĵ�С�������������ڵĴ�С��height����padding����������margin��
    // ��������Ҫ˵�������������ռ�õĿ��= margin.left + width + margin.right
    // Ҳ����˵�����width = padding.left + padding.right + content.width
    long  m_nConfigLeft;
    long  m_nConfigRight;
    long  m_nConfigTop;
    long  m_nConfigBottom;
    long  m_nConfigLayoutFlags;
};


//
//	ָ���������Ե��λ��
//
class CanvasLayout : public LayoutImpl<CanvasLayout, ICanvasLayout, CanvasLayoutParam, LAYOUT_TYPE_CANVAS>
{
public:
    virtual void  Serialize(SERIALIZEDATA*) override{};
    virtual SIZE  Measure() override;
    virtual void  DoArrage(IObject* pObjToArrage = NULL) override;
    virtual void  OnChildObjectVisibleChanged(IObject* pObj) override;

public:
    static void  ArrangeObject(Object*  pChild, const int& nWidth, const int& nHeight);
};

}
