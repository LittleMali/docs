#include "stdafx.h"
#include "ClassB.h"
#include <shlwapi.h>
#include "../Common/Log.h"

#pragma comment(lib, "Shlwapi.lib")

typedef void (__cdecl *PFNGetInterface)(void** ppInterface);

ClassB::ClassB() : m_pDataMgr(NULL)
{
    Init();
}

ClassB::~ClassB()
{
    UnInit();
}

bool ClassB::Init()
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

void ClassB::UnInit()
{

}

void ClassB::UpdateData(int nData)
{
    if (m_pDataMgr) {
        DBGLOGW(L"[class B] update data: %d", nData);
        m_pDataMgr->SetNumber(nData);
    }
}

void ClassB::PrintData()
{
    if (m_pDataMgr) {
        int nNum = m_pDataMgr->GetNumber();
        DBGLOGW(L"[class B] print data: %d", nNum);
    }
}

