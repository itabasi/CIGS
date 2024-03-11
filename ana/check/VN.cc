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
extern double calcN(double C1,double C2, double V1,double V2);

const double e = 1.6e-9;
const double epsilon_0 = 8.854188e-14; // F/m
const double ep_CIGS = 13.5*epsilon_0;
const double area = 0.005; // [cm2]





void VN(){

  //  string name ="CIGS_ID33_CH6";
  string name ="CIGS_ID33_CH6";
  name = "CIGS_ID21_CH6_20240124";

  string fname =    Form("../../data/CV/%s.txt",name.c_str());
  string frname =   Form("../../rootfiles/ana/CV/%s_vn.root",name.c_str());
  TFile* ofr = new TFile(frname.c_str(),"recreate");


    string buf;
  float V[10000],C6[10000],C7[10000],N6[10000],N7[10000];
  int ii=0;
  vector<string>strvec;
  ifstream ifp(fname.c_str(),ios::in);
  TGraph* g6 = new TGraph();
  g6->SetName("g6");
  TGraph* g7 = new TGraph();
  g7->SetName("g7");
  g6->SetTitle("; HV [V];  1/C^{2} \times10^15[cm^{4}/F^{2}]");
  g7->SetTitle("; HV [V];  1/C^{2} \times10^15[cm^{4}/F^{2}]");
  g6 -> SetMarkerStyle(7);
  g6 -> SetMarkerColor(4);
  g6 -> SetLineColor(4);
  g7 -> SetMarkerStyle(7);
  g7 -> SetMarkerColor(2);
  g7 -> SetLineColor(2);


  TGraph* gN6 = new TGraph();
  gN6->SetName("gN6");
  TGraph* gN7 = new TGraph();
  gN7->SetName("gN7");
  gN6->SetTitle("; HV [V]; [1/cm^{3}]");
  gN7->SetTitle("; HV [V]; [1/cm^{3}]");
  gN6 -> SetMarkerStyle(7);
  gN6 -> SetMarkerColor(4);
  gN6 -> SetLineColor(4);
  gN7 -> SetMarkerStyle(7);
  gN7 -> SetMarkerColor(2);
  gN7 -> SetLineColor(2);

  

  cout<<"================"<<endl;
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
    if(ii > 0 ){
      N6[ii]= calcN(C6[ii],C6[ii-1],V[ii],V[ii-1]);
      N7[ii]= calcN(C7[ii],C7[ii-1],V[ii],V[ii-1]);
      g6->SetPoint(ii,V[ii],area*area/C6[ii]/C6[ii]*1e-15);
      g7->SetPoint(ii,V[ii],area*area/C7[ii]/C7[ii]*1e-15);
      gN6->SetPoint(ii,V[ii],N6[ii]);
      gN7->SetPoint(ii,V[ii],N7[ii]);
    }
      ii++;
      
  }


  TLegend*leg = new TLegend(0.15,0.60,0.5,0.90,"","NDC");

  leg->AddEntry(g6, "CIGS Sample 16 ch6(P6) ","p");
  leg->AddEntry(g7, "CIGS Sample 16 ch7(P7) ","p");
  

  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  //  gW6->GetYaxis()->SetRangeUser(0,3);
  //  gPad->SetLogy(1);
  gPad->SetGridy(1);
  gN6->Draw("APL");
  //  g7->Draw("APL");
  //  leg->Draw();

  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  //  gW6->GetYaxis()->SetRangeUser(0,3);
  //  gPad->SetLogy(1);
  gPad->SetGridy(1);
  g6->Draw("APL");
  //  g7->Draw("APL");
  //  leg->Draw();


  
  g6->Write();
  g7->Write();
  gN6->Write();
  gN7->Write();
  ofr->Write();




};



double calcN(double C1,double C2, double V1,double V2){



  double dV = V2- V1;
  if(C1<=0 || C2 <= 0 || dV <=0) return 0;
  double dC_2 = area*area*(1/C2/C2 - 1/C1/C1);
  double N =2.0/e/ep_CIGS/epsilon_0/(dC_2/dV);
  if(N>0 ) return 0;
  return -N;

  
};

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
