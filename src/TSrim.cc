#include "TSrim.h"
#include <Mass.h>
#include <TF1.h>
#include <TString.h>

#include <fstream>
#include <iomanip>
#include <iostream>

/*
  Thickness in mm
  Temperature in K
  Pressure in Torr
  Energy in MeV
  Atomic number: Z
  Mass number: A
  Material name: mat (he, mylar, etc.)

  Log(Range) vs Log(E) plots are fitted by poynomial function
  Applicable for Emin - Emax
  Parameter file format: z a mat par0 - par16 Mean(fit dev) Std dev(fit dev)
*/

ClassImp(TSrim);
///////////////////////////////////////////////////////////////////////////////
TSrim::TSrim() : Nmat(0) {};
TSrim::TSrim(const char *name, const Int_t npol, const char *datafile) : Nmat(0) {
    TSrim::AddElement(name, npol, datafile);
};
TSrim::TSrim(const char *name, const Int_t npol, const char *datafile, Int_t Z, Int_t A) : Nmat(0) {
    TSrim::AddElement(name, npol, datafile, Z, A);
};
TSrim::TSrim(const char *name, const Int_t npol, const char *datafile,
             Int_t Zmin, Int_t Amin, Int_t Zmax, Int_t Amax) : Nmat(0) {
    TSrim::AddElement(name, npol, datafile, Zmin, Amin, Zmax, Amax);
};
///////////////////////////////////////////////////////////////////////////////
Double_t TSrim::Range(Int_t Z, Int_t A, Double_t E, const std::string &mat) {
    if (E <= TSrim::Emin) {
        return 0.;
    } else {
        if (auto it_mat = mat_mapping.find(mat); it_mat != mat_mapping.end()) {
            if (auto it = self_mapping.find(get_key(Z, A, it_mat->second)); it != self_mapping.end()) {
                return pow(10, this->at(it->second).Eval(log10(E)));
            }
        }
        std::cerr << "No data in the range list" << std::endl;
        return TSrim::dummy;
    }
}
Double_t TSrim::Range(Int_t Z, Int_t A, Double_t E, const std::string &mat,
                      Double_t P, Double_t T) { // For gas density correction
    if (P == 0) {
        std::cout << "Gas pressure is zero" << std::endl;
        return TSrim::dummy;
    } else {
        Double_t fd = TSrim::T0 / T * P / TSrim::P1;
        return TSrim::Range(Z, A, E, mat) / fd;
    }
}
Double_t TSrim::RangePu(Int_t Z, Int_t A, Double_t Epu, const std::string &mat) {
    return TSrim::Range(Z, A, Epu * amdc::Mass(Z, A), mat);
}
Double_t TSrim::RangePu(Int_t Z, Int_t A, Double_t Epu, const std::string &mat,
                        Double_t P, Double_t T) {
    return TSrim::Range(Z, A, Epu * amdc::Mass(Z, A), mat, P, T);
}
///////////////////////////////////////////////////////////////////////////////
Double_t TSrim::f(Double_t x, Double_t *c, Int_t npar) {
    Double_t y = 0;
    for (Int_t i = npar; i > 0; i--)
        y = (c[i] + y) * x;
    y += c[0];
    return y;
}
Double_t TSrim::dfdx(Double_t x, Double_t *c, Int_t npar) {
    Double_t y = 0;
    for (Int_t i = npar - 1; i > 1; i--)
        y = (i * c[i] + y) * x;
    y += c[1];
    return y;
}
/////////////
Double_t TSrim::GetXNewton(Int_t n, Double_t y, Double_t epsilon, Int_t maxiter) {
    Double_t *c = this->at(n).GetParameters();
    Int_t npar = this->at(n).GetNpar();
    if (fabs(c[0] - y) < epsilon) {
        return 0;
    }
    Double_t x0 = 0.;
    Double_t x1 = -(c[0] - y) / c[1];
    x0 = x1;
    for (Int_t j = 0; j < maxiter - 1; j++) {
        // x1 = x0 - (this->at(n).Eval(x0) - y) / TSrim::dfdx(x0, c, npar);
        x1 = x0 - (TSrim::f(x0, c, npar) - y) / TSrim::dfdx(x0, c, npar);
        if (fabs(x1 / x0 - 1.) < epsilon) {
            // if (fabs(f.Eval(x1) - y) < epsilon) {
            break;
        }
        x0 = x1;
    }
    return x1;
}
/////////////
Double_t TSrim::RangeToE(Int_t Z, Int_t A, const std::string &mat, Double_t thk) {
    return TSrim::RangeToE(Z, A, mat, thk, TSrim::P1, TSrim::T0);
}
Double_t TSrim::RangeToE(Int_t Z, Int_t A, const std::string &mat, Double_t thk,
                         Double_t P, Double_t T) { // for gas
    Double_t fd = TSrim::T0 / T * P / TSrim::P1;
    if (thk * fd <= TSrim::Rmin) {
        return 0.;
    } else {
        if (auto it_mat = mat_mapping.find(mat); it_mat != mat_mapping.end()) {
            if (auto it = self_mapping.find(get_key(Z, A, it_mat->second)); it != self_mapping.end()) {
                return pow(10, TSrim::GetXNewton(it->second, log10(thk * fd),
                                                 1.e-8, 20)); //, c, npar
                // return pow(10, this->at(it->second).GetX(log10(thk * fd)));
                //  this->at(it->second).SetNpx(20);
                //  return pow(10, this->at(it->second).GetX(log10(thk * fd), TSrim::log10Emin,
                //  				    TSrim::log10Emaxpu * Mass(Z, A),
                //  				    1.e-6,100));
            }
        }
        std::cout << "No data in the range list" << std::endl;
        // exit(0);
        return TSrim::dummy;
    }
}
Double_t TSrim::RangeToEPu(Int_t Z, Int_t A, const std::string &mat, Double_t thk) {
    return TSrim::RangeToE(Z, A, mat, thk) / amdc::Mass(Z, A);
}
Double_t TSrim::RangeToEPu(Int_t Z, Int_t A, const std::string &mat, Double_t thk,
                           Double_t P, Double_t T) { // for gas
    return TSrim::RangeToE(Z, A, mat, thk, P, T) / amdc::Mass(Z, A);
}

