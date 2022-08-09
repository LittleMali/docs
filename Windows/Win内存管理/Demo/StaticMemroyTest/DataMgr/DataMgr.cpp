#include "stdafx.h"
#include "DataMgr.h"

static char g_szString[1000] = "aaaaaaaaaaaaaaaaaaaaaaaaaa";

CDataMgr::CDataMgr()
{
    m_nData = 0;
    m_pIntArray = new int[10];
    for (int i = 0; i < 10; ++i) {
        m_pIntArray[i] = i;
    }

    m_pStatic = g_szString;
}

CDataMgr::~CDataMgr()
{
    if (m_pIntArray) {
        delete[] m_pIntArray;
        m_pIntArray = NULL;
    }

}

