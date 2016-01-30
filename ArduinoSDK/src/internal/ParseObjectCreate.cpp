#include "ParseInternal.h"
#include "ParseClient.h"
#include "ParseObjectCreate.h"

void ParseObjectCreate::addKey(const char* key) {
	if (isBodySet) {
		return;
	}
	if (!requestBody.equals("{")) {
		requestBody += ",";
	}
	requestBody += "\"";
	requestBody += key;
	requestBody += "\"";
	requestBody += ":";
}

void ParseObjectCreate::add(const char* key, int d) {
	addKey(key);
	requestBody += d;
}

void ParseObjectCreate::add(const char* key, double d) {
	addKey(key);
	requestBody += d;
}

void ParseObjectCreate::add(const char* key, bool b) {
	addKey(key);
	if(b) {
		requestBody += "true";
	} else {
		requestBody += "false";
	}
}

void ParseObjectCreate::add(const char* key, const char* s) {
	addKey(key);
	requestBody += "\"";
	requestBody += s;
	requestBody += "\"";
}

void ParseObjectCreate::addGeoPoint(const char* key, double lat, double lon) {
	addKey(key);
	String geoPoint = "{\"__type\":\"GeoPoint\",\"latitude\":";
	geoPoint += lat;
	geoPoint += ",\"longitude\":";
	geoPoint += lon;
	geoPoint += "}";
  requestBody += geoPoint;
}

void ParseObjectCreate::addJSONValue(const char* key, const char* json) {
	addKey(key);
	requestBody += json;
}

void ParseObjectCreate::addJSONValue(const char* key, const String& json) {
	addKey(key);
	requestBody += json;
}

void ParseObjectCreate::setJSONBody(const char* jsonBody) {
	requestBody = jsonBody;
	isBodySet = true;
}

void ParseObjectCreate::setJSONBody(const String& jsonBody) {
	requestBody = jsonBody;
	isBodySet = true;
}

ParseResponse ParseObjectCreate::send() {
	if (!isBodySet) {
		requestBody += "}";
	}
	return Parse.sendRequest("POST", httpPath, requestBody, "");
}
