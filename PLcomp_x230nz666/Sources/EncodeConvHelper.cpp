#include "EncodeConvHelper.h"
#include <fstream>

LPWSTR CFileEncodeIO::ANSIToUnicode(LPCSTR lpcstrSource)
{
    int textlen = MultiByteToWideChar(CP_ACP, 0, lpcstrSource, -1, NULL, 0);
    LPWSTR result = (LPWSTR)malloc((textlen + 1)*sizeof(WCHAR));
    ZeroMemory(result, (textlen + 1)*sizeof(WCHAR));
    //Call system function to convert
    MultiByteToWideChar(CP_ACP, 0, lpcstrSource, -1, (LPWSTR)result, textlen);
    return result;
}

LPSTR CFileEncodeIO::UnicodeToANSI(LPCWSTR lpcstrSource)
{
    // wide char to multi char
    int textlen = WideCharToMultiByte(CP_ACP, 0, lpcstrSource, -1, NULL, 0, NULL, NULL);
    LPSTR result = (LPSTR)malloc((textlen + 1)*sizeof(CHAR));
    ZeroMemory(result, sizeof(CHAR) * (textlen + 1));
    WideCharToMultiByte(CP_ACP, 0, lpcstrSource, -1, result, textlen, NULL, NULL);
    return result;
}

LPWSTR CFileEncodeIO::UTF8ToUnicode(LPCSTR lpcstrSource)
{
    int textlen = MultiByteToWideChar(CP_UTF8, 0, lpcstrSource, -1, NULL, 0);
    LPWSTR result = (LPWSTR)malloc((textlen + 1)*sizeof(WCHAR));
    ZeroMemory(result, (textlen + 1)*sizeof(WCHAR));
    MultiByteToWideChar(CP_UTF8, 0, lpcstrSource, -1, (LPWSTR)result, textlen);
    return result;
}

LPSTR CFileEncodeIO::UnicodeToUTF8(LPCWSTR lpcstrSource)
{
    // wide char to multi char
    int textlen = WideCharToMultiByte(CP_UTF8, 0, lpcstrSource, -1, NULL, 0, NULL, NULL);
    LPSTR result = (LPSTR)malloc((textlen + 1)*sizeof(CHAR));
    ZeroMemory(result, sizeof(CHAR) * (textlen + 1));
    WideCharToMultiByte(CP_UTF8, 0, lpcstrSource, -1, result, textlen, NULL, NULL);
    return result;
}

void *CFileEncodeIO::SwitchUCSEncode(void *lpSource, DWORD dwLength)
{
    WORD *p = (WORD*)lpSource;
    DWORD i;
    for (i = 0; i < dwLength; i++)
        *(p + i) = ((*(p + i) >> 8) & 0xff) + ((*(p + i) << 8) & 0xff00);
    return lpSource;
}

CFileEncodeIO::~CFileEncodeIO()
{
}

CFileEncodeIO::CFileEncodeIO()
{
    m_eePrfEncode = ENUMFILEENCODE::EE_UTF8_BOM;
}

CFileEncodeIO::CFileEncodeIO(LPCWSTR lpszwSource)
{
    CFileEncodeIO();
    SetValue(lpszwSource);
}

CFileEncodeIO::ENUMFILEENCODE CFileEncodeIO::GetEncode()
{
    return m_eePrfEncode;
}

void CFileEncodeIO::SetEncode(CFileEncodeIO::ENUMFILEENCODE eeNewEncode)
{
    m_eePrfEncode = eeNewEncode;
}

void CFileEncodeIO::SetValue(TString value)
{
    m_strFileBuff = value;
}

LPCWSTR CFileEncodeIO::GetValue()
{
    return m_strFileBuff.c_str();
}

BOOL CFileEncodeIO::SaveFile(LPCWSTR lpszFileName)
{
    std::ofstream fout(lpszFileName, std::ios::binary);
    if (m_eePrfEncode == EE_UTF8 || m_eePrfEncode == EE_UTF8_BOM)
    {
        //
        // if UTF-8, add 0xEF, 0xBB, 0xBF header
        //
        if (m_eePrfEncode == EE_UTF8_BOM)
            fout.write("\xef\xbb\xbf", 3);
        LPCSTR lpcBuffer = UnicodeToUTF8(m_strFileBuff.c_str());
        fout.write(lpcBuffer, strlen(lpcBuffer));
        delete[] lpcBuffer;
    }
    else if (m_eePrfEncode == EE_UCS2LE || m_eePrfEncode == EE_UCS2BE)
    {
        ENUMFILEENCODE m_EELocalMachine;

        //
        //detect the native UCS-2LE encoding format
        //if different, switch the encode.
        //

        WORD value = 0xFEFF;
        if (*((char *)&value) == '\xff')
            m_EELocalMachine = EE_UCS2LE;
        else
            m_EELocalMachine = EE_UCS2BE;

        DWORD dwBufferSize = lstrlenW(m_strFileBuff.c_str());
        try
        {
            LPWSTR lpcwBuffer = new WCHAR[dwBufferSize];
            memcpy(lpcwBuffer, m_strFileBuff.c_str(), sizeof(WCHAR) * dwBufferSize);

            //
            // big endian: FEFF
            // small endian: FFFE
            //

            if (m_eePrfEncode == EE_UCS2LE)
                fout.write("\xff\xfe", 2);
            else
                fout.write("\xfe\xff", 2);

            if (m_EELocalMachine != m_eePrfEncode)
                SwitchUCSEncode((void *)lpcwBuffer, dwBufferSize);

            fout.write((const char *)lpcwBuffer, sizeof(WCHAR) * dwBufferSize);

            delete[] lpcwBuffer;
        }
#pragma warning(disable:4101)
        catch (std::bad_alloc &e)
#pragma warning(default:4101)
        {
            return FALSE;
        }
    }
    else if (m_eePrfEncode == EE_ANSI)
    {
        LPCSTR lpBuffer = UnicodeToANSI(m_strFileBuff.c_str());
        fout.write(lpBuffer, strlen(lpBuffer));
        delete[]lpBuffer;
    }
    fout.close();
    return TRUE;
}

