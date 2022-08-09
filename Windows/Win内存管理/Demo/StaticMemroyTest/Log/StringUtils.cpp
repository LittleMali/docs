#include "stdafx.h"
#include "StringUtils.h"


bool StringUtils::MultiByteToUnicode(const std::string& strMultiByte, std::wstring& strUnicode, UINT uCodePage)
{
    int nUnicodeCount = MultiByteToWideChar(uCodePage, 0, strMultiByte.c_str(), -1, NULL, 0);
    int nWrittenChar = -1;

    if (nUnicodeCount > 0)
    {
        wchar_t* szTemp = new(std::nothrow) wchar_t[nUnicodeCount];
        if (szTemp == NULL)
        {
            return false;
        }
        nWrittenChar = MultiByteToWideChar(uCodePage, 0, strMultiByte.c_str(), -1, szTemp, nUnicodeCount);
        strUnicode = szTemp;
        delete[] szTemp;
        szTemp = NULL;
    }

    return (nUnicodeCount == nWrittenChar);
}

bool StringUtils::UnicodeToMultiByte(const std::wstring& strUnicode, std::string& strMultiByte, UINT uCodePage)
{
    int nUTF8Count = WideCharToMultiByte(uCodePage, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);
    int nWrittenChar = -1;

    if (nUTF8Count > 0)
    {
        char* szTemp = new(std::nothrow) char[nUTF8Count];
        if (szTemp == NULL)
        {
            return false;
        }
        nWrittenChar = WideCharToMultiByte(uCodePage, 0, strUnicode.c_str(), -1, szTemp, nUTF8Count, NULL, NULL);
        strMultiByte = szTemp;
        delete[] szTemp;
        szTemp = NULL;
    }

    return (nUTF8Count == nWrittenChar);
}

std::wstring StringUtils::MultiByteToUnicode(const std::string& strMultiByte, UINT uCodePage /*= CP_ACP*/)
{
    std::wstring Temp;

    MultiByteToUnicode(strMultiByte, Temp, uCodePage);
    return Temp;
}

std::string StringUtils::UnicodeToMultiByte(const std::wstring& strUnicode, UINT uCodePage /*= CP_ACP*/)
{
    std::string Temp;

    UnicodeToMultiByte(strUnicode, Temp, uCodePage);
    return Temp;
}

