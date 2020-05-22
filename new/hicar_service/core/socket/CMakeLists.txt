include_directories(${CMAKE_SOURCE_DIR}/3rd/asio/asio/include)

file(GLOB_RECURSE socket_SOURCES "*.cpp")
file(GLOB_RECURSE socket_HEADERS "*.h")

add_library(socket SHARED ${socket_SOURCES} ${socket_HEADERS})

install(TARGETS socket LIBRARY DESTINATION "/usr/lib")
