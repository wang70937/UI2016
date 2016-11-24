#ifndef XMLDEFINE_H_CBBD5468_6BF1_45ac_B02C_F10AC19A651A
#define XMLDEFINE_H_CBBD5468_6BF1_45ac_B02C_F10AC19A651A

namespace UI
{

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//                                XML中的tag定义                               //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#define XML_SEPARATOR             _T(',')            // xml中的分隔符，如表示颜色、区域
#define XML_PATH_SEPARATOR        _T('/')            // xml中的路径分隔符（如样式路径，子对象路径）
#define XML_MULTI_SEPARATOR       _T(';')            // xml中的分隔符,如分隔一个控件的多个style.
#define XML_FLAG_SEPARATOR        _T('|')            // xml中的分隔符，如文本对齐方式: left|top|vcenter
#define XML_CHILD_SEPARATOR       _T('.')            // xml中父子之间的分隔符，如Menu.String
#define XML_KEYVALUE_SEPARATOR    _T(':')            // xml中key:value之间的分隔条
#define XML_HEADER                _T("<?xml version=\"1.0\" encoding=\"utf-8\" ?>")


#define XML_BOOL_VALUE_YES        _T("yes")
#define XML_BOOL_VALUE_NO         _T("no")
#define XML_BOOL_VALUE_1          _T("1")
#define XML_BOOL_VALUE_0          _T("0")
#define XML_BOOL_VALUE_TRUE       _T("true")
#define XML_BOOL_VALUE_FALSE      _T("false")

// #define XML_UI_XMLFILE_EXT   _T(".ui")          // 工程描述文件的后缀名
#define XML_SKIN_XML              _T("skin.xml")
#define XML_SKIN_PACKET_EXT       _T("skn")        // 皮肤包后缀名

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
#define XML_PROJECT_EXT      _T("uiproj")          // 工程文件后缀名
#define XML_PROJECT_EXT2     _T(".uiproj")
#define XML_LOG_UI_ID        _T("ui")             // ui日志id
#define XML_LAYOUTCONFIG     _T("layoutconfig")   // 延迟加载皮肤列表
#define XML_PROP             _T("prop")           // 复杂属性，单独配置在一行
#define XML_KEY              _T("key")            // <prop> </prop>下面的key

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
#define XML_FONT_FACENAME2   _T("face2")  // 当facename字体不存在时，尝试facename2
#define XML_FONT_HEIGHT      _T("size")
#define XML_FONT_ORIENTATION _T("orientation") 
#define XML_FONT_BOLD        _T("bold")
#define XML_FONT_ITALIC      _T("italic")
#define XML_FONT_UNDERLINE   _T("underline")
#define XML_FONT_STRIKEOUT   _T("strikeout")  
#define XML_FONT_CLEARTYPE   _T("cleartype")   // 是否使用cleartype样式

#define XML_IMAGE_THEMEHLS            _T("themehls")    // 改变皮肤色调时，该图片是否参与 0/1，默认为0
#define XML_IMAGE_NEED_ANTIALIASING   _T("antialiasing")  // 图片是否需要支持抗锯齿（缩放时）
#define XML_IMAGE_DPI_ADAPT           _T("dpiadapt")    // 是否DPI自适应缩放

#define XML_IMAGE_ITEM_TYPE           _T("type")          // image类型
#define XML_IMAGE_ITEM_TYPE_IMAGE     _T("image")         // 默认值
#define XML_IMAGE_ITEM_TYPE_ICON      _T("icon")          // 图标 
#define XML_IMAGE_ITEM_TYPE_IMAGELIST _T("imagelist")     // 图片列表
#define XML_IMAGE_ITEM_TYPE_GIF       _T("gif")           // gif图片
#define XML_IMAGE_ITEM_TYPE_PNGLISTGIF _T("pnglistgif")   // png列表形式的gif图片
enum IMAGE_ITEM_TYPE
{
    IMAGE_ITEM_TYPE_IMAGE,
    IMAGE_ITEM_TYPE_ICON,
    IMAGE_ITEM_TYPE_IMAGE_LIST,
    IMAGE_ITEM_TYPE_GIF,
    IMAGE_ITEM_TYPE_PNGLISTGIF,
    IMAGE_ITEM_TYPE_CURSOR,
};

#define XML_IMAGE_ICON_WIDTH          _T("width")         // 要加载的图标宽度
#define XML_IMAGE_ICON_HEIGHT         _T("height")        // 要加载的图标高度
#define XML_IMAGE_IMAGELIST_COUNT     _T("count")         // 图片列表项的数目 
#define XML_IMAGE_IMAGELIST_LAYOUT    _T("layout")        // 图片列表项的排布方式
#define XML_IMAGE_IMAGELIST_LAYOUT_H  _T("h")  
#define XML_IMAGE_IMAGELIST_LAYOUT_V  _T("v")

#define XML_COLOR_USESKINHLS          _T("useskinhls")    // 改变皮肤色调时，该颜色是否参与 0/1，默认为1

// 工程配置
#define XML_PROJECT_NAME       _T("projectname")     // 工程名称
#define XML_PROJECT_SELECT     _T("select")          // 当前工程使用了哪款皮肤名称
#define XML_PROJECT_SKIN       _T("skin")            // 工程中一款皮肤
#define XML_PROJECT_SKIN_NAME  _T("name")            // 皮肤名称
#define XML_PROJECT_SKIN_PATH  _T("path")            // 皮肤ui.xml所在路径
#define XML_PROJECT_SKIN_HLS   _T("hls")             // 该皮肤的色调信息,如"10,0,0"

#define XML_SKINBUILDER_CTRLSDLL _T("ctrlsdll")      // 外部控件所在的DLL，用于注册创建信息

//
// layout.xml
//

// Object对象属性

//#define XML_ID                   _T("id")            // 每个对象的标识符
#define XML_STYLECLASS           _T("style")         // 对象的样式类型
#define XML_WIDTH                _T("width")         // 对象的宽度，可以为 auto | 数值
#define XML_HEIGHT               _T("height")        // 对象的高度，可以为 auto | 数值
#define XML_MARGIN               _T("margin")        // 对象的外边距
#define XML_PADDING              _T("padding")       // 对象的内边距
#define XML_BORDER               _T("border")        // 对象的边框
#define XML_MINWIDTH             _T("minwidth")      // 对象的最小宽度
#define XML_MINHEIGHT            _T("minheigth")     // 对象的最小高度
#define XML_MAXWIDTH             _T("maxwidth")      // 对象的最大宽度
#define XML_MAXHEIGHT            _T("maxheight")     // 对象的最大高度
#define XML_AUTO                 _T("auto")          // 对象的宽和高设置为由内部内容决定
#define XML_ASTERISK             _T("*")             // 对象的宽和高设置为平分
#define XML_AVERAGE              _T("avg")           // 对象的宽和高设置为平均
#define XML_PERCENT              _T("percent")       // 对象的宽和高设置为百分比 pct ?
#define XML_PERCENT_CHAR         _T('%') 
#define XML_AVERAGE_CHAR         _T('*')             // 暂不支持 x*,

// EraseBkgnd
#define XML_BACKGND_IS_TRANSPARENT        _T("transparent")             // 是否需要进行透明处理（刷上父对象的背景）0/1
#define XML_NO_CLIP_DRAW                  _T("noclip")                  // 在绘制自己时，不需要维护剪裁区域，默认剪裁

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
#define XML_RENDER_TYPE                   _T("render.type")             // 设置渲染类型
#define XML_RENDER_TYPE_GRADIENT          _T("gradient")                // 横向渐变 Color + Color
#define XML_RENDER_TYPE_IMAGE             _T("image")                   // 贴图Image + Color
#define XML_RENDER_TYPE_IMAGELISTITEM     _T("imagelistitem")           // 图片列表中的一项，需要指定其索引
#define XML_RENDER_TYPE_IMAGELIST         _T("imagelist")               // 图片列表
#define XML_RENDER_TYPE_COLOR             _T("color")                   // 纯色填充 Color
#define XML_RENDER_TYPE_SYSCOLOR          _T("syscolor")                // 系统色填充 Color
#define XML_RENDER_TYPE_COLORLIST         _T("colorlist")               // 类似office 2003工具栏式按钮
#define XML_RENDER_TYPE_NOTHEME           _T("notheme")                 // win2000主题控件
#define XML_RENDER_TYPE_THEME             _T("theme")                   // 系统主题控件
#define XML_RENDER_TYPE_NULL              _T("null")                    // 不绘制该render，但也会实例化该render指针，避免控件默认使用theme主题


#define XML_RENDER_THEME_DISABLE          _T("render.theme.disable")    // 是否强制使用无主题的渲染
#define XML_RENDER_COLOR                  _T("render.color")            // 填充颜色
#define XML_RENDER_BORDER                 _T("render.border")           // 边框大小 RECT
#define XML_RENDER_BORDERCOLOR            _T("render.border.color")     // 边框颜色
#define XML_RENDER_COLORFROM              _T("render.color.from")       // 渐变起始色
#define XML_RENDER_COLORTO                _T("render.color.to")         // 渐变终点色
#define XML_RENDER_COLOR_GRADIENT_DIR     _T("render.color.gradient.dir") // 渐变色方向

#define XML_RENDER_COLOR_GRADIENT_DIR_H   _T("h")
#define XML_RENDER_COLOR_GRADIENT_DIR_V   _T("v")
enum
{
    RENDER_COLOR_GRADIENT_DIR_H,
    RENDER_COLOR_GRADIENT_DIR_V,
};

#define XML_RENDER_IMAGE                  _T("render.image")            // 背景图片
#define XML_RENDER_IMAGE9REGION           _T("render.image.9region")    // 背景伸缩方式
#define XML_RENDER_LIST_STATEMAPINDEX     _T("render.list.statemap")    // 设置控件绘制状态对应的图片项，例如0:0,1:0,2:0,3:0,4:1 表示有两个图片项，但selection用第二张图片，其它用第一张图片
#define XML_RENDER_COLORLIST_COUNT        _T("render.colorlist.count")  // 颜色列表的数量
// #define XML_RENDER_IMAGELIST_COUNT        _T("render.imagelist.count")  // 图片列表中的图片数量
// #define XML_RENDER_IMAGELIST_LAYOUT       _T("render.imagelist.layout") // 图片排列方向(默认为横向)
// #define XML_RENDER_IMAGELIST_LAYOUT_H     _T("h")                       // 图片横向排列
// #define XML_RENDER_IMAGELIST_LAYOUT_V     _T("v")                       // 图片纵向排列
#define XML_RENDER_IMAGELIST_ALPHA_ANIMATE _T("render.imagelist.alphaanimate") // 是否启用alpha渐变动画

#define XML_RENDER_IMAGELISTITEM_INDEX    _T("render.imagelistitem.index")  // 图片列表项的索引值

#define XML_RENDER_IMAGE_DRAWTYPE         _T("render.image.drawtype")   // 图片绘制方式
#define XML_RENDER_IMAGE_DRAWTYPE_BITBLT  _T("bitblt")                  // (0,0)位置开始贴图Image + Color
#define XML_RENDER_IMAGE_DRAWTYPE_TILE    _T("tile")                    // (0,0)位置开始平铺Image
#define XML_RENDER_IMAGE_DRAWTYPE_STRETCH _T("stretch")                 // 拉伸模式Image + Image9Region*
#define XML_RENDER_IMAGE_DRAWTYPE_ADAPT   _T("adapt")                   // 自适应居中，不会超出绘制范围
#define XML_RENDER_IMAGE_DRAWTYPE_CENTER  _T("center")                  // 居中，可能超出绘制范围
#define XML_RENDER_IMAGE_DRAWTYPE_STRETCH_BORDER      _T("stretchborder")     // 通过9宫拉伸绘制边框，不绘制中间部分
#define XML_RENDER_IMAGE_DRAWTYPE_BITBLT_RIGHTTOP     _T("bitbltrighttop")    // 左上角对齐
#define XML_RENDER_IMAGE_DRAWTYPE_BITBLT_LEFTBOTTOM   _T("bitbltleftbottom")  // 左下角对齐
#define XML_RENDER_IMAGE_DRAWTYPE_BITBLT_RIGHTBOTTOM  _T("bitbltrightbottom") // 右下角对齐

#define XML_RENDER_IMAGE_ALPHA            _T("render.image.alpha")      // 图片绘制时使用的alpha值
#define XML_RENDER_IMAGE_SRC_REGION       _T("render.image.srcregion")  // 图片绘制的源区域

#define XML_BACKGND_RENDER_PREFIX         _T("back.")    // 背景渲染属性前缀，如back.render.type="" fore.render.image=""
#define XML_FOREGND_RENDER_PREFIX         _T("fore.")    // 前景
#define XML_ICON_RENDER_PREFIX            _T("icon.")    // 图标属性渲染前缀
#define XML_BKGCOLOR                      _T("bkgcolor")  // 背景颜色
#define XML_BKGCOLOR_PREFIX               _T("bkgcolor.") // 背景颜色前缀

enum IMAGELIST_LAYOUT_TYPE
{
    IMAGELIST_LAYOUT_TYPE_H,
    IMAGELIST_LAYOUT_TYPE_V
};

#define XML_TEXTRENDER                      _T("text.render")            // 文字渲染
#define XML_TEXTRENDER_TYPE                 _T("text.render.type")       // 设置文字渲染类型
#define XML_TEXTRENDER_TYPE_SIMPLE          _T("simple")                // color + font
#define XML_TEXTRENDER_TYPE_COLORLIST       _T("colorlist")             // color list + font
#define XML_TEXTRENDER_TYPE_FONTLIST        _T("fontlist")              // font list + color
#define XML_TEXTRENDER_TYPE_FONTCOLORLIST   _T("fontcolorlist")         // font list + color list
#define XML_TEXTRENDER_TYPE_CONTRAST_COLOR    _T("contrastcolor")       // font + 跟随背景色自适应的黑白color
#define XML_TEXTRENDER_TYPE_CONTRASTCOLORLIST _T("contrastcolorlist")   // 

#define XML_TEXTRENDER_COLOR                _T("text.render.color")      // 字体颜色
#define XML_TEXTRENDER_FONT                 _T("text.render.font")       // 字体
#define XML_TEXTRENDER_COLORLIST_COUNT      _T("text.render.colorlist.count")
#define XML_TEXTRENDER_FONTCOLORLIST_COUNT  _T("text.render.fontcolorlist.count")  // 字体、颜色数量
#define XML_TEXTRENDER_ALIGN                _T("text.render.align")      // 对齐
#define XML_TEXTRENDER_EFFECT               _T("text.render.effect")     // 特效

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

#define  XML_WINDOW_MAXIMIZE_BORDER                     _T("maximizeborder")   // int类型，最大化时位于屏幕外的区域大小
#define  XML_WINDOW_RESIZE_BORDER                       _T("resizeborder")     // int类型，拖拽区域大小
#define  XML_WINDOW_RESIZE_CAPABILITY                   _T("resizecapability") // 拖拽能力
#define  XML_WINDOW_RESIZE_CAPABILITY_NONE              _T("none")             // 禁止拖拽
#define  XML_WINDOW_RESIZE_CAPABILITY_CAPTION           _T("caption")          // 只能拖拽，不能修改大小
#define  XML_WINDOW_RESIZE_CAPABILITY_ALL               _T("all")              // 能拖拽，能修改大小

#define WRSB_TOP              0x8000     // ↑
#define WRSB_LEFT             0x4000     // ←
#define WRSB_BOTTOM           0x2000     // ↓
#define WRSB_RIGHT            0x1000     // →
#define WRSB_TOPLEFT          0x0800     // ↖
#define WRSB_TOPRIGHT         0x0400     // ↗
#define WRSB_BOTTOMLEFT       0x0200     // ↙
#define WRSB_BOTTOMRIGHT      0x0100     // ↘
#define WRSB_CAPTION          0x0080     // 
#define WRSB_DRAGSIZE         (WRSB_TOP | WRSB_LEFT | WRSB_BOTTOM | WRSB_RIGHT | WRSB_TOPLEFT | WRSB_TOPRIGHT | WRSB_BOTTOMLEFT | WRSB_BOTTOMRIGHT)
#define WRSB_ALL              (WRSB_CAPTION | WRSB_DRAGSIZE)
#define WRSB_NONE             0x0

// window rgn
#define  XML_WINDOW_TRANSPARENT_TYPE                    _T("transparent.type")
#define  XML_WINDOW_TRANSPARENT_9REGION                 _T("transparent.9region") // 窗口透明区域参数（9Region类型，可为4，8，9三种）

#define  XML_WINDOW_TRANSPARENT_TYPE_AERO               _T("aero")             // 使用分层窗口进行透明
#define  XML_WINDOW_TRANSPARENT_TYPE_LAYERED            _T("layered")          // 使用分层窗口进行透明
#define  XML_WINDOW_TRANSPARENT_TYPE_MASKCOLOR          _T("maskcolor")        // 使用颜色值
#define  XML_WINDOW_TRANSPARENT_TYPE_MASKALPHA          _T("maskalpha")        // 使用透明值
#define  XML_WINDOW_TRANSPARENT_TYPE_ANTIALIASING       _T("anti")             // 抗锯齿圆角
#define  XML_WINDOW_TRANSPARENT_TYPE_GLOW               _T("glow")             // 类VS2013边框发光效果

#define  XML_WINDOW_TRANSPARENT_MASKCOLOR_VALUE         _T("transparent.maskcolor")  // 窗口透明色
#define  XML_WINDOW_TRANSPARENT_MASKALPHA_VALUE         _T("transparent.maskalpha")  // 将窗口中小于alpha值通过的抠掉

#define  XML_WINDOW_TRANSPARENT_ANTI_9REGION            _T("transparent.anti.9region")       // 左上角圆角度数，上侧缩进，右上角圆角度数，左侧缩进，右侧缩进，左下角圆角度数，下侧缩进，右下角圆角度数
#define  XML_WINDOW_TRANSPARENT_ANTI_BORDERINDENT       _T("transparent.anti.borderIndent")  // 四周缩进值。默认为全0。用于解决aero blur切换到anti时，四周有一部分阴影的问题
#define  XML_WINDOW_TRANSPARENT_MASKALPHA_9REGION       _T("transparent.maskalpha.9region")  // 全0表示什么也不做。全-1表示整个窗口过滤。top,left,right.bottom四个值如果大于0表示要过滤这四个边，小于0表示不过滤，仅用于设置四个角的位置参数
#define  XML_WINDOW_TRANSPARENT_MASKCOLOR_9REGION       _T("transparent.maskcolor.9region")  // 同XML_WINDOW_TRANSPARENT_MASKALPHA_9REGION

#define  XML_WINDOW_TRANSPARENT_AERO_MODE               _T("transparent.aero.mode")   // aero透明模式
#define  XML_WINDOW_TRANSPARENT_AERO_MODE_TRANS         _T("trans")
#define  XML_WINDOW_TRANSPARENT_AERO_MODE_BLUR          _T("blur")
enum AERO_MODE 
{
    AERO_MODE_TRANSPARENT,   // 只透明
    AERO_MODE_BLUR,          // 带模糊
};
#define  XML_WINDOW_TRANSPARENT_AERO_TRANS_MARGINS      _T("transparent.aero.trans.margins")  // rect 透明区域设置（DwmExtendFrameIntoClientArea参数）
#define  XML_WINDOW_TRANSPARENT_AERO_BLUR_REGION        _T("transparent.aero.blur.region")    // rect 磨沙效果区域（DwmEnableBlurBehindWindow参数）
#define  XML_WINDOW_TRANSPARENT_AERO_BLUR_AUTOCLIPCHILDCTRL  _T("transparent.aero.blur.autoclipchildctrl")  // 是否自动剪裁子控件
#define  XML_WINDOW_TRANSPARENT_TYPE_AERO_DISABLE       _T("transparent.aero.disable")        // aero模式不可用时的窗口透明类型


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

#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY          _T("graphics")         // 图形引擎类型
#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY_GDI      _T("gdi")
#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY_GDIPLUS  _T("gdiplus")
#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY_D2D      _T("d2d")
#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY_DIRECT2D _T("direct2d")
#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY_D3D      _T("d3d")
#define XML_WINDOW_GRAPHICS_RENDER_LIBRARY_DIRECT3D _T("direct3d")

#define XML_WINDOW_NEED_ALPHACHANNEL                _T("needalphachannel")  // 窗口是否需要渲染alpha通道。默认为true


enum GRAPHICS_RENDER_LIBRARY_TYPE
{
	//	GRAPHICS_RENDER_LIBRARY_TYPE_AUTO = 0,   // <- 依据窗口类型决定。分层窗口使用GDIPLUS,普通窗口使用GDI，废弃，逻辑太复杂
	GRAPHICS_RENDER_LIBRARY_TYPE_GDI = 1,
	GRAPHICS_RENDER_LIBRARY_TYPE_GDIPLUS,
	GRAPHICS_RENDER_LIBRARY_TYPE_DIRECT2D,
	GRAPHICS_RENDER_LIBRARY_TYPE_DIRECT3D
};

#define XML_TEXTURE_RENDER_PREFIX            _T("texture.")         // customwindow用于支持换肤的皮肤render前缀
#define XML_MASK_RENDER_PREFIX               _T("mask.")            // 在绘制完子结点后，再画的一层(PostPaint)
#define XML_PANEL_RENDER_REGION              _T("render.region")    // 皮肤图片在窗口上显示的区域，类型于padding的配置

enum ROUND_CORNER_TYPE
{
    ROUND_CORNER_NONE,
    ROUND_CORNER_ANTI, // 抗锯齿模式
    ROUND_CORNER_DIRECT_THROW,  // 直接删除圆角处像素值
};

// 其它通用属性
#define XML_CURSOR                           _T("cursor")       // 对象的鼠标样式
#define XML_CURSOR_IDC_ARROW                 _T("idc_arrow")    // 各种系统鼠标样式配置
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


#define XML_TOOLTIP                          _T("tooltip")      // 对象的提示信息，自定义的提示内容需要代码自行添加
#define XML_CONTROL_STYLE_GROUP              _T("group")
#define XML_CONTROL_STYLE_TABABLE            _T("tabstop")
#define XML_REJEST_MOUSE_MSG                 _T("rejectmousemsg")  // 不接受鼠标消息
#define XML_ZINDEX_OVERLAP                   _T("zindexoverlap")   // z序重叠了标识
#define XML_ZORDER                           _T("zorder")          // 在本层中的z序，当不为0时，将为该对象创建一个layer
#define XML_ISNCCHILD                        _T("ncchild")         // 是一个ncchild，例如滚动条

#define XML_LAYER                            _T("layer")           // 为该对象建立一个renderlayer（带缓存）

// window
// #define  XML_WINDOW_BK_TYPE                    _T("bktype")     // 窗口背景绘制类型
// #define  XML_WINDOW_BK_TYPE_COLOR              _T("color")      // 窗口背景绘制类型--颜色，画刷画笔
// #define  XML_WINDOW_BK_TYPE_TILE               _T("tile")       // 窗口背景绘制类型--平铺
// #define  XML_WINDOW_BK_TYPE_SIMPLE             _T("simple")     // 窗口背景绘制类型--简单，(0,0)bitblt
// #define  XML_WINDOW_BK_TYPE_CENTER             _T("center")     // 窗口背景绘制类型--居中
// #define  XML_WINDOW_BK_TYPE_STRETCH            _T("stretch")    // 窗口背景绘制类型--拉伸
// #define  XML_WINDOW_BK_TYPE_FRAME              _T("frame")      // 窗口背景绘制类型--边框
// #define  XML_WINDOW_BK_IMAGE                   _T("bkimage")    // 窗口背景图片名称
// #define  XML_WINDOW_BK_TYPE_COLOR_PEN          _T("bkpencol")   // bktype=color，窗口边框色
// #define  XML_WINDOW_BK_TYPE_COLOR_BRUSH        _T("bkbrushcol") // bktype=color，窗口背景色
// #define  XML_WINDOW_BK_TYPE_FRAME_9REGION      _T("bk9region")  // bktype=frame，窗口切割范围

// 系统按钮ID
#define XML_SYS_BTN_RESTORE       _T("sys_restore")   // 系统还原按钮ID
#define XML_SYS_BTN_MAXIMIZE      _T("sys_maximize")  // 系统最大化按钮ID
#define XML_SYS_BTN_MINIMIZE      _T("sys_minimize")  // 系统最小化按钮ID
#define XML_SYS_BTN_CLOSE         _T("sys_close")     // 系统关闭按钮ID

// hwnd host
#define XML_HWNDHOST_CONTROL_ID   _T("controlid")     // 控件ID数值，整形

#pragma region // 布局layout的属性
#define XML_LAYOUT_PREFIX        _T("layout.")
#define XML_LAYOUT_TYPE          _T("layout.type")    // 对象的布局类型
//#define XML_LAYOUT_STACK         _T("stack")          // 布局类型：stack
#define XML_LAYOUT_VERT          _T("vert")           // 布局类型：vert
#define XML_LAYOUT_HORZ          _T("horz")           // 布局类型：horz
#define XML_LAYOUT_NULL          _T("null")           // 布局类型：null
#define XML_LAYOUT_GRID          _T("grid")           // 布局类型：grid
#define XML_LAYOUT_DOCK          _T("dock")           // 布局类型：dock
#define XML_LAYOUT_CANVAS        _T("canvas")         // 布局类型：canvas
#define XML_LAYOUT_CARD          _T("card")           // 布局类型：card
#define XML_LAYOUT_AVERAGE       _T("average")        // 布局类型：average

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

// stack布局的属性
// #define XML_LAYOUT_STACK_DIRECTION      _T("layout.direction")  // stack布局的方向
// #define XML_LAYOUT_STACK_LEFTTORIGHT    _T("lefttoright")       // xml中可配置的值
// #define XML_LAYOUT_STACK_RIGHTTOLEFT    _T("righttoleft")       // xml中可配置的值
// #define XML_LAYOUT_STACK_LEFTANDRIGHT   _T("leftandright")      // xml中可配置的值
// #define XML_LAYOUT_STACK_TOPTOBOTTOM    _T("toptobottom")       // xml中可配置的值
// #define XML_LAYOUT_STACK_BOTTOMTOTOP    _T("bottomtotop")       // xml中可配置的值
// #define XML_LAYOUT_STACK_TOPANDBOTTOM   _T("topandbottom")      // xml中可配置的值

// enum LAYOUT_STACK_DIRECTION
// {
//     LAYOUT_STACK_LEFTTORIGHT  = 0,   
//     LAYOUT_STACK_RIGHTTOLEFT  = 1,   
//     LAYOUT_STACK_LEFTANDRIGHT = 2,  
//     LAYOUT_STACK_TOPTOBOTTOM  = 3,   
//     LAYOUT_STACK_BOTTOMTOTOP  = 4,   
//     LAYOUT_STACK_TOPANDBOTTOM = 5   
// };
#define XML_LAYOUT_STACK_GAP            _T("layout.gap")        // 每一项之间的间隙

// average布局
#define XML_LAYOUT_AVERAGE_DIRECTION    _T("layout.direction")  // stack布局的方向
#define XML_LAYOUT_AVERAGE_H            _T("h")
#define XML_LAYOUT_AVERAGE_V            _T("v")

enum LAYOUT_AVERAGE_DIRECTION
{
    LAYOUT_AVERAGE_H,
    LAYOUT_AVERAGE_V
};

#define XML_LAYOUT_AVERAGE_GAP         _T("layout.gap")        // 每一项之间的间隙

// dock布局的属性

// grid布局的属性
#define XML_LAYOUT_GRID_WIDTH           _T("layout.width")      // grid布局的各列的宽度定义
#define XML_LAYOUT_GRID_HEIGHT          _T("layout.height")     // grid布局的各列的高度定义

#pragma endregion

#pragma region // 布局子元素属性

enum LAYOUT_FLAG
{
    LAYOUT_ITEM_ALIGN_LEFT   = 0x0001,
    LAYOUT_ITEM_ALIGN_TOP    = 0x0002,
    LAYOUT_ITEM_ALIGN_RIGHT  = 0x0004,
    LAYOUT_ITEM_ALIGN_BOTTOM = 0x0010,
    LAYOUT_ITEM_ALIGN_CENTER = 0x0020,
    LAYOUT_ITEM_ALIGN_VCENTER = 0x0040,
    LAYOUT_ITEM_ALIGN_FILL   = 0x0080,  // 填充剩余空间--stack布局
	// LAYOUT_FLAG_FORCE_DESIREDSIZE = 0x0100, // 强制使用控件自己的期望大小，废弃，再调一次get desired size得了

