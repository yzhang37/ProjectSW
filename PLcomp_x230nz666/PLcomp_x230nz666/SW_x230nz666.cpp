// SW_x230nz666.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CLexicalParser.h"


int main(int argc, wchar_t *argv[])
{
	CLexicalParser lParser(std::_Ptr_wcin);
	while (lParser.Next())
	{
		switch (lParser.GetSymbolType())
		{
		case CLexicalParser::int_val:
			wprintf(L"%s -> %I64d\n", lParser.GetSymbol(), lParser.GetInteger());
			
			break;
		case CLexicalParser::dec_val:
			wprintf(L"%s -> %f\n", lParser.GetSymbol(), lParser.GetDecimal());
			break;
		default:
			if (lParser.GetSymbolType() == CLexicalParser::ident_type)
				wprintf(L"%s -> TYPE: ident, NAME:%s\n", lParser.GetSymbol(), lParser.GetSymbol());
			else
				wprintf(L"%s -> TYPE: %04x\n", lParser.GetSymbol(), lParser.GetSymbolType());
			break;
		}
	}
	
    return 0;
}

