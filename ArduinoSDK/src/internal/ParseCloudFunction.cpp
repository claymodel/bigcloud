#include "ParseCloudFunction.h"

ParseCloudFunction::ParseCloudFunction() : ParseObjectCreate() {
}

void ParseCloudFunction::setFunctionName(const char* function) {
	httpPath += "/functions/";
	httpPath += function;
}
