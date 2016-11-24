#ifndef _UI_CONTROLDEFINE_H_
#define _UI_CONTROLDEFINE_H_

// ���뵼������
#ifdef UICTRL_EXPORTS
#define UICTRL_API __declspec(dllexport)
#else
#define UICTRL_API __declspec(dllimport)
#endif

#define UICTRL_API_UUID(guid)  __declspec(uuid(#guid)) UICTRL_API

namespace UI
{
// �����λ��Control Sub Type��ʶ

#if 0


#define SLIDER_STYLE_POINT_LEFT         0x0100    // ���鰴ť��ͷָ����� Ĭ��Ϊ��ָ��
#define SLIDER_STYLE_POINT_TOP          0x0100    // ���鰴ť��ͷָ���ϲ�
#define SLIDER_STYLE_POINT_RIGHT        0x0200    // ���鰴ť��ͷָ���Ҳ�
#define SLIDER_STYLE_POINT_BOTTOM       0x0200    // ���鰴ť��ͷָ���²�
#define SLIDER_STYLE_SHOWTICKS          0x0400    // ��ʾ�̶ȣ�Ĭ��ֻ��ʾ��һ�������һ��ֵ�Ŀ̶�
#define SLIDER_STYLE_AUTOTICKS          0x0800    // �Զ���ʾ�̶ȣ�Ĭ��ÿ��һ��ֵ��ʾһ���̶�

// richedit style
#define RICHEDIT_STYLE_TEXTSHADOW       0x0100    // �����ı���Ӱ������͸�������ϣ����������뱳����ɫ̫�ӽ���������Ӱ��������

    // tree style
#define TREE_STYLE_HASLINE              0x0100    // ��������

    // listbox style

    // menu style
#define MENU_STYLE_AUTO_DELETE_SUBMENU  0x1000    // ����������ʱ���Ƿ��Զ��ͷ��Ӳ˵��ڴ档��Ҫ���ڴ�xml��load�Ĳ˵�
    // ��Ϊ���ڲ��������Ӳ˵����ڲ����������Ӳ˵������ⲿ�Լ�create����

// tab ctrl
#define TABCTRL_STYLE_BOTTOM            0x0100    // ��ťλ���²�
#endif
}

#endif  // _UI_CONTROLDEFINE_H_