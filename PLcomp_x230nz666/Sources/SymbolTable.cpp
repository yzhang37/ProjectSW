#include "stdafx.h"
#include "SymbolTable.h"

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

CSymbolTable::~CSymbolTable()
{
    for (auto pSymbol : m_tableData)
    {
        if (pSymbol)
            delete pSymbol;
    }
}

size_t CSymbolTable::GetTableSize()
{
    return m_tableData.size();
}

bool CSymbolTable::EnterSymbol(CSimpleIdentSymbol * pSymbol)
{
    for (auto existed : m_tableData)
    {
        if (existed->GetName() == pSymbol->GetName() &&
            existed->GetLevel() == pSymbol->GetLevel())
        {
            // have the same name and the same level
            return false;
        }
    }
    m_tableData.push_back(pSymbol);
    return true;
}
