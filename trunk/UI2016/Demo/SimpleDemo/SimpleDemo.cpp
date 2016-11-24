// SimpleDemo.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SimpleDemo.h"

#include "UI/UISDK/Inc/inc.h"
#include "UI/UISDK/Inc/Util/util.h"
#pragma comment(lib, "UISDK.lib")

#include "UI/UICTRL/Inc/inc.h"
#pragma comment(lib, "UICTRL.lib")

#include "UI/UICTRL/Inc/Interface/ibutton.h"
#include "UI/UISDK/Inc/Interface/icustomwindow.h"
#include "UI/UICTRL/Inc/Interface/ilabel.h"
#include "UI/UICTRL/Inc/Interface/imenu.h"
#include "UI/UICTRL/Inc/Interface/iedit.h"
#include <atlcomcli.h>
#include "UI/UICTRL/Inc/Interface/ilistbox.h"
#include "UI/UICTRL/Inc/Interface/icombobox.h"
#include "UI/UICTRL/Inc/Interface/irichtext.h"

// #include "UI/UICef/Inc/inc.h"
// #include "UI/UICef/Inc/icefctrl.h"
// #pragma comment(lib, "UICef.lib")


#define PI 3.141592653f
float __stdcall TimingFuction(float f)
{
    return sin(PI * f);
}

class LoginWindow : public UI::IWindowDelegate, public UIA::IAnimateEventCallback
{
public:
	LoginWindow()
	{
		m_pWindow = nullptr;
	}

	void  Create(UI::ISkinRes* pSkinRes)
	{
		m_pWindow = UI::ICustomWindow::CreateInstance(pSkinRes);
		m_pWindow->SetWindowMessageCallback(this);

		m_pWindow->Create(L"LoginWnd");
		m_pWindow->ShowWindow();
	}

protected:
	virtual BOOL  OnWindowMessage(UINT msg, WPARAM wParam, LPARAM, LRESULT& lResult)
	{
		if (msg == WM_DESTROY)
		{
			PostQuitMessage(0);
		}
		else if (msg == WM_MOUSEWHEEL)
		{
		}
		return FALSE;
	}
	virtual BOOL  OnWindowUIMessage(UI::UIMSG* pMsg) override
	{
		if (pMsg->message == WM_CONTEXTMENU)
		{
			UI::LoadMenuData data = { 0 };
			data.pSkinRes = m_pWindow->GetSkinRes();
			data.hWndParent = m_pWindow->GetHWND();
			UI::CMenuPtr pMenu = UILoadMenu(&data);

			pMenu->AppendString(L"AAAA", 1);
			pMenu->AppendSeparator(0);
			pMenu->AppendString(L"BBBB", 2);

			POINT pt;
			GetCursorPos(&pt);
			pMenu->TrackPopupMenu(TPM_RETURNCMD, pt.x, pt.y, 0);
		}
		return FALSE;
	}


