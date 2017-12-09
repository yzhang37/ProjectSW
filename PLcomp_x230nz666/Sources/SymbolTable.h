#pragma once
#include "stdafx.h"
#include "DataType.h"

// The bedrock class about data.
class CSimpleIdentSymbol
{
public:
    CSimpleIdentSymbol() :m_Level(0) {}
    PropertyHelper(Name, m_symbolName, std::wstring)
    PropertyHelper(Level, m_Level, int)
protected:
    int m_Level;
    std::wstring m_symbolName;
    SymbolAttribute m_symbolAttribute;
};

// An Class represent A Constant Data.
class CConstantSymbol : public CSimpleIdentSymbol
{
public:
    CConstantSymbol() :m_constValue(0) { m_symbolAttribute = SA_CONSTANT; }
    PropertyHelper(Value, m_constValue, __int64)
protected:
    __int64 m_constValue;
};

class CIdentSymbol : public CSimpleIdentSymbol
{
public:
    CIdentSymbol() : m_Offset(0) {}

    //properties:
    PropertyHelper(Offset, m_Offset, int)
    PropertyHelper(Status, m_dataStatus, SymbolStatus)
protected:
    SymbolStatus m_dataStatus;
    int m_Offset; // The address of the symbol in level
};

class CVariableSymbol : public CIdentSymbol
{
public:
    CVariableSymbol() { m_symbolAttribute = SA_VARIABLE; }
};

class CFunctionSymbol : public CIdentSymbol
{
public:
    CFunctionSymbol() { m_symbolAttribute = SA_FUNCTION; }
protected:
    std::vector<SymbolStatus> m_paramList;
};

class CSymbolTable
{
public:
    ~CSymbolTable();
    size_t GetTableSize();
    bool EnterSymbol(CSimpleIdentSymbol *);
protected:
    std::vector<CSimpleIdentSymbol *> m_tableData;
};
