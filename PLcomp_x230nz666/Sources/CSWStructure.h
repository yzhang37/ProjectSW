#pragma once
#include "stdafx.h"
#include "VirtualMachine.h"

class CSWStructure
{
private:
    struct StructData
    {
        StructData(size_t tb, size_t cb) :tBase(tb), cBase(cb), cSize(0) {}
        size_t tBase;
        size_t cBase;
        size_t cSize;
        std::vector<CVirtualMachineInstruction> m_codeStashed;
    };

public:
    CSWStructure();
    // allocate more space.
    void AppendSpace(size_t size);
    // in the current, in the future when the program
    // want to fill back the jump position, call GetCurCodeBase function.
    // This maintain the current base position.
    size_t GetCurCodeBase();
    // get the base position in the table.
    size_t GetCurTableBase();
    // get the space
    size_t GetCurVariableSpace();

    // Get current block level
    size_t GetCurLevel();

    void PushStashCodes(std::vector<CVirtualMachineInstruction> & codeList);
    void PopStashed(std::vector<CVirtualMachineInstruction> & codeList);

    // add a new level of the program structure
    void push(size_t table_base, size_t code_base);
    void pop();
protected:
    std::vector<StructData> m_structData;
};

class CSWLoopStructure
{
private:
    struct LoopStructData
    {
        std::vector<size_t>breakExitList;
        std::vector<size_t>continueList;
    };
public:
    void push();
    bool addContinue(size_t line);
    bool addBreak(size_t line);
    void pop(size_t selfopLine, size_t exitLine, std::vector<CVirtualMachineInstruction> & codeList);
protected:
    std::vector<LoopStructData> m_structData;
};