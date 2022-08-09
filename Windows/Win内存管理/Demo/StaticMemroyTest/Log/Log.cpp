#include "stdafx.h"
#include "Log.h"
#include "StringUtils.h"
#include <atlstr.h>

void OutDbgLogW(LPCWSTR pszFile, LPCWSTR pszFunction, DWORD dwLine, LPCTSTR fmt, ...)
{
	SYSTEMTIME timeNow;
	GetLocalTime(&timeNow);
	CString var_str;

	var_str.Format(L"[LITTLE][t:%d] Line %d %s ", GetCurrentThreadId(), dwLine, pszFunction);
	va_list	ap;
	va_start(ap, fmt);
	var_str.AppendFormatV(fmt, ap);
	va_end(ap);

	var_str.Append(_T("\r\n"));
	OutputDebugStringW(var_str.GetString());
}

void OutDbgLogA(LPCSTR pszFile, LPCSTR pszFunction, DWORD dwLine, LPCSTR fmt, ...)
{
	SYSTEMTIME timeNow;
	GetLocalTime(&timeNow);
	CStringA OutStr;
	CStringA var_str;

	var_str.Format("[LITTLE][t:%d] Line %d %s ", GetCurrentThreadId(), dwLine, pszFunction);

	va_list	ap;
	va_start(ap, fmt);
	var_str.AppendFormatV(fmt, ap);
	va_end(ap);

	var_str.Append("\r\n");
	OutputDebugStringA(var_str.GetString());
}