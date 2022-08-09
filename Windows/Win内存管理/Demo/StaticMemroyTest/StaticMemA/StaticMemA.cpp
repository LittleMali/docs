// StaticMemA.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Windows.h>
#include "..\DataMgr\DataMgr.h"

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) CDataMgr* GetDataMgr()
{
    static CDataMgr g_sDataMgr;
    return &g_sDataMgr;
}

#ifdef __cplusplus
}
#endif