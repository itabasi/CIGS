
const int nch =16;


void GaN(){

  string dir ="/home/togawa/work/SENSOR/RAPID_16ch_16/test/RootFile/";
  string runname = "run_59.root";
  string rname =dir + runname;
  
  TChain* T = new TChain("T");
  T->Add(rname.c_str());

  float IntegratedADC[nch];
  unsigned short adc[nch][100];

  T->SetBranchAddress("IntegratedADC",&IntegratedADC);
  T->SetBranchAddress("adc",&adc);
  int ENum = T->GetEntries();

  TH1D* hIntegratedADC[nch];
  TH1D* hIntegratedADC_cut[nch];
  double threshold = 525;
  int a=0;
  int b=0;
  for(int i=0; i<nch; i++){
    hIntegratedADC[i] = new TH1D(Form("hIntegratedADC_%d",i),"",1000,0,1000);
    hIntegratedADC_cut[i] = new TH1D(Form("hIntegratedADC_cut_%d",i),Form("IntegratedADC CH%d ; IntegratedADC[ch] ; Counts",i),1000,0,1000);
  }
  
  
  for(int i=0; i<ENum;i++){

    T->GetEntry(i);

    

    for(int j=0;j<nch;j++){

    for(int k=0; k<100; k++){
      if(adc[j][k]> 525){
	hIntegratedADC_cut[j] ->Fill(IntegratedADC[j]);
	if(j==9){
	  //	  a++;
	  //	  cout<<"iev "<<i<<" ch "<<j<<" sample "<<k<<" adc "<<adc[j][k]<<" ncut "<<a<<endl;
	}
	break;
      }
    }

      //      if(IntegratedADC[j]>100) // cut therehold
	hIntegratedADC[j] ->Fill(IntegratedADC[j]);

    }
    

  }




  // =========== ANALYSIS ================= //


  TGraphErrors* gADC = new TGraphErrors();
  gADC->SetName("gADC");
  gADC->SetTitle("; Channel ; mean [ch]");
  gADC->SetMarkerStyle(20);
  gADC->SetMarkerColor(2);

  TGraphErrors* gADC_chi2 = new TGraphErrors();
  gADC_chi2->SetName("gADC_chi2");
  gADC_chi2->SetTitle("; Channel ; Chi2/ndf");
  gADC_chi2->SetMarkerStyle(20);
  gADC_chi2->SetMarkerColor(2);
  
  double mean[nch],sigma[nch], amp[nch];
  double mean_err[nch];
  double chi2[nch];
  double ndf[nch];
  TF1* fgaus = new TF1("fgaus","gausn(0)",0,1000);

    for(int j=0;j<nch;j++){
      mean[j] = hIntegratedADC_cut[j]->GetBinCenter(hIntegratedADC_cut[j]->GetMaximumBin());
      amp[j] = hIntegratedADC_cut[j]->GetBinContent(hIntegratedADC_cut[j]->GetMaximumBin());

      sigma[j] = 19.66;
      fgaus->SetParameters(amp[j],mean[j],sigma[j]);
      hIntegratedADC_cut[j] ->Fit("fgaus","QR","QR",mean[j]-2.*sigma[j],mean[j]+2.*sigma[j]);
     
      
      mean[j] = fgaus->GetParameter(1);
      mean_err[j] = fgaus->GetParError(1);
      chi2[j] = fgaus->GetChisquare();
      ndf[j]  = fgaus->GetNDF();
      gADC->SetPoint(j,j,mean[j]);
      gADC->SetPointError(j,0,mean_err[j]);
      gADC_chi2->SetPoint(j,j,chi2[j]/ndf[j]);
      
    }

  

    TCanvas* c0 = new TCanvas("c0","c0");
    c0->cd();
    gADC->Draw("AP");


    //    TCanvas* c1 = new TCanvas("c1","c1");
    //    c1->cd();
    //    gADC_chi2->Draw("AP");



    TCanvas* c[4];
    for(int i=0;i<4;i++){
      c[i]= new TCanvas(Form("c0%d",i),Form("c0%d",i));
      c[i]->Divide(2,2);
      for(int j=0;j<4;j++){
	c[i]->cd(j+1);
	hIntegratedADC[4*i+j]->SetLineColor(1);
	//	hIntegratedADC[i]->SetFillColor(2);
	//	hIntegratedADC[i]->SetFillStyle(3002);
	hIntegratedADC_cut[4*i+j]->SetLineColor(4);
	hIntegratedADC_cut[4*i+j]->SetFillColor(4);
	hIntegratedADC_cut[4*i+j]->SetFillStyle(3002);
	//	gPad->SetLogy(1);
	//	hIntegratedADC[i]->Draw();
	hIntegratedADC_cut[4*i+j]->Draw();
      }    
    }

    
  

    TFile* ofr = new TFile(runname.c_str(),"recreate");

  for(int j=0;j<nch;j++){
    hIntegratedADC[j]->Write();
  }
  gADC->Write();
  gADC_chi2->Write();

  
  
}
