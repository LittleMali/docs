#include "stdafx.h"
#include <Strsafe.h>
#include <psapi.h>
#include "FileHandleMgr.h"

PFN_ZwQuerySystemInformation CFileHandleMgr::m_pfnZwQuerySystemInformation = NULL;
PFN_ZwQueryObject CFileHandleMgr::m_pfnZwQueryObject = NULL;

CFileHandleMgr::CFileHandleMgr()
{
    m_dwTxtFileType = -1;
    m_hWorkThread = NULL;
    m_hWorkEvent = NULL;
    m_hQuitEvent = NULL;

    m_bQuit = false;
}

CFileHandleMgr::~CFileHandleMgr()
{
}

bool CFileHandleMgr::Init()
{
    bool bret = false;
    do
    {
        m_hWorkEvent = CreateEvent(NULL, TRUE, FALSE, L"start-work");
        if (!m_hWorkEvent)
        {
            DBGLOGA("init work event failed");
            break;
        }

        m_hQuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (!m_hQuitEvent)
        {
            DBGLOGA("init quit event failed");
            break;
        }

        if (!InitNtdll())
        {
            DBGLOGA("init ntdll failed");
            break;
        }

        if (!GetFileObjectType())
        {
            DBGLOGA("get file object type failed");
            break;
        }

        m_strTargetFilePath = "c:\\test.txt";

        bret = true;
    } while (false);

    if (!bret)
    {
        UnInit();
    }

    DBGLOGA("file handle mgr init, bret: %d", bret);
    return bret;
}

void CFileHandleMgr::UnInit()
{
    m_bQuit = true;

    if (m_hQuitEvent)
    {
        SetEvent(m_hQuitEvent);
    }

    if (m_hWorkThread)
    {
        DWORD dwRet = WaitForSingleObject(m_hWorkThread, INFINITE);
        if (dwRet == WAIT_OBJECT_0)
        {
            CloseHandle(m_hWorkThread);
            m_hWorkThread = NULL;
        }
        else
        {
            DBGLOGA("wait work thread quit failed, last err: %d", GetLastError());
            TerminateThread(m_hWorkThread, 2);
            CloseHandle(m_hWorkThread);
            m_hWorkThread = NULL;
        }
    }

    if (m_hQuitEvent)
    {
        CloseHandle(m_hQuitEvent);
        m_hQuitEvent = NULL;
    }

    if (m_hWorkEvent)
    {
        CloseHandle(m_hWorkEvent);
        m_hWorkEvent = NULL;
    }
}

void CFileHandleMgr::WaitForWork()
{
    HANDLE hWorkThread = (HANDLE)_beginthreadex(NULL, 0, &CFileHandleMgr::WorkThreadFunc, this, 0, 0);
    if (!hWorkThread)
    {
        DBGLOGA("create work thread failed");
        return;
    }

    m_hWorkThread = hWorkThread;
}

bool CFileHandleMgr::InitNtdll()
{
    HMODULE hNtdll = LoadLibrary(L"Ntdll.dll");
    if (!hNtdll)
    {
        return false;
    }

    PFN_ZwQuerySystemInformation pfnZwQuerySystemInformation = (PFN_ZwQuerySystemInformation)GetProcAddress(hNtdll, "ZwQuerySystemInformation");
    if (!pfnZwQuerySystemInformation)
    {
        DBGLOGA("get query pfn failed");
        return false;
    }

    PFN_ZwQueryObject pfnZwQueryObject = (PFN_ZwQueryObject)GetProcAddress(hNtdll, "ZwQueryObject");
    if (!pfnZwQueryObject)
    {
        DBGLOGA("get query pfn failed");
        return false;
    }

    m_pfnZwQuerySystemInformation = pfnZwQuerySystemInformation;
    m_pfnZwQueryObject = pfnZwQueryObject;
    return true;
}

