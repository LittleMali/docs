#ifndef _DATA_MGR_H_
#define _DATA_MGR_H_

#include "../Common/IDataMgrInterface.h"

class CDataMgr : public IDataMgrInterface {
public:
    CDataMgr();
    virtual ~CDataMgr();

    // IDataMgrInterface
    virtual int GetNumber();
    virtual void SetNumber(int nNum);

private:
    int m_nNumber;
};

#endif
