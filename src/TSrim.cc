#include "TSrim.h"
#include <TF1.h>
#include <TString.h>

#include <stdlib.h>
#include <fstream>
using namespace std;

/*
  Thickness in mm
  Temperature in K
  Pressure in Torr
  Energy in MeV
  Atomic number: Z
  Mass number: A
  Material name: mat (he, mylar, etc.)

  Log(Range) vs Log(E) plots are fitted by pol10
  Applicable for Emin - Emax
  Parameter file format: z m mat par0 - par10 chi^2/NDF 

*/

// const Double_t T0 = 273.15;
// const Double_t P1 = 760.;
// const Double_t Emin = 0.001; // MeV
// const Double_t Emax = 300.; // MeV
// const Double_t log10Emin = log10(Emin);
// const Double_t log10Emax = log10(Emax);

ClassImp(TSrim);

TSrim::TSrim(){};
TSrim::TSrim(const char *name, const char *datafile){
  const Int_t npar = 11;
  const Char_t fn_name[] = "pol10";
  Double_t dummy;
  Int_t Z, A;
  TString mat;
  Double_t par[npar];
  vector<Int_t> vZ, vA;
  vector<TString> vmat;
  vector<Double_t> vpar[npar];
  ifstream fpar(datafile);
  while( !fpar.eof() ){
    fpar >> Z >> A >> mat
	 >> par[0] >> par[1] >> par[2] >> par[3] >> par[4] >> par[5]
	 >> par[6] >> par[7] >> par[8] >> par[9] >> par[10] >> dummy;
    if(fpar.eof())
      break;
    
    vZ.push_back(Z);
    vA.push_back(A);
    vmat.push_back(mat);
    for(Int_t j=0; j<npar; j++)
      vpar[j].push_back(par[j]);
    
  }
  fpar.close();
  
  for(Int_t i=0; i<vZ.size(); i++){
    this->push_back(TF1(Form("%d-%d_%s",vZ.at(i),vA.at(i),vmat.at(i).Data()),
			fn_name,TSrim::log10Emin,TSrim::log10Emax));
    for(Int_t j=0; j<npar; j++){
      this->at(i).SetParameter(j,vpar[j].at(i));
    }
  }
};

Double_t TSrim::Range(Int_t Z, Int_t A, Double_t E, TString mat){
  if( E <= 0.){
    return 0.;
  }else{
    for(Int_t i=0; i<this->size(); i++){
      if( !strcmp(this->at(i).GetName(),Form("%d-%d_%s",Z,A,mat.Data())) )
	return pow(10,this->at(i).Eval(log10(E)));
    }
    cout << "No data in the range list" << endl;
    exit(0);
  }

}

Double_t TSrim::Range(Int_t Z, Int_t A, Double_t E, TString mat,
		      Double_t P, Double_t T){ // For gas density correction
  Double_t fd = TSrim::T0/T*P/TSrim::P1;
  return this->TSrim::Range(Z, A, E, mat)/fd;
}


Double_t TSrim::EnergyNew(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk){
  if( Eold < TSrim::Emin){
    cout << "Energy is too small" << endl;
    return 0.;
  }else if( Eold > TSrim::Emax){
    cout << "Energy is too high" << endl;
    return Eold;
  }else if( Eold <= 0. && thk >= 0.){
    return 0.;
  }else if( thk == 0.){
    return Eold;
  }else{
    for(Int_t i=0; i<this->size(); i++){
      if( !strcmp(this->at(i).GetName(),Form("%d-%d_%s",Z,A,mat.Data())) ){
	Double_t Rold = this->TSrim::Range(Z, A, Eold, mat);
	Double_t Rnew = Rold - thk;

	if(Rnew <= 0.)
	  return 0.;
	else
	  return pow(10,this->at(i).GetX(log10(Rnew),TSrim::log10Emin,TSrim::log10Emax));	
      }
    }
    cout << "No data in the range list" << endl;
    exit(0);
  }
}

Double_t TSrim::EnergyNew(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk,
			  Double_t P, Double_t T){
  Double_t fd = TSrim::T0/T*P/TSrim::P1;
  if( Eold < TSrim::Emin){
    cout << "Energy is too small" << endl;
    return 0.;
  }else if( Eold > TSrim::Emax){
    cout << "Energy is too high" << endl;
    return Eold;
  }else if( Eold <= 0. && thk >= 0.){
    return 0.;
  }else if( thk == 0.){
    return Eold;
  }else{
    for(Int_t i=0; i<this->size(); i++){
      if( !strcmp(this->at(i).GetName(),Form("%d-%d_%s",Z,A,mat.Data())) ){
	Double_t Rold = this->TSrim::Range(Z, A, Eold, mat, P, T);
	Double_t Rnew = Rold - thk;

	if(Rnew <= 0.)
	  return 0.;
	else
	  return pow(10,this->at(i).GetX(log10(Rnew*fd),TSrim::log10Emin,TSrim::log10Emax));	
      }
    }
    cout << "No data in the range list" << endl;
    exit(0);
  }
}


Double_t TSrim::EnergyLoss(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk){
  return Eold - this->TSrim::EnergyNew(Z, A, Eold, mat, thk);
}

Double_t TSrim::EnergyLoss(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk,
			   Double_t P, Double_t T){
  return Eold - this->TSrim::EnergyNew(Z, A, Eold, mat, thk, P, T);
}

// Double_t TSrim::T0(){
//   return TSrim::T0_;
// }

// Double_t TSrim::P1(){
//   return TSrim::P1_;
// }

// Double_t TSrim::Emin(){
//   return TSrim::Emin_;
// }

// Double_t TSrim::Emax(){
//   return TSrim::Emax_;
// }

// Double_t TSrim::log10Emin(){
//   return TSrim::log10Emin_;
// }

// Double_t TSrim::log10Emax(){
//   return TSrim::log10Emax_;
// }
