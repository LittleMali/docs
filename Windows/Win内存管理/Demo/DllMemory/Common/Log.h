#ifndef __LOG_G__
#define __LOG_G__
#pragma once

#define USELOG	1

#if USELOG
#define ENABLELOG	1
#pragma message("Enablelog is 1, all log in binary file")
#else
#define ENABLELOG	0
#pragma message("Enablelog is 0, all log don't in binary file")
#endif

void OutDbgLogW(LPCWSTR pszFile, LPCWSTR pszFunction, DWORD dwLine, LPCTSTR fmt, ...);
void OutDbgLogA(LPCSTR pszFile, LPCSTR pszFunction, DWORD dwLine, LPCSTR fmt, ...);

#if ENABLELOG
#define DBGLOGA(fmt, ...)		OutDbgLogA(__FILE__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define DBGLOGW(fmt, ...)		OutDbgLogW(__FILEW__, __FUNCTIONW__, __LINE__, fmt, __VA_ARGS__)
#else
#define DBGLOGA(fmt, ...)
#define DBGLOGW(fmt, ...)
#endif

#endif // #ifndef __LOG_G__
