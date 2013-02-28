#include "FormManager.h"
#include"BaseForm.h"
#include"LoginForm.h"
#include"UserProfileForm.h"
#include"FriendsForm.h"


using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;

FormManager::FormManager(void)
{
}

FormManager::~FormManager(void)
{
}

bool
FormManager::Initialize(void)
{
	result r = E_SUCCESS;
	r = Construct(FORM_STYLE_NORMAL);
	SetName(L"FormManager");

	return true;
}

bool FormManager::SetStarterForm(RequestId requestId, Osp::Base::Collection::IList* pArgs)
{
	Form* pCurrentForm = Application::GetInstance()->GetAppFrame()->GetFrame()->GetCurrentForm();

	if (pCurrentForm == this)
		SwitchToForm(requestId, pArgs);
	else
		return false;

	return true;
}

void FormManager::OnUserEventReceivedN(RequestId requestId, Osp::Base::Collection::IList* pArgs)
{
	SwitchToForm(requestId, pArgs);
}

void
FormManager::SwitchToForm(RequestId requestId, Osp::Base::Collection::IList* pArgs)
{
	Frame* pFrame = Application::GetInstance()->GetAppFrame()->GetFrame();
	BaseForm* pExeForm = null;

	switch (requestId)
	{
	case REQUEST_MAINFORM:
	{
		if (__pMainForm == null)
		{
			__pMainForm = new MainForm();
			__pMainForm->Initialize();
			pFrame->AddControl(*__pMainForm);
		}
		pFrame->SetCurrentForm(*__pMainForm);
		__pMainForm->Draw();
		__pMainForm->Show();
		if (__pPreviousForm != null)
		{
			if (__pPreviousForm != __pMainForm)
				pFrame->RemoveControl(*__pPreviousForm);
		}
		__pPreviousForm = __pMainForm;

		return;
	}
	break;

	case REQUEST_FACEBOOKLOGINFORM:

		pExeForm = new LoginForm();
		break;

	case REQUEST_USERPROFILEFORM:

		pExeForm = new UserProfileForm();
		break;


	case REQUEST_FRIENDSFORM:
		pExeForm = new FriendsForm();
		break;

	default:
		return;
		break;
	}

	pExeForm->Initialize();
	pFrame->AddControl(*pExeForm);
	pFrame->SetCurrentForm(*pExeForm);
	pExeForm->Draw();
	pExeForm->Show();

	if (__pPreviousForm != null)
	{
		if (__pPreviousForm != __pMainForm)
			pFrame->RemoveControl(*__pPreviousForm);
	}
	__pPreviousForm = pExeForm;

	return;
}


