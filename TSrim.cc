#include "TSrim.h"
#include <TF1.h>
#include <TString.h>
#include <string.h>

#include <stdlib.h>
#include <fstream>

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <Mass.h>
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
  Parameter file format: z a mat par0 - par10 Mean(fit dev) Std dev(fit dev)
*/

ClassImp(TSrim);
///////////////////////////////////////////////////////////////////////////////
TSrim::TSrim(){};
TSrim::TSrim(const char *name, const Int_t npol, const char *datafile){
  TString fn_name = Form("pol%d",npol);
  Double_t dummy;
  Int_t Z, A;
  TString mat;
  Double_t par[npol+1];
  vector<Int_t> vZ, vA;
  vector<TString> vmat;
  vector<Double_t> vpar[npol+1];
  ifstream fpar(datafile);
  while( !fpar.eof() ){
    fpar >> Z >> A >> mat;
    for (Int_t j=0; j<npol+1; j++)
      fpar >> par[j];
    fpar >> dummy >> dummy;
    if(fpar.eof())
      break;    
    vZ.push_back(Z);
    vA.push_back(A);
    vmat.push_back(mat);
    for(Int_t j=0; j<npol+1; j++)
      vpar[j].push_back(par[j]);    
  }
  fpar.close();  
  if(vZ.size()==0){
    cout << "No isotope data loaded." << endl;    
  }

  for(Int_t i=0; i<vZ.size(); i++){
    this->push_back(TF1(Form("%d-%d_%s",vZ.at(i),vA.at(i),vmat.at(i).Data()),
			fn_name.Data(),TSrim::log10Emin,TSrim::log10Emaxpu*Mass(Z,A)));
    for(Int_t j=0; j<npol+1; j++){
      this->at(i).SetParameter(j,vpar[j].at(i));
    }
  }
  // TSrim(name, npol, datafile, 1, 1, 20, 40);
};
TSrim::TSrim(const char *name, const Int_t npol, const char *datafile,
	     Int_t Z, Int_t A){
  TString fn_name = Form("pol%d",npol);
  Double_t dummy;
  Int_t Zdat, Adat;
  TString mat;
  Double_t par[npol+1];
  vector<Int_t> vZ, vA;
  vector<TString> vmat;
  vector<Double_t> vpar[npol+1];
  ifstream fpar(datafile);
  while( !fpar.eof() ){
    fpar >> Zdat >> Adat >> mat;
    for (Int_t j=0; j<npol+1; j++)
      fpar >> par[j];
    fpar >> dummy >> dummy;
    if(fpar.eof()){
      break;
    }
    if(Zdat==Z && Adat==A){      
      vZ.push_back(Z);
      vA.push_back(A);
      vmat.push_back(mat);
      for(Int_t j=0; j<npol+1; j++)
	vpar[j].push_back(par[j]);
      break;
    }    
  }
  fpar.close();
  if(vZ.size()==0){
    cout << "No isotope data loaded." << endl;    
  }

  for(Int_t i=0; i<vZ.size(); i++){
    this->push_back(TF1(Form("%d-%d_%s",vZ.at(i),vA.at(i),vmat.at(i).Data()),
			fn_name.Data(),TSrim::log10Emin,TSrim::log10Emaxpu*Mass(Z,A)));
    for(Int_t j=0; j<npol+1; j++){
      this->at(i).SetParameter(j,vpar[j].at(i));
    }
  }
};
TSrim::TSrim(const char *name, const Int_t npol, const char *datafile,
	     Int_t Zmin, Int_t Amin, Int_t Zmax, Int_t Amax){
  TString fn_name = Form("pol%d",npol);
  Double_t dummy;
  Int_t Z, A;
  TString mat;
  Double_t par[npol+1];
  vector<Int_t> vZ, vA;
  vector<TString> vmat;
  vector<Double_t> vpar[npol+1];
  ifstream fpar(datafile);
  while( !fpar.eof() ){
    fpar >> Z >> A >> mat;
    for (Int_t j=0; j<npol+1; j++)
      fpar >> par[j];
    fpar >> dummy >> dummy;
    if(fpar.eof())
      break;
    if(Z>=Zmin && A>=Amin && Z<=Zmax && A<=Amax){      
      vZ.push_back(Z);
      vA.push_back(A);
      vmat.push_back(mat);
      for(Int_t j=0; j<npol+1; j++)
	vpar[j].push_back(par[j]);
    }    
  }
  fpar.close();  
  if(vZ.size()==0){
    cout << "No isotope data loaded." << endl;    
  }

  for(Int_t i=0; i<vZ.size(); i++){
    this->push_back(TF1(Form("%d-%d_%s",vZ.at(i),vA.at(i),vmat.at(i).Data()),
			fn_name.Data(),TSrim::log10Emin,TSrim::log10Emaxpu*Mass(Z,A)));
    for(Int_t j=0; j<npol+1; j++){
      this->at(i).SetParameter(j,vpar[j].at(i));
    }
  }
};
///////////////////////////////////////////////////////////////////////////////
Double_t TSrim::Range(Int_t Z, Int_t A, Double_t E, TString mat){
  if( E <= 0.){
    return 0.;
  }else{
    for(Int_t i=0; i<this->size(); i++){
      if( !strcmp(this->at(i).GetName(),Form("%d-%d_%s",Z,A,mat.Data())) )
	return pow(10,this->at(i).Eval(log10(E)));
    }
    cout << "No data in the range list" << endl;
    //exit(0);
    return TSrim::dummy;
  }

}
Double_t TSrim::Range(Int_t Z, Int_t A, Double_t E, TString mat,
		      Double_t P, Double_t T){ // For gas density correction
  Double_t fd = TSrim::T0/T*P/TSrim::P1;
  return this->TSrim::Range(Z, A, E, mat)/fd;
}
Double_t TSrim::RangePu(Int_t Z, Int_t A, Double_t Epu, TString mat){
  return this->TSrim::Range(Z, A, Epu*Mass(Z,A), mat); 
}
Double_t TSrim::RangePu(Int_t Z, Int_t A, Double_t Epu, TString mat,
			Double_t P, Double_t T){
  return this->TSrim::Range(Z, A, Epu*Mass(Z,A), mat, P, T); 
}
///////////////////////////////////////////////////////////////////////////////
Double_t TSrim::EnergyNew(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk){
  return this->TSrim::EnergyNew(Z, A, Eold, mat, thk, TSrim::P1, TSrim::T0);
  // if( Eold < TSrim::Emin){
  //   cout << "Energy is too small" << endl;
  //   return 0.;
  // }else if( Eold > TSrim::Emaxpu*Mass(Z,A)){
  //   cout << "Energy is too high" << endl;
  //   return Eold;
  // }else if( Eold <= 0. && thk >= 0.){
  //   return 0.;
  // }else if( thk == 0.){
  //   return Eold;
  // }else{
  //   for(Int_t i=0; i<this->size(); i++){
  //     if( !strcmp(this->at(i).GetName(),Form("%d-%d_%s",Z,A,mat.Data())) ){
  // 	Double_t Rold = this->TSrim::Range(Z, A, Eold, mat);
  // 	Double_t Rnew = Rold - thk;

  // 	if(Rnew <= 0.)
  // 	  return 0.;
  // 	else
  // 	  return pow(10,this->at(i).GetX(log10(Rnew),TSrim::log10Emin,TSrim::log10Emaxpu*Mass(Z,A)));	
  //     }
  //   }
  //   cout << "No data in the range list" << endl;
  //   //exit(0);
  //   return TSrim::dummy;
  // }
}
Double_t TSrim::EnergyNew(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk,
			  Double_t P, Double_t T){
  Double_t fd = TSrim::T0/T*P/TSrim::P1;
  if( Eold < TSrim::Emin){
    cout << "Energy is too small" << endl;
    return 0.;
  }else if( Eold > TSrim::Emaxpu*Mass(Z,A)){
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
	  return pow(10,this->at(i).GetX(log10(Rnew*fd),TSrim::log10Emin,TSrim::log10Emaxpu*Mass(Z,A)));	
      }
    }
    cout << "No data in the range list" << endl;
    //exit(0);
    return TSrim::dummy;
  }
}
Double_t TSrim::ENew(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk){
  return this->TSrim::EnergyNew(Z, A, Eold, mat, thk);
}
Double_t TSrim::ENew(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk,
			  Double_t P, Double_t T){
  return this->TSrim::EnergyNew(Z, A, Eold, mat, thk, P, T);
}
Double_t TSrim::EnergyNewPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat, Double_t thk){
  return this->TSrim::EnergyNew(Z, A, Eoldpu*Mass(Z,A), mat, thk)/Mass(Z,A);
}
Double_t TSrim::EnergyNewPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat, Double_t thk,
	    Double_t P, Double_t T){
  return this->TSrim::EnergyNew(Z, A, Eoldpu*Mass(Z,A), mat, thk, P, T)/Mass(Z,A);
}
Double_t TSrim::ENewPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat, Double_t thk){
  return this->TSrim::EnergyNewPu(Z, A, Eoldpu, mat, thk);
}
Double_t TSrim::ENewPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat, Double_t thk,
	    Double_t P, Double_t T){
  return this->TSrim::EnergyNewPu(Z, A, Eoldpu, mat, thk, P, T);
}
///////////////////////////////////////////////////////////////////////////////
Double_t TSrim::EnergyLoss(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk){
  return Eold - this->TSrim::EnergyNew(Z, A, Eold, mat, thk);
}
Double_t TSrim::EnergyLoss(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk,
			   Double_t P, Double_t T){
  return Eold - this->TSrim::EnergyNew(Z, A, Eold, mat, thk, P, T);
}
Double_t TSrim::ELoss(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk){
  return this->TSrim::EnergyLoss(Z, A, Eold, mat, thk);
}
Double_t TSrim::ELoss(Int_t Z, Int_t A, Double_t Eold, TString mat, Double_t thk,
		      Double_t P, Double_t T){
  return this->TSrim::EnergyLoss(Z, A, Eold, mat, thk, P, T);
}
Double_t TSrim::EnergyLossPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat, Double_t thk){
  return Eoldpu - this->TSrim::EnergyNewPu(Z, A, Eoldpu, mat, thk);
}
Double_t TSrim::EnergyLossPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat, Double_t thk,
			   Double_t P, Double_t T){
  return Eoldpu - this->TSrim::EnergyNewPu(Z, A, Eoldpu, mat, thk, P, T);
}
Double_t TSrim::ELossPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat, Double_t thk){
  return this->TSrim::EnergyLossPu(Z, A, Eoldpu, mat, thk);
}
Double_t TSrim::ELossPu(Int_t Z, Int_t A, Double_t Eoldpu, TString mat, Double_t thk,
			Double_t P, Double_t T){
  return this->TSrim::EnergyLossPu(Z, A, Eoldpu, mat, thk, P, T);
}
///////////////////////////////////////////////////////////////////////////////
void TSrim::ShowMatList(){
  TString matlist[100];
  TString mat;
  Int_t k=0;
  for(Int_t i=0; i<this->size()-20; i++){
    Bool_t exitFlag = false;
    mat = this->at(i).GetName();
    mat = mat(mat.First("_")+1,mat.Length()-mat.First("_"));
    if(i==0){
      matlist[0] = mat;
      k++;
    }else{
      for(Int_t j=0; j<k+1; j++){
	if(matlist[j]==mat){
	  exitFlag=true;
	  break;
	}
      }
      if(!exitFlag){
	matlist[k] = mat;
	k++;
      }
    }
  }  
  Int_t kmax = k;  
  for(k=0; k<kmax; k++){
    if(k>100){
      cout << "Too many materials" << endl;
      break;
    }
    cout << matlist[k].Data() << endl;
  }
}

