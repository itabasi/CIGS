
const int nch =8;

void ADChist(){


  string buf;
  int s=0;
  string runlist="./runlist/CIGS_Rad.txt";
  //    string runlist="./runlist/CIGS5mum_Rad.txt";
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
  TGraphErrors* gDiff = new TGraphErrors();
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




  // ================================================= //


  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();


  hIntegratedADC[1][0]->SetLineColor(1);
  hIntegratedADC[1][0]->SetFillColor(1);
  hIntegratedADC[1][0]->SetFillStyle(3002);

  hIntegratedADC[1][16]->SetLineColor(2);
  hIntegratedADC[1][16]->SetFillColor(2);
  hIntegratedADC[1][16]->SetFillStyle(3002);

  hIntegratedADC[1][18]->SetLineColor(4);
  hIntegratedADC[1][18]->SetFillColor(4);
  hIntegratedADC[1][18]->SetFillStyle(3002);
  hIntegratedADC[1][16]->Draw("");
  hIntegratedADC[1][0]->Draw("same");
  //  hIntegratedADC[1][18]->Draw("same");

  // ================================================= //
  
  

}
