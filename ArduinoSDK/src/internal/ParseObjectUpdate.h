#ifndef ParseObjectUpdate_h
#define ParseObjectUpdate_h

#include "ParseObjectCreate.h"

/*! \file ParseObjectUpdate.h
 *  \brief ParseObjectUpdate object for the Yun
 *  include Parse.h, not this file
 */

/*! \class ParseObjectUpdate
 *  \brief Class responsible for object update
 */
class ParseObjectUpdate : public ParseObjectCreate {
public:
  /*! \fn ParseObjectUpdate()
   *  \brief Constructor of ParseObjectUpdate object
   */
  ParseObjectUpdate();

  /*! \fn ParseResponse send() override
   *  \brief launch the update object request and execute.
   *
   *  \result the response
   */
  ParseResponse send();
};

#endif
