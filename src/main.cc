/**
 * Copyright (C) 2021-2022 Justus Languell
 * This file is part of Capstone, which is licensed under the MIT license.
 * For more details, see ./license.txt or write <jus@gtsbr.org>.
 */
#include "main.h"

class Some {
  public:
    std::string foo;
    int bar;
    Some(const std::string& foo, const int bar) : foo(foo), bar(bar){};
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <file.bc>" << std::endl;
        return 1;
    }

    try {
        std::string fileName = argv[1];
        std::string rootName = fileName.substr(0, fileName.find_last_of('.'));

        auto yea = new Some("shit", 42);
        std::cout << yea->foo << " " << yea->bar << std::endl;

        auto lex = new Lexer(readFile(fileName));
        while (lex->tk) {
            std::cout << lex->getPosition() << ": "
                      << Lexer::getTokenStr(lex->tk) << ": " << lex->tkStr
                      << std::endl;
            lex->getNextToken();
        }

        // auto parser = new Parser(lex);
        // auto ast = parser->parse();
        // //dumpStringToFile(rootName + ".xml", ast->asXML());
        // //std::printf("%s\n", ast->asJSON().c_str());
        // dumpStringToFile(rootName + ".json", ast->asJSON());

    } catch (Exception* e) { std::printf("ERROR: %s\n", e->text.c_str()); }
}