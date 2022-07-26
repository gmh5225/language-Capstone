/**
 * Copyright (C) 2021-2022 Justus Languell
 * This file is part of Capstone, which is licensed under the MIT license.
 * For more details, see ./license.txt or write <jus@gtsbr.org>.
 */
#include "main.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <file.cap>" << std::endl;
        return 1;
    }

    try {
        const std::string fileName = argv[1];
        const std::string rootName = fileName.substr(0, fileName.find_last_of('.'));

        const std::string source = readFile(fileName);

        std::cout << source << std::endl;

        auto lex = new Lexer(source);

        // while (lex->tk) {
        //     std::cout << lex->getPosition() << ": "
        //               << Lexer::getTokenStr(lex->tk) << "\t" << lex->tkStr << ""
        //               << std::endl;
        //     lex->getNextToken();
        // }

        auto parser = new Parser(lex);
        auto ast = parser->parse();
        LOG(-1);
        // //dumpStringToFile(rootName + ".xml", ast->asXML());
        // //std::printf("%s\n", ast->asJSON().c_str());
        dumpStringToFile(rootName + ".json", ast->toJSON());

    } catch (Exception* e) { std::printf("ERROR: %s\n", e->text.c_str()); }
}