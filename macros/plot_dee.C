#include <Mass.h>
#include <TSrim.h>
// R__LOAD_LIBRARY(/usr/local/lib/libSrim.so);
/* const Int_t npol = 16; */
/* TString filename = Form("range_fit/range_fit_pol%d_",npol); */
/* TString fileext = "txt"; */
TString range_dir = "/usr/local/share/range_fit";

struct isotope {
    Int_t Z;
    Int_t A;
};

void plot_dee(Double_t th_de, Double_t th_e, vector<struct isotope> itlist,
              Double_t dEmax, Double_t Etotmax) {
    TCanvas *c1 = new TCanvas("c1", "", 0, 0, 800, 800);

    TH1F *fr1 = gPad->DrawFrame(0, 0, dEmax, Etotmax);
    fr1->Draw();

    gSystem->Load("libSrim.so");
    TSrim *srim = new TSrim("si", 16, Form("%s/range_fit_pol16_si.txt", range_dir.Data()),
                            1, 1, 20, 40);

    Double_t Estep = 0.01;
    const Int_t nlist = itlist.size();
    TGraph *gr[nlist];
    for (Int_t i = 0; i < nlist; i++) {
        vector<Double_t> vdE;
        vector<Double_t> vEtot;
        Double_t Etot = 0.01;
        while (Etot < Etotmax) {
            Double_t dE = srim->ELoss(itlist.at(i).Z, itlist.at(i).A,
                                      Etot, "si", th_de);
            vdE.push_back(dE);
            if (Etot - dE > 0.001)
                vEtot.push_back(dE + srim->ELoss(itlist.at(i).Z, itlist.at(i).A,
                                                 Etot - dE, "si", th_e));
            else
                vEtot.push_back(Etot);
            Etot += Estep;
        }
        gr[i] = new TGraph(vdE.size(), &(vdE.at(0)), &(vEtot.at(0)));
        gr[i]->Draw("same");
        c1->Update();
        gSystem->ProcessEvents();
        // getchar();
    }
}

void plot_dee(Double_t th_de, Double_t th_e,
              Double_t dEmax, Double_t Etotmax) {
    // For p, d, t, 3He and a
    plot_dee(th_de, th_e, {{1, 1}, {1, 2}, {1, 3}, {2, 3}, {2, 4}}, dEmax, Etotmax);
}
