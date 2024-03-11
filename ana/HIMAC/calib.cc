

const double e = 1.602e-19;

void calib(){

  string rname[20][8];

  string rdir = "/home/togawa/work/SENSOR/CIGS/Ana/analysis/RootFile/";
  rname[0][0] =rdir + "config1/calib_ch0_100m.root";
  rname[1][0] =rdir + "config1/calib_ch0_150m.root";
  rname[2][0] =rdir + "config1/calib_ch0_200m.root";
  rname[3][0] =rdir + "config1/calib_ch0_250m.root";
  rname[4][0] =rdir + "config1/calib_ch0_300m.root";
  rname[5][0] =rdir + "config1/calib_ch0_tp.root";
  
  rname[0][1] =rdir + "config1/calib_ch1_100m.root";
  rname[1][1] =rdir + "config1/calib_ch1_150m.root";
  rname[2][1] =rdir + "config1/calib_ch1_200m.root";
  rname[3][1] =rdir + "config1/calib_ch1_250m.root";
  rname[4][1] =rdir + "config1/calib_ch1_300m.root";
  rname[5][1] =rdir + "config1/calib_ch1_tp.root";
  
  rname[0][2] =rdir + "config2/calib_ch0_100m.root";
  rname[1][2] =rdir + "config2/calib_ch0_200m.root";
  rname[2][2] =rdir + "config2/calib_ch0_300m.root";
  rname[3][2] =rdir + "config2/calib_ch0_400m.root";
  rname[4][2] =rdir + "config2/calib_ch0_500m.root";
  rname[5][2] =rdir + "config2/calib_ch0_tp.root";
  
  rname[0][3] =rdir + "config2/calib_ch1_100m.root";
  rname[1][3] =rdir + "config2/calib_ch1_200m.root";
  rname[2][3] =rdir + "config2/calib_ch1_300m.root";
  rname[3][3] =rdir + "config2/calib_ch1_400m.root";
  rname[4][3] =rdir + "config2/calib_ch1_500m.root";
  rname[5][3] =rdir + "config2/calib_ch1_tp.root";


  
  double HV[20][4]; // mV
  HV[0][0] = 100;
  HV[1][0] = 150;
  HV[2][0] = 200;
  HV[3][0] = 250;
  HV[4][0] = 300;


  HV[0][1] = 100;
  HV[1][1] = 150;
  HV[2][1] = 200;
  HV[3][1] = 250;
  HV[4][1] = 300;

  HV[0][2] = 100;
  HV[1][2] = 200;
  HV[2][2] = 300;
  HV[3][2] = 400;
  HV[4][2] = 500;

  HV[0][3] = 100;
  HV[1][3] = 200;
  HV[2][3] = 300;
  HV[3][3] = 400;
  HV[4][3] = 500;
  

  //  double Cdet = 33.; // pF
  double Cdet = 1.; // Fp
  double conv_factor = Cdet*1.e-12*1.e-3* 1.e12;
  TChain* T[20][6];
  int ENum;
  TH1D* hADC[20][6];
  TGraphErrors* gADC[6];
  TGraphErrors* gADC_TP[6];
  TGraphErrors* gADC_frame = new TGraphErrors();
  double xmin =0;
  double xmax =1.;
  gADC_frame ->SetPoint(0,xmin,0);
  gADC_frame ->SetPoint(1,xmax,1000);
  gADC_frame ->SetName("gADC_frame");
  //  gADC_frame ->SetTitle("; HV [mV]; IntegratedADC [ch]");
  gADC_frame ->SetTitle(";Injection Charge [pC]; ADC [ch]");
  int ch;
  double TP[6];
  double TP_err[6];
  TF1* fADC[6];
  double par[6][2];
  double par_err[6][2];
  for(int j=0; j<4; j++){
    gADC[j] = new TGraphErrors();
    gADC[j] -> SetName(Form("gADC_%d",j));
    gADC[j] ->SetMarkerStyle(20);
    gADC[j] ->SetMarkerColor(j+1);
    gADC[j] ->SetLineColor(j+1);

    gADC_TP[j] = new TGraphErrors();
    gADC_TP[j] -> SetName(Form("gADC_TP_%d",j));
    gADC_TP[j] ->SetMarkerStyle(43);
    gADC_TP[j] ->SetMarkerColor(j+1);
    gADC_TP[j] ->SetLineColor(j+1);


    fADC[j] = new TF1(Form("fADC_%d",j),"[0]*x+[1]",xmin,xmax);
    fADC[j] ->SetLineColor(j+1);
    fADC[j] ->SetLineStyle(2);
    ch = j%2;

    
    for(int i=0;i<6;i++){
      float IntegratedADC[8];
      T[i][j] = new TChain("T");
      T[i][j]->Add(rname[i][j].c_str());
      T[i][j] -> SetBranchAddress("IntegratedADC",&IntegratedADC);
      ENum = T[i][j]->GetEntries();
      hADC[i][j] = new TH1D(Form("hADC_%d_%d",i,j),"",1000,0,1000);
      
      for(int iev=0; iev<ENum; iev++){
	IntegratedADC[0] =0.0;
	IntegratedADC[1] =0.0;
	T[i][j]->GetEntry(iev);
	hADC[i][j]->Fill(IntegratedADC[ch]);
      }// iev
      TF1* f = new TF1("f","gausn(0)",0,1000);
      double mean = hADC[i][j] ->GetBinCenter(hADC[i][j]->GetMaximumBin());
      double sigma =10;
      hADC[i][j] -> Fit("f","QR","QR",mean-3.*sigma, mean+3.*sigma);
      mean = f->GetParameter(1);
      if(i<5) gADC[j] -> SetPoint(i,HV[i][j]*conv_factor,mean);
      else if(i==5 ){
	TP[j] = mean;
	TP_err[j] = f->GetParError(1);
      }
	//gADC_TP[j]-> SetPoint(0,HV[i][j]*conv_factor,mean);
      
    } // end i

    gADC[j] ->Fit(Form("fADC_%d",j),"QR","QR",xmin,xmax);
    par[j][0] = fADC[j]-> GetParameter(0);
    par[j][1] = fADC[j]-> GetParameter(1);
    par_err[j][0] = fADC[j]-> GetParError(0);
    par_err[j][1] = fADC[j]-> GetParError(1);
  } // end j



  // ============ Inner Capacity ============ //
  TF1* fCin[4];
  double Cin[4];
  double Cin_err[4];
  double HV_TP[4]={1.2,1.2,1.2,1.2}; // V
  for(int i=0; i<4; i++){
    fCin[i] = new TF1(Form("fCin_%d",i),"[0]*x",0,1000);// Q=CV V=[0], C=x[0]
    fCin[i]->SetParameter(0,1.2);// 1.2 V
    Cin[i] = (TP[i] - par[i][1])/par[i][0]/HV_TP[i];
    Cin_err[i] = Cin[i]*(par_err[i][0]/par[i][0] + (TP_err[i] + par_err[i][1])/(TP[i] - par[i][1]));
  }

  



  
  TLegend*leg = new TLegend(0.7,0.30,0.9,0.50,"","NDC");
  leg->AddEntry(gADC[0], "CH0 config1 file ","lp");
  leg->AddEntry(gADC[1], "CH1 config1 file ","lp");
  leg->AddEntry(gADC[2], "CH0 config2 file ","lp");
  leg->AddEntry(gADC[3], "CH1 config2 file ","lp");
  TLine* LTP[4];

  for(int i=0; i<4;i++){
    LTP[i] = new TLine(xmin,TP[i],xmax,TP[i]);
    LTP[i] ->SetLineColor(i+1);
    LTP[i] ->SetLineStyle(2);
  }


  TCanvas* c0 = new TCanvas("c0","c0",800,500);
  c0->cd();
  gADC_frame->Draw("AP");
  for(int j=0;j<4;j++){
  gADC[j]->Draw("P");
  fADC[j]->Draw("same");
  //LTP[j]->Draw("same");
  }

  //  gADC[1]->Draw("PL");
  //  gADC[2]->Draw("PL");
  //  gADC[3]->Draw("PL");
  

  /*
  TLine* l[4];
  for(int j=0;j<4;j++){
    l[j] =new TLine(xmin,TP[j],xmax,TP[j]);
    l[j]->SetLineColor(j+1);
    l[j]->SetLineStyle(2);
    l[j]->Draw("same");
  }
  */

  leg->Draw();
  c0->Print("calib1.pdf");
  
  TCanvas* c1 = new TCanvas("c1","c1",800,500);
  c1->Divide(2,2);

  for(int i=0;i<4;i++){
    c1->cd(i+1);
    gADC[i]->Draw("APL");
    //    l[i]->Draw("same");
  }
  c1->Print("calib2.pdf");

  // ========================================= //
  cout<<" ========== Fitting Results ==========="<<endl;
  cout<<"config 1 CH0 (p0*x+p1) : p0 "<<par[0][0]<<" p1 "<<par[0][1]<<endl;
  cout<<"config 1 CH1 (p0*x+p1) : p0 "<<par[1][0]<<" p1 "<<par[1][1]<<endl;
  cout<<"config 2 CH0 (p0*x+p1) : p0 "<<par[2][0]<<" p1 "<<par[2][1]<<endl;
  cout<<"config 2 CH1 (p0*x+p1) : p0 "<<par[3][0]<<" p1 "<<par[3][1]<<endl;


  cout<<"=========== TP Corr ==================="<<endl;
  cout<<"config 1 CH0 Cin : "<<Cin[0]<<" +- "<<Cin_err[0]<<endl;
  cout<<"config 1 CH1 Cin : "<<Cin[1]<<" +- "<<Cin_err[1]<<endl;
  cout<<"config 2 CH0 Cin : "<<Cin[2]<<" +- "<<Cin_err[2]<<endl;
  cout<<"config 2 CH1 Cin : "<<Cin[3]<<" +- "<<Cin_err[3]<<endl;

  
}


  

