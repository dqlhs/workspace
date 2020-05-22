cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
#PROJECT(Hicar)

set(CMAKE_VERBOSE_MAKEFILE on)

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/src HICARSTREAM)
    
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/usr/include
			${SYSROOT_DIR}/usr/include/gstreamer-1.0
			${SYSROOT_DIR}/usr/lib/gstreamer-1.0/include
                    ${SYSROOT_DIR}/usr/include/glib-2.0
                    ${SYSROOT_DIR}/usr/lib/glib-2.0/include
                    ${SYSROOT_DIR}/usr/include/libxml2)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/3rd/hicar_sdk/include)



#LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

ADD_LIBRARY(HicarStream SHARED ${HICARSTREAM})
TARGET_LINK_LIBRARIES(HicarStream pthread gstreamer-1.0 gobject-2.0 glib-2.0 ffi gthread-2.0 gmodule-2.0 xml2 z)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

INSTALL(TARGETS HicarStream LIBRARY DESTINATION /usr/lib)
#INSTALL(TARGETS HicarStream RUNTIME DESTINATION /usr/bin)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/HicarStream.h
		DESTINATION /usr/include)



