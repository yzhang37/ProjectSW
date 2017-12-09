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

CSimpleIdentSymbol * CSymbolTable::FindLatest(const std::wstring & str)
{
    for (size_t i = m_tableData.size(); i > 0; --i)
    {
        if (m_tableData[i - 1]->GetName() == str)
            return m_tableData[i - 1];
    }
    return nullptr;
}

CFunctionSymbol * CSymbolTable::GetLastFunction()
{
    for (size_t i = m_tableData.size(); i > 0; --i)
    {
        if (m_tableData[i - 1]->GetAttribute() == SA_FUNCTION)
            return (CFunctionSymbol *)m_tableData[i - 1];
    }
    return nullptr;
}

void CSymbolTable::PruneTo(size_t level)
{
    for (size_t i = m_tableData.size(); i > 0; --i)
    {
        if (m_tableData[i - 1]->GetLevel() == level)
        {
            m_tableData.resize(i);
            return;
        }
    }
}

void CFunctionSymbol::AppendParam(SymbolStatus status)
{
    m_paramList.push_back(status);
}

const std::vector<SymbolStatus>& CFunctionSymbol::GetParams()
{
    return m_paramList;
}
