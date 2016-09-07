#ifndef _UI_IIMAGERENDER_H_
#define _UI_IIMAGERENDER_H_
#include "irenderbase.h"

namespace UI
{
	interface IRenderBitmap;

enum BKCOLOR_FILL_TYPE
{
    BKCOLOR_FILL_ALL,    // Ĭ��
    BKCOLOR_FILL_EMPTY   // ֻ�ڱ���ͼû�л��Ƶĵط�������䡣��Ϊ�����fill all���ٻ�͸��λͼ�Ļ���λͼ�ı���ɫ�ͱ���Ⱦ�ˡ�����IM��������Ƥ��ͼƬ���䱳��ɫ
};
class ImageRender;
interface __declspec(uuid("6DAC8F58-390D-4660-A35F-2EBE956ED442"))
UISDKAPI IImageRender : public IRenderBase
{
    void  SetAlpha(int nAlpha);
    int   GetAlpha();
    void  SetColor(Color c);
    Color GetColor();
    void  SetImageDrawType(int n);
    int   GetImageDrawType();

    BKCOLOR_FILL_TYPE  GetBkColorFillType();
    void  SetBkColorFillType(BKCOLOR_FILL_TYPE eType);

    IRenderBitmap*  GetRenderBitmap();
    void  SetRenderBitmap(IRenderBitmap* pBitmap);

	UI_DECLARE_INTERFACE(ImageRender);
};

class ImageListItemRender;
interface __declspec(uuid("51FFF758-737E-4252-BDBF-7DAF9DB261A6"))
UISDKAPI IImageListItemRender : public IImageRender
{
    UI_DECLARE_INTERFACE(ImageListItemRender);
};

class ImageListRender;
interface IImageListRenderBitmap;
interface __declspec(uuid("34CBE966-3ADE-49F9-98D5-79584248DBDB"))
UISDKAPI IImageListRender : public IRenderBase
{
    void  SetIImageListRenderBitmap(IImageListRenderBitmap* pBitmap);
    IRenderBitmap*  GetRenderBitmap();
    void  SetImageDrawType(int n);
    int   GetImageDrawType();
    void  SetImageStretch9Region(C9Region* p);
    int   GetItemWidth();
    int   GetItemHeight();
    int   GetItemCount();

	UI_DECLARE_INTERFACE(ImageListRender);
};


}

#endif 