bool CFileHandleMgr::GetFileObjectType()
{
    if (!m_pfnZwQuerySystemInformation)
    {
        return false;
    }

    // step1: create text file
    wchar_t szTextFilePath[MAX_PATH + 2] = L"57719069-DA06-4397-94EF-CA96318D2FC9.txt";
    HANDLE hFile = CreateFileW(szTextFilePath, GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        DBGLOGW(L"create text file failed: %s.", szTextFilePath);
        return false;
    }

    // step2: enum all system handles
    PSYSTEM_HANDLE_INFORMATION_EX pSystemHandleInfo = NULL;
    ULONG ulBufferSize = 0x200;
    pSystemHandleInfo = (PSYSTEM_HANDLE_INFORMATION_EX)malloc(ulBufferSize);

    ULONG ulRetSize = 0;
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    do
    {
        if (!pSystemHandleInfo)
        {
            break;
        }

        ntStatus = m_pfnZwQuerySystemInformation(SystemHandleInformation, pSystemHandleInfo, ulBufferSize, &ulRetSize);
        if (NT_SUCCESS(ntStatus))
        {
            break;
        }

        if (STATUS_INFO_LENGTH_MISMATCH == ntStatus)
        {
            free(pSystemHandleInfo);
            pSystemHandleInfo = NULL;

            ulBufferSize = ulRetSize + 0x1000;
            pSystemHandleInfo = (PSYSTEM_HANDLE_INFORMATION_EX)malloc(ulBufferSize);
            continue;
        }

        break;
    } while (true);

    if (!NT_SUCCESS(ntStatus) || !pSystemHandleInfo)
    {
        if (!pSystemHandleInfo)
        {
            free(pSystemHandleInfo);
            pSystemHandleInfo = NULL;
        }

        CloseHandle(hFile);
        DeleteFileW(szTextFilePath);
        return false;
    }

    // step3: get text file type
    // 不同的系统实现可能不一样，动态获取比较好
    DWORD dwCurProcId = GetCurrentProcessId();
    for (ULONG i = 0; i < pSystemHandleInfo->NumberOfHandles; ++i)
    {
        SYSTEM_HANDLE_INFORMATION handleInfoItem = pSystemHandleInfo->Information[i];
        // 只能拿到pid小于65535的进程，因此，获取FileType不一定能成功
        if (handleInfoItem.UniqueProcessId != dwCurProcId)
        {
            continue;
        }

        if (handleInfoItem.Handle == (USHORT)hFile)
        {
            m_dwTxtFileType = handleInfoItem.ObjectTypeNumber;
            break;
        }
    }

    free(pSystemHandleInfo);
    CloseHandle(hFile);
    DeleteFileW(szTextFilePath);

    DBGLOGA("get file type finished, txt file type: %d", m_dwTxtFileType);

    return true;
}

unsigned WINAPI CFileHandleMgr::WorkThreadFunc(LPVOID lpParam)
{
    CFileHandleMgr *pThis = (CFileHandleMgr *)lpParam;
    if (!pThis)
    {
        return 0;
    }

    pThis->WaitForWorkImpl();
    return 0;
}

void CFileHandleMgr::WaitForWorkImpl()
{
    if (!m_hWorkEvent || !m_hQuitEvent)
    {
        DBGLOGA("event is NOT valid, maybe init failed");
        return;
    }

    HANDLE hEvents[] = {m_hQuitEvent, m_hWorkEvent};
    DWORD dwCnt = sizeof(hEvents) / sizeof(hEvents[0]);
    do
    {
        DWORD dwRet = WaitForMultipleObjects(dwCnt, hEvents, FALSE, INFINITE);
        if (dwRet == WAIT_OBJECT_0)
        {
            DBGLOGA("notify quit");
            break;
        }
        else if (dwRet == WAIT_OBJECT_0 + 1)
        {
            DBGLOGA("notify work");
            EnumFileHandles();

            ResetEvent(m_hWorkEvent);

            if (m_bQuit)
            {
                DBGLOGA("quit because of uninit");
                break;
            }
        }
        else
        {
            DBGLOGA("wait error: %d", GetLastError());
            break;
        }
    } while (true);

    DBGLOGA("work thread is quiting");
}

void CFileHandleMgr::TryFindDbHandles()
{
}

