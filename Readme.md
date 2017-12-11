## *Lacus X230-NZ666*'s<br/>SW Language Compiler (v1.0) 

[Lacus X230-NZ666](https://github.com/yzhang37) are designing his first compiler and his Compiling principle lesson Big job. The SW Language is a Swift-like language, but only has some few features. 

## Compiler User's Document 

:-) *Under construction here. When Lacus X230 NZ666 has finished these, they will be posted here.* 

## Compiler Architectural

### Lexical Parser

#### File List

The following lists the source files related to the SW Lexical Parser Module.

| Field    | Information          |
| -------- | -------------------- |
| Name     | SW Lexical Parser    |
| *Source* | `CLexicalParser.cpp` |
| *Header* | `CLexicalParser.h`   |

#### Keywords and reserved words

In SW language, an symbol can be keywords, variable names, or operators. Typically, when defining this language, it's not allowed to define a variable, a function with these reserved words. The definition of these words is given as the following:

`break` `call` `const` `continue` `else` `for` `func` `if` `in` `odd` `print` `read` `ref` `repeat` `return` `step` `unless` `until` `var` `while`.


### Syntax Parser

#### File List

The following lists the source files related to the SW Syntax Parser Module.

| Field    | Information                              | Description                              |
| -------- | ---------------------------------------- | ---------------------------------------- |
| Name     | SW Syntax Parser                         |                                          |
| *Source* | [CSyntaxParser.cpp](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/CSyntaxParser.cpp) | Initialize the *FIRST* and *FOLLOW* sets information declared in `CSyntaxParser.h`. |
| *Header* | [CSyntaxParser.h](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/CLexicalParser.h) | When syntax parsing, each subroutine needs the *FIRST* and *FOLLOW* sets of related procedures. In `CSyntaxParser.h` a series of classes is defined to store these sets information. |
| *Source* | [SW_x230nz666.cpp](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/SW_x230nz666.cpp) | The **main** source file of the whole compiler, contains the main entrance `wmain` function and all the calling hierarchy. |

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
| `function_declaration_list` | Represents a series of function callers. |
| `param_ident_declaration`   | Represents a single parameter declaration in the function header. `Ref` keyword is optional, it makes the parameter a reference to the original variable. |
| `function_body`             | Represents the whole structure of a function. A function includes a series of variable definition, which is followed by a number of statements. |
| `statement_list`            | Represents a series of statements. Each statement should end with a semicolon `;` symbol. |
| `statement`                 | Represents all the statement in SW language. |
| `if_stat`                   | `if` *condition* { *statement* } `else` { *statement* }; |
| `while_stat`                | `while`                                  |
| `repeat_stat`               | `repeat` { *statement* } `while`         |
| `read_stat`                 | `read` ( *variable_name* );              |
| `print_stat`                | `print` ( *variable_name* );             |
| `assign_stat`               | `variable_name` = *expression*;          |
| `for_stat`                  | `for` *variable_name* `in` *expression*...*expression* `step` *expression* <br/>{ *statement* }; |
| `call_stat`                 | `call` *function_name* (*parameters*);   |
| `condition`                 | Represents some boolean operations.      |
| `expression`                | For computation use.                     |
| `term`                      | For computation use.                     |
| `factor`                    | For computation use.                     |

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

Symbol tables are important for a compiler. Typically, programming language are not strictly context-free grammar. For example, when accessing a variable, commonly the variable should be predefined, or an error occurred. The symbol tables are included to make the original programming grammar a context-sensitive grammar.

#### File List

The following lists the source files related to the SW Symbol Tables Module.

