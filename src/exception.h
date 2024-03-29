/**
 * Copyright (C) 2021-2022 Justus Languell
 * This file is part of Capstone, which is licensed under the MIT license.
 * For more details, see ./license.txt or write <jus@gtsbr.org>.
 */
#ifndef CAPSTONE_EXCEPTION
#define CAPSTONE_EXCEPTION

#include "common.h"

// Could be struct?
class Exception {
  public:
    const std::string text;
    Exception(const std::string& text) : text(text) {
    }
};

#endif