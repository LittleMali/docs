#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <string>

namespace StringUtils
{
    bool MultiByteToUnicode(const std::string& strMultiByte, std::wstring& strUnicode, UINT uCodePage);
    bool UnicodeToMultiByte(const std::wstring& strUnicode, std::string& strMultiByte, UINT uCodePage);

    std::wstring MultiByteToUnicode(const std::string& strMultiByte, UINT uCodePage = CP_ACP);
    std::string UnicodeToMultiByte(const std::wstring& strUnicode, UINT uCodePage = CP_ACP);
}