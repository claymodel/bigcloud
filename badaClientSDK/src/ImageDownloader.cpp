
#include "ImageDownloader.h"



using namespace Osp::Base;
using namespace Osp::Net::Http;
using namespace Osp::Media;
using namespace Osp::Graphics;
using namespace Osp::Base::Collection;

LinkedList imageBitmap;


ImageDownloader::ImageDownloader(void):
				__pBuffer(null)
{

}

ImageDownloader::~ImageDownloader(void)
{
	if(__pBuffer)
	{
		delete __pBuffer;
		__pBuffer = null;

	}

}

void
ImageDownloader::SendRequestGet(String requestUrl)
{
	result r = E_SUCCESS;
	HttpTransaction* pTransaction = null;
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
ImageDownloader::OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen)
{

	AppLogDebug("ImageDownloader::availableBodyLen=%d",availableBodyLen);

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();

	if(pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		__pBuffer = pHttpResponse->ReadBodyN();
	}

	Image *pFriendsImage;
	pFriendsImage = new Image();
	pFriendsImage->Construct();
	Bitmap *pBitmap;
	pBitmap=pFriendsImage->DecodeN(*__pBuffer,IMG_FORMAT_JPG,BITMAP_PIXEL_FORMAT_ARGB8888,100,100);
	imageBitmap.Add(*pBitmap);
	delete pFriendsImage;

}

void
ImageDownloader::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{
	AppLog("OnTransactionAborted\n");

	delete &httpTransaction;
}

void
ImageDownloader::OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize)
{
	AppLog("OnTransactionReadyToWrite\n");
}

void
ImageDownloader::OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool rs)
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

}

void
ImageDownloader::OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction)
{
	AppLog("OnTransactionCompleted\n");

	AppLog("ImageDownloader::Closing Transaction");
	__pSession->CloseTransaction(httpTransaction);
	delete __pSession;
	__pSession = null;

	FriendsForm*  __pFriendsForm = new FriendsForm();
	__pFriendsForm->DisplayFriendsImage();
	delete __pFriendsForm;
	__pFriendsForm = null;

	delete &httpTransaction;
}

void
ImageDownloader::OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Osp::Base::String* pCert)
{
	httpTransaction.Resume();
	AppLog("OnTransactionCertVerificationRequiredN\n");
}










