
const int nch =2;
const int nsample =100;
void ShapeSum(){
  gStyle->SetPalette(kRainBow); 

  string rname[10];
  int runnum[10];
  // ---------- CIGS 5 num --------- //
  runnum[0] = 63;
  runnum[1] = 64;
  runnum[2] = 65;
  runnum[3] = 66;
  runnum[4] = 67;
  runnum[5] = 68;
  runnum[6] = 69;

  int nfile =7;
  TFile* f[10];
  TH1F* hADC[nfile][nch];
  TH1F* hRes[nfile];
  TCanvas* c0 = new TCanvas("c0","c0");
  c0->Divide(2,1);
  
  for(int i=0;i<nfile; i++){
    rname[i] = Form("./RootFiles/shapeAna/run_%d.root",runnum[i]);
    f[i] = new TFile(rname[i].c_str());    
    hRes[i] = new TH1F(Form("hRes_%d",i),"",nfile,-0.5,nfile-0.5);
    for(int j=0; j<nch;j++){
      hADC[i][j] = (TH1F*)f[i]->Get(Form("hADC_%d",j));
      hADC[i][j] ->SetName(Form("hADC_%d_%d",i,j));
      if(j==0){
	c0->cd(1);
	gPad->SetLogy(1);
	if(i==0) hADC[i][j]->Draw("PLC PMC hist");
	else  hADC[i][j]->Draw("PLC PMC hist same");
      }else if(j ==1){
	c0->cd(2);
	gPad->SetLogy(1);
	if(i==0) hADC[i][j]->Draw("PLC PMC hist");
	else  hADC[i][j]->Draw("PLC PMC hist same");
      }
    }
  }
  



  // ========= Analysis  ============ //



  TCanvas* c1 = new TCanvas("c1","c1");
  c1->Divide(2,1);
  
  TH1F* hADC_diff[nfile][nch];
  TGraphErrors* gRes[nch];
  TGraphErrors* gFWHM[nch];
  for(int i=0; i<nch; i++){
    gRes[i] = new TGraphErrors();
    gRes[i] -> SetName(Form("gRes_%d",i));
    gFWHM[i] = new TGraphErrors();
    gFWHM[i] -> SetName(Form("gFWHM_%d",i));

  }

  double FWHM_xst,FWHM_xend;
  double Height[nfile][nch];
      
  for(int ifile =0; ifile<nfile;ifile++){  
    for(int ich =0; ich < nch ; ich++){
      hADC_diff[ifile][ich] = new TH1F(Form("hADC_diff_%d_%d",ifile,ich),"",nsample,-0.5,nsample-0.5);
      bool FWHM_st = false;
      bool FWHM_end = false;
      double peak_y = hADC[ifile][ich]->GetBinContent(hADC[ifile][ich]->GetMaximumBin());
      double peak_x =hADC[ifile][ich]->GetBinCenter(hADC[ifile][ich]->GetMaximumBin());
      Height[ifile][ich] = peak_y; 
      for(int ibin=0; ibin < nsample; ibin++){
	double rate = hADC[ifile][ich]->GetBinContent(ibin)/hADC[6][ich]->GetBinContent(ibin);
	double x = hADC[ifile][ich]->GetBinCenter(ibin);
	double y = hADC[ifile][ich]->GetBinContent(ibin);
	hADC_diff[ifile][ich] ->Fill(x,rate);

	if(x < peak_x && peak_y/2< y && !FWHM_st){
	  FWHM_st = true;
	  FWHM_xst = x;
	}
	if(peak_x < x  && y < peak_y/2 && !FWHM_end){
	  FWHM_end = true;
	  FWHM_xend = x;
	}
      } // end bin 

      
      if(ich==0){
	c1->cd(1);
	//	gPad->SetLogy(1);
	if(ifile==0){ hADC_diff[ifile][ich]->Draw("PLC PMC hist");
	  hADC_diff[ifile][ich] ->GetYaxis()->SetRangeUser(-100,100);
	}else  hADC_diff[ifile][ich]->Draw("PLC PMC hist same");
      }else if(ich ==1){
	c1->cd(2);
	//	gPad->SetLogy(1);
	if(ifile==0){ hADC_diff[ifile][ich]->Draw("PLC PMC hist");
	  hADC_diff[ifile][ich] ->GetYaxis()->SetRangeUser(-100,100);
	}else  hADC_diff[ifile][ich]->Draw("PLC PMC hist same");
      }
      gFWHM[ich] ->SetPoint(ifile,ifile,(FWHM_xend - FWHM_xst)/Height[ifile][ich]);
    }
  }

  TCanvas* c2 = new TCanvas("c2","c2");
  c2->Divide(2,1);
  c2->cd(1);
  gFWHM[0]->Draw("APL");
  c2->cd(2);
  gFWHM[1]->Draw("APL");
  
  // =============================== //
  




    
  
}
