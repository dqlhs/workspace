# common
set(CMAKE_INCLUDE_CURRENT_DIR on)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE common_SOURCES "*.cpp")
file(GLOB_RECURSE common_HEADERS "*.h")
add_library(oxybase SHARED ${common_SOURCES} ${common_HEADERS})
target_link_libraries(oxybase dl)

install(TARGETS oxybase LIBRARY DESTINATION "/usr/lib")
