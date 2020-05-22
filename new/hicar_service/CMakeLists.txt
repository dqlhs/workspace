cmake_minimum_required(VERSION 2.8)

project(hicar_service)

set(CMAKE_CXX_STANDARD 14)

option(x86 "This is a option for x86 machine" OFF)

#add_compile_definitions(ASIO_STANDALONE)
#add_compile_definitions(ASIO_HAS_MOVE)

#set(CMAKE_CXX_FLAGS "-std=c++14 -D ASIO_STANDALONE -D ASIO_HAS_MOVE")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -D ASIO_STANDALONE -D ASIO_HAS_MOVE ")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/bin)

# add asio
include_directories(${CMAKE_SOURCE_DIR}/3rd/asio/asio/include)

# include hsae core lib
include_directories(core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core/oxygen/common)
add_subdirectory(core)

add_subdirectory(src)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/config/
    DESTINATION "/"
    USE_SOURCE_PERMISSIONS)
#install(DIRECTORY ${PROJECT_SOURCE_DIR}/thrid_party/rootfs/ DESTINATION "/")

install(DIRECTORY ${PROJECT_SOURCE_DIR}/3rd/hicar_sdk/lib/ DESTINATION "/usr/lib")

# install debug script
install(
    FILES
        ${PROJECT_SOURCE_DIR}/script/inmachine/clear_hicar_data.sh
        ${PROJECT_SOURCE_DIR}/script/inmachine/runenv
        ${PROJECT_SOURCE_DIR}/script/inmachine/set_bt_mac
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_WRITE GROUP_EXECUTE
                WORLD_READ WORLD_WRITE WORLD_EXECUTE
    DESTINATION "/usr/bin")
