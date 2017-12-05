#include "stdafx.h"
#include "CSyntaxParser.h"

const std::set<SymbolType>& CSimpleSyntax::GetFirst() const
{
	return m_setFirst;
}

const std::set<SymbolType>& CSimpleSyntax::GetFollow() const
{
	return m_setFollow;
}

ProgramSymSyntax::ProgramSymSyntax()
{
	m_setFirst.insert(const_sym);
	m_setFirst.insert(var_sym);
	m_setFirst.insert(func_sym);
	m_setFirst.insert(if_sym);
	m_setFirst.insert(unless_sym);
	m_setFirst.insert(while_sym);
	m_setFirst.insert(until_sym);
	m_setFirst.insert(repeat_sym);
	m_setFirst.insert(read_sym);
	m_setFirst.insert(print_sym);
	m_setFirst.insert(for_sym);
	m_setFirst.insert(call_sym);
	m_setFirst.insert(continue_sym);
	m_setFirst.insert(break_sym);
	m_setFirst.insert(ident_type);
	
	m_setFollow.insert(eof);
}

DeclarationListSymSyntax::DeclarationListSymSyntax()
{
	m_setFirst.insert(const_sym);
	m_setFirst.insert(var_sym);
	m_setFirst.insert(func_sym);

	m_setFollow.insert(eof);
	m_setFollow.insert(if_sym);
	m_setFollow.insert(unless_sym);
	m_setFollow.insert(while_sym);
	m_setFollow.insert(until_sym);
	m_setFollow.insert(repeat_sym);
	m_setFollow.insert(read_sym);
	m_setFollow.insert(print_sym);
	m_setFollow.insert(for_sym);
	m_setFollow.insert(call_sym);
	m_setFollow.insert(continue_sym);
	m_setFollow.insert(break_sym);
	m_setFollow.insert(ident_type);
}

IdentTypeSymSyntax::IdentTypeSymSyntax()
{
	m_setFirst.insert(int_datatype);
	m_setFirst.insert(dec_datatype);

	m_setFollow.insert(eql_op);
	m_setFollow.insert(semicolon_op);
	m_setFollow.insert(comma_op);
	m_setFollow.insert(rparen_op);
}

VarDeclarationListSymSyntax::VarDeclarationListSymSyntax()
{
	m_setFirst.insert(var_sym);

	m_setFollow.insert(eof);
	m_setFollow.insert(const_sym);
	m_setFollow.insert(func_sym);
	m_setFollow.insert(if_sym);
	m_setFollow.insert(unless_sym);
	m_setFollow.insert(while_sym);
	m_setFollow.insert(until_sym);
	m_setFollow.insert(repeat_sym);
	m_setFollow.insert(read_sym);
	m_setFollow.insert(print_sym);
	m_setFollow.insert(for_sym);
	m_setFollow.insert(call_sym);
	m_setFollow.insert(continue_sym);
	m_setFollow.insert(break_sym);
	m_setFollow.insert(ident_type);
}

VarDeclarationSymSyntax::VarDeclarationSymSyntax()
{
	m_setFirst.insert(var_sym);

	m_setFollow.insert(semicolon_op);
}

VarIdentDeclarationSymSyntax::VarIdentDeclarationSymSyntax()
{
	m_setFirst.insert(int_datatype);
	m_setFirst.insert(dec_datatype);

	m_setFollow.insert(semicolon_op);
	m_setFollow.insert(comma_op);
}

ConstDeclarataionListSymSyntax::ConstDeclarataionListSymSyntax()
{
	m_setFirst.insert(const_sym);

	m_setFollow.insert(eof);
	m_setFollow.insert(func_sym);
	m_setFollow.insert(if_sym);
	m_setFollow.insert(unless_sym);
	m_setFollow.insert(while_sym);
	m_setFollow.insert(until_sym);
	m_setFollow.insert(repeat_sym);
	m_setFollow.insert(read_sym);
	m_setFollow.insert(print_sym);
	m_setFollow.insert(for_sym);
	m_setFollow.insert(call_sym);
	m_setFollow.insert(continue_sym);
	m_setFollow.insert(break_sym);
	m_setFollow.insert(ident_type);
}

ConstDeclarationSymSyntax::ConstDeclarationSymSyntax()
{
	m_setFirst.insert(const_sym);

	m_setFollow.insert(semicolon_op);
}

ConstIdentDeclarationSymSyntax::ConstIdentDeclarationSymSyntax()
{
	m_setFirst.insert(int_datatype);
	m_setFirst.insert(dec_datatype);

	m_setFollow.insert(semicolon_op);
	m_setFollow.insert(comma_op);
}

FunctionDeclarationListSymSyntax::FunctionDeclarationListSymSyntax()
{
	m_setFirst.insert(func_sym);

	m_setFollow.insert(eof);
	m_setFollow.insert(if_sym);
	m_setFollow.insert(unless_sym);
	m_setFollow.insert(while_sym);
	m_setFollow.insert(until_sym);
	m_setFollow.insert(repeat_sym);
	m_setFollow.insert(read_sym);
	m_setFollow.insert(print_sym);
	m_setFollow.insert(for_sym);
	m_setFollow.insert(call_sym);
	m_setFollow.insert(continue_sym);
	m_setFollow.insert(break_sym);
	m_setFollow.insert(ident_type);
}

