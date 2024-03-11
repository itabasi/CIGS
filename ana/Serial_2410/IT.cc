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


void IT(){ // Current timde dependence

  string fname =
    //    "~/work/CIGS/MPPC/Serial_2410/data/20221003_17:20:16.txt";
    //    "../../../Serial_2410/data/20221005_13:17:27.txt";
    //        "../../../Serial_2410/data/20221020_21:25:06.txt"; //CIGS16 CH7 -2V
    //    "../../../Serial_2410/data/20221020_12:43:21.txt"; //CIGS14 CH7 -2V
    //    "../../../Serial_2410/data/20221021_00:00:19.txt"; //CIGS16 CH7 -3V
    //    "../../../Serial_2410/data/20221019_15:37:29.txt"; //CIGS14 -2V
    //    "../../../Serial_2410/data/20221021_00:00:19.txt"; // CIGS16 CH6 -3V
    //    "../../../Serial_2410/data/20221028_15:21:31.txt"; // CIGS14 CH7 -3V
    //    "../../../Serial_2410/data/20221029_19:48:51.txt"; // CIGS14 CH6 -3V
    //    "../../../Serial_2410/data/20221101_18:21:28.txt"; // CIGS17 CH6 -2V
    //    "../../../Serial_2410/data/20221104_16:58:29.txt"; // CIGS17 CH7 -2V
  //    string fname ="./test.txt";
    //    "../../../Serial_2410/data/20221121_16:56:53.txt"; //CIGS21 CH6 -2V
    //        "../../../Serial_2410/data/20221121_19:12:01.txt"; //CIGS21 CH7 -2V
    //            "../../../Serial_2410/data/20221121_19:26:31.txt"; //CIGS22 CH6 -2Vw
    //    "../../../Serial_2410/data/20221121_19:35:48.txt"; //CIGS22 CH7 -2V
    //                "../../../Serial_2410/data/20221124_23:38:11.txt"; //CIGS22 CH7 -2V
    //    "../../../Serial_2410/data/20221125_01:29:03.txt"; //CIGS22 CH7 -2V
    //    "../../../Serial_2410/data/20240110_13:20:01.txt"; // Ag-CIGS
    //"../../../Serial_2410/data/20240109_16:13:59.txt";
    //    "../../data/20221110_14:15:33.txt";
    //    "../../Serial_2410/data/20240124_19:58:48.txt";
    //    "../../Serial_2410/data/20240125_06:58:41.txt";
    //    "../../Serial_2410/data/20240124_19:58:48.txt";
    //        "../../Serial_2410/data/20240109_16:13:59.txt";
    //    "../../Serial_2410/data/20240124_19:58:48.txt"; // Ag-CIGS
    "../../Serial_2410/data/20240109_16:13:59.txt"; // CIGS ID21
    //       "../../Serial_2410/data/20240125_06:58:41.txt"; // CIGS ID21
    //     "../../../Serial_2410/data/20240110_13:37:07.txt"; // Ga-rich-CIGS
  string buf;
  float TIME[100000],I[100000],HV[100000];
  int SEC[100000];
  int ii=0;
  float HV_OFF=-1.65;
  vector<string>strvec;
  ifstream ifp(fname.c_str(),ios::in);
  TGraph* g = new TGraph();
  TGraph* g2 = new TGraph();
  //  g->SetTitle("; TIME [min]; Current [A]");
  g->SetTitle("; TIME [min]; Current [A]");
  g -> SetMarkerStyle(20);
  g -> SetMarkerColor(4);
  g2->SetTitle("; TIME [min]; Current [A]");
  g2 -> SetMarkerStyle(20);
  g2 -> SetMarkerColor(6);

  cout<<"fname "<<fname<<endl;
  
  while(1){
    getline(ifp,buf);
    if(ifp.eof() || buf.size()==0)break;
    strvec = split(buf,',');
    if(strvec.size()==1){
      TIME[ii] =-100.;      I[ii]  =-100.; HV[ii] =-100.;
      SEC[ii] = stoi(strvec.at(0));
      //      TIME[ii] = sec_to_hour(SEC[ii]-SEC[0]);
      TIME[ii] = sec_to_min(SEC[ii]-SEC[0]);

      // ==== Get Current Value ======== //
      
    }else if(strvec.size()>1){
      HV[ii] = stof(strvec.at(0));
      I[ii]  = stof(strvec.at(1));
      cout<<"ii "<<ii<<" HV "<<HV[ii]<<" I "<<I[ii]<<endl;
      //      g->SetPoint(ii,HV[ii] + HV_OFF,I[ii]);
      g->SetPoint(ii,TIME[ii],fabs(I[ii]));
      if(fabs(I[ii])<0.4e-7)
	g2->SetPoint(ii,TIME[ii],fabs(I[ii]));
      //      else 	g->SetPoint(ii,TIME[ii],0.0);
      ii++;
    }
  }

  // ==== Time distribution ======== //
  cout<<"test ============"<<endl;
  //  TF1* f0 = new TF1("f0","[0]*log(x[0])+[1]",0.1,1000);
  //    TF1* f0 = new TF1("f0","[0]/(-[1]*x[0]+[2])+[3]",-100,1000);
  //  TF1* f0 = new TF1("f0","pol1(0)",-100,1000);
  //  f0->SetLineColor(2);
  //  f0->SetParameter(0,1.91878e-09);
  //  f0->SetParameter(1,3.70115e-11);


  cout<<"test1 ============"<<endl;

  // ====== Draw ======== //
  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  g->GetYaxis()->SetRangeUser(1.e-13,1.e-1);
  gPad->SetLogy(1);
  gPad->SetGridy(1);
  g->Draw("AP");
  //  f0->Draw("same");

  TLegend*leg = new TLegend(0.15,0.60,0.5,0.90,"","NDC");
  leg->AddEntry(g, "CIGS Sample 22 ch6(P7) ","p");

  //  leg->Draw();
  
  
  cout<<"test2 ============"<<endl;
    

  TCanvas* cx= new TCanvas("cx","cx");
  cx->cd();
  g->Draw("A");
  g2->Draw("AP");
  
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



