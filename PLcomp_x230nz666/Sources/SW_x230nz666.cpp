// SW_x230nz666.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "error_messages.h"
#include "sw_symHeader.h"
#include "CSWStructure.h"
#include "EncodeConvHelper.h"

void CallStatSymParamProc(CLexicalParser & lParser, CSymbolTable & sTable,
    CFunctionSymbol *pFunction, size_t curParamId);

ProgramSymSyntax program;
DeclarationListSymSyntax declarationListSymSyntax;
VarDeclarationListSymSyntax varDeclarationListSymSyntax;
VarDeclarationSymSyntax varDeclarationSymSyntax;
VarIdentDeclarationSymSyntax varIdentDeclarationSymSyntax;
ConstDeclarataionListSymSyntax constDeclarataionListSymSyntax;
ConstDeclarationSymSyntax constDeclarationSymSyntax;
ConstIdentDeclarationSymSyntax constIdentDeclarationSymSyntax;
FunctionDeclarationListSymSyntax functionDeclarationListSymSyntax;
ParamIdentDeclarationSymSyntax paramIdentDeclarationSymSyntax;
FunctionBodySymSyntax functionBodySymSyntax;
StatementListSymSyntax statementListSymSyntax;
StatementSymSyntax statementSymSyntax;
IfStatSymSyntax ifStatSymSyntax;
WhileStatSymSyntax whileStatSymSyntax;
RepeatStatSymSyntax repeatStatSymSyntax;
ReadStatSymSyntax readStatSymSyntax;
PrintStatSymSyntax printStatSymSyntax;
AssignStatSymSyntax assignStatSymSyntax;
ForStatSymSyntax forStatSymSyntax;
CallStatSymSyntax callStatSymSyntax;
ConditionSymSyntax conditionSymSyntax;
ExpressionSymSyntax expressionSymSyntax;
TermSymSyntax termSymSyntax;
FactorSymSyntax factorSymSyntax;

std::vector<CVirtualMachineInstruction> outputCodes;
CSWStructure sw_Struct;
CFileEncodeIO fil_coder;

int error(int error_code)
{
    printf("%d\n", error_code);
    exit(EXIT_FAILURE);
    return error_code;
}

int wmain(int argc, wchar_t** argv)
{
    wchar_t *source_file = NULL;
    for (int i = 1; i < argc; ++i)
    {
        source_file = argv[i];
        break;
    }
    if (!source_file)
    {
        printf("Error: no source file entered.\n");
        exit(IDS_ERROR_VM_NOFILE);
    }
    bool bRet = fil_coder.LoadFile(source_file, TRUE);
    if (!bRet)
    {
        printf("Open file \'%ws\' failed.", source_file);
        exit(IDS_ERROR_VM_FILEERROR);
    }
    const wchar_t *file_data = fil_coder.GetValue();
    std::wistringstream sourcein(file_data);

    // the main lexical parser for the whole program	
    CLexicalParser lParser(&sourcein);

    // the definition of the Symbol Table
    CSymbolTable main_Table;

    if (lParser.Next())
    {
        ProgramSym(lParser, main_Table);

        CInstructionTranslator translator;

        std::wstringstream sout;
        std::wstring str_inst;
        for (auto asm_code : outputCodes)
        {
            translator.GetString(asm_code.inst, str_inst);
            sout << str_inst << L" " << asm_code.param_a << L" " << asm_code.param_b << std::endl;
        }
        std::wstring output_str = sout.str();
        fil_coder.SetValue(output_str);
        // make the output path
        std::wstring target_file = source_file;
        if (target_file.find_last_of(L".") != -1)
            target_file.resize(target_file.find_last_of(L"."));
        target_file.append(L".asw");
        fil_coder.SaveFile(target_file.c_str());
    }
    return 0;
}

void ProgramSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    // sw_Struct when created, will automatically push(0,0)
    // sw_Struct.push(0, 0);
    outputCodes.push_back(CVirtualMachineInstruction(VI_BRANCH, 0, 0));
    DeclarationListSym(lParser, sTable);
    outputCodes[sw_Struct.GetCurCodeBase()].param_b = outputCodes.size();
    outputCodes.push_back(CVirtualMachineInstruction(VI_INITIALIZE_VAR, 0, sw_Struct.GetCurVariableSpace()));
    sw_Struct.PopStashed(outputCodes);
    StatementListSym(lParser, sTable);
    outputCodes.push_back(CVMInst_Return());
}

void DeclarationListSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    ConstDeclarationListSym(lParser, sTable);
    VarDeclarationListSym(lParser, sTable);
    // until now, there shouldn't be any more variable intialize operations
    // exists here. However, indeed, they exist.

    // have to push all these code into sw_Struct, and after all those 
    // functions are finished, they're moved back.
    sw_Struct.PushStashCodes(outputCodes);

    FunctionDeclarationListSym(lParser, sTable);
}

void VarDeclarationListSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    while (varDeclarationListSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
    {
        VarDeclarationSym(lParser, sTable);

        if (lParser.GetSymbolType() == semicolon_op)
        {
            lParser.Next();
        }
        else
            error(IDS_ERROR_SYNTAX_DECLARATION_NOENDSEMICOLON);
    }
}

void VarDeclarationSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    if (lParser.GetSymbolType() == var_sym)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_UNEXPECTED);

    VarIdentDeclarationSym(lParser, sTable);

    while (lParser.GetSymbolType() == comma_op)
    {
        lParser.Next();
        VarIdentDeclarationSym(lParser, sTable);
    }
}

void VarIdentDeclarationSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    std::wstring strIdent;
    CVariableSymbol * newVariable = nullptr;
    if (lParser.GetSymbolType() == ident)
    {
        strIdent = lParser.GetSymbol();
        lParser.Next();

        newVariable = new CVariableSymbol();
        newVariable->SetName(strIdent);
        newVariable->SetStatus(SymbolStatus(SDT_INTEGER));
        newVariable->SetLevel(sw_Struct.GetCurLevel());
        newVariable->SetOffset(3 + (int)sw_Struct.GetCurVariableSpace());
        sw_Struct.AppendSpace(1); //add 1 space for use.

        if (!sTable.EnterSymbol(newVariable))
        {
            error(IDS_ERROR_SYNTAX_DECLARATION_NAMEREPEAT);
        }
    }
    else
        error(IDS_ERROR_SYNTAX_DECLARATION_MISSIDENT);

    if (lParser.GetSymbolType() == eql_op)
    {
        lParser.Next();

        size_t curLine = outputCodes.size();
        bool isConst = true;
        __int64 rValue;
        ExpressionSym(lParser, sTable, &isConst, &rValue);
        if (isConst)
        {
            outputCodes.resize(curLine);
            outputCodes.push_back(CVirtualMachineInstruction(VI_LIT_INTEGER, 0, rValue));
        }
        outputCodes.push_back(CVMInst_Store(*newVariable));
    }
}

void ConstDeclarationListSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    while (constDeclarationSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
    {
        ConstDeclarationSym(lParser, sTable);

        if (lParser.GetSymbolType() == semicolon_op)
        {
            lParser.Next();
        }
        else
            error(IDS_ERROR_SYNTAX_DECLARATION_NOENDSEMICOLON);
    }
}

void ConstDeclarationSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    if (lParser.GetSymbolType() == const_sym)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_UNEXPECTED);

    ConstIdentDeclarationSym(lParser, sTable);

    while (lParser.GetSymbolType() == comma_op)
    {
        lParser.Next();
        ConstIdentDeclarationSym(lParser, sTable);
    }
}

void ConstIdentDeclarationSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    std::wstring symbolName;
    if (lParser.GetSymbolType() == ident)
    {
        symbolName = lParser.GetSymbol();
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_DECLARATION_MISSIDENT);

    if (lParser.GetSymbolType() == eql_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_DECLARATION_CONST_NOEQL);

    size_t curLine = outputCodes.size();
    bool isConst = true;
    __int64 rValue;
    ExpressionSym(lParser, sTable, &isConst, &rValue);

    if (!isConst)
        error(IDS_ERROR_SYNTAX_DECLARATION_CONST_NONLITERAL);
    else
    {
        CConstantSymbol *pNewConst = new CConstantSymbol;
        pNewConst->SetName(symbolName);
        pNewConst->SetLevel(sw_Struct.GetCurLevel());
        pNewConst->SetValue(rValue);
        sTable.EnterSymbol(pNewConst);
        outputCodes.resize(curLine);
    }
}

void FunctionDeclarationListSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    while (lParser.GetSymbolType() == func_sym)
    {
        lParser.Next();

        std::wstring symbolName;
        if (lParser.GetSymbolType() == ident)
        {
            symbolName = lParser.GetSymbol();
            lParser.Next();
            CFunctionSymbol *newFunction = new CFunctionSymbol();
            newFunction->SetName(symbolName);
            newFunction->SetLevel(sw_Struct.GetCurLevel());

            // save the current code pos, as the function's address
            newFunction->SetOffset((int)outputCodes.size());

            if (!sTable.EnterSymbol(newFunction))
            {
                error(IDS_ERROR_SYNTAX_DECLARATION_NAMEREPEAT);
            }
        }
        else
            error(IDS_ERROR_SYNTAX_DECLARATION_MISSIDENT);

        // push, before params are generated
        sw_Struct.push(sTable.GetTableSize(), outputCodes.size());

        if (lParser.GetSymbolType() == lparen_op)
        {
            lParser.Next();
        }
        else
            error(IDS_ERROR_SYNTAX_DECLARATION_FUNC_NOLPAREN);

        if (paramIdentDeclarationSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
        {
            ParamIdentDeclarationSym(lParser, sTable);

            while (lParser.GetSymbolType() == comma_op)
            {
                lParser.Next();
                ParamIdentDeclarationSym(lParser, sTable);
            }
        }

        sTable.EncapLastFuncParams();

        if (lParser.GetSymbolType() == rparen_op)
        {
            lParser.Next();
        }
        else
            error(IDS_ERROR_SYNTAX_DECLARATION_FUNC_NORPAREN);

        if (lParser.GetSymbolType() == lbrace_op)
        {
            lParser.Next();
        }
        else
            error(IDS_ERROR_SYNTAX_DECLARATION_FUNC_NOLBRACE);

        FunctionBodySym(lParser, sTable);
        sw_Struct.pop();
        // prune the sTable: remove all the symbols higher than the current level.
        sTable.PruneTo(sw_Struct.GetCurLevel());

        if (lParser.GetSymbolType() == rbrace_op)
        {
            lParser.Next();
        }
        else
            error(IDS_ERROR_SYNTAX_DECLARATION_FUNC_NORBRACE);
    }
}

void ParamIdentDeclarationSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    // first we need to fetch,
    // how many params now already existed.
    CFunctionSymbol *lastFunc = sTable.GetLastFunction();
    if (!lastFunc)
        error(IDS_ERROR_SYNTAX_DECLARATION_REF_NOFUNCTION);
    size_t param_count = lastFunc->GetParams().size();

    CVariableSymbol *newSymbol = new CVariableSymbol();

    // only integer type, no others type
    newSymbol->_Status().dateType = SDT_INTEGER;
    newSymbol->SetLevel(sw_Struct.GetCurLevel());

    // notice: now the offset of these param are marked as "positive"
    // number. However indeed, it is processed as "negative" when running
    // inside the stack structure. When push these params before bl, 
    // the index - foundFunc->GetParams().size is the real positioin.
    newSymbol->SetOffset((int)param_count);

    if (lParser.GetSymbolType() == ref_sym)
    {
        newSymbol->_Status().isRef = true;
        lParser.Next();
    }

    // we need to "register" the current symbol status
    // into the last function.
    lastFunc->AppendParam(newSymbol->GetStatus());

    if (lParser.GetSymbolType() == ident)
    {
        newSymbol->SetName(lParser.GetSymbol());
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_DECLARATION_MISSIDENT);

    if (!sTable.EnterSymbol(newSymbol))
    {
        error(IDS_ERROR_SYNTAX_DECLARATION_NAMEREPEAT);
    }
}

void FunctionBodySym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    outputCodes.push_back(CVirtualMachineInstruction(VI_BRANCH, 0, 0));
    VarDeclarationListSym(lParser, sTable);
    sw_Struct.PushStashCodes(outputCodes);
    // where is the function?
    // haha, currently nested functions is not allowed.

    outputCodes[sw_Struct.GetCurCodeBase()].param_b = outputCodes.size();
    outputCodes.push_back(CVirtualMachineInstruction(VI_INITIALIZE_VAR, 0, sw_Struct.GetCurVariableSpace()));

    sw_Struct.PopStashed(outputCodes);

    StatementListSym(lParser, sTable);
    outputCodes.push_back(CVMInst_Return());
}

void StatementListSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    while (statementSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
    {
        StatementSym(lParser, sTable);
        if (lParser.GetSymbolType() == semicolon_op)
        {
            lParser.Next();
        }
        else
            error(IDS_ERROR_SYNTAX_STATEMENT_NOENDSEMICOLON);
    }
}

void StatementSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    if (lParser.GetSymbolType() == continue_sym)
    {
        // TODO: continue
        lParser.Next();
    }
    else if (lParser.GetSymbolType() == break_sym)
    {
        // TODO: break
        lParser.Next();
    }
    else if (ifStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
    {
        // TODO: if
        IfStatSym(lParser, sTable);
    }
    else if (whileStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
    {
        // TODO: while
        WhileStatSym(lParser, sTable);
    }
    else if (repeatStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
    {
        // TODO: repeat
        RepeatStatSym(lParser, sTable);
    }
    else if (readStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
    {
        ReadStatSym(lParser, sTable);
    }
    else if (printStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
    {
        PrintStatSym(lParser, sTable);
    }
    else if (assignStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
    {
        AssignStatSym(lParser, sTable);
    }
    else if (forStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
    {
        //TODO: for
        ForStatSym(lParser, sTable);
    }
    else if (callStatSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
    {
        CallStatSym(lParser, sTable);
    }
}

void IfStatSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    bool neg = false;
    if (lParser.GetSymbolType() == if_sym)
        lParser.Next();
    else if (lParser.GetSymbolType() == unless_sym)
    {
        neg = true;
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_UNEXPECTED);

    ConditionSym(lParser, sTable);
    if (neg)
        outputCodes.push_back(CVMInst_Not());

    size_t ifJumpBase1 = outputCodes.size();
    // bc 0 pos
    outputCodes.push_back(CVirtualMachineInstruction(VI_BRANCH_COND, 0, 0));

    if (lParser.GetSymbolType() == lbrace_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NOLBRACE);

    StatementListSym(lParser, sTable);

    if (lParser.GetSymbolType() == rbrace_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NORBRACE);

    // if there exists else
    if (lParser.GetSymbolType() == else_sym)
    {
        lParser.Next();
        outputCodes.push_back(CVirtualMachineInstruction(VI_BRANCH, 0, 0));
        outputCodes[ifJumpBase1].param_b = outputCodes.size();

        if (lParser.GetSymbolType() == lbrace_op)
        {
            lParser.Next();
        }
        else
            error(IDS_ERROR_SYNTAX_STATEMENT_NOLBRACE);

        StatementListSym(lParser, sTable);

        if (lParser.GetSymbolType() == rbrace_op)
        {
            lParser.Next();
        }
        else
            error(IDS_ERROR_SYNTAX_STATEMENT_NORBRACE);

        outputCodes[outputCodes[ifJumpBase1].param_b - 1].param_b = outputCodes.size();
    }
    else // do not exist else
    {
        outputCodes[ifJumpBase1].param_b = outputCodes.size();
    }
}

void WhileStatSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    if (lParser.GetSymbolType() == while_sym)
    {
        lParser.Next();
    }
    else if (lParser.GetSymbolType() == until_sym)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_UNEXPECTED);

    ConditionSym(lParser, sTable);

    if (lParser.GetSymbolType() == lbrace_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NOLBRACE);

    StatementListSym(lParser, sTable);

    if (lParser.GetSymbolType() == rbrace_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NORBRACE);
}

void RepeatStatSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    if (lParser.GetSymbolType() == repeat_sym)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_UNEXPECTED);

    if (lParser.GetSymbolType() == lbrace_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NOLBRACE);

    StatementListSym(lParser, sTable);

    if (lParser.GetSymbolType() == rbrace_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NORBRACE);
}

void ReadStatSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    if (lParser.GetSymbolType() == read_sym)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_UNEXPECTED);

    outputCodes.push_back(CVMInst_Read());

    if (lParser.GetSymbolType() == lparen_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NOLPAREN);

    std::wstring symbolName;
    if (lParser.GetSymbolType() == ident)
    {
        symbolName = lParser.GetSymbol();
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_MISSIDENT);

    auto pSymbol = sTable.FindLatest(symbolName);
    if (!pSymbol)
        error(IDS_ERROR_SYNTAX_STATEMENT_IDENTNOTEXIST);

    if (pSymbol->GetAttribute() != SA_VARIABLE)
        error(IDS_ERROR_SYNTAX_STATEMENT_NOTVARIABLE);

    CVariableSymbol *pVar = (CVariableSymbol *)pSymbol;
    if (pVar->GetStatus().isRef) // if it is a reference
        outputCodes.push_back(CVMInst_StoreRef(*pVar));
    else
        outputCodes.push_back(CVMInst_Store(*pVar));

    if (lParser.GetSymbolType() == rparen_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NORPAREN);
}

void PrintStatSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    if (lParser.GetSymbolType() == print_sym)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_UNEXPECTED);

    if (lParser.GetSymbolType() == lparen_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NOLPAREN);

    std::wstring symbolName;
    if (lParser.GetSymbolType() == ident)
    {
        symbolName = lParser.GetSymbol();
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_MISSIDENT);

    auto pSymbol = sTable.FindLatest(symbolName);
    if (!pSymbol)
        error(IDS_ERROR_SYNTAX_STATEMENT_IDENTNOTEXIST);

    if (pSymbol->GetAttribute() != SA_VARIABLE)
        error(IDS_ERROR_SYNTAX_STATEMENT_NOTVARIABLE);

    CVariableSymbol *pVar = (CVariableSymbol *)pSymbol;
    if (pVar->GetStatus().isRef) // if it is a reference
        outputCodes.push_back(CVMInst_LoadRef(*pVar));
    else
        outputCodes.push_back(CVMInst_Load(*pVar));

    outputCodes.push_back(CVMInst_Print());

    if (lParser.GetSymbolType() == rparen_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NORPAREN);
}

void AssignStatSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    std::wstring symbolName;
    if (lParser.GetSymbolType() == ident)
    {
        symbolName = lParser.GetSymbol();
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_UNEXPECTED);

    auto pFoundSymbol = sTable.FindLatest(symbolName);
    if (!pFoundSymbol)
        error(IDS_ERROR_SYNTAX_STATEMENT_IDENTNOTEXIST);
    if (pFoundSymbol->GetAttribute() != SA_VARIABLE)
        error(IDS_ERROR_SYNTAX_STATEMENT_ASSIGN_NONLVALUE);

    auto pFoundVariable = (CVariableSymbol *)pFoundSymbol;
    if (lParser.GetSymbolType() == eql_op)
    {
        lParser.Next();
        size_t curLine = outputCodes.size();
        bool isConst = true;
        __int64 rValue;
        ExpressionSym(lParser, sTable, &isConst, &rValue);
        if (isConst)
        {
            outputCodes.resize(curLine);
            outputCodes.push_back(CVirtualMachineInstruction(VI_LIT_INTEGER, 0, rValue));
        }

        if (pFoundVariable->GetStatus().isRef)
            outputCodes.push_back(CVMInst_StoreRef(*pFoundVariable));
        else
            outputCodes.push_back(CVMInst_Store(*pFoundVariable));
    }
    else if (lParser.GetSymbolType() == dblplus_op ||
        lParser.GetSymbolType() == dblminus_op)
    {
        auto symbOper = lParser.GetSymbolType();
        // load the original value
        if (pFoundVariable->GetStatus().isRef)
            outputCodes.push_back(CVMInst_LoadRef(*pFoundVariable));
        else
            outputCodes.push_back(CVMInst_Load(*pFoundVariable));
        // push 1
        outputCodes.push_back(CVirtualMachineInstruction(VI_LIT_INTEGER, 0, 1));
        if (symbOper == dblplus_op)
            outputCodes.push_back(CVMInst_Plus());
        else
            outputCodes.push_back(CVMInst_Minus());
        // store the value
        if (pFoundVariable->GetStatus().isRef)
            outputCodes.push_back(CVMInst_StoreRef(*pFoundVariable));
        else
            outputCodes.push_back(CVMInst_Store(*pFoundVariable));
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_ASSIGN_UNEXPECTED);
}

void ForStatSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    // for
    if (lParser.GetSymbolType() == for_sym)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_UNEXPECTED);

    // ident
    if (lParser.GetSymbolType() == ident)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_MISSIDENT);

    // in
    if (lParser.GetSymbolType() == in_sym)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_FOR_MISSIN);

    if (lParser.GetSymbolType() == number)
    {
        lParser.Next();
    }
    else if (lParser.GetSymbolType() == dec_val)
    {
        error(IDS_ERROR_LEXICAL_NODECIMAL);
    }
    else if (lParser.GetSymbolType() == ident)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_FOR_WRONGTYPE);

    if (lParser.GetSymbolType() == to_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_FOR_MISSTO);

    if (lParser.GetSymbolType() == number)
    {
        lParser.Next();
    }
    else if (lParser.GetSymbolType() == dec_val)
    {
        error(IDS_ERROR_LEXICAL_NODECIMAL);
    }
    else if (lParser.GetSymbolType() == ident)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_FOR_WRONGTYPE);

    if (lParser.GetSymbolType() == lbrace_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NOLBRACE);

    StatementListSym(lParser, sTable);

    if (lParser.GetSymbolType() == rbrace_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NORBRACE);
}

