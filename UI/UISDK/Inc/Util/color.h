#ifndef _UI_COLOR_H_
#define _UI_COLOR_H_

namespace UI
{
// ��ɫ
#if 0
#define MIN_HUE_VALUE           0              // ɫ��  HLS��Χ����
#define MAX_HUE_VALUE           240
#define MIN_LUMINANCE_VALUE     0              // ����
#define MAX_LUMINANCE_VALUE     240            
#define MIN_SATURATION_VALUE    0              // ���Ͷ�
#define MAX_SATURATION_VALUE    240            
#else
#define MIN_HUE_VALUE           0              // ɫ��  HLS��Χ����
#define MAX_HUE_VALUE           360
#define MIN_LUMINANCE_VALUE     0              // ����
#define MAX_LUMINANCE_VALUE     1            
#define MIN_SATURATION_VALUE    0              // ���Ͷ�
#define MAX_SATURATION_VALUE    1            
#endif

#define CHANGE_SKIN_HLS_FLAG_H             1
#define CHANGE_SKIN_HLS_FLAG_L             2
#define CHANGE_SKIN_HLS_FLAG_S             4
#define CHANGE_SKIN_HLS_FLAG_HLS           (CHANGE_SKIN_HLS_FLAG_H|CHANGE_SKIN_HLS_FLAG_L|CHANGE_SKIN_HLS_FLAG_S)
#define CHANGE_SKIN_HLS_FALG_REPLACE_MODE  8 // ��ɫģʽ??


typedef struct 
{
    double  hue;         // [0 ~ 360^o)
    double  saturation;  // [0, 1]
    double  lightness;   // [0, 1]
}HSL;

typedef struct
{
    double  hue;         // [0 ~ 360^o)
    double  saturation;  // [0, 1]
    double  value;       // [0, 1]
}HSV, HSB;

class UISDKAPI Color
{
public:
    Color();
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    Color(DWORD rgba);
    static Color* CreateInstance(COLORREF color);

    operator COLORREF();
    union
    {
        COLORREF   m_col;
        struct  
        {
			unsigned char   r;  // ע��λͼ�ڴ�����ɫ������˳��Ϊb g r a
			unsigned char   g;
			unsigned char   b;
			unsigned char   a;
        };
    };

	unsigned char   GetR();
	unsigned char   GetG();
	unsigned char   GetB();
	unsigned char   GetA();

	void  SetR(unsigned char n);
	void  SetG(unsigned char n);
	void  SetB(unsigned char n);
	void  SetA(unsigned char n);

    // ����ALPHAWֵ��ɫ����
    void  ReplaceRGB(COLORREF c);
	void  ReplaceRGB(unsigned char r1, unsigned char g1, unsigned char b1);
    void  SetColor(COLORREF c);
    COLORREF  GetGDIValue();

    void  GetHSL(HSL& hsl);
    void  GetHSV(HSV& hsv);
    void  SetHSL(HSL& hsl);
    void  SetHSV(HSV& hsv);
    
    void  ToHexString(TCHAR* szBuffer);

    // ���ⲿʹ��Color����ʱ�����п�����ͨ��GetColor��ȡ����Color*ָ�룬Ҳ�п����ⲿ�Լ�ֱ������
    // һ��COLORREFֵ�����ʱ��͵�newһ��Color*��ʹ���߼���GetColor����һ�¡�Ϊ�˴ﵽ�ͷ��߼�һ
    // �µ�Ŀ�ģ����ͷź�����д��Release()
    long  AddRef();
    long  Release();

private:
    long  lRef;
};
typedef Color* ColorPtr;

}
#endif  // _UI_COLOR_H_
