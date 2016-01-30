#include "ParseInternal.h"
#include "ParseClient.h"
#include "ParseObjectUpdate.h"

ParseObjectUpdate::ParseObjectUpdate() : ParseObjectCreate() {
}


ParseResponse ParseObjectUpdate::send() {
	if (!isBodySet) {
		requestBody += "}";
	}

	return Parse.sendRequest("PUT", httpPath, requestBody, "");
}
