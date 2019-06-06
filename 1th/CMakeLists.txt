CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


PROJECT (test)

#INCLUDE_DIRECTORIES(/usr/include/glib-2.0/)


find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)

include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})

#target_include_directories(mytarget PRIVATE ${GLIB_INCLUDE_DIRS})

#target_link_libraries(mytarget INTERFACE ${GLIB_LDFLAGS})



add_executable(server gdbus_server.c Building.c)
#add_library(serverlib STATIC ${server})
add_definitions(${GLIB_CFLAGS_OTHER})
target_link_libraries(server ${GLIB_LIBRARIES})



add_executable(client gdbus_client.c Building.c)
#add_library(clientlib STATIC ${client})
add_definitions(${GLIB_CFLAGS_OTHER})
target_link_libraries(client ${GLIB_LIBRARIES})




