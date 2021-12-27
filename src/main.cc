// Copyright (C) 2021 Justus Languell - All Rights Reserved.
//
// This file is part of Bounce which is released under the MIT license.
// See file LICENSE or go write <jus@gtsbr.org> for full license details.

#include "main.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <file.bc>" << std::endl;
        return 1;
    }

    try {
        std::string fileName = argv[1];
        std::string rootName = fileName.substr(0, fileName.find_last_of('.'));

        auto lex = new Lexer(readFile(fileName));
        // while (lex->tk) {
        //     std::cout << lex->getPosition() << ": " <<
        //     Lexer::getTokenStr(lex->tk) << ": " << lex->tkStr << std::endl;
        //     lex->getNextToken();
        // }

        auto parser = new Parser(lex);
        auto ast = parser->parse();
        dumpXML(rootName + ".xml", ast->asXML());

    } catch (Exception* e) { std::printf("ERROR: %s\n", e->text.c_str()); }
}