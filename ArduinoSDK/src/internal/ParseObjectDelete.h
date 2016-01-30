#ifndef ParseObjectDelete_h
#define ParseObjectDelete_h

#include "ParseRequest.h"

/*! \file ParseObjectDelete.h
 *  \brief ParseObjectDelete object for the Yun
 *  include Parse.h, not this file
 */

/*! \class ParseObjectDelete
 *  \brief Class responsible for object deletion.
 */
class ParseObjectDelete : public ParseRequest {
public:
  /*! \fn ParseObjectDelete()
   *  \brief Constructor of ParseObjectDelete object
   */
  ParseObjectDelete();

  /*! \fn ParseResponse send() override
   *  \brief launch the object deletion request and execute.
   *
   *  \return  response of request.
   */
  ParseResponse send();
};

#endif
