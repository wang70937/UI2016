#include "stdafx.h"
#include "dpihelper.h"

namespace UI
{

long  GetDpi()
{
    static long dpi = 0;
    if (0 == dpi)
    {
        // �ȼ���û��Ƿ������DPI����
        bool bDisableScale = false;

        ATL::CRegKey reg;
        if (ERROR_SUCCESS == reg.Open(
            HKEY_CURRENT_USER, 
            TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers"),
            KEY_READ))
        {
            //REG_SZ
            TCHAR szExePath[MAX_PATH] = {0};
            GetModuleFileName(NULL, szExePath, MAX_PATH-1);

            TCHAR szValue[256] = {0};
            unsigned long count = 256;
            if (ERROR_SUCCESS == reg.QueryStringValue(szExePath, szValue, &count))
            {
                if (_tcsstr(szValue, TEXT("HIGHDPIAWARE")))
                {
                    bDisableScale = true;
                }
            }
        }

        if (bDisableScale)
        {
            dpi = DEFAULT_SCREEN_DPI;
        }
        else
        {
            // �ɳ����Լ�ʵ��DPI���ţ�����ϵͳ��æ��
            HMODULE hModule = GetModuleHandle(L"User32.dll");
            if (hModule)
            {
                // SetProcessDPIAware();
                FARPROC f = GetProcAddress(hModule, "SetProcessDPIAware");
                if (f)
                    f();
            }

            HDC hDC = GetDC(NULL);
            dpi = GetDeviceCaps(hDC, LOGPIXELSY);
            ReleaseDC(NULL, hDC);
        }
    }

    return dpi;
}

float  GetDpiScale()
{
    static bool bInit = false;
    static float fScale = 0;

    if (!bInit)
    {
        bInit = true;
        fScale = (float)GetDpi() / DEFAULT_SCREEN_DPI;
    }

    return fScale;
}

long  ScaleByDpi(long x)
{
    if (0==x)
        return 0;

    if (GetDpi() == DEFAULT_SCREEN_DPI)
        return x;

    return _round(x * GetDpiScale());
}
long  RestoreByDpi(long x)
{
    if (0==x)
        return 0;

    if (GetDpi() == DEFAULT_SCREEN_DPI)
        return x;

	return _round(x / GetDpiScale());
}

// ��ȡ��߶�С��0ʱ����AUTO/NDEF����Ӧ��dpi
long  ScaleByDpi_if_gt0(long x)
{
    if (x <= 0)
        return x;

    return ScaleByDpi(x);
}

long  RestoreByDpi_if_gt0(long x)
{
    if (x <= 0)
        return x;

    return RestoreByDpi(x);
}

}