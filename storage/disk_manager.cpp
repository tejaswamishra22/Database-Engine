#include "disk_manager.h"

#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <system_error>

DiskManager::DiskManager(const std::string& dbName) : fd(-1), name(dbName) {}

void DiskManager::Open() {
    if (fd < 0) {
        fd = open(name.c_str(), O_RDWR | O_CREAT, 00700);
        if (fd < 0) {
            throw std::system_error(errno, std::generic_category(), "Failed to open " + std::string(name));
        }
        Page pg0;
        (void)pg0;
    }
}