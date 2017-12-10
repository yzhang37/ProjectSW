#pragma once
#include "stdafx.h"
#include "SymbolTable.h"

enum VirtualInstruction
{
    // nop 0, 0
    VI_NOP,

    // liti <constant>, 0
    VI_LIT_INTEGER,

    // lodi <level>, <offset>
    VI_LOAD_INTEGER,

    // lori <level = 1>, <offset>
    VI_LOAD_INTEGER_REF,

    // mkri
    VI_MAKE_REFERENCE,

    // stoi <level>, <offset>
    VI_STORE_INTEGER,

    // stri <level = 1>, <offset>
    VI_STORE_INTEGER_REF,

    // b
    VI_BRANCH,

    // bc
    VI_BRANCH_COND,

    // bl 0, addr;
    VI_BRANCH_LINK,

    // int 0, count
    VI_INITIALIZE_VAR,

    // opr 
    VI_OPERATION,

    // halt
    VI_HALT,
};

class CInstructionTranslator
{
public:
    CInstructionTranslator();
    bool GetInstruction(const std::wstring & str, VirtualInstruction &);
    bool GetString(VirtualInstruction, std::wstring & str);
    std::map<std::wstring, VirtualInstruction> str2ins;
    std::map<VirtualInstruction, std::wstring> ins2str;
};

struct CVirtualMachineInstruction
{
    CVirtualMachineInstruction() {}
    CVirtualMachineInstruction(VirtualInstruction vi, __int64 a, __int64 b);
    void set(VirtualInstruction vi, __int64 a, __int64 b);;
    VirtualInstruction inst;
    __int64 param_a;
    __int64 param_b;
};

struct CVMInst_Load : CVirtualMachineInstruction { CVMInst_Load(const CVariableSymbol & sym); };
struct CVMInst_Store : CVirtualMachineInstruction { CVMInst_Store(const CVariableSymbol & sym); };
struct CVMInst_LoadRef : CVirtualMachineInstruction { CVMInst_LoadRef(const CVariableSymbol & sym); };
struct CVMInst_StoreRef : CVirtualMachineInstruction { CVMInst_StoreRef(const CVariableSymbol & sym); };
struct CVMInst_Return : CVirtualMachineInstruction { CVMInst_Return(); };
struct CVMInst_MakeNegative : CVirtualMachineInstruction { CVMInst_MakeNegative(); };
struct CVMInst_Plus : CVirtualMachineInstruction { CVMInst_Plus(); };
struct CVMInst_Minus : CVirtualMachineInstruction { CVMInst_Minus(); };
struct CVMInst_Times : CVirtualMachineInstruction { CVMInst_Times(); };
struct CVMInst_Divide : CVirtualMachineInstruction { CVMInst_Divide(); };
struct CVMInst_Module : CVirtualMachineInstruction { CVMInst_Module(); };
struct CVMInst_Print : CVirtualMachineInstruction { CVMInst_Print(); };
struct CVMInst_Read : CVirtualMachineInstruction { CVMInst_Read(); };
struct CVMInst_Odd : CVirtualMachineInstruction { CVMInst_Odd(); };
struct CVMInst_Not : CVirtualMachineInstruction { CVMInst_Not(); };