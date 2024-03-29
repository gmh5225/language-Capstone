/**
 * Copyright (C) 2021-2022 Justus Languell
 * This file is part of Capstone, which is licensed under the MIT license.
 * For more details, see ./license.txt or write <jus@gtsbr.org>.
 */
#ifndef CAPSTONE_TOKENS
#define CAPSTONE_TOKENS

#include "common.h"

enum TOKEN_TYPES {
    TOK_EOF = 0,
    TOK_ID = 256,
    TOK_INT,
    TOK_FLOAT,
    TOK_STR,

    TOK_EQUAL,          //-  ==
    TOK_NEQUAL,         //- !=
    TOK_LEQUAL,         //- <=
    TOK_LSHIFT,         //- <<
    TOK_LSHIFTEQUAL,    //- <<=
    TOK_GEQUAL,         //- >=
    TOK_RSHIFT,         //- >>
    TOK_RSHIFTUNSIGNED, //- >>>
    TOK_RSHIFTEQUAL,    //- >>=
    TOK_PLUSEQUAL,      //- +=
    TOK_MINUSEQUAL,     //- -=
    TOK_TIMESEQUAL,     //- *=
    TOK_DIVIDEEQUAL,    //- /=
    TOK_MODEQUAL,       //- %=
    TOK_PLUSPLUS,       //- ++
    TOK_MINUSMINUS,     //- --
    TOK_ANDEQUAL,       //- &=
    TOK_ANDAND,         //- &&
    TOK_OREQUAL,        //- |=
    TOK_OROR,           //- ||
    TOK_XOREQUAL,       //- ^=
    TOK_COLONCOLON,     //- ::
    TOK_LSINGLEARROW,   //- <-
    TOK_RSINGLEARROW,   //- ->
    TOK_RDOUBLEARROW,   //- =>
    TOK_SPACESHIP,      //- <=>
    TOK_GENERIC,        //- :<

// reserved words
#define TOK_R_LIST_START TOK_R_IF
    TOK_R_IF,
    TOK_R_ELSE,
    TOK_R_WHILE,
    TOK_R_FOR,
    TOK_R_BREAK,
    TOK_R_CONTINUE,
    TOK_R_FUNC,
    TOK_R_RETURN,

    TOK_R_ENUM,
    TOK_R_CLASS,
    TOK_R_STATIC,
    TOK_R_PUBLIC,
    TOK_R_PRIVATE,
    TOK_R_PROTECTED,


    TOK_R_TRUE,
    TOK_R_FALSE,
    TOK_R_NULL,

    TOK_R_CONST,
    TOK_R_VAR,

    TOK_R_IMPORT,

    TOK_R_LIST_END /* always the last entry */
};

#endif