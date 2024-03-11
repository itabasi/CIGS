const int nch =8;




void ADC_Vdep2(){

  const int nfile =7;
  //  const int nfile =3;
  string dir ="../../rootfiles/";
  string fname[nfile];
  int runID[nfile];
  double HV[nfile];
  double offset = -1.65;

  // -------------- CIGS ID21 --------------- //
  runID[0] = 49; HV[0] =1.52+offset;
  runID[1] = 50; HV[1] =0 +offset;
  runID[2] = 51; HV[2] = 1.65 + offset;
  runID[3] = 46; HV[3] = 1.15 + offset;
  runID[4] = 53; HV[4] = 0. +offset;
  runID[5] = 54; HV[5] = 1.52 + offset;
  runID[6] = 58; HV[6] = 0.65 + offset;

  //  runID[0] = 49; HV[0] =1.52+offset;
  //  runID[1] = 50; HV[1] =0 +offset;
  //  runID[2] = 51; HV[2] = 1.65 + offset;
  //  runID[3] = 53; HV[3] = 0. +offset;
  //  runID[4] = 54; HV[4] = 1.52 + offset;
  // ---------------------------------------- //


  // ------------- Ga-rich CIGS --------------- //
  //  runID[0] = 47; HV[0] =0+offset;
  //  runID[1] = 55; HV[1] = -0.35 + offset;
  //  runID[1] = 56; HV[1] = 0.65 + offset;
  //  runID[2] = 57; HV[2] = 1.15 + offset;
  

  
  TFile* f[nfile];
  TTree* T[nfile];
  TF1* ffit[nfile];
  float sumADC[nch];
  int  pulse[nch];
  TH1F* hADC[nch];
  double mean[nch];
  double mean_err[nch];
  double sigma[nch];
  float height[nch];
  TGraphErrors* gADC_V = new TGraphErrors();
  gADC_V ->SetMarkerStyle(22);
  gADC_V ->SetMarkerColor(2);
  gADC_V->SetName("gADC_V");
  for(int i = 0; i<nfile; i++){
    hADC[i] = new TH1F(Form("hADC_%d",i),"",200,-100,1500);
    fname[i] = dir +Form("run_%03d.root",runID[i]);
    cout<<"i "<<i<<" file "<<fname[i]<<endl;
    f[i] = new TFile(fname[i].c_str(),"read");
    //    T[i] = new TTree("T","T");
    T[i] = (TTree*)f[i] ->Get("T");
    T[i] ->SetBranchAddress("IntegratedADC",&sumADC);
    T[i] ->SetBranchAddress("AfterPulse",&pulse);
    T[i] ->SetBranchAddress("PeakHeight",&height);
    int ENum = T[i]->GetEntries();

    for(int k =0; k<ENum;k++){
      T[i]->GetEntry(k);
      //      if(pulse[0] <0 ){
      if(height[0]<420 ){
	hADC[i] ->Fill(sumADC[0]);	
      }
    } 

    ffit[i] = new TF1(Form("ffit_%d",i),"gausn(0)",-100,1500);
    double ymax = hADC[i] ->GetBinContent(hADC[i]->GetMaximumBin());
    mean[i] = hADC[i] ->GetBinCenter(hADC[i]->GetMaximumBin());
    sigma[i] = 100;
    //    ffit[i]->SetParameters(ymax,mean,sigma);
    ffit[i]->SetParameter(0,ymax);
    ffit[i]->SetParameter(1,mean[i]);
    ffit[i]->SetParameter(2,sigma[i]);
    //    hADC[i]->Fit(Form("ffit_%d",i),"QR","QR",mean[i]-3*sigma[i], mean[i]+3*sigma[i]);
    hADC[i]->Fit(Form("ffit_%d",i),"QR","QR",mean[i]-sigma[i], mean[i]+sigma[i]);
    mean[i] = ffit[i]->GetParameter(1);
    mean_err[i] = ffit[i]->GetParError(1);
    sigma[i] = ffit[i]->GetParameter(2);

    gADC_V ->SetPoint(i,HV[i],mean[i]);
    //    gADC_V ->SetPointError(i,0,sigma[i]);
        gADC_V ->SetPointError(i,0,mean_err[i]);
    
  }

  // ------------- VW curve ------------ //

  string vwfile = dir +"ana/CV/CIGS_ID21_CH6_20240124_vw.root";
  TFile* fw = new TFile(vwfile.c_str());
  TGraphErrors* gW6 = (TGraphErrors*)fw ->Get("gW6");
  TGraphErrors* gW = new TGraphErrors();
  TGraphErrors* gADC_W = new TGraphErrors();
  gADC_W->SetName("gADC_W");
  double W[nfile];
  for(int i =0; i<nfile; i++){
    W[i] = gW6 ->Eval(HV[i]);
    gW ->SetPoint(i,HV[i],W[i]);
    gADC_W ->SetPoint(i,HV[i],mean[i]/W[i]);
    gADC_W ->SetPointError(i,0,sigma[i]/W[i]);
  }

  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  gADC_V ->Draw("AP");

  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  gADC_W ->Draw("AP");

  TCanvas* c2 = new TCanvas("c2","c2");
  c2->cd();
  hADC[6]->Draw();


  

  //string name = "CIGSID21CH6";
  string name = "CIGSID33CH6";
  string frname =   Form("../../rootfiles/ana/ADC/%s_vdep.root",name.c_str());
  TFile* ofr = new TFile(frname.c_str(),"recreate");

  gADC_V->Write();
  gADC_W->Write();
  


  
  
}
