#ifndef XMLDEFINE_H_CBBD5468_6BF1_45ac_B02C_F10AC19A651A
#define XMLDEFINE_H_CBBD5468_6BF1_45ac_B02C_F10AC19A651A

namespace UI
{

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                XML�е�tag����                               //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#define XML_SEPARATOR             _T(',')            // xml�еķָ��������ʾ��ɫ������
#define XML_PATH_SEPARATOR        _T('/')            // xml�е�·���ָ���������ʽ·�����Ӷ���·����
#define XML_MULTI_SEPARATOR       _T(';')            // xml�еķָ���,��ָ�һ���ؼ��Ķ��style.
#define XML_FLAG_SEPARATOR        _T('|')            // xml�еķָ��������ı����뷽ʽ: left|top|vcenter
#define XML_CHILD_SEPARATOR       _T('.')            // xml�и���֮��ķָ�������Menu.String
#define XML_KEYVALUE_SEPARATOR    _T(':')            // xml��key:value֮��ķָ���
#define XML_HEADER                _T("<?xml version=\"1.0\" encoding=\"utf-8\" ?>")


#define XML_BOOL_VALUE_YES        _T("yes")
#define XML_BOOL_VALUE_NO         _T("no")
#define XML_BOOL_VALUE_1          _T("1")
#define XML_BOOL_VALUE_0          _T("0")
#define XML_BOOL_VALUE_TRUE       _T("true")
#define XML_BOOL_VALUE_FALSE      _T("false")

// #define XML_UI_XMLFILE_EXT   _T(".ui")          // ���������ļ��ĺ�׺��
#define XML_SKIN_XML              _T("skin.xml")
#define XML_SKIN_PACKET_EXT       _T("skn")        // Ƥ������׺��

#define XML_UI               _T("ui")            // root
#define XML_SKIN             _T("skin")          // root
#define XML_XML              _T("xml")
#define XML_LOG              _T("log")
#define XML_IMG              _T("image")
#define XML_FONT             _T("font")
#define XML_COLOR            _T("color")
#define XML_LAYOUT           _T("layout")
#define XML_STYLE            _T("style")
#define XML_I18N             _T("i18n")
#define XML_I18N_PREFIX      _T('#')
#define XML_INCLUDE          _T("include")
#define XML_PROJECT_EXT      _T("uiproj")          // �����ļ���׺��
#define XML_PROJECT_EXT2     _T(".uiproj")
#define XML_LOG_UI_ID        _T("ui")             // ui��־id
#define XML_LAYOUTCONFIG     _T("layoutconfig")   // �ӳټ���Ƥ���б�
#define XML_PROP             _T("prop")           // �������ԣ�����������һ��
#define XML_KEY              _T("key")            // <prop> </prop>�����key

#define XML_ITEM             _T("item")
#define XML_IMAGE_ITEM_GIF   _T("gif")
#define XML_IMAGE_ITEM_CURSOR _T("cursor")
#define XML_ID               _T("id")
#define XML_NAME             _T("name")
#define XML_TEXT             _T("text")
#define XML_TOOLTIP          _T("tooltip")
#define XML_PATH             _T("path")
#define XML_VISIBLE          _T("visible")
#define XML_READONLY         _T("readonly")
#define XML_DISABLE          _T("disable")
#define XML_FILE             _T("file")
#define XML_WINDOW           _T("window")
#define XML_WINDOW_          _T("Window")
#define XML_LISTITEM         _T("listitem")
#define XML_LISTITEM_        _T("ListItem")

#define XML_ALIGN_LEFT       _T("left")
#define XML_ALIGN_RIGHT      _T("right")
#define XML_ALIGN_TOP        _T("top")
#define XML_ALIGN_BOTTOM     _T("bottom")
#define XML_ALIGN_CENTER     _T("center")
#define XML_ALIGN_VCENTER    _T("vcenter")

enum ALIGN_TYPE
{
    ALIGN_LEFT = 0x00,
    ALIGN_TOP  = 0x01,     
    ALIGN_RIGHT  = 0x02,     
    ALIGN_BOTTOM = 0x04,      
    ALIGN_CENTER =  0x08,
    ALIGN_VCENTER = 0x10,
};


#define XML_FONT_FACENAME    _T("face")
#define XML_FONT_FACENAME2   _T("face2")  // ��facename���岻����ʱ������facename2
#define XML_FONT_HEIGHT      _T("size")
#define XML_FONT_ORIENTATION _T("orientation") 
#define XML_FONT_BOLD        _T("bold")
#define XML_FONT_ITALIC      _T("italic")
#define XML_FONT_UNDERLINE   _T("underline")
#define XML_FONT_STRIKEOUT   _T("strikeout")  
#define XML_FONT_CLEARTYPE   _T("cleartype")   // �Ƿ�ʹ��cleartype��ʽ

#define XML_IMAGE_THEMEHLS            _T("themehls")    // �ı�Ƥ��ɫ��ʱ����ͼƬ�Ƿ���� 0/1��Ĭ��Ϊ0
#define XML_IMAGE_NEED_ANTIALIASING   _T("antialiasing")  // ͼƬ�Ƿ���Ҫ֧�ֿ���ݣ�����ʱ��
#define XML_IMAGE_DPI_ADAPT           _T("dpiadapt")    // �Ƿ�DPI����Ӧ����

#define XML_IMAGE_ITEM_TYPE           _T("type")          // image����
#define XML_IMAGE_ITEM_TYPE_IMAGE     _T("image")         // Ĭ��ֵ
#define XML_IMAGE_ITEM_TYPE_ICON      _T("icon")          // ͼ�� 
#define XML_IMAGE_ITEM_TYPE_IMAGELIST _T("imagelist")     // ͼƬ�б�
#define XML_IMAGE_ITEM_TYPE_GIF       _T("gif")           // gifͼƬ
#define XML_IMAGE_ITEM_TYPE_PNGLISTGIF _T("pnglistgif")   // png�б���ʽ��gifͼƬ
enum IMAGE_ITEM_TYPE
{
    IMAGE_ITEM_TYPE_IMAGE,
    IMAGE_ITEM_TYPE_ICON,
    IMAGE_ITEM_TYPE_IMAGE_LIST,
    IMAGE_ITEM_TYPE_GIF,
    IMAGE_ITEM_TYPE_PNGLISTGIF,
    IMAGE_ITEM_TYPE_CURSOR,
};

#define XML_IMAGE_ICON_WIDTH          _T("width")         // Ҫ���ص�ͼ����
#define XML_IMAGE_ICON_HEIGHT         _T("height")        // Ҫ���ص�ͼ��߶�
#define XML_IMAGE_IMAGELIST_COUNT     _T("count")         // ͼƬ�б������Ŀ 
#define XML_IMAGE_IMAGELIST_LAYOUT    _T("layout")        // ͼƬ�б�����Ų���ʽ
#define XML_IMAGE_IMAGELIST_LAYOUT_H  _T("h")  
#define XML_IMAGE_IMAGELIST_LAYOUT_V  _T("v")

#define XML_COLOR_USESKINHLS          _T("useskinhls")    // �ı�Ƥ��ɫ��ʱ������ɫ�Ƿ���� 0/1��Ĭ��Ϊ1

// ��������
#define XML_PROJECT_NAME       _T("projectname")     // ��������
#define XML_PROJECT_SELECT     _T("select")          // ��ǰ����ʹ�����Ŀ�Ƥ������
#define XML_PROJECT_SKIN       _T("skin")            // ������һ��Ƥ��
#define XML_PROJECT_SKIN_NAME  _T("name")            // Ƥ������
#define XML_PROJECT_SKIN_PATH  _T("path")            // Ƥ��ui.xml����·��
#define XML_PROJECT_SKIN_HLS   _T("hls")             // ��Ƥ����ɫ����Ϣ,��"10,0,0"

#define XML_SKINBUILDER_CTRLSDLL _T("ctrlsdll")      // �ⲿ�ؼ����ڵ�DLL������ע�ᴴ����Ϣ

//
// layout.xml
//

// Object��������

//#define XML_ID                   _T("id")            // ÿ������ı�ʶ��
#define XML_STYLECLASS           _T("style")         // �������ʽ����
#define XML_WIDTH                _T("width")         // ����Ŀ�ȣ�����Ϊ auto | ��ֵ
#define XML_HEIGHT               _T("height")        // ����ĸ߶ȣ�����Ϊ auto | ��ֵ
#define XML_MARGIN               _T("margin")        // �������߾�
#define XML_PADDING              _T("padding")       // ������ڱ߾�
#define XML_BORDER               _T("border")        // ����ı߿�
#define XML_MINWIDTH             _T("minwidth")      // �������С���
#define XML_MINHEIGHT            _T("minheigth")     // �������С�߶�
#define XML_MAXWIDTH             _T("maxwidth")      // ����������
#define XML_MAXHEIGHT            _T("maxheight")     // ��������߶�
#define XML_AUTO                 _T("auto")          // ����Ŀ�͸�����Ϊ���ڲ����ݾ���
#define XML_ASTERISK             _T("*")             // ����Ŀ�͸�����Ϊƽ��
#define XML_AVERAGE              _T("avg")           // ����Ŀ�͸�����Ϊƽ��
#define XML_PERCENT              _T("percent")       // ����Ŀ�͸�����Ϊ�ٷֱ� pct ?
#define XML_PERCENT_CHAR         _T('%') 
#define XML_AVERAGE_CHAR         _T('*')             // �ݲ�֧�� x*,

// EraseBkgnd
#define XML_BACKGND_IS_TRANSPARENT        _T("transparent")             // �Ƿ���Ҫ����͸������ˢ�ϸ�����ı�����0/1
#define XML_NO_CLIP_DRAW                  _T("noclip")                  // �ڻ����Լ�ʱ������Ҫά����������Ĭ�ϼ���

enum VISIBILITY_TYPE
{	
	VISIBILITY_COLLAPSED = 0,   // visible=false, need no layout
	VISIBILITY_VISIBLE = 1,     // visible=true
	VISIBILITY_HIDDEN = 2       // visible=false, but layout
};
#define  XML_VISIBILITY_COLLAPSED         _T("collapsed")
#define  XML_VISIBILITY_VISIBLE           _T("visible")
#define  XML_VISIBILITY_HIDDEN            _T("hidden")

#define XML_RENDER                        _T("render")
#define XML_RENDER_TYPE                   _T("render.type")             // ������Ⱦ����
#define XML_RENDER_TYPE_GRADIENT          _T("gradient")                // ���򽥱� Color + Color
#define XML_RENDER_TYPE_IMAGE             _T("image")                   // ��ͼImage + Color
#define XML_RENDER_TYPE_IMAGELISTITEM     _T("imagelistitem")           // ͼƬ�б��е�һ���Ҫָ��������
#define XML_RENDER_TYPE_IMAGELIST         _T("imagelist")               // ͼƬ�б�
#define XML_RENDER_TYPE_COLOR             _T("color")                   // ��ɫ��� Color
#define XML_RENDER_TYPE_SYSCOLOR          _T("syscolor")                // ϵͳɫ��� Color
#define XML_RENDER_TYPE_COLORLIST         _T("colorlist")               // ����office 2003������ʽ��ť
#define XML_RENDER_TYPE_NOTHEME           _T("notheme")                 // win2000����ؼ�
#define XML_RENDER_TYPE_THEME             _T("theme")                   // ϵͳ����ؼ�
#define XML_RENDER_TYPE_NULL              _T("null")                    // �����Ƹ�render����Ҳ��ʵ������renderָ�룬����ؼ�Ĭ��ʹ��theme����


#define XML_RENDER_THEME_DISABLE          _T("render.theme.disable")    // �Ƿ�ǿ��ʹ�����������Ⱦ
#define XML_RENDER_COLOR                  _T("render.color")            // �����ɫ
#define XML_RENDER_BORDER                 _T("render.border")           // �߿��С RECT
#define XML_RENDER_BORDERCOLOR            _T("render.border.color")     // �߿���ɫ
#define XML_RENDER_COLORFROM              _T("render.color.from")       // ������ʼɫ
#define XML_RENDER_COLORTO                _T("render.color.to")         // �����յ�ɫ
#define XML_RENDER_COLOR_GRADIENT_DIR     _T("render.color.gradient.dir") // ����ɫ����

#define XML_RENDER_COLOR_GRADIENT_DIR_H   _T("h")
#define XML_RENDER_COLOR_GRADIENT_DIR_V   _T("v")
enum
{
    RENDER_COLOR_GRADIENT_DIR_H,
    RENDER_COLOR_GRADIENT_DIR_V,
};

#define XML_RENDER_IMAGE                  _T("render.image")            // ����ͼƬ
#define XML_RENDER_IMAGE9REGION           _T("render.image.9region")    // ����������ʽ
#define XML_RENDER_LIST_STATEMAPINDEX     _T("render.list.statemap")    // ���ÿؼ�����״̬��Ӧ��ͼƬ�����0:0,1:0,2:0,3:0,4:1 ��ʾ������ͼƬ���selection�õڶ���ͼƬ�������õ�һ��ͼƬ
#define XML_RENDER_COLORLIST_COUNT        _T("render.colorlist.count")  // ��ɫ�б������
// #define XML_RENDER_IMAGELIST_COUNT        _T("render.imagelist.count")  // ͼƬ�б��е�ͼƬ����
// #define XML_RENDER_IMAGELIST_LAYOUT       _T("render.imagelist.layout") // ͼƬ���з���(Ĭ��Ϊ����)
// #define XML_RENDER_IMAGELIST_LAYOUT_H     _T("h")                       // ͼƬ��������
// #define XML_RENDER_IMAGELIST_LAYOUT_V     _T("v")                       // ͼƬ��������
#define XML_RENDER_IMAGELIST_ALPHA_ANIMATE _T("render.imagelist.alphaanimate") // �Ƿ�����alpha���䶯��

#define XML_RENDER_IMAGELISTITEM_INDEX    _T("render.imagelistitem.index")  // ͼƬ�б��������ֵ

#define XML_RENDER_IMAGE_DRAWTYPE         _T("render.image.drawtype")   // ͼƬ���Ʒ�ʽ
#define XML_RENDER_IMAGE_DRAWTYPE_BITBLT  _T("bitblt")                  // (0,0)λ�ÿ�ʼ��ͼImage + Color
#define XML_RENDER_IMAGE_DRAWTYPE_TILE    _T("tile")                    // (0,0)λ�ÿ�ʼƽ��Image
#define XML_RENDER_IMAGE_DRAWTYPE_STRETCH _T("stretch")                 // ����ģʽImage + Image9Region*
#define XML_RENDER_IMAGE_DRAWTYPE_ADAPT   _T("adapt")                   // ����Ӧ���У����ᳬ�����Ʒ�Χ
#define XML_RENDER_IMAGE_DRAWTYPE_CENTER  _T("center")                  // ���У����ܳ������Ʒ�Χ
#define XML_RENDER_IMAGE_DRAWTYPE_STRETCH_BORDER      _T("stretchborder")     // ͨ��9��������Ʊ߿򣬲������м䲿��
#define XML_RENDER_IMAGE_DRAWTYPE_BITBLT_RIGHTTOP     _T("bitbltrighttop")    // ���ϽǶ���
#define XML_RENDER_IMAGE_DRAWTYPE_BITBLT_LEFTBOTTOM   _T("bitbltleftbottom")  // ���½Ƕ���
#define XML_RENDER_IMAGE_DRAWTYPE_BITBLT_RIGHTBOTTOM  _T("bitbltrightbottom") // ���½Ƕ���

#define XML_RENDER_IMAGE_ALPHA            _T("render.image.alpha")      // ͼƬ����ʱʹ�õ�alphaֵ
#define XML_RENDER_IMAGE_SRC_REGION       _T("render.image.srcregion")  // ͼƬ���Ƶ�Դ����

#define XML_BACKGND_RENDER_PREFIX         _T("back.")    // ������Ⱦ����ǰ׺����back.render.type="" fore.render.image=""
#define XML_FOREGND_RENDER_PREFIX         _T("fore.")    // ǰ��
#define XML_ICON_RENDER_PREFIX            _T("icon.")    // ͼ��������Ⱦǰ׺
#define XML_BKGCOLOR                      _T("bkgcolor")  // ������ɫ
#define XML_BKGCOLOR_PREFIX               _T("bkgcolor.") // ������ɫǰ׺

enum IMAGELIST_LAYOUT_TYPE
{
    IMAGELIST_LAYOUT_TYPE_H,
    IMAGELIST_LAYOUT_TYPE_V
};

#define XML_TEXTRENDER                      _T("text.render")            // ������Ⱦ
#define XML_TEXTRENDER_TYPE                 _T("text.render.type")       // ����������Ⱦ����
#define XML_TEXTRENDER_TYPE_SIMPLE          _T("simple")                // color + font
#define XML_TEXTRENDER_TYPE_COLORLIST       _T("colorlist")             // color list + font
#define XML_TEXTRENDER_TYPE_FONTLIST        _T("fontlist")              // font list + color
#define XML_TEXTRENDER_TYPE_FONTCOLORLIST   _T("fontcolorlist")         // font list + color list
#define XML_TEXTRENDER_TYPE_CONTRAST_COLOR    _T("contrastcolor")       // font + ���汳��ɫ����Ӧ�ĺڰ�color
#define XML_TEXTRENDER_TYPE_CONTRASTCOLORLIST _T("contrastcolorlist")   // 

#define XML_TEXTRENDER_COLOR                _T("text.render.color")      // ������ɫ
#define XML_TEXTRENDER_FONT                 _T("text.render.font")       // ����
#define XML_TEXTRENDER_COLORLIST_COUNT      _T("text.render.colorlist.count")
#define XML_TEXTRENDER_FONTCOLORLIST_COUNT  _T("text.render.fontcolorlist.count")  // ���塢��ɫ����
#define XML_TEXTRENDER_ALIGN                _T("text.render.align")      // ����
#define XML_TEXTRENDER_EFFECT               _T("text.render.effect")     // ��Ч

#define XML_TEXTRENDER_ALIGN_TOP            _T("top")
#define XML_TEXTRENDER_ALIGN_BOTTOM         _T("bottom")
#define XML_TEXTRENDER_ALIGN_LEFT           _T("left")
#define XML_TEXTRENDER_ALIGN_RIGHT          _T("right")
#define XML_TEXTRENDER_ALIGN_CENTER         _T("center")
#define XML_TEXTRENDER_ALIGN_VCENTER        _T("vcenter")
#define XML_TEXTRENDER_ALIGN_END_ELLIPSIS   _T("endellipsis")
#define XML_TEXTRENDER_ALIGN_SINGLELINE     _T("singleline")
#define XML_TEXTRENDER_ALIGN_MULTILINE      _T("multiline")
#define XML_TEXTRENDER_ALIGN_NO_PREFIX      _T("noprefix")

#define XML_TEXTRENDER_EFFECT_NONE          _T("none")
#define XML_TEXTRENDER_EFFECT_HALO          _T("halo")
#define XML_TEXTRENDER_EFFECT_HALO_RADIUS   _T("text.render.halo.radius")
#define XML_TEXTRENDER_EFFECT_HALO_SHADOW_COLOR _T("text.render.halo.shadow.color")
#define XML_TEXTRENDER_EFFECT_ENDALPHAMASK  _T("endalphamask")

// windows attribute
#define  XML_WINDOW_MAX_WIDTH                           _T("maxwidth")
#define  XML_WINDOW_MAX_HEIGHT                          _T("maxheight")
#define  XML_WINDOW_MIN_WIDHT                           _T("minwidth")
#define  XML_WINDOW_MIN_HEIGHT                          _T("minheight")

#define  XML_WINDOW_MAXIMIZE_BORDER                     _T("maximizeborder")   // int���ͣ����ʱλ����Ļ��������С
#define  XML_WINDOW_RESIZE_BORDER                       _T("resizeborder")     // int���ͣ���ק�����С
#define  XML_WINDOW_RESIZE_CAPABILITY                   _T("resizecapability") // ��ק����
#define  XML_WINDOW_RESIZE_CAPABILITY_NONE              _T("none")             // ��ֹ��ק
#define  XML_WINDOW_RESIZE_CAPABILITY_CAPTION           _T("caption")          // ֻ����ק�������޸Ĵ�С
#define  XML_WINDOW_RESIZE_CAPABILITY_ALL               _T("all")              // ����ק�����޸Ĵ�С

#define WRSB_TOP              0x8000     // ��
#define WRSB_LEFT             0x4000     // ��
#define WRSB_BOTTOM           0x2000     // ��
#define WRSB_RIGHT            0x1000     // ��
#define WRSB_TOPLEFT          0x0800     // �I
#define WRSB_TOPRIGHT         0x0400     // �J
#define WRSB_BOTTOMLEFT       0x0200     // �L
#define WRSB_BOTTOMRIGHT      0x0100     // �K
#define WRSB_CAPTION          0x0080     // 
#define WRSB_DRAGSIZE         (WRSB_TOP | WRSB_LEFT | WRSB_BOTTOM | WRSB_RIGHT | WRSB_TOPLEFT | WRSB_TOPRIGHT | WRSB_BOTTOMLEFT | WRSB_BOTTOMRIGHT)
#define WRSB_ALL              (WRSB_CAPTION | WRSB_DRAGSIZE)
#define WRSB_NONE             0x0

// window rgn
#define  XML_WINDOW_TRANSPARENT_TYPE                    _T("transparent.type")
#define  XML_WINDOW_TRANSPARENT_9REGION                 _T("transparent.9region") // ����͸�����������9Region���ͣ���Ϊ4��8��9���֣�

#define  XML_WINDOW_TRANSPARENT_TYPE_AERO               _T("aero")             // ʹ�÷ֲ㴰�ڽ���͸��
#define  XML_WINDOW_TRANSPARENT_TYPE_LAYERED            _T("layered")          // ʹ�÷ֲ㴰�ڽ���͸��
#define  XML_WINDOW_TRANSPARENT_TYPE_MASKCOLOR          _T("maskcolor")        // ʹ����ɫֵ
#define  XML_WINDOW_TRANSPARENT_TYPE_MASKALPHA          _T("maskalpha")        // ʹ��͸��ֵ
#define  XML_WINDOW_TRANSPARENT_TYPE_ANTIALIASING       _T("anti")             // �����Բ��
#define  XML_WINDOW_TRANSPARENT_TYPE_GLOW               _T("glow")             // ��VS2013�߿򷢹�Ч��

#define  XML_WINDOW_TRANSPARENT_MASKCOLOR_VALUE         _T("transparent.maskcolor")  // ����͸��ɫ
#define  XML_WINDOW_TRANSPARENT_MASKALPHA_VALUE         _T("transparent.maskalpha")  // ��������С��alphaֵͨ���Ŀٵ�

#define  XML_WINDOW_TRANSPARENT_ANTI_9REGION            _T("transparent.anti.9region")       // ���Ͻ�Բ�Ƕ������ϲ����������Ͻ�Բ�Ƕ���������������Ҳ����������½�Բ�Ƕ������²����������½�Բ�Ƕ���
#define  XML_WINDOW_TRANSPARENT_ANTI_BORDERINDENT       _T("transparent.anti.borderIndent")  // ��������ֵ��Ĭ��Ϊȫ0�����ڽ��aero blur�л���antiʱ��������һ������Ӱ������
#define  XML_WINDOW_TRANSPARENT_MASKALPHA_9REGION       _T("transparent.maskalpha.9region")  // ȫ0��ʾʲôҲ������ȫ-1��ʾ�������ڹ��ˡ�top,left,right.bottom�ĸ�ֵ�������0��ʾҪ�������ĸ��ߣ�С��0��ʾ�����ˣ������������ĸ��ǵ�λ�ò���
#define  XML_WINDOW_TRANSPARENT_MASKCOLOR_9REGION       _T("transparent.maskcolor.9region")  // ͬXML_WINDOW_TRANSPARENT_MASKALPHA_9REGION

#define  XML_WINDOW_TRANSPARENT_AERO_MODE               _T("transparent.aero.mode")   // aero͸��ģʽ
#define  XML_WINDOW_TRANSPARENT_AERO_MODE_TRANS         _T("trans")
#define  XML_WINDOW_TRANSPARENT_AERO_MODE_BLUR          _T("blur")
enum AERO_MODE 
{
    AERO_MODE_TRANSPARENT,   // ֻ͸��
    AERO_MODE_BLUR,          // ��ģ��
};
#define  XML_WINDOW_TRANSPARENT_AERO_TRANS_MARGINS      _T("transparent.aero.trans.margins")  // rect ͸���������ã�DwmExtendFrameIntoClientArea������
#define  XML_WINDOW_TRANSPARENT_AERO_BLUR_REGION        _T("transparent.aero.blur.region")    // rect ĥɳЧ������DwmEnableBlurBehindWindow������
#define  XML_WINDOW_TRANSPARENT_AERO_BLUR_AUTOCLIPCHILDCTRL  _T("transparent.aero.blur.autoclipchildctrl")  // �Ƿ��Զ������ӿؼ�
#define  XML_WINDOW_TRANSPARENT_TYPE_AERO_DISABLE       _T("transparent.aero.disable")        // aeroģʽ������ʱ�Ĵ���͸������


enum WINDOW_TRANSPARENT_TYPE
{
    WINDOW_TRANSPARENT_TYPE_INVALID = 0,
    WINDOW_TRANSPARENT_TYPE_NORMAL = 1,      
    WINDOW_TRANSPARENT_TYPE_MASKCOLOR = 2,
    WINDOW_TRANSPARENT_TYPE_MASKALPHA = 4,
    WINDOW_TRANSPARENT_TYPE_ANTIALIASING = 8,
    WINDOW_TRANSPARENT_TYPE_LAYERED = 16,   
    WINDOW_TRANSPARENT_TYPE_AERO = 32,     
	WINDOW_TRANSPARENT_TYPE_GLOW = 64,
};

#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY          _T("graphics")         // ͼ����������
#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY_GDI      _T("gdi")
#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY_GDIPLUS  _T("gdiplus")
#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY_D2D      _T("d2d")
#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY_DIRECT2D _T("direct2d")
#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY_D3D      _T("d3d")
#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY_DIRECT3D _T("direct3d")

#define XML_WINDOW_NEED_ALPHACHANNEL                _T("needalphachannel")  // �����Ƿ���Ҫ��Ⱦalphaͨ����Ĭ��Ϊtrue


enum GRAPHICS_RENDER_LIBRARY_TYPE
{
	//	GRAPHICS_RENDER_LIBRARY_TYPE_AUTO = 0,   // <- ���ݴ������;������ֲ㴰��ʹ��GDIPLUS,��ͨ����ʹ��GDI���������߼�̫����
	GRAPHICS_RENDER_LIBRARY_TYPE_GDI = 1,
	GRAPHICS_RENDER_LIBRARY_TYPE_GDIPLUS,
	GRAPHICS_RENDER_LIBRARY_TYPE_DIRECT2D,
	GRAPHICS_RENDER_LIBRARY_TYPE_DIRECT3D
};

#define XML_TEXTURE_RENDER_PREFIX            _T("texture.")         // customwindow����֧�ֻ�����Ƥ��renderǰ׺
#define XML_MASK_RENDER_PREFIX               _T("mask.")            // �ڻ������ӽ����ٻ���һ��(PostPaint)
#define XML_PANEL_RENDER_REGION              _T("render.region")    // Ƥ��ͼƬ�ڴ�������ʾ������������padding������

enum ROUND_CORNER_TYPE
{
    ROUND_CORNER_NONE,
    ROUND_CORNER_ANTI, // �����ģʽ
    ROUND_CORNER_DIRECT_THROW,  // ֱ��ɾ��Բ�Ǵ�����ֵ
};

// ����ͨ������
#define XML_CURSOR                           _T("cursor")       // ����������ʽ
#define XML_CURSOR_IDC_ARROW                 _T("idc_arrow")    // ����ϵͳ�����ʽ����
#define XML_CURSOR_IDC_IBEAM                 _T("idc_ibeam")
#define XML_CURSOR_IDC_WAIT                  _T("idc_wait")
#define XML_CURSOR_IDC_CROSS                 _T("idc_cross")
#define XML_CURSOR_IDC_UPARROW               _T("idc_uparrow")
#define XML_CURSOR_IDC_SIZE                  _T("idc_size")
#define XML_CURSOR_IDC_ICON                  _T("idc_icon")
#define XML_CURSOR_IDC_SIZENWSE              _T("idc_sizenwse")
#define XML_CURSOR_IDC_SIZENESW              _T("idc_sizenesw")
#define XML_CURSOR_IDC_SIZEWE                _T("idc_sizewe")
#define XML_CURSOR_IDC_SIZENS                _T("idc_sizens")
#define XML_CURSOR_IDC_SIZEALL               _T("idc_sizeall")
#define XML_CURSOR_IDC_NO                    _T("idc_no")
#define XML_CURSOR_IDC_HAND                  _T("idc_hand")
#define XML_CURSOR_IDC_APPSTARTING           _T("idc_appstarting")
#define XML_CURSOR_IDC_HELP                  _T("idc_help")

#define XML_CURSOR_IDC_DRAG_SEPERATOR        _T("idc_drag_seperator")
#define XML_CURSOR_IDC_SPLITBAR_H            _T("idc_splitbar_h")
#define XML_CURSOR_IDC_SPLITBAR_V            _T("idc_splitbar_v")


#define XML_TOOLTIP                          _T("tooltip")      // �������ʾ��Ϣ���Զ������ʾ������Ҫ�����������
#define XML_CONTROL_STYLE_GROUP              _T("group")
#define XML_CONTROL_STYLE_TABABLE            _T("tabstop")
#define XML_REJEST_MOUSE_MSG                 _T("rejectmousemsg")  // �����������Ϣ
#define XML_ZINDEX_OVERLAP                   _T("zindexoverlap")   // z���ص��˱�ʶ
#define XML_ZORDER                           _T("zorder")          // �ڱ����е�z�򣬵���Ϊ0ʱ����Ϊ�ö��󴴽�һ��layer
#define XML_ISNCCHILD                        _T("ncchild")         // ��һ��ncchild�����������

#define XML_LAYER                            _T("layer")           // Ϊ�ö�����һ��renderlayer�������棩

// window
// #define  XML_WINDOW_BK_TYPE                    _T("bktype")     // ���ڱ�����������
// #define  XML_WINDOW_BK_TYPE_COLOR              _T("color")      // ���ڱ�����������--��ɫ����ˢ����
// #define  XML_WINDOW_BK_TYPE_TILE               _T("tile")       // ���ڱ�����������--ƽ��
// #define  XML_WINDOW_BK_TYPE_SIMPLE             _T("simple")     // ���ڱ�����������--�򵥣�(0,0)bitblt
// #define  XML_WINDOW_BK_TYPE_CENTER             _T("center")     // ���ڱ�����������--����
// #define  XML_WINDOW_BK_TYPE_STRETCH            _T("stretch")    // ���ڱ�����������--����
// #define  XML_WINDOW_BK_TYPE_FRAME              _T("frame")      // ���ڱ�����������--�߿�
// #define  XML_WINDOW_BK_IMAGE                   _T("bkimage")    // ���ڱ���ͼƬ����
// #define  XML_WINDOW_BK_TYPE_COLOR_PEN          _T("bkpencol")   // bktype=color�����ڱ߿�ɫ
// #define  XML_WINDOW_BK_TYPE_COLOR_BRUSH        _T("bkbrushcol") // bktype=color�����ڱ���ɫ
// #define  XML_WINDOW_BK_TYPE_FRAME_9REGION      _T("bk9region")  // bktype=frame�������иΧ

// ϵͳ��ťID
#define XML_SYS_BTN_RESTORE       _T("sys_restore")   // ϵͳ��ԭ��ťID
#define XML_SYS_BTN_MAXIMIZE      _T("sys_maximize")  // ϵͳ��󻯰�ťID
#define XML_SYS_BTN_MINIMIZE      _T("sys_minimize")  // ϵͳ��С����ťID
#define XML_SYS_BTN_CLOSE         _T("sys_close")     // ϵͳ�رհ�ťID

// hwnd host
#define XML_HWNDHOST_CONTROL_ID   _T("controlid")     // �ؼ�ID��ֵ������

#pragma region // ����layout������
#define XML_LAYOUT_PREFIX        _T("layout.")
#define XML_LAYOUT_TYPE          _T("layout.type")    // ����Ĳ�������
//#define XML_LAYOUT_STACK         _T("stack")          // �������ͣ�stack
#define XML_LAYOUT_VERT          _T("vert")           // �������ͣ�vert
#define XML_LAYOUT_HORZ          _T("horz")           // �������ͣ�horz
#define XML_LAYOUT_NULL          _T("null")           // �������ͣ�null
#define XML_LAYOUT_GRID          _T("grid")           // �������ͣ�grid
#define XML_LAYOUT_DOCK          _T("dock")           // �������ͣ�dock
#define XML_LAYOUT_CANVAS        _T("canvas")         // �������ͣ�canvas
#define XML_LAYOUT_CARD          _T("card")           // �������ͣ�card
#define XML_LAYOUT_AVERAGE       _T("average")        // �������ͣ�average

enum LAYOUTTYPE
{
    LAYOUT_TYPE_CANVAS = 0,
    //LAYOUT_TYPE_STACK,
	LAYOUT_TYPE_HORZ,
	LAYOUT_TYPE_VERT,
    LAYOUT_TYPE_GRID,
    LAYOUT_TYPE_DOCK,
    LAYOUT_TYPE_CARD,
    LAYOUT_TYPE_AVERAGE,
};

// stack���ֵ�����
// #define XML_LAYOUT_STACK_DIRECTION      _T("layout.direction")  // stack���ֵķ���
// #define XML_LAYOUT_STACK_LEFTTORIGHT    _T("lefttoright")       // xml�п����õ�ֵ
// #define XML_LAYOUT_STACK_RIGHTTOLEFT    _T("righttoleft")       // xml�п����õ�ֵ
// #define XML_LAYOUT_STACK_LEFTANDRIGHT   _T("leftandright")      // xml�п����õ�ֵ
// #define XML_LAYOUT_STACK_TOPTOBOTTOM    _T("toptobottom")       // xml�п����õ�ֵ
// #define XML_LAYOUT_STACK_BOTTOMTOTOP    _T("bottomtotop")       // xml�п����õ�ֵ
// #define XML_LAYOUT_STACK_TOPANDBOTTOM   _T("topandbottom")      // xml�п����õ�ֵ

// enum LAYOUT_STACK_DIRECTION
// {
//     LAYOUT_STACK_LEFTTORIGHT  = 0,   
//     LAYOUT_STACK_RIGHTTOLEFT  = 1,   
//     LAYOUT_STACK_LEFTANDRIGHT = 2,  
//     LAYOUT_STACK_TOPTOBOTTOM  = 3,   
//     LAYOUT_STACK_BOTTOMTOTOP  = 4,   
//     LAYOUT_STACK_TOPANDBOTTOM = 5   
// };
#define XML_LAYOUT_STACK_GAP            _T("layout.gap")        // ÿһ��֮��ļ�϶

// average����
#define XML_LAYOUT_AVERAGE_DIRECTION    _T("layout.direction")  // stack���ֵķ���
#define XML_LAYOUT_AVERAGE_H            _T("h")
#define XML_LAYOUT_AVERAGE_V            _T("v")

enum LAYOUT_AVERAGE_DIRECTION
{
    LAYOUT_AVERAGE_H,
    LAYOUT_AVERAGE_V
};

#define XML_LAYOUT_AVERAGE_GAP         _T("layout.gap")        // ÿһ��֮��ļ�϶

// dock���ֵ�����

// grid���ֵ�����
#define XML_LAYOUT_GRID_WIDTH           _T("layout.width")      // grid���ֵĸ��еĿ�ȶ���
#define XML_LAYOUT_GRID_HEIGHT          _T("layout.height")     // grid���ֵĸ��еĸ߶ȶ���

#pragma endregion

#pragma region // ������Ԫ������

enum LAYOUT_FLAG
{
    LAYOUT_ITEM_ALIGN_LEFT   = 0x0001,
    LAYOUT_ITEM_ALIGN_TOP    = 0x0002,
    LAYOUT_ITEM_ALIGN_RIGHT  = 0x0004,
    LAYOUT_ITEM_ALIGN_BOTTOM = 0x0010,
    LAYOUT_ITEM_ALIGN_CENTER = 0x0020,
    LAYOUT_ITEM_ALIGN_VCENTER = 0x0040,
    LAYOUT_ITEM_ALIGN_FILL   = 0x0080,  // ���ʣ��ռ�--stack����
	// LAYOUT_FLAG_FORCE_DESIREDSIZE = 0x0100, // ǿ��ʹ�ÿؼ��Լ���������С���������ٵ�һ��get desired size����

