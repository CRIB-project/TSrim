#ifndef __TSRIM
#define __TSRIM
#include <TF1.h>

class TSrim : public std::vector<TF1> {
public:
  //////////////////////////////////////////////////////////////////////////
  // Constructors
  TSrim();
  TSrim(const char *name, const Int_t npol, const char *datafile);
  TSrim(const char *name, const Int_t npol, const char *datafile, Int_t Z,
        Int_t A);
  TSrim(const char *name, const Int_t npol, const char *datafile, Int_t Zmin,
        Int_t Amin, Int_t Zmax, Int_t Amax);
  //////////////////////////////////////////////////////////////////////////
  // Range in a material of an ion at an energy
  Double_t Range(Int_t Z, Int_t A, Double_t E, TString mat);
  Double_t Range(Int_t Z, Int_t A, Double_t E, TString mat,
		 Double_t P, Double_t T); // for gas
  // Energy per nucleon
  Double_t RangePu(Int_t Z, Int_t A, Double_t Epu, TString mat);
  Double_t RangePu(Int_t Z, Int_t A, Double_t Epu, TString mat,
		   Double_t P, Double_t T); // for gas
  //////////////////////////////////////////////////////////////////////////
  // Energy for a specified range in a material of an ion
  Double_t RangeToE(Int_t Z, Int_t A, TString mat, Double_t thk);
  Double_t RangeToE(Int_t Z, Int_t A, TString mat, Double_t thk,
		    Double_t P, Double_t T); // for gas
  // Energy per nucleon
  Double_t RangeToEPu(Int_t Z, Int_t A, TString mat, Double_t thk);
  Double_t RangeToEPu(Int_t Z, Int_t A, TString mat, Double_t thk,
		      Double_t P, Double_t T); // for gas
  //////////////////////////////////////////////////////////////////////////
  // Energy of an ion after a material with a specified thickness
  Double_t EnergyNew(Int_t Z, Int_t A, Double_t Eold, TString mat,
                     Double_t thk);
  Double_t EnergyNew(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk,
                     Double_t P, Double_t T); // for gas
  // Energy per nucleon
  Double_t EnergyNewPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat,
                       Double_t thk);
  Double_t EnergyNewPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat,
                       Double_t thk, Double_t P, Double_t T); // for gas
  // Aliases
  Double_t ENew(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk);
  Double_t ENew(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk,
                Double_t P, Double_t T); // for gas
  // Aliases
  Double_t ENewPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat, Double_t thk);
  Double_t ENewPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat, Double_t thk,
                  Double_t P, Double_t T); // for gas
  //////////////////////////////////////////////////////////////////////////
  // Energy loss of an ion in a material with a specified thickness
  Double_t EnergyLoss(Int_t Z, Int_t A, Double_t Eold, TString mat,
                      Double_t thk);
  Double_t EnergyLoss(Int_t Z, Int_t A, Double_t Eold, TString mat,
                      Double_t thk, Double_t P, Double_t T); // for gas
  // Energy per nucleon
  Double_t EnergyLossPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat,
                        Double_t thk);
  Double_t EnergyLossPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat,
                        Double_t thk, Double_t P, Double_t T); // for gas
  // Aliases
  Double_t ELoss(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk);
  Double_t ELoss(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk,
                 Double_t P, Double_t T); // for gas
  // Aliases
  Double_t ELossPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat,
                   Double_t thk);
  Double_t ELossPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat, Double_t thk,
                   Double_t P, Double_t T); // for gas
  //////////////////////////////////////////////////////////////////////////
  // To show material list
  void ShowMatList();
  // To show material list and the nuclide range
  void ShowMatListZAN();
  //////////////////////////////////////////////////////////////////////////

private:
  const Double_t T0 = 273.15;
  const Double_t P1 = 760.;
  const Double_t Emin = 0.001;  // MeV
  const Double_t Emaxpu = 400.; // MeV/u
  const Double_t log10Emin = log10(Emin);
  const Double_t log10Emaxpu = log10(Emaxpu);
  const Double_t Rmin = 0.00001; // Range > 10 nm: To avoid too large EnergyNew
  const Double_t dummy = -1000.;

  ClassDef(TSrim, 1);
};

#endif
