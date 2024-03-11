const int nch =8;
const int nsample =2048;

void MultiHit(){
  int runnum = 51; // FADC 2048
  //   runnum = 52; // FADC 2048
  //  runnum = 74; // FADC 2048
  string rname =Form("../RootFile/run_%d.root",runnum);
  TChain* T = new TChain("T");
  T->Add(rname.c_str());
  float IntegratedADC[nch];
  short adc[nch][nsample];
  float  Pedestal[nch];
  T->SetBranchAddress("IntegratedADC",&IntegratedADC);
  T->SetBranchAddress("adc",&adc);
  T->SetBranchAddress("Pedestal",&Pedestal);
  int ENum = T->GetEntries();
  cout<<"ENum "<<ENum<<endl;
  TH1D* hMulti[nch];
  TH1D* hMulti_cut[nch];
  TH1D* hMulti_width[nch];
  TH1D* hMulti_width_cut[nch];
  TH2D* hMulti_dep = new TH2D("hMulti_dep",";ch0 ;ch1",100,-0.5,99.5,100,-0.5,99.5);
  TH1D* hMulti_adc[nch];
  TGraphErrors* g[nch];
  TGraphErrors* gR = new  TGraphErrors();
  gR->SetName("gR");
  gR->SetMarkerColor(2);
  TGraphErrors* gP[nch];


  TH1D* hMulti_min[nch];
  // ========= Calc Npeak in 10 spill ========= //

  double ppp[7];//={25967,43010,42081,43018,45023};
  int time_sepa[7];
  double spel_to_min = 60./3.3;
  int nsepa =7;
  int run_piriod =30;

  if(runnum == 51){
    // Run51 : 14:03 - 14:08
    ppp[0] = 25967* spel_to_min; // 14:03 Run 51
    ppp[1] = 25967* spel_to_min; // 14:03 Run 51
    time_sepa[0] = 0;
    time_sepa[1] = 6;
    nsepa =1;
    run_piriod =6;
  }else if(runnum == 52){
    // Run52 : 14:09 - 14:38   
    nsepa =6;
    run_piriod =30;
    ppp[0] = 43010* spel_to_min; // 14:09 Run 52
    ppp[1] = 42081* spel_to_min; // 14:14 Run 52
    ppp[2] = 43018* spel_to_min; // 14:20 Run 52
    ppp[3] = 45023* spel_to_min; // 14:25 Run 52
    ppp[4] = 42617* spel_to_min; // 14:31 Run 52
    ppp[5] = 42472* spel_to_min; // 14:36 Run 52
    ppp[6] = 46430* spel_to_min; // 14:42 Run 52
    time_sepa[0] = 0;
    time_sepa[1] = 6;
    time_sepa[2] = 12;
    time_sepa[3] = 18;
    time_sepa[4] = 23;
    time_sepa[5] = 28;
    time_sepa[6] = 30;
  }else if( runnum == 74){

    // ====== Run 74 ====== //
    // Run74 : 21:48 - 22:03 (16 min)
    nsepa =4;
    run_piriod =20;
    ppp[0] = 77653* spel_to_min; // 21:45
    ppp[1] = 75534* spel_to_min; // 21:51
    ppp[2] = 73702* spel_to_min; // 21:56
    ppp[3] = 73396* spel_to_min; // 22:02
    ppp[4] = 71717* spel_to_min; // 22:07
  
    time_sepa[0] = 0;
    time_sepa[1] = 4;
    time_sepa[2] = 9;
    time_sepa[3] = 15;
    time_sepa[4] = 20;

  }
  
  
  for(int i=0;i<nch;i++){
    hMulti[i] = new TH1D(Form("hMulti_%d",i),";multi-hits;Counts",200,-0.5,199.5);
    hMulti_cut[i] = new TH1D(Form("hMulti_cut_%d",i),";multi-hits;Counts",200,-0.5,199.5);
    
    hMulti_width[i] = new TH1D(Form("hMulti_width_%d",i),";peak width [ch];Counts",100,-0.5,99.5);

    hMulti_width_cut[i] = new TH1D(Form("hMulti_width_cut_%d",i),";peak width [ch];Counts",100,-0.5,99.5);

    //    hMulti_adc[i] = new TH1D(Form("hMulti_adc_%d",i),"",1000,-10000,20000);
    hMulti_adc[i] = new TH1D(Form("hMulti_adc_%d",i),"",100,0,100);

    hMulti_min[i] = new TH1D(Form("hMulti_min_%d",i),";run time [min]; Npeaks",run_piriod+1,-0.5,run_piriod+0.5);
    hMulti_min[i]->SetLineColor(i+1);
    
    g[i]= new TGraphErrors();
    g[i]->SetName(Form("g_%d",i));
    g[i]->SetMarkerColor(i+1);
    g[i]->SetMarkerStyle(1);

    gP[i]= new TGraphErrors();
    gP[i]->SetName(Form("gP_%d",i));
    gP[i]->SetMarkerColor(i+1);
    gP[i]->SetMarkerStyle(1);


    
  }

  int threshold = 500;
  int npeak =0;
  int npeak_cut =0;
  int nwidth =0;
  int width=0;
  int peak_mean[2][10000];
  //  int peak_st[2][10000];
  int peak_End[2][10000];
  int sum[2]={0,0};
  int evmax =200000;
  int ww[evmax][2];
  double nn[1000][2];
  int nnn =1000;
  int nnnn=0;
  int nthreshold =0;
  int threshold_sum =0;
  int npeak_sample =0;
  int peak_sum =0;
  int npeak_sum =0;
  double integratedADC_per_peak[2]={2.76609e+02,2.54241e+02};
  if(runnum == 74){
    integratedADC_per_peak[0] = 4.88954e+02;
    integratedADC_per_peak[1] =  4.73586e+02;
  }

  
  double www[10000][2];



  



  
  
  TGraphErrors* gPPP = new TGraphErrors();
  gPPP->SetName("gPPP");
  gPPP->SetMarkerStyle(20);
  gPPP->SetMarkerColor(2);
  gPPP->SetLineColor(2);

  TGraphErrors* gScale[nch];

  
  TH1D* hPPP[nch];
  for(int i=0;i<nch; i++){
    hPPP[i]= new TH1D(Form("hPPP_%d",i),"",run_piriod+1,-0.5,run_piriod+0.5);
    gScale[i]= new TGraphErrors();
    gScale[i]->SetName(Form("gScale_%d",i));
    gScale[i]->SetMarkerStyle(20);
    gScale[i]->SetMarkerColor(i+1);
    gScale[i]->SetLineColor(i+1);

  }
  for(int i=0;i<nsepa;i++){
    gPPP->SetPoint(i,(double)(time_sepa[i]+time_sepa[i+1])/2,ppp[i]);
    gPPP->SetPointError(i,(double)(time_sepa[i]+time_sepa[i+1])/2 - time_sepa[i],0.0);
  }


  for(int l=0;l<1000;l++){
      nn[l][0] =0;
      nn[l][1] =0;
    }


    
  for(int iev =0;iev<ENum ; iev++){
    T->GetEntry(iev);

    int run_time = (double)iev/(double)(ENum/run_piriod);
  
    for(int j=0;j<2;j++){
      npeak =0;
      npeak_cut =0;
      width =0;
      double SUM=0;
      bool peak_st =false;
      bool peak_end =true;
      nthreshold = 0;
      threshold_sum =0;
      npeak_sample =0;
      npeak_sum =0;
      for(int i=0;i<nsample; i++){
	if(adc[j][i] > 600. || adc[j][i] < 300) continue;
	SUM += -adc[j][i] + Pedestal[j];
	if(adc[j][i] > 505){
	  nthreshold ++;
	  threshold_sum += adc[j][i];
	}
	
	if(peak_end && adc[j][i] < threshold){ // Start of Peak
	  peak_st = true;
	  peak_end = false;
	  npeak++;
	  npeak_cut++;
	  width =0;
	}
			     
	if(peak_st && adc[j][i] > threshold){ // End of Peak
	  peak_end = true;
	  peak_st = false;
	  peak_End[j][npeak] = i;
	  npeak_sum +=adc[j][i];
	  npeak_sample ++;
	  if(width<3)	    npeak_cut--;
	  hMulti_width[j]->Fill(width);	 	  
	}
	if(peak_st && !peak_end){
	  npeak_sum +=adc[j][i];
	  npeak_sample ++;
	}
	width++;

      }

      // ===== calc threshold ====== ///
      //      double peak_sum_d =(double)(npeak_sum/npeak_sample);
      double npeak_sum_d =(double)npeak_sum;
      double threshold_sum_d;
      if(nthreshold>0)
	threshold_sum_d=(double)(threshold_sum/nthreshold);
      else threshold_sum_d =0;

      double peak_integratedADC = (-npeak_sum_d + threshold_sum_d*(double)(npeak_sample));
      //      hMulti_adc[j] ->Fill(peak_integratedADC);
      hMulti_adc[j] ->Fill(peak_integratedADC/integratedADC_per_peak[j]);
      hMulti[j]->Fill(npeak);
      hMulti_cut[j]->Fill(npeak_cut);
      hMulti_min[j]->Fill(run_time,peak_integratedADC/integratedADC_per_peak[j]);
      for(int k=0; k<nsepa;k++){
	if(  run_time < time_sepa[k+1] ){
	  hPPP[j]->Fill(run_time,peak_integratedADC/integratedADC_per_peak[j]);
	  //hPPP[j]->Fill((double)(time_sepa[k]+time_sepa[k+1])/2.,peak_integratedADC/integratedADC_per_peak[j]);
	  break;}
      }
      
      g[j] ->SetPoint(iev,iev,npeak_cut);
      gP[j] ->SetPoint(iev,iev,peak_integratedADC/integratedADC_per_peak[j]);
      sum[j] +=npeak_cut;
      if(iev<evmax)ww[iev][j] = npeak_cut;
      npeak =0;
      // ===== Ratio ======= //
      nnnn  = (iev/nnn);
      nn[nnnn][j] += npeak_cut;
    } // end for j
    for(int k =0; k<nnnn; k++){
      if(nn[k][0]>0){
	gR ->SetPoint(k,nnn*k-1 + nnn/2.,nn[k][1]/nn[k][0]);
	gR ->SetPointError(k,nnn/2.,0);
      }else{
	gR ->SetPoint(k,nnn*k-1 + nnn/2.,0.0);
	gR ->SetPointError(k,nnn/2.,0);

      }
    }
  }

  for(int iev =0;iev<ENum ; iev++){
    for(int j=0;j<2;j++){
      hMulti_dep ->Fill(ww[iev][0],ww[iev][1]);
    }
  }
  



  // ============================================ //

  double smin[2],smax[2],smean[2];
  for(int j =0; j<2; j++){
    smin[j] = 10000;
    smax[j] =0;
    smean[j] =0;
    int nbin = hPPP[j] ->GetXaxis()->GetNbins();
    for(int i=1;i<nbin+1;i++){
      double x = hPPP[j]->GetBinCenter(i);
      double y = hPPP[j]->GetBinContent(i);

      for(int k=0; k<nsepa;k++){
	if(x < time_sepa[k+1]){
	  //	  double yy = (ppp[k+1]-ppp[k])/(time_sepa[k+1]-time_sepa[k])*x+ppp[k];
	  double yy = ppp[k];
	  //	  cout<<"x "<<x<<" yy "<<yy<<" y "<<y<<" ppp "<<ppp[k]<<endl;
	  hPPP[j] -> SetBinContent(i,y/yy);
	  hPPP[j] -> SetBinError(i,0);
	  gScale[j]->SetPoint(i,x,y/yy);
	  if(smin[j] > y/ppp[k]) smin[j] = y/yy;
	  if(smax[j] < y/ppp[k]) smax[j] = y/yy;
	  break;
	}
      }
    }
    smean[j] = (smin[j]+smax[j])/2.0;
  }

  // ============================================ //

  
  TLegend*leg = new TLegend(0.15,0.60,0.5,0.90,"","NDC");
  leg->AddEntry(hMulti_cut[0], "CIGS(No20) CH0 ","pl");
  leg->AddEntry(hMulti_cut[1], "CIGS(No20) CH1 ","pl");
  

  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  hMulti_cut[0]->SetLineColor(2);
  hMulti_cut[0]->SetFillColor(2);
  hMulti_cut[0]->SetFillStyle(3002);
  hMulti_cut[1]->SetLineColor(4);
  hMulti_cut[1]->SetFillColor(4);
  hMulti_cut[1]->SetFillStyle(3002);

  hMulti_cut[1]->Draw("");
  hMulti_cut[0]->Draw("same");
  leg->Draw();

  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();    
  hMulti_width[0]->SetLineColor(2);
  hMulti_width[0]->SetFillColor(2);
  hMulti_width[0]->SetFillStyle(3002);

  hMulti_width_cut[0]->SetLineColor(2);
  hMulti_width_cut[0]->SetFillColor(4);
  hMulti_width_cut[0]->SetFillStyle(3002);

  hMulti_width[1]->SetLineColor(4);
  hMulti_width[1]->SetFillColor(4);
  hMulti_width[1]->SetFillStyle(3002);

  hMulti_width[0]->Draw();
  hMulti_width[1]->Draw("same");


  TCanvas* c2 = new TCanvas("c2","c2");
  c2->Divide(1,2);
  c2->cd(1);      
  g[0]->Draw("AP");
  c2->cd(2);      
  g[1]->Draw("AP");

  TCanvas* c3 = new TCanvas("c3","c3");
  c3->cd();
  gR->Draw("AP");



  TCanvas* c4 = new TCanvas("c4","c4");
  c4->cd();
  hMulti_adc[0]->Draw();
  hMulti_adc[0]->SetLineColor(1);
  hMulti_adc[1]->Draw("same");
  hMulti_adc[1]->SetLineColor(2);
  

  TCanvas* c5 = new TCanvas("c5","c5");
  c5->Divide(1,2);
  c5->cd(1);      
  gP[0]->Draw("AP");
  c5->cd(2);      
  gP[1]->Draw("AP");

  TCanvas* c6 = new TCanvas("c6","c6");
  gStyle->SetOptStat(0000);
  c6->Divide(1,2);
  c6->cd(1);      
  hMulti_min[1]->GetXaxis()->SetTitleSize(0.08);
  hMulti_min[1]->GetXaxis()->SetTitleOffset(0.5);
  hMulti_min[1]->GetYaxis()->SetTitleSize(0.08);
  hMulti_min[1]->GetYaxis()->SetTitleOffset(0.5);
  hMulti_min[1]->GetXaxis()->SetLabelSize(0.05);
  hMulti_min[1]->GetYaxis()->SetLabelSize(0.05);

  hMulti_min[1]->Draw();
  hMulti_min[0]->Draw("same");

  c6->cd(2);      
  gPPP->GetXaxis()->SetTitleSize(0.08);
  gPPP->GetXaxis()->SetTitleOffset(0.7);
  gPPP->GetYaxis()->SetTitleSize(0.08);
  gPPP->GetYaxis()->SetTitleOffset(0.7);
  gPPP->GetXaxis()->SetLabelSize(0.05);
  gPPP->GetYaxis()->SetLabelSize(0.05);
  gPPP->SetTitle(";run time [min]; ppp [/min]");
  gPPP->Draw("ALP");

  TCanvas* c7 = new TCanvas("c7","c7");
  c7->cd();
  gPPP->Draw("APL");

  TCanvas* c8 = new TCanvas("c8","c8");
  c8->cd();
  gScale[1]->SetTitle(";time [min]; Scaling factor");
  gScale[1]->GetXaxis()->SetTitleSize(0.05);
  gScale[1]->GetXaxis()->SetTitleOffset(0.9);
  gScale[1]->GetYaxis()->SetTitleSize(0.05);
  gScale[1]->GetYaxis()->SetTitleOffset(0.9);
  gScale[1]->GetXaxis()->SetLabelSize(0.05);
  gScale[1]->GetYaxis()->SetLabelSize(0.05);

  gScale[1]->Draw("ALP");
  gScale[0]->Draw("LP");


  TCanvas* c9 = new TCanvas("c9","c9");
  c9->Divide(1,2);
  c9->cd(1);
  hMulti_adc[0]->Draw();
  hMulti_adc[0]->SetLineColor(1);
  hMulti_cut[0]->Draw("same");

  c9->cd(2);
  hMulti_adc[1]->Draw("");
  hMulti_adc[1]->SetLineColor(2);
  hMulti_cut[1]->Draw("same");

  
  
  cout<<"=============================="<<endl;
  cout<<"Total peaks CH0 "<<sum[0]<<" Counts"<<endl;
  cout<<"Total peaks CH1 "<<sum[1]<<" Counts"<<endl;
  cout<<"CH0 Peak Mean "<<hMulti_cut[0]->GetMean()<<endl;
  cout<<"CH1 Peak Mean "<<hMulti_cut[1]->GetMean()<<endl;

  cout<<"========== Count Npeak by IntegratedADC =========== "<<endl;
  int nbin_adc = hMulti_adc[0]->GetXaxis()->GetNbins();
  int npeak_adc[2];
  
  cout<<"Total peaks CH0 "<<hMulti_adc[0]->GetMean()<<endl;
  cout<<"Total peaks CH1 "<<hMulti_adc[1]->GetMean()<<endl;

  cout<<"Run : "<<runnum<<endl;
  cout<<"Scaling factor CH0 : "<<smean[0]<<" +- "<<smax[0] - smean[0]<<endl;
  cout<<"Scaling factor CH1 : "<<smean[1]<<" +- "<<smax[1] - smean[1]<<endl;  
  
}
