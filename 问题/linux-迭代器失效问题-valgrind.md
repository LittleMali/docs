# 迭代器失效问题定位

在使用valgrind检测内存占用问题时，valgrind运行一段时间以后退出，报错进程退出。
```
==9781== 
==9781== Process terminating with default action of signal 11 (SIGSEGV): dumping core
==9781==  Access not within mapped region at address 0x19
==9781==    at 0x81E8680: std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) (in /opt/apps/files/bin/libtxcore.so)
==9781==    by 0x5467573: CronTaskPlugin::CronInfo::CronInfo(CronTaskPlugin::CronInfo const&) (crontaskplugin.h:68)
==9781==    by 0x546218E: CronTaskPlugin::DispatchEvent(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) (crontaskplugin.cpp:210)
==9781==    by 0x5462663: std::_Function_handler<void (), CronTaskPlugin::OnRecvRunTimer(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&)::{lambda()#1}>::_M_invoke(std::_Any_data const&) (std_function.h:300)
...
==9781==    by 0x854D608: start_thread (pthread_create.c:477)
==9781==    by 0x8689292: clone (clone.S:95)

==9781==  If you believe this happened as a result of a stack
==9781==  overflow in your program's main thread (unlikely but
==9781==  possible), you can try to increase the size of the
==9781==  main thread stack using the --main-stacksize= flag.
==9781==  The main thread stack size used in this run was 8388608.
```
看上面报错的异常栈，是跟CronTaskPlugin有关。

看CronTaskPlugin的日志，核心几行如下。
```
[ INFO][2024-02-01 18:59:02.149.404][9781.47418][void CronTaskPlugin::OnRecvRunTimer(const string&, std::string&)@193]: cron task arrived: params({"Param":"","Tag":"crontask_tag_xxx_check"})
[ INFO][2024-02-01 18:59:02.510.134][9781.47430][void CronTaskPlugin::DispatchEvent(const string&, const string&)@213]: Dispatched event tag(crontask_tag_xxx_check) param() successfully. 

[ INFO][2024-02-01 18:59:10.801.682][9781.47430][virtual bool CronTaskPlugin::Delete(const string&)@164]: delete cron task: tag(crontask_tag_xxx_check)

[ INFO][2024-02-01 18:59:10.891.216][9781.47430][virtual bool CronTaskPlugin::Attach(const string&, const string&, void*, FnCronTimerCallback, const string&)@69]: New cron task: crontab(59 19 1 2 *), tag(crontask_tag_xxx_check), params()
```
1. CronTaskPlugin::OnRecvRunTimer，收到通知以后，进行DispatchEvent。
2. 在DispatchEvent时，for循环逐个进行回调。回调里面还是做了不少事情的，因为回调都是一个线程，我们可以通过追踪tid=47430来确认。
3. 在日志中看到，tid=47430最后走到了Delete逻辑，Delete里面删除了callback，因此，可能导致变量被析构了。在回调执行的时候，对象被析构了，因此，出异常了。
4. 再确认一下，我们有加锁，但是，recursive_mutex在同一线程内是是可以重入的，所以，这里锁不住。我们相当于在回调里面绕了一圈，又操作回了callback。
5. callback里面存的是CronInfo，是一个对象。我们for循环的时候，用的是临时对象，都是对象的操作，不存在析构。
6. 因此，准确的说，是Delete时erase元素，导致DispatchEvent时迭代器失效。
7. 所以，这里报stack overflow，也可以理解。

```c++
// std::list<CronInfo> m_eventCallbacks;

bool CronTaskPlugin::Delete(const std::string& tag) {
    std::unique_lock<std::recursive_mutex> lock(m_mtxForCallbacks);
    for (auto i = m_eventCallbacks.begin(); i != m_eventCallbacks.end();) {
        if (i->tag == tag) {
            i = m_eventCallbacks.erase(i);
        } else {
            i++;
        }
    }

    return true;
}

void CronTaskPlugin::DispatchEvent(const std::string& tag, const std::string& param) {
    std::unique_lock<std::recursive_mutex> lock(m_mtxForCallbacks);
    for (auto c : m_eventCallbacks) {
        if (c.tag == tag) {
            if (c.oncall) {
                c.oncall(param);
            }
        }
    }
}
```