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