    void onInsert()
    {
        UI::IRichText* pRichText = (UI::IRichText*)
            m_pWindow->FindObject(TEXT("richtext"));
        if (pRichText)
        {
            pRichText->ReplaceText(L"Insert");
        }
    }
    void  OnRichTextLinkClicked(UI::IRichText*, UI::RT::ILink* pLink)
    {
        LPCTSTR szText = pLink->GetId();
        OutputDebugString(szText);
        OutputDebugString(L"\n");
        ShellExecute(nullptr, L"open", pLink->GetReference(), 0, 0, SW_SHOW);
    }
    virtual void  OnWindowInitialize()
	{
        UI::IButton* pBtn = (UI::IButton*)m_pWindow->FindObject(TEXT("btnInsert"));
        if (pBtn)
            pBtn->ClickedEvent().connect0(this, &LoginWindow::onInsert);
        
        UI::IRichText* pRichText = (UI::IRichText*)
            m_pWindow->FindObject(TEXT("richtext"));
        if (pRichText)
        {
            pRichText->LickClickedEvent().connect(this, &LoginWindow::OnRichTextLinkClicked);
            pRichText->SetPageMode(UI::PageMode::multiline);
            //pRichText->AppendText(TEXT("1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\nBCDE"));
     
//             {
// 
//                 UI::RT::IChatMessage* msg = pRichText->BeginInsertMessage();
//                 msg->SetAlign(UI::RT::HALIGN::left);
// 
//                 pRichText->ReplaceText(L"[·͸�����]:");
// 
//                 HANDLE bubble = msg->EnterBubble();
//                 pRichText->ReplaceText(L"�����й����кͻ��еĲ��������˹���Ͷ���ߵĸ߶ȹ�ע��������������Ϊ�й��Ľ����г�Ŀǰ������Щ��Ҫ�������ս���й������Խ����г�δ���ķ�չ�ͼ�ǿ�����ʲô�ƻ������л��к�ծȯ�г���������Щ�ص�ĸ��ʩ�����ڵ��г������᲻��Ӱ��ĸ�Ľ��ȣ����ͨ�᲻�������Ƴ���лл��");
//                 msg->LeaveBubble(bubble);
// 
//                 pRichText->EndInsertMessage(msg);
//             }
#if 1
            UI::RT::IParagraph* para = nullptr;
            
            pRichText->ReplaceText(L"���ǿ��������������(ʵ¼)");
            para = pRichText->GetSelectionAnchorParagraph();
            UIASSERT(para);
            {
                UI::CharFormat cf;
                pRichText->GetDefaultCharFormat(&cf);

                cf.bBold = true;
                cf.lFontSize = 14;
                para->SetCharformat(&cf);

                para->SetAlign(UI::RT::HALIGN::center);
                para->SetBottomSpace(10);
                pRichText->ReplaceText(L"\n");
            }

            // link
            {
                
                UI::RT::ILink* pLink = pRichText->ReplaceLink(
                    L"http://news.qq.com/a/20160316/015694.htm");
                if (pLink)
                {
                    pLink->SetId(L"AAA");
                    pLink->SetReference(L"http://news.qq.com/a/20160316/015694.htm");
                    pLink->SetToolTip(L"���ǿ��������������");
                }
                para = pRichText->GetSelectionAnchorParagraph();
                para->SetBottomSpace(10);

                pRichText->ReplaceText(L"\n");
            }

            // image
            {
                para = pRichText->GetSelectionAnchorParagraph();
                UIASSERT(para);
                {
                    para->SetAlign(UI::RT::HALIGN::center);
                    para->SetBottomSpace(10);
                }

                TCHAR szPath[MAX_PATH] = { 0 };
                UI::Util::PathInBin(L"Sample\\132451642.jpg", szPath);
                pRichText->AppendImage(szPath);
                pRichText->ReplaceText(L"\n����Ժ�������ǿ�ش���������ʡ����������� �ڿ� ��");
                para = pRichText->GetSelectionAnchorParagraph();
                UIASSERT(para);
                {
                    para->SetAlign(UI::RT::HALIGN::center);
                    para->SetBottomSpace(20);
                }
                pRichText->ReplaceText(L"\n");
            }

            if (1)
            {
                pRichText->ReplaceText(L"3��16������ʮ����ȫ���˴��Ĵλ����Ļ�󣬹���Ժ�������ǿ������������¥��ɫ��������ɷ�ʮ����ȫ���˴��Ĵλ����������߲��ش������������⡣\r\n");
            }
            if (1)
            {
                UI::RT::IChatMessage* msg = pRichText->BeginInsertMessage();
                pRichText->ReplaceText(L"[��Ө]:");
                HANDLE bubble = msg->EnterBubble();
                pRichText->ReplaceText(L"��Һã��������Ǻ����ҵ����뵽���ǿ������������߼��档���쵽���ļ��߻��ǱȽ϶�ģ�������ÿλ���㵽�ļ���ÿ�ξ���һ�����⣬������ҵĻ�����Ը���һЩ�����������ǿ�������仰��");
                msg->LeaveBubble(bubble);
                pRichText->EndInsertMessage(msg);
            }

            {
                
                UI::RT::IChatMessage* msg = pRichText->BeginInsertMessage();
                msg->SetAlign(UI::RT::HALIGN::right);
                pRichText->ReplaceText(L"[���ǿ]:");

                HANDLE bubble = msg->EnterBubble();
                pRichText->ReplaceText(L"��ӭ������μӼ��߻ᣬ��л���Ƕ��й�����Ĺ�ע�͸��������͡��ղ�������˵�ˣ�ʱ�����ޣ��͵���ֱ��ɡ�");
                msg->LeaveBubble(bubble);
                pRichText->EndInsertMessage(msg);
            }

            
            {
                UI::RT::IChatMessage* msg = pRichText->BeginInsertMessage();
                msg->SetAlign(UI::RT::HALIGN::left);

                pRichText->ReplaceText(L"[·͸�����]:");
                
                HANDLE bubble = msg->EnterBubble();
                pRichText->ReplaceText(L"�����й����кͻ��еĲ��������˹���Ͷ���ߵĸ߶ȹ�ע��������������Ϊ�й��Ľ����г�Ŀǰ������Щ��Ҫ�������ս���й������Խ����г�δ���ķ�չ�ͼ�ǿ�����ʲô�ƻ������л��к�ծȯ�г���������Щ�ص�ĸ��ʩ�����ڵ��г������᲻��Ӱ��ĸ�Ľ��ȣ����ͨ�᲻�������Ƴ���лл��");
                msg->LeaveBubble(bubble);

                pRichText->EndInsertMessage(msg);
            }
            
            {
                UI::RT::IChatMessage* msg = pRichText->BeginInsertMessage();
                msg->SetAlign(UI::RT::HALIGN::right);

                pRichText->ReplaceText(L"[���ǿ]:");

                HANDLE bubble = msg->EnterBubble();
                pRichText->ReplaceText(
                   L"�����ʵ�һ�����⣬��Ͱѹ��л��еȽ����г����⵱����ͷ�ڡ�������Ҳ������⡣��Ϊ����������ı����������ھ�������ķ��������ǽ�����Ҫ������Ҫ֧��ʵ�徭�õķ�չ��ʵ�徭�ò���չ���ǽ������ķ��ա�ȥ�����ǲ�ȡ��һϵ����Ϣ����׼������׼�ȴ�ʩ���ⲻ���������ɣ�����ʼ��ע����ջ��ҹ�Ӧ�����ɽ��ʶȣ���Ҫ����Ϊ�˽���ʵ�徭�����ʵĳɱ������Խ��ڻ�������Ҫ����ȥ֧��ʵ�徭�ã��ر���С΢��ҵ�Ľ�����չ����������ʵ���顣\n"
                   L"��Ȼ����Ҳ��������Ĺ��ɣ�Ҫ�������ա��Ҹ���ע���ǽ��ڻ�������ȥ������һЩ��ҵ����ҵ��Ӫ���ѣ����ڻ�����������������������ġ����������е������յ���������Ϊ��ҵ���е��ʱ������ʳ�����13%�����ڹ��ʱ�׼�����������ʴﵽ180%���ϣ��������Ƕ���150%�ı�׼���������ǻ����������г������ֶν�����ҵ��ծ���ʡ���ҵծ���ʸ����������ˣ���Ϊ�й��Ǽ������Ϊ�����������ǵľ�������Ҳ�Ƚϸߡ�������ˣ������г����������Ĳ��������ǻ���Ҫ�ᶨ���Ƶط�չ���ε��ʱ��г�������Ҳ����ͨ���г���ծת�ɵķ�ʽ���𲽽�����ҵ�ĸܸ��ʡ�\n"
                   L"ȥ�����ڶ������ص�ԭ���й���Ʊ�г��������쳣�������йط����ȡ�ۺ����ȶ��г��ľٴ룬ʵ������Ҫ��������ϵͳ�ԵĽ��ڷ��գ���һ���������˵ġ���һ����ô�죿ǰ�������������ε�֤�����ϯ���ھ��������Ѿ����˲�������Ϊʱ�������Ҳ�չ���ˡ������ǹ��С�ծ�С�������Щ�����г������������г�������Ҫ�ƽ��г��������λ��ĸĸ��Ȼ�������м�ܵ����Σ����ڿ����������Ƶı仯����Ҫ�ĸ���������ǵĽ��ڼ����ϵ��Ҫʵ��ȫ���ǣ���Ϊ���ڽ��ڴ��µĲ�Ʒ�ܶ࣬�������¼�ܿհף�Ҫ��ǿЭ���ԣ���Ϊ�����г���Ʒ֮������ȱȽϸߣ�Э��Ҫ��Ȩ������Ҫ����Ȩ��һ�¡������йز��ź͵ط�Ҫ�ֲ㸺�𣬷�������Ҫ��ʱ���ã���ֹ��ͷ�Ե��������ӣ���ȻҲ�������̵��·��ա���֮������Ҫ�ɴ��۾�������һ˫��ǿ��ܵġ����۽𾦡���\n"
                   L"����������һ�Ҫǿ��һ�㣬���ƺ͸ĸ���ڼ���ƶ���һ�����̣���ǰ���йز��ź͵ط�����Ҫ�����Ѷ���ְ������ְ���������𣬾��������κε���и�����һ�Ҫ�ܽᾭ��ͽ�ѵ����Ҳ�Ǳ����ý��������ߺ�Ͷ���ߵĺϷ�Ȩ�棬����ɾ�Ҫ���������ˡ�лл��\n"
                   );
                msg->LeaveBubble(bubble);
                pRichText->EndInsertMessage(msg);
            }
#endif
        }
	}

