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

void CSWStructure::PushStashCodes(std::vector<CVirtualMachineInstruction> & codeList)
{
    auto & stashList = m_structData[m_structData.size() - 1].m_codeStashed;
    for (size_t i = GetCurCodeBase() + 1; i < codeList.size(); ++i)
    {
        stashList.push_back(codeList[i]);
    }
    codeList.resize(GetCurCodeBase() + 1);
}

void CSWStructure::PopStashed(std::vector<CVirtualMachineInstruction> & codeList)
{
    for (auto code : m_structData[m_structData.size() - 1].m_codeStashed)
    {
        codeList.push_back(code);
    }
    m_structData[m_structData.size() - 1].m_codeStashed.clear();
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
    if (m_structData.size() > 0)
        m_structData.pop_back();
}

void CSWLoopStructure::push()
{
    m_structData.push_back(LoopStructData());
}

bool CSWLoopStructure::addContinue(size_t line)
{
    if (m_structData.size() > 0)
    {
        m_structData[m_structData.size() - 1].continueList.push_back(line);
        return true;
    }
    else
        return false;
}

bool CSWLoopStructure::addBreak(size_t line)
{
    if (m_structData.size() > 0)
    {
        m_structData[m_structData.size() - 1].breakExitList.push_back(line);
        return true;
    }
    else
        return false;
}

void CSWLoopStructure::pop(size_t selfopLine, size_t exitLine, std::vector<CVirtualMachineInstruction> & codeList)
{
    if (m_structData.size() > 0)
    {
        for (auto continue_port : m_structData[m_structData.size() - 1].continueList)
            codeList[continue_port].param_b = selfopLine;
        for (auto exit_port : m_structData[m_structData.size() - 1].breakExitList)
            codeList[exit_port].param_b = exitLine;
        m_structData.pop_back();
    }
}
