// Copyright (C) 2021 Justus Languell - All Rights Reserved.
//
// This file is part of Bounce which is released under the MIT license.
// See file LICENSE or go write <jus@gtsbr.org> for full license details.

#ifndef BOUNCE_LEXER
#define BOUNCE_LEXER

#include "common.h"

#include "exception.h"
#include "token.h"
#include "utils.h"

class Lexer {
  public:
    Lexer(const std::string& input);
    Lexer(Lexer* owner, int startChar, int endChar);
    ~Lexer();

    char currCh, nextCh;
    int tk, tokenStart, tokenEnd, tokenLastEnd;
    std::string tkStr;

    void match(int expectedTk);
    static std::string getTokenStr(int token);
    void reset();

    std::string getSubString(int pos);
    Lexer* getSubLex(int lastPosition);

    std::string getPosition(int pos = -1);

    std::string nextTokenString();
    int nextToken();

  protected:
    char* data;
    int dataStart, dataEnd;
    bool dataOwned;

    int dataPos;

    void getNextCh();
    void getNextToken();
};

#endif