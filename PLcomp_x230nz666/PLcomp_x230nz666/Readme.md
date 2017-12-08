###X230 大青椒's

## SW Language Compiler (v1.0) 

Description here.

## Compiler User's Document 





## Compiler Framework

### Lexical Parser

#### Summary

| Field                        | Information          |
| ---------------------------- | -------------------- |
| Name                         | SW Lexical Parser    |
| Source File Name             | `CLexicalParser.cpp` |
| Header File Name (if exists) | `CLexicalParser.h`   |
|                              |                      |
|                              |                      |

#### Keywords and reserved words

In SW language, an symbol can be keywords, variable names, or operators. Typically, when defining this language, it's not allowed to define a variable, a function with these reserved words. Here the definition of these words is given as the following:

### Syntax Parser

Description here.

#### EBNF Format Syntax Description:

The following are the syntax description using EBNF formating.

```EBNF
program = declaration_list statement_list.

declaration_list = const_declaration_list
                   var_declaration_list
                   function_declaration_list.

ident_type = "integer" | "decimal" .

var_declaration_list = { var_declaration ";" }.

var_declaration = "var" var_ident_declaration
                        {"," var_ident_declaration }.

var_ident_declaration = ident ["=" expression | 
                               "as" ident_type ["=" expression] ].
                               
const_declaration_list = { const_declaration ";" }.

const_declaration = "const" const_ident_declaration
                            {"," const_ident_declaration }.

const_ident_declaration = ident ["as" ident_type ] "=" expression .

function_declaration_list = 
    { "func" ident "(" 
        [ 
            param_ident_declaration 
            {"," param_ident_declaration }
        ] ")" 
        "{" 
            function_body
        "}" 
    }.
    
param_ident_declaration = ["ref"] ident "as" ident_type .

function_body = var_declaration_list statement_list.

statement_list = { statement ";"}.

statement = if_stat | while_stat | 
            repeat_stat | read_stat | 
            print_stat | assign_stat |
            for_stat | call_stat |
            "continue" | "break" .

if_stat = ( "if" | "unless" ) condition "{" statement_list "}" 
          ["else" "{" statement_list "}"].
          
while_stat = ( "while" | "until" ) condition "{" statement_list "}".

repeat_stat = "repeat" "{" statement_list "}" 
              ( "while" | "until" ) condition.

read_stat = "read" "(" ident ")".

print_stat = "print" "(" ident ")".

assign_stat = ident ( "=" expression | "++" | "--" ).

for_stat = "for" ident "in" (ident|number) "..." (ident|number) "{" statement_list "}".

call_stat = "call" ident "(" [ expression { "," expression }  ] ")".

condition = expression ("=="|"!="|"<"|"<="|">"|">=") expression |
            "odd" "(" expression ")".

expression = ["+"|"-"] term { ("+"|"-") term}.

term = factor { ("*"|"/") factor}.

factor = ident | number | "(" expression ")".
```

Above, each the non-terminal symbols mean:

| Non-terminal                | Description                              |
| --------------------------- | ---------------------------------------- |
| `program`                   | The whole program architectural.         |
| `declaration_list`          | Represents all the declaration in the program, including global variables, constant values and functions. |
| `ident_type`                | In SW language, only two kind of data types are allowed: `Integer` (Int64) and `decimal` (double, in-memory storage using 8 bytes). |
| `var_declaration_list`      | Represents a series of variable declarations. |
| `var_declaration`           | Represents a single variable declaration statement. <br/>Variables can be declared simultaneously in just one statement. |
| `var_ident_declaration`     | Represents one symbol declaration. e.g. a, a as integer, a = 10, etc. The initial value is optional. |
| `const_declaration_list`    | Represents a series of constant declarations. |
| `const_declaration`         | Represents a single constant declaration statement. <br/>Just like how variables are declared, multiple constants can also be declared in just one statement. |
| `const_ident_declaration`   | Represents one constant declaration, e.g. a = 10, a as integer = 10, etc. Remarkably, unlike in the variable declaration, the initial value is an essential part in constant declaration. |
| `function_declaration_list` |                                          |
| `param_ident_declaration`   | Represents a single parameter declaration in the function header. `Ref` keyword is optional, it makes the parameter a reference to the original variable. |
| `function_body`             | Represents the whole structure of a function. A function includes a series of variable definition, which is followed by a number of statements. |
| `statement_list`            | Represents a series of statements. Each statement should end with a semicolon `;` symbol. |
| `statement`                 | Represents all the statement in SW language. |
| `if_stat`                   |                                          |
| `while_stat`                |                                          |
| `repeat_stat`               |                                          |
| `read_stat`                 |                                          |
| `print_stat`                |                                          |
| `assign_stat`               |                                          |
| `for_stat`                  |                                          |
| `call_stat`                 |                                          |
| `condition`                 |                                          |
| `expression`                |                                          |
| `term`                      |                                          |
| `factor`                    |                                          |

