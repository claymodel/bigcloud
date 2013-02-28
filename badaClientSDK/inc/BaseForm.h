#ifndef _BASEFORM_H_
#define _BASEFORM_H_

#include <FUi.h>
#include <FGraphics.h>
#include <FMedia.h>
#include <FSystem.h>

class BaseForm :
	public Osp::Ui::Controls::Form,
	public Osp::Ui::IActionEventListener,
	public Osp::Ui::Controls::IFormBackEventListener
{
public:
	BaseForm(void);
	virtual ~BaseForm(void);

	virtual bool Initialize(void) = 0;
	bool SetFooter(void);
	static const int ID_EXIT = 100;

public:
	virtual result OnInitializing(void);
	void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
	void OnFormBackRequested(Osp::Ui::Controls::Form& source) ;
};

#endif
