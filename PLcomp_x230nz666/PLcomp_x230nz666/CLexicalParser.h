#pragma once
#include "stdafx.h"
#include "IdentType.h"

class CLexicalParser
{
public:
	struct SymbolInfo
	{
		std::wstring sym_name;
	};
public:
	CLexicalParser(std::wistream *);
	bool Next();
	// properties:
public:
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

