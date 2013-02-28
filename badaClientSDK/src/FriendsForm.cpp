#include "FriendsForm.h"
#include "FormManager.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;
using namespace Osp::Net::Http;
using namespace Osp::Web::Json;
using namespace Osp::Base::Utility;
using namespace Osp::Io;

static Osp::Ui::Controls::ListView* __pList;
extern LinkedList imageBitmap;
static LinkedList   list;
static int cnt;


FriendsForm::FriendsForm(void):
			__pBuffer(null),__pImageDownloader(null)
{

}

FriendsForm::~FriendsForm(void)
{

}

bool
FriendsForm::Initialize(void)
{
	Construct(FORM_STYLE_NORMAL | FORM_STYLE_INDICATOR| FORM_STYLE_HEADER| FORM_STYLE_FOOTER);
	SetFooter();
	return true;
}

result FriendsForm::OnTerminating(void)
{
	if (__pSession)
	{
		delete __pSession;
		__pSession = null;
	}
	if(__pBuffer)
	{
		delete __pBuffer;
		__pBuffer = null;

	}
	if(__pImageDownloader)
	{
		delete __pImageDownloader;
		__pImageDownloader=null;
	}

	delete __pLeftItemBitmap;

	list.RemoveAll(true);
	__hashMap.RemoveAll(true);
	imageBitmap.RemoveAll(true);
	cnt=0;
	AppLog("FriendsForm::OnTerminating");
	return E_SUCCESS;
}

result
FriendsForm::OnInitializing(void)
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
	pHeader->SetTitleText(L"Your Friends");
	CreateListView();

	result r = E_SUCCESS;

	//Read Access Token

	Registry reg;
	r = reg.Construct(L"/Home/FacebookReg.ini", false );
	String token;
	String section = L"Facebook";
	String entry = L"AccessToken";
	r = reg.GetValue(section, entry , token);
	__accessToken =token;

	String friendsurl;
	friendsurl.Append(L"https://graph.facebook.com/me/friends?");
	friendsurl.Append(L"access_token=");
	friendsurl.Append(__accessToken);
	__hashMap.Construct();
	SendRequestGet(friendsurl);


	return r;
}

void
FriendsForm::SendRequestGet(String requestUrl)
{
	result r = E_SUCCESS;
	HttpTransaction* pTransaction = null;
	HttpRequest* pRequest = null;

	String hostAddr(L"https://graph.facebook.com");
	if(__pSession == null)
	{
		__pSession = new HttpSession();
		r = __pSession->Construct(NET_HTTP_SESSION_MODE_PIPELINING, null, hostAddr, null);
		if (IsFailed(r))
		{
			AppLog("Fail to HttpSession::Construct. [%s]", GetErrorMessage(r));
			return;
		}
		__pSession->SetAutoRedirectionEnabled(true);
	}
	pTransaction = __pSession->OpenTransactionN();
	if (pTransaction)
	{
		result r = pTransaction->AddHttpTransactionListener(*this);
		pRequest = pTransaction->GetRequest();
			if (pRequest)
			{
				pRequest->SetUri(requestUrl);
				pRequest->SetMethod(NET_HTTP_METHOD_GET);
				r = pTransaction->Submit();
				if(IsFailed(r))
				{
					AppLog("Fail to HttpRequest::Submit. [%s]", GetErrorMessage(r));
				}
			}
			else
			{
				delete pTransaction;
				pTransaction = null;
			}
	}
}

void
FriendsForm::TraverseFunction( IJsonValue *pValue )
{
	String	*pFriendId=null;
	String	*pFriendName=null;
	if( !pValue )
		return ;
	else
	{
		switch (pValue->GetType())
		{
		case JSON_TYPE_OBJECT:
		{
			JsonObject* pObject = static_cast<JsonObject*>(pValue);
			IMapEnumeratorT<const String*, IJsonValue*>*  pMapEnum = pObject->GetMapEnumeratorN();
			while( pMapEnum->MoveNext() == E_SUCCESS )
			{
				const String* pKey = null;
				IJsonValue* value = null;
				pMapEnum->GetKey( pKey );
				// Get value of the id key
				if(*pKey=="id")
				{
					JsonString *pKeyId = new JsonString("id");
					IJsonValue *pValId = null;
					pObject->GetValue(pKeyId, pValId);
					JsonString* pId = static_cast<JsonString*>(pValId);
					pFriendId = new String(*pId);
					delete pKeyId;
				}
				// Get value of the name key
				if(*pKey=="name")
				{
					JsonString *pKeyName = new JsonString("name");
					IJsonValue *pValName = null;
					pObject->GetValue(pKeyName, pValName);
					JsonString* pName = static_cast<JsonString*>(pValName);
					pFriendName = new String(*pName);
					delete pKeyName;
				}
				//Store them in a hashmap
				if(pFriendId && pFriendName)
				{
					__hashMap.Add(*pFriendId,*pFriendName);
					pFriendId =null;
					pFriendName =null;
				}
				pMapEnum->GetValue( value );
				TraverseFunction( value );
			}
			delete pMapEnum;
		}
		break;

		case JSON_TYPE_ARRAY:
		{
			JsonArray* pJsonArray = static_cast<JsonArray*>(pValue);
			pJsonArray->GetCount();
			IEnumeratorT<IJsonValue*>* pEnum = pJsonArray->GetEnumeratorN();
			while( pEnum->MoveNext() == E_SUCCESS )
			{
				IJsonValue* pValue = null;
				pEnum->GetCurrent( pValue );
				TraverseFunction( pValue );
			}
			delete pEnum;
		}
		break;
		default:
			break;
		}
	}
}

