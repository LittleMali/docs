#ifndef _I_DATA_MGR_INTERFACE_H_
#define _I_DATA_MGR_INTERFACE_H_

class IDataMgrInterface {
public:
    virtual ~IDataMgrInterface() {}

    virtual int GetNumber() = 0;
    virtual void SetNumber(int nNum) = 0;
};


#endif
