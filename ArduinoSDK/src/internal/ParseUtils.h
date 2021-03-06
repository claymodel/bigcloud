#ifndef Utils_h
#define Utils_h

/*! \file ParseUtils.h
 *  \brief ParseUtils object for the Yun
 *  include Parse.h, not this file
 */

/*! \class ParseUtils
 *  \brief Utility class.
 */
class ParseUtils {
public:
  /*! \fn static int getStringFromJSON(const char* data, const char *key, char* value, int size)
   *  \brief A very lightweight JSON parser to get the string value by key
   *
   *  \param data - JSON string to parse
   *  \param key - key to find
   *  \param value - returned value (always as string) or NULL if just to check values'presence
   *  \param size - size of the return buffer
   *  \result 1 if found 0 otherwise
   */
  static int getStringFromJSON(const char* data, const char *key, char* value, int size) {
    const char* found = NULL;
    int inString = 0;
    int json = 0;
    int backslash = 0;
    int endWithQuote = 1;
    int keyLen = strlen(key);
    int escape = 0;
    int jsonLevel = 0;
    if (!data || !key || !*key)
         return 0;

    for (found = data; *found; ++found) {
      if (strncmp(found, key, keyLen)) {
        switch (*found) {
          case '\"':
          inString = 1 - inString;
          break;
          case '\\':
          if (inString) {
            if (*(found + 1))
              ++found;
          }
          break;
          case '{':
          case '[': // treat array as JSON
          if (!inString) {
            ++jsonLevel;
          }
          break;
          case '}':
          case ']':
          if (!inString) {
            --jsonLevel;
            if (jsonLevel < 0) {
              // Quit on malformed json
              return 0;
            }
          }
          break;
        }
        continue;
      } else if (jsonLevel > 1) {
        continue;
      }
      found += keyLen;
      if (*found != '\"') {
        data = found;
        continue;
      } else {
        ++found;
        for (; *found == ' ' || *found == '\t'; ++found);
        if (*found != ':') {
          data = found;
          continue;
        }
        ++found;
        for (; *found == ' ' || *found == '\t'; ++found);
        if (*found == '{' || *found == '[') {
          json = 1;
          endWithQuote = 0;
        } else {
          if (*found != '\"')
            endWithQuote = 0;
        }
        break;
      }
    }
    if (!*found)
      return 0;
    if (!value)
      return 1;
    if (endWithQuote)
      ++found;
    inString = !json;
    jsonLevel = 0;
    for (; size > 1 && *found; --size, ++found, ++value) {
      if (backslash) {
        backslash = 0;
        *value = *found;
        continue;
      }
      switch (*found) {
        case '{':
        case '[':
        if (!inString)
          ++jsonLevel;
        break;
        case '}':
        case ']':
        if (!endWithQuote) {
          if (!inString) {
            if (jsonLevel) {
              --jsonLevel;
              if (jsonLevel < 0) {
                // Quit on malformed json
                return 0;
              }
            }
            if (!jsonLevel) {
              if (json) {
                *value = *found;
                ++value;
              }
              goto RETURN_VALUE;
            }
            break;
          }
          // Fall through
        } else {
          // Fall through
        }
        case '\"':
        case ',':
        if (endWithQuote && *found != '\"')
          break;
        if (*found == '\"') //if(!json)
          inString = 1 - inString;
        if (!json)
          goto RETURN_VALUE;
        break;
        case '\\': backslash = 1; break;
      }
      *value = *found;
    }
    RETURN_VALUE:
    *value = 0;
    if (!*found) // malformed JSON
      return 0;
    return 1;
  }

  /*! \fn static int getIntFromJSON(const char* data, const char* key)
   *  \brief A very lightweight JSON parser to get the integer value by key
   *
   * \param data - JSON string to parse
   * \param key - key to find
   * \param value - returned integer value, 0 if key not found
   */
  static int getIntFromJSON(const char* data, const char* key) {
    char* value = new char[10];
    if (ParseUtils::getStringFromJSON(data, key, value, 10)) {
      String c = value;
      int v = c.toInt();
      delete[] value;
      return  v;
    }

    delete[] value;
    return 0;
  }

  /*! \fn static double getFloatFromJSON(const char* data, const char* key)
   *  \brief A very lightweight JSON parser to get the float value by key
   *
   * \param data - JSON string to parse
   * \param key - key to find
   * \param value - returned double value, .0 if key not found
   */
  static double getFloatFromJSON(const char* data, const char* key) {
    char* value = new char[10];
    if (ParseUtils::getStringFromJSON(data, key, value, 10)) {
      String c = value;
      double v = c.toFloat();
      delete[] value;
      return  v;
    }

    delete[] value;
    return .0;
  }

  /*! \fn static double getFloatFromJSON(const char* data, const char* key)
   *  \brief A very lightweight JSON parser to get the boolean value by key
   *
   * \param data - JSON string to parse
   * \param key - key to find
   * \param value - returned boolean value. false if key not found
   */
  static bool getBooleanFromJSON(const char* data, const char* key) {
    char* value = new char[10];
    if (ParseUtils::getStringFromJSON(data, key, value, 10)) {
      String c = value;
      bool v = (c == "true")?true:false;
      delete[] value;
      return v;
    }

    delete[] value;
    return false;
  }

  static bool isSanitizedString(const String& userData) {
    static char badChars[] = " \t\n\r";
    int k;
    int i;
    for(k = 0; k < userData.length(); k++) {
      for(i = 0; i < strlen(badChars); i++) {
        if(userData[k] == badChars[i]){
          return false;
        }
      }
    }
    return true;
  }
};

#endif
