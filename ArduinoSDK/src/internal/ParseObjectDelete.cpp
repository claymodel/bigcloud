#include "ParseInternal.h"
#include "ParseClient.h"
#include "ParseObjectDelete.h"

ParseObjectDelete::ParseObjectDelete() : ParseRequest() {
}

ParseResponse ParseObjectDelete::send() {
	return Parse.sendRequest("DELETE", httpPath, "", "");
}
