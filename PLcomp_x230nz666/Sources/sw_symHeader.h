#pragma once
#include "CLexicalParser.h"
#include "CSyntaxParser.h"
#include "IdentType.h"
#include "DataType.h"

// program = declaration_list statement_list.
void ProgramSym(CLexicalParser &lParser);

//declaration_list = const_declaration_list
//                   var_declaration_list
//                   function_declaration_list.
void DeclarationListSym(CLexicalParser &lParser);

//var_declaration_list = { var_declaration ";" }.
void VarDeclarationListSym(CLexicalParser &lParser);

//var_declaration = "var" var_ident_declaration
//                        {"," var_ident_declaration }.
void VarDeclarationSym(CLexicalParser &lParser);

//var_ident_declaration = ident ["=" expression | 
//                               "as" ident_type |
//                               "as" ident_type "=" expression ].
void VarIdentDeclarationSym(CLexicalParser &lParser);

//const_declaration_list = { const_declaration ";" }.
void ConstDeclarationListSym(CLexicalParser &lParser);

//const_declaration = "const" const_ident_declaration
//                            {"," const_ident_declaration }.
void ConstDeclarationSym(CLexicalParser &lParser);

//const_ident_declaration = ident ["as" ident_type ] "=" expression .
void ConstIdentDeclarationSym(CLexicalParser &lParser);

// function_declaration_list = 
//     { "func" ident "(" 
//         [ 
//             param_ident_declaration 
//             {"," param_ident_declaration }
//         ] ")" 
//         "{" 
//             function_body
//         "}" 
//     }.
void FunctionDeclarationListSym(CLexicalParser &lParser);

// param_ident_declaration = ["ref"] ident "as" ident_type .
void ParamIdentDeclarationSym(CLexicalParser &lParser);

// function_body = var_declaration_list statement_list.
void FunctionBodySym(CLexicalParser &lParser);

// statement_list = { statement ";"}.
void StatementListSym(CLexicalParser &lParser);

// statement = if_stat | while_stat | 
//             repeat_stat | read_stat | 
//             print_stat | assign_stat |
//             for_stat | call_stat |
//             "continue" | "break" .
void StatementSym(CLexicalParser &lParser);

// if_stat = ( "if" | "unless" ) condition "{" statement_list "}" 
//           ["else" "{" statement_list "}"].
void IfStatSym(CLexicalParser &lParser);

// while_stat = ( "while" | "until" ) condition "{" statement_list "}".
void WhileStatSym(CLexicalParser &lParser);

// repeat_stat = "repeat" "{" statement_list "}"
// ("while" | "until") condition.
void RepeatStatSym(CLexicalParser &lParser);

// read_stat = "read" "(" ident ")".
void ReadStatSym(CLexicalParser &lParser);

// print_stat = "print" "(" ident ")".
void PrintStatSym(CLexicalParser &lParser);

// assign_stat = ident ( "=" expression | "++" | "--" ).
void AssignStatSym(CLexicalParser &lParser);

// for_stat = "for" ident "in" (ident|number) "..." (ident|number) "{" statement_list "}".
void ForStatSym(CLexicalParser &lParser);

// call_stat = "call" ident "(" [ expression { "," expression }  ] ")".
void CallStatSym(CLexicalParser &lParser);

// condition = expression ("=="|"!="|"<"|"<="|">"|">=") expression | "odd" "(" expression ")".
void ConditionSym(CLexicalParser &lParser);

// expression = ["+"|"-"] term { ("+"|"-") term}.
void ExpressionSym(CLexicalParser &lParser);

// term = factor { ("*"|"/") factor}.
void TermSym(CLexicalParser &lParser);

//factor = ident | number | "(" expression ")".
void FactorSym(CLexicalParser &lParser);