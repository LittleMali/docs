#include "stdafx.h"
#include "TestMgr.h"
#include "../Log/Log.h"
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

extern "C" {
typedef CDataMgr* (*PFN_GetDataMgr)();
};

bool TestMgr::LoadAndInitMemA()
{
    wchar_t szDll[MAX_PATH] = {0};
    ::GetModuleFileNameW(NULL, szDll, MAX_PATH);
    ::PathAppendW(szDll, L"..\\StaticMemA.dll");

    HMODULE hDll = ::LoadLibraryW(szDll);
    if (!hDll)
        return false;
    DBGLOGW(L"load dll, hDll=%p", hDll);

    PFN_GetDataMgr pfn = (PFN_GetDataMgr)::GetProcAddress(hDll, "GetDataMgr");
    if (!pfn)
        return false;
    DBGLOGW(L"get pfn, pfn=%p", pfn);

    CDataMgr* pDataMgr = pfn();
    if (!pDataMgr)
        return false;

    DBGLOGW(L"pDataMgr=%p", pDataMgr);

    m_pDtaMgrA = pDataMgr;

    return true;
}

