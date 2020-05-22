#specify the minimum version of cmake
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8 FATAL_ERROR)

#set PROJECT_NAME
PROJECT(WifiService)

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/utils)
#add include path
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)
include_directories(${CMAKE_SOURCE_DIR}/3rd/hicar_sdk/include)

#set common source
SET(SRC_COM ${CMAKE_CURRENT_SOURCE_DIR}/WifiService.c
            ${CMAKE_CURRENT_SOURCE_DIR}/WifiClient.c
            )

#set service source
SET(SRC_SRV ${CMAKE_CURRENT_SOURCE_DIR}/WifiServiceImpl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/WifiMonitor.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/WifiNative.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/wpa_ctrl.c
    )

SET(SRCCLIIMP ${CMAKE_CURRENT_SOURCE_DIR}/WifiClientImpl.cc)
SET(DRVWIFI ${CMAKE_CURRENT_SOURCE_DIR}/driver_adapter.c)

#set client source
SET(SRC_CLI ${CMAKE_CURRENT_SOURCE_DIR}/client.c
            ${CMAKE_CURRENT_SOURCE_DIR}/WifiClientImpl.cc
            )

SET(DEMOWIFI ${CMAKE_CURRENT_SOURCE_DIR}/demo_wifi.c)

SET(SOCKET ${CMAKE_CURRENT_SOURCE_DIR}/socket.c)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

# Look for pkgconfig using builtin CMake scripts
FIND_PACKAGE(PkgConfig)

# These dependencies require pkg-config to be found
PKG_CHECK_MODULES(GLIBMM REQUIRED glibmm-2.4)
PKG_CHECK_MODULES(GIOMM REQUIRED giomm-2.4)
PKG_CHECK_MODULES(GIOUNIX REQUIRED gio-unix-2.0)
PKG_CHECK_MODULES(SIGC REQUIRED sigc++-2.0 )

# Include giomm glibmm giounix
INCLUDE_DIRECTORIES(
    ${GLIBMM_INCLUDE_DIRS}
    ${GIOMM_INCLUDE_DIRS}
    ${GIOUNIX_INCLUDE_DIRS}
)

find_program(GDBUS_CODEGEN_EXECUTABLE NAMES gdbus-codegen DOC "gdbus-codegen executable")

if(NOT GDBUS_CODEGEN_EXECUTABLE)
    message(FATAL_ERROR "Executable gdbus-codegen not found")

endif(NOT GDBUS_CODEGEN_EXECUTABLE)

ADD_DEFINITIONS(-DCONFIG_CTRL_IFACE)
ADD_DEFINITIONS(-DCONFIG_CTRL_IFACE_UNIX)

#add executable WifiService
ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_SRV} ${SRC_COM} ${SRCCLIIMP})
#add executable client
ADD_EXECUTABLE(client ${SRC_CLI} ${SRC_COM} ${DRVWIFI} ${SOCKET})

ADD_LIBRARY(driverwifi  SHARED  ${SOCKET} ${DRVWIFI} ${SRC_COM})
TARGET_LINK_LIBRARIES(driverwifi
    ${GLIBMM_LDFLAGS}
    ${GIOMM_LDFLAGS}
    ${GIOUNIX_LDFLAGS}
    ${SIGC_LDFLAGS}
    )

## Add WifiService link library
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${GLIBMM_LDFLAGS}
    ${GIOMM_LDFLAGS}
    ${GIOUNIX_LDFLAGS}
    utils
)

## Add client link library
TARGET_LINK_LIBRARIES(client ${GLIBMM_LDFLAGS}
    ${GIOMM_LDFLAGS}
    ${GIOUNIX_LDFLAGS}
    pthread
)

#install file
INSTALL(TARGETS ${PROJECT_NAME} client RUNTIME DESTINATION "/usr/bin")
INSTALL(TARGETS driverwifi LIBRARY DESTINATION "/usr/lib")

ADD_EXECUTABLE(wifidemo ${DEMOWIFI})
TARGET_LINK_LIBRARIES(wifidemo driverwifi)
INSTALL(TARGETS ${PROJECT_NAME} wifidemo RUNTIME DESTINATION "/usr/bin")

