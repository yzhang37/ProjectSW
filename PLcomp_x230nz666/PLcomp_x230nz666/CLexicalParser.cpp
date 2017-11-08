#include "stdafx.h"
#include "CLexicalParser.h"
#pragma warning(disable:4101)
#define isAlphabet(__wchar) (towlower(__wchar) >= L'a' && towlower(__wchar) <= L'z')
#define isNumeric(__wchar) ((__wchar) >= L'0' && (__wchar) <= L'9')
#define isWhiteless(__wchar) ((__wchar) == L' ' || (__wchar) == L'\n' || (__wchar) == L'\r')

CLexicalParser::CLexicalParser(std::wistream *wsIn)
{
	m_msSym[L'+'] = plus_op;
	m_msSym[L'-'] = minus_op;
	m_msSym[L'*'] = times_op;
	m_msSym[L'/'] = slash_op;
	m_msSym[L'%'] = mod_op;
	m_msSym[L'='] = eql_op;
	m_msSym[L'<'] = lss_op;
	m_msSym[L'>'] = gtr_op;
	m_msSym[L'('] = lparen_op;
	m_msSym[L')'] = rparen_op;
	m_msSym[L','] = comma_op;
	m_msSym[L';'] = semicolon_op;
	m_msSym[L':'] = colon_op;
	m_msSym[L'.'] = period_op;

	m_curCh = L' ';

	//add all the keywords
	insertKeyWords(L"and", and_op);
	insertKeyWords(L"as", as_sym);
	insertKeyWords(L"boolean", bool_datatype);
	insertKeyWords(L"break", break_sym);
	insertKeyWords(L"call", call_sym);
	insertKeyWords(L"case", case_sym);
	insertKeyWords(L"const", const_sym);
	insertKeyWords(L"continue", continue_sym);
	insertKeyWords(L"decimal", dec_datatype);
	insertKeyWords(L"else", else_sym);
	insertKeyWords(L"false", false_val);
	insertKeyWords(L"for", for_sym);
	insertKeyWords(L"func", func_sym);
	insertKeyWords(L"if", if_sym);
	insertKeyWords(L"in", in_sym);
	insertKeyWords(L"integer", int_datatype);
	insertKeyWords(L"not", not_sym);
	insertKeyWords(L"odd", odd_sym);
	insertKeyWords(L"or", or_op);
	insertKeyWords(L"print", print_sym);
	insertKeyWords(L"read", read_sym);
	insertKeyWords(L"ref", ref_sym);
	insertKeyWords(L"repeat", repeat_sym);
	insertKeyWords(L"return", return_sym);
	insertKeyWords(L"step", step_sym);
	insertKeyWords(L"switch", switch_sym);
	insertKeyWords(L"true", true_val);
	insertKeyWords(L"unless", unless_sym);
	insertKeyWords(L"until", until_sym);
	insertKeyWords(L"var", var_sym);
	insertKeyWords(L"while", while_sym);
	insertKeyWords(L"xor", xor_op);

	m_isInput = wsIn;
}

