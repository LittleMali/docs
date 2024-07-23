# bugreport

每个进程要接入bugreport，以便在异常crash的时候触发dmp上报。

接入bugreport的关键步骤就是设置异常捕获：SetUnhandledExceptionFilter。
```c++
InitBugReport
    InitBugReportSupport

    ValidateBugReport
        // 设置纯虚函数调用的异常处理器
        _set_purecall_handler(custom_purecall_handler);

        // 设置参数错误的异常处理器
        _set_invalid_parameter_handler(custom_invalid_parameter_handler);

        ::SetUnhandledExceptionFilter(BugReportExceptionFilter);


custom_purecall_handler
    custom_handler(STATUS_FATAL_APP_EXIT);

custom_invalid_parameter_handler
    custom_handler(STATUS_INVALID_CRUNTIME_PARAMETER);

BugReportExceptionFilter
    DoBugReport
        // 获取当前进程信息： Exception, teb, peb, tid, procInfos.
        run bugreport.exe /params
        waitfor exe quit
```

bugreport进程。