    virtual UIA::E_ANIMATE_TICK_RESULT  OnAnimateTick(
                    UIA::IStoryboard* p) override
    {
        int x = p->GetTimeline(0)->GetCurrentIntValue();
        int y = p->GetTimeline(1)->GetCurrentIntValue();
        UI::IPanel* pPanel = (UI::IPanel*)m_pWindow->FindObject(TEXT("test"));
        pPanel->SetObjectPos(x, y, 0, 0, SWP_NOSIZE);

        return UIA::ANIMATE_TICK_RESULT_CONTINUE;
    }

    void on_listitem_btn_click(/*UI::IButton* pBtn*/)
    {
    }

	void on_click(/*UI::IButton**/)
	{
        UI::IGifCtrl* pGif = (UI::IGifCtrl*)m_pWindow->FindObject(TEXT("gif"));
        if (pGif)
        {
            pGif->SetVisible(!pGif->IsSelfVisible());
        }
	}

private:
	UI::ICustomWindow*  m_pWindow;
	UI::bindable_int  m_nCount;
	UI::bindable_string  m_strName;
};

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UI::IUIApplication* pUIApp = NULL;
	UI::CreateUIApplication(&pUIApp);
	UICTRL_RegisterUIObject(pUIApp);
//    UICef_RegisterUIObject(pUIApp);

	// ������Դ
	TCHAR szResPath[MAX_PATH] = { 0 };
	UI::Util::PathInBin(L"Skin\\Root", szResPath);
	UI::ISkinRes* pRootSkinRes = pUIApp->LoadSkinRes(szResPath);

	// ��¼�����Դ
	UI::Util::PathInBin(L"Skin\\Plugin\\Login", szResPath);
	UI::ISkinRes* pLoginSkinRes = pUIApp->LoadSkinRes(szResPath);

	// ��
	pLoginSkinRes->SetParentSkinRes(pRootSkinRes);

	LoginWindow m;
	m.Create(pLoginSkinRes);
	

// 	MainWindow m2;
// 	m2.Create(pSkinRes2);

	pUIApp->MsgHandleLoop();
	pUIApp->Release();
	
	return 0;
}