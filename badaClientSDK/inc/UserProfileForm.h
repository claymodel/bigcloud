
#ifndef USERPROFILEFORM_H_
#define USERPROFILEFORM_H_

#include "BaseForm.h"
#include <FApp.h>
#include <FBase.h>
#include <FUi.h>
#include <FWeb.h>
#include <FNet.h>
#include <FWebJson.h>
#include <FMedia.h>

class UserProfileForm :
	public BaseForm,
	public Osp::Net::Http::IHttpTransactionEventListener

{
public:

	UserProfileForm(void);
	virtual ~UserProfileForm(void);

	bool Initialize(void);



public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	void OnTransactionReadyToRead(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int availableBodyLen);
	void OnTransactionAborted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, result r);
	void OnTransactionReadyToWrite(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int recommendedChunkSize);
	void OnTransactionHeaderCompleted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int headerLen, bool rs);
	void OnTransactionCompleted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction);
	void OnTransactionCertVerificationRequiredN(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, Osp::Base::String* pCert);
	void OnImageDecodeUrlReceived( RequestId reqId, Osp::Graphics::Bitmap* pBitmap, result r, const Osp::Base::String errorCode, const Osp::Base::String errorMessage);

	void SendRequestGet(Osp::Base::String requestUrl);
	void GetImage(const Osp::Base::String url);
	void ParseJsonData();


private:
	Osp::Graphics::Bitmap* __pLeftItemBitmap;
	Osp::Base::String __accessToken;
	Osp::Net::Http::HttpSession* 		__pSession;
	Osp::Base::String* __pProfileName;
	Osp::Media::Image *__pProfileImage;
	Osp::Base::ByteBuffer* __pBuffer;
    Osp::Net::Http::HttpTransaction* __pTransaction;
    static const int ID_BUTTON_FRIENDS_CLICKED = 101;
    static const int ID_BUTTON_LOGOUT_CLICKED =102;

};



#endif /* USERPROFILEFORM_H_ */
