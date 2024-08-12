#Linux进程监控之preload

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <dlfcn.h>
#include <string.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif

    __attribute__((constructor)) static void SoInit(void)
    {
        // 当前进程是系统进程，则注入后不要工作
        char procPath[1024] = {0};
        if (readlink("/proc/self/exe", procPath, 1023) <= 0)
        {
            return;
        }

        if (IsFilter(procPath))
        {
            return;
        }

        g_init = true;
    }

    __attribute__((destructor)) static void SoFinal(void)
    {
        if (!g_init)
            return;

        // do something
    }

    // hook api: execve
    typedef int (*execve_func_t)(const char *filename, char *const argv[], char *const envp[]);
    int execve(const char *filename, char *const argv[], char *const envp[])
    {
        execve_func_t old_execve = (execve_func_t)dlsym((void *)RTLD_NEXT, "execve");

        // 过滤掉不关心的进程
        if (!IsFilter(filename))
        {
            auto pid = getpid();                        // get pid
            auto ret = HandleProc(pid, filename, argv); // 可以直接处理，也可以ipc通知出去给别人处理。

            if (ret != 0)
            {
                return -1;
            }
        }

        if (old_execve)
        {
            return old_execve(filename, argv, envp);
        }

        return -1;
    }

#ifdef __cplusplus
}
#endif