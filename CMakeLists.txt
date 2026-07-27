cmake_minimum_required(VERSION 3.16)
project(DatabaseEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

option(BUILD_TESTING "Build tests" ON)

if(BUILD_TESTING)
    enable_testing()
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_library(database_storage
    src/storage/disk_manager.cpp
    src/storage/page.cpp
)

target_include_directories(database_storage
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(database_engine
    src/storage/disk_manager.cpp
)

target_link_libraries(database_engine
    PRIVATE
        database_storage
)

if(BUILD_TESTING)
    add_executable(storage_tests
        tests/storage/disk_manager_test.cpp
    )

    target_link_libraries(storage_tests
        PRIVATE
            database_storage
            GTest::gtest_main
    )

    target_include_directories(storage_tests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    add_test(NAME storage_tests COMMAND storage_tests)
endif()
