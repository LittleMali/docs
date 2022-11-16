#ifndef _FILE_HANDLE_MGR_H_
#define _FILE_HANDLE_MGR_H_

#include <winternl.h>

//////////////////////////////////////////////////////////////////////////

// typedef LONG NTSTATUS;

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#endif

#ifndef STATUS_UNSUCCESSFUL
#define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)
#endif

#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)

// ULONG ProcessId is wrong!!!!
typedef struct _SYSTEM_HANDLE_INFORMATION
{
    USHORT UniqueProcessId;
    USHORT CreatorBackTraceIndex;
    UCHAR ObjectTypeNumber;
    UCHAR Flags;
    USHORT Handle;
    PVOID Object;
    ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef struct _SYSTEM_HANDLE_INFORMATION_EX
{
    ULONG NumberOfHandles;
    SYSTEM_HANDLE_INFORMATION Information[1];
} SYSTEM_HANDLE_INFORMATION_EX, *PSYSTEM_HANDLE_INFORMATION_EX;

typedef enum _SYSTEM_INFORMATION_CLASS_EX
{
    SystemHandleInformation = 16,
} SYSTEM_INFORMATION_CLASS_EX;

typedef enum _OBJECT_INFORMATION_CLASS
{
    ObjectBasicInformation,     // = 0
    ObjectNameInformation,      // = 1
    ObjectTypeInformation,      // = 2
    ObjectTypesInformation,     // = 3    //object handle is ignored
    ObjectHandleFlagInformation // = 4
} OBJECT_INFORMATION_CLASS;

typedef NTSTATUS(WINAPI *PFN_ZwQuerySystemInformation)(
    IN SYSTEM_INFORMATION_CLASS_EX SystemInformationClass,
    IN OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength);

typedef NTSTATUS(WINAPI *PFN_ZwQueryObject)(
    IN HANDLE ObjectHandle,
    IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
    OUT PVOID ObjectInformation,
    IN ULONG Length,
    OUT PULONG ResultLength OPTIONAL);

//////////////////////////////////////////////////////////////////////////

typedef struct _QueryHandleInfo
{
    DWORD dwPid;
    HANDLE hFile;
    NTSTATUS ntQueryRet;
    wchar_t wszFileName[MAX_PATH + 2];

    _QueryHandleInfo()
    {
        ZeroMemory(this, sizeof(_QueryHandleInfo));
    }

} QueryHandleInfo, *PQueryHandleInfo;

//////////////////////////////////////////////////////////////////////////

class CFileHandleMgr
{
public:
    CFileHandleMgr();
    ~CFileHandleMgr();

public:
    bool Init();
    void UnInit();

    void WaitForWork();

private:
    bool InitNtdll();
    bool GetFileObjectType();

    static unsigned WINAPI WorkThreadFunc(LPVOID lpParam);
    void WaitForWorkImpl();

    void TryFindDbHandles();
    void EnumFileHandles();
    bool GetFileNameUsingThread(QueryHandleInfo &queryInfo);
    BOOL GetLongDosPathFromNtPath(LPWSTR buffer);
    BOOL GetProcessPathByPid(const DWORD dwPid, std::wstring &strProcPath);

    static unsigned WINAPI QueryInfoThreadFunc(LPVOID lpParam);

private:
    static PFN_ZwQuerySystemInformation m_pfnZwQuerySystemInformation;
    static PFN_ZwQueryObject m_pfnZwQueryObject;

    DWORD m_dwTxtFileType;

    HANDLE m_hWorkThread;
    HANDLE m_hWorkEvent;
    HANDLE m_hQuitEvent;

    bool m_bQuit;

    std::wstring m_strTargetFilePath;
};

#endif
