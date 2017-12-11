#pragma once
#include "stdafx.h"
#include "error_messages.h"

class CErrorDetails
{
public:
    CErrorDetails();
	std::wstring GetErrorMessage(size_t err_code);
protected:
	std::map<int, std::wstring> m_mapDetails;
};

