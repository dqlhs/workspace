
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB unique_config_sources "*.cpp")
file(GLOB unique_config_headers "*.h")

add_library(unique_config SHARED ${unique_config_sources} ${unique_config_headers})
target_link_libraries(unique_config oxybase)

install(
    TARGETS unique_config
    LIBRARY
    DESTINATION "/usr/lib"
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_WRITE GROUP_EXECUTE
                WORLD_READ WORLD_WRITE WORLD_EXECUTE)
