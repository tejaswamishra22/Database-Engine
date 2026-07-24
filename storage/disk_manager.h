#pragma once

#include <cstdint>
#include <string>
#include "page.h"

struct Page0Header {
    uint32_t magic;
    uint32_t version;
    uint32_t pageSize;
    uint32_t nextPage;
};

class DiskManager {
public:
    explicit DiskManager(const std::string& dbName = "database.db");

    void Open();
    ssize_t WritePage(uint32_t,const Page&);

private:
    static constexpr uint32_t kMagicNumber = 0x4D594442;
    int fd;
    std::string name;
};
