/**
 * Copyright (C) 2021-2022 Justus Languell
 * This file is part of Capstone, which is licensed under the MIT license.
 * For more details, see ./license.txt or write <jus@gtsbr.org>.
 */
#include "lexer.h"

Lexer::Lexer(const std::string& input) {
    data = _strdup(input.c_str());
    dataOwned = true;
    dataStart = 0;
    dataEnd = strlen(data);
    reset();
}

Lexer::Lexer(Lexer* owner, int startChar, int endChar) {
    data = owner->data;
    dataOwned = false;
    dataStart = startChar;
    dataEnd = endChar;
    reset();
}

Lexer::~Lexer(void) {
    if (dataOwned) free((void*)data);
}

void Lexer::reset() {
    dataPos = dataStart;
    tokenStart = 0;
    tokenEnd = 0;
    tokenLastEnd = 0;
    tk = 0;
    tkStr = "";
    getNextCh();
    getNextCh();
    getNextToken();
}

void Lexer::match(int expectedTk) {
    if (tk != expectedTk) {
        throw new Exception("Got " + getTokenStr(tk) + " expected " +
                            getTokenStr(expectedTk) + " at " +
                            getPosition(tokenStart));
        getNextToken();
    }

    getNextToken();
}

std::string Lexer::getTokenStr(int token) {
    // if (token == 60) return "＜"; // for markdown
    if (token == 60) return "<";
    // if (token == 62) return "＞";
    if (token == 62) return ">";
    if (token > 32 && token < 128) return std::string(1, token);
    switch (token) {
    case TOK_EOF: return "EOF";
    case TOK_ID: return "ID";
    case TOK_INT: return "INT";
    case TOK_FLOAT: return "FLOAT";
    case TOK_STR: return "STRING";
    case TOK_EQUAL: return "==";
    case TOK_NEQUAL: return "!=";
    case TOK_LEQUAL: return "<=";
    case TOK_LSHIFT: return "<<";
    case TOK_LSHIFTEQUAL: return "<<=";
    case TOK_GEQUAL: return ">=";
    case TOK_RSHIFT: return ">>";
    case TOK_RSHIFTEQUAL: return ">>=";
    case TOK_PLUSEQUAL: return "+=";
    case TOK_MINUSEQUAL: return "-=";
    case TOK_TIMESEQUAL: return "*=";
    case TOK_DIVIDEEQUAL: return "/=";
    case TOK_MODEQUAL: return "%=";
    case TOK_PLUSPLUS: return "++";
    case TOK_MINUSMINUS: return "--";
    case TOK_ANDEQUAL: return "&=";
    case TOK_ANDAND: return "&&";
    case TOK_OREQUAL: return "|=";
    case TOK_OROR: return "||";
    case TOK_XOREQUAL: return "^=";
    case TOK_COLONCOLON: return "::";
    case TOK_LSINGLEARROW: return "<-";
    case TOK_RSINGLEARROW: return "->";
    case TOK_RDOUBLEARROW: return "=>";
    case TOK_SPACESHIP: return "<=>";

    case TOK_R_IF: return "if";
    case TOK_R_ELSE: return "else";
    case TOK_R_WHILE: return "while";
    case TOK_R_FOR: return "for";
    case TOK_R_BREAK: return "break";
    case TOK_R_CONTINUE: return "continue";
    case TOK_R_FUNC: return "func";
    case TOK_R_RETURN: return "return";

    case TOK_R_ENUM: return "enum";
    case TOK_R_CLASS: return "class";
    case TOK_R_PUBLIC: return "public";
    case TOK_R_PROTECTED: return "protected";
    case TOK_R_PRIVATE: return "private";

    case TOK_R_TRUE: return "true";
    case TOK_R_FALSE: return "false";
    case TOK_R_NULL: return "null";
    case TOK_R_CONST: return "const";
    case TOK_R_VAR: return "var";

    case TOK_R_IMPORT: return "import";
    }

    return "?[" + std::to_string(token) + "]";
}

void Lexer::getNextCh() {
    currCh = nextCh;
    if (dataPos < dataEnd)
        nextCh = data[dataPos];
    else
        nextCh = 0;
    dataPos++;
}

