#include "disk_manager.h"

#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <system_error>
#include <cstring>
#include <sys/stat.h>

DiskManager::DiskManager(const std::string& dbName) : fd(-1), name(dbName) {}

void DiskManager::Open() {
    if (fd < 0) {
        fd = open(name.c_str(), O_RDWR | O_CREAT, 0600);
        if (fd < 0) {
            throw std::system_error(errno, std::generic_category(), "Failed to open " + name);
        }
        struct stat file_Info;
        int file_stats = fstat(fd, &file_Info);
        if(file_stats == 0 && static_cast<int>(file_Info.st_size) > 0){
            return ;
        }
        else if(file_stats < 0){
            throw std::system_error(errno, std::generic_category(), "Failed to get File Stats " + name);
        }
        Page0Header page0;
        page0.magic = kMagicNumber;
        page0.version = 1;
        page0.pageSize = PAGE_SIZE;
        page0.nextPage = 1;
        Page pg0;
        std::memcpy(pg0.getByte(), &page0, sizeof(page0));
        WritePage(0, pg0);
    }
}
ssize_t DiskManager::WritePage(uint32_t pageId,const Page& page) {
    if (fd < 0) {
        return -1;
    }
    const off_t offset = static_cast<off_t>(pageId * PAGE_SIZE);
    ssize_t written = pwrite(fd, page.getByte(), PAGE_SIZE, offset);
    if (written < 0) {
        throw std::system_error(errno, std::generic_category(), "Writes failed " + name);
    } 
    if (fsync(fd) < 0) {
        throw std::system_error(errno, std::generic_category(), "Failed to write to disk " + name);
    }
    return written;
}