#include <pcap.h>
#include <ifaddrs.h>
#include <string>
#include "dev.h"

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