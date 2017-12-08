#pragma once
#include "stdafx.h"
#include "CLexicalParser.h"
#include "IdentType.h"

class CSimpleSyntax
{
public:
	virtual const std::set<SymbolType> & GetFirst() const;
	virtual const std::set<SymbolType> & GetFollow() const;
protected:
	std::set<SymbolType> m_setFirst;
	std::set<SymbolType> m_setFollow;
};

// program = declaration_list statement_list.
struct ProgramSymSyntax : CSimpleSyntax
{
	ProgramSymSyntax();
};

//declaration_list = const_declaration_list
//                   var_declaration_list
//                   function_declaration_list.
struct DeclarationListSymSyntax : CSimpleSyntax
{
	DeclarationListSymSyntax();
};

//var_declaration_list = { var_declaration ";" }.
struct VarDeclarationListSymSyntax : CSimpleSyntax
{
	VarDeclarationListSymSyntax();
};

//var_declaration = "var" var_ident_declaration
//                        {"," var_ident_declaration }.
struct VarDeclarationSymSyntax : CSimpleSyntax
{
	VarDeclarationSymSyntax();
};

//var_ident_declaration = ident ["=" expression | 
//                               "as" ident_type |
//                               "as" ident_type "=" expression ].
struct VarIdentDeclarationSymSyntax : CSimpleSyntax
{
	VarIdentDeclarationSymSyntax();
};

//const_declaration_list = { const_declaration ";" }.
struct ConstDeclarataionListSymSyntax : CSimpleSyntax
{
	ConstDeclarataionListSymSyntax();
};

//const_declaration = "const" const_ident_declaration
//                            {"," const_ident_declaration }.
struct ConstDeclarationSymSyntax : CSimpleSyntax
{
	ConstDeclarationSymSyntax();
};

//const_ident_declaration = ident ["as" ident_type ] "=" expression .
struct ConstIdentDeclarationSymSyntax : CSimpleSyntax
{
	ConstIdentDeclarationSymSyntax();
};

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
struct FunctionDeclarationListSymSyntax : CSimpleSyntax
{
	FunctionDeclarationListSymSyntax();
};

// param_ident_declaration = ["ref"] ident "as" ident_type .
struct ParamIdentDeclarationSymSyntax : CSimpleSyntax
{
	ParamIdentDeclarationSymSyntax();
};

// function_body = var_declaration_list statement_list.
struct FunctionBodySymSyntax : CSimpleSyntax
{
	FunctionBodySymSyntax();
};

// statement_list = { statement ";"}.
struct StatementListSymSyntax : CSimpleSyntax
{
	StatementListSymSyntax();
};

// statement = if_stat | while_stat | 
//             repeat_stat | read_stat | 
//             print_stat | assign_stat |
//             for_stat | call_stat |
//             "continue" | "break" .
struct StatementSymSyntax : CSimpleSyntax
{
	StatementSymSyntax();
};

// if_stat = ( "if" | "unless" ) condition "{" statement_list "}" 
//           ["else" "{" statement_list "}"].
struct IfStatSymSyntax : CSimpleSyntax
{
	IfStatSymSyntax();
};

// while_stat = ( "while" | "until" ) condition "{" statement_list "}".
struct WhileStatSymSyntax : CSimpleSyntax
{
	WhileStatSymSyntax();
};

// repeat_stat = "repeat" "{" statement_list "}"
// ("while" | "until") condition.
struct RepeatStatSymSyntax : CSimpleSyntax
{
	RepeatStatSymSyntax();
};

// read_stat = "read" "(" ident ")".
struct ReadStatSymSyntax : CSimpleSyntax
{
	ReadStatSymSyntax();
};

// print_stat = "print" "(" ident ")".
struct PrintStatSymSyntax : CSimpleSyntax
{
	PrintStatSymSyntax();
};

// assign_stat = ident ( "=" expression | "++" | "--" ).
struct AssignStatSymSyntax : CSimpleSyntax
{
	AssignStatSymSyntax();
};

// for_stat = "for" ident "in" (ident|number) "..." (ident|number) "{" statement_list "}".
struct ForStatSymSyntax : CSimpleSyntax
{
	ForStatSymSyntax();
};

// call_stat = "call" ident "(" [ expression { "," expression }  ] ")".
struct CallStatSymSyntax : CSimpleSyntax
{
	CallStatSymSyntax();
};

// condition = expression ("=="|"!="|"<"|"<="|">"|">=") expression | "odd" "(" expression ")".
struct ConditionSymSyntax : CSimpleSyntax
{
	ConditionSymSyntax();
};

// expression = ["+"|"-"] term { ("+"|"-") term}.
struct ExpressionSymSyntax : CSimpleSyntax
{
	ExpressionSymSyntax();
};

// term = factor { ("*"|"/") factor}.
struct TermSymSyntax : CSimpleSyntax
{
	TermSymSyntax();
};

//factor = ident | number | "(" expression ")".
struct FactorSymSyntax : CSimpleSyntax
{
	FactorSymSyntax();
};

