#pragma once
#include "stdafx.h"
#include "windows.h"

typedef std::wstring TString;

class CFileEncodeIO
{
public:
    enum ENUMFILEENCODE
    {
        EE_UTF8_BOM = 0,
        EE_UTF8 = 1,
        EE_UCS2LE = 2,
        EE_UCS2BE = 3,
        EE_ANSI = 4
    };
    ~CFileEncodeIO();
    CFileEncodeIO();
    CFileEncodeIO(LPCWSTR str);
    ENUMFILEENCODE GetEncode();
    void SetEncode(ENUMFILEENCODE);
    void SetValue(TString);
    LPCWSTR GetValue();
    BOOL SaveFile(LPCWSTR lpszFileName);
    // Load a text file from computer
    // if you specify bAutoDetect = FALSE, then
    // it won't automatically set the encode mode guessed by file content.
    BOOL LoadFile(LPCWSTR lpszFileName, BOOL bAutoDetect = TRUE);

    //used in LCPSTR constructor
    static LPWSTR ANSIToUnicode(LPCSTR /*lpcstrSource*/);
    static LPSTR UnicodeToANSI(LPCWSTR /*lpcstrSource*/);
    static PWSTR UTF8ToUnicode(LPCSTR /*lpcstrSource*/);
    static LPSTR UnicodeToUTF8(LPCWSTR /*lpcstrSource*/);
protected:
private:
    TString m_strFileBuff;
    ENUMFILEENCODE m_eePrfEncode;
    void *SwitchUCSEncode(void * /* lpSource */, DWORD /*dwLength*/);
};