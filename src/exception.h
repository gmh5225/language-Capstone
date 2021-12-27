// Copyright (C) 2021 Justus Languell - All Rights Reserved.
//
// This file is part of Bounce which is released under the MIT license.
// See file LICENSE or go write <jus@gtsbr.org> for full license details.

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