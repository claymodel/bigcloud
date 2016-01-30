#include "ParseInternal.h"
#include "ParseRequest.h"

ParseRequest::ParseRequest() {
	requestBody = "{";
	httpPath = "/1/";
	isBodySet = false;
}

ParseRequest::~ParseRequest() {
}

void ParseRequest::setObjectId(const char* key) {
	httpPath += "/";
	httpPath += key;
}

void ParseRequest::setClassName(const char* className) {
	if (className == "_User") {
		httpPath += "users";
	} else if (className == "_Installation") {
		httpPath += "installations";
	} else if (className == "_Role") {
		httpPath += "roles";
	} else {
		httpPath += "classes/";
		httpPath += className;
	}
}
