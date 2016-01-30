#include "ParseInternal.h"
#include "ParseClient.h"
#include "ParseQuery.h"

ParseQuery::ParseQuery() : ParseRequest() {
	whereClause = "";
	limit = -1;
	skip = -1;
	order = "";
	returnedFields = "";
}

long getDecimal(double v) {
  long decimal = 1000 * (v - int(v));
  return decimal > 0 ? decimal : -1 * decimal;
}

void ParseQuery::addConditionKey(const char* key) {
	if (whereClause == "") {
		whereClause += "{";
	} else {
		whereClause += ",";
	}

	whereClause += "\"";
	whereClause += key;
	whereClause += "\"";
	whereClause += ":";
}

void ParseQuery::addConditionNum(const char* key, const char* comparator, double v) {
	addConditionKey(key);
  String stringVal = String(int(v)) + "." + String(getDecimal(v));

	if (comparator == "$=") {
		whereClause += stringVal;
	} else {
		whereClause += "{\"";
		whereClause += comparator;
		whereClause += "\":";
		whereClause += stringVal;
		whereClause += "}";
	}
}

void ParseQuery::whereExists(const char* key) {
	addConditionKey(key);
	whereClause += "{\"exists\":true}";
}

void ParseQuery::whereDoesNotExist(const char* key) {
	addConditionKey(key);
	whereClause += "{\"exists\":false}";
}

void ParseQuery::whereEqualTo(const char* key, const char* v) {
	addConditionKey(key);
	whereClause += "\"";
	whereClause += v;
	whereClause += "\"";
}

void ParseQuery::whereNotEqualTo(const char* key, const char* v) {
	addConditionKey(key);
	whereClause += "{\"$ne\":\"";
	whereClause += v;
	whereClause += "\"}";
}

void ParseQuery::whereEqualTo(const char* key, bool v) {
	addConditionKey(key);
	whereClause += v?"true":"false";
}

void ParseQuery::whereNotEqualTo(const char* key, bool v) {
	addConditionKey(key);
	whereClause += "{\"$ne\":";
	whereClause += v?"true":"false";
	whereClause += "}";
}

void ParseQuery::whereEqualTo(const char* key, int v) {
	addConditionNum(key, "$=", v);
}

void ParseQuery::whereNotEqualTo(const char* key, int v) {
	addConditionNum(key, "$ne", v);
}

void ParseQuery::whereLessThan(const char* key, int v) {
	addConditionNum(key, "$lt", v);
}

void ParseQuery::whereGreaterThan(const char* key, int v) {
	addConditionNum(key, "$gt", v);
}

void ParseQuery::whereLessThanOrEqualTo(const char* key, int v) {
	addConditionNum(key, "$lte", v);
}

void ParseQuery::whereGreaterThanOrEqualTo(const char* key, int v) {
	addConditionNum(key, "$gte", v);
}

void ParseQuery::whereEqualTo(const char* key, double v) {
	addConditionNum(key, "$=", v);
}

void ParseQuery::whereNotEqualTo(const char* key, double v) {
	addConditionNum(key, "$ne", v);
}

void ParseQuery::whereLessThan(const char* key, double v) {
	addConditionNum(key, "$lt", v);
}

void ParseQuery::whereGreaterThan(const char* key, double v) {
	addConditionNum(key, "$gt", v);
}

void ParseQuery::whereLessThanOrEqualTo(const char* key, double v) {
	addConditionNum(key, "$lte", v);
}

void ParseQuery::whereGreaterThanOrEqualTo(const char* key, double v) {
	addConditionNum(key, "$gte", v);
}

void ParseQuery::setLimit(int n) {
	limit = n;
}

void ParseQuery::setSkip(int n) {
	skip = n;
}

void ParseQuery::orderBy(const char* key) {
	order = key;
}

void ParseQuery::setKeys(const char* keys) {
	returnedFields = keys;
}

ParseResponse ParseQuery::send() {
  String urlParameters = "";
  if (whereClause != "") {
    whereClause += "}"; // close where clause if there is any
    urlParameters += "where=";
    urlParameters += whereClause;
  } 
  
	if (limit>0) {
		urlParameters += "&limit=";
		urlParameters += limit;
	}
	if (skip>0) {
		urlParameters += "&skip=";
		urlParameters += skip;
	}
	if (order != "") {
		urlParameters += "&order=";
		urlParameters += order;
	}
	if (returnedFields != "") {
		urlParameters += "&keys=";
		urlParameters += returnedFields;
	}
	return Parse.sendRequest("GET", httpPath, "", urlParameters);
}

