#pragma once

#define DEFAULT_SCREEN_DPI  96   // USER_DEFAULT_SCREEN_DPI

//
// DPI �ı�֪ͨ�� Windows 8.1����Ч
// Windows 7 ���޸�DPI��ϵͳ��Ҫ��ע��������Ч��
//
// wParam:
//   HIWORD:  Y DPI
//   LOWORD:  X DPI
// lParam:
//   ������������λ��
//   RECT* const prcNewWindow = (RECT*)lParam;
//#define WM_DPICHANGED      0x02E0
//

// Ӧ��ʹ�� GetSystemMetrics(SM_CXSCREEN) �� GetSystemMetrics(SM_CXSCREEN) ����ȡ��Ļ��С�ߴ硣
// Ӧ��ʹ�� GetSystemMetrics(SM_CXBORDER) �� GetSystemMetrics(SM_CYBORDER) ����ȡ�߿�ߴ硣
// Ӧ��ʹ�� GetSystemMetrics(SM_CXICON) �� GetSystemMetrics(SM_CXSMICON) ����ȡ��ͼ��ߴ��Сͼ��ߴ硣


//
// ͼƬ��DPI����Ӧ����Ŀǰ�뵽���ַ�ʽ��
// 1. �ڼ���ͼƬʱ���ͽ�ͼƬֱ�����ŵ���Ӧ�ߴ�
//    �ŵ㣺
//          1.�򵥣�ֻ��ҪԴͷ�����ɣ���Ӱ����ƴ���.
//          2.��֧����Button��autosize by bkgimage w/h�ĳ�����
//          3.ʹ��GdiPlus��֧������ʱ����ݹ���
//    ȱ�㣺
//          1.ռ�ø�����ڴ�
//          2.����imagelist����֧�ֲ��ˡ�
//            ����һ��154*14��imagelist�������itemΪ14*14
//            �Ŵ�125%֮���ȱ���� 154*1.25 = 192.5�� 14*1.25 = 17.5
//            ��ʱ�����ǽ�item width���17����18���ǲ��Եģ�û��ƽ��������.
//            �����ڲ��ÿ����ʱ���ڶ���item��Ӱ���1��item�ı�Ե������1��
//            item�ı�Ե�����Ǵ�͸��ʱ�ܵ���Ӱ��ǳ�����
//            -->���������
//             . ��item����������������ء�
//             . ��ʹ��DPI���죬ֱ��9������
//             . ��imagelist�ֶ�DPI���죬ÿ��itemֻ�ܼ����������ܼ�����������Ϊ�������������ϵ��֮���ֳ�С���ˡ�
//
// 2. �ڻ��Ƶ�ʱ���޸�Ŀ���Ⱥ͸߶ȣ��ﵽ���ŵ�Ч��
//    ��Ҫȱ�㣺GDI AlphaBlend���첻֧�ֿ����
//
namespace UI
{
    long  GetDpi();

    long  ScaleByDpi(long x);
    long  ScaleByDpi_if_gt0(long x);
    long  RestoreByDpi(long x);
    long  RestoreByDpi_if_gt0(long x);
	float  GetDpiScale();
}