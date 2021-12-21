#include "main.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <file.bc>" << std::endl;
        return 1;
    }

    try {
        auto lex = new Lexer(readFile(argv[1]));
        while (lex->tk) {
            std::cout << lex->getPosition() << ": " << Lexer::getTokenStr(lex->tk) << ": " << lex->tkStr << std::endl;
            lex->nextToken();
        }
    } catch (Exception* e) { 
        std::printf("ERROR: %s\n", e->text.c_str()); 
    }
}