void CFileHandleMgr::EnumFileHandles()
{
    // 枚举当前句柄
    PSYSTEM_HANDLE_INFORMATION_EX pSystemHandleInfo = NULL;
    ULONG ulBufferSize = 0x200;
    pSystemHandleInfo = (PSYSTEM_HANDLE_INFORMATION_EX)malloc(ulBufferSize);

    ULONG ulRetSize = 0;
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    do
    {
        if (!pSystemHandleInfo)
        {
            break;
        }

        ntStatus = m_pfnZwQuerySystemInformation(SystemHandleInformation, pSystemHandleInfo, ulBufferSize, &ulRetSize);
        if (NT_SUCCESS(ntStatus))
        {
            break;
        }

        if (STATUS_INFO_LENGTH_MISMATCH == ntStatus)
        {
            free(pSystemHandleInfo);
            pSystemHandleInfo = NULL;

            ulBufferSize = ulRetSize + 0x1000;
            pSystemHandleInfo = (PSYSTEM_HANDLE_INFORMATION_EX)malloc(ulBufferSize);
            continue;
        }

        break;
    } while (true);

    if (!NT_SUCCESS(ntStatus) || !pSystemHandleInfo)
    {
        if (!pSystemHandleInfo)
        {
            free(pSystemHandleInfo);
            pSystemHandleInfo = NULL;
        }

        DBGLOGA("enum handles failed");
        return;
    }

    // 遍历文件句柄，找到谁打开了Enterprise_GlobalMgr.db文件
    for (ULONG i = 0; i < pSystemHandleInfo->NumberOfHandles; ++i)
    {
        if (m_bQuit)
        {
            break;
        }

        SYSTEM_HANDLE_INFORMATION handleInfoItem = pSystemHandleInfo->Information[i];
        if (handleInfoItem.UniqueProcessId <= 4)
        {
            continue;
        }

        if (m_dwTxtFileType != -1 && m_dwTxtFileType != handleInfoItem.ObjectTypeNumber)
        {
            // 获取filetype时，增加文件类型判断，减少查询次数。
            // 当前句柄的filetype不是我们关心的类型。我们只关心文本type。
            continue;
        }

        QueryHandleInfo queryHandle;
        queryHandle.dwPid = (DWORD)handleInfoItem.UniqueProcessId;
        queryHandle.hFile = (HANDLE)handleInfoItem.Handle;
        queryHandle.ntQueryRet = STATUS_UNSUCCESSFUL;
        if (GetFileNameUsingThread(queryHandle)) // 获取文件名
        {
            if (GetLongDosPathFromNtPath(queryHandle.wszFileName)) // 路径转换
            {
                if (StrCmpIW(queryHandle.wszFileName, m_strTargetFilePath.c_str()) == 0) // 判断是否目标文件
                {
                    // 打印进程信息
                    std::wstring strProcFullPath;
                    BOOL bGet = GetProcessPathByPid(queryHandle.dwPid, strProcFullPath);
                    LOG_WARN(L"Match DB file !!!");
                    LOG_WARN(L"get: %d, proc id: %d, name: %s, is opening db !!!", bGet, queryHandle.dwPid, strProcFullPath.c_str());
                }
            }
        }
    }

    free(pSystemHandleInfo);
    pSystemHandleInfo = NULL;
}

bool CFileHandleMgr::GetFileNameUsingThread(QueryHandleInfo &queryInfo)
{
    // ZwQueryObject查询文件路径时，可能发送阻塞，通过线程超时的方式来调用ZwQueryObject，避免卡死。
    bool bret = false;

    HANDLE hQueryThread = (HANDLE)_beginthreadex(NULL, 0, &CFileHandleMgr::QueryInfoThreadFunc, (PVOID)&queryInfo, 0, NULL);
    if (hQueryThread)
    {
        DWORD dwRet = WaitForSingleObject(hQueryThread, 1 * 1000);
        if (dwRet == WAIT_OBJECT_0)
        {
            if (NT_SUCCESS(queryInfo.ntQueryRet) && wcslen(queryInfo.wszFileName) != 0)
            {
                bret = true;
            }
        }
        else
        {
            DBGLOGA("wait for query info thread timeout or failed");
            TerminateThread(hQueryThread, 3);
            hQueryThread = NULL;
        }
    }

    return bret;
}

