#include "stdafx.h"
#include "stack"
#include "DataType.h"
#include "VM_instructions.h"
#include "VirtualMachine.h"
#include "EncodeConvHelper.h"
#include "error_messages.h"

#define STACK_INITSIZE 100

// the stack
// because all the data in 

std::vector<CVirtualMachineInstruction> vec_inst;

//////////////////////////////////////////////
//////////////////////////////////////////////

// wmain parameters
// 

CFileEncodeIO fil_coder;

int wmain(int argc, wchar_t** argv)
{
    bool isText = false;
    wchar_t *source_file = NULL;
    for (int i = 1; i < argc; ++i)
    {
        if (wcscmp(argv[i], L"-t") == 0)
            isText = true;
        else
            source_file = argv[i];
    }

    if (!source_file)
    {
        printf("Error: no assembly file entered.\n");
        exit(IDS_ERROR_VM_NOFILE);
    }

    // text format

    bool bRet = fil_coder.LoadFile(source_file, TRUE);
    if (!bRet)
    {
        printf("Open file \'%ws\' failed.", source_file);
        exit(IDS_ERROR_VM_FILEERROR);
    }
    CInstructionTranslator translator;

    const wchar_t *file_data = fil_coder.GetValue();
    std::wstringstream din(file_data);
    size_t iLine = 0;
    while (!din.eof())
    {
        iLine++;
        std::wstring line;
        std::getline(din, line);

        if (line.size() == 0)
            continue;

        std::wstringstream linein(line);

        std::wstring code_str;
        __int64 p_a = 0, p_b = 0;
        linein >> code_str >> p_a >> p_b;

        CVirtualMachineInstruction inst(VI_NOP, p_a, p_b);
        bRet = translator.GetInstruction(code_str, inst.inst);
        if (!bRet)
        {
            printf("On Line %zd,\n", iLine);
            wprintf(L"%ws\n\n", line.c_str());
            wprintf(L"invalid code \'%ws\'.\n", code_str.c_str());
            exit(IDS_ERROR_VM_INSTRUCTION);
        }
        vec_inst.push_back(inst);
    }

    CVirutalMachineInterpreter prog;
    if (vec_inst.size() > 0)
        prog.interpret(&vec_inst[0]);
    return 0;
}
