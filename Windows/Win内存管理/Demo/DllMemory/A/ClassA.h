#ifndef _CLASS_A_H_
#define _CLASS_A_H_

#include "../Common/IClassAInterface.h"
#include "../Common/IDataMgrInterface.h"

class ClassA : public IClassAInterface {
public:
    ClassA();
    virtual ~ClassA();

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
