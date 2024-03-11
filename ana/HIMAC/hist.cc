
const int nch =8;

void hist(){


  string runnum ="38";
  string rname = Form("../RootFile/itabashi/run_%s.root",runnum.c_str());
  //    string rname = Form("../RootFile/run_%s.root",runnum.c_str());
  
  TChain * T = new TChain("T");
  T->Add(rname.c_str());
  int ENum = T->GetEntries();
  cout<<"GetEntries : "<<ENum<<endl;
  float IntegratedADC[nch];
  T->SetBranchAddress("IntegratedADC",&IntegratedADC);
  float PeakHeight[nch];
  T->SetBranchAddress("PeakHeight",&PeakHeight);
  
  TH1D* hIntegratedADC[8];
  TH1D* hPeakHeight[8];
  TH1D* hPedestal[8];
  for(int i=0;i<nch ; i++){
    hIntegratedADC[i] = new TH1D(Form("hIntegratedADC_%d",i),"Integrated ADC; ADC [ch] ; Counts",1000,0,1000);
    hPeakHeight[i] = new TH1D(Form("hPeakHeight_%d",i),"PeakHeigth ADC; ADC [ch] ; Counts",1000,0,1000);
    hPedestal[i] = new TH1D(Form("hPedestal_%d",i),"Integral ADC (Pedestal); ADC [ch] ; Counts",1000,-100,100);
  }

  
  for(int iev =0; iev<ENum; iev++){
    T->GetEntry(iev);
    for(int i=0;i<nch; i++){
      if(IntegratedADC[i]>100.){
      hIntegratedADC[i] ->Fill(IntegratedADC[i]);
      }else{
	hPedestal[i] ->Fill(IntegratedADC[i]);
      }
      if(PeakHeight[i]<500){
	hPeakHeight[i] ->Fill(PeakHeight[i]);
      }
    }
  }


  string rname2=Form("RootFiles/itabashi/ana/run_%s.root",runnum.c_str());
  TFile* ofr = new TFile(rname2.c_str(),"recreate");
  cout<<"Output File : "<<rname2<<endl;
  for(int i=0;i<nch;i++){
    hIntegratedADC[i]->Write();
    hPeakHeight[i]->Write();
    hPedestal[i]->Write();
  }
  
}
