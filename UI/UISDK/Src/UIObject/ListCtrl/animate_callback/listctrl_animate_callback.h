#pragma once

// �����ķ�װʵ���� UIAnimate.dll �У�uisdk.dll�������� AnimateBase.dll��
// ����ֱ�ӵ��� UIAnimate ģ��Ľӿڡ������ uisdk ��Ԥ���ص��ӿڣ���ʵ��
// ���� UIAnimate ���С�


namespace UI
{
    class ListItemBase;
}

typedef bool (__stdcall *pfnListItemRectChangedCallback)(
                                IListItemBase& item,
                                LPCRECT prcOld, 
                                LPCRECT prcNew);
#if 0
class ListCtrlAnimateCallback
{
protected:
    ListCtrlAnimateCallback();
    ~ListCtrlAnimateCallback();

public:
    static ListCtrlAnimateCallback&  Get();

    static void  SetListItemRectChangedCallback(
                    pfnListItemRectChangedCallback);

    static bool  HandleItemRectChanged(
                    ListItemBase& item,
                    LPCRECT prcOld, 
                    LPCRECT prcNew);

private:
    pfnListItemRectChangedCallback  m_pfnListItemRectChangedCallback;
};
#endif