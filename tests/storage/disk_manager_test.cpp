#include <chrono>
#include <cstring>
#include <filesystem>
#include <string>

#include <gtest/gtest.h>

#include "storage/disk_manager.h"

namespace {

std::string MakeTempDbPath() {
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    return (std::filesystem::temp_directory_path() / ("database_engine_test_" + std::to_string(stamp) + ".db")).string();
}

}

TEST(DiskManagerTest, InitializesPageZeroHeaderOnFirstOpen) {
    const std::string db_path = MakeTempDbPath();
    {
        DiskManager manager(db_path);
        manager.Open();
        Page page;
        manager.ReadPage(0, page);
        Page0Header header{};
        std::memcpy(&header, page.getByte(), sizeof(header));
        EXPECT_EQ(header.magic, 0x4D594442u);
        EXPECT_EQ(header.version, 1u);
        EXPECT_EQ(header.pageSize, PAGE_SIZE);
        EXPECT_EQ(header.nextPage, 1u);
        manager.Close();
    }

    EXPECT_TRUE(std::filesystem::exists(db_path));
    std::filesystem::remove(db_path);
}

TEST(DiskManagerTest, ValidateDbFile) {
    const std::string db_path = MakeTempDbPath();
    {
        DiskManager manager(db_path);
        manager.Open();
        bool flag=manager.validate();
        EXPECT_EQ(flag, true);
        manager.Close();
    }
    EXPECT_TRUE(std::filesystem::exists(db_path));
    std::filesystem::remove(db_path);
}