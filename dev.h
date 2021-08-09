#pragma once

#include <string>
#include "ip.h"

struct Dev final
{
    Dev(const std::string r) : dev_(r) {}

    explicit operator std::string() const { return dev_; }

    Ip getIp() const;

protected:
    std::string dev_;
};
