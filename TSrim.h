#include <TF1.h>

class TSrim: public std::vector<TF1>{
public:
  TSrim();
  TSrim(const char *name, const char *datafile);

  Double_t Range(Int_t Z, Int_t A, Double_t E, TString mat);
  Double_t Range(Int_t Z, Int_t A, Double_t E, TString mat,
		 Double_t P, Double_t T); // for gas
  Double_t EnergyNew(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk);
  Double_t EnergyNew(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk,
		     Double_t P, Double_t T); // for gas
  Double_t EnergyLoss(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk);
  Double_t EnergyLoss(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk,
		      Double_t P, Double_t T); // for gas

  const Double_t T0 = 273.15;
  const Double_t P1 = 760.;
  const Double_t Emin = 0.001; // MeV
  const Double_t Emax = 300.; // MeV
  const Double_t log10Emin = log10(Emin);
  const Double_t log10Emax = log10(Emax);
  
  // Double_t T0();
  // Double_t P1();
  // Double_t Emin();
  // Double_t Emax();
  // Double_t log10Emin();
  // Double_t log10Emax();
  
  ClassDef(TSrim,1);
};
