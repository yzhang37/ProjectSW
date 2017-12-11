#pragma once
#include "stdafx.h"
#include "IdentType.h"

class CLexicalParser
{
    // declaration of classes
public:
	struct SymbolInfo
	{
		std::wstring sym_name;
	};
protected:
    struct TextCursor
    {
        TextCursor();
        inline void append(size_t);
        inline void feed();
        inline void reset();
        size_t line;
        size_t col;
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
    size_t GetCurrentLineNo() const;
    size_t GetCurrentColumnNo() const;
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
    TextCursor textcur;
	// helpers
private:
	inline void insertKeyWords(const std::wstring &, SymbolType);
	void _makeError();
	inline void resetvalues();
	inline void getnextc();
};

