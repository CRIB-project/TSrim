# pyMass
set(PyMassModule pyMass)

nanobind_add_module(${PyMassModule} NB_STATIC pyMass.cpp)
target_compile_options(${PyMassModule} PRIVATE -Wall -Wextra -O2)
target_include_directories(${PyMassModule} PRIVATE ${CMAKE_SOURCE_DIR}/mass)
target_link_libraries(${PyMassModule} PRIVATE ${MassLibrary})

# pyTSrim
set(PySrimModule pyTSrim)

nanobind_add_module(${PySrimModule} NB_STATIC pyTSrim.cpp)
target_compile_options(${PySrimModule} PRIVATE -Wall -Wextra -O2)
target_include_directories(${PySrimModule} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${PySrimModule} PRIVATE ${ROOT_LIBRARY} ${SrimLibrary})

# install
install(TARGETS ${PyMassModule} ${PySrimModule} LIBRARY DESTINATION lib)
