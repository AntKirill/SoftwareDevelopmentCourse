cmake_minimum_required(VERSION 2.8)

project(LRUCacheCpp)

enable_language(CXX)
option(ENABLE_TESTING "Enables testing of project" OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if (ENABLE_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

add_library(LRUCache "LRUCache.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Only LRUCacheUsage appears in bin
add_executable(LRUCacheUsage "main.cpp")
target_link_libraries(LRUCacheUsage LRUCache)
