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

    Dev attacker;
    attacker.setIfa(std::string(argv[1]));

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live(attacker.ifa().c_str(), BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL)
    {
        fprintf(stderr, "Couldn't open device %s: %s\n", attacker.ifa().c_str(), errbuf);
        return -1;
    }

    printf("interface: %s\tip address: %s\n", attacker.ifa().c_str(), std::string(attacker.myIp()).c_str());
    printf("interface: %s\tmac address: %s\n", attacker.ifa().c_str(), std::string(attacker.myMac()).c_str());

    // int num_victim = (argc - 2) / 2;
    // Flow *flow = new Flow[num_victim];
    // for (int i = 0; i < num_victim; i++)
    // {
    //     flow[i].sender_ip = Ip(argv[(i + 1) * 2]);
    //     myMac(handle, attacker_ip, attacker_mac, flow[i].sender_ip, &flow[i].sender_mac);

    //     flow[i].target_ip = Ip(argv[(i + 1) * 2 + 1]);
    //     myMac(handle, attacker_ip, attacker_mac, flow[i].target_ip, &flow[i].target_mac);

    //     arpInfect(handle, attacker_mac, flow[i].sender_ip, flow[i].sender_mac, flow[i].target_ip);
    // }

    pcap_close(handle);
    return 0;
}
