const int nfile =2;
const int nch   =8;

void hist_CIGS_sigma(){


 

  string rname[nfile];
  rname[0]= "/home/togawa/work/SENSOR/CIGS/Ana/analysis/RootFile/itabashi/run_32.root";//CIGS 2um
  rname[1]= "/home/togawa/work/SENSOR/CIGS/Ana/analysis/RootFile/itabashi/run_62.root";//CIGS 5um


  TF1* f[nfile];
  TChain* T[nfile];


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
    ENum[i] = T[i]->GetEntries();
    for(int ich=0; ich<nch ; ich++){
      hADC[i][ich] = new TH1F(Form("hADC_%d_%d",i,ich),"",bin_adc,min_adc,max_adc);
    }

    cout<<"ENum "<<ENum[i]<<endl;
    
    for(int iev=0; iev<ENum[i]; iev++){
      T[i]->GetEntry(iev);
      for(int ich=0; ich<nch ; ich++){
       	if(IntegratedADC[ich]>100. && ich <2)
	  hADC[i][ich] ->Fill(IntegratedADC[ich]);
      }
    }
  }

  // ============== Fitting =================== //
  TF1* fgaus[nch][nfile];

 
  for(int i=0; i<nch;i++){
    for(int j=0;j<nfile;j++){
      fgaus[i][j]= new TF1(Form("fgaus_%d_%d",i,j),"gausn(0)",0,2000);
      fgaus[i][j]->SetLineColor(2);
      double x = hADC[j][i]->GetBinCenter(hADC[j][i]->GetMaximumBin());
      double y = hADC[j][i]->GetBinContent(hADC[j][i]->GetMaximumBin());
      fgaus[i][j]->SetParameter(1,x);
      fgaus[i][j]->SetParameter(2,y);
      hADC[j][i]->Fit(Form("fgaus_%d_%d",i,j),"QR","QR",100,2000.);

      cout<<"ich "<<i<<" ifile "<<j<<" sigma "<<fgaus[i][j]->GetParameter(2)<<" sigma/mean "<<fgaus[i][j]->GetParameter(2)/fgaus[i][j]->GetParameter(1)<<endl;
      
    }
   
  }

  
  // ========================================= //

  
  int ncanvas=nfile;
  TCanvas* c[ncanvas];

  for(int ic=0; ic<ncanvas; ic++){

    c[ic] = new TCanvas(Form("c%d",ic),Form("c%d",ic));
    c[ic]->Divide(2,1);
    c[ic]->cd(1);
    gPad->SetLogy(1);
    hADC[ic][0]->Draw();
    fgaus[ic][0]->Draw("same");
    c[ic]->cd(2);
    gPad->SetLogy(1);
    hADC[ic][1]->Draw();
    fgaus[ic][1]->Draw("same");
  }


}
