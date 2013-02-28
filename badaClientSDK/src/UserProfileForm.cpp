
#include "UserProfileForm.h"
#include "FormManager.h"


using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;
using namespace Osp::Net::Http;
using namespace Osp::Web::Json;
using namespace Osp::Media;
using namespace Osp::Base::Utility;
using namespace Osp::Io;

static int count = 0;

UserProfileForm::UserProfileForm(void):
				__pProfileName(null),__pBuffer(null),__pTransaction(null)
{
}

UserProfileForm::~UserProfileForm(void)
{
}

bool
UserProfileForm::Initialize(void)
{
	Construct(FORM_STYLE_NORMAL | FORM_STYLE_INDICATOR| FORM_STYLE_HEADER| FORM_STYLE_FOOTER);
	SetFooter();

	return true;
}


result UserProfileForm::OnTerminating(void)
{
	if (__pSession)
	{
		delete __pSession;
		__pSession = null;

	}

	if(__pProfileName)
	{
		delete __pProfileName;
		__pProfileName = null;
	}

	if(__pProfileImage)
	{
		delete __pProfileImage;
		__pProfileImage = null;

	}

	delete __pLeftItemBitmap;


	return E_SUCCESS;
}

result
UserProfileForm::OnInitializing(void)
{
	BaseForm::OnInitializing();

	AppResource* pAppResource = Application::GetInstance()->GetAppResource();
	__pLeftItemBitmap = pAppResource->GetBitmapN(L"facebook_icon1.png");

	ButtonItem  buttonLeftItem;
	buttonLeftItem.Construct(BUTTON_ITEM_STYLE_ICON,NULL);
	buttonLeftItem.SetIcon(BUTTON_ITEM_STATUS_NORMAL, __pLeftItemBitmap);

	Header* pHeader = GetHeader();
	pHeader->SetStyle(HEADER_STYLE_TITLE);
	pHeader->SetButton(BUTTON_POSITION_LEFT, buttonLeftItem);
	pHeader->SetTitleText(L"Your Profile Page");


	result r = E_SUCCESS;

	//Read Access Token

	Registry reg;
	r = reg.Construct(L"/Home/FacebookReg.ini", false );
	String token;
	String section = L"Facebook";
	String entry = L"AccessToken";
	r = reg.GetValue(section, entry , token);
	if(r == E_SUCCESS)
		AppLog("token is %ls",token.GetPointer());
	else
		AppLog("Reading failed: %s", GetErrorMessage(r));
	__accessToken =token;

	String profileurl;
	profileurl.Append(L"https://graph.facebook.com/me?");
	profileurl.Append(L"access_token=");
	profileurl.Append(__accessToken);
	SendRequestGet(profileurl);

	return r;
}

void
UserProfileForm::SendRequestGet(String requestUrl)
{
	result r = E_SUCCESS;
	HttpRequest* pRequest = null;
	String hostAddr(L"https://graph.facebook.com");

	if(__pSession == null)
	{
		__pSession = new HttpSession();
		r = __pSession->Construct(NET_HTTP_SESSION_MODE_NORMAL, null, hostAddr, null);
		if (IsFailed(r))
		{
			AppLog("Fail to HttpSession::Construct. [%s]", GetErrorMessage(r));
			return;
		}
		__pSession->SetAutoRedirectionEnabled(true);
	}


	__pTransaction = __pSession->OpenTransactionN();
	if (__pTransaction)
	{
		result r = __pTransaction->AddHttpTransactionListener(*this);

			pRequest = __pTransaction->GetRequest();
			if (pRequest)
			{
				pRequest->SetUri(requestUrl);
				pRequest->SetMethod(NET_HTTP_METHOD_GET);
				r = __pTransaction->Submit();
				AppLog("RequestUrl is =%ls",requestUrl.GetPointer());
				if(IsFailed(r))
				{
					AppLog("Fail to HttpRequest::Submit. [%s]", GetErrorMessage(r));
				}
			}
			else
			{
				delete __pTransaction;
				__pTransaction = null;
			}
	}
}

