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

## Reserved Words

Capstone has *19 + 2* reserved words. Reserved words can be either a keyword, a statement, or a modifier, or multiple.

* `break` Statement to break a loop.
* `class` Keyword to declare a class.
* `const` or `final` Keyword to declare an immutable variable or specify parameter immutability.
* `continue` Statement to skip to the next pass of a loop.
* `else` Keyword to declare the "else" body of an "if" statement.
* `false` Literal for boolean false.
* `for` Keyword to declare a "for" loop.
* `func` Keyword to declare a function.
* `if` Keyword to declare a "if" statement.
* `import` Keyword to use classes and functions from another package.
* `null` Literal for a null reference.
* `private` Modifier to mark a field accessible only in the class scope.
* `protected` Modifier to mark a field accessible only in the class and child class scope.
* `public` Modifier to mark a field accessible outside the class scope.
* `return` Statement to return from a function.
* `static` Modifier to mark a field static instead.
* `true` Literal for boolean true.
* `var` or `let` Keyword for declaring a mutable variable.
* `while` Keyword to declare a "while" loop.

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

### Uncommon Operators

#### Size of

The sizeof operator is expressed as the `$` character. It is used to find the size of a variable in ~~bits~~ (this is yet to be determined), returned as an unsigned integer. To store the size of variable `x` into variable `y`, the following code would be employed:

```swift
y: u16 = $x;
```

#### Instance of

The instanceof operator is used to determine if a variable is an instance of a certain type, returning the true or false boolean as a single bit unsigned integer (`u1`). It is represented with the "spaceship" operator: `<=>`.

```swift
class MyObject { }

class AnotherObject { }

func main(String[] args) u1 {
    x: MyObject = MyObject();
    y: AnotherObject = AnotherObject();
    u1 a = x <=> MyObject;
    u1 b = y <=> MyObject;
}

```

## Current TODO

* Multiple declaration
* Enum declaration
