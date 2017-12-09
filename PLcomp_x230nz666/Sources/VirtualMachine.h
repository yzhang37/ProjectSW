#pragma once
#include "stdafx.h"
#include "VM_instructions.h"

struct CVirtualMachineInstruction
{
    CVirtualMachineInstruction() {}
    CVirtualMachineInstruction(VirtualInstruction vi, __int64 a, __int64 b);
    void set(VirtualInstruction vi, __int64 a, __int64 b);;
    VirtualInstruction inst;
    __int64 param_a;
    __int64 param_b;
};

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