ParamIdentDeclarationSymSyntax::ParamIdentDeclarationSymSyntax()
{
	m_setFirst.insert(ref_sym);
	m_setFirst.insert(int_datatype);
	m_setFirst.insert(dec_datatype);

	m_setFollow.insert(comma_op);
	m_setFollow.insert(rparen_op);
}

FunctionBodySymSyntax::FunctionBodySymSyntax()
{
	m_setFirst.insert(var_sym);

	m_setFollow.insert(rbrace_op);
}

StatementListSymSyntax::StatementListSymSyntax()
{
	m_setFirst.insert(if_sym);
	m_setFirst.insert(unless_sym);
	m_setFirst.insert(while_sym);
	m_setFirst.insert(until_sym);
	m_setFirst.insert(repeat_sym);
	m_setFirst.insert(read_sym);
	m_setFirst.insert(print_sym);
	m_setFirst.insert(for_sym);
	m_setFirst.insert(call_sym);
	m_setFirst.insert(continue_sym);
	m_setFirst.insert(break_sym);
	m_setFirst.insert(ident_type);

	m_setFollow.insert(eof);
	m_setFollow.insert(semicolon_op);
}

StatementSymSyntax::StatementSymSyntax()
{
	m_setFirst.insert(if_sym);
	m_setFirst.insert(unless_sym);
	m_setFirst.insert(while_sym);
	m_setFirst.insert(until_sym);
	m_setFirst.insert(repeat_sym);
	m_setFirst.insert(read_sym);
	m_setFirst.insert(print_sym);
	m_setFirst.insert(for_sym);
	m_setFirst.insert(call_sym);
	m_setFirst.insert(continue_sym);
	m_setFirst.insert(break_sym);
	m_setFirst.insert(ident_type);

	m_setFollow.insert(semicolon_op);
}

IfStatSymSyntax::IfStatSymSyntax()
{
	m_setFirst.insert(if_sym);
	m_setFirst.insert(unless_sym);

	m_setFollow.insert(semicolon_op);
}

WhileStatSymSyntax::WhileStatSymSyntax()
{
	m_setFirst.insert(while_sym);
	m_setFirst.insert(until_sym);

	m_setFollow.insert(semicolon_op);
}

RepeatStatSymSyntax::RepeatStatSymSyntax()
{
	m_setFirst.insert(repeat_sym);

	m_setFollow.insert(semicolon_op);
}

ReadStatSymSyntax::ReadStatSymSyntax()
{
	m_setFirst.insert(read_sym);

	m_setFollow.insert(semicolon_op);
}

PrintStatSymSyntax::PrintStatSymSyntax()
{
	m_setFirst.insert(print_sym);

	m_setFollow.insert(semicolon_op);
}

AssignStatSymSyntax::AssignStatSymSyntax()
{
	m_setFirst.insert(ident_type);

	m_setFollow.insert(semicolon_op);
}

ForStatSymSyntax::ForStatSymSyntax()
{
	m_setFirst.insert(for_sym);

	m_setFollow.insert(semicolon_op);
}

CallStatSymSyntax::CallStatSymSyntax()
{
	m_setFirst.insert(call_sym);

	m_setFollow.insert(semicolon_op);
}

ConditionSymSyntax::ConditionSymSyntax()
{
	m_setFirst.insert(ident_type);
	m_setFirst.insert(int_val);
	m_setFirst.insert(dec_val);
	m_setFirst.insert(lparen_op);
	m_setFirst.insert(plus_op);
	m_setFirst.insert(minus_op);
	m_setFirst.insert(odd_sym);

	m_setFollow.insert(rbrace_op);
	m_setFollow.insert(semicolon_op);
}

ExpressionSymSyntax::ExpressionSymSyntax()
{
	m_setFirst.insert(ident_type);
	m_setFirst.insert(int_val);
	m_setFirst.insert(dec_val);
	m_setFirst.insert(lparen_op);
	m_setFirst.insert(plus_op);
	m_setFirst.insert(minus_op);

	m_setFollow.insert(semicolon_op);
	m_setFollow.insert(comma_op);
	m_setFollow.insert(rparen_op);
}

TermSymSyntax::TermSymSyntax()
{
	m_setFirst.insert(ident_type);
	m_setFirst.insert(int_val);
	m_setFirst.insert(dec_val);
	m_setFirst.insert(lparen_op);

	m_setFollow.insert(plus_op);
	m_setFollow.insert(minus_op);
	m_setFollow.insert(semicolon_op);
	m_setFollow.insert(comma_op);
	m_setFollow.insert(rparen_op);
}

FactorSymSyntax::FactorSymSyntax()
{
	m_setFirst.insert(ident_type);
	m_setFirst.insert(int_val);
	m_setFirst.insert(dec_val);
	m_setFirst.insert(lparen_op);

	m_setFollow.insert(times_op);
	m_setFollow.insert(slash_op);
	m_setFollow.insert(plus_op);
	m_setFollow.insert(minus_op);
	m_setFollow.insert(semicolon_op);
	m_setFollow.insert(comma_op);
	m_setFollow.insert(rparen_op);
}