    LAYOUT_ITEM_ALIGN_LTRB = LAYOUT_ITEM_ALIGN_LEFT|LAYOUT_ITEM_ALIGN_TOP|LAYOUT_ITEM_ALIGN_RIGHT|LAYOUT_ITEM_ALIGN_BOTTOM,
};


// canvas�����е���Ԫ�ص�����
#define XML_LAYOUT_ITEM_LEFT        _T("layout.left")
#define XML_LAYOUT_ITEM_TOP         _T("layout.top")
#define XML_LAYOUT_ITEM_RIGHT       _T("layout.right")
#define XML_LAYOUT_ITEM_BOTTOM      _T("layout.bottom")

// stack�����е���Ԫ�ص�����
#define XML_LAYOUT_ITEM_ALIGN          _T("layout.align")       // ��layout.direction=leftandright||topandbottomʱ����Ҫָ����Ԫ�ص���ͣ������һ��
#define XML_LAYOUT_ITEM_ALIGN_LEFT     _T("left")
#define XML_LAYOUT_ITEM_ALIGN_RIGHT    _T("right")
#define XML_LAYOUT_ITEM_ALIGN_TOP      _T("top")
#define XML_LAYOUT_ITEM_ALIGN_BOTTOM   _T("bottom")

#define XML_LAYOUT_ITEM_ALIGN_CENTER   _T("center")  // -- stack/dock layout
#define XML_LAYOUT_ITEM_ALIGN_VCENTER  _T("vcenter") // -- stack layout
#define XML_LAYOUT_ITEM_ALIGN_FILL     _T("fill")    // -- stack layout

// �ӿؼ��ı�ȫ���벼�֣���������Measure�Լ�ʱ����δ�����ӿؼ���desiredsize
// 1. ��������£������ӿؼ����õ���lrtb���룬���������Լ��Ĵ�С������ֱ�������ڸ�����
// 2. �ڲ˵����ڳ����£����ڵĴ�С����ȡ���ڲ˵����ݴ�С�����˵�����lrtb���벼�ֵģ�
//    ��˴����ڱ���ʱ���˵���desiredsize��Ӧ�����������󣬶��Ƿ�����ʵ��С
// �������ٵ�һ��get desired size����
//#define XML_LAYOUT_ALIGN_FORCE_DESIREDSIZE  _T("xxxx")


// enum LAYOUT_STACK_ITEM_ALIGN
// {
// 	LAYOUT_STACK_ITEM_ALIGN_LEFT = 0,
// 	LAYOUT_STACK_ITEM_ALIGN_TOP = 0,
// 	LAYOUT_STACK_ITEM_ALIGN_RIGHT = 1,
// 	LAYOUT_STACK_ITEM_ALIGN_BOTTOM = 1,
// };

// dock�����е���Ԫ�ص�����
// #define XML_LAYOUT_DOCK_DOCK          _T("layout.dock")       // ��Ԫ��ͣ������һ������
// #define XML_LAYOUT_DOCK_DOCK_LEFT     _T("left")
// #define XML_LAYOUT_DOCK_DOCK_RIGHT    _T("right")
// #define XML_LAYOUT_DOCK_DOCK_TOP      _T("top")
// #define XML_LAYOUT_DOCK_DOCK_BOTTOM   _T("bottom")
// #define XML_LAYOUT_DOCK_DOCK_CENTER   _T("center")

// grid�����е���Ԫ�ص�����
#define XML_LAYOUT_GRID_ROW          _T("layout.row")        // ��Ӧ m_nConfigTop
#define XML_LAYOUT_GRID_COL          _T("layout.col")        // ��Ӧ m_nConfigLeft
#define XML_LAYOUT_GRID_ROWSPAN      _T("layout.rowspan")    // ��Ӧ m_nConfigBottom
#define XML_LAYOUT_GRID_COLSPAN      _T("layout.colspan")    // ��Ӧ m_nConfigRight

#pragma endregion
//
// style.xml ===>
//
enum STYLE_SELECTOR_TYPE
{
    STYLE_SELECTOR_TYPE_ID,
    STYLE_SELECTOR_TYPE_CLASS,
    STYLE_SELECTOR_TYPE_TAG
};
#define  XML_STYLE_SELECTOR_TYPE_ID            _T("id")       // id ѡ����
#define  XML_STYLE_SELECTOR_TYPE_CLASS         _T("class")    // classѡ����
#define  XML_STYLE_INHERIT                     _T("inherit")  // �̳�����

#define  STYLE_ID_PREFIX                       _T('#')        // idѡ����ǰ׺
#define  STYLE_CLASS_PREFIX                    _T('.')        // classѡ����ǰ׺

// Control

// Label

// Picture

// GifPicture
#define  XML_GIFCTRL_GIF   _T("gif")       // gif��Դid
#define  XML_GIFCTRL_AUTOPLAY    _T("autoplay")   // �Ƿ��Զ���ʼ��Ĭ��Ϊfalse����Ҫ�ֶ�����start����

// GroupBox
#define XML_GROUPBOX_TEXTINDENT   _T("textindent")  // ��������ֵ
#define XML_GROUPBOX_TEXTMARGIN   _T("textmargin")  // ������߿�ļ��
#define XML_GROUPBOX_TEXTALIGN    _T("textalign")   // ���ֶ��뷽ʽ

//////////////////////////////////////////////////////////////////////////
// Button

#define  XML_BUTTON_DEFPUSHBUTTON       _T("defpushbutton")    // �Ƿ���Ĭ�ϰ�ť
#define  XML_BUTTON_DRAW_FOCUS_TYPE     _T("drawfocus")        // ��λ��ƽ���
#define  XML_BUTTON_DRAW_FOCUS_TYPE_NONE   _T("none")        
#define  XML_BUTTON_DRAW_FOCUS_TYPE_DOT    _T("dot")    
#define  XML_BUTTON_HAND_CUROSR         _T("handcursor")       // ��ʾ���������ʽ

typedef enum
{
    BUTTON_RENDER_DRAW_FOCUS_TYPE_NONE,                               // ������
    BUTTON_RENDER_DRAW_FOCUS_TYPE_DOT,                                // ʹ�����ߵ����
}BUTTON_RENDER_DRAW_FOCUS_TYPE;

#define XML_BUTTON_AUTOSIZE_TYPE           _T("autosize")     // ��û��ָ����ť��Сʱ����μ������С
#define XML_BUTTON_AUTOSIZE_TYPE_BKIMAGE   _T("backimage")      // ����ͼƬ�Ĵ�С������ť��С
#define XML_BUTTON_AUTOSIZE_TYPE_CONTENT   _T("content")      // ����ͼƬ������(ͼ��+�ı�)������ť��С
#define XML_BUTTON_AUTOSIZE_TYPE_BKIMAGEHEIGHT_CONTENTWIDTH _T("bkimagehcontentw")  // ͼƬ�ĸ߶�+���ݵĿ��

enum BUTTON_AUTO_SIZE_TYPE
{
    BUTTON_AUTOSIZE_TYPE_NDEF,
    BUTTON_AUTOSIZE_TYPE_BKIMAGE,
    BUTTON_AUTOSIZE_TYPE_CONTENT,
    BUTTON_AUTOSIZE_TYPE_BKIMAGEHEIGHT_CONTENTWIDTH
};

#define XML_BUTTON_ICON_ALIGN  _T("iconalign")    // ��ťͼ���λ��

// #define  XML_BUTTON_ICON                             _T("icon")             // ��ťͼ��
// #define  XML_BUTTON_ICON_RENDER_TYPE                 _T("icontype")         // ��ťͼ���������
// #define  XML_BUTTON_ICON_RENDER_TYPE_STATIC          _T("static")           // ��̬ͼ��
// #define  XML_BUTTON_ICON_RENDER_TYPE_8STATE          _T("8state")           // 8̬ͼ��
// #define  XML_BUTTON_ICON_RENDER_TYPE_THEME_RADIO     _T("themeradio")       // ���ⵥѡͼ��
// #define  XML_BUTTON_ICON_RENDER_TYPE_THEME_CHECK     _T("themecheck")       // ���⸴ѡͼ��
// typedef enum
// {
// 	BUTTON_ICON_RENDER_TYPE_NONE,        // ������ͼ��
// 	BUTTON_ICON_RENDER_TYPE_STATIC,
// 	BUTTON_ICON_RENDER_TYPE_GIF,
// 	BUTTON_ICON_RENDER_TYPE_4STATE,
// 	BUTTON_ICON_RENDER_TYPE_8STATE,
// 	BUTTON_ICON_RENDER_TYPE_THEME_RADIO,
// 	BUTTON_ICON_RENDER_TYPE_THEME_CHECK
// }BUTTON_ICON_RENDER_TYPE;

#define   XML_BUTTON_ICON_TEXT_SPACE           _T("icontextspace")   // �ı���ͼ��֮��ļ��
// #define  XML_BUTTON_TEXT_RENDER_TYPE           _T("texttype")         // �ı���������
// #define  XML_BUTTON_TEXT_RENDER_TYPE_SINGLELINE _T("singleline")      // �����ı�
// #define  XML_BUTTON_TEXT_RENDER_TYPE_HYPERLINK  _T("hyperlink")       // ������ģʽ
// typedef enum
// {
// 	BUTTON_TEXT_RENDER_TYPE_NONE,         // �������ı�
// 	BUTTON_TEXT_RENDER_TYPE_SINGLELINE,   // �����ı�
// 	BUTTON_TEXT_RENDER_TYPE_HYPERLINK,    // ������
// 
// }BUTTON_TEXT_RENDER_TYPE;


// HyperLink
#define  XML_HYPERLINK_HREF              _T("href")     // ������


// #define  XML_BRUSHBUTTON_BKHOVERCOL            _T("bkhovercol")
// #define  XML_BRUSHBUTTON_BKPRESSCOL            _T("bkpresscol")
// #define  XML_BRUSHBUTTON_BORDERHOVERCOL        _T("borderhovercol")
// #define  XML_BRUSHBUTTON_BORDERPRESSCOL        _T("borderpresscol")
// #define  XML_BRUSHBUTTON_TEXTNORMALCOL         _T("textnormalcol")
// #define  XML_BRUSHBUTTON_TEXTHOVERCOL          _T("texthovercol")
// #define  XML_BRUSHBUTTON_TEXTPRESSCOL          _T("textpresscol")
// #define  XML_BRUSHBUTTON_TEXTDISABLECOL        _T("textdisablecol")
// #define  XML_BRUSHBUTTON_TEXT                  _T("text")
// #define  XML_BRUSHBUTTON_ICON                  _T("icon")

//#define  XML_BUTTON_BK_TYPE                    _T("bktype")     // ��ť������������
//#define  XML_BUTTON_BK_TYPE_COLOR              _T("color")    

//////////////////////////////////////////////////////////////////////////
// ProgressCtrl / SliderCtrl

#define  XML_PROGRESSBAR_DIRECTION            _T("direction")       // �������ķ���
#define  XML_PROGRESSBAR_DIRECTION_LEFTRIGHT  _T("lefttoright")       // ��->��
#define  XML_PROGRESSBAR_DIRECTION_BOTTOMTOP  _T("bottomtotop")       // ��->��
#define  XML_PROGRESSBAR_DIRECTION_RIGHTLEFT  _T("righttoleft")       // ��->��
#define  XML_PROGRESSBAR_DIRECTION_TOPBOTTOM  _T("toptobottom")       // ��->��

enum PROGRESSBAR_SCROLL_DIRECTION_TYPE
{
    PROGRESSBAR_SCROLL_LEFT_2_RIGHT,
    PROGRESSBAR_SCROLL_BOTTOM_2_TOP,
    PROGRESSBAR_SCROLL_RIGHT_2_LEFT,
    PROGRESSBAR_SCROLL_TOP_2_BOTTOM
};

#define  XML_SLIDERCTRL_THUMB_POINT_DIRECTION          _T("thumbarrow")  // ���鰴ť��ָ��
#define  XML_SLIDERCTRL_THUMB_POINT_DIRECTION_BOTH     _T("both")            // Ĭ��ֵ
#define  XML_SLIDERCTRL_THUMB_POINT_DIRECTION_LEFT     _T("left")    
#define  XML_SLIDERCTRL_THUMB_POINT_DIRECTION_TOP      _T("top")     
#define  XML_SLIDERCTRL_THUMB_POINT_DIRECTION_RIGHT    _T("right")   
#define  XML_SLIDERCTRL_THUMB_POINT_DIRECTION_BOTTOM   _T("bottom")   


// #define  PROGRESSCTRL_FLAG_DIRECTION_LEFTRIGHT    0x0000
// #define  PROGRESSCTRL_FLAG_DIRECTION_TOPBOTTOM    0x0001                
// #define  PROGRESSCTRL_FLAG_DIRECTION_RIGHTLEFT    0x0002
// #define  PROGRESSCTRL_FLAG_DIRECTION_BOTTOMTOP    0x0003

#define  XML_SLIDERCTRL_BUTTON_ATTR_PRIFIX     _T("trackbtn.")       // �����ؼ��İ�ť����ǰ׺

#define  XML_IMAGEPROGRESSCTRL_FOREIMAGE       _T("foreimage")       // ǰ��ͼƬ��һ�ţ�
#define  XML_IMAGEPROGRESSCTRL_BKGNDIMAGE      _T("bkgndimage")      // ����ͼƬ��һ�ţ�

// Edit
#define  XML_EDIT_TEXT                        _T("text")             // ��������
#define  XML_EDIT_TEXT_ALIGN                  _T("align")            // ���ֶ��뷽ʽ

#define  XML_EDIT_COLOR                       _T("color.text")       // ����������ʾ��ɫ
#define  XML_EDIT_SELECT_COLOR                _T("color.select")     // ���ֱ�ѡ��ʱ����ɫ
#define  XML_EDIT_SELECT_BK_COLOR             _T("color.select.back")// ���ֱ�ѡ��ʱ��������ɫ
#define  XML_EDIT_DISABLE_COLOR               _T("color.disable")    // ������ʱ���ı���ɫ
#define  XML_EDIT_READONLY_COLOR              _T("color.readonly")   // ֻ��ʱ���ı���ɫ
#define  XML_INSTANTEDIT_COLOR                _T("color.quiet")      // ��ʱ�༭ʱ��δ�����༭ʱ���ı���ɫ 
#define  XML_EDIT_MAX_LENGTH                  _T("limit")            // ���ݵ��ֵ
#define  XML_EDIT_CARET_HEIGHT                _T("caretheight")      // ��꼰ѡ���ı������ĸ߶�
//#define  XML_EDIT_MAX_INT

// Scroll Bar
#define  XML_SCROLLBAR_RANGE                  _T("range")            // ��������Χ����
#define  XML_SCROLLBAR_PAGE                   _T("page")             // ������ҳ��С����
#define  XML_SCROLLBAR_POS                    _T("pos")              // ������λ������
#define  XML_SCROLLBAR_BTNLINE                _T("btnline")          // �����������ť�������С����
#define  XML_SCROLLBAR_WHEELLINE              _T("wheelline")        // ʹ����껬�ֺ������С����

#define  XML_HSCROLLBAR                           _T("hscrollbar")  // �������������
#define  XML_VSCROLLBAR                           _T("vscrollbar")  // �������������
#define  XML_SCROLLBAR_NONE                       _T("none")        // �÷����޹�����
#define  XML_SCROLLBAR_AUTO                       _T("auto")        // �÷����ϵĹ���������Ҫʱ��ʾ
#define  XML_SCROLLBAR_ALWAYSSHOW                 _T("show")        // �÷����ϵĹ�����������ʾ
#define  XML_SCROLLBAR_ALWAYSHIDE                 _T("hide")        // �÷�����֧�ֹ�����������ʾ������

enum SCROLLBAR_VISIBLE_TYPE
{
    SCROLLBAR_VISIBLE_NONE,          // ��ʹ��
    SCROLLBAR_VISIBLE_AUTO,          // ����Ҫ������ʱ������ʾ
    SCROLLBAR_VISIBLE_SHOW_ALWAYS,   // һֱ��ʾ
    SCROLLBAR_VISIBLE_HIDE_ALWAYS,   // һֱ���أ��߼��ϵ�
};
enum SCROLLBAR_DIRECTION_TYPE
{
    HSCROLLBAR = WM_HSCROLL,
    VSCROLLBAR = WM_VSCROLL
};

// #define  XML_SCROLLBAR_TYPE                       _T("type")        // ǰ��XML_xxSCROLLBAR_PRIFIX��������������
// #define  XML_SCROLLBAR_TYPE_SYSTEM                _T("system")      // ϵͳ��ʽ�Ĺ�������������ť+һ���϶���
// #define  XML_SCROLLBAR_TYPE_LOGIC                 _T("logic")       // �޹�������ʾ����ά���󶨶�����й���������
// #define  XML_SCROLLBAR_TYPE_MENU                  _T("menu")        // 
// 
// enum SCROLLBAR_RENDER_TYPE
// {
//     SCROLLBAR_RENDER_TYPE_SYSTEM,
//     SCROLLBAR_RENDER_TYPE_LOGIC,
//     SCROLLBAR_RENDER_TYPE_MENU,
// };

#define  XML_HSCROLLBAR_PRIFIX                    _T("hscrollbar.") // ���������������ǰ׺
#define  XML_VSCROLLBAR_PRIFIX                    _T("vscrollbar.") // ���������������ǰ׺

#define  XML_SCROLLBAR_RENDER_TYPE                _T("type")        // ���ù���������Ⱦ��

// scrollpanel
#define  XML_SCROLLPANEL_VIEWSIZE                 _T("viewsize")    // ��ͼ��С����ʽΪ "100,100"

// listbox
#define  XML_LISTCTRL_ITEM_HEIGHT                 _T("itemheight")  // �б�ؼ�ÿһ��ĸ߶�
#define  XML_LISTCTRL_ITEM_PADDING                _T("itempadding") // �б��������

#define  XML_LISTCTRL_MIN_WIDTH                   _T("minwidth")    //  auto size�б�����С����ߴ�
#define  XML_LISTCTRL_MIN_HEIGHT                  _T("minheight")
#define  XML_LISTCTRL_MAX_WIDTH                   _T("maxwidth")
#define  XML_LISTCTRL_MAX_HEIGHT                  _T("maxheight")

#define  XML_LISTCTRL_EDIT_PRIFIX                 _T("edit.")       // �༭�������ǰ׺
#define  XML_LISTCTRL_BUTTON_PRIFIX               _T("button.")     // ��ť������ǰ׺
#define  XML_LISTCTRL_COMBOBOX_PRIFIX             _T("combobox.")   // �����б������ǰ׺

// combobox
#define  XML_COMBOBOX_EDIT_PRIFIX                 _T("edit.")       // �༭�������ǰ׺
#define  XML_COMBOBOX_BUTTON_PRIFIX               _T("button.")     // ��ť������ǰ׺
#define  XML_COMBOBOX_LISTBOX_PRIFIX              _T("listbox.")    // �����б������ǰ׺
#define  XML_COMBOBOX_DROPWNDID                   _T("dropwnd.id")  // ��������id

// ledctrl
#define  XML_LEDCTRL_INDEXMAP                     _T("indexmap")    // ͼƬ����ӳ���ϵ
#define  XML_LEDCTRL_ITEMWIDTH                    _T("itemwidth")   // ����ÿһ�����ʾ��ȣ�Ĭ��ΪͼƬ��ȣ�
#define  XML_LEDCTRL_ITEMHEIGHT                   _T("itemheight")  // ����ÿһ�����ʾ�߶ȣ�Ĭ��ΪͼƬ�߶ȣ�
#define  XML_LEDEXCTRL_SAMEWIDTH                  _T("samewidth")   // bool�������С��ͬ�Ļ����Ͳ�������posmap������
#define  XML_LEDEXCTRL_POSMAP                     _T("posmap")      // ͼƬ��λ��ӳ���ϵ  0,10;10,16;16,6;22,6;28,6;34,6;40,6;46,6;52,6;58,6;64,6 ...

// menu
#define  XML_MENU                 _T("Menu")      // layout.xml�в˵�����tag name
#define  XML_MENU_STRINGITEM      _T("String")    //
#define  XML_MENU_POPUPITEM       _T("Popup")     //
#define  XML_MENU_SEPARATORITEM   _T("Separator") // 
#define  XML_MENU_DISABLE         _T("disable")   // menu item�Ƿ���ã�Ϊ1����true��ʾ������
#define  XML_MENU_POPUP_WNDID     _T("wnd.id")    // popup�Ӳ˵�Ƥ����Դ����id
#define  XML_MENU_POPUP_MENUID    _T("menu.id")   // popup�Ӳ˵��ڴ����еĿؼ�id

#define  XML_MENU_ICONGUTTERWIDTH            _T("icongutterwidth")  // �˵�ͼ���еĿ��
#define  XML_MENU_POPUPTRIANGLEWIDTH         _T("trianglewidth")    // �Ҳ������ε�ռ�ÿ��
#define  XML_MENU_SEPARATORHEIGHT            _T("separatorheight")  // �ָ����ĸ߶�
#define  XML_MENU_LEFT_TEXT_INDENT           _T("textleftindent")   // ����ı�����
#define  XML_MENU_RIGHT_TEXT_INDENT          _T("textrightindent")  // ����ı�����
#define  XML_MENU_SUBMENU_GAP                _T("submenugap")       // �Ӳ˵��븸�˵�֮��ļ�϶��Ĭ��Ϊ-2

#define  XML_MENU_RADIO_ICON_RENDER_PREFIX   _T("radioicon.")       // ѡ��ͼ�����renderǰ׺
#define  XML_MENU_CHECK_ICON_RENDER_PREFIX   _T("checkicon.")       // ѡ��ͼ�����renderǰ׺
#define  XML_MENU_RADIOCHECK_ICONBK_RENDER_PREFIX   _T("radiocheckiconbk.") // ѡ��ͼ�걳������renderǰ׺
#define  XML_MENU_SEPARATOR_RENDEER_PREFIX   _T("separator.")       // �ָ�������renderǰ׺
#define  XML_MENU_TRAIANGLE_RENDER_PREFIX    _T("triangle.")        // ������ͷ����renderǰ׺

#define  XML_MENU_ITEM_ICON_RENDER_PREFIX    _T("icon.")            // �˵����ͼ��

#define  MENU_SEPARATOR_ID    -1
#define  MENU_POPUP_ID        0

// header ctrl
#define  XML_HEADERCTRL_RIGHT_BLANK_RENDER_PRIFIX  _T("rightblank.") // �Ҳ�հ�����ı�������ǰԵ


// listview ctrl
#define  XML_LISTVIEWCTRL_HEADERCTRL_PREFIX   _T("headerctrl.")  // �б�ؼ��е�header����ǰ׺

// Flash Wrap
#define  XML_FLASH_URI                        _T("uri")          // flash�ļ���ַ
#define  XML_FLASH_WMODE                      _T("wmode")        // flash����ģʽ
#define  XML_FLASH_WMODE_WINDOW               _T("window")
#define  XML_FLASH_WMODE_OPAQUE               _T("opaque")
#define  XML_FLASH_WMODE_TRANSPARENT          _T("transparent")
enum FLASH_WMODE
{
    FLASH_WMODE_WINDOW,
    FLASH_WMODE_OPAQUE,
    FLASH_WMODE_TRANSPARENT,
};

// Layer -- ����
// #define  XML_LAYER_TYPE             _T("type")    // render chain�е�layer����
// #define  XML_LAYER_TYPE_DIRECT      _T("direct")  // ֱ����Ⱦ�����軺�档renderbase����
// #define  XML_LAYER_TYPE_CONTROL     _T("control") // ��Ƕһ���ؼ�������flash

//#define  XML_LAYER_TYPE_FLASH       _T("flash")   // flash��


// TabCtrl
#define   XML_TABCTRL_HEAD_HEIGHT               _T("head.height")  // ͷ���ĸ߶�
#define   XML_TABCTRL_HEAD_LAYOUT_TYPE          _T("head.layout")  // ͷ�����ַ�ʽ
#define   XML_TABCTRL_HEAD_LAYOUT_TYPE_Win32    _T("win32")
#define   XML_TABCTRL_HEAD_LAYOUT_TYPE_AVERAGE  _T("average")
#define   XML_TABCTRL_HEAD_LAYOUT_TYPE_FIX      _T("fix")
#define   XML_TABCTRL_CONTENTPANEL_PREFIX       _T("contentpanel.")

enum TABCTRL_HEAD_LAYOUT_TYPE
{
    TABCTRL_HEAD_LAYOUT_TYPE_FIX = 0,     // �̶���С���������
    TABCTRL_HEAD_LAYOUT_TYPE_AVERAGE = 1, // ����TABCTRL�Ĵ�Сƽ������ÿ����ť�Ŀ��
    TABCTRL_HEAD_LAYOUT_TYPE_Win32 = 2,   // ����ϵͳ����
};

// TreeView
#define   XML_EXPANDICON_RENDER_TYPE_PREFIX   _T("expandicon.")   // չ��/����ͼ��renderǰ׺


}
#endif XMLDEFINE_H_CBBD5468_6BF1_45ac_B02C_F10AC19A651A