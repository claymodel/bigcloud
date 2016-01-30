#ifdef ARDUINO_AVR_YUN

#include "../ParseClient.h"
#include "../ParsePush.h"

ParsePush::ParsePush(Process* pushClient) : ParseResponse(pushClient) {
}

void ParsePush::close() {
  // send signal to linux that the push is consumed.
  // iterate into next push
  client->write('n');
  freeBuffer();
}

#endif
