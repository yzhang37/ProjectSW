// SW_x230nz666.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "sw_symHeader.h"

ProgramSymSyntax program;
DeclarationListSymSyntax declarationListSymSyntax;
IdentTypeSymSyntax identTypeSymSyntax;
VarDeclarationListSymSyntax varDeclarationListSymSyntax;
VarDeclarationSymSyntax varDeclarationSymSyntax;
VarIdentDeclarationSymSyntax varIdentDeclarationSymSyntax;
ConstDeclarataionListSymSyntax constDeclarataionListSymSyntax;
ConstDeclarationSymSyntax constDeclarationSymSyntax;
ConstIdentDeclarationSymSyntax constIdentDeclarationSymSyntax;
FunctionDeclarationListSymSyntax functionDeclarationListSymSyntax;
ParamIdentDeclarationSymSyntax paramIdentDeclarationSymSyntax;
FunctionBodySymSyntax functionBodySymSyntax;
StatementListSymSyntax statementListSymSyntax;
StatementSymSyntax statementSymSyntax;
IfStatSymSyntax ifStatSymSyntax;
WhileStatSymSyntax whileStatSymSyntax;
RepeatStatSymSyntax repeatStatSymSyntax;
ReadStatSymSyntax readStatSymSyntax;
PrintStatSymSyntax printStatSymSyntax;
AssignStatSymSyntax assignStatSymSyntax;
ForStatSymSyntax forStatSymSyntax;
CallStatSymSyntax callStatSymSyntax;
ConditionSymSyntax conditionSymSyntax;
ExpressionSymSyntax expressionSymSyntax;
TermSymSyntax termSymSyntax;
FactorSymSyntax factorSymSyntax;

void error()
{

}

int main(int argc, wchar_t *argv[])
{
	CLexicalParser lParser(std::_Ptr_wcin);
	// the main lexical parser for the whole program	
	if (lParser.Next())
	{
		ProgramSym(lParser);
	}
	return 0;
}

void ProgramSym(CLexicalParser & lParser)
{
	DeclarationListSym(lParser);
	StatementListSym(lParser);
}

void DeclarationListSym(CLexicalParser & lParser)
{
	ConstDeclarationListSym(lParser);
	VarDeclarationListSym(lParser);
	FunctionDeclarationListSym(lParser);
}

void IdentTypeSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == int_datatype)
	{
		lParser.Next();
	}
	else if (lParser.GetSymbolType() == dec_datatype)
	{
		lParser.Next();
	}
	else
		error();
}

void VarDeclarationListSym(CLexicalParser & lParser)
{
	while (varDeclarationListSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
	{
		VarDeclarationSym(lParser);

		if (lParser.GetSymbolType() == semicolon_op)
		{
			lParser.Next();
		}
		else
			error();
	}
}

void VarDeclarationSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == var_sym)
	{
		lParser.Next();
	}
	else
		error();

	VarIdentDeclarationSym(lParser);

	while (lParser.GetSymbolType() == comma_op)
	{
		lParser.Next();
		VarIdentDeclarationSym(lParser);
	}
}

void VarIdentDeclarationSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == ident_type)
	{
		lParser.Next();
	}
	else
		error();

	if (lParser.GetSymbolType() == eql_op)
	{
		lParser.Next();
		ExpressionSym(lParser);
	}
	else if (lParser.GetSymbolType() == as_sym)
	{
		lParser.Next();
		IdentTypeSym(lParser);
		if (lParser.GetSymbolType() == eql_op)
		{
			lParser.Next();
			ExpressionSym(lParser);
		}
	}
}

void ConstDeclarationListSym(CLexicalParser & lParser)
{
	while (constDeclarationSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
	{
		ConstDeclarationSym(lParser);

		if (lParser.GetSymbolType() == semicolon_op)
		{
			lParser.Next();
		}
		else
			error();
	}
}

void ConstDeclarationSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == const_sym)
	{
		lParser.Next();
	}
	else
		error();

	ConstIdentDeclarationSym(lParser);

	while (lParser.GetSymbolType() == comma_op)
	{
		lParser.Next();
		ConstIdentDeclarationSym(lParser);
	}
}

void ConstIdentDeclarationSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == ident_type)
	{
		lParser.Next();
	}
	else
		error();

	if (lParser.GetSymbolType() == as_sym)
	{
		lParser.Next();
		IdentTypeSym(lParser);
	}

	if (lParser.GetSymbolType() == eql_op)
	{
		lParser.Next();
	}
	else
		error();

	ExpressionSym(lParser);
}