    LAYOUT_ITEM_ALIGN_LTRB = LAYOUT_ITEM_ALIGN_LEFT|LAYOUT_ITEM_ALIGN_TOP|LAYOUT_ITEM_ALIGN_RIGHT|LAYOUT_ITEM_ALIGN_BOTTOM,
};


// canvas布局中的子元素的属性
#define XML_LAYOUT_ITEM_LEFT        _T("layout.left")
#define XML_LAYOUT_ITEM_TOP         _T("layout.top")
#define XML_LAYOUT_ITEM_RIGHT       _T("layout.right")
#define XML_LAYOUT_ITEM_BOTTOM      _T("layout.bottom")

// stack布局中的子元素的属性
#define XML_LAYOUT_ITEM_ALIGN          _T("layout.align")       // 当layout.direction=leftandright||topandbottom时，需要指定子元素到底停靠在哪一侧
#define XML_LAYOUT_ITEM_ALIGN_LEFT     _T("left")
#define XML_LAYOUT_ITEM_ALIGN_RIGHT    _T("right")
#define XML_LAYOUT_ITEM_ALIGN_TOP      _T("top")
#define XML_LAYOUT_ITEM_ALIGN_BOTTOM   _T("bottom")

#define XML_LAYOUT_ITEM_ALIGN_CENTER   _T("center")  // -- stack/dock layout
#define XML_LAYOUT_ITEM_ALIGN_VCENTER  _T("vcenter") // -- stack layout
#define XML_LAYOUT_ITEM_ALIGN_FILL     _T("fill")    // -- stack layout

// 子控件四边全对齐布局，父对象在Measure自己时，如何处理该子控件的desiredsize
// 1. 正常情况下，由于子控件采用的是lrtb对齐，将不测试自己的大小，而是直接依赖于父对象
// 2. 在菜单窗口场景下，窗口的大小则是取决于菜单内容大小，但菜单又是lrtb对齐布局的，
//    因此窗口在遍历时，菜单的desiredsize不应该依赖父对象，而是返回真实大小
// 废弃，再调一次get desired size得了
//#define XML_LAYOUT_ALIGN_FORCE_DESIREDSIZE  _T("xxxx")


// enum LAYOUT_STACK_ITEM_ALIGN
// {
// 	LAYOUT_STACK_ITEM_ALIGN_LEFT = 0,
// 	LAYOUT_STACK_ITEM_ALIGN_TOP = 0,
// 	LAYOUT_STACK_ITEM_ALIGN_RIGHT = 1,
// 	LAYOUT_STACK_ITEM_ALIGN_BOTTOM = 1,
// };

// dock布局中的子元素的属性
// #define XML_LAYOUT_DOCK_DOCK          _T("layout.dock")       // 子元素停靠在哪一个方向
// #define XML_LAYOUT_DOCK_DOCK_LEFT     _T("left")
// #define XML_LAYOUT_DOCK_DOCK_RIGHT    _T("right")
// #define XML_LAYOUT_DOCK_DOCK_TOP      _T("top")
// #define XML_LAYOUT_DOCK_DOCK_BOTTOM   _T("bottom")
// #define XML_LAYOUT_DOCK_DOCK_CENTER   _T("center")

// grid布局中的子元素的属性
#define XML_LAYOUT_GRID_ROW          _T("layout.row")        // 对应 m_nConfigTop
#define XML_LAYOUT_GRID_COL          _T("layout.col")        // 对应 m_nConfigLeft
#define XML_LAYOUT_GRID_ROWSPAN      _T("layout.rowspan")    // 对应 m_nConfigBottom
#define XML_LAYOUT_GRID_COLSPAN      _T("layout.colspan")    // 对应 m_nConfigRight

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
#define  XML_STYLE_SELECTOR_TYPE_ID            _T("id")       // id 选择器
#define  XML_STYLE_SELECTOR_TYPE_CLASS         _T("class")    // class选择器
#define  XML_STYLE_INHERIT                     _T("inherit")  // 继承属性

#define  STYLE_ID_PREFIX                       _T('#')        // id选择器前缀
#define  STYLE_CLASS_PREFIX                    _T('.')        // class选择器前缀

// Control

// Label

// Picture

// GifPicture
#define  XML_GIFCTRL_GIF   _T("gif")       // gif资源id
#define  XML_GIFCTRL_AUTOPLAY    _T("autoplay")   // 是否自动开始，默认为false，需要手动调用start函数

// GroupBox
#define XML_GROUPBOX_TEXTINDENT   _T("textindent")  // 文字缩进值
#define XML_GROUPBOX_TEXTMARGIN   _T("textmargin")  // 文字与边框的间距
#define XML_GROUPBOX_TEXTALIGN    _T("textalign")   // 文字对齐方式

//////////////////////////////////////////////////////////////////////////
// Button

#define  XML_BUTTON_DEFPUSHBUTTON       _T("defpushbutton")    // 是否是默认按钮
#define  XML_BUTTON_DRAW_FOCUS_TYPE     _T("drawfocus")        // 如何绘制焦点
#define  XML_BUTTON_DRAW_FOCUS_TYPE_NONE   _T("none")        
#define  XML_BUTTON_DRAW_FOCUS_TYPE_DOT    _T("dot")    
#define  XML_BUTTON_HAND_CUROSR         _T("handcursor")       // 显示手型鼠标样式

typedef enum
{
    BUTTON_RENDER_DRAW_FOCUS_TYPE_NONE,                               // 不绘制
    BUTTON_RENDER_DRAW_FOCUS_TYPE_DOT,                                // 使用虚线点绘制
}BUTTON_RENDER_DRAW_FOCUS_TYPE;

#define XML_BUTTON_AUTOSIZE_TYPE           _T("autosize")     // 当没有指定按钮大小时，如何计算其大小
#define XML_BUTTON_AUTOSIZE_TYPE_BKIMAGE   _T("backimage")      // 背景图片的大小决定按钮大小
#define XML_BUTTON_AUTOSIZE_TYPE_CONTENT   _T("content")      // 背景图片的内容(图标+文本)决定按钮大小
#define XML_BUTTON_AUTOSIZE_TYPE_BKIMAGEHEIGHT_CONTENTWIDTH _T("bkimagehcontentw")  // 图片的高度+内容的宽度

enum BUTTON_AUTO_SIZE_TYPE
{
    BUTTON_AUTOSIZE_TYPE_NDEF,
    BUTTON_AUTOSIZE_TYPE_BKIMAGE,
    BUTTON_AUTOSIZE_TYPE_CONTENT,
    BUTTON_AUTOSIZE_TYPE_BKIMAGEHEIGHT_CONTENTWIDTH
};

#define XML_BUTTON_ICON_ALIGN  _T("iconalign")    // 按钮图标的位置

// #define  XML_BUTTON_ICON                             _T("icon")             // 按钮图标
// #define  XML_BUTTON_ICON_RENDER_TYPE                 _T("icontype")         // 按钮图标绘制类型
// #define  XML_BUTTON_ICON_RENDER_TYPE_STATIC          _T("static")           // 静态图标
// #define  XML_BUTTON_ICON_RENDER_TYPE_8STATE          _T("8state")           // 8态图标
// #define  XML_BUTTON_ICON_RENDER_TYPE_THEME_RADIO     _T("themeradio")       // 主题单选图标
// #define  XML_BUTTON_ICON_RENDER_TYPE_THEME_CHECK     _T("themecheck")       // 主题复选图标
// typedef enum
// {
// 	BUTTON_ICON_RENDER_TYPE_NONE,        // 不绘制图标
// 	BUTTON_ICON_RENDER_TYPE_STATIC,
// 	BUTTON_ICON_RENDER_TYPE_GIF,
// 	BUTTON_ICON_RENDER_TYPE_4STATE,
// 	BUTTON_ICON_RENDER_TYPE_8STATE,
// 	BUTTON_ICON_RENDER_TYPE_THEME_RADIO,
// 	BUTTON_ICON_RENDER_TYPE_THEME_CHECK
// }BUTTON_ICON_RENDER_TYPE;

#define   XML_BUTTON_ICON_TEXT_SPACE           _T("icontextspace")   // 文本与图标之间的间距
// #define  XML_BUTTON_TEXT_RENDER_TYPE           _T("texttype")         // 文本绘制类型
// #define  XML_BUTTON_TEXT_RENDER_TYPE_SINGLELINE _T("singleline")      // 单行文本
// #define  XML_BUTTON_TEXT_RENDER_TYPE_HYPERLINK  _T("hyperlink")       // 超链接模式
// typedef enum
// {
// 	BUTTON_TEXT_RENDER_TYPE_NONE,         // 不绘制文本
// 	BUTTON_TEXT_RENDER_TYPE_SINGLELINE,   // 单行文本
// 	BUTTON_TEXT_RENDER_TYPE_HYPERLINK,    // 超链接
// 
// }BUTTON_TEXT_RENDER_TYPE;


// HyperLink
#define  XML_HYPERLINK_HREF              _T("href")     // 超链接


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

//#define  XML_BUTTON_BK_TYPE                    _T("bktype")     // 按钮背景绘制类型
//#define  XML_BUTTON_BK_TYPE_COLOR              _T("color")    

//////////////////////////////////////////////////////////////////////////
// ProgressCtrl / SliderCtrl

#define  XML_PROGRESSBAR_DIRECTION            _T("direction")       // 滚动条的方向
#define  XML_PROGRESSBAR_DIRECTION_LEFTRIGHT  _T("lefttoright")       // 左->右
#define  XML_PROGRESSBAR_DIRECTION_BOTTOMTOP  _T("bottomtotop")       // 下->上
#define  XML_PROGRESSBAR_DIRECTION_RIGHTLEFT  _T("righttoleft")       // 右->左
#define  XML_PROGRESSBAR_DIRECTION_TOPBOTTOM  _T("toptobottom")       // 上->下

enum PROGRESSBAR_SCROLL_DIRECTION_TYPE
{
    PROGRESSBAR_SCROLL_LEFT_2_RIGHT,
    PROGRESSBAR_SCROLL_BOTTOM_2_TOP,
    PROGRESSBAR_SCROLL_RIGHT_2_LEFT,
    PROGRESSBAR_SCROLL_TOP_2_BOTTOM
};

#define  XML_SLIDERCTRL_THUMB_POINT_DIRECTION          _T("thumbarrow")  // 滑块按钮的指向
#define  XML_SLIDERCTRL_THUMB_POINT_DIRECTION_BOTH     _T("both")            // 默认值
#define  XML_SLIDERCTRL_THUMB_POINT_DIRECTION_LEFT     _T("left")    
#define  XML_SLIDERCTRL_THUMB_POINT_DIRECTION_TOP      _T("top")     
#define  XML_SLIDERCTRL_THUMB_POINT_DIRECTION_RIGHT    _T("right")   
#define  XML_SLIDERCTRL_THUMB_POINT_DIRECTION_BOTTOM   _T("bottom")   


// #define  PROGRESSCTRL_FLAG_DIRECTION_LEFTRIGHT    0x0000
// #define  PROGRESSCTRL_FLAG_DIRECTION_TOPBOTTOM    0x0001                
// #define  PROGRESSCTRL_FLAG_DIRECTION_RIGHTLEFT    0x0002
// #define  PROGRESSCTRL_FLAG_DIRECTION_BOTTOMTOP    0x0003

#define  XML_SLIDERCTRL_BUTTON_ATTR_PRIFIX     _T("trackbtn.")       // 滑动控件的按钮属性前缀

#define  XML_IMAGEPROGRESSCTRL_FOREIMAGE       _T("foreimage")       // 前景图片（一张）
#define  XML_IMAGEPROGRESSCTRL_BKGNDIMAGE      _T("bkgndimage")      // 背景图片（一张）

// Edit
#define  XML_EDIT_TEXT                        _T("text")             // 文字内容
#define  XML_EDIT_TEXT_ALIGN                  _T("align")            // 文字对齐方式

#define  XML_EDIT_COLOR                       _T("color.text")       // 正常文字显示颜色
#define  XML_EDIT_SELECT_COLOR                _T("color.select")     // 文字被选中时的颜色
#define  XML_EDIT_SELECT_BK_COLOR             _T("color.select.back")// 文字被选中时背景的颜色
#define  XML_EDIT_DISABLE_COLOR               _T("color.disable")    // 被禁用时的文本颜色
#define  XML_EDIT_READONLY_COLOR              _T("color.readonly")   // 只读时的文本颜色
#define  XML_INSTANTEDIT_COLOR                _T("color.quiet")      // 即时编辑时，未开启编辑时的文本颜色 
#define  XML_EDIT_MAX_LENGTH                  _T("limit")            // 内容的最长值
#define  XML_EDIT_CARET_HEIGHT                _T("caretheight")      // 光标及选中文本背景的高度
//#define  XML_EDIT_MAX_INT

// Scroll Bar
#define  XML_SCROLLBAR_RANGE                  _T("range")            // 滚动条范围设置
#define  XML_SCROLLBAR_PAGE                   _T("page")             // 滚动条页大小设置
#define  XML_SCROLLBAR_POS                    _T("pos")              // 滚动条位置设置
#define  XML_SCROLLBAR_BTNLINE                _T("btnline")          // 滚动条点击按钮后滚动大小设置
#define  XML_SCROLLBAR_WHEELLINE              _T("wheelline")        // 使用鼠标滑轮后滚动大小设置

#define  XML_HSCROLLBAR                           _T("hscrollbar")  // 横向滚动条类型
#define  XML_VSCROLLBAR                           _T("vscrollbar")  // 纵向滚动条类型
#define  XML_SCROLLBAR_NONE                       _T("none")        // 该方向无滚动条
#define  XML_SCROLLBAR_AUTO                       _T("auto")        // 该方向上的滚动条在需要时显示
#define  XML_SCROLLBAR_ALWAYSSHOW                 _T("show")        // 该方向上的滚动条保持显示
#define  XML_SCROLLBAR_ALWAYSHIDE                 _T("hide")        // 该方向上支持滚动，但不显示滚动条

enum SCROLLBAR_VISIBLE_TYPE
{
    SCROLLBAR_VISIBLE_NONE,          // 不使用
    SCROLLBAR_VISIBLE_AUTO,          // 当需要滚动的时候再显示
    SCROLLBAR_VISIBLE_SHOW_ALWAYS,   // 一直显示
    SCROLLBAR_VISIBLE_HIDE_ALWAYS,   // 一直隐藏，逻辑上的
};
enum SCROLLBAR_DIRECTION_TYPE
{
    HSCROLLBAR = WM_HSCROLL,
    VSCROLLBAR = WM_VSCROLL
};

// #define  XML_SCROLLBAR_TYPE                       _T("type")        // 前接XML_xxSCROLLBAR_PRIFIX，滚动条的类型
// #define  XML_SCROLLBAR_TYPE_SYSTEM                _T("system")      // 系统样式的滚动条，两个按钮+一个拖动条
// #define  XML_SCROLLBAR_TYPE_LOGIC                 _T("logic")       // 无滚动条显示，但维护绑定对象进行滚动的数据
// #define  XML_SCROLLBAR_TYPE_MENU                  _T("menu")        // 
// 
// enum SCROLLBAR_RENDER_TYPE
// {
//     SCROLLBAR_RENDER_TYPE_SYSTEM,
//     SCROLLBAR_RENDER_TYPE_LOGIC,
//     SCROLLBAR_RENDER_TYPE_MENU,
// };

#define  XML_HSCROLLBAR_PRIFIX                    _T("hscrollbar.") // 横向滚动条的属性前缀
#define  XML_VSCROLLBAR_PRIFIX                    _T("vscrollbar.") // 纵向滚动条的属性前缀

#define  XML_SCROLLBAR_RENDER_TYPE                _T("type")        // 设置滚动条的渲染类

// scrollpanel
#define  XML_SCROLLPANEL_VIEWSIZE                 _T("viewsize")    // 视图大小，格式为 "100,100"

// listbox
#define  XML_LISTCTRL_ITEM_HEIGHT                 _T("itemheight")  // 列表控件每一项的高度
#define  XML_LISTCTRL_ITEM_PADDING                _T("itempadding") // 列表项的缩进

#define  XML_LISTCTRL_MIN_WIDTH                   _T("minwidth")    //  auto size列表框的最小、大尺寸
#define  XML_LISTCTRL_MIN_HEIGHT                  _T("minheight")
#define  XML_LISTCTRL_MAX_WIDTH                   _T("maxwidth")
#define  XML_LISTCTRL_MAX_HEIGHT                  _T("maxheight")

#define  XML_LISTCTRL_EDIT_PRIFIX                 _T("edit.")       // 编辑框的属性前缀
#define  XML_LISTCTRL_BUTTON_PRIFIX               _T("button.")     // 按钮的属性前缀
#define  XML_LISTCTRL_COMBOBOX_PRIFIX             _T("combobox.")   // 下拉列表的属性前缀

// combobox
#define  XML_COMBOBOX_EDIT_PRIFIX                 _T("edit.")       // 编辑框的属性前缀
#define  XML_COMBOBOX_BUTTON_PRIFIX               _T("button.")     // 按钮的属性前缀
#define  XML_COMBOBOX_LISTBOX_PRIFIX              _T("listbox.")    // 下拉列表的属性前缀
#define  XML_COMBOBOX_DROPWNDID                   _T("dropwnd.id")  // 下拉窗口id

// ledctrl
#define  XML_LEDCTRL_INDEXMAP                     _T("indexmap")    // 图片索引映射关系
#define  XML_LEDCTRL_ITEMWIDTH                    _T("itemwidth")   // 配置每一项的显示宽度（默认为图片宽度）
#define  XML_LEDCTRL_ITEMHEIGHT                   _T("itemheight")  // 配置每一项的显示高度（默认为图片高度）
#define  XML_LEDEXCTRL_SAMEWIDTH                  _T("samewidth")   // bool，如果大小相同的话，就不用配置posmap属性了
#define  XML_LEDEXCTRL_POSMAP                     _T("posmap")      // 图片项位置映射关系  0,10;10,16;16,6;22,6;28,6;34,6;40,6;46,6;52,6;58,6;64,6 ...

// menu
#define  XML_MENU                 _T("Menu")      // layout.xml中菜单结点的tag name
#define  XML_MENU_STRINGITEM      _T("String")    //
#define  XML_MENU_POPUPITEM       _T("Popup")     //
#define  XML_MENU_SEPARATORITEM   _T("Separator") // 
#define  XML_MENU_DISABLE         _T("disable")   // menu item是否可用，为1或者true表示被禁用
#define  XML_MENU_POPUP_WNDID     _T("wnd.id")    // popup子菜单皮肤资源窗口id
#define  XML_MENU_POPUP_MENUID    _T("menu.id")   // popup子菜单在窗口中的控件id

#define  XML_MENU_ICONGUTTERWIDTH            _T("icongutterwidth")  // 菜单图标列的宽度
#define  XML_MENU_POPUPTRIANGLEWIDTH         _T("trianglewidth")    // 右侧三角形的占用宽度
#define  XML_MENU_SEPARATORHEIGHT            _T("separatorheight")  // 分隔条的高度
#define  XML_MENU_LEFT_TEXT_INDENT           _T("textleftindent")   // 左侧文本缩进
#define  XML_MENU_RIGHT_TEXT_INDENT          _T("textrightindent")  // 左侧文本缩进
#define  XML_MENU_SUBMENU_GAP                _T("submenugap")       // 子菜单与父菜单之间的间隙，默认为-2

#define  XML_MENU_RADIO_ICON_RENDER_PREFIX   _T("radioicon.")       // 选中图标绘制render前缀
#define  XML_MENU_CHECK_ICON_RENDER_PREFIX   _T("checkicon.")       // 选中图标绘制render前缀
#define  XML_MENU_RADIOCHECK_ICONBK_RENDER_PREFIX   _T("radiocheckiconbk.") // 选中图标背景绘制render前缀
#define  XML_MENU_SEPARATOR_RENDEER_PREFIX   _T("separator.")       // 分隔条绘制render前缀
#define  XML_MENU_TRAIANGLE_RENDER_PREFIX    _T("triangle.")        // 弹出箭头绘制render前缀

#define  XML_MENU_ITEM_ICON_RENDER_PREFIX    _T("icon.")            // 菜单项的图标

#define  MENU_SEPARATOR_ID    -1
#define  MENU_POPUP_ID        0

// header ctrl
#define  XML_HEADERCTRL_RIGHT_BLANK_RENDER_PRIFIX  _T("rightblank.") // 右侧空白区域的背景绘制前缘


// listview ctrl
#define  XML_LISTVIEWCTRL_HEADERCTRL_PREFIX   _T("headerctrl.")  // 列表控件中的header属性前缀

// Flash Wrap
#define  XML_FLASH_URI                        _T("uri")          // flash文件地址
#define  XML_FLASH_WMODE                      _T("wmode")        // flash窗口模式
#define  XML_FLASH_WMODE_WINDOW               _T("window")
#define  XML_FLASH_WMODE_OPAQUE               _T("opaque")
#define  XML_FLASH_WMODE_TRANSPARENT          _T("transparent")
enum FLASH_WMODE
{
    FLASH_WMODE_WINDOW,
    FLASH_WMODE_OPAQUE,
    FLASH_WMODE_TRANSPARENT,
};

// Layer -- 过期
// #define  XML_LAYER_TYPE             _T("type")    // render chain中的layer类型
// #define  XML_LAYER_TYPE_DIRECT      _T("direct")  // 直接渲染，无需缓存。renderbase类型
// #define  XML_LAYER_TYPE_CONTROL     _T("control") // 内嵌一个控件，例如flash

//#define  XML_LAYER_TYPE_FLASH       _T("flash")   // flash层


// TabCtrl
#define   XML_TABCTRL_HEAD_HEIGHT               _T("head.height")  // 头部的高度
#define   XML_TABCTRL_HEAD_LAYOUT_TYPE          _T("head.layout")  // 头部布局方式
#define   XML_TABCTRL_HEAD_LAYOUT_TYPE_Win32    _T("win32")
#define   XML_TABCTRL_HEAD_LAYOUT_TYPE_AVERAGE  _T("average")
#define   XML_TABCTRL_HEAD_LAYOUT_TYPE_FIX      _T("fix")
#define   XML_TABCTRL_CONTENTPANEL_PREFIX       _T("contentpanel.")

enum TABCTRL_HEAD_LAYOUT_TYPE
{
    TABCTRL_HEAD_LAYOUT_TYPE_FIX = 0,     // 固定大小排列在左侧
    TABCTRL_HEAD_LAYOUT_TYPE_AVERAGE = 1, // 按照TABCTRL的大小平均设置每个按钮的宽度
    TABCTRL_HEAD_LAYOUT_TYPE_Win32 = 2,   // 操作系统类型
};

// TreeView
#define   XML_EXPANDICON_RENDER_TYPE_PREFIX   _T("expandicon.")   // 展开/收缩图标render前缀


}
#endif XMLDEFINE_H_CBBD5468_6BF1_45ac_B02C_F10AC19A651A