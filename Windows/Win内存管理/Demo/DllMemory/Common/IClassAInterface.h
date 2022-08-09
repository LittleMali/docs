#ifndef _I_CLASS_A_INTERFACE_H_
#define _I_CLASS_A_INTERFACE_H_

class IClassAInterface {
public:
    virtual ~IClassAInterface() {}

    virtual void UpdateData(int nData) = 0;
    virtual void PrintData() = 0;
};


#endif
