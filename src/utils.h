// Copyright (C) 2021 Justus Languell - All Rights Reserved.
//
// This file is part of Bounce which is released under the MIT license.
// See file LICENSE or go write <jus@gtsbr.org> for full license details.

#ifndef BOUNCE_UTILS
#define BOUNCE_UTILS

#include "common.h"

bool isWhitespace(char ch);
bool isNumeric(char ch);
bool isNumber(const std::string& str);
bool isHexadecimal(char ch);
bool isAlpha(char ch);
bool isIdentString(const char* s);

void replace(std::string& str, char textFrom, const char* textTo);
std::string getXTString(const std::string& str);

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args);

template<typename... Args>
std::string stringFormat(const std::string& format, Args... args);

std::string readFile(const std::string& path);

std::string ignoreShebang(const std::string& code);

std::string exeFilename();

class UtilPath {
  public:
    std::string path;

    UtilPath() : path("") {
    }
    UtilPath(const std::string& path) : path(path) {
    }

    std::string getPath();
    std::string getFilename();
    std::string getExtension();
    std::string getBasename();
    std::string getDirname();

    std::string printable();
};

std::vector<std::string> splitStringIntoVector(const std::string& subject,
                                               const char delimiter);

std::string replaceAllStringInString(std::string subject,
                                     const std::string& search,
                                     const std::string& replace);

std::string stripWhitespace(const std::string& subject);

void dumpXML(const std::string& filename, const std::string& xml);

std::string safeLiterals(const std::string& str);

#endif