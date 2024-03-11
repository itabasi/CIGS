// Study for CIGS thickness dependence 2um and 5um


const int nch =8;
extern double GetCharge(double x, double ich, double iconfig);
void CIGS_thick_dep(){

  gStyle->SetPalette(kRainBow);

  string rname_2um ="../RootFile/itabashi/run_32.root";
  string rname_5um ="../RootFile/itabashi/run_61.root";


  TChain* T[2];
  T[0] = new TChain("T");
  T[0] -> Add(rname_2um.c_str());
  T[1] = new TChain("T");
  T[1] -> Add(rname_5um.c_str());

  float IADC_2um[nch];
  float IADC_5um[nch];

  T[0]->SetBranchAddress("IntegratedADC",&IADC_2um);
  T[1]->SetBranchAddress("IntegratedADC",&IADC_5um);
  int ENum_2um = T[0]->GetEntries();
  int ENum_5um = T[1]->GetEntries();

  TH1D* hADC[2][nch];
  TH1D* hCharge[2][nch];

  for(int i=0;i<2;i++){
    for(int j=0;j<nch;j++){
      hADC[i][j] = new TH1D(Form("hADC_%d_%d",i,j),"",400,-200,2000);
      hCharge[i][j] = new TH1D(Form("hCharge_%d_%d",i,j),"",400,-0.1,0.5);
      //      hCharge[i][j] ->SetLineColor(i+2);
      //      hCharge[i][j] ->SetFillColor(i+2);
      //      hCharge[i][j] ->SetFillStyle(3003);
    }
  }



  // --------- CIGS 2um ---------------//
  for(int iev=0; iev<ENum_2um; iev++){
    T[0]->GetEntry(iev);
    for(int i=0;i<2;i++){
      double q = GetCharge(IADC_2um[i],i,1);
    hADC[0][i] ->Fill(IADC_2um[i]);
    hCharge[0][i] ->Fill(q);
    }
  }


  // --------- CIGS 5um ---------------//
  for(int iev=0; iev<ENum_5um; iev++){
    T[1]->GetEntry(iev);
    for(int i=0;i<2;i++){
      double q = GetCharge(IADC_5um[i],i,2);
    hADC[1][i] ->Fill(IADC_5um[i]);
    hCharge[1][i] ->Fill(q);
    }
  }



  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  gStyle->SetOptStat(0000);
  hADC[0][0]->Draw();
  hADC[1][0]->Draw("same");
  //  hADC[0][1]->Draw("same");
  //  hADC[1][1]->Draw("same");

  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  gPad->SetLogy(1);
  hCharge[0][0]->SetTitle("; Charge [pC]; Counts/1.5 pC");
  hCharge[0][0]->GetXaxis()->SetTitleSize(0.04);
  hCharge[0][0]->GetXaxis()->SetLabelSize(0.04);
  hCharge[0][0]->GetYaxis()->SetTitleSize(0.04);
  hCharge[0][0]->GetYaxis()->SetLabelSize(0.04);
  hCharge[0][0]->SetLineColor(1);
  hCharge[0][0]->SetLineWidth(2);
  hCharge[1][0]->SetLineColor(2);
  hCharge[1][0]->SetLineWidth(2);
  
  hCharge[0][0]->Draw("PLC PMC");
  hCharge[1][0]->Draw("PLC PMC same");

  TLegend*leg = new TLegend(0.6,0.60,0.9,0.90,"","NDC");

  leg->AddEntry(hCharge[0][0], "2#mum thickness CIGS","l");
  leg->AddEntry(hCharge[1][0], "5#mum thickness CIGS","l");
  leg->Draw();

  
}


double GetCharge(double x, double ich, double iconfig){

  double a,b; // y = a*x +b ;
  double par[2];
  par[0] = ich; // Channel param 0=CH0 or 1=CH1
  par[1] = iconfig; // Number of config file : 1 = CIGS 2um , 2= CIGS 5um
  if(par[0]==0 && par[1]==1){a = 2894.8; b = 17.4009;} // config 1 CH0
  else if(par[0]==0 && par[1]==2){a = 2790.5; b = 24.6268;} // config 1 CH1
  else if(par[0]==1 && par[1]==1){a = 1113.78; b = 11.8572;} // config 2 CH0
  else if(par[0]==1 && par[1]==2){a = 1076.6; b = 3.30293;} // config 2 CH1
  else {a=0; b=0;}


  double y = (x-b)/a ;
  return y;

}
