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


void IV(){ // Current timde dependence

  //  int runnum = 16;
  string name  =  "CIGS16";
  name = "CIGS14_IV_wGamma";
  name = "CIGS14_IV_woGamma";
  name = "CIGS15_IV_wGamma";
  name = "CIGS15_IV_woGamma";
  //  name = "CIGS14_IV_woGamma";
  string fname =    Form("./%s.txt",name.c_str());
  string frname =   Form("./RootFile/%s.root",name.c_str());
  TFile* ofr = new TFile(frname.c_str(),"recreate");
  
  string buf;
  float V[1000],C6[1000],C7[1000];
  int ii=0;
  vector<string>strvec;
  ifstream ifp(fname.c_str(),ios::in);
  TGraph* g6 = new TGraph();
  g6->SetName("g6");
  TGraph* g7 = new TGraph();
  g7->SetName("g7");
  //  g6->SetTitle("; HV [V]; 1/C^{2} [10^{16} (F/cm^{2})^{-2}");
  g6->SetTitle("; HV [V]; Current [A]");
  g6 -> SetMarkerStyle(20);
  g6 -> SetMarkerColor(4);
  g7 -> SetMarkerStyle(20);
  g7 -> SetMarkerColor(2);

  while(1){
    getline(ifp,buf);
    if(ifp.eof() || buf.size()==0)break;
    strvec = split(buf,',');
      V[ii] = stof(strvec.at(0));
      C6[ii] = stof(strvec.at(1));
      C7[ii] = stof(strvec.at(2));
      //      cout<<"ii "<<ii<<" V "<<V[ii]<<" C6 "<<C6[ii]<<" C7 "<<C7[ii]<<endl;
      g6->SetPoint(ii,V[ii],C6[ii]);
      g7->SetPoint(ii,V[ii],C7[ii]);
      ii++;
      
  }


  
  // ====== Draw ======== //

  TLegend*leg = new TLegend(0.15,0.60,0.5,0.90,"","NDC");

  leg->AddEntry(g6, "CIGS Sample 16 ch6(P6) ","p");
  leg->AddEntry(g7, "CIGS Sample 16 ch7(P7) ","p");
  

  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  g6->GetYaxis()->SetRangeUser(0,1.e-4);
  gPad->SetLogy(1);
  //  gPad->SetLogx(1);
  gPad->SetGridy(1);
  gPad->SetGridx(1);
  g6->Draw("AP");
  g7->Draw("P");
  leg->Draw();


  g6->Write();
  g7->Write();
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



