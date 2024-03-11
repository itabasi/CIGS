const int nfile =1;
const int nch   =16;

void hist_GaN_sigma(){


  string rname_GaN    = "/home/togawa/work/SENSOR/RAPID_16ch_16/Ana/RootFile/run_30.root"; // 4V

  string rname[nfile];
  //  rname[0]=rname_CIGS2um;
  //  rname[1]=rname_CIGS5um;
  rname[0]=rname_GaN;

  TF1* f[nfile];
  bool CIGS_flag[nfile];
  TChain* T[nfile];
  //  float IntegratedADC[nfile][nch];

  int ENum[nfile];

  TH1F* hADC[nfile][nch];
  double min_adc = 0.;
  double max_adc = 500;
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
	if(IntegratedADC[ich]>50.)
	  hADC[i][ich] ->Fill(IntegratedADC[ich]);
      }
    }
  }


  // ============== Fitting =================== //
  TF1* fgaus[nch];
  TGraphErrors* gGaN = new TGraphErrors();
  gGaN -> SetName("gGaN");
  gGaN -> SetMarkerStyle(20);
  gGaN -> SetMarkerColor(2);
  gGaN -> SetLineColor(2);

  TGraphErrors* gGaN_Res = new TGraphErrors();
  gGaN_Res -> SetName("gGaN_Res");
  gGaN_Res -> SetMarkerStyle(20);
  gGaN_Res -> SetMarkerColor(2);
  gGaN_Res -> SetLineColor(2);
  
  
  for(int i=0; i<nch;i++){
    fgaus[i]= new TF1(Form("fgaus_%d",i),"gausn(0)",0,500);
    fgaus[i]->SetLineColor(2);
    double x = hADC[0][i]->GetBinCenter(hADC[0][i]->GetMaximumBin());
    double y = hADC[0][i]->GetBinContent(hADC[0][i]->GetMaximumBin());
    fgaus[i]->SetParameter(1,x);
    fgaus[i]->SetParameter(2,y);
    hADC[0][i]->Fit(Form("fgaus_%d",i),"","",100,400.);
    
    gGaN->SetPoint(i,i,fgaus[i]->GetParameter(2));
    gGaN->SetPointError(i,0,fgaus[i]->GetParError(2));

    gGaN_Res->SetPoint(i,i,fgaus[i]->GetParameter(2)/fgaus[i]->GetParameter(1));


    //    cout<<"i "<<i<<" x "<<x<<" mu "<<fgaus[i]->GetParameter(1)<<" mean "<<fgaus[i]->GetParameter(2)<<endl;

    
  }

  
  // ========================================= //

  
  int ncanvas=nfile*nch/4;
  TCanvas* c[ncanvas];

  for(int ic=0; ic<ncanvas; ic++){
    cout<<"ic "<<ic<<" nc "<<ncanvas<<endl;
    c[ic] = new TCanvas(Form("c%d",ic),Form("c%d",ic));
    c[ic]->Divide(2,2);
    c[ic]->cd(1);
    gPad->SetLogy(1);
    hADC[0][4*ic]->Draw();
    fgaus[4*ic]->Draw("same");
    c[ic]->cd(2);
    gPad->SetLogy(1);
    hADC[0][4*ic+1]->Draw();
    fgaus[4*ic+1]->Draw("same");
    c[ic]->cd(3);
    gPad->SetLogy(1);
    hADC[0][4*ic+2]->Draw();
    fgaus[4*ic+2]->Draw("same");
    c[ic]->cd(4);
    gPad->SetLogy(1);
    hADC[0][4*ic+3]->Draw();
    fgaus[4*ic+3]->Draw("same");
  }



  
  TCanvas* c100 = new TCanvas("c100","c100");
  c100->cd();
  gGaN->Draw("AP");

  // === CIGS sigma fitting results =======//
  double sigma_CIGS_2um[2]={49.5,70.};
  double sigma_CIGS_5um[2]={48,69.}; 
  TLine* LCIGS_2um_0CH = new TLine(7,sigma_CIGS_2um[0],nch,sigma_CIGS_2um[0]);
  LCIGS_2um_0CH->SetLineColor(1);
  TLine* LCIGS_2um_1CH = new TLine(7,sigma_CIGS_2um[1],nch,sigma_CIGS_2um[1]);
  LCIGS_2um_1CH->SetLineColor(3);
  TLine* LCIGS_5um_0CH = new TLine(7,sigma_CIGS_5um[0],nch,sigma_CIGS_5um[0]);
  LCIGS_5um_0CH->SetLineColor(4);
  TLine* LCIGS_5um_1CH = new TLine(7,sigma_CIGS_5um[1],nch,sigma_CIGS_5um[1]);
  LCIGS_5um_1CH->SetLineColor(6);

  LCIGS_2um_0CH->Draw("same");
  LCIGS_2um_1CH->Draw("same");
  LCIGS_5um_0CH->Draw("same");
  LCIGS_5um_1CH->Draw("same");

  TLegend*leg = new TLegend(0.15,0.60,0.5,0.90,"","NDC");
  leg->AddEntry(gGaN, "GaN HV=-4V","ple");
  leg->AddEntry(LCIGS_2um_0CH, "CIGS 2um CH0 HV=-2V","le");
  leg->AddEntry(LCIGS_2um_1CH, "CIGS 2um CH1 HV=-2V","le");
  leg->AddEntry(LCIGS_5um_0CH, "CIGS 5um CH0 HV=-2V","le");
  leg->AddEntry(LCIGS_5um_1CH, "CIGS 5um CH1 HV=-2V","le");

  leg->Draw();
  
  TCanvas* c200 = new TCanvas("c200","c200");
  c200->cd();
  gGaN_Res->Draw("AP");
  double res_CIGS_2um[2]={0.105,0.0608};
  double res_CIGS_5um[2]={0.1,0.06}; 
  TLine* LCIGS_Res_2um_0CH = new TLine(7,res_CIGS_2um[0],nch,res_CIGS_2um[0]);
  LCIGS_Res_2um_0CH->SetLineColor(1);
  TLine* LCIGS_Res_2um_1CH = new TLine(7,res_CIGS_2um[1],nch,res_CIGS_2um[1]);
  LCIGS_Res_2um_1CH->SetLineColor(3);
  TLine* LCIGS_Res_5um_0CH = new TLine(7,res_CIGS_5um[0],nch,res_CIGS_5um[0]);
  LCIGS_Res_5um_0CH->SetLineColor(4);
  TLine* LCIGS_Res_5um_1CH = new TLine(7,res_CIGS_5um[1],nch,res_CIGS_5um[1]);
  LCIGS_Res_5um_1CH->SetLineColor(6);

  LCIGS_Res_2um_0CH->Draw("same");
  LCIGS_Res_2um_1CH->Draw("same");
  LCIGS_Res_5um_0CH->Draw("same");
  LCIGS_Res_5um_1CH->Draw("same");

  TLegend*leg2 = new TLegend(0.15,0.60,0.5,0.90,"","NDC");
  leg2->AddEntry(gGaN, "GaN HV=-4V","ple");
  leg2->AddEntry(LCIGS_Res_2um_0CH, "CIGS 2um CH0 HV=-2V","le");
  leg2->AddEntry(LCIGS_Res_2um_1CH, "CIGS 2um CH1 HV=-2V","le");
  leg2->AddEntry(LCIGS_Res_5um_0CH, "CIGS 5um CH0 HV=-2V","le");
  leg2->AddEntry(LCIGS_Res_5um_1CH, "CIGS 5um CH1 HV=-2V","le");

  leg2->Draw();

  
  
}
