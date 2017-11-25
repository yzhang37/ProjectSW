###X230 大青椒's
##SW Language Compiler (v1.0) 

Description here.

##Compiler User's Document 





##Compiler Framework

###Lexical Parser

####Summary

| Field                        | Information          |
| ---------------------------- | -------------------- |
| Name                         | SW Lexical Parser    |
| Source File Name             | `CLexicalParser.cpp` |
| Header File Name (if exists) | `CLexicalParser.h`   |
|                              |                      |
|                              |                      |

#### Keywords and reserved words

In SW language, an symbol can be keywords, variable names, or operators. Typically, when defining this language, it's not allowed to define a variable, a function with these reserved words. Here the definition of these words is given as the following:

Statement
###Syntax Parser

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
                               "as" ident_type |
                               "as" ident_type "=" expression ].
                               
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
            odd "(" expression ")".

expression = ["+"|"-"] term { ("+"|"-") term}.

term = factor { ("*"|"/") factor}.

factor = ident | number | "(" expression ")".
```

Above, each the non-terminal symbols mean:

| Non-terminal                | Description                              |
| --------------------------- | ---------------------------------------- |
| `program`                   | The whole program architectural.         |
| `declaration_list`          | Represent all the declaration in the program, including global variables, constant values and functions. |
| `ident_type`                | In SW language, only two kind of data types are allowed: `Integer` (Int64) and `decimal` (double, in-memory storage using 8 bytes). |
| `var_declaration_list`      | Represent a series of variable declarations. |
| `var_declaration`           | Represent a single variable declaration statement. <br/>Variables can be declared simultaneously in just one statement. |
| `var_ident_declaration`     | Represent one symbol declaration. e.g. a, a as integer, a = 10, etc. The initial value is optional. |
| `const_declaration_list`    | Represent a series of constant declarations. |
| `const_declaration`         | Represent a single constant declaration statement. <br/>Just like how variables are declared, multiple constants can also be declared in just one statement. |
| `const_ident_declaration`   | Represent one constant declaration, e.g. a = 10, a as integer = 10, etc. Remarkably, unlike in the variable declaration, the initial value is an essential part in constant declaration. |
| `function_declaration_list` |                                          |
| `param_ident_declaration`   | Represent a single parameter declaration in the function header. `Ref` keyword is optional, it makes the parameter a reference to the original variable. |
| `function_body`             |                                          |
| `statement_list`            |                                          |
| `statement`                 | Represent all the statement in SW language. |
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






符号表结构



类型: 变量，常量，引用，无返回值函数，返回值函数

数据类型：整数，小数，逻辑值，

|标识符|标识符类型|c|d|e|



