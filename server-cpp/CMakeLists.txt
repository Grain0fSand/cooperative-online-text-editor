cmake_minimum_required(VERSION 3.14)
project(sqlite)

set(CMAKE_CXX_STANDARD 17)


set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(SQLITE_DIR library)


find_package(Boost 1.45.0 COMPONENTS filesystem regex system thread system)

if (APPLE)
    find_package(nlohmann_json 3.2.0 REQUIRED)
endif()

if(WIN32)
    set(nlohmann_DIR library/nlohmann_json)
    include_directories(${nlohmann_DIR})
endif()

include_directories(${Boost_INCLUDE_DIRS})

include_directories(
        ${SQLITE_DIR}/SQLiteCpp/include
)

add_subdirectory(${SQLITE_DIR}/SQLiteCpp)

add_executable(main main.cpp sha256.cpp sha256.h)

target_link_libraries(main
        SQLiteCpp
        sqlite3
        pthread
        dl
        nlohmann_json::nlohmann_json
        ${Boost_LIBRARIES}
        )