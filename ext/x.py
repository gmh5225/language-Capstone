x = '''* `break` Statement to break a loop.
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
* `while` Keyword to declare a "while" loop.'''

i = False
a = []
e = "";
for c in x:
    if c == '`':
        if i:
            a.append(e)
            e = ""
        i = not i
    else:
        if i:
            e += c

print('|'.join(a))
