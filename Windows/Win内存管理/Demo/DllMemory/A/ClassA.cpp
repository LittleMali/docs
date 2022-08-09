#include "stdafx.h"
#include "ClassA.h"
#include <shlwapi.h>
#include "../Common/Log.h"

#pragma comment(lib, "Shlwapi.lib")

typedef void (__cdecl *PFNGetInterface)(void** ppInterface);

ClassA::ClassA() : m_pDataMgr(NULL)
{
     Init();
}

ClassA::~ClassA()
{
    UnInit();
}

bool ClassA::Init()
{
    wchar_t szDll[MAX_PATH] = {0};
    GetModuleFileNameW(NULL, szDll, MAX_PATH);
    PathAppendW(szDll, L"..\\CC.dll");
    HMODULE hDll = LoadLibraryW(szDll);

    PFNGetInterface pfnGetInterface = (PFNGetInterface)GetProcAddress(hDll, "GetInterface");
    IDataMgrInterface* pInterface = NULL;
    pfnGetInterface((void**)&pInterface);

    m_pDataMgr = pInterface;

    return true;
}

void ClassA::UnInit()
{

}

void ClassA::UpdateData(int nData)
{
    if (m_pDataMgr) {
        DBGLOGW(L"[class B] update data: %d", nData);
        m_pDataMgr->SetNumber(nData);
    }
}

void ClassA::PrintData()
{
    if (m_pDataMgr) {
        int nNum = m_pDataMgr->GetNumber();
        DBGLOGW(L"[class A] print data: %d", nNum);
    }
}

