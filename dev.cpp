#include <pcap.h>
#include <ifaddrs.h>
#include <string>
#include "dev.h"

#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>

/* REFERENCE: https://man7.org/linux/man-pages/man3/getifaddrs.3.html */
Ip Dev::getIp() const
{
    ifaddrs *ifaddr;
    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(-1);
    }

    const char *dev = dev_.c_str();
    size_t devlen = strlen(dev);
    for (ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        int family = ifa->ifa_addr->sa_family;
        if (family != AF_INET)
            continue;

        if (strncmp(ifa->ifa_name, dev, devlen) != 0)
            continue;

        char host[NI_MAXHOST];
        int s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        if (s != 0)
        {
            printf("getnameinfo() failed: %s\n", gai_strerror(s));
            exit(-1);
        }
        freeifaddrs(ifaddr);
        return Ip(host);
    }

    exit(-1);
}

/* REFERENCE: https://stackoverflow.com/questions/10593736/mac-address-from-interface-on-os-x-c */
Mac Dev::getMac() const
{
    int mib[6] = {CTL_NET, AF_ROUTE, 0, AF_LINK, NET_RT_IFLIST};
    if ((mib[5] = if_nametoindex(std::string(dev_).c_str())) == 0)
    {
        perror("if_nametoindex error");
        exit(-1);
    }

    size_t len;
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0)
    {
        perror("sysctl 1 error");
        exit(-1);
    }

    char *buf;
    if ((buf = (char *)malloc(len)) == NULL)
    {
        perror("malloc error");
        exit(-1);
    }

    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0)
    {
        perror("sysctl 2 error");
        exit(-1);
    }

    if_msghdr *ifm = (if_msghdr *)buf;
    sockaddr_dl *sdl = (sockaddr_dl *)(ifm + 1);
    return Mac((uint8_t *)LLADDR(sdl));
}