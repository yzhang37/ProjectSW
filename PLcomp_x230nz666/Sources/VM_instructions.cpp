#include "VM_instructions.h"

CInstructionTranslator::CInstructionTranslator()
{
    str2ins[L"nop"] = VI_NOP;
    str2ins[L"liti"] = VI_LIT_INTEGER;
    str2ins[L"lodi"] = VI_LOAD_INTEGER;
    str2ins[L"lori"] = VI_LOAD_INTEGER_REF;
    str2ins[L"mkri"] = VI_MAKE_REFERENCE;
    str2ins[L"stoi"] = VI_STORE_INTEGER;
    str2ins[L"stri"] = VI_STORE_INTEGER_REF;
    str2ins[L"b"] = VI_BRANCH;
    str2ins[L"bc"] = VI_BRANCH_COND;
    str2ins[L"bl"] = VI_BRANCH_LINK;
    str2ins[L"int"] = VI_INITIALIZE_VAR;
    str2ins[L"opr"] = VI_OPERATION;
    str2ins[L"halt"] = VI_HALT;

    for (auto pitem = str2ins.begin(); pitem != str2ins.end(); ++pitem)
    {
        ins2str[pitem->second] = pitem->first;
    }
}

bool CInstructionTranslator::GetInstruction(const std::wstring & str, VirtualInstruction & vinst)
{
    if (str2ins.count(str) > 0)
    {
        vinst = str2ins[str];
        return true;
    }
    else
        return false;
}

bool CInstructionTranslator::GetString(VirtualInstruction vinst, std::wstring &str )
{
    if (ins2str.count(vinst) > 0)
    {
        str = ins2str[vinst];
        return true;
    }
    else
        return false;
}

CVirtualMachineInstruction::CVirtualMachineInstruction(VirtualInstruction vi, __int64 a, __int64 b)
    :inst(vi), param_a(a), param_b(b) {}

void CVirtualMachineInstruction::set(VirtualInstruction vi, __int64 a, __int64 b)
{
    inst = vi; param_a = a; param_b = b;
}

CVMInst_Return::CVMInst_Return()
{
    inst = VI_OPERATION;
    param_a = 0;
    param_b = 0;
}

CVMInst_Print::CVMInst_Print()
{
    inst = VI_OPERATION;
    param_a = 14;
    param_b = 0;
}

CVMInst_Read::CVMInst_Read()
{
    inst = VI_OPERATION;
    param_a = 15;
    param_b = 0;
}

CVMInst_MakeNegative::CVMInst_MakeNegative()
{
    inst = VI_OPERATION;
    param_a = 1;
    param_b = 0;
}

CVMInst_Plus::CVMInst_Plus()
{
    inst = VI_OPERATION;
    param_a = 2;
    param_b = 0;
}

CVMInst_Minus::CVMInst_Minus()
{
    inst = VI_OPERATION;
    param_a = 3;
    param_b = 0;
}

CVMInst_Times::CVMInst_Times()
{
    inst = VI_OPERATION;
    param_a = 4;
    param_b = 0;
}

CVMInst_Divide::CVMInst_Divide()
{
    inst = VI_OPERATION;
    param_a = 5;
    param_b = 0;
}

CVMInst_Module::CVMInst_Module()
{
    inst = VI_OPERATION;
    param_a = 6;
    param_b = 0;
}

CVMInst_Odd::CVMInst_Odd()
{
    inst = VI_OPERATION;
    param_a = 13;
    param_b = 0;
}

CVMInst_Not::CVMInst_Not()
{
    inst = VI_OPERATION;
    param_a = 16;
    param_b = 0;
}

CVMInst_Load::CVMInst_Load(const CVariableSymbol & sym)
{
    inst = VI_LOAD_INTEGER;
    param_a = sym.GetLevel();
    param_b = sym.GetOffset();
}

CVMInst_LoadRef::CVMInst_LoadRef(const CVariableSymbol & sym)
{
    inst = VI_LOAD_INTEGER_REF;
    param_a = sym.GetLevel();
    param_b = sym.GetOffset();
}

CVMInst_Store::CVMInst_Store(const CVariableSymbol & sym)
{
    inst = VI_STORE_INTEGER;
    param_a = sym.GetLevel();
    param_b = sym.GetOffset();
}

CVMInst_StoreRef::CVMInst_StoreRef(const CVariableSymbol & sym)
{
    inst = VI_STORE_INTEGER_REF;
    param_a = sym.GetLevel();
    param_b = sym.GetOffset();
}

