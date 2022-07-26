/**
 * Copyright (C) 2021-2022 Justus Languell
 * This file is part of Capstone, which is licensed under the MIT license.
 * For more details, see ./license.txt or write <jus@gtsbr.org>.
 */
#ifndef CAPSTONE_UTILS
#define CAPSTONE_UTILS

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

void dumpStringToFile(const std::string& filename, const std::string& content);

#endif