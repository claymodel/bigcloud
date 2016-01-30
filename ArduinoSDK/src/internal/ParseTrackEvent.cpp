#include "ParseInternal.h"
#include "ParseTrackEvent.h"

ParseTrackEvent::ParseTrackEvent() : ParseObjectCreate() {
}

void ParseTrackEvent::setEventName(const char* eventName) {
  httpPath += "/events/";
  httpPath += eventName;
}
