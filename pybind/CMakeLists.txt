# library name
set(PYLIBNAME pyTSrim)

# bind file
set(PYBINDFILE pyTSrim.cpp)

nanobind_add_module(${PYLIBNAME} NB_STATIC ${PYBINDFILE})
target_include_directories(${PYLIBNAME} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${PYLIBNAME} PRIVATE ${ROOT_LIBRARY} ${LIBNAME})

install(TARGETS ${PYLIBNAME} LIBRARY DESTINATION lib)
