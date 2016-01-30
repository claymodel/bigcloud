#if defined (ARDUINO_AVR_YUN)

#include "../ParseInternal.h"
#include "../ParseClient.h"
#include "../ParseResponse.h"
#include "../ParsePlatformSupport.h"

ParseResponse::ParseResponse(Process* client) {
  p = 0;
  buf = NULL;
  tmpBuf = NULL;
  bufSize = 0;
  isUserBuffer = false;
  this->client = client;
}

ParseResponse::~ParseResponse() {
  close();
}

void ParseResponse::setBuffer(char* buffer, int size) {
  if(!buffer || size <= 0) {
    return;
  }

  buf = buffer;
  bufSize = size;
  isUserBuffer = true;
  memset(buf, 0, bufSize);
}

int ParseResponse::available() {
  return client->available();
}

void ParseResponse::read() {
  if(buf == NULL) {
    bufSize = BUFSIZE;
    buf = new char[bufSize];
  }
  p += ParsePlatformSupport::read(client, buf + p, bufSize - p);
}

int ParseResponse::getErrorCode() {
  return getInt("code");
}

const char* ParseResponse::getJSONBody() {
  read();
  return buf;
}

const char* ParseResponse::getString(const char* key) {
  read();
  if(!tmpBuf) {
    tmpBuf = new char[64];
  }
  memset(tmpBuf, 0, 64);
  ParseUtils::getStringFromJSON(buf, key, tmpBuf, 64);
  return tmpBuf;
}

int ParseResponse::getInt(const char* key) {
  read();
  return ParseUtils::getIntFromJSON(buf, key);
}

double ParseResponse::getDouble(const char* key) {
  read();
  return ParseUtils::getFloatFromJSON(buf, key);
}

bool ParseResponse::getBoolean(const char* key) {
  read();
  return ParseUtils::getBooleanFromJSON(buf, key);
}

void ParseResponse::readWithTimeout(int maxSec) {
  while((!available()) && (maxSec--)) { // wait till response
    delay(1000);
  }
  read();
}

bool ParseResponse::nextObject(){
  if(resultCount <= 0) {
    count();
  }

  if(resultCount <= 0) {
    return false;
  }

  client->write('n');
  // reset buffer and read next object
  p = 0;
  memset(buf, 0, bufSize);
  readWithTimeout(5);

  if (*buf) {
    return true;
  } else {
    return false;
  }
}

int ParseResponse::count() {
  client->write('c');

  // reset buffer and read count
  p = 0;
  memset(buf, 0, bufSize);
  readWithTimeout(30);
  read();

  String c = buf;
  int count = c.toInt();
  resultCount = count;
  return count;
}

void  ParseResponse::freeBuffer() {
  if (!isUserBuffer && buf) { // only free non-user buffer
    delete[] buf;
    buf = NULL;
  }
  if (tmpBuf) {
    delete[] tmpBuf;
    tmpBuf = NULL;
  }
}

void ParseResponse::close() {
  if (client && client != &Parse.pushClient) {
    client->close();
  }
  freeBuffer();
}

#endif