In order to make the compile using a LL(1), the FIRST characters and FOLLOW characters of each non-terminal symbols are giving in the following table.

| Non-terminal                | First                                    | Follow                                   |
| --------------------------- | ---------------------------------------- | ---------------------------------------- |
| `program`                   | `const` `var` `func` `if` `unless` `while` `until` `repeat` `read` `print` `for` `call` `continue` `break` `IDENT` | #                                        |
| `declaration_list`          | `const` `var` `func`                     | \# `if` `unless` `while` `until` `repeat` `read` `print` `for` `call` `continue` `break` `IDENT` |
| `ident_type`                | `integer` `decimal`                      | `=` `;` `,` `)`                          |
| `var_declaration_list`      | `var`                                    | \# `func` `const` `if` `unless` `while` `until` `repeat` `read` `print` `for` `call` `continue` `break` `IDENT` |
| `var_declaration`           | `var`                                    | `;`                                      |
| `var_ident_declaration`     | `integer` `decimal`                      | `;` `,`                                  |
| `const_declaration_list`    | `const`                                  | \# `func` `if` `unless` `while` `until` `repeat` `read` `print` `for` `call` `continue` `break` `IDENT` |
| `const_declaration`         | `const`                                  | `;`                                      |
| `const_ident_declaration`   | `integer` `decimal`                      | `;` `,`                                  |
| `function_declaration_list` | `func`                                   | \# `if` `unless` `while` `until` `repeat` `read` `print` `for` `call` `continue` `break` `IDENT` |
| `param_ident_declaration`   | `ref` `integer` `decimal`                | `,` `)`                                  |
| `function_body`             | `var`                                    | `}`                                      |
| `statement_list`            | `if` `unless` `while` `until` `repeat` `read` `print` `for` `call` `continue` `break` `IDENT` | \# `}`                                   |
| `statement`                 | `if` `unless` `while` `until` `repeat` `read` `print` `for` `call` `continue` `break` `IDENT` | `;`                                      |
| `if_stat`                   | `if` `unless`                            | `;`                                      |
| `while_stat`                | `while` `until`                          | `;`                                      |
| `repeat_stat`               | `repeat`                                 | `;`                                      |
| `read_stat`                 | `read`                                   | `;`                                      |
| `print_stat`                | `print`                                  | `;`                                      |
| `assign_stat`               | `IDENT`                                  | `;`                                      |
| `for_stat`                  | `for`                                    | `;`                                      |
| `call_stat`                 | `call`                                   | `;`                                      |
| `condition`                 | `IDENT` `NUMBER` `(` `+` `-` `odd`       | `{` `;`                                  |
| `expression`                | `IDENT` `NUMBER` `(` `+` `-`             | `;` `,` `)`                              |
| `term`                      | `IDENT` `NUMBER` `(`                     | `+` `-` `;` `,` `)`                      |
| `factor`                    | `IDENT` `NUMBER` `(`                     | `*` `/` `+` `-` `;` `,` `)`              |



### The structure of Symbol Tables

| Symbol Identifier | Attribute | Data Type | Level |
| ----------------- | --------- | --------- | ----- |
| `pi`              | constant  | Decimal   | 0     |
| `y`               | variable  | Decimal   | 0     |
| `x`               | variable  | Decimal   | 0     |
| `idx`             | variable  | Integer   | 0     |
| `sin`             | function  | Unknown   | 0     |
| `y`               | Reference | Decimal   | 1     |
| `x`               |           |           | 1     |

```Swift
const pi = 3.1415926;
var y as decimal, x as decimal,
    idx as integer;

func sin(ref y as decimal, x as decimal)
{
	y = 0;
}



```

类型: 变量，常量，引用，无返回值函数，返回值函数

数据类型：整数，小数，逻辑值，

|标识符|标识符类型|c|d|e|



