#pragma once

#include <cstdint>
#include <string>
#include "page.h"

struct Page0Header {
    uint32_t magic;
    uint32_t version;
    uint32_t pageSize;
};


class DiskManager {
public:
    explicit DiskManager(const std::string& dbName = "database.db");

    void Open();

private:
    static constexpr uint32_t kMagicNumber = 0x4D594442;
    int fd;
    std::string name;
};
