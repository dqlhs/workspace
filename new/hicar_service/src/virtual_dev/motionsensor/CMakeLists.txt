include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/unique_config)

file(GLOB motionsensor_sources "*.cpp")
file(GLOB motionsensor_headers "*.h")

add_library(dmsdpgpshandler SHARED ${motionsensor_sources} ${motionsensor_headers})
target_link_libraries(dmsdpgpshandler oxybase)

install(
    TARGETS dmsdpgpshandler
    LIBRARY
    DESTINATION "/usr/lib"
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_WRITE GROUP_EXECUTE
                WORLD_READ WORLD_WRITE WORLD_EXECUTE)
