#ifndef ParsePush_h
#define ParsePush_h

#include "ConnectionClient.h"
#include "ParseResponse.h"

/*! \file ParsePush.h
 *  \brief ParsePush object for the Yun
 *  include Parse.h, not this file
 */

/*! \class ParsePush
 *  \brief Class with a push. Not created directly.
 */
class ParsePush : public ParseResponse {
protected:
  ParsePush(ConnectionClient* pushClient);

#if defined (ARDUINO_SAMD_ZERO) || defined(ARDUINO_ARCH_ESP8266)
  char lookahead[5];
  void setLookahead(const char *read_data);
  void read();
#endif  // defined (ARDUINO_SAMD_ZERO)

public:

  /*! \fn void close()
   *  \brief free resource including data buffer.
   *
   */
  void close();

  friend class ParseClient;
};

#endif