void FunctionDeclarationListSym(CLexicalParser & lParser)
{
	while (lParser.GetSymbolType() == func_sym)
	{
		lParser.Next();

		if (lParser.GetSymbolType() == ident_type)
		{
			lParser.Next();
		}
		else
			error();

		if (lParser.GetSymbolType() == lparen_op)
		{
			lParser.Next();
		}
		else
			error();
		
		if (paramIdentDeclarationSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
		{
			ParamIdentDeclarationSym(lParser);

			while (lParser.GetSymbolType() == comma_op)
			{
				lParser.Next();
				ParamIdentDeclarationSym(lParser);
			}
		}

		if (lParser.GetSymbolType() == rparen_op)
		{
			lParser.Next();
		}
		else
			error();

		if (lParser.GetSymbolType() == lbrace_op)
		{
			lParser.Next();
		}
		else
			error();

		FunctionBodySym(lParser);

		if (lParser.GetSymbolType() == rbrace_op)
		{
			lParser.Next();
		}
		else
			error();
	}
}

void ParamIdentDeclarationSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == ref_sym)
	{
		lParser.Next();
	}

	if (lParser.GetSymbolType() == ident_type)
	{
		lParser.Next();
	}
	else
		error();

	if (lParser.GetSymbolType() == as_sym)
	{
		lParser.Next();
	}
	else
		error();

	IdentTypeSym(lParser);
}

void FunctionBodySym(CLexicalParser & lParser)
{
	VarDeclarationListSym(lParser);
	StatementListSym(lParser);
}

void StatementListSym(CLexicalParser & lParser)
{
	while (statementSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
	{
		StatementSym(lParser);
		if (lParser.GetSymbolType() == semicolon_op)
		{
			lParser.Next();
		}
		else
			error();
	}
}

void StatementSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == continue_sym)
	{
		lParser.Next();
	}
	else if (lParser.GetSymbolType() == break_sym)
	{
		lParser.Next();
	}
	else if (ifStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
	{
		IfStatSym(lParser);
	}
	else if (whileStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
	{
		WhileStatSym(lParser);
	}
	else if (repeatStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
	{
		RepeatStatSym(lParser);
	}
	else if (readStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
	{
		ReadStatSym(lParser);
	}
	else if (printStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
	{
		PrintStatSym(lParser);
	}
	else if (assignStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
	{
		AssignStatSym(lParser);
	}
	else if (forStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
	{
		ForStatSym(lParser);
	}
	else if (callStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
	{
		CallStatSym(lParser);
	}
}

void IfStatSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == if_sym)
	{
		lParser.Next();
	}
	else if (lParser.GetSymbolType() == unless_sym)
	{
		lParser.Next();
	}
	else
		error();

	ConditionSym(lParser);

	if (lParser.GetSymbolType() == lbrace_op)
	{
		lParser.Next();
	}
	else
		error();

	StatementListSym(lParser);

	if (lParser.GetSymbolType() == rbrace_op)
	{
		lParser.Next();
	}
	else
		error();

	if (lParser.GetSymbolType() == else_sym)
	{
		lParser.Next();

		if (lParser.GetSymbolType() == lbrace_op)
		{
			lParser.Next();
		}
		else
			error();

		StatementListSym(lParser);

		if (lParser.GetSymbolType() == rbrace_op)
		{
			lParser.Next();
		}
		else
			error();
	}
}

void WhileStatSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == while_sym)
	{
		lParser.Next();
	}
	else if (lParser.GetSymbolType() == until_sym)
	{
		lParser.Next();
	}
	else
		error();

	ConditionSym(lParser);

	if (lParser.GetSymbolType() == lbrace_op)
	{
		lParser.Next();
	}
	else
		error();

	StatementListSym(lParser);

	if (lParser.GetSymbolType() == rbrace_op)
	{
		lParser.Next();
	}
	else
		error();
}

void RepeatStatSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == repeat_sym)
	{
		lParser.Next();
	}
	else
		error();

	if (lParser.GetSymbolType() == lbrace_op)
	{
		lParser.Next();
	}
	else
		error();

	StatementListSym(lParser);

	if (lParser.GetSymbolType() == rbrace_op)
	{
		lParser.Next();
	}
	else
		error();
}

void ReadStatSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == read_sym)
	{
		lParser.Next();
	}
	else
		error();

	if (lParser.GetSymbolType() == lparen_op)
	{
		lParser.Next();
	}
	else
		error();

	if (lParser.GetSymbolType() == ident_type)
	{
		lParser.Next();
	}
	else
		error();

	if (lParser.GetSymbolType() == rparen_op)
	{
		lParser.Next();
	}
	else
		error();
}

void PrintStatSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == print_sym)
	{
		lParser.Next();
	}
	else
		error();

	if (lParser.GetSymbolType() == lparen_op)
	{
		lParser.Next();
	}
	else
		error();

	if (lParser.GetSymbolType() == ident_type)
	{
		lParser.Next();
	}
	else
		error();

	if (lParser.GetSymbolType() == rparen_op)
	{
		lParser.Next();
	}
	else
		error();
}

void AssignStatSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == ident_type)
	{
		lParser.Next();
	}
	else
		error();

	if (lParser.GetSymbolType() == eql_op)
	{
		lParser.Next();
		ExpressionSym(lParser);
	}
	else if (lParser.GetSymbolType() == dblplus_op)
	{
		lParser.Next();
	}
	else if (lParser.GetSymbolType() == dblminus_op)
	{
		lParser.Next();
	}
	else
		error();
}

void ForStatSym(CLexicalParser & lParser)
{
	// for
	if (lParser.GetSymbolType() == for_sym)
	{
		lParser.Next();
	}
	else
		error();

	// ident
	if (lParser.GetSymbolType() == ident_type)
	{
		lParser.Next();
	}
	else
		error();

	// in
	if (lParser.GetSymbolType() == in_sym)
	{
		lParser.Next();
	}
	else
		error();

	if (lParser.GetSymbolType() == int_val)
	{
		lParser.Next();
	}
	else if (lParser.GetSymbolType() == dec_val)
	{
		lParser.Next();
	}
	else if (lParser.GetSymbolType() == ident_type)
	{
		lParser.Next();
	}
	else
		error(); // want a number or a symbol

	if (lParser.GetSymbolType() == to_op)
	{
		lParser.Next();
	}
	else
		// lack of ...
		error();

	if (lParser.GetSymbolType() == int_val)
	{
		lParser.Next();
	}
	else if (lParser.GetSymbolType() == dec_val)
	{
		lParser.Next();
	}
	else if (lParser.GetSymbolType() == ident_type)
	{
		lParser.Next();
	}
	else
		error(); // want a number or a symbol

	if (lParser.GetSymbolType() == lparen_op)
	{
		lParser.Next();
		StatementListSym(lParser);
	}
	else
		error();

	if (lParser.GetSymbolType() == rparen_op)
	{
		lParser.Next();
	}
	else
		error();
}

void CallStatSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == call_sym)
	{
		lParser.Next();
		if (lParser.GetSymbolType() == ident_type)
		{
			lParser.Next();
			if (lParser.GetSymbolType() == lparen_op)
			{
				lParser.Next();

				if (expressionSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
				{
					ExpressionSym(lParser);

					while (lParser.GetSymbolType() == comma_op)
					{
						lParser.Next();
						ExpressionSym(lParser);
					}
				}

				if (lParser.GetSymbolType() == rparen_op)
				{
					lParser.Next();
				}
				else
					error();
			}
			else
				error();
		}
		else
			error();
	}
}

void ConditionSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == odd_sym)
	{
		lParser.Next();
		if (lParser.GetSymbolType() == lparen_op)
		{
			lParser.Next();
			ExpressionSym(lParser);
			if (lParser.GetSymbolType() == rparen_op)
			{
				lParser.Next();
			}
			else
				error();
		}
		else
			error();
	}
	else
	{
		ExpressionSym(lParser);

		if (lParser.GetSymbolType() == eql_op)
		{
			lParser.Next();
		}
		else if (lParser.GetSymbolType() == neq_op)
		{
			lParser.Next();
		}
		else if (lParser.GetSymbolType() == lss_op)
		{
			lParser.Next();
		}
		else if (lParser.GetSymbolType() == leq_op)
		{
			lParser.Next();
		}
		else if (lParser.GetSymbolType() == gtr_op)
		{
			lParser.Next();
		}
		else if (lParser.GetSymbolType() == geq_op)
		{
			lParser.Next();
		}
		else
			error();
		ExpressionSym(lParser);
	}
}

void ExpressionSym(CLexicalParser & lParser)
{
	if (lParser.GetSymbolType() == plus_op ||
		lParser.GetSymbolType() == minus_op)
	{
		lParser.Next();
	}
	TermSym(lParser);
	while (lParser.GetSymbolType() == plus_op ||
		lParser.GetSymbolType() == minus_op)
	{
		lParser.Next();
		TermSym(lParser);
	}
}

void TermSym(CLexicalParser & lParser)
{
	FactorSym(lParser);
	while (lParser.GetSymbolType() == times_op ||
		lParser.GetSymbolType() == slash_op)
	{
		lParser.Next();
		FactorSym(lParser);
	}
}

void FactorSym(CLexicalParser &lParser)
{
	if (lParser.GetSymbolType() == ident_type)
	{
		lParser.Next();
	}
	else if (lParser.GetSymbolType() == int_val ||
		lParser.GetSymbolType() == dec_val)
	{
		lParser.Next();
	}
	else if (lParser.GetSymbolType() == lparen_op)
	{
		lParser.Next();
		ExpressionSym(lParser);
		if (lParser.GetSymbolType() == rparen_op)
		{
			lParser.Next();
		}
		else
			error();
	}
	else
		error();
}