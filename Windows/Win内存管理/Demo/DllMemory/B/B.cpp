// B.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "ClassB.h"

#ifdef __cplusplus
extern "C" {
#endif

    __declspec(dllexport) void __cdecl GetInterface(void** ppInterface) {
        static ClassB staticObj;
        if (ppInterface) {
            *ppInterface = (void *)&staticObj;
        }
    }

#ifdef __cplusplus
}
#endif