void Lexer::getNextToken() {
    tk = TOK_EOF;
    tkStr.clear();
    while (currCh && isWhitespace(currCh)) getNextCh();

    if (currCh == '/' && nextCh == '/') {
        while (currCh && currCh != '\n') getNextCh();
        getNextCh();
        getNextToken();
        return;
    }

    if (currCh == '/' && nextCh == '*') {
        while (currCh && (currCh != '*' || nextCh != '/')) getNextCh();
        getNextCh();
        getNextCh();
        getNextToken();
        return;
    }

    tokenStart = dataPos - 2;

    if (isAlpha(currCh)) {
        while (isAlpha(currCh) || isNumeric(currCh)) {
            tkStr += currCh;
            getNextCh();
        }

        tk = TOK_ID;

        if (tkStr == "if")
            tk = TOK_R_IF;
        else if (tkStr == "else")
            tk = TOK_R_ELSE;
        else if (tkStr == "while")
            tk = TOK_R_WHILE;
        else if (tkStr == "for")
            tk = TOK_R_FOR;
        else if (tkStr == "break")
            tk = TOK_R_BREAK;
        else if (tkStr == "continue")
            tk = TOK_R_CONTINUE;
        else if (tkStr == "func")
            tk = TOK_R_FUNC;
        else if (tkStr == "return")
            tk = TOK_R_RETURN;
        else if (tkStr == "enum")
            tk = TOK_R_ENUM;
        else if (tkStr == "class")
            tk = TOK_R_CLASS;
        else if (tkStr == "public")
            tk = TOK_R_PUBLIC;
        else if (tkStr == "protected")
            tk = TOK_R_PROTECTED;
        else if (tkStr == "private")
            tk = TOK_R_PRIVATE;
        else if (tkStr == "static")
            tk = TOK_R_STATIC;
        else if (tkStr == "true")
            tk = TOK_R_TRUE;
        else if (tkStr == "false")
            tk = TOK_R_FALSE;
        else if (tkStr == "null")
            tk = TOK_R_NULL;
        else if (tkStr == "nil")
            tk = TOK_R_NULL;
        else if (tkStr == "const")
            tk = TOK_R_CONST;
        else if (tkStr == "final")
            tk = TOK_R_CONST;
        else if (tkStr == "import")
            tk = TOK_R_IMPORT;
        else if (tkStr == "var")
            tk = TOK_R_VAR;
        else if (tkStr == "let")
            tk = TOK_R_VAR;

    } else if (isNumeric(currCh)) {
        bool isHex = false;
        if (currCh == '0') {
            tkStr += currCh;
            getNextCh();
        }
        if (currCh == 'x') {
            isHex = true;
            tkStr += currCh;
            getNextCh();
        }

        tk = TOK_INT;
        while (isNumeric(currCh) || (isHex && isHexadecimal(currCh))) {
            tkStr += currCh;
            getNextCh();
        }

        if (!isHex && currCh == '.') {
            tk = TOK_FLOAT;
            tkStr += '.';
            getNextCh();
            while (isNumeric(currCh)) {
                tkStr += currCh;
                getNextCh();
            }
        }

        if (!isHex && (currCh == 'e' || currCh == 'E')) {
            tk = TOK_FLOAT;
            tkStr += currCh;
            getNextCh();

            if (currCh == '-') {
                tkStr += currCh;
                getNextCh();
            }

            while (isNumeric(currCh)) {
                tkStr += currCh;
                getNextCh();
            }
        }
    } else if (currCh == '"') {
        getNextCh();
        while (currCh && currCh != '"') {
            if (currCh == '\\') {
                getNextCh();
                switch (currCh) {
                case 'n': tkStr += '\n'; break;
                case '"': tkStr += '"'; break;
                case '\\': tkStr += '\\'; break;
                default: tkStr += currCh;
                }
            } else {
                tkStr += currCh;
            }
            getNextCh();
        }

        getNextCh();
        tk = TOK_STR;
    } else if (currCh == '\'') {
        // strings again...
        getNextCh();
        while (currCh && currCh != '\'') {
            if (currCh == '\\') {
                getNextCh();
                switch (currCh) {
                case 'n': tkStr += '\n'; break;
                case 'a': tkStr += '\a'; break;
                case 'r': tkStr += '\r'; break;
                case 't': tkStr += '\t'; break;
                case '\'': tkStr += '\''; break;
                case '\\': tkStr += '\\'; break;
                case 'x': { // hex digits
                    char buf[3] = "??";
                    getNextCh();
                    buf[0] = currCh;
                    getNextCh();
                    buf[1] = currCh;
                    tkStr += (char)strtol(buf, 0, 16);
                } break;
                default:
                    if (currCh >= '0' && currCh <= '7') {
                        // octal digits
                        char buf[4] = "???";
                        buf[0] = currCh;
                        getNextCh();
                        buf[1] = currCh;
                        getNextCh();
                        buf[2] = currCh;
                        tkStr += (char)strtol(buf, 0, 8);
                    } else
                        tkStr += currCh;
                }
            } else {
                tkStr += currCh;
            }
            getNextCh();
        }
        getNextCh();
        tk = TOK_STR;
    } else {
        // single chars
        tk = currCh;
        if (currCh) getNextCh();
        if (tk == '=' && currCh == '=') {
            tk = TOK_EQUAL;
            getNextCh();
        } else if (tk == '!' && currCh == '=') {
            tk = TOK_NEQUAL;
            getNextCh();
        } else if (tk == '<' && currCh == '=') {
            tk = TOK_LEQUAL;
            getNextCh();
        } else if (tk == '<' && currCh == '<') {
            tk = TOK_LSHIFT;
            getNextCh();
            if (currCh == '=') {
                tk = TOK_LSHIFTEQUAL;
                getNextCh();
            }
        } else if (tk == '>' && currCh == '=') {
            tk = TOK_GEQUAL;
            getNextCh();
        } else if (tk == '>' && currCh == '>') {
            tk = TOK_RSHIFT;
            getNextCh();
            if (currCh == '=') {
                tk = TOK_RSHIFTEQUAL;
                getNextCh();
            }
        } else if (tk == '+' && currCh == '=') {
            tk = TOK_PLUSEQUAL;
            getNextCh();
        } else if (tk == '-' && currCh == '=') {
            tk = TOK_MINUSEQUAL;
            getNextCh();
        } else if (tk == '*' && currCh == '=') {
            tk = TOK_TIMESEQUAL;
            getNextCh();
        } else if (tk == '%' && currCh == '=') {
            tk = TOK_MODEQUAL;
            getNextCh();
        } else if (tk == '/' && currCh == '=') {
            tk = TOK_DIVIDEEQUAL;
            getNextCh();
        } else if (tk == '+' && currCh == '+') {
            tk = TOK_PLUSPLUS;
            getNextCh();
        } else if (tk == '-' && currCh == '-') {
            tk = TOK_MINUSMINUS;
            getNextCh();
        } else if (tk == '&' && currCh == '=') {
            tk = TOK_ANDEQUAL;
            getNextCh();
        } else if (tk == '&' && currCh == '&') {
            tk = TOK_ANDAND;
            getNextCh();
        } else if (tk == '|' && currCh == '=') {
            tk = TOK_OREQUAL;
            getNextCh();
        } else if (tk == '|' && currCh == '|') {
            tk = TOK_OROR;
            getNextCh();
        } else if (tk == '^' && currCh == '=') {
            tk = TOK_XOREQUAL;
            getNextCh();
        } else if (tk == '-' && currCh == '>') {
            tk = TOK_RSINGLEARROW;
            getNextCh();
        } else if (tk == '<' && currCh == '-') {
            tk = TOK_LSINGLEARROW;
            getNextCh();
        } else if (tk == TOK_LEQUAL && currCh == '>') {
            tk = TOK_SPACESHIP;
            getNextCh();
        } else if (tk == ':') {
            if (currCh == ':') {
                tk = TOK_COLONCOLON;
                getNextCh();
            } else if (currCh == '<') {
                tk = TOK_GENERIC;
                getNextCh();
            }
        }
    }

    tokenLastEnd = tokenEnd;
    tokenEnd = dataPos - 3;
}

