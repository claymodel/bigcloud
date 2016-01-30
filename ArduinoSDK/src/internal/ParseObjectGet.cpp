#include "ParseInternal.h"
#include "ParseClient.h"
#include "ParseObjectGet.h"

ParseObjectGet::ParseObjectGet() : ParseRequest() {
}

ParseResponse ParseObjectGet::send() {
	return Parse.sendRequest("GET", httpPath, "", "");
}