void
FriendsForm::CreateFriendsImageUrl()
{
	IMapEnumerator*     pMapEnum = __hashMap.GetMapEnumeratorN();

	while (pMapEnum->MoveNext() == E_SUCCESS)
	{

		String *pKey  = null;
		pKey = static_cast<String*> (pMapEnum->GetKey());

		//Read Access Token
		result r = E_SUCCESS;
		Registry reg;
		r = reg.Construct(L"/Home/FacebookReg.ini", false );
		String token;
		String section = L"Facebook";
		String entry = L"AccessToken";
		r = reg.GetValue(section, entry , token);

		__accessToken =token;

		String *pFriendImageurl =new String();
		pFriendImageurl->Append(L"https://graph.facebook.com/");
		pFriendImageurl->Append(*pKey);
		pFriendImageurl->Append(L"/picture?");
		pFriendImageurl->Append(L"access_token=");
		pFriendImageurl->Append(__accessToken);

		//Store Url in an list
		list.Add(*pFriendImageurl);
	}
	delete pMapEnum;
	GetFriendsImage();

}
void
FriendsForm::GetFriendsImage()
{
	IEnumerator*    pEnum = list.GetEnumeratorN();
	if(cnt>0 && cnt<=list.GetCount())
	{
		for(int i=0;i<cnt;i++)
			pEnum->MoveNext();
	}

	if(pEnum->MoveNext() == E_SUCCESS)
	{
		cnt++;
		String *pFriendImageurl = null;
		pFriendImageurl =dynamic_cast<String*>( pEnum->GetCurrent());

		AppLog("FriendImageurl is %ls",pFriendImageurl->GetPointer());

		__pImageDownloader =new ImageDownloader();
		__pImageDownloader->SendRequestGet(*pFriendImageurl);
	}

	delete pEnum;

}

void
FriendsForm::OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen)
{

	AppLogDebug("OnTransactionReadyToRead,..availableBodyLen=%d",availableBodyLen);
	HttpResponse* pHttpResponse = httpTransaction.GetResponse();

	if(pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		__pBuffer = pHttpResponse->ReadBodyN();
	}

	IJsonValue* pJson = JsonParser::ParseN(*__pBuffer);
	TraverseFunction(pJson);

	//clean up allocated jsonValues
	JsonType type = pJson->GetType();
	if( type == JSON_TYPE_OBJECT )
	{
		JsonObject *pObject = static_cast<JsonObject*>(pJson);
		pObject->RemoveAll(true);
	}
	if( type == JSON_TYPE_ARRAY )
	{
		JsonArray *pArray = static_cast<JsonArray*>(pJson);
		pArray->RemoveAll(true);
	}
	delete pJson;

}

void
FriendsForm::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{
	AppLog("OnTransactionAborted\n");
	if (!IsFailed(r))
	{
		AppLog("errno=%5d,Error=%s",r,GetErrorMessage(r));

	}
	delete &httpTransaction;
}

void
FriendsForm::OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize)
{
	AppLog("OnTransactionReadyToWrite\n");
}

void
FriendsForm::OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool rs)
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
FriendsForm::OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction)
{
	AppLog("OnTransactionCompleted\n");
	__pSession->CloseTransaction(httpTransaction);
	delete __pSession;
	__pSession = null;
	CreateFriendsImageUrl();

	delete &httpTransaction;
}

void
FriendsForm::OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Osp::Base::String* pCert)
{
	httpTransaction.Resume();
	AppLog("OnTransactionCertVerificationRequiredN\n");
}

void
FriendsForm::DisplayFriendsImage()
{

	if(__pList)
		__pList->RefreshList(0,LIST_REFRESH_TYPE_ITEM_ADD);
	GetFriendsImage();

}

void
FriendsForm::CreateListView()
{
	AppLog("FriendsForm::CreateListView()");
	__pList = new ListView();
	__pList->Construct(Rectangle(0, 0, 480, 640 ), true, false);
	__pList->SetItemProvider(*this);
	__pList->AddListViewItemEventListener(*this);
	AddControl(*__pList);

}

void
FriendsForm::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{

}

void
FriendsForm::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
}
void
FriendsForm::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{

}

void
FriendsForm::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{

}

int
FriendsForm::GetItemCount(void)
{
	return cnt;
}
Osp::Ui::Controls::ListItemBase*
FriendsForm::CreateItem(int index, int itemWidth)
{

	ListAnnexStyle style = LIST_ANNEX_STYLE_NORMAL;
	CustomItem* pItem = new CustomItem();
	AppLog("FriendsForm::CreateItem,cnt=%5d",cnt);

	String *pValue = null;

	IEnumerator*    pEnumBitmap = imageBitmap.GetEnumeratorN();
	IMapEnumerator* pEnumMap    =__hashMap.GetMapEnumeratorN();
	for (int i=0;i<cnt;i++ )
	{
		pEnumMap->MoveNext();
	}
	for(int i=0;i<imageBitmap.GetCount();i++)
		pEnumBitmap->MoveNext();

	Bitmap *pBitmap = null ;

	pBitmap =dynamic_cast<Bitmap*>( pEnumBitmap->GetCurrent());
	pValue = static_cast<String*> (pEnumMap->GetValue());

	delete pEnumBitmap;
	delete pEnumMap;

	pItem->Construct(Osp::Graphics::Dimension(itemWidth,101), style);
	pItem->AddElement(Rectangle(10, 10, 100, 100), ID_FORMAT_BITMAP, *pBitmap, null, null);
	pItem->AddElement(Rectangle(160,25, 300, 100), ID_FORMAT_STRING, *pValue, true);

	return pItem;
}

bool
FriendsForm::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}