std::string Lexer::getSubString(int lastPosition) {
    // Fuckshit memory hack
    int lastCharIdx = tokenLastEnd + 1;
    if (lastCharIdx < dataEnd) {
        char old = data[lastCharIdx];
        data[lastCharIdx] = 0;
        std::string value = &data[lastPosition];
        data[lastCharIdx] = old;
        return value;
    } else {
        return std::string(&data[lastPosition]);
    }
}

Lexer* Lexer::getSubLex(int lastPosition) {
    int lastCharIdx = tokenLastEnd + 1;
    if (lastCharIdx < dataEnd)
        return new Lexer(this, lastPosition, lastCharIdx);
    else
        return new Lexer(this, lastPosition, dataEnd);
}

std::string Lexer::getPosition(int pos) {
    if (pos < 0) pos = tokenLastEnd;
    int line = 1, col = 1;
    for (int i = 0; i < pos; i++) {
        char ch;
        if (i < dataEnd)
            ch = data[i];
        else
            ch = 0;
        col++;
        if (ch == '\n') {
            line++;
            col = 0;
        }
    }
    char buf[256];
    sprintf_s(buf, 256, "(line: %d, col: %d)", line, col);
    return buf;
}

int Lexer::nextToken() {
    getNextToken();
    return tk;
}
