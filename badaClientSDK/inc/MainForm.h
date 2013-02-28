#ifndef _MAINFORM_H_
#define _MAINFORM_H_

#include <FUi.h>

class MainForm :
	public Osp::Ui::Controls::Form,
	public Osp::Ui::IActionEventListener
{
private:
	Osp::Graphics::Bitmap* __pLeftItemBitmap;
	Osp::Graphics::Bitmap* __pLogInBitmap;

public:
	MainForm(void);
	virtual ~MainForm(void);

	bool Initialize(void);
	bool CreateMainList(void);
	bool MoveToForm(int index);

protected:
	Osp::Ui::Controls::ListView* __pList;
	static const int ID_USER_PROFILE = 501;
	static const int ID_GET_FRIENDS = 502;
	static const int ID_HEADER_LEFTBUTTON = 601;
	static const int ID_BUTTON_LOGIN_CLICKED =101;
public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

#endif
