if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "DO NOT BUILD in-tree.")
endif()

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(TSrim)

# Default installation dir
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      /usr/local
      CACHE PATH "..." FORCE)
endif()

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-as-needed")
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -g")

# -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ROOT 6 installation
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

# Locate the ROOT package and defines a number of variables (e.g.
# ROOT_INCLUDE_DIRS)
find_package(
  ROOT REQUIRED
  COMPONENTS RIO
             Net
             Physics
             Minuit
             Minuit2
             Geom
             Gui
             Genetic)

# Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# external link directories
link_directories(${ROOTSYS}/lib)

# ====================== MAIN =======================
# make library
set(LIBNAME Srim)
# source files for the library
set(SOURCES TSrim.cc Mass.cc)

# header files
set(HEADERS TSrim.h Mass.h)

# library setting (TSrim)
add_library(${LIBNAME} SHARED ${SOURCES})

target_include_directories(
  ${LIBNAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                    $<INSTALL_INTERFACE:include>)

target_link_libraries(${LIBNAME} ${ROOT_LIBRARIES})

root_generate_dictionary(G__${LIBNAME} ${HEADERS} LINKDEF LinkDef.h MODULE
                         ${LIBNAME})

set_target_properties(${LIBNAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(
  TARGETS ${LIBNAME}
  EXPORT ${PROJECT_NAME}Targets
  INCLUDES DESTINATION include
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include
  LIBRARY DESTINATION lib)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${LIBNAME}_rdict.pcm
        DESTINATION lib)

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/cmake/${PROJECT_NAME})

configure_file(cmake/${PROJECT_NAME}Config.cmake.in ${PROJECT_NAME}Config.cmake
               @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION lib/cmake/${PROJECT_NAME})

include(CMakePackageConfigHelpers)
# ============================================================

# make uninstall
add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
  TARGET "uninstall"
  POST_BUILD
  COMMENT "Uninstall files with install_manifest.txt"
  COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
          install_manifest.txt to be uninstalled!)
