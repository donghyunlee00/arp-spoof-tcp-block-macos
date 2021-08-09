#include <stdio.h>
#include <pcap.h>
#include "dev.h"

void usage()
{
    printf("syntax : arp-spoof-tcp-block <interface> <sender ip 1> <target ip 1> [<sender ip 2> <target ip 2>...]\n");
    printf("sample : arp-spoof-tcp-block en0 192.168.10.2 192.168.10.1 192.168.10.1 192.168.10.2\n");
    exit(-1);
}

int main(int argc, char *argv[])
{
    if (argc < 4)
        usage();

    Dev dev = Dev(argv[1]);
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live(std::string(dev).c_str(), BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL)
    {
        fprintf(stderr, "Couldn't open device %s: %s\n", std::string(dev).c_str(), errbuf);
        return -1;
    }

    Ip attacker_ip = dev.getIp();
    // printf("interface: %s\tip address: %s\n", std::string(dev).c_str(), std::string(attacker_ip).c_str());
    Mac attacker_mac = dev.getMac();
    // printf("interface: %s\tmac address: %s\n", std::string(dev).c_str(), std::string(attacker_mac).c_str());

    pcap_close(handle);
    return 0;
}