#include "stdafx.h"
#include "Exception.h"

const wchar_t * Exception::GetMessage() const
{
	return m_wstrMessage.c_str();
}

inline const wchar_t * Exception::GetSource() const
{
	return m_wstrSource.c_str();
}

const int Exception::GetHResult() const
{
	return m_iHresult;
}
