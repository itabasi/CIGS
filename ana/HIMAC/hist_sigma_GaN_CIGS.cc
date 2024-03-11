const int nfile =2;
const int nch   =16;

void hist_sigma_GaN_CIGS(){

  string rname_CIGS2um= "../RootFile/itabashi/run_38.root"; // 2V
  string rname_CIGS5um= "../RootFile/itabashi/run_61.root"; // 2V
  string rname_GaN    = "/home/togawa/work/SENSOR/RAPID_16ch_16/Ana/run_30.root" // 4V

  string rname[nfile];
  rname[0]=rname_CIGS2um;
  rname[1]=rname_CIGS5um;
  //  rname[2]=rname_GaN;

  TF1* f[nfile];
  bool CIGS_flag[nfile];
  TChain* T[nfile];
  //  float IntegratedADC[nfile][nch];

  int ENum[nfile];

  TH1F* hADC[nfile][nch];
  double min_adc = 0.;
  double max_adc = 2000;
  int bin_adc=(int)(max_adc-min_adc);
  
  for(int i=0; i<nfile; i++){

    T[i] = new TChain("T");
    T[i] ->Add(rname[i].c_str());
    float IntegratedADC[nch];
    T[i] -> SetBranchAddress("IntegratedADC",IntegratedADC);
    //    bool AfterPulse[nch];
    //    T[i] -> SetBranchAddress("AfterPulse",AfterPulse);
    ENum[i] = T[i]->GetEntries();
    for(int ich=0; ich<nch ; ich++){
      hADC[i][ich] = new TH1F(Form("hADC_%d_%d",i,ich),"",bin_adc,min_adc,max_adc);
    }
    
    for(int iev=0; iev<ENum[i]; iev++){
      T[i]->GetEntry(iev);
      for(int ich=0; ich<nch ; ich++){
	//	if(AfterPulse[ich]==0)
	hADC[i][ich] ->Fill(IntegratedADC[ich]);
      }
    }
  }


  int ncanvas=nfile;
  TCanvas* c[ncanvas];

  for(int ic=0; ic<ncanvas; ic++){

    c[ic] = new TCanvas(Form("c%d",ic),Form("c%d",ic));
    c[ic]->Divide(2,1);
    c[ic]->cd(1);
    gPad->SetLogy(1);
    hADC[ic][0]->Draw();
    c[ic]->cd(2);
    gPad->SetLogy(1);
    hADC[ic][1]->Draw();
  }
  

}
