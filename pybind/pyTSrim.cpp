#include "TSrim.h"
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

std::string tstring_to_string(const TString &tstr) {
    return std::string(tstr.Data());
}

TString string_to_tstring(const std::string &str) {
    return TString(str.c_str());
}

NB_MODULE(pyTSrim, m) {
    nb::class_<TSrim>(m, "TSrim")
        .def(nb::init<>())
        .def(nb::init<const char *, const Int_t, const char *>())
        .def(nb::init<const char *, const Int_t, const char *, Int_t, Int_t>())
        .def(nb::init<const char *, const Int_t, const char *, Int_t, Int_t, Int_t, Int_t>())
        .def("Range", [](TSrim &self, Int_t Z, Int_t A, Double_t E, const std::string &mat) {
            return self.Range(Z, A, E, string_to_tstring(mat));
        })
        .def("Range", [](TSrim &self, Int_t Z, Int_t A, Double_t E, const std::string &mat, Double_t P, Double_t T) {
            return self.Range(Z, A, E, string_to_tstring(mat), P, T);
        })
        .def("RangePu", [](TSrim &self, Int_t Z, Int_t A, Double_t Epu, const std::string &mat) {
            return self.RangePu(Z, A, Epu, string_to_tstring(mat));
        })
        .def("RangePu", [](TSrim &self, Int_t Z, Int_t A, Double_t Epu, const std::string &mat, Double_t P, Double_t T) {
            return self.RangePu(Z, A, Epu, string_to_tstring(mat), P, T);
        })
        .def("RangeToE", [](TSrim &self, Int_t Z, Int_t A, const std::string &mat, Double_t thk) {
            return self.RangeToE(Z, A, string_to_tstring(mat), thk);
        })
        .def("RangeToE", [](TSrim &self, Int_t Z, Int_t A, const std::string &mat, Double_t thk, Double_t P, Double_t T) {
            return self.RangeToE(Z, A, string_to_tstring(mat), thk, P, T);
        })
        .def("RangeToEPu", [](TSrim &self, Int_t Z, Int_t A, const std::string &mat, Double_t thk) {
            return self.RangeToEPu(Z, A, string_to_tstring(mat), thk);
        })
        .def("RangeToEPu", [](TSrim &self, Int_t Z, Int_t A, const std::string &mat, Double_t thk, Double_t P, Double_t T) {
            return self.RangeToEPu(Z, A, string_to_tstring(mat), thk, P, T);
        })
        .def("EnergyNew", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, const std::string &mat, Double_t thk) {
            return self.EnergyNew(Z, A, Eold, string_to_tstring(mat), thk);
        })
        .def("EnergyNew", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, const std::string &mat, Double_t thk, Double_t P, Double_t T) {
            return self.EnergyNew(Z, A, Eold, string_to_tstring(mat), thk, P, T);
        })
        .def("EnergyNewPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat, Double_t thk) {
            return self.EnergyNewPu(Z, A, Eoldpu, string_to_tstring(mat), thk);
        })
        .def("EnergyNewPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat, Double_t thk, Double_t P, Double_t T) {
            return self.EnergyNewPu(Z, A, Eoldpu, string_to_tstring(mat), thk, P, T);
        })
        .def("ENew", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, const std::string &mat, Double_t thk) {
            return self.ENew(Z, A, Eold, string_to_tstring(mat), thk);
        })
        .def("ENew", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, const std::string &mat, Double_t thk, Double_t P, Double_t T) {
            return self.ENew(Z, A, Eold, string_to_tstring(mat), thk, P, T);
        })
        .def("ENewPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat, Double_t thk) {
            return self.ENewPu(Z, A, Eoldpu, string_to_tstring(mat), thk);
        })
        .def("ENewPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat, Double_t thk, Double_t P, Double_t T) {
            return self.ENewPu(Z, A, Eoldpu, string_to_tstring(mat), thk, P, T);
        })
        .def("EnergyLoss", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, const std::string &mat, Double_t thk) {
            return self.EnergyLoss(Z, A, Eold, string_to_tstring(mat), thk);
        })
        .def("EnergyLoss", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, const std::string &mat, Double_t thk, Double_t P, Double_t T) {
            return self.EnergyLoss(Z, A, Eold, string_to_tstring(mat), thk, P, T);
        })
        .def("EnergyLossPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat, Double_t thk) {
            return self.EnergyLossPu(Z, A, Eoldpu, string_to_tstring(mat), thk);
        })
        .def("EnergyLossPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat, Double_t thk, Double_t P, Double_t T) {
            return self.EnergyLossPu(Z, A, Eoldpu, string_to_tstring(mat), thk, P, T);
        })
        .def("ELoss", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, const std::string &mat, Double_t thk) {
            return self.ELoss(Z, A, Eold, string_to_tstring(mat), thk);
        })
        .def("ELoss", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, const std::string &mat, Double_t thk, Double_t P, Double_t T) {
            return self.ELoss(Z, A, Eold, string_to_tstring(mat), thk, P, T);
        })
        .def("ELossPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat, Double_t thk) {
            return self.ELossPu(Z, A, Eoldpu, string_to_tstring(mat), thk);
        })
        .def("ELossPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat, Double_t thk, Double_t P, Double_t T) {
            return self.ELossPu(Z, A, Eoldpu, string_to_tstring(mat), thk, P, T);
        })
        .def("EnergiesToThick", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, Double_t Enew, const std::string &mat) {
            return self.EnergiesToThick(Z, A, Eold, Enew, string_to_tstring(mat));
        })
        .def("EnergiesToThick", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, Double_t Enew, const std::string &mat, Double_t P, Double_t T) {
            return self.EnergiesToThick(Z, A, Eold, Enew, string_to_tstring(mat), P, T);
        })
        .def("EnergiesToThickPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, Double_t Enewpu, const std::string &mat) {
            return self.EnergiesToThickPu(Z, A, Eoldpu, Enewpu, string_to_tstring(mat));
        })
        .def("EnergiesToThickPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, Double_t Enewpu, const std::string &mat, Double_t P, Double_t T) {
            return self.EnergiesToThickPu(Z, A, Eoldpu, Enewpu, string_to_tstring(mat), P, T);
        })
        .def("EToThk", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, Double_t Enew, const std::string &mat) {
            return self.EToThk(Z, A, Eold, Enew, string_to_tstring(mat));
        })
        .def("EToThk", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, Double_t Enew, const std::string &mat, Double_t P, Double_t T) {
            return self.EToThk(Z, A, Eold, Enew, string_to_tstring(mat), P, T);
        })
        .def("EToThkPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, Double_t Enewpu, const std::string &mat) {
            return self.EToThkPu(Z, A, Eoldpu, Enewpu, string_to_tstring(mat));
        })
        .def("EToThkPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, Double_t Enewpu, const std::string &mat, Double_t P, Double_t T) {
            return self.EToThkPu(Z, A, Eoldpu, Enewpu, string_to_tstring(mat), P, T);
        })
        .def("ELossToThk", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, Double_t dE, const std::string &mat) {
            return self.ELossToThk(Z, A, Eold, dE, string_to_tstring(mat));
        })
        .def("ELossToThk", [](TSrim &self, Int_t Z, Int_t A, Double_t Eold, Double_t dE, const std::string &mat, Double_t P, Double_t T) {
            return self.ELossToThk(Z, A, Eold, dE, string_to_tstring(mat), P, T);
        })
        .def("ELossToThkPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, Double_t dEpu, const std::string &mat) {
            return self.ELossToThkPu(Z, A, Eoldpu, dEpu, string_to_tstring(mat));
        })
        .def("ELossToThkPu", [](TSrim &self, Int_t Z, Int_t A, Double_t Eoldpu, Double_t dEpu, const std::string &mat, Double_t P, Double_t T) {
            return self.ELossToThkPu(Z, A, Eoldpu, dEpu, string_to_tstring(mat), P, T);
        })
        .def("ShowMatList", &TSrim::ShowMatList)
        .def("ShowMatNuclList", &TSrim::ShowMatNuclList);

    m.doc() = "python extention of TSrim";
}