void CallStatSymParamProc(CLexicalParser & lParser, CSymbolTable & sTable,
    CFunctionSymbol *pFunction, size_t curParamId)
{
    if (pFunction->GetParams()[curParamId].isRef)
    {
        // if it is an ref, then the next symbol must be an ident, not an expression;
        if (lParser.GetSymbolType() != ident)
            error(IDS_ERROR_SYNTAX_STATEMENT_CALL_NOTREFERENCE);
        std::wstring symbolName = lParser.GetSymbol();
        auto pFoundSymbol = sTable.FindLatest(symbolName);
        if (!pFoundSymbol)
            error(IDS_ERROR_SYNTAX_STATEMENT_IDENTNOTEXIST);
        if (pFoundSymbol->GetAttribute() != SA_VARIABLE)
            error(IDS_ERROR_SYNTAX_STATEMENT_CALL_NOTREFERENCE);

        CVariableSymbol *var = (CVariableSymbol *)pFoundSymbol;
        if (var->GetStatus().isRef)
        {
            // notice: here is a tricky use.
            outputCodes.push_back(CVMInst_Load(*var));
        }
        else
        {
            outputCodes.push_back(CVirtualMachineInstruction(VI_MAKE_REFERENCE, var->GetLevel(),
                var->GetOffset()));
        }
        lParser.Next();
    }
    else
    {
        size_t curLine = outputCodes.size();
        bool isConst = true;
        __int64 rValue;
        ExpressionSym(lParser, sTable, &isConst, &rValue);
        if (isConst)
        {
            outputCodes.resize(curLine);
            outputCodes.push_back(CVirtualMachineInstruction(VI_LIT_INTEGER, 0, rValue));
        }
    }
}

void CallStatSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    if (lParser.GetSymbolType() == call_sym)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_UNEXPECTED);

    std::wstring symbolName;
    if (lParser.GetSymbolType() == ident)
    {
        symbolName = lParser.GetSymbol();
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_MISSIDENT);

    // look up for the symbol in the sTable
    auto pFoundSymbol = sTable.FindLatest(symbolName);
    if (!pFoundSymbol)
    {
        error(IDS_ERROR_SYNTAX_STATEMENT_IDENTNOTEXIST);
    }
    if (pFoundSymbol->GetAttribute() != SA_FUNCTION)
    {
        error(IDS_ERROR_SYNTAX_STATEMENT_CALL_NONFUNC);
    }
    CFunctionSymbol *pFoundFunction = (CFunctionSymbol *)pFoundSymbol;

    if (lParser.GetSymbolType() == lparen_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NOLPAREN);

    size_t usedParams = 0;
    if (expressionSymSyntax.GetFirst().count(lParser.GetSymbolType()) > 0)
    {
        if (usedParams < pFoundFunction->GetParams().size())
        {
            CallStatSymParamProc(lParser, sTable, pFoundFunction, usedParams);
            ++usedParams;
        }
        else
            error(IDS_ERROR_SYNTAX_STATEMENT_CALL_NUMDISMATCH);

        while (lParser.GetSymbolType() == comma_op)
        {
            lParser.Next();
            if (usedParams < pFoundFunction->GetParams().size())
            {
                CallStatSymParamProc(lParser, sTable, pFoundFunction, usedParams);
                ++usedParams;
            }
            else
                error(IDS_ERROR_SYNTAX_STATEMENT_CALL_NUMDISMATCH);
        }
    }
    if (usedParams < pFoundFunction->GetParams().size())
    {
        // too little params
        error(IDS_ERROR_SYNTAX_STATEMENT_CALL_NUMDISMATCH);
    }

    if (lParser.GetSymbolType() == rparen_op)
    {
        lParser.Next();
    }
    else
        error(IDS_ERROR_SYNTAX_STATEMENT_NORPAREN);

    outputCodes.push_back(CVirtualMachineInstruction(VI_LIT_INTEGER, 0, pFoundFunction->GetParams().size()));
    outputCodes.push_back(CVirtualMachineInstruction(VI_BRANCH_LINK, 0, pFoundFunction->GetOffset()));
}

