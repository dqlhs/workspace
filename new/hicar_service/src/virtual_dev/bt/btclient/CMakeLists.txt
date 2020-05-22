CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8 FATAL_ERROR)
#set PROJECT_NAME
project(hsaebt)
SET( CMAKE_VERBOSE_MAKEFILE on )
# set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_PREFIX_PATH ${SYSROOT_DIR}/usr/lib/cmake)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++0x ")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -D__GXX_EXPERIMENTAL_CXX0X__ ")
#add include path
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/dbus-1.0/)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/lib/dbus-1.0/include)
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/hicar_service/3rd/hicar_sdk/include)
include_directories(${CMAKE_SOURCE_DIR}/3rd/hicar_sdk/include)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/libxml2)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/glib-2.0)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/gio-unix-2.0)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/gstreamer-1.0)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/lib/gstreamer-1.0/include)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/lib/glib-2.0/include)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/c++/4.9.2)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/c++/4.9.2/include)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/c++/4.9.2/backward)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/c++/4.9.2/arm-linux-gnueabihf)

#uncomment & modify this line on the basis of your project.
#enumrate the source file under src folder

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/src/ SRC_SERVER_LIST)


#add static library target
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRC_SERVER_LIST})

#link the libhello.so which is under "/usr/lib" against the

TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread dbus-1 glib-2.0 gobject-2.0 gio-2.0 )
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "/usr/lib")
