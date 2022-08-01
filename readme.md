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

| Type              | Operators                                                  |
| ----------------- | ---------------------------------------------------------- |
| Unary             | `!` [`$`](#size-of) [`#`](#type-of) [`@`](#length-of) `[]` |
| Multiplicative    | `*` `/` `%`                                                |
| Additive          | `+` `-`                                                    |
| Shift             | `<<` `>>`                                                  |
| Relational        | `<` `>` `<=` `>=` [`<=>`](#instance-of)                    |
| Equality          | `==` `!=`                                                  |
| Bitwise (ordered) | `&` `^` `\|`                                               |
| Logical (ordered) | `&&` `\|\|`                                                |
| Ternary           | `? :`                                                      |
| Assignment        | `=` `+=` `-=` `*=` `/=` `%=` `<<=` `>>=` `&=` `^=` `\|=`   |

### Uncommon Operators

#### Size of

The sizeof operator is expressed as the `$` character. It is used to find the size of a variable in ~~bits~~ (this is yet to be determined), returned as an unsigned, 32 bit integer (u32). Usage:

```swift
var x: i32 = 5;
var y: u32 = $x;
// y = 32
```

#### Type of

**This is not finalized to be in the language!** The typeof operator is expressed using the `#` character. It is used to find the data type or class that of a variable. It returns the class.

#### Length of

The length of operator is expressed with the `@` character. It is used to find the length of an array, and returns an unsigned, 32 bit integer (u32). Usage:

```swift
var x: i32[] = [1, 2, 3, 4, 5];
var y: u32 = #x;
// y = 5
```

#### Instance of

The instanceof operator is used to determine if a variable is an instance of a certain type, returning the true or false boolean as a single bit unsigned integer (`u1`). It is represented with the "spaceship" operator: `<=>`.

```swift
class MyObject { }

class AnotherObject { }

func main(args: String[]) u0 {
    var x: MyObject = MyObject();
    var y: AnotherObject = AnotherObject();
    var a: u1 = x <=> MyObject;
    var b: u1 = y <=> MyObject;
}

```

## Builtin Types

### Primatives

Primative datatypes are named using a character specification followed by a numerical size.

| Character | Meaning          | Sizes               |  
| --------- | ---------------- | ------------------- |
| i         | Signed integer   | 8, 16, 32, 64       |
| u         | Unsigned integer | 0, 1, 8, 16, 32, 64 |
| f         | Floating point   | 16, 32, 64, 128     |

### Type aliases

There are a few semi-reserved words that are builtin aliases for existing types.

| Alias     | Type          | Why                            |  
| --------- | ------------- | ------------------------------ |
| void      | u0            | Specifies returning no data    |
| bool      | u1            | Boolean data                   |
| char      | u8            | Represents a unicode character |

## Builtin classes

There are two builtin classes: String and Error.

### String

Under the hood, strings are represented as an array of characters: `char[]`. In addition, the String class provides useful methods like the string class in Java or C++. What makes the String class special is that it can:

* be assigned with a string literal
* uses + for concatanation <!-- TYPO -->
* uses == for direct comparison <!-- TYPO? -->

### Error

Error handling is done through multiple return similar to a Go. However, the Error class eliminates the ugly nil checking.

```swift

func functionThatCanError(x: i32) (u32, Error) {
    if (x < 0) return null, Error("Paramter cannot be negative"); // An error with a string argument is a failure.
    return x, Error(); // An error with no string argument is a success.
}

func main(args: String[]) void {

    var x, err = functionThatCanError(-5);
    if (err.failed()) {
        // handle the error
    }

    var y, _ = functionThatCanError(-5);
    // Ignore the error, y will be null
}

```

## Current TODO

* Multiple declaration
* Enum declaration
