#if defined (ARDUINO_AVR_YUN)

#include "../ConnectionClient.h"
#include "../ParsePlatformSupport.h"

int ParsePlatformSupport::read(ConnectionClient* client, char* buf, int len) {
  int p = 0;
  while (p < (len-1) && client->available()) {
    buf[p++] = client->read();
  }
  if (p > 0 && buf[p - 1] == '\n') {
      buf[p - 1] = '\0';
  } else {
    buf[p] = '\0';
  }
  return p;
}

#endif
