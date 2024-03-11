// Check DAQ Diff corde //
// Auther : Kosuke Itabashi  //

const int nfile =2;
const int nch =8;
void Diff(){


  string runnum ="40";

  string rname = Form("../RootFile/itabashi/run_%s.root",runnum.c_str());
  string rname2 = Form("../RootFile/run_%s.root",runnum.c_str());

  TChain* T[nfile];
  T[0] = new TChain("T");
  T[0] ->Add(rname.c_str());
  T[1] = new TChain("T");
  T[1] ->Add(rname2.c_str());
  //  for(int i=0; i< nfile; i++){
  //    T[i] -> Add(rname.c_str());
  //  }


  float pedestal[nfile];
  float integratedADC[nfile];

  float ped[nch],integratedadc[nch];
  float ped2[nch],integratedadc2[nch];

  T[0] -> SetBranchAddress("IntegratedADC",&integratedadc);
  T[1] -> SetBranchAddress("IntegratedADC",&integratedadc2);
  T[0] -> SetBranchAddress("Pedestal",&ped);
  T[1] -> SetBranchAddress("Pedestal",&ped2);

  int bin_ped_diff = 200;
  double min_ped_diff = -5.;
  double max_ped_diff =  5.;
  TH1F* hPed_CH0_diff = new TH1F("hPed_CH0_diff","",bin_ped_diff,min_ped_diff,max_ped_diff);
  TH1F* hPed_CH1_diff = new TH1F("hPed_CH1_diff","",bin_ped_diff,min_ped_diff,max_ped_diff);
  
  int bin_ped = 200;
  double min_ped = 400;
  double max_ped = 550;
  TH1F* hPed_CH0[nfile];
  TH1F* hPed_CH1[nfile];
  
  int bin_adc_diff = 200;
  double min_adc_diff = -200.;
  double max_adc_diff =  200.;
  TH1F* hADC_CH0_diff = new TH1F("hADC_CH0_diff","",bin_adc_diff,min_adc_diff,max_adc_diff);
  TH1F* hADC_CH1_diff = new TH1F("hADC_CH1_diff","",bin_adc_diff,min_adc_diff,max_adc_diff);
  
  int bin_adc = 400;
  double min_adc = -100;
  double max_adc = 800;
  TH1F* hADC_CH0[nfile];
  TH1F* hADC_CH1[nfile];


  
  for(int i=0 ; i<nfile; i++){

    hPed_CH0[i] = new TH1F(Form("hPed_CH0_%d",i),"",bin_ped,min_ped,max_ped);
    hPed_CH1[i] = new TH1F(Form("hPed_CH1_%d",i),"",bin_ped,min_ped,max_ped);
    hADC_CH0[i] = new TH1F(Form("hADC_CH0_%d",i),"",bin_adc,min_adc,max_adc);
    hADC_CH1[i] = new TH1F(Form("hADC_CH1_%d",i),"",bin_adc,min_adc,max_adc);

    
  }
  
  int ENum = T[0]->GetEntries();    

  for(int iev =0; iev<ENum;iev++){
    integratedadc[0] = 0.0;
    integratedadc2[0] = 0.0;
    integratedadc[1] = 0.0;
    integratedadc2[1] = 0.0;
    ped[0]  = 0.0;
    ped[1]  = 0.0;
    ped2[0]  = 0.0;
    ped2[0]  = 0.0;

    for(int i=0; i<nfile; i++){    
      T[i] ->GetEntry(iev);
      if(i==0){
	hADC_CH0[i] ->Fill(integratedadc[0]);
	hADC_CH1[i] ->Fill(integratedadc[1]);	
	hPed_CH0[i] ->Fill(ped[0]);
	hPed_CH1[i] ->Fill(ped[1]);	

      }else if(i==1){
	hADC_CH0[i] ->Fill(integratedadc2[0]);
	hADC_CH1[i] ->Fill(integratedadc2[1]);	
	hPed_CH0[i] ->Fill(ped2[0]);
	hPed_CH1[i] ->Fill(ped2[1]);	

	hADC_CH0_diff->Fill(integratedadc2[0] - integratedadc[0]);
	hADC_CH1_diff->Fill(integratedadc2[1] - integratedadc[1]);
	hPed_CH0_diff->Fill(ped2[0] - ped[0]);
	hPed_CH1_diff->Fill(ped2[1] - ped[1]);

      }
    }
  }



  
  
  // ================================================= //

  TFile* ofr = new TFile("RootFiles/diff.root","recreate");

  for(int i=0; i<nfile ;i++){

    hADC_CH0[i]->Write();
    hADC_CH1[i]->Write();
    hPed_CH0[i]->Write();
    hPed_CH0[i]->Write();

    hADC_CH0[i]->SetLineColor(i+1);
    hADC_CH0[i]->SetFillColor(i+1);
    hADC_CH0[i]->SetFillStyle(3365);
    hADC_CH1[i]->SetLineColor(i+1);
    hADC_CH1[i]->SetFillColor(i+1);
    hADC_CH1[i]->SetFillStyle(3395);

    hPed_CH0[i]->SetLineColor(i+1);
    hPed_CH0[i]->SetFillColor(i+1);
    hPed_CH0[i]->SetFillStyle(3365);
    hPed_CH1[i]->SetLineColor(i+1);
    hPed_CH1[i]->SetFillColor(i+1);
    hPed_CH1[i]->SetFillStyle(3395); 
  }

  hADC_CH0_diff->Write();
  hADC_CH1_diff->Write();
  hPed_CH0_diff->Write();
  hPed_CH1_diff->Write();

  // ================================================= //
  
  

}
