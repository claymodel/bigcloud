#ifndef ParseObjectGet_h
#define ParseObjectGet_h

#include "ParseRequest.h"

/*! \file ParseObjectGet.h
 *  \brief ParseObjectGet object for the Yun
 *  include Parse.h, not this file
 */

/*! \class ParseObjectGet()
 *  \brief Class responsible for getting the object from Parse.
 */
class ParseObjectGet : public ParseRequest {
public:
  /*! \fn ParseObjectGet()
   *  \brief Constructor of ParseObjectGet object
   */
  ParseObjectGet();

  /*! \fn ParseResponse send() override
   *  \brief launch the get object request and execute.
   *
   *  \result response of request
   */
  ParseResponse send();
};

#endif
