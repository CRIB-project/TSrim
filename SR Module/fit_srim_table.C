/* #include <TString.h> */
/* #include <TStyle.h> */
/* #include <TApplication.h> */
/* #include <vector> */
/* #include <iostream> */
/* #include <fstream> */
/* #include <string.h> */
/* #include <TGraph.h> */
/* #include <TAxis.h> */
/* #include <TF1.h> */
/* #include <TCanvas.h> */
/* #include <TPad.h> */
/* #include <TSystem.h> */
//#include <TROOT.h>

/* using namespace std; */


// 平均を計算する関数
Double_t calculateMean(const std::vector<Double_t>& data) {
  Double_t sum = 0.0;
  for(Double_t num : data) {
    sum += num;
  }
  return sum / data.size();
}

// 標準偏差を計算する関数
Double_t calculateStdDev(const std::vector<Double_t>& data) {
  Double_t mean = calculateMean(data);
  Double_t standardDeviation = 0.0;
  
  for(Double_t num : data) {
    standardDeviation += pow(num - mean, 2);
  }

  return sqrt(standardDeviation / data.size());
}

/* int main( */
void fit_srim_table(Int_t Z, Int_t A, const Char_t *mat, const Char_t *fin,
		    const Int_t npol, const Char_t *fout, Option_t* option=""){
/*   Int_t Z; */
/*   Int_t A; */
/*   const Char_t *mat; */
/*   const Char_t *fin; */
/*   const Int_t npol = 16; */
/*   const Char_t *fout; */
/*   Option_t* option=""; */
  TString opt = option; // c = canvas, w = write to file 
  opt.ToLower();
  gStyle->SetMarkerStyle(2);
  gStyle->SetMarkerSize(0.5);
  //TApplication theApp("App", NULL, NULL);

  Double_t dummy;
  Char_t char_dummy[10];
  Char_t e_unit[10];
  Char_t r_unit[10];
  Double_t E, dEdx, R;
  vector<Double_t> vE, vR;
  vector<Double_t> vdEdx; //, vdxdE;
  vector<Double_t> vlogE, vlogR;
  ifstream ftable(fin);
  while( !ftable.eof() ){
    ftable >> E >> e_unit >> dummy >> dEdx
	   >> R >> r_unit >> dummy >> char_dummy >> dummy >> char_dummy;
    if(ftable.eof())
      break;
    // Energy
    if(!strcmp(e_unit,"eV"))
      E /= 1000000.;
    else if(!strcmp(e_unit,"keV"))
      E /= 1000.;    
    else if(!strcmp(e_unit,"GeV"))
      E *= 1000.;    
    vE.push_back(E); // in MeV
    if(opt.Contains("c"))
    // Stopping Power
       vdEdx.push_back(dEdx);
    /* vdxdE.push_back(1./dEdx); */
    // RangeX
    if(!strcmp(r_unit,"A"))
      R /=10000000.;
    else if(!strcmp(r_unit,"um"))
      R /= 1000.;
    else if(!strcmp(r_unit,"mm"))
      R *= 1.;
    else if(!strcmp(r_unit,"m"))
      R *= 1000.;
    else if(!strcmp(r_unit,"km"))
      R *= 1000000.;
    else{
      cout << "Range out of range (<A or >km)" << endl;
      return;
    }
      
    vR.push_back(R); // in mm

    vlogE.push_back(log10(E));
    vlogR.push_back(log10(R));
    //cout << E << " " << dEdx << endl;
  }
  ftable.close();

  // Get the minimum and maximum energies
  Double_t logEmin = vlogE.at(0);
  Double_t logEmax = vlogE.back();
  
  TGraph *gr3=new TGraph(vlogE.size(),&(vlogE.at(0)),&(vlogR.at(0)));
  /* gr3->Draw("ap"); */
  TF1 *fn3=new TF1("fn3",Form("pol%d",npol),logEmin,logEmax);
  gr3->Fit(fn3,"","",logEmin,logEmax);

  // Get mean and standard deviation of (range fit - range)/range
  vector<Double_t> vdRR;
  vector<Double_t> vRfit;
  Double_t logR;
  for(Int_t i=0; i<vlogE.size(); i++){
    logR = fn3->Eval(vlogE.at(i)); // Fit function at logE
    vdRR.push_back( (pow(10,logR) - vR.at(i))/vR.at(i) );
    if(opt.Contains("c"))
      vRfit.push_back(pow(10,logR));
  }

  cout << endl;
  cout << "Z = " << Z << ", A = " << A << " fitted"  << endl;
  cout << "Chi2/NDf = " << fn3->GetChisquare()/fn3->GetNDF() << endl;
  cout << "Mean +/- StdDev of (Range fit - Range)/Range: "
       << calculateMean(vdRR) << "+/-" << calculateStdDev(vdRR) << endl;
  cout << endl;
  
  // Write to File option
  if(opt.Contains("w")){
    ofstream fpar(fout, std::ios::app); //appending mode
    fpar << Z << " " << A << " " << mat << " ";
    for(Int_t j=0; j<npol+1; j++){
      fpar << fn3->GetParameter(j) << " ";
    }
    //fpar << fn3->GetChisquare()/fn3->GetNDF();
    fpar << calculateMean(vdRR) << " ";
    fpar << calculateStdDev(vdRR);
    fpar << endl;
    fpar.close();
    cout << Form("Parameters written in %s",fout) << endl;
  }
  
  //// Draw to Canvas option
  if(opt.Contains("c")){
    TCanvas *c1=new TCanvas("c1","c1",0,0,800,800);
    c1->Divide(2,2);
  
    c1->cd(1);
    gPad->SetLogx(); 
    TGraph *gr1=new TGraph(vE.size(),&(vE.at(0)),&(vdEdx.at(0)));
    gr1->SetTitle("Energy (MeV) vs Stopping power (MeV/(mg/cm2))");
    gr1->GetXaxis()->SetRangeUser(pow(10,logEmin),pow(10,logEmax));
    gr1->Draw("ap");

    c1->cd(2);
    TGraph *gr2=new TGraph(vE.size(),&(vE.at(0)),&(vR.at(0)));
    gr2->SetTitle("Energy (MeV) vs Range (mm)");
    gr2->GetXaxis()->SetRangeUser(pow(10,logEmin),pow(10,logEmax));
    gr2->Draw("ap");
    gPad->SetLogx();
    gPad->SetLogy();

    c1->cd(3);
    gr3->SetTitle("Log(Energy (MeV)) vs log(Range (mm))");
    gr3->GetXaxis()->SetRangeUser(logEmin,logEmax);
    gr3->Draw("ap");
  
    c1->cd(4);
    TGraph *gr4=new TGraph(vlogE.size(),&(vlogE.at(0)),&(vdRR.at(0)));
    gr4->SetTitle("Log(Energy) vs (Range fit - Range SRIM)/(Range SRIM)");
    gr4->GetXaxis()->SetRangeUser(logEmin,logEmax);
    gr4->Draw("ap");
    
    TGraph *gr3fit=new TGraph(vE.size(),&(vE.at(0)),&(vRfit.at(0)));
    c1->cd(2);
    gr3fit->SetLineColor(kRed);
    gr3fit->SetLineStyle(1);
    gr3fit->SetLineWidth(2);
    gr3fit->Draw("l");

    gr2->Draw("p");

    c1->Update();
    gSystem->ProcessEvents();
  }
  //theApp.Run();

  if(!opt.Contains("c"))
    gROOT->ProcessLine(".q");

  //  return 0;
}
