#include "MainForm.h"
#include "FormManager.h"

using namespace Osp::App;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Base;
using namespace Osp::Graphics;

MainForm::MainForm(void):
		__pLeftItemBitmap(null),__pLogInBitmap(null)
{
}

MainForm::~MainForm(void)
{

}
result
MainForm::OnTerminating(void)
{
	result r = E_SUCCESS;
	delete __pLeftItemBitmap;
	delete __pLogInBitmap;
	return r;
}



bool
MainForm::Initialize(void)
{
	Construct(FORM_STYLE_NORMAL| FORM_STYLE_HEADER| FORM_STYLE_INDICATOR);
	return true;
}

result
MainForm::OnInitializing(void)
{
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();
	__pLeftItemBitmap = pAppResource->GetBitmapN(L"facebook_icon1.png");
	__pLogInBitmap   = pAppResource->GetBitmapN(L"facebook_login.png");

	ButtonItem  buttonLeftItem;
	buttonLeftItem.Construct(BUTTON_ITEM_STYLE_ICON, ID_HEADER_LEFTBUTTON);
	buttonLeftItem.SetIcon(BUTTON_ITEM_STATUS_NORMAL, __pLeftItemBitmap);

	Header* pHeader = GetHeader();
	pHeader->SetStyle(HEADER_STYLE_TITLE);
	pHeader->SetButton(BUTTON_POSITION_LEFT, buttonLeftItem);
	pHeader->SetTitleText(L"Bada FaceBook");


	Button* pLogin = new Button();
	pLogin->Construct(Rectangle(150,200, 150, 80), L" ");
	pLogin->SetNormalBackgroundBitmap(*__pLogInBitmap);
	pLogin->SetActionId(ID_BUTTON_LOGIN_CLICKED);
	pLogin->AddActionEventListener(*this);
	AddControl(*pLogin);

	return E_SUCCESS;
}

void
MainForm::OnActionPerformed(const Control& source, int actionId)
{
	switch (actionId)
	{
	case ID_BUTTON_LOGIN_CLICKED:
	{
		Frame *pFrame = Application::GetInstance()->GetAppFrame()->GetFrame();
		FormManager *pFormManager = static_cast<FormManager *>(pFrame->GetControl(L"FormManager"));
		if (pFormManager == null)
			return;
		pFormManager->SendUserEvent(FormManager::REQUEST_FACEBOOKLOGINFORM, null);
		break;
	}
	default:
		break;


	}
}






