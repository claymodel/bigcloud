#if defined (ARDUINO_ARCH_ESP8266)

#include "../ParsePush.h"
#include "../ParseClient.h"
#include "../ParseUtils.h"

ParsePush::ParsePush(WiFiClientSecure* pushClient) : ParseResponse(pushClient) {
    memset(lookahead, 0, sizeof(lookahead));
}

void ParsePush::close() {
    freeBuffer();
}

void ParsePush::setLookahead(const char *read_data) {
    strncpy(lookahead, read_data, sizeof(lookahead));
}

void ParsePush::read() {
    if (buf == NULL) {
        bufSize = BUFSIZE;
        buf = new char[bufSize];
    }

    if (p == bufSize - 1) {
        return;
    }

    if (p == 0) {
        memset(buf, 0, bufSize);
        for (; lookahead[p]; ++p)
            buf[p] = lookahead[p];
        lookahead[0] = 0;
    }

    char c;
    while (client->connected()) {
        if (client->available()) {
            c = client->read();
            if (c == '\n') c = '\0';
            if (p < bufSize - 1) {
                *(buf + p) = c;
                p++;
            }
            if (c == '\0') break;
        }
    }
    p = 0;
    char time[41];
    if (ParseUtils::getStringFromJSON(buf, "time", time, sizeof(time))) {
      Parse.saveLastPushTime(time);
    }
}

#endif // ARDUINO_ARCH_ESP8266

