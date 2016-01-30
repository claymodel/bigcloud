#ifndef ParseResponse_h
#define ParseResponse_h

#include "ConnectionClient.h"

/*! \file ParseResponse.h
 *  \brief ParseResponse object for the Yun
 *  include Parse.h, not this file
 */

/*! \class ParseResponse
 *  \brief Class that encapsulates Rest API response.
 *  This object created indirectly.
 */
class ParseResponse {
protected:
  const static int BUFSIZE = 128;
  int bufSize;
  char* buf;
  char* tmpBuf;
  bool isUserBuffer;
  int p;
  int resultCount;
#if defined (ARDUINO_SAMD_ZERO) || defined(ARDUINO_ARCH_ESP8266)
  long responseLength;
  bool isChunked;
  bool firstObject;
  bool dataDone;
  char chunkedBuffer[1024];
  int bufferPos;
  int lastRead;
#endif
  ConnectionClient* client;

  virtual void read();
  void readWithTimeout(int maxSec);

#if defined (ARDUINO_SAMD_ZERO) || defined(ARDUINO_ARCH_ESP8266)
  // Zero functions only - do nothing on Yun
  void readLine(char *buff, int sz);
  bool readJson(char *buff, int sz);
  bool readJsonInternal(char *buff, int sz, int *read_bytes, char started);
  int readChunkedData(int timeout);
  // End Zero only functions
#endif

  int available();
  void freeBuffer();

  ParseResponse(ConnectionClient* client);

public:
  /*! \fn ParseResponse()
   *  \brief Destructor of ParseResponse object
   */
  ~ParseResponse();

  /*! \fn void setBuffer(char* buffer, int size)
   *  \brief set the customer buffer for writing response data.
   *
   *  \param buffer - char array buffer
   *  \param size - size of buffer
   *                NOTE: if buffer is not set, a default size of 128
   *                will be initialized.
   */
  void setBuffer(char* buffer, int size);

  /*! \fn int getErrorCode()
   *  \brief get the "error" field in the response.
   *
   *  \result error_code when error happens, 0 when there is no error
   */
  int getErrorCode();

  /*! \fn int getInt(const char* key)
   *  \brief get the integer value in the response by key
   *
   *  \param key - key
   *  \result the value
   */
  int getInt(const char* key);

  /*! \fn double getDouble(const char* key)
   *  \brief get the double value in the response by key.
   *
   *  \param key - key
   *  \result the value
   */
  double getDouble(const char* key);

  /*! \fn bool getBoolean(const char* key)
   *  \brief get the boolean value in the response by key
   *
   *  \param key - key
   *  \result the value
   */
  bool getBoolean(const char* key);

  /*! \fn const char* getString(const char* key)
   *  \brief get the string value in the response by key
   *
   *  \param key - key
   *  \result the value
   */
  const char* getString(const char* key);

  /*! \fn const char* getJSONBody()
   *  \brief get the complete json value of response.
   *
   *  \result returned JSON
   */
  const char* getJSONBody();

  /*! \fn bool nextObject()
   *  \brief ParseQuery ONLY: iterate to next object in query result
   *
   *  it has to be called before any object opreation(getInt/String/Double)
   *  including the first object
   *  \result  true if successfully iterate to next object
   *           false if there is no more object
   */
  bool nextObject();

  /*! \fn int count()
   *  \brief ParseQuery ONLY: get the count of the objects in query results
   *
   *  NOTE: if the query resutls exceed 2048 bytes(query result buffer size),
   *        it will only return the count of objects in the buffer.
   *  NOTE2(Zero only): the returned count is approximation of the number of
   *        returned objects. If the server sends results multi-chunked, the
   *        count is an approximation of the objects in the first chunk
   *        (usually 4K-16K)
   *  \result number of objects in the result
   */
  int count();

  /*! \fn void close()
   *  \brief free resource including system data buffer and client
   *
   *  NOTE: the customer buffer will NOT be freed, you need
   *        to free them by yourself
   */
  void close();

  friend class ParseClient;
};

#endif
