#pragma once

#include <set>
#include <map>
#include <vector>
#include <cmath>

class CLexicalParser
{
public:
	enum SymbolType {
		nul, ident_type, number_type, funcident_type, voidfuncident_type,
		int_datatype, dec_datatype, bool_datatype, true_val, false_val,
		int_val, dec_val, 
		plus_op, minus_op, times_op, slash_op, mod_op, eql_op, neq_op,
		lss_op, leq_op, gtr_op, geq_op, lparen_op, rparen_op, lbrace_op,
		rbrace_op, comma_op, semicolon_op, colon_op, period_op, and_op, 
		or_op, xor_op, dblplus_op, dblminus_op, to_op, 
		if_sym, unless_sym, else_sym, while_sym, repeat_sym, until_sym,
		break_sym, continue_sym, for_sym, in_sym, print_sym, read_sym,
		return_sym, switch_sym, case_sym, not_sym, odd_sym, call_sym,
		const_sym, ref_sym, var_sym, as_sym, func_sym
	};
	struct SymbolInfo
	{
		std::wstring sym_name;
	};
public:
	CLexicalParser(std::wistream *);
	bool Next();
	// properties:
public:
	const std::wstring & GetIdent() const;
	const SymbolType GetSymbolType() const;
	const __int64 GetInteger() const;
	const double GetDecimal() const;
	const wchar_t * GetSymbol() const;
private:
	wchar_t m_curCh;

	SymbolType m_curSymbolType;
	__int64 m_curSymbolInt;
	double m_curSymbolDec;

	std::wstring m_wstrCurSymbol;


	std::set <std::wstring> m_setReserved;
	std::map <wchar_t, SymbolType> m_msSym;
	std::map <std::wstring, SymbolType> m_mwSym;
	std::wistream *m_isInput;

	// helpers
private:
	// add keywords and map
	
	inline void insertKeyWords(const std::wstring &, SymbolType);
	void error();
	inline void resetvalues();
	inline void getnextc();
};

