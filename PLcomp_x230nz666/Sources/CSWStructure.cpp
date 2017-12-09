#include "CSWStructure.h"

CSWStructure::CSWStructure()
{
    push(0, 0);
}

void CSWStructure::AppendSpace(size_t size)
{
    m_structData[m_structData.size() - 1].cSize += size;
}

size_t CSWStructure::GetCurTableBase()
{
    return m_structData[m_structData.size() - 1].tBase;
}

size_t CSWStructure::GetCurVariableSpace()
{
    return m_structData[m_structData.size() - 1].cSize;
}

size_t CSWStructure::GetCurLevel()
{
    return m_structData.size() - 1;
}

size_t CSWStructure::GetCurCodeBase()
{
    return m_structData[m_structData.size() - 1].cBase;
}

void CSWStructure::push(size_t table_base, size_t code_base)
{
    m_structData.push_back(StructData(table_base, code_base));
}

void CSWStructure::pop()
{
    m_structData.pop_back();
}
