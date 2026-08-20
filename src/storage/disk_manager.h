#include <cstdint>
#include <string>
#include <vector>
#include "page.h"

using PageId = uint64_t;

struct Page0Header {
    uint32_t magic;
    uint32_t version;
    uint32_t pageSize;
    PageId nextPage;
    uint32_t freePages;
};

class DiskManager {
public:
    explicit DiskManager(const std::string& dbName = "database.db");

    void Open();
    void Close();

    ssize_t WritePage(uint32_t,const Page&);
    ssize_t ReadPage(uint32_t, Page&);

    PageId AllocatePage();
    void DeallocatePage(PageId id);    

private:
    static constexpr uint32_t kMagicNumber = 0x4D594442;
    int fd;
    bool validate();
    bool fileDescClose();
    void LoadHeader();
    void WriteHeader();
    void LoadFreePageList();
    void PersistFreePageList();    
    std::string name_;
    std::vector<PageId> free_pages_;
    Page0Header header_;
};
