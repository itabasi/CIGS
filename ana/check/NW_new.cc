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
extern double calcN(int st_i,float* C, float* V);

const double e = 1.6e-9;
const double epsilon_0 = 8.854188e-14; // F/m
const double ep_CIGS = 13.5*epsilon_0;
const double area = 0.005; // [cm2]


void NW_new(){ // Current timde dependence

  //  int runnum = 16;
  //  string name  =  "CIGS16";
  string name  =  "CIGS20";
  //  name = "CIGS20_0h_VW";
  // name = "CIGS20_1h_VW";
  //  name = "CIGS20_2h_VW";
  //  name = "CIGS20_KEK_VW";
  //  name = "CIGS15_woAnnealing";
  //  name = "CIGS22_0h_VW";

  //  string fname =    Form("./%s.txt",name.c_str());
  name = "CIGS_ID30_CH7_annealing2";
  //  name = "CIGS_ID30_CH6";
  //name = "CIGS_ID31_CH6";
  //  name = "CIGS_ID21_CH6_20240124";
  string fname =    Form("../../data/CV/%s.txt",name.c_str());
  string frname =   Form("../../rootfiles/ana/CV/%s_nw.root",name.c_str());
  TFile* ofr = new TFile(frname.c_str(),"recreate");
  string buf;
  float V[10000],C6[10000],C7[10000],W6[10000],W7[10000], N6[10000],N7[10000];
  int ii=0;
  vector<string>strvec;
  ifstream ifp(fname.c_str(),ios::in);
  TGraph* g6 = new TGraph();
  g6->SetName("g6");
  TGraph* g7 = new TGraph();
  g7->SetName("g7");
  g6->SetTitle("; Depletion layer width [#mum]; Carrior density [10^{16}./cm^{3}]");
  g7->SetTitle("; Depletion layer width [#mum]; Carrior density [10^{16}/cm^{3}]");
  
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

    if(ii>0){
      N6[ii]= calcN(ii,C6,V);
      N7[ii]= calcN(ii,C7,V);

      
    }else{
      N6[ii] = 0;
      N7[ii] = 0;
    }
    
    g6->SetPoint(ii,W6[ii],N6[ii]);
    g7->SetPoint(ii,W7[ii],N7[ii]);
    gW6->SetPoint(ii,V[ii],W6[ii]);
    gW7->SetPoint(ii,V[ii],W7[ii]);

    cout<<"ii "<<ii<<" V "<<V[ii]<<"N6 "<<N6[ii]<<endl;
    ii++;
      
  }


  
  // ====== Draw ======== //

  TLegend*leg = new TLegend(0.15,0.60,0.5,0.90,"","NDC");

  leg->AddEntry(g6, "CIGS Sample 16 ch6(P6) ","p");
  leg->AddEntry(g7, "CIGS Sample 16 ch7(P7) ","p");
  

  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  //  g6->GetYaxis()->SetRangeUser(0.1,1e2);
  //  gPad->SetLogy(1);
  gPad->SetGridy(1);
  g6->Draw("AP");
  //  g7->Draw("APL");
  //  leg->Draw();

  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  //  gW6->GetYaxis()->SetRangeUser(0,3);
  //  gPad->SetLogy(1);
  gPad->SetGridy(1);
  gW6->Draw("AP");
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

double calcN(int st_i, float* C, float* V){

  int nsample =10;
  //  if(st_i == 0) return 0;
  if(st_i <=5) nsample =st_i;
  TGraph* g =new TGraph();
  double N =0;
  TF1* f =new TF1("f","[0]*x+[1]",V[st_i],V[st_i+nsample]);
  if(st_i >nsample){
    double dV = V[st_i+nsample] - V[st_i];
    int n= 0;
    if(dV <=0) return 0;
    for(int i =st_i-nsample ; i<st_i; i++){
      if(C[i] <=0 )return 0;
      double C2 = area*area*(1/C[i]/C[i]);
      g->SetPoint(n,V[i],C2);
      n++;
    }
    double par0 = area*area*(1/C[st_i]/C[st_i] -1/C[st_i-nsample]/C[st_i-nsample])/(V[st_i] - V[st_i-nsample]);
    double par1 =area*area/C[st_i]/C[st_i] - par0*V[st_i];
    f->SetParameter(0,par0);
    f->SetParameter(1,par1);
      g->Fit(f,"QR","QR",V[st_i],V[st_i+nsample]);
      double dC_2 =f->GetParameter(0);
      N =- 2.0/e/ep_CIGS/epsilon_0/(dC_2/dV);
    }
    delete g;
    delete f;
    cout<<"ii "<<st_i<<" N "<<N<<endl;
    if(N<0 || N>1.0e20) return 0;
    return N*1.0e-16;
  
  
};