BOOL CFileEncodeIO::LoadFile(LPCWSTR lpszFileName, BOOL bAutoDetect)
{
    std::ifstream fin(lpszFileName, std::ios::binary);
    if (!fin)
        return FALSE;

    auto pbuf = fin.rdbuf();
    size_t szBufSize = (size_t)pbuf->pubseekoff(0, std::ios::end, std::ios::in);
    pbuf->pubseekpos(0, std::ios::in);
    BYTE *lpBuffer = new BYTE[szBufSize + 2];
    fin.read((char *)lpBuffer, szBufSize);
    lpBuffer[szBufSize] = '\0';
    lpBuffer[szBufSize + 1] = '\0';
    fin.close();

    if (bAutoDetect)
    {
        // UCS-2LE, UCS-2BE
        if (lpBuffer[0] == 0xff && lpBuffer[1] == 0xfe ||
            lpBuffer[0] == 0xfe && lpBuffer[1] == 0xff) //UTF-16
        {
            if (lpBuffer[0] == 0xff && lpBuffer[1] == 0xfe)
                m_eePrfEncode = ENUMFILEENCODE::EE_UCS2LE;
            else
                m_eePrfEncode = ENUMFILEENCODE::EE_UCS2BE;
        }
        // UTF-8 BOM, for Windows
        else if (lpBuffer[0] == 0xef && lpBuffer[1] == 0xbb && lpBuffer[2] == 0xbf)
            m_eePrfEncode = ENUMFILEENCODE::EE_UTF8_BOM;
        // ANSI or UTF-8
        else
        {
            m_eePrfEncode = ENUMFILEENCODE::EE_UTF8;
            size_t i = 0;
            while (i < szBufSize)
            {
                if (lpBuffer[i] < 0x80) i++; // (10000000): 值小于0x80的为ASCII字符    
                else if (lpBuffer[i] < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符    
                {
                    m_eePrfEncode = ENUMFILEENCODE::EE_ANSI;
                    break;
                }
                else if (lpBuffer[i] < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符    
                {
                    if (i >= szBufSize - 1) break;

                    if ((lpBuffer[i + 1] & (0xC0)) != 0x80)
                    {
                        m_eePrfEncode = ENUMFILEENCODE::EE_ANSI;
                        break;
                    }
                    i += 2;
                }
                else if (lpBuffer[i] < (0xF0)) // (11110000): 此范围内为3字节UTF-8字符    
                {
                    if (i >= szBufSize - 2) break;

                    if ((lpBuffer[i + 1] & (0xC0)) != 0x80 || (lpBuffer[i + 2] & (0xC0)) != 0x80)
                    {
                        m_eePrfEncode = ENUMFILEENCODE::EE_ANSI;
                        break;
                    }
                    i += 3;
                }
                else
                {
                    m_eePrfEncode = ENUMFILEENCODE::EE_ANSI;
                    break;
                }
            }
            //
        }
        //
    }

    if (m_eePrfEncode == ENUMFILEENCODE::EE_ANSI)
    {
        LPWSTR lpwBuffer = ANSIToUnicode((char *)lpBuffer);
        m_strFileBuff = lpwBuffer;
        delete[]lpwBuffer;
    }
    else if (m_eePrfEncode == ENUMFILEENCODE::EE_UTF8 ||
        m_eePrfEncode == ENUMFILEENCODE::EE_UTF8_BOM)
    {
        LPWSTR lpwBuffer = UTF8ToUnicode((m_eePrfEncode == ENUMFILEENCODE::EE_UTF8) ? (char *)lpBuffer : (char *)lpBuffer + 3);
        m_strFileBuff = lpwBuffer;
        delete[]lpwBuffer;
    }
    else
    {
        ENUMFILEENCODE m_EELocalMachine;
        //
        //detect the native UCS-2LE encoding format
        //if different, switch the encode.
        //

        WORD value = 0xFEFF;
        if (*((char *)&value) == '\xff')
            m_EELocalMachine = EE_UCS2LE;
        else
            m_EELocalMachine = EE_UCS2BE;

        if (m_EELocalMachine != m_eePrfEncode)
            SwitchUCSEncode((void *)lpBuffer, (DWORD)szBufSize >> 1);
        m_strFileBuff = ((LPCWSTR)lpBuffer) + 1;
    }

    delete[] lpBuffer;
    return TRUE;
}
