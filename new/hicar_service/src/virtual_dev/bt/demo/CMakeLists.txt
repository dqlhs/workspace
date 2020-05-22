CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8 FATAL_ERROR)

project(demo_hicar)

FIND_PACKAGE(PkgConfig)

# These dependencies require pkg-config to be found
PKG_CHECK_MODULES(GLIBMM REQUIRED glibmm-2.4)
PKG_CHECK_MODULES(GIOMM REQUIRED giomm-2.4)
PKG_CHECK_MODULES(GIOUNIX REQUIRED gio-unix-2.0)



INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/dbus-1.0/)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/lib/dbus-1.0/include)

INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/libxml2)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/glib-2.0)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/gio-unix-2.0)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/gstreamer-1.0)
 
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/lib/glib-2.0/include)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/c++/4.9.2)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/c++/4.9.2/include)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/c++/4.9.2/backward)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include/c++/4.9.2/arm-linux-gnueabihf)

#LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/)
LINK_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/")
add_executable(${PROJECT_NAME} "main.cpp")
TARGET_LINK_LIBRARIES(demo_hicar hsaebt driverwifi 
        pthread dbus-1
        glib-2.0
        gobject-2.0
        gio-2.0
        ${GLIBMM_LDFLAGS}
        ${GIOMM_LDFLAGS}
        ${GIOUNIX_LDFLAGS}
        )
        
INSTALL(TARGETS demo_hicar RUNTIME DESTINATION "/usr/bin/bluetooth")

