# Capstone

Compiler frontend for the Capstone language, built on LLVM.

## Design

Capstone is a compiler frontend built on LLVM.
The compiler is built from compartmentalized components encapsulated as classes.

## Components

Lexer → (tokens) → Parser → (ast) → Validator → (ast) → Generator

## Lexer and tokens

The lexer tokenizes the source code to provide a meaningful representation to the parser. The data is represented as an iterable series of tokens.

I (Justus) wrote the current lexer a while ago (like 7 months) and could use a rewrite, but will work for now. This is the benifit of compartmentalizing components.

Files:

* `lexer.h` The lexer header declaration.
* `lexer.cc` The lexer source implementation.
* `token.h` The header of token types.

## Parser and AST

The parser is responsible for grouping the tokens in a meaningful representation of program evaluation and control flow. The parser uses an instance of the lexer and transforms the token representation into a tree called the abstract syntax tree (AST).

The AST node source code is generated using a Python script (`./scripts/ast_gen.py`) from a declaration in `./src/ast.template`.

Files:

* `parser.h` The parser header declaration.
* `parser.cc` The parser source implementation.
* `ast.h` The declaration of the node classes for the AST.
* `ast.cc` The implementation of the AST node methods.

## Operators

In order of precedence:

| Type              | Operators                                                |
| ----------------- | -------------------------------------------------------- |
| Unary             | `!` [`$`](#size-of)                                      |
| Multiplicative    | `*` `/` `%`                                              |
| Additive          | `+` `-`                                                  |
| Shift             | `<<` `>>`                                                |
| Relational        | `<` `>` `<=` `>=` [`<=>`](#instance-of)                  |
| Equality          | `==` `!=`                                                |
| Bitwise (ordered) | `&` `^` `\|`                                             |
| Logical (ordered) | `&&` `\|\|`                                              |
| Ternary           | `? :`                                                    |
| Assignment        | `=` `+=` `-=` `*=` `/=` `%=` `<<=` `>>=` `&=` `^=` `\|=` |

## TODO

* Unary expressions
* Class declarations
* Const handling
* Ternary expressions
* Multiple declaration
