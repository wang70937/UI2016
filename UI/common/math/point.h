#ifndef _UI_MATH_POINT_H_
#define _UI_MATH_POINT_H_

namespace UI
{
	
// ������w����2D�����͵�
typedef struct tagPOINTF2
{
	union
	{
		float p[2];
		struct { float x, y; };
	};
}
POINTF2, VECTOR2, *LPVECTOR2, *LPPOINTF2;

// ������w������3D�����͵�
typedef struct tagPOINTF3
{
	union
	{
		float p[3];
		struct { float x, y, z; };
	};
} 
POINTF3, VECTOR3, *LPVECTOR3, *LPPOINTF3;

// ����w������4D��������͵�
typedef struct tagPOINTF4
{
	union
	{
		float p[4];
		struct { float x, y, z, w; };
	};
} 
POINTF4, VECTORF4, *LPVECTOR4, *LPPOINTF4;

// 2D ����
// typedef struct tagVERTEX2DI
// {
// 	int x, y;
// }VERTEX2DI, *VERTEX2DI_PTR;
// 
// typedef struct tagVERTEX2DF
// {
// 	float x, y;
// }VERTEX2DF, *VERTEX2DF_PTR;

}

#endif 