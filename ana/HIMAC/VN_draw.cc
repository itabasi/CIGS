#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <TGraph.h>
const int nch =2;
const int nfile =4;

void VN_draw(){
  gStyle->SetPalette(kRainBow);
  
  string rname[nfile];
  TFile* f[nfile];

  rname[0] = "~/work/SENSOR/CIGS/Ana/analysis/itamacro/RootFiles/CIGS20_KEK_VW.root";
  rname[1] = "~/work/SENSOR/CIGS/Ana/analysis/itamacro/RootFiles/CIGS20_0h_VW.root";
  rname[2] = "~/work/SENSOR/CIGS/Ana/analysis/itamacro/RootFiles/CIGS20_1h_VW.root";
  rname[3] = "~/work/SENSOR/CIGS/Ana/analysis/itamacro/RootFiles/CIGS20_2h_VW.root";

  TGraph* gCH0[nfile];
  TGraph* gCH1[nfile];

  TGraph* gCH0_s[nfile];
  TGraph* gCH1_s[nfile];  

  for(int i=0; i<nfile; i++){

    f[i] = new TFile(rname[i].c_str());
    gCH0[i] = (TGraph*)f[i]->Get("g6");
    gCH0[i] -> SetName(Form("gCH0_%d",i));
    gCH0[i] -> SetMarkerColor(kRainBow);
    gCH0[i] -> SetLineColor(kRainBow);
    gCH0[i] -> SetMarkerStyle(20);
    gCH1[i] = (TGraph*)f[i]->Get("g7");
    gCH1[i] -> SetName(Form("gCH1_%d",i));    
    gCH1[i] -> SetMarkerColor(kRainBow);
    gCH1[i] -> SetLineColor(kRainBow);
    gCH1[i] -> SetMarkerStyle(20);

    gCH0_s[i] = new TGraph();
    gCH0_s[i] ->SetName(Form("gCH0_s_%d",i));
    gCH0_s[i] -> SetMarkerColor(kRainBow);
    gCH0_s[i] -> SetLineColor(kRainBow);
    gCH0_s[i] -> SetMarkerStyle(20);
    gCH1_s[i] = new TGraph();
    gCH1_s[i] ->SetName(Form("gCH1_s_%d",i));     
    gCH1_s[i] -> SetMarkerColor(kRainBow);
    gCH1_s[i] -> SetLineColor(kRainBow);
    gCH1_s[i] -> SetMarkerStyle(20);
    
  }



  // ============ Scaliang =============== //

  double scale[2] ={0.0,0.0};
  int Nbin[2];
  Nbin[0] = gCH0[0] -> GetN();
  Nbin[1] = gCH1[0] -> GetN();

  for(int i=0;i<Nbin[0]; i++){
    double y0 = gCH0[0]->GetPointY(i);
    double y1 = gCH1[0]->GetPointY(i);
    if(scale[0] < y0) scale[0] =y0;
    if(scale[1] < y1) scale[1] =y1;
  }




    for(int j=0; j<nfile; j++){
      Nbin[0] = gCH0[j] -> GetN();
      for(int i=0;i<Nbin[0]; i++){
	double x0 = gCH0[j]->GetPointX(i);
	double y0 = gCH0[j]->GetPointY(i);
	double x1 = gCH1[j]->GetPointX(i);
	double y1 = gCH1[j]->GetPointY(i);
	gCH0_s[j] -> SetPoint(i,x0,y0/(scale[0]/2.));
	gCH1_s[j] -> SetPoint(i,x1,y1/(scale[1]/2.));
	if(j<2)
	  cout<<"j "<<j<<" x "<<x0<<" y0 "<<y0/scale[0]*2.<<" y1 "<<y1/scale[1]*2.<<endl;
      }
    }
  
  // ====================================== //

  
  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  gCH0[0]->Draw("PCL PMC AP");
  gCH1[0]->Draw("PCL PMC P");
  gCH0[1]->Draw("PCL PMC P");
  gCH1[1]->Draw("PCL PMC P");  
  //  gCH0[2]->Draw("PCL PMC P");
  //  gCH0[2]->Draw("PCL PMC P");
  gCH1[3]->Draw("PCL PMC P");
  gCH1[3]->Draw("PCL PMC P");  
  

  TLegend*leg = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
  leg->AddEntry(gCH0[0], "CIGS 2#mum CH0 before irradiation ","pe");
  leg->AddEntry(gCH1[0], "CIGS 2#mum CH1 before irradiation","pe");
  leg->AddEntry(gCH0[1], "CIGS 2#mum CH0 after  irradiation ","pe");
  leg->AddEntry(gCH1[1], "CIGS 2#mum CH1 after  irradiation ","pe");
  //  leg->AddEntry(gCH0[2], "CIGS 2#mum CH0 after 130C anealing (1h) ","pe");
  //  leg->AddEntry(gCH1[2], "CIGS 2#mum CH1 after 130C anealing (1h)","pe");
  leg->AddEntry(gCH0[3], "CIGS 2#mum CH0 after 130C anealing (2h) ","pe");
  leg->AddEntry(gCH1[3], "CIGS 2#mum CH1 after 130C anealing (2h) ","pe");
    
  leg->Draw();




  TCanvas* c2 = new TCanvas("c2","c2");
  c2->cd();
  gCH0_s[0]->Draw("PCL PMC AP");
  gCH0_s[1]->Draw("PCL PMC P");
  gCH0_s[3]->Draw("PCL PMC P");
  gCH1_s[0]->Draw("PCL PMC P");
  gCH1_s[1]->Draw("PCL PMC P");  
  gCH1_s[3]->Draw("PCL PMC P");    

  TLegend*leg2 = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
  leg2->AddEntry(gCH0_s[0], "CIGS 2#mum CH0 before irradiation ","pe");
  leg2->AddEntry(gCH0_s[1], "CIGS 2#mum CH1 after  irradiation ","pe");
  leg2->AddEntry(gCH1_s[0], "CIGS 2#mum CH0 before irradiation","pe");
  leg2->AddEntry(gCH1_s[1], "CIGS 2#mum CH1 after  irradiation ","pe");
  leg2->AddEntry(gCH0_s[3], "CIGS 2#mum CH0 after 130C anealing (2h) ","pe");
  leg2->AddEntry(gCH1_s[3], "CIGS 2#mum CH1 after 130C anealing (2h) ","pe");
  
  leg2->Draw();


}

