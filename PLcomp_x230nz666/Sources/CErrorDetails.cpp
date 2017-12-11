#include "stdafx.h"
#include "CErrorDetails.h"

CErrorDetails::CErrorDetails()
{
    m_mapDetails[IDS_ERROR_VM_NOFILE] = L"";
    m_mapDetails[IDS_ERROR_VM_FILEERROR] = L"";
    m_mapDetails[IDS_ERROR_VM_INSTRUCTION] = L"";

    m_mapDetails[IDS_ERROR_LEXICAL_NODECIMAL] = L"decimal value isn't allowed in sw.";

    m_mapDetails[IDS_ERROR_SYNTAX_UNEXPECTED] = L"unexpected error occurred.";
    m_mapDetails[IDS_ERROR_SYNTAX_DECLARATION_MISSTYPE] = L"missing datatype in variable or constant declaration after 'as' keyword.";
    m_mapDetails[IDS_ERROR_SYNTAX_DECLARATION_NOENDSEMICOLON] = L"missing a ';'";
    m_mapDetails[IDS_ERROR_SYNTAX_DECLARATION_MISSIDENT] = L"missing an symbol name identifer.";
    m_mapDetails[IDS_ERROR_SYNTAX_DECLARATION_CONST_NOEQL] = L"The '=' symbol is required in a constant declaration.";
    m_mapDetails[IDS_ERROR_SYNTAX_DECLARATION_FUNC_NOLPAREN] = L"missing a '(' in function declaration.";
    m_mapDetails[IDS_ERROR_SYNTAX_DECLARATION_FUNC_NORPAREN] = L"missing a ')' in function declaration.";
    m_mapDetails[IDS_ERROR_SYNTAX_DECLARATION_FUNC_NOLBRACE] = L"missing a '{' before the start of a function body.";
    m_mapDetails[IDS_ERROR_SYNTAX_DECLARATION_FUNC_NORBRACE] = L"missing a '}' after the definition of a function body.";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_NOENDSEMICOLON] = L"missing a ';' after the statement.";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_NOLBRACE] = L"missing a '{' before a series of statements.";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_NORBRACE] = L"missing a '}' after a series of statements.";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_NOLPAREN] = L"excepted a '(' after the function caller.";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_NORPAREN] = L"excepted a ')' at the end of the function caller.";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_MISSIDENT] = L"excepted an symbol identifer.";
    m_mapDetails[IDS_ERROR_SYNTAX_ASSIGN_UNEXPECTED] = L"Invalid assignment statement.";
    m_mapDetails[IDS_ERROR_SYNTAX_FOR_MISSIN] = L"expect an 'in' keyword in for loop statement.";
    m_mapDetails[IDS_ERROR_SYNTAX_FOR_WRONGTYPE] = L"for loop expect a number, or a symbol identifer.";
    m_mapDetails[IDS_ERROR_SYNTAX_FOR_MISSTO] = L"expect a '...' keyword.";
    m_mapDetails[IDS_ERROR_SYNTAX_CONDITION_MISSREL] = L"expect a relation operation";
    m_mapDetails[IDS_ERROR_SYNTAX_EXPRESSION_UNEXPECTED] = L"invalid expression.";
    m_mapDetails[IDS_ERROR_SYNTAX_REPEAT_NOWHILEUNTIL] = L"expect a 'while' or 'until' keyword.";

    m_mapDetails[IDS_ERROR_SYNTAX_DECLARATION_NAMEREPEAT] = L"symbol redefinition.";
    m_mapDetails[IDS_ERROR_SYNTAX_DECLARATION_REF_NOFUNCTION] = L"";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_IDENTNOTEXIST] = L"symbol is undefined.";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_CALL_NONFUNC] = L"the symbol after 'call' keyword isn't a function.";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_CALL_NOTREFERENCE] = L"only variable symbol can be placed into a reference parameter.";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_CALL_NUMDISMATCH] = L"the number of parameters doesn't match the definition of the function.";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_NOTVARIABLE] = L"not a variable symbol.";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_EXP_FUNCNOTALLOW] = L"function shouldn't be placed into expressions.";
    m_mapDetails[IDS_ERROR_SYNTAX_DECLARATION_CONST_NONLITERAL] = L"expression must have a constant value.";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_ASSIGN_NONLVALUE] = L"";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_FOR_NONLVALUE] = L"for-loop needs a variable symbol for iteration.";
    m_mapDetails[IDS_ERROR_SYNTAX_FOR_NONLITERAL] = L"for-loop only accepts constant value expression.";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_CONTINUE_NOLOOP] = L"a continue statement may only be used within a loop";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_BREAK_NOLOOP] = L"a break statement may only be used within a loop";
    m_mapDetails[IDS_ERROR_SYNTAX_STATEMENT_CALL_NOCALL] = L"'call' keyword should comes before function name.";
}

std::wstring CErrorDetails::GetErrorMessage(size_t err_code)
{
    if (m_mapDetails.count(err_code) > 0)
        return m_mapDetails[err_code];
    else
    {
        wchar_t buff[16];
        swprintf_s(buff, L"error %zd", err_code);
        return buff;
    }
}
