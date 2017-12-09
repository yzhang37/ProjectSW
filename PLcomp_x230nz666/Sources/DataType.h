#pragma once

enum SymbolDataType
{
    SDT_NULL,
    SDT_INTEGER
};

enum SymbolAttribute
{
	SA_CONSTANT,
    SA_VARIABLE,
    SA_FUNCTION
};

struct SymbolStatus
{
    SymbolStatus():dateType(SDT_NULL) {}
    SymbolStatus(SymbolDataType type, bool ref = false) : dateType(type), isRef(ref) {}
    SymbolDataType dateType;
    bool isRef;
};