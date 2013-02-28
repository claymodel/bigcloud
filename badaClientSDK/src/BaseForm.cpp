
#include "BaseForm.h"
#include "FormManager.h"

using namespace Osp::App;
using namespace Osp::Media;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;
using namespace Osp::Base;

BaseForm::BaseForm(void)
{
}

BaseForm::~BaseForm(void)
{
}

bool
BaseForm::SetFooter(void)
{	
	Footer* pFooter = GetFooter();
	pFooter->SetStyle(FOOTER_STYLE_SEGMENTED_TEXT);
	pFooter->SetBackButton();

	return true;
}

void
BaseForm::OnActionPerformed(const Control& source, int actionId)
{
	// Get the frame pointer
	Frame* pFrame = Application::GetInstance()->GetAppFrame()->GetFrame();

	switch (actionId)
	{	
	case ID_EXIT:
		FormManager* pFormManager = static_cast<FormManager *>(pFrame->GetControl(L"FormManager"));
		if (pFormManager != null)
			pFormManager->SendUserEvent(FormManager::REQUEST_MAINFORM, null);
		break;
	}
}

result
BaseForm::OnInitializing(void)
{
	result r = E_SUCCESS;

	SetFormBackEventListener(this);

	return r;
}

void
BaseForm::OnFormBackRequested(Osp::Ui::Controls::Form& source)
{
	// Get the frame pointer
	Frame* pFrame = Application::GetInstance()->GetAppFrame()->GetFrame();
	FormManager* pFormManager = static_cast<FormManager *>(pFrame->GetControl(L"FormManager"));
	if (pFormManager != null)
		pFormManager->SendUserEvent(FormManager::REQUEST_MAINFORM, null);

}
