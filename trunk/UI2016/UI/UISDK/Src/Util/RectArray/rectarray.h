#pragma once

class RectArray
{
public:
    RectArray();
    ~RectArray();
    RectArray(RectArray& o);
    RectArray& operator=(const RectArray& o);

    LPRECT  GetArrayPtr();
    LPCRECT  GetArrayPtr2() const;
    LPRECT  GetRectPtrAt(uint nIndex);
    uint  GetCount() const;
    void  AddRect(LPCRECT);

    void  Destroy();
    void  CopyFrom(const RectArray* po);
    void  CopyFromArray(LPCRECT pArray, uint nCount);
    void  SetSize(uint nCount);
    bool  SetAt(uint nIndex, RECT*  pValue);
    void  Offset(int x, int y);
    
    // ���һ�������򣬺ϲ������е����鵱�С�
    void  UnionDirtyRect(LPCRECT prc);

    // ������֮��m_prcArray�Ĵ�С��m_nCount���ܲ�ƥ��
    bool  IntersectRect(LPCRECT prc, bool OnlyTest=false);

    HRGN  CreateRgn();
    void  GetUnionRect(RECT* prc);

    enum {
        STACK_SIZE = 3
    };
private:
    // ����ʹ��ջ���飬�Ų���ʱ��ʹ�ö����顣������string��
    RECT  m_stackArray[STACK_SIZE];
    RECT*  m_heapArray;

    // ע��m_nCountС�ڵ���m_prcArray��ʵ�ʴ�С����IntersectRect
    uint   m_nCount;
	uint   m_nCapacity;  // TODO
};
