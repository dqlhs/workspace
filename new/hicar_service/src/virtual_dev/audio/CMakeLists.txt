cmake_minimum_required(VERSION 2.8)
project(HiCarAudioHAL C)

set(AUDIOHAL_VERSION_MAJOR 0)
set(AUDIOHAL_VERSION_MINOR 2)
set(AUDIOHAL_VERSION_PATCH 0)
set(AUDIO_VERSION_STR ${AUDIOHAL_VERSION_MAJOR}.${AUDIOHAL_VERSION_MINOR}.${AUDIOHAL_VERSION_PATCH})

set(CMAKE_MODULE_PATH $ENV{SDKTARGETSYSROOT})

option(ENABLE_AUDIO_CAPTURE
  "Capture in-out pcm"
  OFF)

if (ENABLE_AUDIO_CAPTURE)
  if (NOT DEFINED IN_PCM_PATH)
    set(IN_PCM_PATH "/home/root/in.pcm")
  endif ()

  if (NOT DEFINED OUT_PCM_PATH)
    set(OUT_PCM_PATH "/home/root/out.pcm")
  endif ()
else ()
  if (DEFINED IN_PCM_PATH)
    unset(IN_PCM_PATH)
  endif ()
  if (DEFINED OUT_PCM_PATH)
    unset(OUT_PCM_PATH)
  endif()
endif()


configure_file(${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_SOURCE_DIR}/include/config.h)


include(FindPkgConfig)
pkg_search_module(JSONC REQUIRED json-c>=0.11)
pkg_search_module(ALSA REQUIRED alsa)
pkg_search_module(GIO REQUIRED gio-2.0)

set(COMPDB ${PROJECT_SOURCE_DIR}/compile_commands.json)
set(SRCS src/HiCarAudioHal.c
         src/util.c
     ${CMAKE_SOURCE_DIR}/3rd/hicar_sdk/src/dmsdp_log.c)
include_directories("include"
  ${JSONC_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/3rd/hicar_sdk/include
  ${GIO_INCLUDE_DIRS})

add_library(dmsdpaudiohandler SHARED ${SRCS})
target_link_libraries(dmsdpaudiohandler ${JSONC_LIBRARIES} ${ALSA_LIBRARIES} ${GIO_LIBRARIES})

set_target_properties(dmsdpaudiohandler PROPERTIES VERSION ${AUDIO_VERSION_STR}
	SOVERSION ${AUDIOHAL_VERSION_MAJOR})

add_executable(dmsdpaudiohandler-test-capastr tests/TestGetAudioCapabilityString.c)
target_link_libraries(dmsdpaudiohandler-test-capastr dmsdpaudiohandler ${JSONC_LIBRARIES} ${GIO_LIBRARIES})

add_executable(dmsdpaudiohandler-test-play tests/TestPlayAudio.c)
target_link_libraries(dmsdpaudiohandler-test-play dmsdpaudiohandler ${JSONC_LIBRARIES} ${GIO_LIBRARIES})

add_executable(dmsdpaudiohandler-test-record tests/TestAudioRecord.c)
target_link_libraries(dmsdpaudiohandler-test-record dmsdpaudiohandler ${JSONC_LIBRARIES} ${GIO_LIBRARIES})

add_executable(dmsdpaudiohandler-test-underrun tests/TestUnderrun.c)
target_link_libraries(dmsdpaudiohandler-test-underrun dmsdpaudiohandler ${JSONC_LIBRARIES} ${GIO_LIBRARIES})

install(TARGETS dmsdpaudiohandler LIBRARY DESTINATION "/usr/lib")

if (DEFINED LOCAL_DEV)
add_custom_command(
  OUTPUT ${COMPDB}
  WORKING_DIR ${PROJECT_SOURCE_DIR}/build
  COMMAND compiledb -n make
  COMMAND cp compile_commands.json ${PROJECT_SOURCE_DIR})

add_custom_target(compdb ALL
  COMMAND echo "Generate compile db."
  DEPENDS ${COMPDB})
endif ()
