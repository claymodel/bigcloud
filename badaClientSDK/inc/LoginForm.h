#ifndef _LOGINFORM_H_
#define _LOGINFORM_H_

#include "BaseForm.h"
#include <FApp.h>
#include <FBase.h>
#include <FUi.h>
#include <FWeb.h>
#include <FWebJson.h>

class LoginForm :
	public BaseForm,
	public Osp::Web::Controls::ILoadingListener
{
public:
	LoginForm(void);
	virtual ~LoginForm(void);
	bool Initialize(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
	Osp::Base::String Encode(const Osp::Base::String& param);
	virtual bool	OnHttpAuthenticationRequestedN(const Osp::Base::String& host, const Osp::Base::String& realm, const Osp::Web::Controls::AuthenticationChallenge& authentication);
	virtual void	OnHttpAuthenticationCanceled();
	virtual void	OnLoadingStarted(void);
	virtual void	OnLoadingCanceled(void);
	virtual void	OnLoadingErrorOccurred(Osp::Web::Controls::LoadingErrorType error, const Osp::Base::String &reason );
	virtual void	OnLoadingCompleted();
	virtual void	OnEstimatedProgress(int progress);
	virtual void	OnPageTitleReceived(const Osp::Base::String& title);
	virtual bool	OnLoadingRequested(const Osp::Base::String& url, Osp::Web::Controls::WebNavigationType type);
	virtual Osp::Web::Controls::DecisionPolicy OnWebDataReceived(const Osp::Base::String& mime, const Osp::Net::Http::HttpHeader& header);
	void ParseAccessToken();
	void SaveAccessToken();


private:
	Osp::Web::Controls::Web* __pWebView;
	Osp::Base::String __accessToken;


};

#endif
