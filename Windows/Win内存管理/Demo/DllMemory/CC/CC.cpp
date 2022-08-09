// CC.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "../Common/IDataMgrInterface.h"
#include "DataMgr.h"

#ifdef __cplusplus
extern "C" {
#endif

    __declspec(dllexport) void __cdecl GetInterface(void** ppInterface) {
        static CDataMgr staticObj;
        if (ppInterface) {
            *ppInterface = (void *)&staticObj;
        }
    }

#ifdef __cplusplus
}
#endif