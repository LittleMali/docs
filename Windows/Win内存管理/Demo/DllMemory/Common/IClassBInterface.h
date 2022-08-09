#ifndef _I_CLASS_B_INTERFACE_H_
#define _I_CLASS_B_INTERFACE_H_

class IClassBInterface {
public:
    virtual ~IClassBInterface() {}

    virtual void UpdateData(int nData) = 0;
    virtual void PrintData() = 0;
};


#endif
