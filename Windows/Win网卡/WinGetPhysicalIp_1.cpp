// GetLocalIp.cpp : Defines the entry point for the console application.
//

/*
通过 MediaSubType 和 PnpInstanceID名字 判断是否物理网卡。
不建议在虚拟机下使用。
*/

#include <iostream>
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <set>
#include <shlwapi.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

enum ADAPTER_TYPE
{
    AT_UNKNOWN = 0,
    AT_CABLE = 1,
    AT_WIRENESS = 2,
    AT_VIRTUAL = 3,
};

ADAPTER_TYPE GetAdapterType(const std::string &strAdapterName)
{
    ADAPTER_TYPE enType = AT_UNKNOWN;

    if (strAdapterName.empty())
    {
        return enType;
    }

    LONG lRet = ERROR_SUCCESS;

    HKEY hNetworkKey = NULL;
    const std::string strNetworkRegPath = "System\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}";
    lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, strNetworkRegPath.c_str(), 0, KEY_READ, &hNetworkKey);
    if (ERROR_SUCCESS != lRet)
    {
        return enType;
    }

    char szAdapterSubPath[MAX_PATH + 2] = {0};
    _snprintf_s(szAdapterSubPath, MAX_PATH, _TRUNCATE, "%s\\Connection", strAdapterName.c_str());

    HKEY hAdapter = NULL;

    do
    {
        lRet = RegOpenKeyExA(hNetworkKey, szAdapterSubPath, 0, KEY_READ, &hAdapter);
        if (ERROR_SUCCESS != lRet)
        {
            break;
        }

        DWORD dwRegType = REG_DWORD;
        DWORD dwMediaSbuType = 0;
        DWORD dwDataLen = sizeof(dwMediaSbuType);
        lRet = RegQueryValueExA(hAdapter, "MediaSubType", NULL, &dwRegType, (BYTE *)&dwMediaSbuType, &dwDataLen);
        if (ERROR_SUCCESS != lRet)
        {
            // 虚拟机里面获取不到这个字段
            break; // Note，是否要break ？？？
        }

        dwRegType = REG_SZ;
        char szPnpInstanceId[MAX_PATH + 2] = {0};
        dwDataLen = MAX_PATH;
        lRet = RegQueryValueExA(hAdapter, "PnpInstanceID", NULL, &dwRegType, (BYTE *)szPnpInstanceId, &dwDataLen);
        if (ERROR_SUCCESS != lRet)
        {
            break;
        }

        // 如果MediaSubType = 1，且PnpInstanceID前面以PCI字样开头则表示是本地实际网卡
        // 如果MediaSubType = 1，且PnpInstanceID前面不以PCI字样开头则表示是本地虚拟网卡
        // 如果MediaSubType = 2，则表示是无线网卡

        // 如果考虑到虚拟机的场景，下面的if就不要判断 dwMediaSbuType == 1了，仅识别无线网卡就和PCI就好了。
        if (dwMediaSbuType == 1 && strncmp(szPnpInstanceId, "PCI", strlen("PCI") == 0))
        {
            enType = AT_CABLE;
        }
        else if (dwMediaSbuType == 1 && strncmp(szPnpInstanceId, "PCI", strlen("PCI") != 0))
        {
            enType = AT_VIRTUAL;
        }
        else if (dwMediaSbuType == 2)
        {
            enType = AT_WIRENESS;
        }
        else
        {
            enType = AT_UNKNOWN;
        }

    } while (false);

    if (hAdapter)
    {
        RegCloseKey(hAdapter);
        hAdapter = NULL;
    }

    if (hNetworkKey)
    {
        RegCloseKey(hNetworkKey);
        hNetworkKey = NULL;
    }

    return enType;
}

std::vector<std::string> GetPhysicalIpList()
{
    std::vector<std::string> vecIpList;

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

    if (ERROR_SUCCESS == nRet)
    {
        while (pIpAdapterInfo)
        {
            strAddress = pIpAdapterInfo->IpAddressList.IpAddress.String;
            if (strAddress.compare("0.0.0.0") != 0)
            {
                ADAPTER_TYPE enType = GetAdapterType(pIpAdapterInfo->AdapterName);
                if (enType == AT_CABLE || enType == AT_WIRENESS)
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
