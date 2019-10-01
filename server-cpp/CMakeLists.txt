cmake_minimum_required(VERSION 3.14)
project(sqlite)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(SQLITE_DIR library)
set(nlohmann_DIR library/nlohmann_json)

find_package(Boost 1.45.0 COMPONENTS filesystem regex system thread system)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${nlohmann_DIR})
include_directories(
        ${SQLITE_DIR}/SQLiteCpp/include
)

add_subdirectory(${SQLITE_DIR}/SQLiteCpp)

add_executable(
        main
        main.cpp
        sha256.cpp
        sha256.h
        exchangeable_data.cpp
        exchangeable_data.h
        database.cpp
        database.h)

target_link_libraries(
        main
        SQLiteCpp
        sqlite3
        pthread
        dl
        ${Boost_LIBRARIES}
)
