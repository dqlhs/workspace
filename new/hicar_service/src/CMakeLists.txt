# add rootfs
if (x86)

else()
    include_directories(${PROJECT_SOURCE_DIR}/3rd/rootfs/usr/include)
    link_directories(${PROJECT_SOURCE_DIR}/3rd/rootfs/usr/lib)
endif()
set(LINKSOURCE ${CMAKE_CURRENT_SOURCE_DIR}/hicarlink.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/hicarlinkImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RrConfig.cpp)

include_directories(${CMAKE_SOURCE_DIR}/3rd/)
include_directories(${CMAKE_SOURCE_DIR}/3rd/hicar_sdk/include)

# add ble
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/virtual_dev/bt/btclient)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/virtual_dev/bt/btserver)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/virtual_dev/bt/demo)

# add video
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/virtual_dev/video/HicarStream)

# add wi-fi
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/virtual_dev/wifi/WiFi_Services)

# add camera
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/virtual_dev/camera/hicarCap)

# add usb
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/usb)

# add unique_config
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/unique_config)

# add motionsensor
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/virtual_dev/motionsensor)

# add asio
include_directories(${CMAKE_SOURCE_DIR}/3rd/asio/asio/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/virtual_dev/video/HicarStream/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/virtual_dev/bt/btclient/include)
include_directories(${CMAKE_SOURCE_DIR}/src/unique_config)
INCLUDE_DIRECTORIES(
    ${SYSROOT_DIR}/usr/include
    ${SYSROOT_DIR}/usr/include/gstreamer-1.0
    ${SYSROOT_DIR}/usr/lib/gstreamer-1.0/include
    ${SYSROOT_DIR}/usr/include/glib-2.0
    ${SYSROOT_DIR}/usr/lib/glib-2.0/include
    ${SYSROOT_DIR}/usr/include/libxml2)
include_directories(${CMAKE_SOURCE_DIR}/3rd/hicar_sdk/include)
link_directories(${CMAKE_SOURCE_DIR}/3rd/hicar_sdk/lib)
file(GLOB main_sources "*.cpp")
file(GLOB main_headers "*.h")

add_custom_target(hicarfiles SOURCES ${main_sources} ${main_headers})
add_library(hicarlink SHARED ${LINKSOURCE})
target_link_libraries(hicarlink unique_config oxybase crypto pthread hicar driverwifi hsaebt socket usbplugin
    authagent dmsdp dmsdpcrypto dmsdpdvaudio dmsdpdvcamera dmsdpdvdevice dmsdpdvinterface
    dmsdphisight dmsdpplatform dmsdpsec HisightSink HwDeviceAuthSDK HwKeystoreSDK nearby
    HicarStream gstreamer-1.0 gobject-2.0 glib-2.0 ffi gthread-2.0 gmodule-2.0 xml2 z ssl crypto)
install(TARGETS hicarlink LIBRARY DESTINATION "/usr/lib")

add_subdirectory(virtual_dev/audio)
