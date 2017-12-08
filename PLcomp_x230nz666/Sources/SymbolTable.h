#pragma once
#include "stdafx.h"
#include "DataType.h"

struct a
{
	bool m_bIsConst;
	bool m_bIsRef;
	int m_nLevel;
	int m_nOffset;
};

class CSymbolTable
{
public:
	struct
	{
		
	};
	CSymbolTable();
	~CSymbolTable();
protected:
	std::map<std::wstring, int> m_tableData;
};
