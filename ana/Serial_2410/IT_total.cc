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
extern int time_to_unixt(string time,int days);
extern int time_to_scale(int unixt);
const int nch =8;

  string TIMES_s[100000];
  int TIMES[100000];
  float TIMES_f[100000];
  int SCALE0[10000];
  int SCALE1[10000];
  int SCALE2[10000];
  int SCALE3[10000];
  int SCALE4[10000];


void IT_total(){ // Current timde dependence


  string sfile[2];
  sfile[0] ="scaler_25.txt";
  sfile[1] ="scaler_26.txt";
  ifstream ifS(sfile[0].c_str(),ios::in); // Scale file
  ifstream ifS1(sfile[1].c_str(),ios::in); // Scale file
  string buf;

  TGraph* gS0 = new TGraph();
  gS0->SetName("gS0");
  gS0->SetMarkerStyle(20);
  gS0->SetMarkerColor(1);

  TGraph* gS1 = new TGraph();
  gS1->SetName("gS1");
  gS1->SetMarkerStyle(20);
  gS1->SetMarkerColor(1);
  TGraph* gS2 = new TGraph();
  gS2->SetName("gS2");
  gS2->SetMarkerStyle(20);
  gS2->SetMarkerColor(2);
  TGraph* gS3 = new TGraph();
  gS3->SetName("gS3");
  gS3->SetMarkerStyle(20);
  gS3->SetMarkerColor(3);

  const int start_time = time_to_unixt("23:53:00",24); //RUN32
  const int NOTCIGS_st = time_to_unixt("18:15:00",25);
  const int NOTCIGS_end = time_to_unixt("20:48:00",25);

  int ANEALING130_st[4];
  ANEALING130_st[0] = time_to_unixt("17:50:00",25);
  ANEALING130_st[1] = time_to_unixt("19:30:00",25);
  ANEALING130_st[2] = time_to_unixt("6:30:00",26);
  ANEALING130_st[3] = time_to_unixt("7:18:00",26);

  int ANEALING130_end[4];
  ANEALING130_end[0] = time_to_unixt("18:50:00",25);
  ANEALING130_end[1] = time_to_unixt("20:30:00",25);
  ANEALING130_end[2] = time_to_unixt("7:00:00",26);
  ANEALING130_end[3] = time_to_unixt("7:38:00",26);

  int ANEALING90_st[5];
  ANEALING90_st[0] = time_to_unixt("3:40:00",26);
  ANEALING90_st[1] = time_to_unixt("4:10:00",26);
  ANEALING90_st[2] = time_to_unixt("5:09:00",26);
  //  ANEALING90_st[3] = time_to_unixt("6:30:00",26);
  //  ANEALING90_st[4] = time_to_unixt("7:18:00",26);

  int ANEALING90_end[5];
  ANEALING90_end[0] = time_to_unixt("03:55:00",26);
  ANEALING90_end[1] = time_to_unixt("4:40:00",26);
  ANEALING90_end[2] = time_to_unixt("6:09:00",26);
  //  ANEALING90_end[3] = time_to_unixt("7:00:00",26);
  //  ANEALING90_end[4] = time_to_unixt("7:38:00",26);

  
  float an130_time_st[4];
  float an130_time_end[4];


  // ======= TBOX for Scaler =========== //
  
  TBox* tbox_an130[4];
  for(int i=0;i<4;i++){
    an130_time_st[i] = sec_to_hour(ANEALING130_st[i]-start_time);
    an130_time_end[i] = sec_to_hour(ANEALING130_end[i]-start_time);
    tbox_an130[i] = new TBox(an130_time_st[i],0.0,an130_time_end[i],2.0e9);
    tbox_an130[i] ->SetFillStyle(3002);
    tbox_an130[i] ->SetFillColor(4);
  }

  float an90_time_st[5];
  float an90_time_end[5];
  TBox* tbox_an90[5];

  for(int i=0;i<3;i++){
    an90_time_st[i] = sec_to_hour(ANEALING90_st[i]-start_time);
    an90_time_end[i] = sec_to_hour(ANEALING90_end[i]-start_time);
    tbox_an90[i] = new TBox(an90_time_st[i],0.4,an90_time_end[i],2.0e9);
    tbox_an90[i] ->SetFillStyle(3002);
    tbox_an90[i] ->SetFillColor(6);
  }

  // ======== TBOX for leakage current ========== //
  
  TBox* tboxL_an130[4];
  for(int i=0;i<4;i++){
    an130_time_st[i] = sec_to_hour(ANEALING130_st[i]-start_time);
    an130_time_end[i] = sec_to_hour(ANEALING130_end[i]-start_time);
    tboxL_an130[i] = new TBox(an130_time_st[i],1.e-13,an130_time_end[i],1.e-7);
    tboxL_an130[i] ->SetFillStyle(3002);
    tboxL_an130[i] ->SetFillColor(4);
  }

  TBox* tboxL_an90[5];

  for(int i=0;i<3;i++){
    an90_time_st[i] = sec_to_hour(ANEALING90_st[i]-start_time);
    an90_time_end[i] = sec_to_hour(ANEALING90_end[i]-start_time);
    tboxL_an90[i] = new TBox(an90_time_st[i],1.e-13,an90_time_end[i],1.e-7);
    tboxL_an90[i] ->SetFillStyle(3002);
    tboxL_an90[i] ->SetFillColor(6);
  }


  
  // ======= TBOX for ADC =========== //
  
  TBox* tboxADC_an130[4];
  for(int i=0;i<4;i++){
    an130_time_st[i] = sec_to_hour(ANEALING130_st[i]-start_time);
    an130_time_end[i] = sec_to_hour(ANEALING130_end[i]-start_time);
    tboxADC_an130[i] = new TBox(an130_time_st[i],0.0,an130_time_end[i],1.1);
    tboxADC_an130[i] ->SetFillStyle(3002);
    tboxADC_an130[i] ->SetFillColor(4);
  }


  TBox* tboxADC_an90[5];

  for(int i=0;i<3;i++){
    an90_time_st[i] = sec_to_hour(ANEALING90_st[i]-start_time);
    an90_time_end[i] = sec_to_hour(ANEALING90_end[i]-start_time);
    tboxADC_an90[i] = new TBox(an90_time_st[i],0.4,an90_time_end[i],1.1);
    tboxADC_an90[i] ->SetFillStyle(3002);
    tboxADC_an90[i] ->SetFillColor(6);
  }
  

  
  
  float tst = sec_to_hour(NOTCIGS_st - start_time);
  float tend = sec_to_hour(NOTCIGS_end - start_time);
  TLine* line1 =new TLine(tst,0.0,tst,2.0e9);
  line1->SetLineColor(2);
  line1->SetLineStyle(2);
  TLine* line2 = new TLine(tend,0.0,tend,2.0e9);
  line2->SetLineColor(2);
  line2->SetLineStyle(2);

  TLine* lineL1 =new TLine(tst,1.e-13,tst,1.e-1);
  lineL1->SetLineColor(2);
  lineL1->SetLineStyle(2);
  TLine* lineL2 = new TLine(tend,1.e-13,tend,1.e-1);
  lineL2->SetLineColor(2);
  lineL2->SetLineStyle(2);


  int iii=0;
  int day;
  for(int i =0; i<2; i++){
    
    while(1){
      if(i==0){
	day = 25;
	getline(ifS,buf);
	if(ifS.eof() || buf.size()==0)break;
	
      }else{
	day =26;
	getline(ifS1,buf);
	if(ifS1.eof() || buf.size()==0)break;

      }
      TIMES[iii] =-100.; SCALE1[iii] = -100.; SCALE2[iii] = -100.; SCALE3[iii] = -100.;

      stringstream sbuf(buf);
      sbuf >> SCALE0[iii] >>TIMES_s[iii] >> SCALE1[iii] >> SCALE2[iii] >> SCALE3[iii] >> SCALE4[iii];
      TIMES[iii] = time_to_unixt(TIMES_s[iii],day);
      if(iii>0)
	TIMES_f[iii] = sec_to_hour(TIMES[iii]-start_time);     

      gS1->SetPoint(iii,TIMES_f[iii], SCALE1[iii]);
      gS2->SetPoint(iii,TIMES_f[iii], SCALE2[iii]);
      gS3->SetPoint(iii,TIMES_f[iii], SCALE3[iii]);
      iii++;
    }
  }      



  
  //===== IT dependence =========== //

  
  string list ="time.list";
  string fname[100];
  string dir ="/home/togawa/work/SENSOR/CIGS/Serial_2410/data/";
  
  float TIME[100000],I[100000],HV[100000];
  int SEC[100000];
  int ii=0;
  float HV_OFF=-1.65;
  vector<string>strvec;
  TGraph* g = new TGraph();
  TGraph* g2 = new TGraph();
  TGraph* gs = new TGraph();
  TGraph* gs2 = new TGraph();
  float SEC0;
  int s=0;
  int n=0;

  
  ifstream ifL(list.c_str(),ios::in); // IT runlist

  ofstream ofs("./IT.data",ios::out);
  
  while(1){
    getline(ifL,buf);
    if(buf[0]=='#'){continue;}
    if(ifL.eof()) break;
    stringstream sbuf(buf);
    sbuf >> fname[s] ;
    fname[s] = dir + fname[s];
    ifstream ifp(fname[s].c_str(),ios::in);
    ii=0;
    int NN;
    while(1){

      getline(ifp,buf);
      if(ifp.eof() || buf.size()==0)break;
      strvec = split(buf,',');
      if(strvec.size()==1){
	TIME[ii] =-100.;      I[ii]  =-100.; HV[ii] =-100.;
	SEC[ii] = stoi(strvec.at(0));
	if(s==0 && ii==0) SEC0 = SEC[ii];
	TIME[ii] = sec_to_hour(SEC[ii]-start_time);

	// ==== Get Current Value ======== //
	NN = time_to_scale(SEC[ii]);
      }else if(strvec.size()>1){
	HV[ii] = stof(strvec.at(0));
	I[ii]  = stof(strvec.at(1));
	g->SetPoint(n,TIME[ii],fabs(I[ii]));
	//      	if(fabs(TIME[ii]-22)<1)cout<<"TIME "<<TIME[ii]<<" I "<<fabs(I[ii])<<endl;
	if(fabs(I[ii])<1.e-5)
	  gs->SetPoint(n,NN,fabs(I[ii]));
	if(fabs(I[ii])<4.e-8){
	  ofs <<TIME[ii] <<" "<<fabs(I[ii])<<endl;
	  g2->SetPoint(n,TIME[ii],fabs(I[ii]));
	  gs2->SetPoint(n,NN,fabs(I[ii]));

	}
	  ii++;
	n++;
      }
    }

    s++;
  }
  
  // ================= Integrated ADC Time dependence ============== //
  string dir_runlist ="/home/togawa/work/SENSOR/CIGS/Ana/analysis/itamacro/";
  string runlist = "CIGS_Rad.txt";
  runlist = "CIGS_Rad_all.txt";
  //  runlist = "CIGS_Rad_woAnealing.txt";
  string rname[1000];

  TH1D* hIntegratedADC[nch][100];
  TH1D* hPH[nch][100];
  //  TGraphErrors* g[nch];
  TGraphErrors* gRatio[nch];
  TGraphErrors* gDiff = new TGraphErrors();
  gDiff->SetName("gDiff");
  gDiff->SetMarkerStyle(20);
  gDiff->SetMarkerColor(1);
  gDiff->SetTitle(";TIme [hour]; R_{CH0}/R_{CH1}");
  //  TGraphErrors* gPH[nch];
  //  TGraphErrors* gPHRatio[nch];
  TGraphErrors* gRatio_frame = new TGraphErrors();
  gRatio_frame ->SetTitle("; irradiated time [h]; Remaining factor");
  gRatio_frame->SetPoint(0,0,0);
  gRatio_frame->SetPoint(1,TIME[ii-1],1);

  TGraphErrors* gRatio_s[nch];

  for(int i=0;i<nch;i++){
    //    g[i] = new TGraphErrors();
    gRatio[i] = new TGraphErrors();
    gRatio[i] ->SetTitle("; time [h]; IntegratedADC ratio");
    gRatio_s[i] = new TGraphErrors();

    gRatio[i] -> SetName(Form("gRatio_%d",i));
    gRatio[i] -> SetMarkerColor(i+1);
    gRatio[i] -> SetLineColor(i+1);
    gRatio[i] -> SetMarkerStyle(2);

    gRatio_s[i] -> SetName(Form("gRatio_s_%d",i));
    gRatio_s[i] -> SetMarkerColor(i+1);
    gRatio_s[i] -> SetLineColor(i+1);
    gRatio_s[i] -> SetMarkerStyle(2);    

  }

  ifstream ifr(runlist.c_str(),ios::in);
  s=0;
  TFile* f[1000];
  while(1){
    getline(ifr,buf);
    if(buf[0]=='#'){continue;}
    if(ifr.eof()) break;
    stringstream sbuf(buf);
    sbuf >> rname[s] ;
    rname[s] = dir_runlist + rname[s];
    f[s] = new TFile(rname[s].c_str());
    for(int i=0;i<nch;i++){
      hIntegratedADC[i][s] = (TH1D*)f[s]->Get(Form("hIntegratedADC_%d",i));
      hIntegratedADC[i][s] -> SetName(Form("hIntegratedADC_%d_%d",i,s));
    }

    s++;
  }




  int unixt[100];
  unixt[0] = 1669302000; // RUN32 2022/11/25 00:00:00
  unixt[1] = 1669305540; // RUN38 2022/11/25 00:59:00
  unixt[2] = 1669307400; // RUN39 2022/11/25 01:30:00
  unixt[3] = 1669312260; // RUN40 2022/11/25 02:51:00
  unixt[4] = 1669316880; // RUN41 2022/11/25 04:08:00
  unixt[5] = 1669321260; // RUN42 2022/11/25 05:21:00
  unixt[6] = 1669326300; // RUN43 2022/11/25 06:45:00
  unixt[7] = 1669330500; // RUN44 2022/11/25 07:55:00
  unixt[8] = 1669334700; // RUN45 2022/11/25 09:05:00
  unixt[9] = 1669339380; // RUN46 2022/11/25 10:23:00
  //  unixt[10] = 1669339740; // RUN47 2022/11/25 10:29:00
  unixt[10] = 1669343880; // RUN48 2022/11/25 11:38:00
  unixt[11] = 1669348080; // RUN49 2022/11/25 12:48:00
  unixt[12] = 1669352280; // RUN50 2022/11/25 13:58:00  
  unixt[13] = 1669356180; // RUN53 2022/11/25 15:03:00
  unixt[14] = 1669360200; // RUN54 2022/11/25 16:10:00
  //  unixt[15] = 1669360800; // RUN55 2022/11/25 16:20:00
  unixt[15] = 1669371120; // RUN60 2022/11/25 19:12:00
  unixt[16] = 1669377000; // RUN73 2022/11/25 20:50:00
  unixt[17] = 1669382220; // RUN75 2022/11/25 22:17:00
  unixt[18] = 1669386480; // RUN76 2022/11/25 23:28:00
  //  unixt[19] = 1669387260; // RUN77 2022/11/25 23:41:00  
  unixt[19] = 1669395360; // RUN78 2022/11/26 1:56:00
  unixt[20] = 1669399980; // RUN79 2022/11/26 3:13:00
  unixt[21] = 1669402920; // RUN80 2022/11/26 4:02:00
  unixt[22] = 1669406400; // RUN81 2022/11/26 5:00:00
  unixt[23] = 1669411200; // RUN82 2022/11/26 6:20:00
  unixt[24] = 1669414140; // RUN83 2022/11/26 7:09:00
  unixt[25] = 1669416480; // RUN84 2022/11/26 7:48:00  

  
  // ======= Fitting ============== //

  float TIME2[100];
  
  TF1* fit = new TF1("fit","gausn(0)",0,10000);
  double peak[nch][100];
  double peak_err[nch][100];
  for(int i=0;i<s;i++){
    TIME2[i] = sec_to_hour(unixt[i] - start_time);
    for(int j=0;j<2;j++){
      hIntegratedADC[j][i] ->Fit("fit","QR","QR",100,700);
      peak[j][i] = fit->GetParameter(1);
      peak_err[j][i] = fit->GetParError(1);
      if(j!=1 || i<21){
	cout<<"i "<<i<<" j "<<j<<" peak "<<peak[j][i]<<endl;
	gRatio[j] -> SetPoint(i,TIME2[i],peak[j][i]/peak[j][0]);
	gRatio[j] -> SetPointError(i,0,(peak[j][i]/peak[j][0])*(peak_err[j][i]/peak[j][i] + peak_err[j][0]/peak[j][0]));
      }

      int NN = time_to_scale(unixt[i]);
      
      gRatio_s[j] -> SetPoint(i,NN,peak[j][i]/peak[j][0]);
      gRatio_s[j] -> SetPointError(i,0,(peak[j][i]/peak[j][0])*(peak_err[j][i]/peak[j][i] + peak_err[j][0]/peak[j][0]));

      cout<<"i "<<i<<" ch "<<j<<" ratio "<<peak[j][i]/peak[j][0]<<endl;
      //      cout<<"i "<<i<<" j "<<j<<" NN "<<NN<<" ratio "<<peak[j][i]/peak[j][0]<<endl;


      
      
    }
    if(1.>peak[1][i]/peak[1][0])
      gDiff->SetPoint(i,TIME2[i],(1.- peak[0][i]/peak[0][0])/(1.-peak[1][i]/peak[1][0]));
  }


  
  
  // ================= Draw Picture ================================ //


  g->SetTitle("; TIME [hours]; Current [A]");
  g -> SetMarkerStyle(1);
  g -> SetMarkerColor(1);

  g2->SetTitle("; TIME [hours]; Current [A]");
  g2 -> SetMarkerStyle(1);
  g2 -> SetMarkerColor(1);

  gs->SetTitle("; TIME [hours]; Current [A]");
  gs -> SetMarkerStyle(1);
  gs -> SetMarkerColor(1);

  

  // ==== Time distribution ======== //

  /*
  TF1* f0 = new TF1("f0","pol1(0)",-100,1000);
  f0->SetLineColor(2);
  f0->SetParameter(0,-9.04877e-12);
  f0->SetParameter(1, 2.09738e-09);

  TF1* f1 = new TF1("f1","pol1(0)",-100,1000);
  f1->SetLineColor(4);
  f1->SetParameter(0,-3.71279e-08);
  f1->SetParameter(1,1.88193e-09);
  */


  TF1* f0 = new TF1("f0","[0]*log(x[0]+[1])+[2]",-10,40);
  f0->SetLineColor(2);
  //  f0->SetParameter(0, 1e-7);
  //  f0->SetParameter(1, 33.0);
  //  f0->SetParameter(2, -3.7175e-7);

  f0->SetParameter(0, 1e-7);
  f0->SetParameter(1, 42.0);
  f0->SetParameter(2, -3.7175e-7);

  TF1* f1 = new TF1("f1","[0]*log(x[0]+[1])+[2]",-10,40);
  f1->SetLineColor(4);
  f1->SetParameter(0, 1e-7);
  f1->SetParameter(1, 33.0);
  f1->SetParameter(2, -3.9735e-7);

  //  f1->SetParameter(0, 1e-7);
  //  f1->SetParameter(1, 42.0);
  //  f1->SetParameter(2, -3.9375e-7);
  


  TF1* fs0 = new TF1("fs0","[0]*log(x[0]+[1])+[2]",0.1,2.e9);
  fs0->SetLineColor(2);
  fs0->SetParameter(0, 5e-8);
  fs0->SetParameter(1, 7e8);
  fs0->SetParameter(2, -1.0175e-6);

  TF1* fs1 = new TF1("fs1","[0]*log(x[0]+[1])+[2]",0.1,2.e9);
  fs1->SetLineColor(4);
  fs1->SetParameter(0, 5e-8);
  fs1->SetParameter(1, 7e8);
  fs1->SetParameter(2, -1.05e-6);

  

  TF1* f0R = new TF1("f0R","[2]*pol1(0)",-100,1000);
  f0R->SetLineColor(2);
  f0R->SetParameter(0,-9.04877e-12);
  f0R->SetParameter(1, 2.09738e-09);
  f0R->SetParameter(2,0.5e7);
  TF1* f1R = new TF1("f1R","[2]*pol1(0)",-100,1000);
  f1R->SetLineColor(4);
  f1R->SetParameter(0,-3.71279e-08);
  f1R->SetParameter(1,1.88193e-09);
  f1R->SetParameter(2,0.5e7);


  


  // ====== Draw ======== //
  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  g->GetYaxis()->SetRangeUser(1.e-13,1.e-1);
  gPad->SetLogy(1);
  gPad->SetGridy(1);
  g->Draw("AP");
  f0->Draw("same");
  f1->Draw("same");
  for(int i=0;i<4;i++){
    tboxL_an130[i]->Draw("same");
  }
  for(int i=0;i<3;i++){
    tboxL_an90[i]->Draw("same");
  }

  lineL1->Draw("same");
  lineL2->Draw("same");
  
  
  TLegend*leg = new TLegend(0.15,0.60,0.5,0.90,"","NDC");
  leg->AddEntry(g, "CIGS Sample 22 ch6(P7) ","p");
  //  leg->Draw();

  TCanvas* cx = new TCanvas("cx","cx");
  cx->cd();
  double sepa =0.7;
  TPad* pad0 = new TPad("pad0", "pad0",0.0,0.95-sepa,1.0,1.0);
  pad0->Draw();
  pad0->SetNumber(1);
  cx->cd();
  TPad *pad1 = new TPad("ratio", "ratio",0.0,0.00,1.0,0.95-sepa);
  pad1->Draw();
  pad1->SetNumber(2);
  cx->cd(2);


  cx->cd(1);
  gRatio_frame->Draw("AP");
  for(int i=0;i<2;i++)
  gRatio[i]->Draw("P");

  for(int i=0;i<4;i++){
    tboxADC_an130[i]->Draw("same");
  }
  for(int i=0;i<3;i++){
    tboxADC_an90[i]->Draw("same");
  }

  
  cx->cd(2);

  g->GetYaxis()->SetRangeUser(1.e-13,1.e-1);
  gPad->SetLogy(1);
  gPad->SetGridy(1);
  g->Draw("AP");
  f0->Draw("same");
  f1->Draw("same");  
  for(int i=0;i<4;i++){
    tboxL_an130[i]->Draw("same");
  }
  for(int i=0;i<3;i++){
    tboxL_an90[i]->Draw("same");
  }


  

  TCanvas* c3 = new TCanvas("c3","c3");
  c3->cd();
  sepa =0.5;
  TPad* padS0 = new TPad("padS0", "padS0",0.0,0.95-sepa,1.0,1.0);
  padS0->Draw();
  padS0->SetNumber(1);
  c3->cd();
  TPad *padS1 = new TPad("padS1", "padS1",0.0,0.00,1.0,0.95-sepa);
  padS1->Draw();
  padS1->SetNumber(2);
  c3->cd(2);

  c3->cd(1);
  gS1->Draw("AP");
  gS2->Draw("P");
  gS3->Draw("P");
  line1->Draw("same");
  line2->Draw("same");

  for(int i=0;i<4;i++){
    tbox_an130[i]->Draw("same");
  }
  for(int i=0;i<3;i++){
    tbox_an90[i]->Draw("same");
  }

  c3->cd(2);
  g->GetYaxis()->SetRangeUser(1.e-10,1.e-7);
  //  gPad->SetLogy(1);
  gPad->SetGridy(1);
  g->Draw("AP");
  f0->Draw("same");
  f1->Draw("same");  

  for(int i=0;i<4;i++){
    tboxL_an130[i]->Draw("same");
  }
  for(int i=0;i<3;i++){
    tboxL_an90[i]->Draw("same");
  }



  TCanvas* c00 = new TCanvas("c00","c00");
  c00->cd();
  gRatio_s[0]->Draw("AP");
  gRatio_s[1]->Draw("P");


  TCanvas* c01 = new TCanvas("c01","c01");
  c01->cd();
  gs->Draw("AP");
  fs0->Draw("same");
  fs1->Draw("same");

  TCanvas* c02 = new TCanvas("c02","c02");
  c02->cd();
  gs2->Fit("fs0","QR","QR",0.0,6.e8);
  gs2->Fit("fs1","QR","QR",7.e8,11.e8);

  gs2->Draw("AP");
  fs0->Draw("same");
  fs1->Draw("same");
  
  TCanvas* c03 = new TCanvas("c03","c03");
  c03->cd();

  g->Draw("AP");
  //  f0->Draw("same");
  //  g2->Fit("f0","","",5.0,17);
  //  f1->Draw("same");
  //  g2->Fit("f1","","",22,27);
  for(int i=0;i<4;i++){
    tboxL_an130[i]->Draw("same");
  }
  for(int i=0;i<3;i++){
    tboxL_an90[i]->Draw("same");
  }

  TCanvas* c04 = new TCanvas("c04","c04");
  c04->cd();
  g2->Draw("AP");
  f0->Draw("same");
  f1->Draw("same");

  TCanvas* c05 = new TCanvas("c05","c05");
  c05->cd();

  gRatio_frame->Draw("AP");
  for(int i=0;i<2;i++)
  gRatio[i]->Draw("P");

  for(int i=0;i<4;i++){
    tboxADC_an130[i]->Draw("same");
  }
  for(int i=0;i<3;i++){
    tboxADC_an90[i]->Draw("same");
  }

  TLegend* leg5 = new TLegend(0.15,0.6,0.5,0.9,"","NDC");
  leg5->AddEntry(gRatio[0],"CIGS CH6","pe");
  leg5->AddEntry(gRatio[1],"CIGS CH7","pe");
  leg5->Draw();




  TCanvas* c06 = new TCanvas("c06","c06");
  c06->cd();
   sepa =0.6;
  TPad* pad61 = new TPad("pad61", "pad61",0.0,0.95-sepa,1.0,1.0);
  pad61->Draw();
  pad61->SetNumber(1);
  c06->cd();
  TPad *pad62 = new TPad("pad62", "pad62",0.0,0.00,1.0,0.95-sepa);
  pad62->Draw();
  pad62->SetNumber(2);
  c06->cd(2);


  c06->cd(1);
  gRatio_frame->Draw("AP");
  for(int i=0;i<2;i++)
  gRatio[i]->Draw("P");

  for(int i=0;i<4;i++){
    tboxADC_an130[i]->Draw("same");
  }
  for(int i=0;i<3;i++){
    tboxADC_an90[i]->Draw("same");
  }

  
  c06->cd(2);

  gPad->SetGridy(1);
  gDiff->Draw("AP");
  for(int i=0;i<4;i++){
    tboxL_an130[i]->Draw("same");
  }
  for(int i=0;i<3;i++){
    tboxL_an90[i]->Draw("same");
  }



  TCanvas* c07 = new TCanvas("c07","c07");
  c07->cd();
  TPad* pad71 = new TPad("pad71", "pad71",0.0,0.95-sepa,1.0,1.0);
  pad71->Draw();
  pad71->SetNumber(1);
  c07->cd();
  TPad *pad72 = new TPad("pad72", "pad72",0.0,0.00,1.0,0.95-sepa);
  pad72->Draw();
  pad72->SetNumber(2);
  c07->cd(2);

  TLegend* leg7 = new TLegend(0.15,0.7,0.5,0.9,"","NDC");
  leg7->AddEntry(gRatio[0],"CIGS CH0","pe");
  leg7->AddEntry(gRatio[1],"CIGS CH1","pe");
  leg7->Draw();
  
  c07->cd(1);
  gPad->SetGridx(1);
  gRatio_s[0]->GetXaxis()->SetTitleSize(0.0);
  gRatio_s[0]->GetXaxis()->SetLabelSize(0.0);
  //  gRatio_s[0]->GetXaxis()->SetTitleOffset(1);
  gRatio_s[0]->GetYaxis()->SetTitleSize(0.05);
  gRatio_s[0]->GetYaxis()->SetLabelSize(0.05);
  gRatio_s[0]->GetYaxis()->SetTitleOffset(0.05);
  gRatio_s[0]->GetXaxis()->SetRangeUser(0,620e6);
  gRatio_s[0]->Draw("AP");
  gRatio_s[1]->Draw("P");
  leg7->Draw();

  c07->cd(2);
  gPad->SetGridx(1);
  gPad->SetLogy(1);
  gs2->Draw("AP");
  gs2->SetTitle(";irradiated particles [Counts] ; Current [A]");
  gs2->GetXaxis()->SetTitleSize(0.1);
  gs2->GetXaxis()->SetLabelSize(0.1);
  gs2->GetXaxis()->SetTitleOffset(1);
  gs2->GetYaxis()->SetTitleSize(0.1);
  gs2->GetYaxis()->SetLabelSize(0.1);
  gs2->GetYaxis()->SetTitleOffset(0.5);
  gs2->GetXaxis()->SetRangeUser(0,620e6);
  fs0->Draw("same");
  fs1->Draw("same");


  TCanvas* c08 = new TCanvas("c08","c08");

  TPad* pad81 = new TPad("pad81", "pad81",0.0,0.95-sepa,1.0,1.0);
  pad81->Draw();
  pad81->SetNumber(1);
  c08->cd();
  TPad *pad82 = new TPad("pad82", "pad82",0.0,0.00,1.0,0.95-sepa);
  pad82->Draw();
  pad82->SetNumber(2);
  c08->cd(2);

  TLegend* leg8 = new TLegend(0.15,0.7,0.5,0.9,"","NDC");
  leg8->AddEntry(gRatio[0],"CIGS CH0","pe");
  leg8->AddEntry(gRatio[1],"CIGS CH1","pe");
  leg8->Draw();
  
  c08->cd(1);
  gRatio_frame->Draw("AP");
  //  gRatio_frame->GetXaxis()->SetRangeUser(0,17);
  for(int i=0;i<2;i++)
  gRatio[i]->Draw("P");
  //  for(int i=0;i<4;i++){
  //    tboxADC_an130[i]->Draw("same");
  //  }
  //  for(int i=0;i<3;i++){
  //    tboxADC_an90[i]->Draw("same");
  //  }

  leg8->Draw();

  c08->cd(2);
  gPad->SetGridx(1);
  gPad->SetLogy(1);

  g2->Draw("AP");
  //  gs2->Draw("AP");
  g2->SetTitle(";Time [h] ; Current [A]");
  //  gs2->SetTitle(";irradiated particles [Counts] ; Current [A]");
  g2->GetXaxis()->SetRangeUser(0,17);
  g2->GetXaxis()->SetTitleSize(0.1);
  g2->GetXaxis()->SetLabelSize(0.1);
  g2->GetXaxis()->SetTitleOffset(1);
  g2->GetYaxis()->SetTitleSize(0.1);
  g2->GetYaxis()->SetLabelSize(0.1);
  g2->GetYaxis()->SetTitleOffset(0.5);
  //  fs0->Draw("same");
  //  fs1->Draw("same");


  TCanvas* c09 = new TCanvas("c09","c09");
  c09->cd();
  gRatio_frame->Draw("AP");
  gRatio[0]->Draw("P");
  //  gRatio[1]->Draw("P");
  gRatio_frame->GetYaxis()->SetRangeUser(0.4,1.1);


  TBox* tboxADC2_an130[4];
  for(int i=0;i<4;i++){
    an130_time_st[i] = sec_to_hour(ANEALING130_st[i]-start_time);
    an130_time_end[i] = sec_to_hour(ANEALING130_end[i]-start_time);
    tboxADC2_an130[i] = new TBox(an130_time_st[i],0.4,an130_time_end[i],1.1);
    tboxADC2_an130[i] ->SetFillStyle(3002);
    tboxADC2_an130[i] ->SetFillColor(4);
  }


  for(int i=0;i<4;i++){
    tboxADC2_an130[i]->Draw("same");
  }

  //  for(int i=0;i<4;i++){
  //    tboxADC_an130[i]->Draw("same");
  //  }
   for(int i=0;i<3;i++){
     tboxADC_an90[i]->Draw("same");
   }

  TLegend* leg9 = new TLegend(0.19,0.6,0.9,0.9,"","NDC");
  leg9->AddEntry(gRatio[0],"CIGS CH6","pe");
  leg9->AddEntry(gRatio[1],"CIGS CH7","pe");
  gRatio_frame->GetXaxis()->SetLabelSize(0.04);
  gRatio_frame->GetYaxis()->SetLabelSize(0.04);
  gRatio_frame->GetXaxis()->SetTitleSize(0.05);
  gRatio_frame->GetXaxis()->SetTitleOffset(0.8);
  gRatio_frame->GetYaxis()->SetTitleSize(0.05);
  gRatio_frame->GetYaxis()->SetTitleOffset(0.8);

  //  leg9->Draw();

  
  
  
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



int time_to_unixt(string time,int days){

  vector<string>strvec;
  strvec = split(time,':');
  int hour = stoi(strvec.at(0));
  int min = stoi(strvec.at(1));
  int sec = stoi(strvec.at(2));
  int t_1124 = 1669215600;
  int t_1125 = 1669302000;
  int t_1126 = 1669388400;
  int day;
  if(days == 24) day = t_1124;
  else if(days == 25) day = t_1125;
  else if(days == 26) day = t_1126;
  float unixt = day + 3600*hour + 60*min + sec;
  return unixt;
  
}




int time_to_scale(int unixt){

  int i=0;
  while(i<10000){
    if(TIMES[i] > unixt)
      return SCALE3[i];
    i++;
  }
 
}