///////////////////////////////////////////////////////////////////////////////
Double_t TSrim::EnergyNew(Int_t Z, Int_t A, Double_t Eold, const std::string &mat, Double_t thk) {
    return TSrim::EnergyNew(Z, A, Eold, mat, thk, TSrim::P1, TSrim::T0);
}
Double_t TSrim::EnergyNew(Int_t Z, Int_t A, Double_t Eold, const std::string &mat, Double_t thk,
                          Double_t P, Double_t T) {
    // Double_t fd = TSrim::T0 / T * P / TSrim::P1;
    if (Eold < TSrim::Emin) {
        std::cout << "Energy is too low" << std::endl;
        return 0.;
    } else if (Eold > TSrim::Emaxpu * amdc::Mass(Z, A)) {
        std::cout << "Energy is too high" << std::endl;
        return Eold;
    } else if (Eold <= 0. && thk >= 0.) {
        return 0.;
    } else if (thk == 0. || P == 0.) {
        return Eold;
    } else {
        Double_t Rold = TSrim::Range(Z, A, Eold, mat, P, T);
        if (Rold == TSrim::dummy)
            return TSrim::dummy;

        Double_t Rnew = Rold - thk;

        // if (Rnew <= TSrim::Rmin)
        //     return 0.;
        // else
        //     return pow(10, this->at(i).GetX(log10(Rnew * fd), TSrim::log10Emin,
        //                                     TSrim::log10Emaxpu * Mass(Z, A)));
        return TSrim::RangeToE(Z, A, mat, Rnew, P, T);
    }
}
Double_t TSrim::ENew(Int_t Z, Int_t A, Double_t Eold, const std::string &mat,
                     Double_t thk) {
    return TSrim::EnergyNew(Z, A, Eold, mat, thk);
}
Double_t TSrim::ENew(Int_t Z, Int_t A, Double_t Eold, const std::string &mat, Double_t thk,
                     Double_t P, Double_t T) {
    return TSrim::EnergyNew(Z, A, Eold, mat, thk, P, T);
}
Double_t TSrim::EnergyNewPu(Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat,
                            Double_t thk) {
    return TSrim::EnergyNew(Z, A, Eoldpu * amdc::Mass(Z, A), mat, thk) /
           amdc::Mass(Z, A);
}
Double_t TSrim::EnergyNewPu(Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat,
                            Double_t thk, Double_t P, Double_t T) {
    return TSrim::EnergyNew(Z, A, Eoldpu * amdc::Mass(Z, A), mat, thk, P, T) /
           amdc::Mass(Z, A);
}
Double_t TSrim::ENewPu(Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat,
                       Double_t thk) {
    return TSrim::EnergyNewPu(Z, A, Eoldpu, mat, thk);
}
Double_t TSrim::ENewPu(Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat,
                       Double_t thk, Double_t P, Double_t T) {
    return TSrim::EnergyNewPu(Z, A, Eoldpu, mat, thk, P, T);
}
///////////////////////////////////////////////////////////////////////////////
Double_t TSrim::EnergyLoss(Int_t Z, Int_t A, Double_t Eold, const std::string &mat,
                           Double_t thk) {
    return Eold - TSrim::EnergyNew(Z, A, Eold, mat, thk);
}
Double_t TSrim::EnergyLoss(Int_t Z, Int_t A, Double_t Eold, const std::string &mat,
                           Double_t thk, Double_t P, Double_t T) {
    return Eold - TSrim::EnergyNew(Z, A, Eold, mat, thk, P, T);
}
Double_t TSrim::ELoss(Int_t Z, Int_t A, Double_t Eold, const std::string &mat,
                      Double_t thk) {
    return TSrim::EnergyLoss(Z, A, Eold, mat, thk);
}
Double_t TSrim::ELoss(Int_t Z, Int_t A, Double_t Eold, const std::string &mat,
                      Double_t thk, Double_t P, Double_t T) {
    return TSrim::EnergyLoss(Z, A, Eold, mat, thk, P, T);
}
Double_t TSrim::EnergyLossPu(Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat,
                             Double_t thk) {
    return Eoldpu - TSrim::EnergyNewPu(Z, A, Eoldpu, mat, thk);
}
Double_t TSrim::EnergyLossPu(Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat,
                             Double_t thk, Double_t P, Double_t T) {
    return Eoldpu - TSrim::EnergyNewPu(Z, A, Eoldpu, mat, thk, P, T);
}
Double_t TSrim::ELossPu(Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat,
                        Double_t thk) {
    return TSrim::EnergyLossPu(Z, A, Eoldpu, mat, thk);
}
Double_t TSrim::ELossPu(Int_t Z, Int_t A, Double_t Eoldpu, const std::string &mat,
                        Double_t thk, Double_t P, Double_t T) {
    return TSrim::EnergyLossPu(Z, A, Eoldpu, mat, thk, P, T);
}
///////////////////////////////////////////////////////////////////////////////
Double_t TSrim::EnergiesToThick(Int_t Z, Int_t A, Double_t Eold, Double_t Enew,
                                const std::string &mat) {
    return TSrim::EnergiesToThick(Z, A, Eold, Enew, mat, TSrim::P1,
                                  TSrim::T0);
}
Double_t TSrim::EnergiesToThick(Int_t Z, Int_t A, Double_t Eold, Double_t Enew,
                                const std::string &mat, Double_t P, Double_t T) {
    if (Eold < 0 || Enew < 0) {
        std::cout << "Negative energy" << std::endl;
        return TSrim::dummy;
    } else if (P == 0.) {
        std::cout << "Gas pressure = 0." << std::endl;
        return TSrim::dummy;
    } else {
        if (Eold > TSrim::Emaxpu * amdc::Mass(Z, A)) {
            std::cout << "Eold is too high" << std::endl;
            Eold = Emaxpu * amdc::Mass(Z, A);
        }
        if (Enew > TSrim::Emaxpu * amdc::Mass(Z, A)) {
            std::cout << "Enew is too high" << std::endl;
            Enew = Emaxpu * amdc::Mass(Z, A);
        }
        Double_t Rold = TSrim::Range(Z, A, Eold, mat, P, T);
        Double_t Rnew = TSrim::Range(Z, A, Enew, mat, P, T);
        if (Rold == TSrim::dummy || Rnew == TSrim::dummy) {
            std::cout << "No data in the range list" << std::endl;
            // exit(0);
            return TSrim::dummy;
        }
        return Rold - Rnew;
    }
}
Double_t TSrim::EnergiesToThickPu(Int_t Z, Int_t A, Double_t Eoldpu,
                                  Double_t Enewpu, const std::string &mat) {
    return TSrim::EnergiesToThick(Z, A, Eoldpu * amdc::Mass(Z, A),
                                  Enewpu * amdc::Mass(Z, A), mat);
}
Double_t TSrim::EnergiesToThickPu(Int_t Z, Int_t A, Double_t Eoldpu,
                                  Double_t Enewpu, const std::string &mat, Double_t P,
                                  Double_t T) {
    return TSrim::EnergiesToThick(Z, A, Eoldpu * amdc::Mass(Z, A),
                                  Enewpu * amdc::Mass(Z, A), mat, P, T);
}
Double_t TSrim::EToThk(Int_t Z, Int_t A, Double_t Eold, Double_t Enew,
                       const std::string &mat) {
    return TSrim::EnergiesToThick(Z, A, Eold, Enew, mat);
}
Double_t TSrim::EToThk(Int_t Z, Int_t A, Double_t Eold, Double_t Enew,
                       const std::string &mat, Double_t P, Double_t T) {
    return TSrim::EnergiesToThick(Z, A, Eold, Enew, mat, P, T);
}
Double_t TSrim::EToThkPu(Int_t Z, Int_t A, Double_t Eoldpu, Double_t Enewpu,
                         const std::string &mat) {
    return TSrim::EnergiesToThickPu(Z, A, Eoldpu, Enewpu, mat);
}
Double_t TSrim::EToThkPu(Int_t Z, Int_t A, Double_t Eoldpu, Double_t Enewpu,
                         const std::string &mat, Double_t P, Double_t T) {
    return TSrim::EnergiesToThickPu(Z, A, Eoldpu, Enewpu, mat, P, T);
}
Double_t TSrim::ELossToThk(Int_t Z, Int_t A, Double_t Eold, Double_t dE,
                           const std::string &mat) {
    return TSrim::EnergiesToThick(Z, A, Eold, Eold - dE, mat);
}
Double_t TSrim::ELossToThk(Int_t Z, Int_t A, Double_t Eold, Double_t dE,
                           const std::string &mat, Double_t P, Double_t T) {
    return TSrim::EnergiesToThick(Z, A, Eold, Eold - dE, mat, P, T);
}
Double_t TSrim::ELossToThkPu(Int_t Z, Int_t A, Double_t Eoldpu, Double_t dEpu,
                             const std::string &mat) {
    return TSrim::EnergiesToThickPu(Z, A, Eoldpu, Eoldpu - dEpu, mat);
}
Double_t TSrim::ELossToThkPu(Int_t Z, Int_t A, Double_t Eoldpu, Double_t dEpu,
                             const std::string &mat, Double_t P, Double_t T) {
    return TSrim::EnergiesToThickPu(Z, A, Eoldpu, Eoldpu - dEpu, mat, P, T);
}
///////////////////////////////////////////////////////////////////////////////
void TSrim::ShowMatList() {
    std::vector<std::pair<std::string, int>> vec(mat_mapping.begin(), mat_mapping.end());
    std::sort(vec.begin(), vec.end(),
              [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
                  return a.second < b.second;
              });

    for (const auto &pair : vec) {
        std::cout << "name: " << pair.first << std::endl;
    }
}

