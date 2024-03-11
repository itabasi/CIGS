#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <TGraph.h>

extern vector<string> split(string&input, char delimiter);
extern float sec_to_hour(int sec);
extern float sec_to_min(int sec);


void CV(){ // Current timde dependence

  //  int runnum = 16;
  //  string name  =  "CIGS16";
  string name  =  "CIGS20";
  name = "CIGS14_wGamma";
  name = "CIGS14_woGamma";
  name = "CIGS15_wGamma";
  name = "CIGS15_woGamma";
  name = "CIGS20_0h_CH6";
  name = "CIGS20_1h_CH6";
  name = "CIGS20_2h_CH6";
  name = "CIGS20_KEK";

  
  string fname =    Form("./%s.txt",name.c_str());
  string frname =   Form("./RootFiles/%s.root",name.c_str());
  TFile* ofr = new TFile(frname.c_str(),"recreate");
  
  string buf;
  float V[10000],C6[10000],C7[10000];
  int ii=0;
  vector<string>strvec;
  ifstream ifp(fname.c_str(),ios::in);
  TGraph* g6 = new TGraph();
  g6->SetName("g6");
  TGraph* g7 = new TGraph();
  g7->SetName("g7");
  g6->SetTitle("; HV [V]; 1/C^{2} [10^{16} (F/cm^{2})^{-2}");

  TGraph* gCV_6 = new TGraph();
  gCV_6->SetName("gCV_6");
  TGraph* gCV_7 = new TGraph();
  gCV_7->SetName("gCV_7");
  gCV_7->SetTitle("; HV [V]; C [10^{16} (F/cm^{2})");

  //  g6->SetTitle("; HV [V]; Current [A]");
  g6 -> SetMarkerStyle(7);
  g6 -> SetMarkerColor(4);
  g6 -> SetLineColor(4);
  g7 -> SetMarkerStyle(7);
  g7 -> SetMarkerColor(2);
  g7 -> SetLineColor(2);

  gCV_6 -> SetMarkerStyle(7);
  gCV_6 -> SetMarkerColor(4);
  gCV_6 -> SetLineColor(4);
  gCV_7 -> SetMarkerStyle(7);
  gCV_7 -> SetMarkerColor(2);
  gCV_7 -> SetLineColor(2);


  while(1){
    getline(ifp,buf);
    if(ifp.eof() || buf.size()==0)break;
    strvec = split(buf,',');
      V[ii] = stof(strvec.at(0));
      C6[ii] = stof(strvec.at(1))/1.e16;
      C7[ii] = stof(strvec.at(2))/1.e16;
      g6->SetPoint(ii,V[ii],C6[ii]);
      g7->SetPoint(ii,V[ii],C7[ii]);
      gCV_6->SetPoint(ii,V[ii],1./sqrt(C6[ii]));
      gCV_7->SetPoint(ii,V[ii],1./sqrt(C7[ii]));
      ii++;
      
  }


  
  // ====== Draw ======== //

  TLegend*leg = new TLegend(0.15,0.60,0.5,0.90,"","NDC");

  leg->AddEntry(g6, "CIGS Sample 16 ch6(P6) ","p");
  leg->AddEntry(g7, "CIGS Sample 16 ch7(P7) ","p");
  

  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  g6->GetYaxis()->SetRangeUser(0,10);
  //  gPad->SetLogy(1);
  gPad->SetGridy(1);
  g6->Draw("ALP");
  //  g7->Draw("LP");
  //  leg->Draw();

  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  //  gCV_6->GetYaxis()->SetRangeUser(0,10);
  gPad->SetLogy(1);
  gPad->SetGridy(1);
  gCV_6->Draw("ALP");
  gCV_7->Draw("LP");

  
  g6->Write();
  g7->Write();
  gCV_6->Write();
  gCV_7->Write();  
  ofr->Write();
  
  
}


vector<string> split(string&input, char delimiter){

  istringstream stream(input);
  string field;
  vector<string> result;
  while(getline(stream,field,delimiter))
    result.push_back(field);

  return result;
};


float sec_to_hour(int sec){

  float hour;

  float conv_sec_to_hour = 3600.;

  hour = (float)sec/conv_sec_to_hour;

  return hour;

};




float sec_to_min(int sec){

  float min;

  float conv_sec_to_min = 60.;

  min = (float)sec/conv_sec_to_min;

  return min;

};



