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
extern double calcWidth(double Cp);



const double epsilon_0 = 8.854188e-14; // F/m
const double ep_CIGS = 13.5*epsilon_0;
const double area = 0.005; // [cm2]


void VW(){ // Current timde dependence

  //  int runnum = 16;
  //  string name  =  "CIGS16";
  string name  =  "CIGS20";
  //  name = "CIGS20_0h_VW";
  // name = "CIGS20_1h_VW";
  //  name = "CIGS20_2h_VW";
  name = "CIGS20_KEK_VW";
  //  name = "CIGS15_woAnnealing";
  //  name = "CIGS22_0h_VW";

  //  string fname =    Form("./%s.txt",name.c_str());
  //  name = "CIGS_ID30_CH6";
    name = "CIGS_ID33_CH6";
  string fname =    Form("./CVdata/alpha/%s.txt",name.c_str());
  string frname =   Form("./RootFiles/%s.root",name.c_str());
  TFile* ofr = new TFile(frname.c_str(),"recreate");
  
  string buf;
  float V[10000],C6[10000],C7[10000],W6[10000],W7[10000];
  int ii=0;
  vector<string>strvec;
  ifstream ifp(fname.c_str(),ios::in);
  TGraph* g6 = new TGraph();
  g6->SetName("g6");
  TGraph* g7 = new TGraph();
  g7->SetName("g7");
  g6->SetTitle("; HV [V]; Depletion layer width [#mum]");
  g7->SetTitle("; HV [V]; Depletion layer width [#mum]");
  g6 -> SetMarkerStyle(7);
  g6 -> SetMarkerColor(4);
  g6 -> SetLineColor(4);
  g7 -> SetMarkerStyle(7);
  g7 -> SetMarkerColor(2);
  g7 -> SetLineColor(2);


  TGraph* gW6 = new TGraph();
  gW6->SetName("gW6");
  TGraph* gW7 = new TGraph();
  gW7->SetName("gW7");
  gW6->SetTitle("; HV [V]; Depletion layer width [#mum]");
  gW7->SetTitle("; HV [V]; Depletion layer width [#mum]");
  gW6 -> SetMarkerStyle(7);
  gW6 -> SetMarkerColor(4);
  gW6 -> SetLineColor(4);
  gW7 -> SetMarkerStyle(7);
  gW7 -> SetMarkerColor(2);
  gW7 -> SetLineColor(2);

  
  while(1){
    getline(ifp,buf);
    if(ifp.eof() || buf.size()==0)break;
    strvec = split(buf,',');
    if(strvec.size()==3){
      V[ii] = stof(strvec.at(0));
      C6[ii] = stof(strvec.at(1));
      C7[ii] = stof(strvec.at(2));
    }else if (strvec.size()==2){
      V[ii] = stof(strvec.at(0));
      C6[ii] = stof(strvec.at(1));
      C7[ii] = 0.0;
    }
    W6[ii]= calcWidth(C6[ii]);
    W7[ii]= calcWidth(C7[ii]);
    g6->SetPoint(ii,V[ii],C6[ii]);
    g7->SetPoint(ii,V[ii],C7[ii]);
    gW6->SetPoint(ii,V[ii],W6[ii]);
    gW7->SetPoint(ii,V[ii],W7[ii]);

      ii++;
      
  }


  
  // ====== Draw ======== //

  TLegend*leg = new TLegend(0.15,0.60,0.5,0.90,"","NDC");

  leg->AddEntry(g6, "CIGS Sample 16 ch6(P6) ","p");
  leg->AddEntry(g7, "CIGS Sample 16 ch7(P7) ","p");
  

  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  gW6->GetYaxis()->SetRangeUser(0,3);
  //  gPad->SetLogy(1);
  gPad->SetGridy(1);
  gW6->Draw("APL");
  //  g7->Draw("APL");
  //  leg->Draw();


  
  g6->Write();
  g7->Write();
  gW6->Write();
  gW7->Write();
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



double calcWidth(double Cp){

  if(Cp <= 0) return 0;
  double W = ep_CIGS/Cp*area;
  return W *1.0e4; // [mum]

  
  
};