void TSrim::ShowMatNuclList() {
    std::vector<std::pair<std::size_t, int>> vec(self_mapping.begin(), self_mapping.end());

    std::sort(vec.begin(), vec.end(),
              [](const std::pair<std::size_t, int> &a, const std::pair<std::size_t, int> &b) {
                  return a.second < b.second; // indexでソート
              });

    for (const auto &pair : vec) {
        std::size_t key = pair.first;
        // int index = pair.second;

        int matID = static_cast<int>(key & 0x7F);       // lowest 7 bits
        int A = static_cast<int>((key >> 7) & 0x3FF);   // next 10 bits
        int Z = static_cast<int>((key >> 17) & 0x7FFF); // highest 15 bits

        std::string mat;
        for (const auto &mat_pair : mat_mapping) {
            if (mat_pair.second == matID) {
                mat = mat_pair.first;
                break;
            }
        }
        std::string el = std::to_string(Z) + " (" + amdc::GetEl(Z) + ")";
        std::cout << "Z: " << std::left << std::setw(10) << el
                  << "A: " << std::left << std::setw(5) << A
                  << "mat: " << std::left << std::setw(10) << mat
                  // << "index: " << std::left << std::setw(10) << index
                  << std::endl;
    }
}

void TSrim::AddElement(const char *name, const Int_t npol, const char *datafile) {
    TSrim::AddElement(name, npol, datafile, -1, -1, -1, -1);
}
void TSrim::AddElement(const char *name, const Int_t npol, const char *datafile,
                       Int_t Z, Int_t A) {
    TSrim::AddElement(name, npol, datafile, Z, A, -1, -1);
}
void TSrim::AddElement(const char *name, const Int_t npol, const char *datafile,
                       Int_t Zmin, Int_t Amin, Int_t Zmax, Int_t Amax) {
    TString fn_name = Form("pol%d", npol);
    Double_t dummy;
    Int_t Zdat, Adat;
    std::string mat;
    Double_t par[npol + 1];
    vector<Int_t> vZ, vA;
    vector<std::string> vmat;
    vector<Double_t> vpar[npol + 1];

    auto addData = [&](Int_t Zdat, Int_t Adat, const std::string &mat, const Double_t *par) {
        vZ.emplace_back(Zdat);
        vA.emplace_back(Adat);
        vmat.emplace_back(mat);
        for (Int_t j = 0; j < npol + 1; ++j) {
            vpar[j].emplace_back(par[j]);
        }
    };

    std::ifstream fpar(datafile);
    while (!fpar.eof()) {
        fpar >> Zdat >> Adat >> mat;
        for (Int_t j = 0; j < npol + 1; j++)
            fpar >> par[j];
        fpar >> dummy >> dummy;
        if (fpar.eof())
            break;

        if (Zmin == -1 && Amin == -1 && Zmax == -1 && Amax == -1) {
            addData(Zdat, Adat, mat, par);
        } else if (Zmin != -1 && Amin != -1 && Zdat == Zmin && Adat == Amin) {
            addData(Zdat, Adat, mat, par);
            break;
        } else if (Zdat >= Zmin && Adat >= Amin && Zdat <= Zmax && Adat <= Amax) {
            addData(Zdat, Adat, mat, par);
        }
    }
    fpar.close();
    if (vZ.size() == 0) {
        std::cout << "No isotope data loaded." << std::endl;
    }

    Int_t self_index = this->size();
    for (Size_t i = 0; i < vZ.size(); i++) {
        /// register the mapping
        // Do nothing if the key already exists
        auto mat_result = mat_mapping.emplace(vmat.at(i), Nmat);
        if (mat_result.second)
            Nmat++;

        if (auto it = mat_mapping.find(vmat.at(i)); it != mat_mapping.end()) {
            auto result = self_mapping.emplace(get_key(vZ.at(i), vA.at(i), it->second), self_index);
            if (!result.second) {
                std::cerr << "Z: " << vZ.at(i) << ", A: " << vA.at(i) << ", mat: " << vmat.at(i)
                          << " already exists with index: " << result.first->second << std::endl;
            } else {
                this->emplace_back(TF1(Form("%d-%d_%s", vZ.at(i), vA.at(i), vmat.at(i).c_str()),
                                       fn_name.Data(), TSrim::log10Emin,
                                       TSrim::log10Emaxpu * amdc::Mass(vZ.at(i), vA.at(i))));
                for (Int_t j = 0; j < npol + 1; j++) {
                    this->at(self_index).SetParameter(j, vpar[j].at(i));
                }
                self_index++;
            }
        }
    }
}
