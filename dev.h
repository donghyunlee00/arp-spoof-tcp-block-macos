#pragma once

#include <string>
#include "ip.h"
#include "mac.h"

struct Dev final
{
    Dev(const std::string r) : dev_(r) {}

    explicit operator std::string() const { return dev_; }

    Ip getIp() const;
    Mac getMac() const;

protected:
    std::string dev_;
};
