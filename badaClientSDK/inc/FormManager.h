#ifndef _FORMMANAGER_H_
#define _FORMMANAGER_H_

#include <FApp.h>
#include <FBase.h>
#include <FUi.h>
#include <FSystem.h>
#include "MainForm.h"

class FormManager :
	public Osp::Ui::Controls::Form

{

// Construction
public:
	FormManager(void);
	virtual ~FormManager(void);
	bool Initialize(void);
	bool SetStarterForm(RequestId requestId, Osp::Base::Collection::IList* pArgs);
	static const RequestId REQUEST_MAINFORM = 100;
	static const RequestId REQUEST_FACEBOOKLOGINFORM =101;
	static const RequestId REQUEST_TWITTERLOGINFORM=102;
	static const RequestId REQUEST_USERPROFILEFORM=103;
	static const RequestId REQUEST_FRIENDSFORM=104;


// Implementation
protected:
	void SwitchToForm(RequestId requestId, Osp::Base::Collection::IList* pArgs);
	Osp::Ui::Controls::Form* __pPreviousForm;
	MainForm* __pMainForm;

public:
	virtual void OnUserEventReceivedN(RequestId requestId, Osp::Base::Collection::IList* pArgs);
};

#endif	//_FORMMANAGER_H_
