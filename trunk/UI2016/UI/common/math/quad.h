#ifndef _UI_MATH_QUAD_H_
#define _UI_MATH_QUAD_H_
#include "point.h"
#include "rect.h"

namespace UI
{

//
//   A ��---------------�� B
//     |               |
//     |               |
//     |               |
//   D ��---------------�� C
//
// �ı��� (ע����Щֵ�����±�������Χ����˲��������Ҳ����ײࣩ

struct QUAD
{
    union
    {
        int pos[8];

        struct
        {
            POINT  ptA;
            POINT  ptB;
            POINT  ptC;
            POINT  ptD;
        };

        struct
        {
            int Ax;
            int Ay;
            int Bx;
            int By;
            int Cx;
            int Cy;
            int Dx;
            int Dy;
        };
    };

	float  Az;  // ����ӳ��ʱ��Ҫ�õ���z����
	float  Bz;
	float  Cz;
	float  Dz;
};

struct QUADF
{
	union
	{
		float pos[8];

		struct
		{
			POINTF2  ptA;
			POINTF2  ptB;
			POINTF2  ptC;
			POINTF2  ptD;
		};

		struct
		{
			float Ax;
			float Ay;
			float Bx;
			float By;
			float Cx;
			float Cy;
			float Dx;
			float Dy;
		};
	};
};

class Quad : public QUAD
{
public:
    void  SetByRect(LPRECT prc);
    void  Offset(POINT pt);
    void  Offset(int x, int y);
    void  GetBoundRect2(RECT* prc);
	void  GetBoundRect(RECT* prc);
};

class QuadF : public QUADF
{
public:
	void  SetByRect(LPRECTF prc);
	void  Offset(float x, float y);
	void  ToQuad(QUAD* p);
};

}

#endif 