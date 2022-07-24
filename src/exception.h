/**
 * Copyright (C) 2021-2022 Justus Languell
 * This file is part of Capstone, which is licensed under the MIT license.
 * For more details, see ./license.txt or write <jus@gtsbr.org>.
 */
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