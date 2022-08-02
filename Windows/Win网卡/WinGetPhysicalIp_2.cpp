// GetLocalIp.cpp : Defines the entry point for the console application.
//

/*
通过 Characteristics 判断是否物理网卡。
*/

#include "stdafx.h"
#include <iostream>
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <shlwapi.h>
#include <vector>
#include <string>
#include <algorithm>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

enum NetAdapterType
{
    NCF_VIRTUAL = 0x01,
    NCF_SOFTWARE_ENUMERATED = 0x02,
    NCF_PHYSICAL = 0x04,
    NCF_HIDDEN = 0x08,
    NCF_NO_SERVICE = 0x10,
    NCF_NOT_USER_REMOVABLE = 0x20,
    NCF_MULTIPORT_INSTANCED_ADAPTER = 0x40,
    NCF_HAS_UI = 0x80,
    NCF_FILTER = 0x400,
};

std::vector<std::string> GetPhysicalAdapterName()
{
    std::vector<std::string> vecAdapter;

    const std::string strNetPath = "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}";
    LONG lRet = ERROR_SUCCESS;

    HKEY hNet = NULL;
    lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, strNetPath.c_str(), 0, KEY_READ, &hNet);
    if (lRet != ERROR_SUCCESS)
    {
        return vecAdapter;
    }

    DWORD dwIndex = 0;
    do
    {
        char szSubKeyName[MAX_PATH + 2] = {0};
        DWORD dwSubKeyNameSize = MAX_PATH;

        lRet = RegEnumKeyExA(hNet, dwIndex++, szSubKeyName, &dwSubKeyNameSize, NULL, NULL, NULL, NULL);
        if (lRet == ERROR_SUCCESS)
        {
            LONG lRetTmp = ERROR_SUCCESS;
            HKEY hSubKey = NULL;
            lRetTmp = RegOpenKeyExA(hNet, szSubKeyName, 0, KEY_READ, &hSubKey);
            // printf result
            if (ERROR_SUCCESS == lRetTmp)
            {
                DWORD dwRegType = REG_DWORD;
                DWORD dwCharacteristics = 0;
                DWORD dwDataLen = sizeof(dwCharacteristics);
                lRetTmp = RegQueryValueExA(hSubKey, "Characteristics", NULL, &dwRegType, (BYTE *)&dwCharacteristics, &dwDataLen);
                // printf result

                if (lRetTmp == ERROR_SUCCESS)
                {
                    BOOL bPhysical = FALSE;
                    if ((dwCharacteristics & NCF_VIRTUAL) == NCF_VIRTUAL)
                    {
                        // 虚拟网卡
                    }
                    else if ((dwCharacteristics & NCF_PHYSICAL) == NCF_PHYSICAL)
                    {
                        // 物理网卡
                        bPhysical = TRUE;
                    }

                    if (bPhysical)
                    {
                        dwRegType = REG_SZ;
                        char szNetCfgInstanceId[MAX_PATH + 2] = {0};
                        dwDataLen = MAX_PATH;
                        lRetTmp = RegQueryValueExA(hSubKey, "NetCfgInstanceId", NULL, &dwRegType, (BYTE *)szNetCfgInstanceId, &dwDataLen);
                        // printf result

                        if (lRetTmp == ERROR_SUCCESS)
                        {
                            vecAdapter.push_back(szNetCfgInstanceId);
                        }
                    }
                }

                RegCloseKey(hSubKey);
                hSubKey = NULL;
            }
        }
    } while (lRet != ERROR_NO_MORE_ITEMS || lRet == ERROR_SUCCESS);

    RegCloseKey(hNet);
    hNet = NULL;

    return vecAdapter;
}

std::vector<std::string> GetPhysicalIpList()
{
    std::vector<std::string> vecIpList;

    // 获取物理网卡名字
    std::vector<std::string> vecPhyAdapterName = GetPhysicalAdapterName();

    std::string strAddress;

    PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
    PIP_ADAPTER_INFO pTemp = pIpAdapterInfo;

    unsigned long stSize = sizeof(IP_ADAPTER_INFO);

    int nRet = GetAdaptersInfo(pIpAdapterInfo, &stSize);

    if (ERROR_BUFFER_OVERFLOW == nRet)
    {
        delete pIpAdapterInfo;
        pTemp = NULL;

        pIpAdapterInfo = (PIP_ADAPTER_INFO) new BYTE[stSize];
        nRet = GetAdaptersInfo(pIpAdapterInfo, &stSize);
        pTemp = pIpAdapterInfo;
    }

    // 遍历网卡信息
    if (ERROR_SUCCESS == nRet)
    {
        while (pIpAdapterInfo)
        {
            strAddress = pIpAdapterInfo->IpAddressList.IpAddress.String;
            if (strAddress.compare("0.0.0.0") != 0)
            {
                // 通过网卡设备名，来提取物理网卡对应的ip
                std::vector<std::string>::iterator iter = find(vecPhyAdapterName.begin(), vecPhyAdapterName.end(), std::string(pIpAdapterInfo->AdapterName));
                if (iter != vecPhyAdapterName.end())
                {
                    vecIpList.push_back(strAddress);
                }
            }
            else
            {
                // 网卡未连接时，获取到的ip是0.0.0.0
            }

            pIpAdapterInfo = pIpAdapterInfo->Next;
        }
    }

    if (pTemp)
    {
        delete pTemp;
        pTemp = NULL;
    }

    return vecIpList;
}

int _tmain(int argc, _TCHAR *argv[])
{
    std::vector<std::string> vec = GetPhysicalIpList();
    return 0;
}
