#include <Mass.h>
#include <TSrim.h>

using namespace amdc;

// The header (Mass.h, TSrim.h) directory path should be defined in ~/.rootlogon.C
// The library path (libSrim.so) should be defined in ~/.rootrc

const Int_t npol = 16;
TString dir = "/usr/local/share/range_fit";
TString filename = Form("%s/range_fit_pol%d_", dir.Data(), npol);
TString fileext = "txt";

// Material lists (thinking about better management...)
vector<TString> mat_all = {
    "3he", "air", "al", "anthracene", "ar", "au",
    "be", "butane", "c", "cd2", "ch2", "cu",
    "h", "havar", "he", "he_90_co2_10", "kapton", "mo",
    "mylar", "ni", "si", "ti"};
vector<TString> mat_basic = {
    "al", "ar", "au",
    "c", "ch2",
    "h", "havar", "he", "kapton",
    "mylar", "si"};
vector<TString> mat_adv = {
    "3he", "al", "ar", "au",
    "be", "butane", "c", "cd2", "ch2", "cu",
    "h", "havar", "he", "kapton", "mo",
    "mylar", "si", "ti"};
vector<TString> mat_solid = {
    "al", "anthracene", "au",
    "be", "c", "cd2", "ch2", "cu",
    "havar", "kapton", "mo",
    "mylar", "ni", "si", "ti"};
vector<TString> mat_gas = {
    "3he", "air", "ar",
    "butane",
    "h", "he", "he_90_co2_10"};
vector<TString> mat_cribpid = {
    "al", "ar", "havar", "he",
    "he_90_co2_10", "kapton", "mylar"};

TSrim *load_srim() {
    gSystem->Load("libSrim.so");

    TSrim *srim0 = new TSrim();
    const Int_t nmat = mat_all.size();
    TSrim *srim[nmat];
    for (Int_t i = 0; i < nmat; i++) {
        srim[i] = new TSrim(mat_all.at(i).Data(), npol,
                            Form("%s%s.%s", filename.Data(),
                                 mat_all.at(i).Data(), fileext.Data()));
        cout << Form("%s list loaded.", mat_all.at(i).Data()) << endl;
        srim0->TSrim::insert(srim0->end(), srim[i]->begin(), srim[i]->end());
    }
    return srim0;
}

TSrim *load_srim(vector<TString> mat) {
    gSystem->Load("libSrim.so");
    const Int_t nmat = mat.size();

    TSrim *srim0 = new TSrim();
    TSrim *srim[nmat];
    for (Int_t i = 0; i < nmat; i++) {
        srim[i] = new TSrim(mat.at(i).Data(), npol,
                            Form("%s%s.%s", filename.Data(),
                                 mat[i].Data(), fileext.Data()));
        cout << Form("%s list loaded.", mat[i].Data()) << endl;
        srim0->TSrim::insert(srim0->end(), srim[i]->begin(), srim[i]->end());
    }
    return srim0;
}

TSrim *load_srim(vector<TString> mat, Int_t Z, Int_t A) {
    gSystem->Load("libSrim.so");
    const Int_t nmat = mat.size();

    TSrim *srim0 = new TSrim();
    TSrim *srim[nmat];
    for (Int_t i = 0; i < nmat; i++) {
        srim[i] = new TSrim(mat.at(i).Data(), npol,
                            Form("%s%s.%s", filename.Data(),
                                 mat[i].Data(), fileext.Data()),
                            Z, A);
        cout << Form("%s list loaded for %d%s.",
                     mat[i].Data(), A, GetEl(Z))
             << endl;
        srim0->TSrim::insert(srim0->end(), srim[i]->begin(), srim[i]->end());
    }
    return srim0;
}

TSrim *load_srim(vector<TString> mat,
                 Int_t Zmin, Int_t Amin, Int_t Zmax, Int_t Amax) {
    gSystem->Load("libSrim.so");
    const Int_t nmat = mat.size();

    TSrim *srim0 = new TSrim();
    TSrim *srim[nmat];
    for (Int_t i = 0; i < nmat; i++) {
        srim[i] = new TSrim(mat.at(i).Data(), npol,
                            Form("%s%s.%s", filename.Data(),
                                 mat[i].Data(), fileext.Data()),
                            Zmin, Amin, Zmax, Amax);
        cout << Form("%s list loaded for %d%s - %d%s.",
                     mat[i].Data(), Amin, GetEl(Zmin), Amax, GetEl(Zmax))
             << endl;
        srim0->TSrim::insert(srim0->end(), srim[i]->begin(), srim[i]->end());
    }
    return srim0;
}
