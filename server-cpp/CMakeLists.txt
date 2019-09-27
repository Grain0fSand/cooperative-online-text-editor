cmake_minimum_required(VERSION 3.14)
project(sqlite)

set(CMAKE_CXX_STANDARD 17)


set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS filesystem regex system thread system)

include_directories(${Boost_INCLUDE_DIRS})

set(SQLITE_DIR library)

add_subdirectory(${SQLITE_DIR}/SQLiteCpp)

include_directories(
        ${SQLITE_DIR}/SQLiteCpp/include
)

add_executable(main main.cpp sha256.cpp sha256.h)

target_link_libraries(main
        SQLiteCpp
        sqlite3
        pthread
        dl
        nlohmann_json::nlohmann_json
        ${Boost_LIBRARIES}
        )

if (UNIX AND NOT APPLE)
    include_directories(library/cget/include/nlohmann/json.hpp)
endif()
