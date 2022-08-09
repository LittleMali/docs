#ifndef _DATA_MGR_H_
#define _DATA_MGR_H_

#include <string>

//extern char g_pszDll[1000];

class CDataMgr{
public:
    CDataMgr();
    ~CDataMgr();

    void SetIntNum(int nNum) { m_nData = nNum; }
    int GetIntNum() { return m_nData; }

    void SetString(const char* pString) { if (pString) m_strData = pString; }
    const char* GetString() { return m_strData.c_str(); }

    int* GetIntArray() { return m_pIntArray; }
    int GetIntArrayElem(int nIndex) { return m_pIntArray[nIndex]; }
    void SetIntArrayElem(int nIndex, int nValue) { m_pIntArray[nIndex] = nValue; }

    char* GetStaticString() { return m_pStatic; }
    void SetStaticString(int nIndex, char ch) {m_pStatic[nIndex] = ch;}
private:
    int m_nData;
    std::string m_strData;
    int* m_pIntArray;
    char* m_pStatic;
};

#endif
