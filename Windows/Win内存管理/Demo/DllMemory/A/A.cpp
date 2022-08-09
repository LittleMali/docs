// A.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ClassA.h"

#ifdef __cplusplus
extern "C" {
#endif

    __declspec(dllexport) void __cdecl GetInterface(void** ppInterface) {
        static ClassA staticObj;
        if (ppInterface) {
            *ppInterface = (void *)&staticObj;
        }
    }

#ifdef __cplusplus
}
#endif