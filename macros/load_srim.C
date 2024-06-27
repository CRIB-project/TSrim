//#include "/usr/local/include/Mass.h"
//#include "/usr/local/include/TSrim.h"
#include <Mass.h>
#include <TSrim.h>
//R__LOAD_LIBRARY(libtsrim.so);

const Int_t npol = 16;
TString dir = "/usr/local/share/range_fit";
TString filename = Form("%s/range_fit_pol%d_",dir.Data(),npol);
TString fileext = "txt";

TSrim *load_srim_all(){
  gSystem->Load("libtsrim.so");
  const Int_t nmat = 12;
  
  TSrim *srim0 = new TSrim();
  //const Int_t n = mat.size();
  TSrim *srim[nmat];
  TString mat[nmat] = {
    "si", "mylar", "ch2", "h", "he", "havar",
    "al", "he_90_co2_10", "ar", "cd2", "3he", "mo"
  };
  for(Int_t i=0; i<nmat; i++){
    srim[i]=new TSrim(mat[i].Data(), npol, 
		      Form("%s%s.%s",filename.Data(),
			   mat[i].Data(),fileext.Data()));
    cout << Form("%s list loaded.",mat[i].Data()) << endl;
    srim0->TSrim::insert(srim0->end(), srim[i]->begin(), srim[i]->end());
  }
  return srim0;
}


TSrim *load_srim(vector<TString> mat){
  gSystem->Load("libtsrim.so");
  const Int_t nmat = mat.size();
  
  TSrim *srim0 = new TSrim();
  TSrim *srim[nmat];
  for(Int_t i=0; i<nmat; i++){
    srim[i]=new TSrim(mat.at(i).Data(), npol, 
		      Form("%s%s.%s",filename.Data(),
			   mat[i].Data(),fileext.Data()));
    cout << Form("%s list loaded.",mat[i].Data()) << endl;
    srim0->TSrim::insert(srim0->end(), srim[i]->begin(), srim[i]->end());
  }
  return srim0;
}


TSrim *load_srim(vector<TString> mat,
		 Int_t Zmin, Int_t Amin, Int_t Zmax, Int_t Amax){
  gSystem->Load("libtsrim.so");
  const Int_t nmat = mat.size();
  
  TSrim *srim0 = new TSrim();
  TSrim *srim[nmat];
  for(Int_t i=0; i<nmat; i++){
    srim[i]=new TSrim(mat.at(i).Data(), npol, 
		      Form("%s%s.%s",filename.Data(),
			   mat[i].Data(),fileext.Data()),
		      Zmin,Amin,Zmax,Amax);
    cout << Form("%s list loaded for %d%s - %d%s.",
		 mat[i].Data(), Amin,GetEl(Zmin), Amax,GetEl(Zmax)) << endl;
    srim0->TSrim::insert(srim0->end(), srim[i]->begin(), srim[i]->end());
  }
  return srim0;
}

//TSrim *srim=load_srim_all();

