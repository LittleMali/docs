// # Linux流量监控libpcap
// demo代码，未实际验证，来自GPT
// 代码目录：监控本机所有物理网卡的tcp和udp报文。

#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

// user_data：上下文环境，在c++可以用来传递this指针。
// pkthdr：pcap捕获的报文信息，包括捕获时间等。
// packet：报文原始内容，数据链路层的报文。
void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    // 以太网头部
    struct ether_header *eth_header = (struct ether_header *)packet;

    // IP头部
    struct ip *ip_header = (struct ip *)(packet + sizeof(struct ether_header));

    // 打印源和目的IP地址
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ip_header->ip_src, src_ip, sizeof(src_ip));
    inet_ntop(AF_INET, &ip_header->ip_dst, dst_ip, sizeof(dst_ip));

    // 检查协议类型
    if (ip_header->ip_p == IPPROTO_TCP)
    {
        struct tcphdr *tcp_header = (struct tcphdr *)(packet + sizeof(struct ether_header) + sizeof(struct ip));
        printf("TCP Packet: %s:%d -> %s:%d\n", src_ip, ntohs(tcp_header->source), dst_ip, ntohs(tcp_header->dest));
    }
    else if (ip_header->ip_p == IPPROTO_UDP)
    {
        struct udphdr *udp_header = (struct udphdr *)(packet + sizeof(struct ether_header) + sizeof(struct ip));
        printf("UDP Packet: %s:%d -> %s:%d\n", src_ip, ntohs(udp_header->source), dst_ip, ntohs(udp_header->dest));
    }
}

int main()
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs, *device;

    // 查找所有网络设备
    // 除了pcap_lookupdev，下面的函数也可以遍历。
    if (pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        fprintf(stderr, "Error finding devices: %s\n", errbuf);
        return 1;
    }

    // 遍历所有设备，选择物理网卡
    for (device = alldevs; device != NULL; device = device->next)
    {
        if (device->flags & PCAP_IF_LOOPBACK)
        {
            continue; // 跳过回环接口
        }
        printf("Using device: %s\n", device->name);

        // 打开设备进行捕获
        // 65535，单个报文捕获最大长度。
        pcap_t *handle = pcap_open_live(device->name, 65535, 1, 1000, errbuf);
        if (handle == NULL)
        {
            fprintf(stderr, "Error opening device %s: %s\n", device->name, errbuf);
            continue;
        }

        // 设置过滤器，捕获 TCP 和 UDP 数据包
        struct bpf_program filter;
        const char *filter_exp = "tcp or udp";
        if (pcap_compile(handle, &filter, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1)
        {
            fprintf(stderr, "Error compiling filter: %s\n", pcap_geterr(handle));
            pcap_close(handle);
            continue;
        }
        if (pcap_setfilter(handle, &filter) == -1)
        {
            fprintf(stderr, "Error setting filter: %s\n", pcap_geterr(handle));
            pcap_freecode(&filter);
            pcap_close(handle);
            continue;
        }
        pcap_freecode(&filter);

        // 开始捕获数据包
        pcap_loop(handle, 0, packet_handler, NULL);

        // 关闭捕获
        pcap_close(handle);
    }

    // 释放设备列表
    pcap_freealldevs(alldevs);
    return 0;
}