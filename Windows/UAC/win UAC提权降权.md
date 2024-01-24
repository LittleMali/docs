# UAC提权降权

核心点：子进程的权限（token令牌）默认是继承自父进程的。

以win10管理员账号为例，直接双击运行进程，进程是通过explorer拉起的，由于explorer是普通权限（Medium），因此，拉起的进程默认也是Medium权限。但是，如果进程的Manifest明确要求了`requireAdministrator`，那么，会弹出uac弹框，允许确认以后才能运行。

* 场景一：以普通用户的权限运行了A程序，然后用A程序运行B程序，但是B程序被提升到管理员权限，而不是继承A程序的权限（如果打开UAC的话，这个提权过程系统会弹出窗口进行确认的）。
  * 例如双击运行procexp.exe，procexp是普通权限，Medium权限的procexp是查看不了High权限的进程信息的。procexp以Run as Administrator拉起Notepad.exe，那么notepad是high权限。
* 场景二：C程序是通过管理员权限执行的，然后用C程序运行D程序，但是D程序不继承C程序的管理员权限，而是降级到一个普通用户的权限。（对于降级，系统是不会提示的）。
  * 例如，安装包进程是high权限，安装完成时拉起进程，希望进程是medium权限。

## 如何判断当前进程的权限
```c++
BOOL IsTokenIsElevated() {
	HANDLE hToken = NULL;
	// 打开当前进程的访问令牌
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		return FALSE;
	}

	// 获取令牌信息
	TOKEN_ELEVATION token;
	DWORD dwSize;
	if (!GetTokenInformation(hToken, TokenElevation, &token, sizeof(token), &dwSize)) {
		CloseHandle(hToken);
		return FALSE;
	}

	BOOL isAdmid = token.TokenIsElevated;
	printf("token.TokenIsElevated: %d\n", isAdmid);

	CloseHandle(hToken);
	return isAdmid;
}
```
TokenElevation查询的结果是是否提权，可以直接获取进程的权限状态。
* 如果是asInvoker，双击运行，那么TokenIsElevated = false。
* 如果是asInvoker，管理员运行，那么TokenIsElevated = true。
* 如果是requireAdministrator，运行，那么TokenIsElevated = true。

```c++
TOKEN_ELEVATION_TYPE tokenType;
DWORD dwSize = 0;
if (!GetTokenInformation(hToken, TokenElevationType, &tokenType, sizeof(tokenType), &dwSize)) {
    CloseHandle(hToken);
    return false;
}
```
TokenElevationType查询的是token的详情，它是一个枚举，如下。
 * TokenElevationTypeDefaule
   * The token does not have a linked token.
   * 表示一个进程正在使用默认的用户访问令牌，或者没有开启UAC功能。
 * TokenElevationTypeFull
   * The token is an elevated token.
   * 进程的权限已被提升，进程令牌没有使用filtertoken。
 * 第三个TokenElevationTypeLimited
   * The token is a limited token.
   * 进程使用的令牌是Filter令牌，此时通过GetTokenInformaiton和TokenLinkedToken标志，来获取原令牌（管理员令牌）
  
注意：在win10上，即使将UAC调制最低，UAC依旧是开启状态的。如果要彻底关闭UAC，需要修改注册表`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System`，将`EnableLUA`改为0。

## UAC提权
### runas
如果一个未提权的程序需要运行一个提权的程序，所有它需要做的是调用 ShellExecuteEx() API ，然后指示一个参数为"runas"。
```c
BOOL
RunElevated(    HWND hwnd,
        LPCTSTR pszPath,
        LPCTSTR pszParameters = NULL,
        LPCTSTR pszDirectory = NULL )
{
    SHELLEXECUTEINFO shex;

    memset( &shex, 0, sizeof( shex) );

    shex.cbSize        = sizeof( SHELLEXECUTEINFO );
    shex.fMask        = 0;
    shex.hwnd        = hwnd;
    shex.lpVerb        = _T("runas");
    shex.lpFile        = pszPath;
    shex.lpParameters    = pszParameters;
    shex.lpDirectory    = pszDirectory;
    shex.nShow        = SW_NORMAL;

    return ::ShellExecuteEx( &shex );
}
```
### requireAdministrator
为子进程添加manifest文件，在manifest中指定要求管理员权限（requireAdministrator）。

## UAC降权
在 Windows 系统中，管理员权限和非管理员权限运行的程序之间不能使用 Windows 提供的通信机制进行通信。对于部分文件夹（ProgramData），管理员权限创建的文件是不能以非管理员权限修改和删除的。

从相反的方向（从一个已经提权的进程来执行一个没有提权的进程）变得非常复杂。如果父进程已经提权了的，那么它所执行的所有子程序都将继承它的已经提权的权限，而且无论子程序的清单（manifest）中的 requestedExecutionLevel 属性值是如何指定的。由于某些原因，微软并没有提供一个API来直接降低进程的权限，所以我们需要想出一个间接的办法来达到目的。

### explorer.exe
```c++
void RunExeByExplorer() {
	TCHAR processPath[MAX_PATH] = L"explorer.exe D:\\Program Files\\Notepad++\\notepad++.exe";

	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	si.cb = sizeof(si);
	CreateProcess(NULL, processPath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
}
```
explorer.exe是Medium权限，而且explorer一般都存在，并且是比较早就运行了。我们可以通过explorer拉起其他进程。
1. 进程A是管理员权限High。
2. 进程A通过RunExeByExplorer拉起notepad++，notepad++实际是通过explorer间接拉起的，notepad++实际继承的是explorer的Medium权限。
3. 不能通过ShellExecute open方式打开notepad，这个方式应该是通过CreateProcess来起进程的，因此，notepad实际是拿到了父进程的high权限。

### CreateProcessAsUser
用这个api拉起进程。
