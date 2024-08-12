#include "Mass.h"
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

NB_MODULE(pyMass, m) {
    m.def("Mass", nb::overload_cast<int, int>(&amdc::Mass), "Get the mass of an isotope by Z and A");
    m.def("Mass", nb::overload_cast<int, const std::string &>(&amdc::Mass), "Get the mass of an isotope by element symbol and A");
    m.def("MassExcess", nb::overload_cast<int, int>(&amdc::MassExcess), "Get the mass excess by Z and A");
    m.def("MassExcess", nb::overload_cast<int, const std::string &>(&amdc::MassExcess), "Get the mass excess by element symbol and A");
    m.def("EBindPu", nb::overload_cast<int, int>(&amdc::EBindPu), "Get the binding energy per nucleon by Z and A");
    m.def("EBindPu", nb::overload_cast<int, const std::string &>(&amdc::EBindPu), "Get the binding energy per nucleon by element symbol and A");
    m.def("GetEl", &amdc::GetEl, "Get the element symbol from the atomic number");
    m.def("GetZ", &amdc::GetZ, "Get the atomic number from the element symbol");

    m.doc() = "amdc Mass table";
}
