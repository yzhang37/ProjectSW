#pragma once
#include "CLexicalParser.h"
#include "CSyntaxParser.h"
#include "VM_instructions.h"
#include "SymbolTable.h"
#include "IdentType.h"
#include "DataType.h"

// program = declaration_list statement_list.
void ProgramSym(CLexicalParser &lParser, CSymbolTable & sTable);

//declaration_list = const_declaration_list
//                   var_declaration_list
//                   function_declaration_list.
void DeclarationListSym(CLexicalParser &lParser, CSymbolTable & sTable);

//var_declaration_list = { var_declaration ";" }.
void VarDeclarationListSym(CLexicalParser &lParser, CSymbolTable & sTable);

//var_declaration = "var" var_ident_declaration
//                        {"," var_ident_declaration }.
void VarDeclarationSym(CLexicalParser &lParser, CSymbolTable & sTable);

//var_ident_declaration = ident ["=" expression | 
//                               "as" ident_type |
//                               "as" ident_type "=" expression ].
void VarIdentDeclarationSym(CLexicalParser &lParser, CSymbolTable & sTable);

//const_declaration_list = { const_declaration ";" }.
void ConstDeclarationListSym(CLexicalParser &lParser, CSymbolTable & sTable);

//const_declaration = "const" const_ident_declaration
//                            {"," const_ident_declaration }.
void ConstDeclarationSym(CLexicalParser &lParser, CSymbolTable & sTable);

//const_ident_declaration = ident ["as" ident_type ] "=" expression .
void ConstIdentDeclarationSym(CLexicalParser &lParser, CSymbolTable & sTable);

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
void FunctionDeclarationListSym(CLexicalParser &lParser, CSymbolTable & sTable);

// param_ident_declaration = ["ref"] ident "as" ident_type .
void ParamIdentDeclarationSym(CLexicalParser &lParser, CSymbolTable & sTable);

// function_body = var_declaration_list statement_list.
void FunctionBodySym(CLexicalParser &lParser, CSymbolTable & sTable);

// statement_list = { statement ";"}.
void StatementListSym(CLexicalParser &lParser, CSymbolTable & sTable);

// statement = if_stat | while_stat | 
//             repeat_stat | read_stat | 
//             print_stat | assign_stat |
//             for_stat | call_stat |
//             "continue" | "break" .
void StatementSym(CLexicalParser &lParser, CSymbolTable & sTable);

// if_stat = ( "if" | "unless" ) condition "{" statement_list "}" 
//           ["else" "{" statement_list "}"].
void IfStatSym(CLexicalParser &lParser, CSymbolTable & sTable);

// while_stat = ( "while" | "until" ) condition "{" statement_list "}".
void WhileStatSym(CLexicalParser &lParser, CSymbolTable & sTable);

// repeat_stat = "repeat" "{" statement_list "}"
// ("while" | "until") condition.
void RepeatStatSym(CLexicalParser &lParser, CSymbolTable & sTable);

// read_stat = "read" "(" ident ")".
void ReadStatSym(CLexicalParser &lParser, CSymbolTable & sTable);

// print_stat = "print" "(" ident ")".
void PrintStatSym(CLexicalParser &lParser, CSymbolTable & sTable);

// assign_stat = ident ( "=" expression | "++" | "--" ).
void AssignStatSym(CLexicalParser &lParser, CSymbolTable & sTable);

// for_stat = "for" ident "in" (ident|number) "..." (ident|number) "{" statement_list "}".
void ForStatSym(CLexicalParser &lParser, CSymbolTable & sTable);

// call_stat = "call" ident "(" [ expression { "," expression }  ] ")".
void CallStatSym(CLexicalParser &lParser, CSymbolTable & sTable);

// condition = expression ("=="|"!="|"<"|"<="|">"|">=") expression | "odd" "(" expression ")".
void ConditionSym(CLexicalParser &lParser, CSymbolTable & sTable);

// expression = ["+"|"-"] term { ("+"|"-") term}.
void ExpressionSym(CLexicalParser &lParser, CSymbolTable & sTable, bool *isConst = nullptr, __int64* rValue = nullptr);

// term = factor { ("*"|"/") factor}.
void TermSym(CLexicalParser &lParser, CSymbolTable & sTable, bool *isConst = nullptr, __int64* rValue = nullptr);

//factor = ident | number | "(" expression ")".
void FactorSym(CLexicalParser &lParser, CSymbolTable & sTable, bool *isConst = nullptr, __int64* rValue = nullptr);