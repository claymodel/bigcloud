#ifndef ParseCloudFunction_h
#define ParseCloudFunction_h

#include "ParseObjectCreate.h"

/*! \file ParseCloudFunction.h
 *  \brief ParseCloudFunction object for the Yun
 *  include Parse.h, not this file
 */

/*! \class ParseCloudFunction
 *  \brief Class responsible for cloud function
 */
class ParseCloudFunction : public ParseObjectCreate {
public:
  /*! \fn ParseCloudFunction()
   *  \brief Constructor of ParseCloudFunction object
   */
  ParseCloudFunction();

  /*! \fn void setFunctionName(const char* function)
   *  \brief Set the cloud funciton name to be called.
   *
   *  \param function Function name
   */
  void setFunctionName(const char* function);
};

#endif