void ConditionSym(CLexicalParser & lParser, CSymbolTable & sTable)
{
    if (lParser.GetSymbolType() == odd_sym)
    {
        lParser.Next();
        if (lParser.GetSymbolType() == lparen_op)
        {
            lParser.Next();
        }
        else
            error(IDS_ERROR_SYNTAX_STATEMENT_NOLPAREN);

        size_t curLine = outputCodes.size();
        bool isConst = true;
        __int64 rValue;
        ExpressionSym(lParser, sTable, &isConst, &rValue);
        if (isConst)
        {
            outputCodes.resize(curLine);
            outputCodes.push_back(CVirtualMachineInstruction(VI_LIT_INTEGER, 0, rValue));
        }
        outputCodes.push_back(CVMInst_Odd());

        if (lParser.GetSymbolType() == rparen_op)
        {
            lParser.Next();
        }
        else
            error(IDS_ERROR_SYNTAX_STATEMENT_NORPAREN);
    }
    else
    {
        size_t curLine = outputCodes.size();
        bool isConst = true;
        __int64 rValue;
        ExpressionSym(lParser, sTable, &isConst, &rValue);
        if (isConst)
        {
            outputCodes.resize(curLine);
            outputCodes.push_back(CVirtualMachineInstruction(VI_LIT_INTEGER, 0, rValue));
        }

        if (lParser.GetSymbolType() == eql_op ||
            lParser.GetSymbolType() == neq_op ||
            lParser.GetSymbolType() == lss_op ||
            lParser.GetSymbolType() == leq_op ||
            lParser.GetSymbolType() == gtr_op ||
            lParser.GetSymbolType() == geq_op)
        {
            auto symbol = lParser.GetSymbolType();
            lParser.Next();

            size_t curLine = outputCodes.size();
            bool isConst = true;
            __int64 rValue;
            ExpressionSym(lParser, sTable, &isConst, &rValue);
            if (isConst)
            {
                outputCodes.resize(curLine);
                outputCodes.push_back(CVirtualMachineInstruction(VI_LIT_INTEGER, 0, rValue));
            }

            switch (symbol)
            {
            case eql_op:
                outputCodes.push_back(CVirtualMachineInstruction(VI_OPERATION, 7, 0));
                break;
            case neq_op:
                outputCodes.push_back(CVirtualMachineInstruction(VI_OPERATION, 8, 0));
                break;
            case lss_op:
                outputCodes.push_back(CVirtualMachineInstruction(VI_OPERATION, 9, 0));
                break;
            case leq_op:
                outputCodes.push_back(CVirtualMachineInstruction(VI_OPERATION, 10, 0));
                break;
            case gtr_op:
                outputCodes.push_back(CVirtualMachineInstruction(VI_OPERATION, 11, 0));
                break;
            case geq_op:
                outputCodes.push_back(CVirtualMachineInstruction(VI_OPERATION, 12, 0));
                break;
            }
        }
        else
            error(IDS_ERROR_SYNTAX_CONDITION_MISSREL);
    }
}

void ExpressionSym(CLexicalParser & lParser, CSymbolTable & sTable, bool *pIsConst, __int64 *pRVal)
{
    bool neg = false;
    if (lParser.GetSymbolType() == plus_op ||
        lParser.GetSymbolType() == minus_op)
    {
        if (lParser.GetSymbolType() == minus_op)
            neg = true;
        lParser.Next();
    }

    if (pIsConst && pRVal && *pIsConst) TermSym(lParser, sTable, pIsConst, pRVal);
    else TermSym(lParser, sTable);

    // after handle with it, if neg, we make a neg operation on it.
    if (neg)
    {
        outputCodes.push_back(CVMInst_MakeNegative());
        if (pIsConst && pRVal && *pIsConst)
            *pRVal = -*pRVal;
    }

    while (lParser.GetSymbolType() == plus_op ||
        lParser.GetSymbolType() == minus_op)
    {
        neg = false;
        if (lParser.GetSymbolType() == minus_op)
            neg = true;
        lParser.Next();

        __int64 seRVal;
        if (pIsConst && pRVal && *pIsConst) TermSym(lParser, sTable, pIsConst, &seRVal);
        else TermSym(lParser, sTable);

        if (!neg)
        {
            outputCodes.push_back(CVMInst_Plus());
            if (pIsConst && pRVal && *pIsConst)
                *pRVal += seRVal;
        }
        else
        {
            outputCodes.push_back(CVMInst_Minus());
            if (pIsConst && pRVal && *pIsConst)
                *pRVal -= seRVal;
        }
    }
}

