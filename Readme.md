### X230 大青椒's

## SW Language Compiler (v1.0) 

Description here.

## Compiler User's Document 





## Compiler Architectural

### Lexical Parser

#### File List

The following lists the source files related to the SW Lexical Parser Module.

| Field            | Information          |
| ---------------- | -------------------- |
| Name             | SW Lexical Parser    |
| Source File Name | `CLexicalParser.cpp` |
| Header File Name | `CLexicalParser.h`   |

#### Keywords and reserved words

In SW language, an symbol can be keywords, variable names, or operators. Typically, when defining this language, it's not allowed to define a variable, a function with these reserved words. The definition of these words is given as the following:

`break` `call` `const` `continue` `else` `for` `func` `if` `in` `odd` `print` `read` `ref` `repeat` `return` `step` `unless` `until` `var` `while`.


### Syntax Parser

#### File List

The following lists the source files related to the SW Syntax Parser Module.

| Field            | Information                              | Description                              |
| ---------------- | ---------------------------------------- | ---------------------------------------- |
| Name             | SW Syntax Parser                         |                                          |
| Source File Name | [CSyntaxParser.cpp](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/CSyntaxParser.cpp) | Initialize the *FIRST* and *FOLLOW* sets information declared in `CSyntaxParser.h`. |
| Header File Name | [CSyntaxParser.h](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/CLexicalParser.h) | When syntax parsing, each subroutine needs the *FIRST* and *FOLLOW* sets of related procedures. In `CSyntaxParser.h` a series of classes is defined to store these sets information. |
| Source File Name | [SW_x230nz666.cpp](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/SW_x230nz666.cpp) | The **main** source file of the whole compiler, contains the main entrance `wmain` function and all the calling hierarchy. |

#### EBNF Format Syntax Description:

The following are the syntax description using EBNF formating.

```EBNF
program = declaration_list statement_list.

declaration_list = const_declaration_list
                   var_declaration_list
                   function_declaration_list.
                   
var_declaration_list = { var_declaration ";" }.

var_declaration = "var" var_ident_declaration
                        {"," var_ident_declaration }.

var_ident_declaration = ident ["=" expression ].
                               
const_declaration_list = { const_declaration ";" }.

const_declaration = "const" const_ident_declaration
                            {"," const_ident_declaration }.

const_ident_declaration = ident "=" expression .

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
    
param_ident_declaration = ["ref"] ident.

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

for_stat = "for" ident "in" expression "..." expression step expression "{" statement_list "}".

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
| `var_declaration_list`      | Represents a series of variable declarations. |
| `var_declaration`           | Represents a single variable declaration statement. <br/>Variables can be declared simultaneously in just one statement. |
| `var_ident_declaration`     | Represents one symbol declaration. e.g. a, a = 10, etc. The initial value is optional. |
| `const_declaration_list`    | Represents a series of constant declarations. |
| `const_declaration`         | Represents a single constant declaration statement. <br/>Just like how variables are declared, multiple constants can also be declared in just one statement. |
| `const_ident_declaration`   | Represents one constant declaration, e.g. a = 10. Remarkably, unlike in the variable declaration, the initial value is an essential part in constant declaration. |
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

#### *FIRST* and *FOLLOW* sets table 

In order to make this compiler using **LL(1)** design, the *FIRST* characters and *FOLLOW* characters of each non-terminal symbols are giving in the following table.

| Non-terminal                | First                                    | Follow                                   |
| --------------------------- | ---------------------------------------- | ---------------------------------------- |
| `program`                   | `const` `var` `func` `if` `unless` `while` `until` `repeat` `read` `print` `for` `call` `continue` `break` `IDENT` | #                                        |
| `declaration_list`          | `const` `var` `func`                     | \# `if` `unless` `while` `until` `repeat` `read` `print` `for` `call` `continue` `break` `IDENT` |
| `var_declaration_list`      | `var`                                    | \# `func` `const` `if` `unless` `while` `until` `repeat` `read` `print` `for` `call` `continue` `break` `IDENT` |
| `var_declaration`           | `var`                                    | `;`                                      |
| `var_ident_declaration`     | `IDENT`                                  | `;` `,`                                  |
| `const_declaration_list`    | `const`                                  | \# `func` `if` `unless` `while` `until` `repeat` `read` `print` `for` `call` `continue` `break` `IDENT` |
| `const_declaration`         | `const`                                  | `;`                                      |
| `const_ident_declaration`   | `IDENT`                                  | `;` `,`                                  |
| `function_declaration_list` | `func`                                   | \# `if` `unless` `while` `until` `repeat` `read` `print` `for` `call` `continue` `break` `IDENT` |
| `param_ident_declaration`   | `ref` `IDENT`                            | `,` `)`                                  |
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
| `expression`                | `IDENT` `NUMBER` `(` `+` `-`             | `;` `,` `)` `step`                       |
| `term`                      | `IDENT` `NUMBER` `(`                     | `+` `-` `;` `,` `)` `step`               |
| `factor`                    | `IDENT` `NUMBER` `(`                     | `*` `/` `+` `-` `;` `,` `) ` `step`      |

For the convenience of syntax parsing, all these sets are predefined in [CSyntaxParser.cpp](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/CSyntaxParser.cpp).

### Symbol Tables

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
const abc = 150;
var y, x, idx;

func sin(ref y, x)
{
	y = 0;
}

```

类型: 变量，常量，引用，无返回值函数，返回值函数

数据类型：整数，小数，逻辑值，

|标识符|标识符类型|c|d|e|


## Virtual Machine Architectural ![SW_VirtualMachine](./res/SW_VirtualMachine.png)
Here defines the design of the SW virtual machine.

### Virtual Machines Instructions

| Instruction | Code (integer) | Syntax | Description |
| ----------- | -------------- | ------ | ----------- |
|             |                |        |             |
|             |                |        |             |
|             |                |        |             |
|             |                |        |             |
|             |                |        |             |
|             |                |        |             |
|             |                |        |             |
|             |                |        |             |
|             |                |        |             |
|             |                |        |             |
|             |                |        |             |
|             |                |        |             |
|             |                |        |             |
|             |                |        |             |

