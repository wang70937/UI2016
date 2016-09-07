#pragma once
#include "..\icaret.h"

namespace UI
{

// �ɿؼ��Լ����û��ƣ�ÿ��timerʱ����ˢ�������ؼ���
// �������͵Ĺ��һ��ֻ������Ҫ������Edit/RichEdit���棬
// ��������Ʋü������⡣���ÿ����˸����һ���Ŀ���

// ���λ��Ϊ�ؼ��ͻ�����

class ControlDrawCaret : public ICaret
{
public:
    ControlDrawCaret(ICaretUpdateCallback* pCallback);
    ~ControlDrawCaret();

    virtual CARET_TYPE  GetType() { return CARET_TYPE_CONTROLDRAW; }
    virtual BOOL  Create(IObject* pObj, HWND hWndParent, HBITMAP hbmp, int nWidth, int nHeight);
    virtual void  Destroy(bool bRedraw);
    virtual void  SetPos(int x, int y, bool bRedraw);
    virtual void  Show(bool bRedraw);
    virtual void  Hide(bool bRedraw);
    virtual void  OnControlPaint(IRenderTarget* p);
    virtual IObject*  GetObject() { return m_pObject; }

    void  OnTimer();
protected:
    void  CreateTimer();
    void  DestroyTimer();
    void  UpdateControl();
    void  UpdateObjectEx(RECT* prcArray, int nCount);

private:
    HWND   m_hWnd;
    HBITMAP  m_hBitmap;
    UI::IRenderBitmap*  m_pRenderBitmap;

    IObject*  m_pObject;
    POINT  m_ptLast;
    SIZE   m_sizeCaret;
    SIZE   m_sizeItalicFontCaretOffset;  // ���ڽ��RichEdit���棬��б���ֹ��λ����ʾ����ȷ����

    
    CoordType  m_eCoordType;
    ICaretUpdateCallback*  m_pCallback;

    CRect  m_rcDraw;
    UINT_PTR  m_nTimerId;
    bool   m_bOn;  // ��ǰ�Ƿ������ShowCaret
    bool   m_bVisible; // ��¼������˸״̬

};

}