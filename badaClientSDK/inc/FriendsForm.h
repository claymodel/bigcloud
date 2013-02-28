#ifndef FRIENDSFORM_H_
#define FRIENDSFORM_H_

#include "BaseForm.h"
#include "ImageDownloader.h"
#include <FApp.h>
#include <FBase.h>
#include <FUi.h>
#include <FWeb.h>
#include <FNet.h>
#include <FWebJson.h>
#include <FMedia.h>

class ImageDownloader;

class FriendsForm :
	public BaseForm,
	public Osp::Net::Http::IHttpTransactionEventListener,
	public Osp::Ui::Controls::IListViewItemEventListener,
	public Osp::Ui::Controls::IListViewItemProvider

{
public:

	FriendsForm(void);
	virtual ~FriendsForm(void);

	bool Initialize(void);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

	void OnTransactionReadyToRead(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int availableBodyLen);
	void OnTransactionAborted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, result r);
	void OnTransactionReadyToWrite(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int recommendedChunkSize);
	void OnTransactionHeaderCompleted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int headerLen, bool rs);
	void OnTransactionCompleted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction);
	void OnTransactionCertVerificationRequiredN(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, Osp::Base::String* pCert);
	void OnImageDecodeUrlReceived( RequestId reqId, Osp::Graphics::Bitmap* pBitmap, result r, const Osp::Base::String errorCode, const Osp::Base::String errorMessage);

	void SendRequestGet(Osp::Base::String requestUrl);
	void CreateFriendsImageUrl();
	void GetFriendsImage();
	void TraverseFunction(Osp::Web::Json::IJsonValue *pValue);
	void CreateListView();

	// IListViewItemEventListener
	virtual void OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state);
	virtual void OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status);
	virtual void OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction);
	virtual void OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback);

	//IListViewItemProvider
	virtual Osp::Ui::Controls::ListItemBase* CreateItem (int index, int itemWidth);
	virtual bool  DeleteItem (int index, Osp::Ui::Controls::ListItemBase *pItem, int itemWidth);
	virtual int GetItemCount(void);
	void DisplayFriendsImage();

private:
	Osp::Graphics::Bitmap* __pLeftItemBitmap;
	Osp::Base::String __accessToken;
	Osp::Net::Http::HttpSession* 		__pSession;
	Osp::Base::Collection::HashMap __hashMap;
	Osp::Base::ByteBuffer*__pBuffer;
	ImageDownloader* __pImageDownloader;


    static const int ID_FORMAT_STRING = 500;
    static const int ID_FORMAT_BITMAP = 501;


};


#endif /* FRIENDSFORM_H_ */
