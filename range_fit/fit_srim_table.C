void fit_srim_table(Int_t Z, Int_t A, const Char_t *mat, const Char_t *fin, const Char_t *fout){
  const Double_t d = 1.397; // g/cm^3
  //  const Double_t 

  Double_t dummy;
  Char_t char_dummy[10];
  Char_t e_unit[10];
  Char_t r_unit[10];
  Double_t E, dEdx, R;
  vector<Double_t> vE, vdEdx, vR, vdxdE;
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
    // Stopping Power
    vdEdx.push_back(dEdx);
    vdxdE.push_back(1./dEdx);
    // RangeX
    if(!strcmp(r_unit,"A"))
      R /=10000000.;
    else if(!strcmp(r_unit,"um"))
      R /= 1000.;
    else if(!strcmp(r_unit,"m"))
      R *= 1000.;
    vR.push_back(R); // in mm

    vlogE.push_back(log10(E));
    vlogR.push_back(log10(R));
    //cout << E << " " << dEdx << endl;
  }
  ftable.close();
  
  TCanvas *c1=new TCanvas("c1","c1",0,0,800,800);
  c1->Divide(2,2);
  gStyle->SetMarkerStyle(6);
  
  c1->cd(1);
  gPad->SetLogx(); 
  TGraph *gr1=new TGraph(vE.size(),&(vE.at(0)),&(vdEdx.at(0)));
  gr1->Draw("ap");

  c1->cd(2);
  TGraph *gr2=new TGraph(vE.size(),&(vE.at(0)),&(vR.at(0)));
  gr2->Draw("ap");
  gPad->SetLogx();
  gPad->SetLogy();

  c1->cd(3);
  TGraph *gr3=new TGraph(vlogE.size(),&(vlogE.at(0)),&(vlogR.at(0)));
  gr3->Draw("ap");
  TF1 *fn3=new TF1("fn3","pol10",-3,2);
  //TF1 *fn3=new TF1("fn3","pol3",-3,2);
  gr3->Fit(fn3);
  cout << Z << " " << A << " " << mat << " ";
  for(Int_t j=0; j<11; j++){
    cout << fn3->GetParameter(j) << " ";
  }
  cout << fn3->GetChisquare()/fn3->GetNDF();
  cout << endl;
  
  Double_t dlogR, logR;
  vector<Double_t> vdR, vdlogR, vRfit;
  for(Int_t i=0; i<vlogE.size(); i++){
    logR = fn3->Eval(vlogE.at(i));
    dlogR = vlogR.at(i)/logR;
    vdlogR.push_back(dlogR);
    vRfit.push_back(pow(10,logR));
    //vdR.push_back(pow(10,dlogR));
    vdR.push_back(vR.at(i) - dlogR*pow(10,fn3->Eval(vlogE.at(i))));
  }
  c1->cd(4);
  TGraph *gr4=new TGraph(vlogE.size(),&(vlogE.at(0)),&(vdlogR.at(0)));
  //TGraph *gr4=new TGraph(vE.size(),&(vE.at(0)),&(vdR.at(0)));
  gr4->Draw("ap");

  TGraph *gr3fit=new TGraph(vE.size(),&(vE.at(0)),&(vRfit.at(0)));
  c1->cd(2);
  gr3fit->SetLineColor(kRed);
  gr3fit->SetLineStyle(1);
  gr3fit->SetLineWidth(2);
  gr3fit->Draw("l");

  gr2->Draw("p");

  // Range at E = 54.73 MeV (mm)
  /* Double_t Rold = pow(10,fn3->Eval(log10(54.73))); */
  /* cout << Rold << endl; */
  /* // Energy loss in thk = 100 mm */
  /* Double_t Rnew = Rold - 100.; */
  /* Double_t Enew = pow(10,fn3->GetX(log10(Rnew))); */
  /* cout << Enew << endl; */

  //E_srim(14,25.992,54.73,Form("he"),294.5,300,100);

  c1->Update();
  gSystem->ProcessEvents();
  //getchar();

  cout << Form("Write parameters to the output file %s? [y/n]",
	       fout) << endl;
  TString answer = "n";
  cin >> answer;
  if(answer == "y"){
    ofstream fpar(fout, std::ios::app); //appending mode
    fpar << Z << " " << A << " " << mat << " ";
    for(Int_t j=0; j<11; j++){
      fpar << fn3->GetParameter(j) << " ";
    }
    fpar << fn3->GetChisquare()/fn3->GetNDF();
    fpar << endl;
    fpar.close();
    /* ofstream flog(fin, std::ios::app); //appending mode */
    /* flog << ctime(&t) << endl; */
    /* flog << Z << " " << A << " " << mat << " "; */
    /* for(Int_t j=0; j<11; j++){ */
    /*   flog << fn3->GetParameter(j) << " "; */
    /* } */
    /* flog << fn3->GetChisquare()/fn3->GetNDF(); */
    /* flog << endl; */
    /* flog.close(); */
  }
  else{
    cout << "No parameters written in." << endl; 
  };
  
  time_t t = time(NULL);
  
  gROOT->ProcessLine(".q");
}
