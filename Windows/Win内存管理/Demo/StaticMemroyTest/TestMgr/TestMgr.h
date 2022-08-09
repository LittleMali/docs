#ifndef _TEST_MGR_H_
#define _TEST_MGR_H_


#include <string>
#include "..\DataMgr\DataMgr.h"

class TestMgr {
public:
    bool LoadAndInitMemA();
    CDataMgr* OperDataA() { return m_pDtaMgrA; }

private:
    CDataMgr* m_pDtaMgrA;
};

#endif