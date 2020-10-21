/*
 * Wazuh SysInfo
 * Copyright (C) 2015-2020, Wazuh Inc.
 * October 7, 2020.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */
#include "sysInfo.hpp"
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include "stringHelper.h"
#include "cmdHelper.h"

constexpr auto WM_SYS_HW_DIR{"/sys/class/dmi/id/board_serial"};
constexpr auto WM_SYS_CPU_DIR{"/proc/cpuinfo"};
constexpr auto WM_SYS_MEM_DIR{"/proc/meminfo"};
#define DPKG_PATH "/var/lib/dpkg/"

static bool existDir(const std::string& path)
{
    struct stat info{};
    return !stat(path.c_str(), &info) && (info.st_mode & S_IFDIR);
}

static void parseLineAndFillMap(const std::string& line, const std::string& separator, std::map<std::string, std::string>& systemInfo)
{
    const auto pos{line.find(separator)};
    if (pos != std::string::npos)
    {
        const auto key{Utils::trim(line.substr(0, pos), " \t\"")};
        const auto value{Utils::trim(line.substr(pos + 1), " \t\"")};
        systemInfo[key] = value;
    }
}

static bool getSystemInfo(const std::string& fileName, const std::string& separator, std::map<std::string, std::string>& systemInfo)
{
    std::string info;
    std::fstream file{fileName, std::ios_base::in};
    const bool ret{file.is_open()};
    if (ret)
    {
        std::string line;
        while(file.good())
        {
            std::getline(file, line);
            parseLineAndFillMap(line, separator, systemInfo);
        }
    }
    return ret;
}

static nlohmann::json parsePackage(const std::vector<std::string>& entries)
{
    std::map<std::string, std::string> info;
    nlohmann::json ret;
    for (const auto& entry: entries)
    {
        const auto pos{entry.find(":")};
        if (pos != std::string::npos)
        {
            const auto key{Utils::trim(entry.substr(0, pos))};
            const auto value{Utils::trim(entry.substr(pos + 1), " \n")};
            info[key] = value;
        }
    }
    if (!info.empty() && info.at("Status") == "install ok installed")
    {
        ret["name"] = info.at("Package");
        auto it{info.find("Priority")};
        if (it != info.end())
        {
            ret["priority"] = it->second;
        }
        it = info.find("Section");
        if (it != info.end())
        {
            ret["group"] = it->second;
        }
        it = info.find("Installed-Size");
        if (it != info.end())
        {
            ret["size"] = it->second;
        }
        it = info.find("Multi-Arch");
        if (it != info.end())
        {
            ret["multi-arch"] = it->second;
        }
        it = info.find("Architecture");
        if (it != info.end())
        {
            ret["architecture"] = it->second;
        }
        it = info.find("Source");
        if (it != info.end())
        {
            ret["source"] = it->second;
        }
        it = info.find("Version");
        if (it != info.end())
        {
            ret["version"] = it->second;
        }
    }
    return ret;
}

static nlohmann::json getDpkgInfo(const std::string& fileName)
{
    nlohmann::json ret;
    std::fstream file{fileName, std::ios_base::in};
    if (file.is_open())
    {
        while(file.good())
        {
            std::string line;
            std::vector<std::string> data;
            do
            {
                std::getline(file, line);
                if(line.front() == ' ')//additional info
                {
                    data.back() = data.back() + line + "\n";
                }
                else
                {
                    data.push_back(line + "\n");
                }
            }
            while(!line.empty());//end of package item info
            const auto packageInfo{ parsePackage(data) };
            if (!packageInfo[0].empty())
            {
                ret.push_back(packageInfo[0]);
            }
        }
    }
    return ret;
}

static nlohmann::json parseRpm(const std::string& packageInfo)
{
    nlohmann::json ret;
    std::string token;
    std::istringstream tokenStream{ packageInfo };
    std::map<std::string, std::string> info;
    while (std::getline(tokenStream, token))
    {
        const auto pos{token.find(":")};
        if (pos != std::string::npos && (pos + 1) < token.size())
        {
            const auto key{Utils::trim(token.substr(0, pos))};
            const auto value{Utils::trim(token.substr(pos + 1))};
            info[key] = value;
        }
    }
    auto it{info.find("Name")};
    if (it != info.end() && it->second != "gpg-pubkey")
    {
        std::string version;
        ret["name"] = it->second;
        it = info.find("Size");
        if (it != info.end())
        {
            ret["size"] = it->second;
        }
        it = info.find("Install Date");
        if (it != info.end())
        {
            ret["install_time"] = it->second;
        }
        it = info.find("Group");
        if (it != info.end())
        {
            ret["group"] = it->second;
        }
        it = info.find("Epoch");
        if (it != info.end())
        {
            version += it->second + "-";
        }
        it = info.find("Release");
        if (it != info.end())
        {
            version += it->second + "-";
        }
        it = info.find("Version");
        if (it != info.end())
        {
            version += it->second;
        }
        ret["version"] = version;
    }
    return ret;
}

static nlohmann::json getRpmInfo()
{
    nlohmann::json ret;
    auto rawData{ Utils::exec("rpm -qai") };
    if (!rawData.empty())
    {
        std::vector<std::string> packages;
        auto pos{rawData.rfind("Name")};
        while(pos != std::string::npos)
        {
            const auto package{parseRpm(rawData.substr(pos))};
            if (!package[0].empty())
            {
                ret.push_back(package[0]);
            }
            rawData = rawData.substr(0, pos);
            pos = rawData.rfind("Name");
        }
    }
    else
    {
    }
    return ret;
}

std::string SysInfo::getSerialNumber()
{
    std::string serial;
    std::fstream file{WM_SYS_HW_DIR, std::ios_base::in};
    if (file.is_open())
    {
        file >> serial;
    }
    else
    {
        serial = "unknown";
    }
    return serial;
}

std::string SysInfo::getCpuName()
{
    std::map<std::string, std::string> systemInfo;
    getSystemInfo(WM_SYS_CPU_DIR, ":", systemInfo);
    return systemInfo.at("model name");
}

int SysInfo::getCpuCores()
{
    std::map<std::string, std::string> systemInfo;
    getSystemInfo(WM_SYS_CPU_DIR, ":", systemInfo);
    return (std::stoi(systemInfo.at("processor")) + 1);
}

int SysInfo::getCpuMHz()
{
    std::map<std::string, std::string> systemInfo;
    getSystemInfo(WM_SYS_CPU_DIR, ":", systemInfo);
    return (std::stoi(systemInfo.at("cpu MHz")));
}

void SysInfo::getMemory(nlohmann::json& info)
{
    std::map<std::string, std::string> systemInfo;
    getSystemInfo(WM_SYS_MEM_DIR, ":", systemInfo);
    const auto memTotal{std::stoi(systemInfo.at("MemTotal"))};
    const auto memFree{std::stoi(systemInfo.at("MemFree"))};
    info["ram_total"] = memTotal;
    info["ram_free"] = memFree;
    info["ram_usage"] = 100 - (100*memFree/memTotal);
}

nlohmann::json SysInfo::getPackages()
{
    nlohmann::json packages;
    if (existDir(DPKG_PATH))
    {
        packages.push_back(getDpkgInfo(DPKG_PATH "status"));
    }
    else
    {
        packages.push_back(getRpmInfo());
    }
    return packages[0];
}