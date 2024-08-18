#include "TSrim.h"
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

NB_MODULE(pyTSrim, m) {
    nb::class_<TSrim>(m, "TSrim")
        .def(nb::init<>(), nb::rv_policy::take_ownership)
        .def(nb::init<const char *, const Int_t, const char *>(), nb::rv_policy::take_ownership)
        .def(nb::init<const char *, const Int_t, const char *, Int_t, Int_t>(), nb::rv_policy::take_ownership)
        .def(nb::init<const char *, const Int_t, const char *, Int_t, Int_t, Int_t, Int_t>(), nb::rv_policy::take_ownership)
        // Method bindings
        .def("Range", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &>(&TSrim::Range))
        .def("Range", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t, Double_t>(&TSrim::Range))
        .def("RangePu", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &>(&TSrim::RangePu))
        .def("RangePu", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t, Double_t>(&TSrim::RangePu))
        .def("RangeToE", nb::overload_cast<Int_t, Int_t, const std::string &, Double_t>(&TSrim::RangeToE))
        .def("RangeToE", nb::overload_cast<Int_t, Int_t, const std::string &, Double_t, Double_t, Double_t>(&TSrim::RangeToE))
        .def("RangeToEPu", nb::overload_cast<Int_t, Int_t, const std::string &, Double_t>(&TSrim::RangeToEPu))
        .def("RangeToEPu", nb::overload_cast<Int_t, Int_t, const std::string &, Double_t, Double_t, Double_t>(&TSrim::RangeToEPu))
        .def("EnergyNew", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t>(&TSrim::EnergyNew))
        .def("EnergyNew", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t, Double_t, Double_t>(&TSrim::EnergyNew))
        .def("EnergyNewPu", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t>(&TSrim::EnergyNewPu))
        .def("EnergyNewPu", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t, Double_t, Double_t>(&TSrim::EnergyNewPu))
        .def("ENew", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t>(&TSrim::ENew))
        .def("ENew", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t, Double_t, Double_t>(&TSrim::ENew))
        .def("ENewPu", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t>(&TSrim::ENewPu))
        .def("ENewPu", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t, Double_t, Double_t>(&TSrim::ENewPu))
        .def("EnergyLoss", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t>(&TSrim::EnergyLoss))
        .def("EnergyLoss", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t, Double_t, Double_t>(&TSrim::EnergyLoss))
        .def("EnergyLossPu", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t>(&TSrim::EnergyLossPu))
        .def("EnergyLossPu", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t, Double_t, Double_t>(&TSrim::EnergyLossPu))
        .def("ELoss", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t>(&TSrim::ELoss))
        .def("ELoss", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t, Double_t, Double_t>(&TSrim::ELoss))
        .def("ELossPu", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t>(&TSrim::ELossPu))
        .def("ELossPu", nb::overload_cast<Int_t, Int_t, Double_t, const std::string &, Double_t, Double_t, Double_t>(&TSrim::ELossPu))
        .def("EnergiesToThick", nb::overload_cast<Int_t, Int_t, Double_t, Double_t, const std::string &>(&TSrim::EnergiesToThick))
        .def("EnergiesToThick", nb::overload_cast<Int_t, Int_t, Double_t, Double_t, const std::string &, Double_t, Double_t>(&TSrim::EnergiesToThick))
        .def("EnergiesToThickPu", nb::overload_cast<Int_t, Int_t, Double_t, Double_t, const std::string &>(&TSrim::EnergiesToThickPu))
        .def("EnergiesToThickPu", nb::overload_cast<Int_t, Int_t, Double_t, Double_t, const std::string &, Double_t, Double_t>(&TSrim::EnergiesToThickPu))
        .def("EToThk", nb::overload_cast<Int_t, Int_t, Double_t, Double_t, const std::string &>(&TSrim::EToThk))
        .def("EToThk", nb::overload_cast<Int_t, Int_t, Double_t, Double_t, const std::string &, Double_t, Double_t>(&TSrim::EToThk))
        .def("EToThkPu", nb::overload_cast<Int_t, Int_t, Double_t, Double_t, const std::string &>(&TSrim::EToThkPu))
        .def("EToThkPu", nb::overload_cast<Int_t, Int_t, Double_t, Double_t, const std::string &, Double_t, Double_t>(&TSrim::EToThkPu))
        .def("ELossToThk", nb::overload_cast<Int_t, Int_t, Double_t, Double_t, const std::string &>(&TSrim::ELossToThk))
        .def("ELossToThk", nb::overload_cast<Int_t, Int_t, Double_t, Double_t, const std::string &, Double_t, Double_t>(&TSrim::ELossToThk))
        .def("ELossToThkPu", nb::overload_cast<Int_t, Int_t, Double_t, Double_t, const std::string &>(&TSrim::ELossToThkPu))
        .def("ELossToThkPu", nb::overload_cast<Int_t, Int_t, Double_t, Double_t, const std::string &, Double_t, Double_t>(&TSrim::ELossToThkPu))
        .def("ShowMatList", &TSrim::ShowMatList)
        .def("ShowMatNuclList", &TSrim::ShowMatNuclList)
        .def("AddElement", nb::overload_cast<const char *, const Int_t, const char *>(&TSrim::AddElement))
        .def("AddElement", nb::overload_cast<const char *, const Int_t, const char *, Int_t, Int_t>(&TSrim::AddElement))
        .def("AddElement", nb::overload_cast<const char *, const Int_t, const char *, Int_t, Int_t, Int_t, Int_t>(&TSrim::AddElement))
        .def("GetNmaterial", &TSrim::GetNmaterial);

    m.attr("P1") = nb::float_(TSrim::P1);
    m.attr("T0") = nb::float_(TSrim::T0);

    m.doc() = "python extention of TSrim";
}
