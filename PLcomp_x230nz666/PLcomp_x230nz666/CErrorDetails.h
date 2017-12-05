#pragma once
#include <map>

class CErrorDetails
{
public:
	CErrorDetails();
	~CErrorDetails();
// public member functions
public:
	virtual void GetErrorMessage() = 0;
protected:
	std::map<int, std::wstring> m_mapDetails;
};

