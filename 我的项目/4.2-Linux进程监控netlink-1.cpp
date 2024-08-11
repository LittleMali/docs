// 这是一段GPT给出的代码示例。

// 这个示例使用了libnl库，这是一个用于处理Netlink消息的库

// https://github.com/thom311/libnl
// libnl里面集成了netlink的各个能力，我们仅看进程监控的地方：src/nf-ct-events.c

#include <netlink/netlink.h>
#include <netlink/socket.h>
#include <linux/cn_proc.h>
#include <linux/connector.h>
#include <linux/netlink.h>
#include <iostream>

void handle_msg(struct nl_msg *msg, void *arg)
{
    struct nlmsghdr *nlh = nlmsg_hdr(msg);
    struct cn_msg *cn_hdr = (struct cn_msg *)NLMSG_DATA(nlh);
    struct proc_event *event = (struct proc_event *)cn_hdr->data;

    if (event->what == PROC_EVENT_EXEC)
    {
        std::cout << "Process " << event->event_data.exec.process_pid << " has started." << std::endl;
    }
}

int main()
{
    struct nl_sock *nl_sock = nl_socket_alloc();
    nl_connect(nl_sock, NETLINK_CONNECTOR);

    struct sockaddr_nl sa_nl;
    sa_nl.nl_family = AF_NETLINK;
    sa_nl.nl_groups = CN_IDX_PROC;
    sa_nl.nl_pid = getpid();

    bind(nl_socket_get_fd(nl_sock), (struct sockaddr *)&sa_nl, sizeof(sa_nl));

    nl_socket_modify_cb(nl_sock, NL_CB_VALID, NL_CB_CUSTOM, handle_msg, nullptr);

    while (true)
    {
        nl_recvmsgs_default(nl_sock);
    }

    nl_close(nl_sock);
    nl_socket_free(nl_sock);

    return 0;
}