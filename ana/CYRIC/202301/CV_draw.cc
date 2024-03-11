#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <TGraph.h>
const int nch =2;
const int nfile =4;



void CV_draw(){
  gStyle->SetPalette(kRainBow);
  TFile* ifr[10];
  string rname[10];

  rname[0] ="./RootFiles/CIGS20_KEK.root";
  rname[1] ="./RootFiles/CIGS20_0h_CH6.root";
  rname[2] ="./RootFiles/CIGS20_1h_CH6.root";
  rname[3] ="./RootFiles/CIGS20_2h_CH6.root";


  TGraphErrors* gCH6[10];
  TGraphErrors* gCH7[10];
  
  for(int i = 0; i < 4; i++){
    ifr[i] = new TFile(rname[i].c_str());
    gCH6[i] = (TGraphErrors*)ifr[i]->Get("g6");
    gCH6[i] -> SetName(Form("gCH6_%d",i));
    gCH6[i] -> SetMarkerColor(kRainBow);
    gCH6[i] -> SetMarkerStyle(20);
    gCH6[i] -> SetLineColor(0);

    gCH7[i] = (TGraphErrors*)ifr[i]->Get("g7");
    gCH7[i] -> SetName(Form("gCH7_%d",i));
    gCH7[i] -> SetMarkerColor(kRainBow);
    gCH7[i] -> SetMarkerStyle(20);
    gCH7[i] -> SetLineColor(0);    
  }



  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  gCH6[0]->Draw("PCL PMC AP");
  for(int i = 0; i <4 ; i++){
    gCH6[i]->Draw("PCL PMC P");
  }

  TCanvas* c2 = new TCanvas("c2","c2");
  c2->cd();  
  gCH6[0]->Draw("PCL PMC AP");
  gCH6[1]->Draw("PCL PMC P");
  gCH7[0]->Draw("PCL PMC P");
  gCH7[1]->Draw("PCL PMC P");  
}
