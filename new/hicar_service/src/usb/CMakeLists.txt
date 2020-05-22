if (x86)
    message(STATUS "-> x86 Machine")
else()
    message(STATUS "-> ARM Machine")
    set(MDNS_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/3rd/rootfs/usr/include/avahi-compat-libdns_sd")
    set(MDNS_LIBRARY_DIRS "${CMAKE_SOURCE_DIR}/3rd/rootfs/usr/lib")
endif()

include(FindPkgConfig)
pkg_search_module(LIBUSB REQUIRED libusb-1.0)
pkg_search_module(OPENSSL REQUIRED openssl)

include_directories(
    ${LIBUSB_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIRS}
    ${MDNS_INCLUDE_DIRS})

link_directories(
    ${LIBUSB_LIBRARY_DIRS}
    ${OPENSSL_LIBRARY_DIRS}
    ${MDNS_LIBRARY_DIRS})



include_directories(${CMAKE_SOURCE_DIR}/core/socket)
include_directories(${CMAKE_SOURCE_DIR}/core/oxygen/common)
include_directories(${CMAKE_SOURCE_DIR}/src/unique_config)

include_directories(${CMAKE_SOURCE_DIR}/3rd/hicar_sdk/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB usb_sources "*.cpp")
file(GLOB usb_headers "*.h")

add_library(usbplugin SHARED ${usb_sources} ${usb_headers})
target_link_libraries(
    usbplugin
    ${LIBUSB_LIBRARIES} ${OPENSSL_LIBRARIES} ${MDNS_LIBRARIES}
    oxybase pthread socket dns_sd unique_config)


install(
    TARGETS usbplugin
    LIBRARY
    DESTINATION "/usr/lib"
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_WRITE GROUP_EXECUTE
                WORLD_READ WORLD_WRITE WORLD_EXECUTE)