| Field    | Information                              | Description                              |
| -------- | ---------------------------------------- | ---------------------------------------- |
| Name     | SW Symbol Tables                         |                                          |
| *Source* | [SymbolTable.cpp](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/SymbolTable.cpp) | The  implementation of member functions in `SymbolTable.h`. |
| *Header* | [SymbolTable.h](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/SymbolTable.h) | A series of classes, which represent the data type stored into the Symbol Table, are defined here. For the inheritance relation of these classes, please view [Symbol Table Classes Architectural](#symbol-table-classes-architectural). |
| *Header* | [DataType.h](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/DataType.h) | Definition of symbol attributes, and data types. |
|          |                                          |                                          |

#### Symbol Table Classes Architectural

In this SW project, symbol tables are using an abstract-class designing idea. Since different symbol contains different fields, like **constant** only needs additional space for storing literal value, both **variable** and **function** need `offset` field for addressing function.

*Notice:* <u>The declarations of classes here might have discrepancy with the original source files. You can view these files personally.</u>  

For an overview of table, view [The Overview of Symbol Table](#the-overview-of-symbol-table).

##### Basic Classes (Bedrock level)

The `CSimpleIdentSymbol` class defines the basic information needed by all the identifiers. 

* `m_Level` contains the level of each identifiers. Since SW is a language supporting nested function (in the future), each symbols have its own calling scope. 
* `m_symbolName` contains the name of the identifier.
* `m_symbolAttribute` is an read-only field. It's used only for classes inheritance.

```C++
class CSimpleIdentSymbol
{
protected:
    size_t m_Level;
    std::wstring m_symbolName;
    SymbolAttribute m_symbolAttribute;
};
```

##### Constant Symbols

The `CConstantSymbol` inherits from `CSimpleIdentSymbol`. Except for the inherited fields,

* `m_constValue` stores the literal value of the constant symbol. When generating virtual machine code files (**\*.asw**), they are used.
* When constructing, `m_symbolAttribute` is configured as **SA_CONSTANT**, which represents a constant symbol.

```C++
class CConstantSymbol : public CSimpleIdentSymbol
{
public:
    CConstantSymbol() :m_constValue(0) { m_symbolAttribute = SA_CONSTANT; }
protected:
    __int64 m_constValue;
};
```

##### Identifier Symbols

This is the base classes used by function and normal variables. 

* `m_dataStatus` contains the data type information, and the reference mark. 
* `m_offset` contains the  offset value. The usage of it vary from function symbols and variable symbols.

```C++
class CIdentSymbol : public CSimpleIdentSymbol
{
public:
    CIdentSymbol() : m_Offset(0) { m_dataStatus.isRef = false; }
protected:
    SymbolStatus m_dataStatus;
    int m_Offset;
};
```

##### Variable Symbols

There doesn't contain any additional fields other than `CIdentSymbol`'s inherent fields. 

* When constructing, `m_symbolAttribute` is configured as **SA_VARIABLE**, which represents a variable symbol.

```C++
class CVariableSymbol : public CIdentSymbol
{
public:
    CVariableSymbol() { m_symbolAttribute = SA_VARIABLE; }
};
```

##### Function Symbols

Functions not only needs `offset` fields, but also needs an array (`std::vector`) to store all it's parameters information. When being called, the compiler will check whether the `call` statement's parameters number matches the correct one of the function's definition.

* `m_paramList` store parameters information. It's an array of SymbolStatus data, which contain data types, reference marks.
* `void AppendParam` member function is used to append new parameter information.

```C++
class CFunctionSymbol : public CIdentSymbol
{
public:
    CFunctionSymbol() { m_symbolAttribute = SA_FUNCTION; }
    void AppendParam(SymbolStatus);
    const std::vector<SymbolStatus> &GetParams();
protected:
    std::vector<SymbolStatus> m_paramList;
};
```

#### The Overview of Symbol Table 

Here gives an example of SW language. It's symbol table are listed in the following.

```Swift
// constant declarations
const max = 100;
const dbl_max = 2 * max;
// variables
var y, x;

func abs(ref y, x)
{
  if x >= 0 { y = x; }
  else { y = -x; };
}

// first input data, and then 
// calc the abs
read(x);
call abs(y, x);
print(y);
```

| Name      | Attribute            | Level | Offset | Value | Parameters Data                          |
| --------- | -------------------- | ----- | ------ | ----- | ---------------------------------------- |
| `max`     | *constant*           | 0     |        | 100   |                                          |
| `dbl_max` | *constant*           | 0     |        | 200   |                                          |
| `y`       | *variable*           | 0     | 3      |       |                                          |
| `x`       | *variable*           | 0     | 4      |       |                                          |
| `abs`     | *function*           | 0     | 2      |       | [(type: "integer", is_ref: true), <br/>(type: "integer", is_ref: false)] |
| `y`       | *variable reference* | 1     | -2     |       |                                          |
| `x`       | *variable*           | 1     | -1     |       |                                          |

* The `offest` for variables are their positions in the running **function / block** stack. 
* The `offset` for functions are their address in the generate output virtual machine code files.
* When an `offset` is an negative number, these variables are parameters. They must be push into the running stack before calling the function (`bl` instruction).


## Virtual Machine Architectural ![SW_VirtualMachine](./res/SW_VirtualMachine.png)
Here defines the design of the SW virtual machine.

### File list

The following lists the source files related to the Virtual Machine.

| Field    | Information                              | Description                              |
| -------- | ---------------------------------------- | ---------------------------------------- |
| Name     | SW Syntax Parser                         |                                          |
| *Header* | [VirtualMachine.h](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/CSyntaxParser.cpp) | Define the class `CRuntimeStack` and `CVirtualMachineInterpreter`, which is the core of the virtual machine. |
| *Header* | [VM_instructions.h](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/VM_instructions.h) | The definition of all the virtual machine instructions. |
| *Source* | [VirtualMachine.cpp](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/VirtualMachine.cpp) | Implementation of functions in `VirtualMachine.h`. |
| *Source* | [VM_instructions.cpp](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/VM_instructions.cpp) | Implementation of functions in `VM_instructions.h`. |
| *Source* | [SWVM_Main.cpp](https://github.com/yzhang37/MyPCompiler/blob/master/PLcomp_x230nz666/Sources/SWVM_Main.cpp) | The main program entrance. It contains `wmain` function. |

### Virtual Machines Instructions

Instructions are used by SW virtual machines. When compiler finishing its work, these code will be used to generate an assembly file. Then Virtual Machine can load these instructions and then perform an execution.

| Instruction | Syntax                 | Description                              |
| ----------- | ---------------------- | ---------------------------------------- |
| `nop`       | nop 0, 0               | Blank instruction. It doesn't do anything. |
| `liti`      | liti 0, *literal*      | Load an literal value.                   |
| `lodi`      | lodi *level*, *offset* | Load value from a variable.              |
| `lori`      | lori *level*, *offset* | Load value from a reference.             |
| `mkri`      | mkri *level*, *offset* | Generate a reference value. It's used when push an <br/>variable to a reference parameters. |
| `stoi`      | lodi *level*, *offset* | Store value to a variable.               |
| `stri`      | stri *level*, *offset* | Store value to a reference.              |
| `b`         | b 0, *addr*            | Jump to an address without any condition. |
| `bc`        | bc 0, *addr*           | Jump to an address with condition.       |
| `bl`        | bl 0, *addr*           | Call a function. <br/>Before `bl` is called, the caller must **pre-push** all the <br/>required parameters, and add a *param_count* as the base. |
| `int`       | int 0, *num*           | Preallocate spaces for variables.        |
| `opr`       | opr *inst*, 0          | Doing a series of operation.             |
| `halt`      | halt 0, 0              | Force exit the program. Not used by the compiler. |

