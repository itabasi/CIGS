#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <TGraph.h>
extern vector<string> split(string&input, char delimiter);

void IV_solar(){

  string fname =
    //    "~/work/CIGS/MPPC/Serial_2410/data/20221003_17:20:16.txt";
    //        "../../../Serial_2410/data/20221005_13:17:27.txt";
    //            "../../../Serial_2410/data/20221124_23:38:11.txt";
    "../../../Serial_2410/data/20230211_23:24:14.txt";
  //    string fname ="./test.txt";

  string fname_dark =     "../../../Serial_2410/data/20230211_23:27:16.txt";
  
  string buf;
  float HV[1000],I[1000];
  int ii=0;
  float HV_OFF=0.0;
  vector<string>strvec;
  ifstream ifp(fname.c_str(),ios::in);
  ifstream ifp_dark(fname_dark.c_str(),ios::in);
  TGraph* g = new TGraph();
  TGraph* gmuA = new TGraph();
  TGraph* gW = new TGraph();
  g->SetTitle("; HV[V]; Current [A]");
  g -> SetMarkerStyle(20);
  g -> SetMarkerColor(4);
  gmuA -> SetMarkerStyle(20);
  gmuA -> SetMarkerColor(4);
  gW -> SetMarkerStyle(20);
  gW -> SetMarkerColor(4);    
  while(1){
    getline(ifp,buf);
    if(ifp.eof() || buf.size()==0)break;

    strvec = split(buf,',');
    HV[ii] =-100.;
    I[ii]  =-100.;
    if(strvec.size()>1){
      HV[ii] = stof(strvec.at(0));
      I[ii]  = stof(strvec.at(1));
      //      g->SetPoint(ii,HV[ii] + HV_OFF,I[ii]);
      g->SetPoint(ii,HV[ii] + HV_OFF,fabs(I[ii]));
      gmuA->SetPoint(ii,HV[ii] + HV_OFF,I[ii]*1.e3);
      gW->SetPoint(ii,HV[ii] + HV_OFF,HV[ii]*I[ii]);
      ii++;

    }
  }

  // ====== Draw ======== //
  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  //  g->GetYaxis()->SetRangeUser(1.e-13,1.e-1);
  //  gPad->SetLogy(1);
  //  g->Draw("APL");
  gW->Draw("APL");
  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  //  gmu->GetYaxis()->SetRangeUser(1.e-13,1.e-1);
  //  gPad->SetLogy(1);
  gmuA->Draw("APL");  

  
}


vector<string> split(string&input, char delimiter){

  istringstream stream(input);
  string field;
  vector<string> result;
  while(getline(stream,field,delimiter))
    result.push_back(field);

  return result;
};
