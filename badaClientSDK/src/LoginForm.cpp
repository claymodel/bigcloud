#include "LoginForm.h"
#include "FormManager.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Ui;
using namespace Osp::Io;
using namespace Osp::Ui::Controls;
using namespace Osp::Web::Controls;
using namespace Osp::Web::Json;
using namespace Osp::Base::Utility;
using namespace Osp::Graphics;

LoginForm::LoginForm(void)
{
}

LoginForm::~LoginForm(void)
{
}

bool
LoginForm::Initialize(void)
{
	Construct(FORM_STYLE_NORMAL | FORM_STYLE_INDICATOR|FORM_STYLE_FOOTER);
	SetFooter();

	return true;
}


result LoginForm::OnTerminating(void)
{
	return E_SUCCESS;
}

result
LoginForm::OnInitializing(void)
{
	BaseForm::OnInitializing();
	result r = E_SUCCESS;

	Rectangle rect = GetClientAreaBounds();

	__pWebView = new Web();
	__pWebView->Construct(Rectangle(0,0,rect.width,rect.height));
	__pWebView->SetLoadingListener(this);
	AddControl(*__pWebView);
	__pWebView->SetName(L"IDC_WEBVIEW");

	String FACEBOOK_AUTH_URL = L"https://www.facebook.com/dialog/oauth/";
	String FACEBOOK_CALLBACK = L"http://www.facebook.com/connect/login_success.html";
	String FACEBOOK_APP_ID = L"117454378375941";

	String query = "";
	query += L"client_id=" + Encode(FACEBOOK_APP_ID);
	query += L"&redirect_uri=" + Encode(FACEBOOK_CALLBACK);
	query += L"&display=touch&scope=publish_stream,offline_access,user_photos&response_type=token";

	Uri uri;
	uri.SetUri(FACEBOOK_AUTH_URL);
	uri.SetQuery(query);

	AppLogDebug("%ls",uri.ToString().GetPointer());

	r = __pWebView->ClearCache();
	if(r!=E_SUCCESS)
		AppLog("Failed in cache [%d][%S]",r,GetErrorMessage(r));

	r = __pWebView->ClearCookie();
	if(r!=E_SUCCESS)
		AppLog("Failed in clearing the cookie [%d][%S]",r,GetErrorMessage(r));

	__pWebView->LoadUrl(uri.ToString());

	return r;
}


String LoginForm::Encode(const String& param)
{

	String tmp;

	UrlEncoder::Encode(param,"UTF-8",tmp);

	return tmp;
}


void
LoginForm::ParseAccessToken()
{
	Osp::Base::String url(L"");
	url = __pWebView->GetUrl();
	int index = -1;

	url.IndexOf(L"login_success.html", 0, index);
	if( index >= 0)
	{
		url.IndexOf(L"access_token", 0, index);
		if(index > 0)
		{
			int end = 0;
			url.IndexOf("&",index, end);
			if(end > index)
			{
				url.SubString(index + 13, end - (index + 13), __accessToken);
			}
		}
		else
		{
			__accessToken = "Logon failed";
			return;
		}
		AppLogDebug("Access Token:%ls",__accessToken.GetPointer());
	}
	SaveAccessToken();

	Frame *pFrame = Application::GetInstance()->GetAppFrame()->GetFrame();
	FormManager *pFormManager = static_cast<FormManager *>(pFrame->GetControl(L"FormManager"));
	if (pFormManager == null)
		return;
	pFormManager->SendUserEvent(FormManager::REQUEST_USERPROFILEFORM, null);

}

void
LoginForm::SaveAccessToken()
{
	Registry reg;
	result r = reg.Construct(L"/Home/FacebookReg.ini", true);
	if(r == E_SUCCESS)
	{
		String access_token = __accessToken;
		String section = L"Facebook";
		String entry = L"AccessToken";
		r = reg.AddSection(section);
		if(r == E_SUCCESS || r == E_SECTION_ALREADY_EXIST)
		{
			r = reg.AddValue(section, entry , access_token);
			if(r == E_SUCCESS)
			{
				AppLog("Access_token stored in registry");
				reg.Flush();
			}
			else
			{
				AppLog("Storing fails: %s", GetErrorMessage(r));
				r = reg.SetValue(section, entry , access_token);
				if(r == E_SUCCESS)
				{
					AppLog("Access_token stored in registry");
					reg.Flush();
				}
				else
				{
					AppLog("Storing failes: %s", GetErrorMessage(r));
				}
			}
		}
		else
		{
			AppLog("Section creation failed: %s", GetErrorMessage(r));
		}
	}
	else
	{
		AppLog("File open failes: %s", GetErrorMessage(r));
	}

	String token;
	String section = L"Facebook";
	String entry = L"AccessToken";
	r = reg.GetValue(section, entry , token);

}

bool
LoginForm::OnHttpAuthenticationRequestedN(const Osp::Base::String& host, const Osp::Base::String& realm, const Osp::Web::Controls::AuthenticationChallenge& authentication)
{
	return true;

}

void
LoginForm::OnHttpAuthenticationCanceled(void)
{

}

void
LoginForm::OnLoadingStarted(void)
{
	AppLogDebug("Loading started");
}

void
LoginForm::OnLoadingCanceled(void)
{
	AppLogDebug("Loading canceled");

}

void
LoginForm::OnLoadingErrorOccurred(LoadingErrorType error, const Osp::Base::String& reason)
{
	AppLogDebug("Loading error\n");
	AppLog("[%d][%S]",error,GetErrorMessage(error));
}

void
LoginForm::OnLoadingCompleted(void)
{
	String resUrl = __pWebView->GetUrl();
	int index = -1;

	resUrl.IndexOf(L"facebook.com/connect/uiserver.php", 0, index);
	if( index >= 0)
	{
		String * temp = __pWebView->EvaluateJavascriptN("document.getElementsByName('grant_clicked')[0].click();");
		delete temp;
	}
	else
	{
		resUrl.IndexOf(L"facebook.com/connect/login_success.html", 0, index);
		if(index >= 0)
		{
			AppLog("Login success");
			ParseAccessToken();
		}
	}
}


void
LoginForm::OnEstimatedProgress(int progress)
{

}

void
LoginForm::OnPageTitleReceived(const Osp::Base::String& title)
{
	AppLogDebug("OnPageTitleReceived:%ls",title.GetPointer());
}

bool
LoginForm::OnLoadingRequested(const Osp::Base::String& url, WebNavigationType type)
{
	return false;
}

DecisionPolicy LoginForm::OnWebDataReceived(const Osp::Base::String& mime, const Osp::Net::Http::HttpHeader& httpHeader)
{
	return WEB_DECISION_CONTINUE;
}










