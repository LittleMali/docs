# iOA

## iOA流量转发流程


### win流程
ioa运行 -- ioa登录 -- StartSmartVPNFunc
    setevent - START_NGNENTBIZ_NAME
    |
    通知到了 NgnPlugin
        内部设置bool
        线程循环里面判断bool，拉起iOAEntBiz.exe
        


ioa登出 -- StopSmartVPNAgent

#### 拉起smartvpn

StartSmartVPNFunc
    

### Linux流程

名词定义：
iOANGN - daemon与ioagng通信，ioagng与ngnclient通信。
ngnclient - 就是smartvpn

    CNgnCoreDispatcher::DoAsynDispatch
        if ACT_START_NGN_PROC
            ServiceProtector::StartNgnProcInstant
                if !CheckIOANGNExist
                    StartProc - Plugins/TxNGNGuard/core/iOANGN
                        sh -c cmd
                if !CheckProxyProcExist
                    StartProc - Plugins/TxNGNGuard/core/ngnclient
                        拼接参数: -skipTLSVerify -logpath=/opt/tav/log/ -enableTrafficLog -fakeip=100.12.0.0/22
                        system cmd
                        FlushFlowModeCfg

        if ACT_STOP_NGN_PROC
            ServiceProtector::StopNgnProcInstant
                StopAndKillProc - iOANGN
                    pkill -9 cmd
                StopAndKillProc - ngnclient
                    pkill -9 cmd
                ClearDirectRouteOnExit