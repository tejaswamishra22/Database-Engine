#pragma once

#include <string>
#include "page.h"

class DiskManager {
public:
    explicit DiskManager(const std::string& dbName = "database.db");

    void Open();

private:
    int fd;
    std::string name;
};
