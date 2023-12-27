# 跨session通信

因为session隔离的问题，服务进程是运行在session 0下，无法直接在session 1的用户空间展示ui。  
如果是简单的ui提示，可以用WTSSendMessage。  
如果是复杂的ui，则要通过CreateProcessAsUser拉取独立进程用于展示。
WCF技术。