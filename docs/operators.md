# Operators

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

## Uncommon Operators

### Size of

The sizeof operator is expressed as the `$` character. It is used to find the size of a variable in ~~bits~~ (this is yet to be determined), returned as an unsigned integer. To store the size of variable `x` into variable `y`, the following code would be employed:

```swift
var y = $x;
```

### Instance of

The instanceof operator is used to determine if a variable is an instance of a certain type, returning the true or false boolean as a single bit unsigned integer (`u1`). It is represented with the "spaceship" operator: `<=>`. Examples are shown below:

```swift
class MyObject {
    //...
}

class AnotherObject {
    //...
}

func main(String[] args) u1 {
    var x = MyObject();
    var y = AnotherObject();

    u1 a = x <=> MyObject;
    u1 b = y <=> MyObject;
}
```
