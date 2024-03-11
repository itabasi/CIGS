
const int nch =8;

void IntegratedADC(){


  string buf;
  int s=0;
  //  string runlist="./runlist/CIGS_Rad.txt";
  string runlist="../runlist/CIGS_Rad_test.txt";
  //  string runlist="./runlist/CIGS5mum_Rad.txt";
  string fname[1000];
  TFile* f[100];
  TH1D* hIntegratedADC[nch][100];
  TH1D* hPH[nch][100];
  TGraphErrors* g[nch];
  TGraphErrors* gRatio[nch];
  TGraphErrors* gPH[nch];
  TGraphErrors* gPHRatio[nch];
  TGraphErrors* gRatio_frame = new TGraphErrors();
  TGraphErrors* g_frame = new TGraphErrors();
  TGraphErrors* gPHRatio_frame = new TGraphErrors();
  TGraphErrors* gPH_frame = new TGraphErrors();
  for(int i=0;i<nch;i++){
    g[i] = new TGraphErrors();
    gRatio[i] = new TGraphErrors();
    gPH[i] = new TGraphErrors();
    gPHRatio[i] = new TGraphErrors();
    g[i] -> SetMarkerColor(i+1);
    g[i] -> SetLineColor(i+1);
    g[i] -> SetMarkerStyle(2);

    gRatio[i] -> SetName(Form("gRatio_%d",i));
    gRatio[i] -> SetMarkerColor(i+1);
    gRatio[i] -> SetLineColor(i+1);
    gRatio[i] -> SetMarkerStyle(2);

  }
  ifstream ifp(runlist.c_str(),ios::in);
  while(1){
    getline(ifp,buf);
    if(buf[0]=='#'){continue;}
    if(ifp.eof()) break;
    stringstream sbuf(buf);
    sbuf >> fname[s] ;
    cout<<"fname : "<<fname[s]<<endl;
    f[s] = new TFile(fname[s].c_str());
    for(int i=0;i<nch;i++){
      hIntegratedADC[i][s] = (TH1D*)f[s]->Get(Form("hIntegratedADC_%d",i));
      hIntegratedADC[i][s] -> SetName(Form("hIntegratedADC_%d_%d",i,s));
      hPH[i][s] = (TH1D*)f[s]->Get(Form("hPeakHeight_%d",i));
      hPH[i][s] -> SetName(Form("hPH_%d_%d",i,s));
    }


    
    s++;


    
  }



  g_frame ->SetPoint(0,0,100);
  g_frame ->SetPoint(1,s-1,500);
  g_frame->SetTitle("; ; ");
  g_frame->GetXaxis()->SetLabelSize(0);
  gRatio_frame ->SetPoint(0,0,0.0);
  gRatio_frame ->SetPoint(1,s-1,1.0);

  gPH_frame ->SetPoint(0,0,100);
  gPH_frame ->SetPoint(1,s-1,500);
  gPH_frame->SetTitle("; ; ");
  gPH_frame->GetXaxis()->SetLabelSize(0);
  gPHRatio_frame ->SetPoint(0,0,0.1);
  gPHRatio_frame ->SetPoint(1,s-1,1.0);



  // ======= Fitting ============== //


  TF1* fit = new TF1("fit","gausn(0)",0,10000);
  double peak[nch][100];
  double peak_err[nch][100];
  double peakPH[nch][100];
  double peakPH_err[nch][100];

  for(int i=0;i<s;i++){
    for(int j=0;j<2;j++){
      //      hIntegratedADC[j][i] ->Fit("fit","QR","QR",300,700);
            hIntegratedADC[j][i] ->Fit("fit","QR","QR",100,700);
      peak[j][i] = fit->GetParameter(1);
      peak_err[j][i] = fit->GetParError(1);
      g[j] -> SetPoint(i,i,peak[j][i]);
      g[j] -> SetPointError(i,0,peak_err[j][i]);
      gRatio[j] -> SetPoint(i,i,peak[j][i]/peak[j][0]);
      gRatio[j] -> SetPointError(i,0,(peak[j][i]/peak[j][0])*(peak_err[j][i]/peak[j][i] + peak_err[j][0]/peak[j][0]));

      hPH[j][i] ->Fit("fit","QR","QR",100,700);
      peakPH[j][i] = fit->GetParameter(1);
      peakPH_err[j][i] = fit->GetParError(1);
      gPH[j] -> SetPoint(i,i,peakPH[j][i]);
      gPH[j] -> SetPointError(i,0,peakPH_err[j][i]);
      gPHRatio[j] -> SetPoint(i,i,1 - fabs((peakPH[j][i]-peakPH[j][0])/peakPH[j][0]));
      gPHRatio[j] -> SetPointError(i,0,(peakPH[j][i]/peakPH[j][0])*(peakPH_err[j][i]/peakPH[j][i] + peakPH_err[j][0]/peakPH[j][0]));
      


    }
  }



  // =============================== //

  

  


  TCanvas* c[s];
  /*
  for(int i=0;i<s;i++){
    c[i]= new TCanvas(Form("c%d",i),Form("c%d",i));
    c[i]->cd();
    hIntegratedADC[1][i]->Draw();
  }
  */
  

  TCanvas* cx= new TCanvas("cx","cx");
  cx->cd();
  //  gPHRatio_frame->Draw("AP");
  gPHRatio[0] -> Draw("AP");
  gPHRatio[1] -> Draw("P");



  TCanvas*  c1=new TCanvas("c1","",10,30,1000,800);
  double sepa=0.7;//上下の比
  TPad *pad0 = new TPad("pad0", "pad0",0.0,0.95-sepa,0.99,0.99, 0,0.01,0);
  pad0->Draw();
  pad0->SetNumber(1);//番号をつけてidする

  c1->cd();
  TPad *pad1 = new TPad("ratio", "ratio",0.0,0.05,0.95,0.95-sepa, 0,0.01,0);//divide(bottom)
  pad1->Draw();
  pad1->SetNumber(2);

  c1->cd(1);
  g_frame->Draw("AP");
  g[0]->Draw("PL");
  g[1]->Draw("PL");

  c1->cd(2);
  gRatio_frame->Draw("AP");
  gRatio[0]->Draw("PL");
  gRatio[1]->Draw("PL");
  gRatio_frame->GetYaxis()->SetLabelSize(0.1);
  gRatio_frame->GetXaxis()->SetLabelSize(0.1);


  TFile* ofr = new TFile("./test.root","recreate");

  //  ofr->Write();
  cout<<"end"<<endl;

  

}
