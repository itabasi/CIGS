

void CalcBeam(){

  TF1* fgaus = new TF1("fgaus","gausn(0)",-10,10);

  double width = 1.0; // mm
  double mean[3] = {-1.5,1.5,0.0};
  TF1* fP0 = new TF1("fP0","fgaus",mean[0] - width/2., mean[0] + width/2.);
  TF1* fP1 = new TF1("fP1","fgaus",mean[1] - width/2., mean[1] + width/2.);
  fP0->SetFillColor(2);
  fP0->SetNpx(2000);
  fP0->SetFillStyle(3002);
  fP1->SetFillColor(4);
  fP1->SetFillStyle(3002);
  fP1->SetNpx(2000);
  
  double sum[3];
  TGraph*g0 = new TGraph();
  g0->SetName("g0");
  g0->SetMarkerStyle(20);
  g0->SetMarkerColor(2);
  TGraph*g1 = new TGraph();
  g1->SetName("g1");
  g1->SetMarkerStyle(20);
  g1->SetMarkerColor(4);

  TGraph*gR = new TGraph();
  gR->SetName("gR");
  gR->SetTitle(";Peak Shift [mm];Particle Ratio [N0/N1]");
  gR->SetMarkerStyle(20);
  gR->SetMarkerColor(1);

  TGraph*gR2 = new TGraph();
  gR2->SetName("gR2");
  gR2->SetTitle(";Peak width (phi:diameter) [mm];Particle Ratio [N0/N1]");
  gR2->SetMarkerStyle(20);
  gR2->SetMarkerColor(1);


  
  int n=10;
  for(int i =0; i<n; i++){
    double p0 = 1;
    //    double p1 = (double)i/(double)n*3.0;
    //    double p2 = 4./3.;
    double p1 = 0.0;
    double p2 = (double)i/(double)n*10/3.; //sigma

    
    fgaus->SetParameter(0,p0);
    fgaus->SetParameter(1,p1);
    fgaus->SetParameter(2,p2);

    fP0->SetParameter(0,p0);
    fP0->SetParameter(1,p1);
    fP0->SetParameter(2,p2);
    
    fP1->SetParameter(0,p0);
    fP1->SetParameter(1,p1);
    fP1->SetParameter(2,p2);

    sum[0] = fP0->Integral(mean[0] - width/2. , mean[0] + width/2.);
    sum[1] = fP1->Integral(mean[1] - width/2. , mean[1] + width/2.);
    sum[2] = fgaus->Integral(-1,1);

    g0->SetPoint(i,p1,sum[0]);
    g1->SetPoint(i,p1,sum[1]);
    gR->SetPoint(i,p1,sum[0]/sum[1]);
    gR2->SetPoint(i,p2*3.0*2.0,sum[2]);
    
  }


  double p0 = 1;
  double p1 = 1.;
  double p2 = 4./3.;
    
  fgaus->SetParameter(0,p0);
  fgaus->SetParameter(1,p1);
  fgaus->SetParameter(2,p2);

  fP0->SetParameter(0,p0);
  fP0->SetParameter(1,p1);
  fP0->SetParameter(2,p2);
    
  fP1->SetParameter(0,p0);
  fP1->SetParameter(1,p1);
  fP1->SetParameter(2,p2);






  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  fgaus->SetNpx(2000);
  fgaus->SetLineColor(1);
  fgaus->Draw();

  fP0->Draw("same");
  fP1->Draw("same");

  
  //  cout<<"sum 0 "<<sum[0]<<endl;
  //  cout<<"sum 1 "<<sum[1]<<endl;
  //  cout<<"ratio sum0/sum1 "<<sum[0]/sum[1]<<endl;


  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  gR->Draw("AP");

  TCanvas* c2 = new TCanvas("c2","c2");
  c2->cd();
  gR2->Draw("AP");

  
  
}
