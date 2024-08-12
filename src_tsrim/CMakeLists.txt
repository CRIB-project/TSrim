# TSrim library
set(SrimLibrary Srim)

add_library(${SrimLibrary} SHARED TSrim.cc ../src_mass/Mass.cc)
target_compile_options(${SrimLibrary} PUBLIC -Wall -Wextra -O2)
target_include_directories(
  ${SrimLibrary} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src_mass>
                        $<INSTALL_INTERFACE:include>)

target_link_libraries(${SrimLibrary} ${ROOT_LIBRARIES} ${MassLibrary})

root_generate_dictionary(G__${SrimLibrary} TSrim.h LINKDEF LinkDef.h MODULE
                         ${SrimLibrary})

set_target_properties(
  ${SrimLibrary}
  PROPERTIES VERSION ${PROJECT_VERSION}
             SOVERSION ${PROJECT_VERSION_MAJOR}
             PUBLIC_HEADER "TSrim.h")

install(
  TARGETS ${SrimLibrary}
  EXPORT ${PROJECT_NAME}Targets
  INCLUDES
  DESTINATION include
  PUBLIC_HEADER DESTINATION include
  LIBRARY DESTINATION lib)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${SrimLibrary}_rdict.pcm
        DESTINATION lib)

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/cmake/${PROJECT_NAME})
