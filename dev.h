#pragma once

#include <string>
#include "ip.h"
#include "mac.h"

struct Dev final
{
    void setIfa(std::string ifa)
    {
        ifa_ = ifa;
        ipset_ = false;
        mac_ = Mac::nullMac();
    }

    void setIp(Ip ip)
    {
        ip_ = ip;
        ipset_ = true;
    }

    void setMac(Mac mac) { mac_ = mac; }

    std::string ifa() const { return ifa_; }
    Ip myIp();
    Mac myMac();

protected:
    std::string ifa_;
    Ip ip_;
    bool ipset_;
    Mac mac_;
};