unsigned WINAPI CFileHandleMgr::QueryInfoThreadFunc(LPVOID lpParam)
{
    PQueryHandleInfo pQueryInfo = (PQueryHandleInfo)lpParam;
    if (!pQueryInfo)
    {
        return 1;
    }

    // duplicate file handle
    HANDLE hProcess = NULL;
    HANDLE hFileCopy = NULL;
    do
    {
        hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, pQueryInfo->dwPid);
        if (!hProcess)
        {
            // DBGLOGA("open process failed, err: %d", GetLastError());
            break;
        }

        if (!DuplicateHandle(hProcess, pQueryInfo->hFile,
                             ::GetCurrentProcess(), &hFileCopy,
                             DUPLICATE_SAME_ACCESS, FALSE, DUPLICATE_SAME_ACCESS))
        {
            // DBGLOGA("dup handle failed, err: %d", GetLastError());
            break;
        }

        if (!hFileCopy)
        {
            break;
        }
    } while (false);

    if (!hFileCopy)
    {
        if (!hProcess)
        {
            CloseHandle(hProcess);
            hProcess = NULL;
        }
        return 1;
    }

    // query file name
    ULONG ulNameSize = 0x1000;
    PUNICODE_STRING pusName = NULL;
    do
    {
        pusName = (PUNICODE_STRING)malloc(ulNameSize);
        if (!pusName)
        {
            break;
        }

        DWORD dwType = GetFileType(hFileCopy);
        if (dwType != FILE_TYPE_DISK)
        {
            // DBGLOGA("do not care this type: %d", dwType);
            break;
        }

        NTSTATUS ntStatus = m_pfnZwQueryObject(hFileCopy, ObjectNameInformation, pusName, ulNameSize, NULL);
        if (NT_SUCCESS(ntStatus) && pusName->Buffer && pusName->Length)
        {
            RtlCopyMemory(pQueryInfo->wszFileName, pusName->Buffer, pusName->Length);
            pQueryInfo->ntQueryRet = STATUS_SUCCESS;
        }

    } while (false);

    if (pusName)
    {
        free(pusName);
        pusName = NULL;
    }

    CloseHandle(hProcess);
    CloseHandle(hFileCopy);

    return 0;
}

// Translate path with device name to drive letters.
// Buffer must be MAX_PATH + 1 long.
// https://msdn.microsoft.com/en-us/library/windows/desktop/aa366789(v=vs.85).aspx
BOOL CFileHandleMgr::GetLongDosPathFromNtPath(LPWSTR buffer)
{
    const ULONG BUFSIZE = 512;
    WCHAR szTemp[BUFSIZE + 1] = {0};
    BOOL bRet = FALSE;
    if (GetLogicalDriveStrings(BUFSIZE - 1, szTemp))
    {
        WCHAR szName[MAX_PATH + 1] = {0};
        WCHAR szDrive[3] = TEXT(" :");
        BOOL bFound = FALSE;
        WCHAR *p = szTemp;
        do
        {
            // Copy the drive letter to the template string
            *szDrive = *p;

            // Look up each device name
            if (QueryDosDevice(szDrive, szName, MAX_PATH))
            {
                size_t uNameLen = _tcslen(szName);

                if (uNameLen < MAX_PATH)
                {
                    bFound = _tcsnicmp(buffer, szName, uNameLen) == 0 && *(buffer + uNameLen) == _T('\\');

                    if (bFound)
                    {
                        // Reconstruct pszFilename using szTempFile
                        // Replace device path with DOS path
                        TCHAR szTempFile[MAX_PATH + 1] = {0};
                        StringCchPrintf(szTempFile,
                                        MAX_PATH,
                                        TEXT("%s%s"),
                                        szDrive,
                                        buffer + uNameLen);
                        StringCchCopyN(buffer, MAX_PATH + 1, szTempFile, _tcslen(szTempFile));
                    }
                }
            }

            // Go to the next NULL character.
            while (*p++)
                ;
        } while (!bFound && *p); // end of string

        bRet = (GetLongPathName(buffer, buffer, MAX_PATH) != 0);
    }

    return bRet;
}

BOOL CFileHandleMgr::GetProcessPathByPid(const DWORD dwPid, std::wstring &strProcPath)
{
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, dwPid);
    if (!hProc)
    {
        return FALSE;
    }

    wchar_t szFileImage[MAX_PATH + 2] = {0};
    DWORD dwRet = GetProcessImageFileNameW(hProc, szFileImage, MAX_PATH);
    if (!dwRet)
    {
        DBGLOGA("get file image failed, last err: %d", ::GetLastError());
        return FALSE;
    }

    if (!GetLongDosPathFromNtPath(szFileImage))
    {
        DBGLOGA("get dos path failed");
        return FALSE;
    }

    strProcPath = szFileImage;
    return TRUE;
}
