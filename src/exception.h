#ifndef BOUNCE_EXCEPTION
#define BOUNCE_EXCEPTION

#include "common.h"

class Exception {
  public:
    std::string text;
    Exception(const std::string& exceptionText) {
        text = exceptionText;
    }
};

#endif