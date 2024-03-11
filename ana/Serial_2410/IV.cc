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
    //    "~/work/CIGS/MPPC/Serial_2410/data/20221003_17:20:16.txt";
    //        "../../../Serial_2410/data/20221005_13:17:27.txt";
    //            "../../../Serial_2410/data/20221124_23:38:11.txt";
    //    "../../../Serial_2410/data/20230210_02:56:49.txt";
    //"../../../Serial_2410/data/20231130_13:57:15.txt"; // GaN
    //    "../../../Serial_2410/data/20231130_14:44:49.txt";
    //    string fname ="./test.txt";
    //    "../../../Serial_2410/data/20240109_20:52:00.txt"; // Ga-rich CIGS
    //    "../../../Serial_2410/data/20240109_16:13:59.txt"; // Ga-rich CIGS
  string buf;
  float HV[1000],I[1000];
  int ii=0;
  float HV_OFF=-1.65;
  vector<string>strvec;
  ifstream ifp(fname.c_str(),ios::in);
  TGraph* g = new TGraph();
  g->SetTitle("; HV[V]; Current [A]");
  g -> SetMarkerStyle(20);
  g -> SetMarkerColor(4);
  while(1){
    getline(ifp,buf);
    if(ifp.eof() || buf.size()==0)break;

    strvec = split(buf,',');
    HV[ii] =-100.;
    I[ii]  =-100.;
    if(strvec.size()>1){
      HV[ii] = stof(strvec.at(0));
      I[ii]  = stof(strvec.at(1));
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
  
}


vector<string> split(string&input, char delimiter){

  istringstream stream(input);
  string field;
  vector<string> result;
  while(getline(stream,field,delimiter))
    result.push_back(field);

  return result;
};