bool CLexicalParser::Next()
{
	//执行前先初始化一下当前的内容
	resetvalues();
	try
	{
		while (isWhiteless(m_curCh))
			getnextc();
	}
	catch (EofException &e)
	{
		//什么都没有，因为是空的
		return false;
	}

	//开头是字母的单词，就一定不是纯粹的数字了。
	if (isAlphabet(m_curCh))
	{
		try
		{
			do
			{
				m_wstrCurSymbol.push_back(m_curCh);
				getnextc();
			} while (isAlphabet(m_curCh) && m_isInput);
		}
		catch (EofException &e) {}
		//TODO: Other error?

		if (m_setReserved.count(m_wstrCurSymbol) > 0)
		{
			//m_isInput->putback(m_curCh);
			m_curSymbolType = m_mwSym[m_wstrCurSymbol];
			return true;
		}
		else
		{
			//因为标识符允许有下划线等，因此到下一个空格之前，
			//还需要继续进行连接。

			try
			{
				while (!isWhiteless(m_curCh))
				{
					if (isAlphabet(m_curCh) || m_curCh == L'_') {
						m_wstrCurSymbol.push_back(m_curCh);
						getnextc();
					}
					else break;
				}
			}
			catch (EofException &e) {}
			//TODO: Other error?

			//m_isInput->putback(m_curCh);
			m_curSymbolType = ident_type;
			return true;
		}
	}
	//处理数字的部分
	else if (isNumeric(m_curCh) || m_curCh == L'.')
	{
		bool isInteger = true,
			intOverflow = false;
		__int64 intvalue = 0, newintvalue = 0;
		double decvalue = 0.0;
		if (m_curCh != L'.')
		{
			if (m_curCh == L'0')
			{
				m_wstrCurSymbol.push_back(m_curCh);
				try
				{
					getnextc();
					if (isNumeric(m_curCh))
					{
						//TODO: 报错，不允许整数开头有额外的前导0.
						return false;
					}
					else if (m_curCh == L'.' || towlower(m_curCh) == L'e') //遇到小数点, 或是 E ，转移到浮点数	
					{
						// 遇到小数点，先看看有几个点。如果数量 >= 2，那么就不是本次要处理的任务。

						if (m_curCh == L'.')
							try
						{
							getnextc();
							wchar_t tempC = m_curCh;
							m_isInput->putback(tempC);
							m_curCh = L'.';
							if (tempC == L'.')
							{
								m_curSymbolType = int_val;
								m_curSymbolInt = intvalue;
								return true;
							}
						}
						catch (EofException &e) {}

						if (towlower(m_curCh) == L'e')
							m_isInput->putback(m_curCh);
						else
							m_wstrCurSymbol.push_back(m_curCh);
						isInteger = false;
					}
					else if (isAlphabet(m_curCh)) //直接连着字母，报错
					{
						//TODO: Error
						return false;
					}
					//m_isInput->putback(m_curCh);
				}
				catch (EofException &e) {}
				//TODO: Other error handler?
				if (isInteger)
				{
					m_curSymbolType = int_val;
					m_curSymbolInt = intvalue;
					return true;
				}
			}
			else
			{
				do //遇到数字符号则不断循环
				{
					m_wstrCurSymbol.push_back(m_curCh);
					intvalue = newintvalue;
					newintvalue *= 10;
					newintvalue += __int64(m_curCh - L'0');
					if (newintvalue < intvalue) //发现数字越界，那么就转移到浮点
					{
						decvalue = double(intvalue) * 10.0 + double(m_curCh - L'0');
						isInteger = false;
						intOverflow = true;
						// 如果 int64 存不下，那么多出来的位数也不用计算了
						break;
					}
					getnextc();
				} while (isNumeric(m_curCh));
				if (isInteger)
				{
					intvalue = newintvalue;
					if (m_curCh == L'.' || towlower(m_curCh) == L'e') //遇到小数点 -> 小数
					{
						// 遇到小数点，先看看有几个点。如果数量 >= 2，那么就不是本次要处理的任务。

						if (m_curCh == L'.')
							try
						{
							getnextc();
							wchar_t tempC = m_curCh;
							m_isInput->putback(tempC);
							m_curCh = L'.';
							if (tempC == L'.')
							{
								m_curSymbolType = int_val;
								m_curSymbolInt = intvalue;
								return true;
							}
						}
						catch (EofException &e) {}

						if (towlower(m_curCh) == L'e')
							m_isInput->putback(m_curCh);
						else
							m_wstrCurSymbol.push_back(m_curCh);
						decvalue = double(intvalue);
						isInteger = false;
					}
					else if (isAlphabet(m_curCh)) //直接连着字母，报错
					{
						//TODO: Error
						return false;
					}
					else //遇到别的符号
					{
						//m_isInput->putback(m_curCh);
						m_curSymbolType = int_val;
						m_curSymbolInt = intvalue;
						return true;
					}
				}
				else
				{
					//因为溢出，所以到下一个.之前，所有的数字都要处理

					try
					{
						getnextc();
						while (isNumeric(m_curCh))
						{
							m_wstrCurSymbol.push_back(m_curCh);
							decvalue *= 10;
							decvalue += double(m_curCh - L'0');
							getnextc();
						}
					}
					catch (EofException &e) {}
					//TODO: Other error handler?
				}
			}
		}
		else { //数字直接以 . 开头，意思就是 0.开头的小数
			//还有一种可能就是 ... 符号，这种情况要排除。

			isInteger = false;
			m_wstrCurSymbol.push_back(m_curCh);
			bool eof = false;
			try {
				getnextc();
			}
			catch (EofException &e) {
				eof = true;
			}
			if (m_curCh == L'.') //两个.了
			{
				m_wstrCurSymbol.push_back(m_curCh);
				eof = false;
				try { getnextc(); }
				catch (EofException &e) {
					eof = true;
				}
				if (eof || m_curCh != L'.') //不允许单独 .. 
				{
					// TODO: Error
					return false;
				}
				m_wstrCurSymbol.push_back(m_curCh);
				m_curSymbolType = to_op;
				try { getnextc(); }
				catch (EofException &e) {}
				return true;
			}
			if (eof || !isNumeric(m_curCh))
			{
				// 直接一个 . 是不允许的，报错
				// TODO: Error
				return false;
			}
			else
			{
				m_isInput->putback(m_curCh);
			}
		}

		if (!isInteger)
		{
			double digit = 0.1;
			try
			{
				getnextc();
				while (isNumeric(m_curCh))
				{
					m_wstrCurSymbol.push_back(m_curCh);
					if (!intOverflow)
					{
						decvalue += digit * double(m_curCh - L'0');
						digit /= 10;
					}
					getnextc();
				}
			}
			catch (EofException &e) {}

			if (towlower(m_curCh) == L'e')
			{
				int sign = 1;
				int exp_i = 0;
				bool eof = false;
				m_wstrCurSymbol.push_back(m_curCh);
				try {
					getnextc();
				}
				catch (EofException &e) { eof = true; }

				if (eof || (m_curCh != L'+' && m_curCh != L'-' &&
					!isNumeric(m_curCh)))
				{
					//TODO: Error
					return false;
				}
				else if (m_curCh == L'+')
				{
					m_wstrCurSymbol.push_back(m_curCh);
				}
				else if (m_curCh == L'-')
				{
					m_wstrCurSymbol.push_back(m_curCh);
					sign = -1;
				}
				else
					m_isInput->putback(m_curCh);

				getnextc();
				while (isNumeric(m_curCh))
				{
					m_wstrCurSymbol.push_back(m_curCh);
					exp_i *= 10;
					exp_i += int(m_curCh - L'0');
					if (exp_i > 1000) //位数太多，超过1000，直接报错
					{
						//TODO: Error
						return false;
					}
					try
					{
						getnextc();
					}
					catch (EofException &e) { break; }
				}
				decvalue *= pow(10.0, double(exp_i * sign));
				if (decvalue == HUGE_VAL || decvalue == NAN)
				{
					// 数字溢出了，无法操作
					return false;
				}

				//输入结束，数字后面不能直接跟着字母
				if (isAlphabet(m_curCh))
				{
					//TODO: Error
					return false;
				}

			}
			else if (isAlphabet(m_curCh))
			{
				//输入结束，数字后面不能直接跟着字母
				//TODO: Error
				return false;
			}

			//m_isInput->putback(m_curCh);
			m_curSymbolType = dec_val;
			m_curSymbolDec = decvalue;
			return true;
		}
		//TODO:
		//异常错误 
		return false;
	}
	// 都不是，那就是符号了

	m_wstrCurSymbol.push_back(m_curCh);
	switch (m_curCh)
	{
	case L';': //语句结束符号，后面可以直接跟任何字符
		m_curSymbolType = semicolon_op;
		break;
	case L'=': //赋值或者等于符号，后面可以直接跟任何字符
		m_curSymbolType = eql_op;
		break;
	case L',':
		m_curSymbolType = comma_op;
		break;
	case L'(':
		m_curSymbolType = lparen_op;
		break;
	case L')':
		m_curSymbolType = rparen_op;
		break;
	case L'{':
		m_curSymbolType = lbrace_op;
		break;
	case L'}':
		m_curSymbolType = rbrace_op;
		break;
	case L'*':
		m_curSymbolType = times_op;
		break;
	case L'/':
		m_curSymbolType = slash_op;
		break;
	case L'%':
		m_curSymbolType = mod_op;
		break;
	case L':':
		m_curSymbolType = colon_op;
		break;
	case L'+':
	case L'-':
		try {
			wchar_t wcBegin = m_curCh;
			int iCnt = 1;

			getnextc();
			while (m_curCh == wcBegin)
			{
				iCnt++;
				m_wstrCurSymbol.push_back(m_curCh);
				getnextc();
			}

			if (iCnt >= 3)
			{
				// 不允许 +++..., ---... 这样无限循环下去
				return false;
			}
			else if (iCnt == 2)
			{
				switch (wcBegin)
				{
				case L'+':
					m_curSymbolType = dblplus_op;
					break;
				case L'-':
					m_curSymbolType = dblminus_op;
					break;
				}
				return true;
			}
			else
				m_isInput->putback(m_curCh);
		}
		catch (EofException &e) {}
		m_curSymbolType = plus_op;
		break;
	case '!':
	case '<':
	case '>':
	{
		wchar_t wcBegin = m_curCh;
		try
		{
			getnextc();
			bool isNeq = ((wcBegin == L'!' && m_curCh == L'=') ||
				(wcBegin == L'<' && m_curCh == L'>')),
				isLeq = (wcBegin == L'<' && m_curCh == L'='),
				isGeq = (wcBegin == L'>' && m_curCh == L'=');

			if (isNeq || isLeq || isGeq)
			{
				if (isNeq)
					m_curSymbolType = neq_op;
				else if (isLeq)
					m_curSymbolType = leq_op;
				else if (isGeq)
					m_curSymbolType = geq_op;
				m_wstrCurSymbol.push_back(m_curCh);
				try { getnextc(); }
				catch (EofException &e) {}
				return true;
			}
		}
		catch (EofException &e) {}
		if (wcBegin == '!')
		{
			// TODO: Error;
			return false;
		}
		else if (wcBegin == L'<')
			m_curSymbolType = lss_op;
		else if (wcBegin == L'>')
			m_curSymbolType = gtr_op;
		return true;
	}
	default:
		return false;
	}
	try { getnextc(); }
	catch (EofException &e) {}
	return true;
}

const CLexicalParser::SymbolType CLexicalParser::GetSymbolType() const
{
	return m_curSymbolType;
}

const __int64 CLexicalParser::GetInteger() const
{
	return m_curSymbolInt;
}

const double CLexicalParser::GetDecimal() const
{
	return m_curSymbolDec;
}

const wchar_t * CLexicalParser::GetSymbol() const
{
	return m_wstrCurSymbol.c_str();
}

void CLexicalParser::insertKeyWords(const std::wstring &word, SymbolType symb)
{
	m_setReserved.insert(word);
	m_mwSym[word] = symb;
}

void CLexicalParser::error()
{
	m_curSymbolType = nul;
}

void CLexicalParser::resetvalues()
{
	m_curSymbolType = nul;
	m_curSymbolInt = 0;
	m_curSymbolDec = 0.0;
	m_wstrCurSymbol.clear();
}

void CLexicalParser::getnextc()
{
	m_isInput->get(m_curCh);
	if (m_isInput->eof())
		throw EofException();
	if (m_isInput->bad() || m_isInput->fail())
		throw StreamFailedException();
}

