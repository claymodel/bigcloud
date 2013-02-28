#ifndef IMAGEDOWNLOADER_H_
#define IMAGEDOWNLOADER_H_

#include "FriendsForm.h"
#include <FNet.h>
#include <FBase.h>
#include <FMedia.h>

class ImageDownloader :
	public Osp::Net::Http::IHttpTransactionEventListener
{

public:

	int SaveFile (Osp::Base::ByteBuffer* content);

public:
	ImageDownloader();
	~ImageDownloader();

public:

	void SendRequestGet(Osp::Base::String requestUrl);


	// IHttpTransactionEventListener
	void OnTransactionReadyToRead(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int availableBodyLen);
	void OnTransactionAborted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, result r);
	void OnTransactionReadyToWrite(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int recommendedChunkSize);
	void OnTransactionHeaderCompleted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int headerLen, bool rs);
	void OnTransactionCompleted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction);
	void OnTransactionCertVerificationRequiredN(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, Osp::Base::String* pCert);

private:
	Osp::Net::Http::HttpSession*	__pSession;
	Osp::Base::ByteBuffer* __pBuffer;


};
#endif /* IMAGEDOWNLOADER_H_ */
