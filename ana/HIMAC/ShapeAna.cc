
const int nch =8;
const int nsample =100;

void ShapeAna(){

  int runnum =63;
  string rname =Form("../RootFile/run_%d.root",runnum);
  TChain* T = new TChain("T");
  T->Add(rname.c_str());
  int ENum = T->GetEntries();
  cout<<"ENum : "<<ENum<<endl;
  unsigned short adc[nch][nsample];
  float Pedestal[nch];
  T->SetBranchAddress("adc",&adc);
  T->SetBranchAddress("Pedestal",&Pedestal);

  TH1D*  hADC[nch];

  for(int i=0; i<nch; i++){
    hADC[i] = new TH1D(Form("hADC_%d",i),"ADC Sample ",nsample+1,-0.5,nsample+0.5);
  }
  
  for(int iev =0; iev<ENum; iev++){
    T->GetEntry(iev);

    for(int ich =0; ich<nch;ich++){
      for(int isample=0; isample < nsample; isample++){
	hADC[ich] -> Fill(isample,(Pedestal[ich] - (double)(adc[ich][isample]))/(double)(ENum));
      }
    }
  }
  
  // ========== Hist ================ //

  TCanvas* c0 = new TCanvas("c0","c0");
  c0->Divide(2,1);
  c0->cd(1);hADC[0]->Draw("hist");
  c0->cd(2);hADC[1]->Draw("hist");

  string rname_out = Form("RootFiles/shapeAna/run_%d.root",runnum);
  TFile* ofr = new TFile(rname_out.c_str(),"recreate");
  hADC[0]->Write();
  hADC[1]->Write();
  ofr->Write();
  
}
