#pragma once
#include "stdafx.h"
#include "VM_instructions.h"

class CRuntimeStack
{
public:
    CRuntimeStack();
    inline __int64 & operator[] (size_t);

    inline size_t get_top();
    inline void init_vars(size_t count);
    inline void push(__int64);
    inline void pop();
    inline void return_pop(size_t top_pos);
    inline void store(size_t pos);
    inline __int64 top();

protected:
    std::vector<__int64> m_stackData;
};

class CVirutalMachineInterpreter
{
public:
    CVirutalMachineInterpreter() {}
    void interpret(const CVirtualMachineInstruction *instruction);
protected:
    CRuntimeStack rs;
};

