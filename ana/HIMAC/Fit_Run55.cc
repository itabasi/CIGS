//Study of the noise

const int nch=8;
void Fit_Run55(){

  string rname="../RootFile/itabashi/run_55.root";

  TChain* T = new TChain("T");
  T->Add(rname.c_str());
  int ENum = T->GetEntries();
  cout<<"ENum "<<ENum<<endl;
  float IADC[nch];
  T->SetBranchAddress("IntegratedADC",IADC);
  TH1D* hADC[nch];
  TGraphErrors* gCut[nch];
  TGraphErrors* gPeak[nch];
  TGraphErrors* gPed[nch];  
  for(int i=0;i<nch;i++){
    hADC[i] = new TH1D(Form("hADC_%d",i),"",250,-100,400);
    gCut[i] = new TGraphErrors();
    gCut[i] -> SetName(Form("gCut_%d",i));
    gCut[i] ->SetMarkerStyle(20);

    gPeak[i] = new TGraphErrors();
    gPeak[i] -> SetName(Form("gPeak_%d",i));
    gPeak[i] ->SetMarkerStyle(20);    
    gPeak[i] ->SetMarkerColor(2);
    gPed[i] = new TGraphErrors();
    gPed[i] -> SetName(Form("gPed_%d",i));
    gPed[i] ->SetMarkerStyle(20);
    gPed[i] ->SetMarkerColor(4);
  }
  
  for(int iev=0; iev<ENum;iev++){

    T->GetEntry(iev);
    for(int i=0;i<nch;i++){
      hADC[i]->Fill(IADC[i]);
    }


  }


  //======== Fitting =========//
  TF1* fPeak = new TF1("fPeak","gausn(0)",-100,400);
  TF1* fPed = new TF1("fPed","gausn(0)",-100,400);

  fPeak->SetParameters(2.19046e+02,2.19632e+02,3.29554e+01);
  fPed->SetParameters(7.46905e+02 ,-1.85442e-01,1.14278e+01);

  hADC[0]->Fit("fPeak","","",100,400);
  hADC[0]->Fit("fPed","","",-100,50);

  double Nfit = fPeak->GetParameter(0) + fPed->GetParameter(0);
  double Nhist = hADC[0]->Integral(-100,400);
  cout<<"Nfit "<<Nfit <<" Nhist "<<Nhist*2<<endl;


  // =========== TGraphErrors =========== //
  int nbin=hADC[0]->GetXaxis() ->GetNbins();
  double Npeak = fPeak->Integral(-100,400);
  double Nped  = fPed->Integral(-100,400);
  for(int i=0; i<nbin;i++){
    double x = hADC[0] ->GetBinCenter(i);
    double y = hADC[0] ->GetBinContent(i);
    double yPed = fPed -> Integral(x,400);
    double yPeak  = fPeak  -> Integral(x,400);
    gPed[0]->SetPoint(i,x,yPed/Nped*100.);
    gPeak[0]->SetPoint(i,x,yPeak/Npeak*100.);
  }

  
  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  hADC[0]->Draw();
  fPeak->SetLineColor(2);
  fPeak->SetFillColor(2);
  fPeak->SetFillStyle(3004);
  fPed->SetLineColor(4);
  fPed->SetFillColor(4);
  fPed->SetFillStyle(3004);

  fPeak->Draw("same");
  fPed->Draw("same");

  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();

  gPeak[0]->Draw("AP");
  gPed[0]->Draw("P");


  
}
