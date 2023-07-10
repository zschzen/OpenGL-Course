#ifndef __GUID_H__
#define __GUID_H__

// https://lowrey.me/guid-generation-in-c-11/

#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <string>

unsigned char RandomByte()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    return static_cast<unsigned char>(dis(gen));
}

std::string RandomGUID(const unsigned int len = 32)
{
    std::stringstream ss;
    for (auto i = 0; i < len; i++)
    {
        auto rc = RandomByte();
        std::stringstream hexstream;
        hexstream << std::hex << int(rc);
        auto hex = hexstream.str();
        ss << (hex.length() < 2 ? '0' + hex : hex);
    }
    return ss.str();
}

#endif // __GUID_H__