#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <TGraph.h>
extern vector<string> split(string&input, char delimiter);

void IV(){

  
  string fname =
    //    "../../Serial_2410/data/20240125_06:04:50.txt"; // Ga-rich CIGS
    //        "../../Serial_2410/data/20240125_05:38:22.txt"; // Ga-rich CIGS
            "../../Serial_2410/data/20221110_14:04:18.txt"; // Ga-rich CIGS
  string buf;
  float HV[1000],I[1000];
  int ii=0;
  float HV_OFF=-1.65;
  vector<string>strvec;
  ifstream ifp(fname.c_str(),ios::in);
  TGraph* g = new TGraph();
  g->SetTitle("; HV[V]; Current [nA]");
  g -> SetMarkerStyle(20);
  g -> SetMarkerColor(4);
  g -> SetName("gIV");
  while(1){
    getline(ifp,buf);
    if(ifp.eof() || buf.size()==0)break;

    strvec = split(buf,',');
    HV[ii] =-100.;
    I[ii]  =-100.;
    if(strvec.size()>1){
      HV[ii] = stof(strvec.at(0));
      I[ii]  = stof(strvec.at(1));
      I[ii] = I[ii] * 1.0e9; // -A to nA
      g->SetPoint(ii,HV[ii] + HV_OFF,I[ii]);
      //g->SetPoint(ii,HV[ii] + HV_OFF,fabs(I[ii]));
      ii++;

    }
  }

  // ====== Draw ======== //
  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  //  g->GetYaxis()->SetRangeUser(1.e-13,1.e-1);
  //  gPad->SetLogy(1);
  g->Draw("AP");

  string name = "CIGSID33CH7";
  string frname =   Form("../../rootfiles/ana/IV/%s_iv.root",name.c_str());
  TFile* ofr = new TFile(frname.c_str(),"recreate");


  g->Write();
  
}


vector<string> split(string&input, char delimiter){

  istringstream stream(input);
  string field;
  vector<string> result;
  while(getline(stream,field,delimiter))
    result.push_back(field);

  return result;
};
