#ifndef ParseTrackEvent_h
#define ParseTrackEvent_h

#include "ParseObjectCreate.h"

/*! \file ParseTrackEvent.h
 *  \brief ParseTrackEvent object for the Yun
 *  include Parse.h, not this file
 */

/*! \class ParseTrackEvent
 *  \brief Class responsible for tracking an event.
 */
class ParseTrackEvent : public ParseObjectCreate {
public:
  /*! \fn ParseTrackEvent()
   *  \brief Constructor of ParseTrackEvent object
   */
  ParseTrackEvent();

  /*! \fn void setEventName(const char* eventName)
   *  \brief set the track event name, which will be available in Parse Analytics
   *
   *  \param eventName event name.
   */
  void setEventName(const char* eventName);
};

#endif