void TermSym(CLexicalParser & lParser, CSymbolTable & sTable, bool *pIsConst, __int64 *pRVal)
{
    if (pIsConst && pRVal && *pIsConst)
        FactorSym(lParser, sTable, pIsConst, pRVal);
    else
        FactorSym(lParser, sTable);

    while (lParser.GetSymbolType() == times_op ||
        lParser.GetSymbolType() == slash_op ||
        lParser.GetSymbolType() == mod_op)
    {
        auto thisSymb = lParser.GetSymbolType();
        lParser.Next();

        __int64 seRVal;
        if (pIsConst && pRVal && *pIsConst)
            FactorSym(lParser, sTable, pIsConst, &seRVal);
        else
            FactorSym(lParser, sTable);

        switch (thisSymb)
        {
        case times_op:
            outputCodes.push_back(CVMInst_Times());
            if (pIsConst && pRVal && *pIsConst)
                (*pRVal) *= seRVal;
            break;
        case slash_op:
            outputCodes.push_back(CVMInst_Divide());
            if (pIsConst && pRVal && *pIsConst)
                (*pRVal) /= seRVal;
            break;
        case mod_op:
            outputCodes.push_back(CVMInst_Module());
            if (pIsConst && pRVal && *pIsConst)
                (*pRVal) %= seRVal;
            break;
        }
    }
}

void FactorSym(CLexicalParser &lParser, CSymbolTable & sTable, bool *pIsConst, __int64 *pRVal)
{
    if (lParser.GetSymbolType() == ident)
    {
        // find the variable in the symbol table
        // if not constant, then make *pIsConst = false
        std::wstring symbolName = lParser.GetSymbol();

        auto pFoundSymbol = sTable.FindLatest(symbolName);

        if (!pFoundSymbol)
            error(IDS_ERROR_SYNTAX_STATEMENT_IDENTNOTEXIST);

        // currently, it's not allowed directly put the function
        // into the expression statement. Maybe in the future, 
        // it will be open.
        if (pFoundSymbol->GetAttribute() == SA_FUNCTION)
            error(IDS_ERROR_SYNTAX_STATEMENT_EXP_FUNCNOTALLOW);

        if (pFoundSymbol->GetAttribute() == SA_CONSTANT) // ok, is constant
        {
            CConstantSymbol *pFoundConst = (CConstantSymbol *)pFoundSymbol;
            outputCodes.push_back(CVirtualMachineInstruction(VI_LIT_INTEGER, 0, pFoundConst->GetValue()));

            if (pIsConst && pRVal && *pIsConst)
                *pRVal = pFoundConst->GetValue();
        }
        else if (pFoundSymbol->GetAttribute() == SA_VARIABLE)// is variable, :(
        {
            if (pIsConst && pRVal && *pIsConst)
                *pIsConst = false; // say goodbye to constant literal!
            CVariableSymbol *pFoundVariable = (CVariableSymbol *)pFoundSymbol;
            if (pFoundVariable->GetStatus().isRef) // is reference
            {
                // find the last function
                auto pFunc = sTable.GetLastFunction();
                if (!pFunc)
                    error(IDS_ERROR_SYNTAX_DECLARATION_REF_NOFUNCTION);
                outputCodes.push_back(CVMInst_LoadRef(*pFoundVariable));
            }
            else
                outputCodes.push_back(CVMInst_Load(*pFoundVariable));
        }
        else
            error(IDS_ERROR_SYNTAX_UNEXPECTED); // what? something unexpected happened.

        lParser.Next();
    }
    else if (lParser.GetSymbolType() == number)
    {
        outputCodes.push_back(CVirtualMachineInstruction(VI_LIT_INTEGER, 0, lParser.GetInteger()));
        if (pIsConst && pRVal && *pIsConst)
            *pRVal = lParser.GetInteger();
        lParser.Next();
    }
    else if (lParser.GetSymbolType() == dec_val)
    {
        error(IDS_ERROR_LEXICAL_NODECIMAL);
    }
    else if (lParser.GetSymbolType() == lparen_op)
    {
        lParser.Next();
        if (pIsConst && pRVal && *pIsConst)
            ExpressionSym(lParser, sTable, pIsConst, pRVal);
        else
            ExpressionSym(lParser, sTable);

        if (lParser.GetSymbolType() == rparen_op)
        {
            lParser.Next();
        }
        else
            error(IDS_ERROR_SYNTAX_STATEMENT_NORPAREN);
    }
    else
        error(IDS_ERROR_SYNTAX_EXPRESSION_UNEXPECTED);
}