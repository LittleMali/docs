#ifndef _CLASS_B_H_
#define _CLASS_B_H_

#include "../Common/IClassBInterface.h"
#include "../Common/IDataMgrInterface.h"

class ClassB : public IClassBInterface {
public:
    ClassB();
    virtual ~ClassB();

public:
    bool Init();
    void UnInit();

    // IClassAInterface
    virtual void UpdateData(int nData);
    virtual void PrintData();

private:
    IDataMgrInterface* m_pDataMgr;
};

#endif