void
UserProfileForm::ParseJsonData()
{
	AppLogDebug("ParseJsonData");
	IJsonValue* pJson = JsonParser::ParseN(*__pBuffer);

	if (pJson != NULL)
	{
		JsonObject *pObject = static_cast<JsonObject*>(pJson);
		// Get value of the name key
		JsonString *pKeyName = new JsonString("name");
		IJsonValue *pValName = null;
		pObject->GetValue(pKeyName, pValName);

		if(pValName)
		{
			JsonString* pName = static_cast<JsonString*>(pValName);
			__pProfileName = new String(*pName);
			AppLog("Name=%ls",__pProfileName->GetPointer());
		}

		delete pKeyName;

	}

	else
	{
		AppLog("Json Parser Failed");

	}


	delete __pBuffer;
	__pBuffer = null;

}

void
UserProfileForm::OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen)
{

	AppLogDebug("OnTransactionReadyToRead,..availableBodyLen=%d",availableBodyLen);
	count++;

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();

	if(pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		__pBuffer = pHttpResponse->ReadBodyN();
	}
	if(__pProfileName==NULL)
	{
		ParseJsonData();
		Label *pLabel = new Label();
		String labelText(L"Welcome ");
		labelText.Append(*__pProfileName);
		AppLog("labelText is =%ls",labelText.GetPointer());
		pLabel->Construct(Rectangle(05, 50, 500, 100), labelText);
		AddControl(*pLabel);
		Draw();
		Show();

		__pSession->CloseTransaction(httpTransaction);
		delete __pSession;
		__pSession = null;
	}

	//Get Profile Picture

	String imageurl;
	imageurl.Append(L"https://graph.facebook.com/me");
	imageurl.Append(L"/picture?");
	imageurl.Append(L"access_token=");
	imageurl.Append(__accessToken);
	SendRequestGet(imageurl);

	__pProfileImage = new Image();
	__pProfileImage->Construct();

	Bitmap *pBitmap;
	pBitmap=__pProfileImage->DecodeN(*__pBuffer,IMG_FORMAT_JPG,BITMAP_PIXEL_FORMAT_ARGB8888,150,150);
	Label *pImageLabel = new Label();
	pImageLabel->Construct(Rectangle(150,150, 150, 150),"");
	pImageLabel->SetBackgroundBitmap(*pBitmap);
	AddControl(*pImageLabel);

	Button* pGetFriends = new Button();
	pGetFriends->Construct(Rectangle(30,330, 350, 70), L"Get Friends");
	pGetFriends->SetActionId(ID_BUTTON_FRIENDS_CLICKED);
	pGetFriends->AddActionEventListener(*this);
	AddControl(*pGetFriends);

	Button* pLogout = new Button();
	pLogout->Construct(Rectangle(30,430, 350, 70), L"Logout");
	pLogout->SetActionId(ID_BUTTON_LOGOUT_CLICKED);
	pLogout->AddActionEventListener(*this);
	AddControl(*pLogout);
	Draw();
	Show();

}

void
UserProfileForm::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{
	AppLog("OnTransactionAborted\n");
	delete &httpTransaction;
}

void
UserProfileForm::OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize)
{
	AppLog("OnTransactionReadyToWrite\n");
}

void
UserProfileForm::OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool rs)
{

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();
	if(pHttpResponse == null)
		return;
	HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
	if(pHttpHeader != null)
	{
		String* tempHeaderString = pHttpHeader->GetRawHeaderN();
		AppLog("HeaderString=%ls\n",tempHeaderString->GetPointer());
	}

	AppLog("OnTransactionHeaderCompleted\n");
}

void
UserProfileForm::OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction)
{
	AppLog("UserProfileForm::OnTransactionCompleted\n");

	__pSession->CloseTransaction(httpTransaction);
	delete __pSession;
	__pSession = null;
	delete &httpTransaction;
}

void
UserProfileForm::OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Osp::Base::String* pCert)
{
	httpTransaction.Resume();
	AppLog("OnTransactionCertVerificationRequiredN\n");
}


void
UserProfileForm::OnActionPerformed(const Control& source, int actionId)
{
	Frame *pFrame = Application::GetInstance()->GetAppFrame()->GetFrame();
	FormManager *pFormManager = static_cast<FormManager *>(pFrame->GetControl(L"FormManager"));
	switch (actionId)
	{
	case ID_BUTTON_FRIENDS_CLICKED:

		if (pFormManager == null)
			return;
		pFormManager->SendUserEvent(FormManager::REQUEST_FRIENDSFORM, null);
		break;

	case ID_BUTTON_LOGOUT_CLICKED:

		if (pFormManager == null)
			return;
		pFormManager->SendUserEvent(FormManager::REQUEST_MAINFORM, null);
		break;
	}
}










