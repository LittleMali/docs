#include "stdafx.h"
#include "DataMgr.h"

CDataMgr::CDataMgr()
{
    m_nNumber = 0;
}

CDataMgr::~CDataMgr()
{
    m_nNumber = 0;
}

int CDataMgr::GetNumber()
{
    return m_nNumber;
}

void CDataMgr::SetNumber(int nNum)
{
    m_nNumber = nNum;
}

