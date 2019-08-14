cmake_minimum_required(VERSION 3.14)
project(server_cpp)

# all the properly compiler flag
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -enable=all")


# explaination per flag
#-Wall -Wextra : reasonable and standard
#-Wshadow : warn the user if a variable declaration shadows another with the same name in the same scope
#-Wnon-virtual-dtor : warn the user if a class with virtual functions has a non-virtual destructor. This can lead to hard to track down memory errors
#-pedantic : warn about non-portable code, C++ that uses language extensions.
# -enable=all avaiable only if Cppcheck is installed

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_STANDARD 17)


set(-DBSONCXX_POLY_USE_MNMLSTC=1)
set(-DBSONCXX_POLY_USE_BOOST=1)

include_directories(library)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS filesystem regex system thread system)

find_package(libmongoc-1.0 1.7 REQUIRED)
find_package(libmongocxx REQUIRED)

message ("--   mongoc found version \"${MONGOC_VERSION}\"")
message ("--   mongoc include path \"${MONGOC_INCLUDE_DIRS}\"")
message ("--   mongoc libraries \"${MONGOC_LIBRARIES}\"")

include_directories(${MONGOC_INCLUDE_DIRS})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(server_cpp main.cpp library/crow_all_linux.h)
    target_link_libraries(server_cpp PUBLIC ${Boost_LIBRARIES})
endif()

target_compile_definitions (server_cpp PRIVATE "${MONGOC_DEFINITIONS}")

target_link_libraries(server_cpp PUBLIC ${MONGOC_LIBRARIES})



target_include_directories(server_cpp
        PRIVATE ${LIBMONGOCXX_INCLUDE_DIRS}
        )

target_link_libraries(server_cpp
        PRIVATE ${LIBMONGOCXX_LIBRARIES}
        )

target_compile_definitions(server_cpp
        PRIVATE ${LIBMONGOCXX_DEFINITIONS}
        )

add_custom_target(run
        COMMAND hello_mongocxx
        DEPENDS hello_mongocxx
        WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
        )

# Sanity-check that static library macros are not set when building against the shared library.
# Users don't need to include this section in their projects.
list(FIND LIBMONGOCXX_DEFINITIONS "BSONCXX_STATIC" LIST_IDX)
if (${LIST_IDX} GREATER -1)
    message(FATAL_ERROR "Expected BSONCXX_STATIC to not be defined")
endif()
list(FIND LIBMONGOCXX_DEFINITIONS "MONGOCXX_STATIC" LIST_IDX)
if (${LIST_IDX} GREATER -1)
    message(FATAL_ERROR "Expected MONGOCXX_STATIC to not be defined")
endif()

message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")

# Note that CMAKE_CXX_FLAGS is applied to both compiler and linker, while add_definitions is applied only to compiler.
