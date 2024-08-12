if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "DO NOT BUILD in-tree.")
endif()

# CMake version required
if(APPLE)
  cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
else()
  cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
endif()

# TSrim project setting
project(TSrim VERSION 0.1.0 LANGUAGES CXX)

# Default installation dir
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      /usr/local
      CACHE PATH "..." FORCE)
endif()

# require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ROOT settings
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

add_subdirectory(src_mass)
add_subdirectory(src_tsrim)

# find_package TSrim
configure_file(templates/${PROJECT_NAME}Config.cmake.in
               ${PROJECT_NAME}Config.cmake @ONLY)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "templates/${PROJECT_NAME}Config.cmake.in" "${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION lib/cmake/${PROJECT_NAME})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION lib/cmake/${PROJECT_NAME})

configure_file(templates/this${PROJECT_NAME}.sh.in this${PROJECT_NAME}.sh @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/this${PROJECT_NAME}.sh
        DESTINATION bin)

# python setting reference: nanobind
# https://nanobind.readthedocs.io/en/latest/building.html
if(CMAKE_VERSION VERSION_LESS 3.18)
  set(DEV_MODULE Development)
else()
  set(DEV_MODULE Development.Module)
endif()

find_package(Python 3.8 COMPONENTS Interpreter ${DEV_MODULE})
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()

if(Python_FOUND)
  # Detect the installed nanobind package and import it into CMake
  execute_process(
    COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE nanobind_ROOT)
  find_package(nanobind CONFIG)
  if(nanobind_FOUND)
    message(STATUS "build pyTSrim")
    add_subdirectory(pybind)
  else()
    message(STATUS "no pyTSrim build")
  endif()
else()
  message(STATUS "no pyTSrim build")
endif()

# make uninstall
add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
  TARGET "uninstall"
  POST_BUILD
  COMMENT "Uninstall files with install_manifest.txt"
  COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
          install_manifest.txt to be uninstalled!)
