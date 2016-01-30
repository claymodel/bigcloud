#ifndef ParseRequest_h
#define ParseRequest_h

#include "ParseInternal.h"
#include "ParseResponse.h"

/*! \file ParseRequest.h
 *  \brief ParseRequest object for the Yun
 *  include Parse.h, not this file
 */

/*! \class ParseRequest
 *  \brief Class responsible for Parse requests
 */
class ParseRequest {
protected:
	String httpPath;
	String requestBody;
	bool isBodySet;
public:
  /*! \fn ParseRequest()
   *  \brief Constructor of ParseRequest object
   */
  ParseRequest();

  /*! \fn ~ParseRequest()
   *  \brief Destructor of ParseRequest object
   */
  ~ParseRequest();

  /*! \fn void setClassName(const char* className)
   *  \brief set the ParseObject class name in which request will be performed.
   *
   * NOTE: ONLY use when request(GET/UPDATE/DELETE/CREATE/QUERY) is related with object
   *       for system object "Installation/User/Role", use "_Installatoin/_User/_Role" for className
   *       DO NOT setClassName for ParseCloudFunction, use setFunctionName
   *       DO NOT setClassName for ParseCloudFunction, use setEventName
   *
   *  \param className class name.
   */
  void setClassName(const char* className);

  /*! \fn void setObjectId(const char* objectId)
   *  \brief set the ParseObject object id in which request will be performed.
   *
   * NOTE: ONLY setObjectId for GET/UPDATE/DELETE request on a specific object
   *       for CREATE request, a new object id will be return on response
   *
   *  \param objectId object id.
   */
  void setObjectId(const char* objectId);

  /*! \fn virtual ParseResponse send()
   *  \brief execute the parse request.
   *
   *  \result response of request
   */
  virtual ParseResponse send() = 0;
};

#endif
