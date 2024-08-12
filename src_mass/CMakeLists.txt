# Mass library
set(MassLibrary Mass)

add_library(${MassLibrary} SHARED Mass.cc)
target_compile_options(${MassLibrary} PUBLIC -Wall -Wextra -O2)
target_include_directories(
  ${MassLibrary} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                        $<INSTALL_INTERFACE:include>)

set_target_properties(
  ${MassLibrary}
  PROPERTIES VERSION ${PROJECT_VERSION}
             SOVERSION ${PROJECT_VERSION_MAJOR}
             PUBLIC_HEADER "Mass.h")

install(
  TARGETS ${MassLibrary}
  EXPORT ${PROJECT_NAME}Targets
  INCLUDES
  DESTINATION include
  PUBLIC_HEADER DESTINATION include
  LIBRARY DESTINATION lib)
