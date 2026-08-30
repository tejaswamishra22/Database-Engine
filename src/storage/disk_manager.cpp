#include "disk_manager.h"

#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <system_error>
#include <cstring>
#include <sys/stat.h>

DiskManager::DiskManager(const std::string& dbname_) : fd(-1), name_(dbname_) {}


void DiskManager::Open() {
        if( IsOpen() ) return ;
        fd = open(name_.c_str(), O_RDWR | O_CREAT, 0600);
        if (fd < 0) {
            throw std::system_error(errno, std::generic_category(), "Failed to open " + name_);
        }    
        struct stat file_Info;
        int file_stats = fstat(fd, &file_Info);
        if(file_stats == 0){
            LoadHeader();
            if(file_Info.st_size > 0){
                if(!validate())throw std::system_error(errno, std::generic_category(), "Couldn't read page " + name_);;            
                return ;
            }
            else{
                header_.magic = kMagicNumber;
                header_.version = 1;
                header_.pageSize = PAGE_SIZE;
                header_.nextPage = 1;
                header_.freePageCount = 0;  
                WriteHeader();              
            }    
        }
        else if(file_stats < 0){
            throw std::system_error(errno, std::generic_category(), "Failed to get File Stats " + name_);
        }
        Page pg0;
        std::memcpy(pg0.getByte(), &header_, sizeof(header_));
        WritePage(0, pg0);
}

void DiskManager::Close(){
    if(!IsOpen())return ;
    if(close(fd)<0){
        throw std::system_error(errno, std::generic_category(), "Could not close File Descriptor " + fd);
    }
    fd=-1;
    return ;
}

ssize_t DiskManager::WritePage(PageId pageId,const Page& page) {
    if (!IsOpen()) {
        throw std::system_error(errno, std::generic_category(), "File Descriptor not open " + name_);
    }
    const off_t offset = static_cast<off_t>(pageId * PAGE_SIZE);
    ssize_t written = pwrite(fd, page.getByte(), PAGE_SIZE, offset);
    if (written < 0) {
        throw std::system_error(errno, std::generic_category(), "Writes failed " + name_);
    } 
    if(written < PAGE_SIZE){
        throw std::system_error(errno, std::generic_category(), "Write partially successful " + name_);
    }    
    if (fsync(fd) < 0) {
        throw std::system_error(errno, std::generic_category(), "Failed to write to disk " + name_);
    }
    return written;
}

ssize_t DiskManager::ReadPage(PageId pageId,Page& page){
   if (!IsOpen()) {
        throw std::system_error(errno, std::generic_category(), "File Descriptor not open " + name_);
    }
    const off_t offset = static_cast<off_t>(pageId * PAGE_SIZE); //pageId * PAGE_SIZE is performed before the cast. Can be a problem

    ssize_t ReadData = pread(fd, page.getByte(), PAGE_SIZE, offset);

    if (ReadData < 0) {
        throw std::system_error(errno, std::generic_category(), "Writes failed " + name_);
    } 
    if(ReadData < PAGE_SIZE){
        throw std::system_error(errno, std::generic_category(), "Read partially successful " + name_);
    }    
    return ReadData;
}

bool DiskManager::validate(){
    return header_.magic==kMagicNumber;
}

bool DiskManager::IsOpen(){
    return fd >= 0;
}

void DiskManager::LoadHeader(){
    if(!IsOpen())throw std::system_error(errno, std::generic_category(), "Datavase not open " + name_);;
    Page pg0;
    ssize_t readBytes = ReadPage(0, pg0);
    if (readBytes < static_cast<ssize_t>(sizeof(Page0Header))) {
        throw std::system_error(errno, std::generic_category(), "Couldn't read page " + name_);
    }
    std::memcpy(&header_, pg0.getByte(), sizeof(header_));
    return ;
}

void DiskManager::WriteHeader() {
    if (!IsOpen()) {
        throw std::system_error(errno, std::generic_category(),"Database not open: " + name_);
    }
    Page pg0;
    std::memcpy(pg0.getByte(), &header_, sizeof(header_));
    WritePage(0, pg0);
}

PageId DiskManager::AllocatePage(){
    LoadFreePageList();
}

void DiskManager::LoadFreePageList(){

}
