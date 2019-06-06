# At least this cmake version is needed
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# Define a project name for the SW package
PROJECT (YanLinDbusExample)

# version
SET(YanLinDbusExample_VERSION_MAJOR 1)
SET(YanLinDbusExample_VERSION_MINOR 0)

# XML file name
SET(XML_FILE_LIST com.Yanlin.Gdbus.Example.xml)
# generated file name
SET(XML_GEN_FILE_LIST Example-generated)

# check if the length of one of the above two lists is same as one of the other one
LIST(LENGTH XML_FILE_LIST XML_FILE_LIST_LEN)
LIST(LENGTH XML_GEN_FILE_LIST NAME_LIST_LEN)
MATH(EXPR XML_FILE_LIST_LEN "${XML_FILE_LIST_LEN}-1")
MATH(EXPR NAME_LIST_LEN "${NAME_LIST_LEN}-1")


#IF(${XML_FILE_LIST_LEN} EQUAL ${NAME_LIST_LEN})
# generate files from XML
#   FOREACH(INDEX RANGE 0 ${XML_FILE_LIST_LEN})
#       LIST(GET XML_FILE_LIST ${INDEX} XML)
#       LIST(GET XML_GEN_FILE_LIST ${INDEX} NAME_TMP)
#       EXECUTE_PROCESS(COMMAND gdbus-codegen --interface com.Yanlin. --c-namespace CYanlin --#generate-c-code ${PROJECT_BINARY_DIR}/${NAME_TMP} ${PROJECT_SOURCE_DIR}/${XML})
#    ENDFOREACH(INDEX)
#ELSE (${XML_FILE_LIST_LEN} EQUAL ${NAME_LIST_LEN})
#    MESSAGE(FATAL_ERROR "The length (${XML_FILE_LIST_LEN}) of xml files list does not equal to #the length(${NAME_LIST_LEN}) of the generated files name list")
#ENDIF(${XML_FILE_LIST_LEN} EQUAL ${NAME_LIST_LEN})

#EXECUTE_PROCESS(COMMAND gdbus-codegen --interface com.Yanlin. --c-namespace CYanlin --generate-c-code=$(XML_GEN_FILE_LIST) $(XML_FILE_LIST))


# The library has to be dynamically linked to the header
INCLUDE_DIRECTORIES(/usr/include/glib-2.0)
INCLUDE_DIRECTORIES(/usr/lib/glib-2.0/include)
INCLUDE_DIRECTORIES(/usr/include/gio-unix-2.0)
INCLUDE_DIRECTORIES(/usr/lib/x86_64-linux-gnu/glib-2.0/include/)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/)

link_directories(${PROJECT_BINARY_DIR}/)

set(SRCclient libClient.c Example-generated.c )
add_library(ExampleClientlib SHARED ${SRCclient})

#set(SRCclient libClient.c ${PROJECT_BINARY_DIR}/Example-generated.c )
#add_library(ExampleClientlib STATIC ${SRCclient})

set(SRCserver  libServer.c Example-generated.c)
add_library(ExampleServerlib STATIC ${SRCserver})

add_executable(SvrExample SvrExample.c)
target_link_libraries(SvrExample ExampleServerlib pthread gio-2.0 gobject-2.0 gmodule-2.0 gthread-2.0 rt glib-2.0)

add_executable(TCltExample CltExample.c)
target_link_libraries(TCltExample ExampleClientlib ExampleServerlib pthread gio-2.0 gobject-2.0 gmodule-2.0 gthread-2.0 rt glib-2.0)

