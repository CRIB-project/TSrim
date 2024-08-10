# pyTSrim definition
# library name
set(PYLIBNAME pyTSrim)

# bind file
set(PYBINDFILE pyTSrim.cpp)

nanobind_add_module(${PYLIBNAME} NB_STATIC ${PYBINDFILE})
target_include_directories(${PYLIBNAME} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${PYLIBNAME} PRIVATE ${ROOT_LIBRARY} ${LIBNAME})

install(TARGETS ${PYLIBNAME} LIBRARY DESTINATION lib)

# amdc_mass definition
# library name
set(PYMASSLIBNAME amdc_mass)

# bind file
set(PYMASSBINDFILE amdc_mass.cpp)

nanobind_add_module(${PYMASSLIBNAME} NB_STATIC ${PYMASSBINDFILE})
target_include_directories(${PYMASSLIBNAME} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${PYMASSLIBNAME} PRIVATE ${MASSLIBNAME})

install(TARGETS ${PYMASSLIBNAME} LIBRARY DESTINATION lib)
