#pragma once
class Exception
{
public:
	inline const wchar_t *GetMessage() const;
	inline const wchar_t *GetSource() const;
	inline const int GetHResult() const;
protected:
	std::wstring m_wstrMessage;
	std::wstring m_wstrSource;
	int m_iHresult;
};

class EofException : Exception {};

class StreamFailedException : Exception
{

};