void TSrim::ShowMatListZAN(){
  TString matlist[100];
  TString mat;
  Int_t k=0;
  Int_t Z, A, N;
  Int_t Zmin, Amin, Nmin;
  Int_t Zmax, Amax, Nmax;
  for(Int_t i=0; i<this->size(); i++){
    Bool_t exitFlag = false;
    mat = this->at(i).GetName();
    Z = stoi(mat(0, mat.First("-")-1));
    A = stoi(mat(mat.First("-")+1, mat.First("_")-1));
    mat = mat(mat.First("_")+1, mat.Length()-mat.First("_"));
    if(i==0){
      Zmin = Zmax = Z;
      Amin = Amax = A;
      Nmin = Nmax = A-Z;
      matlist[0] = mat;
      k++;
    }else{
      if(Z<Zmin)
	Zmin = Z;
      if(Z>Zmax)
	Zmax = Z;
      if(A<Amin)
	Amin = A;
      if(A>Amax)
	Amax = A;
      if(N<Nmin)
	Nmin = N;
      if(N>Nmax)
	Nmax = N;
      for(Int_t j=0; j<k+1; j++){
	if(matlist[j]==mat){
	  exitFlag=true;
	  break;
	}
      }
      if(!exitFlag){
	matlist[k] = mat;
	k++;
      }
    }
  }  
  Int_t kmax = k;  
  for(k=0; k<kmax; k++){
    if(k>100){
      cout << "Too many materials" << endl;
      break;
    }
    cout << Form("%s, Z: %d-%d, N: %d-%d, A: %d-%d",
		 matlist[k].Data(), Zmin, Zmax, Nmin, Nmax, Amin, Amax)
	 << endl;
  }
}

