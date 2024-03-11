


const int nfiles =10;
const int nplots =10;
void ADC_Vdep(){

  // ----- ID21 CH6 ------------ //
  double HV[nplots] = {-0.13,-0.5,-1.65};
  double ADC[nplots]={931.0,954,1000.0};
  double ADC2[nplots]={920,1000,1165.0};
  double ADC_H[nplots]={144,180,243};
  double W[nplots] ={1.029,1.98,2.56};
  double ADC_sigma[nplots] ={};
  TGraphErrors* g = new TGraphErrors();
  TGraphErrors* gS = new TGraphErrors();
  TGraphErrors* gH = new TGraphErrors();
  TGraphErrors* gHS = new TGraphErrors();
  for (int i=0;i<nplots;i++){
    g->SetPoint(i,HV[i],ADC2[i]);
    gS->SetPoint(i,HV[i],ADC2[i]/W[i]);
    gH->SetPoint(i,HV[i],ADC_H[i]);
    gHS->SetPoint(i,HV[i],ADC_H[i]/W[i]);
  }
  g->SetLineColor(2);
  g->SetMarkerColor(2);
  g->SetMarkerStyle(20);


  gH->SetLineColor(2);
  gH->SetMarkerColor(2);
  gH->SetMarkerStyle(20);


  gS->SetLineColor(2);
  gS->SetMarkerColor(2);
  gS->SetMarkerStyle(20);


  gHS->SetLineColor(4);
  gHS->SetMarkerColor(4);
  gHS->SetMarkerStyle(20);



  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  g->Draw("AP");


  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  gH->Draw("AP");

  TCanvas* c2 = new TCanvas("c2","c2");
  c2->cd();
  gHS->Draw("AP");

  TCanvas* c3 = new TCanvas("c3","c3");
  c3->cd();
  gS->Draw("AP");
  
  
}
