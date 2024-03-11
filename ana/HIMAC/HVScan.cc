#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <TGraph.h>
const int nfile =20;
const int nch   =2;

extern vector<string> split(string&input, char delimiter);
extern double GetHV(int run);
extern double GetCharge(double*x, double*par);
void HVScan(){

  gStyle->SetPalette(kRainBow);
  
  //  string runlist ="./runlist/CIGSHVScan_5mum.txt";
  string runlist ="./runlist/CIGSHVScan.txt";

  TH1D* hPed[nfile][nch];
  TH1D* hPeak[nfile][nch];

  TH1D* hCharge[nfile][nch];
  
  TFile* f[nfile];
  string fname[nfile];
  int s =0;
  int run[nfile];
  string buf;
  vector<string>  strvec;
  vector<string> strvec2;
  vector<string> strvec3;
  ifstream ifp(runlist.c_str(),ios::in);
  while(1){
    getline(ifp,buf);
    if(buf[0]=='#'){continue;}
    if(ifp.eof()) break;
    stringstream sbuf(buf);
    sbuf >> fname[s] ;
    f[s] = new TFile(fname[s].c_str());
    strvec = split(buf,'/');
    strvec2 = split(strvec.at(strvec.size()-1),'_');
    strvec3  = split(strvec2.at(strvec2.size()-1),'.');
    run[s]   = atoi(strvec2[1].c_str());
    for(int ich =0; ich < nch ; ich++){
      hPed[s][ich] = (TH1D*)f[s]->Get(Form("hPedestal_%d",ich));
      hPed[s][ich] ->SetName(Form("hPed_%d_%d",s,ich));
      hPeak[s][ich] = (TH1D*)f[s]->Get(Form("hIntegratedADC_%d",ich));
      hPeak[s][ich] ->SetName(Form("hPeak_%d_%d",s,ich));
      hCharge[s][ich] = new TH1D(Form("hCharge_%d_%d",s,ich),";Chage [pC];Counts",400,0,1);
    }
    s++;   
  }


  

  // ============= Fitting ================= //

  // ----- Pedestal -------- //
  TF1* fPed[nfile][nch];
  TF1* fPeak[nfile][nch];
  TGraphErrors* gPed_sig_2mum[nch];
  TGraphErrors* gPed_sig_5mum[nch];
  TGraphErrors* gPed_mean_2mum[nch];
  TGraphErrors* gPed_mean_5mum[nch];
  TGraphErrors* gPeak_sig_2mum[nch];
  TGraphErrors* gPeak_sig_5mum[nch];
  TGraphErrors* gPeak_Res_2mum[nch];
  TGraphErrors* gPeak_Res_5mum[nch];
  TGraphErrors* gPeak_mean_2mum[nch];
  TGraphErrors* gPeak_mean_5mum[nch];

  TGraphErrors* gPed_sig_2mum_c[nch];
  TGraphErrors* gPed_sig_5mum_c[nch];
  TGraphErrors* gPed_mean_2mum_c[nch];
  TGraphErrors* gPed_mean_5mum_c[nch];
  TGraphErrors* gPeak_sig_2mum_c[nch];
  TGraphErrors* gPeak_sig_5mum_c[nch];
  TGraphErrors* gPeak_Res_2mum_c[nch];
  TGraphErrors* gPeak_Res_5mum_c[nch];
  TGraphErrors* gPeak_mean_2mum_c[nch];
  TGraphErrors* gPeak_mean_5mum_c[nch];
  

  TF1* fCIGS_CH0_2um = new TF1("fCIGS_CH0_2um","GetCharge",0,3000,2);
  fCIGS_CH0_2um ->SetParameters(0,1);
  TF1* fCIGS_CH1_2um = new TF1("fCIGS_CH1_2um","GetCharge",0,3000,2);
  fCIGS_CH1_2um ->SetParameters(1,1);
  TF1* fCIGS_CH0_5um = new TF1("fCIGS_CH0_5um","GetCharge",0,3000,2);
  fCIGS_CH0_5um ->SetParameters(0,2);
  TF1* fCIGS_CH1_5um = new TF1("fCIGS_CH1_5um","GetCharge",0,3000,2);
  fCIGS_CH1_5um ->SetParameters(1,2);  

  
  for(int ich =0; ich <nch; ich++){
    gPed_sig_2mum[ich] = new TGraphErrors();
    gPed_sig_2mum[ich] -> SetName(Form("gPed_sig_2mum_%d",ich));
    gPed_sig_2mum[ich] -> SetMarkerStyle(20);
    gPed_sig_2mum[ich] -> SetMarkerColor(kRainBow);
    //    gPed_sig_2mum[ich] ->SetTitle(Form("CIGS %d#mum Pedestal Sigma; HV [V] ; #sigma [ch]",2));
    gPed_sig_2mum[ich] ->SetTitle("; HV [V] ; #sigma [ch]");    
    gPed_sig_5mum[ich] = new TGraphErrors();
    gPed_sig_5mum[ich] -> SetName(Form("gPed_sig_5mum_%d",ich));
    gPed_sig_5mum[ich] -> SetMarkerStyle(41);
    gPed_sig_5mum[ich] -> SetMarkerColor(kRainBow);
    //    gPed_sig_5mum[ich] ->SetTitle(Form("CIGS %d#mum Pedestal Sigma; HV [V] ;#sigma [ch]",5));
        gPed_sig_5mum[ich] ->SetTitle("; HV [V] ;#sigma [ch]");
    gPeak_sig_2mum[ich] = new TGraphErrors();
    gPeak_sig_2mum[ich] -> SetName(Form("gPeak_sig_2mum_%d",ich));
    gPeak_sig_2mum[ich] -> SetMarkerStyle(20);
    gPeak_sig_2mum[ich] -> SetMarkerColor(kRainBow);
    //    gPeak_sig_2mum[ich] ->SetTitle(Form("CIGS %d#mum Peak Sigma; HV [V] ; #sigma [ch]",2));
    gPeak_sig_2mum[ich] ->SetTitle(" ;HV [V]; #sigma [pC]");
    gPeak_sig_5mum[ich] = new TGraphErrors();
    gPeak_sig_5mum[ich] -> SetName(Form("gPeak_sig_5mum_%d",ich));
    gPeak_sig_5mum[ich] -> SetMarkerStyle(41);
    gPeak_sig_5mum[ich] -> SetMarkerColor(kRainBow);
    //    gPeak_sig_5mum[ich] ->SetTitle(Form("CIGS %d#mum Peak Sigma; HV [V] ; #sigma [pC]",5));
    gPeak_sig_5mum[ich] ->SetTitle("; HV [V] ; #sigma [pC]");
    gPed_mean_2mum[ich] = new TGraphErrors();
    gPed_mean_2mum[ich] -> SetName(Form("gPed_mean_2mum_%d",ich));
    gPed_mean_2mum[ich] -> SetMarkerStyle(20);
    gPed_mean_2mum[ich] -> SetMarkerColor(kRainBow);
    //    gPed_mean_2mum[ich] ->SetTitle(Form("CIGS %d#mum Ped. Mean; HV [V] ; Mean [pC]",2));
    gPed_mean_2mum[ich] ->SetTitle("; HV [V] ; Mean [pC]");
    gPed_mean_5mum[ich] = new TGraphErrors();
    gPed_mean_5mum[ich] -> SetName(Form("gPed_mean_5mum_%d",ich));
    gPed_mean_5mum[ich] -> SetMarkerStyle(41);
    gPed_mean_5mum[ich] -> SetMarkerColor(kRainBow);
    //    gPed_mean_5mum[ich] ->SetTitle(Form("CIGS %d#mum Ped. Mean; HV [V] ; Mean [pC]",5));
    gPed_mean_5mum[ich] ->SetTitle("; HV [V]; Mean [pC]");    
    gPeak_mean_2mum[ich] = new TGraphErrors();
    gPeak_mean_2mum[ich] -> SetName(Form("gPeak_mean_2mum_%d",ich));
    gPeak_mean_2mum[ich] -> SetMarkerStyle(20);
    gPeak_mean_2mum[ich] -> SetMarkerColor(kRainBow);
    //    gPeak_mean_2mum[ich] ->SetTitle(Form("CIGS %d#mum Peak Mean; HV [V] ; Mean [pC]",2));
    gPeak_mean_2mum[ich] ->SetTitle("; HV [V] ; Mean [pC]");
    gPeak_mean_5mum[ich] = new TGraphErrors();
    gPeak_mean_5mum[ich] -> SetName(Form("gPeak_mean_5mum_%d",ich));
    gPeak_mean_5mum[ich] -> SetMarkerStyle(41);
    gPeak_mean_5mum[ich] -> SetMarkerColor(kRainBow);
    //    gPeak_mean_5mum[ich] ->SetTitle(Form("CIGS %d#mum Peak Mean; HV [V] ; Mean [pC]",5));
    gPeak_mean_5mum[ich] ->SetTitle("; HV [V] ; Mean [pC]");
    gPeak_Res_2mum[ich] = new TGraphErrors();
    gPeak_Res_2mum[ich] -> SetName(Form("gPeak_Res_2mum_%d",ich));
    gPeak_Res_2mum[ich] -> SetMarkerStyle(20);
    gPeak_Res_2mum[ich] -> SetMarkerColor(kRainBow);
    //    gPeak_Res_2mum[ich] ->SetTitle(Form("CIGS %d#mum Peak Res.; HV [V] ; #sigma/mean",2));
    gPeak_Res_2mum[ich] ->SetTitle("; HV [V] ; #sigma/mean");
    gPeak_Res_5mum[ich] = new TGraphErrors();
    gPeak_Res_5mum[ich] -> SetName(Form("gPeak_Res_5mum_%d",ich));
    gPeak_Res_5mum[ich] -> SetMarkerStyle(41);
    gPeak_Res_5mum[ich] -> SetMarkerColor(kRainBow);
    //    gPeak_Res_5mum[ich] ->SetTitle(Form("CIGS %d#mum Peak Res.; HV [V] ; #sigma/mean",5));
    gPeak_Res_5mum[ich] ->SetTitle("; HV [V] ; #sigma/mean");



    gPed_sig_2mum_c[ich] = new TGraphErrors();
    gPed_sig_2mum_c[ich] -> SetName(Form("gPed_sig_2mum_c_%d",ich));
    gPed_sig_2mum_c[ich] -> SetMarkerStyle(20);
    gPed_sig_2mum_c[ich] -> SetMarkerColor(kRainBow);
    //    gPed_sig_2mum_c[ich] ->SetTitle(Form("CIGS %d#mum Pedestal Sigma; HV [V] ; #sigma [pC]",2));
    gPed_sig_2mum_c[ich] ->SetTitle("; HV [V] ; #sigma [pC]");    
    gPed_sig_5mum_c[ich] = new TGraphErrors();
    gPed_sig_5mum_c[ich] -> SetName(Form("gPed_sig_5mum_c_%d",ich));
    gPed_sig_5mum_c[ich] -> SetMarkerStyle(41);
    gPed_sig_5mum_c[ich] -> SetMarkerColor(kRainBow);
    //    gPed_sig_5mum_c[ich] ->SetTitle(Form("CIGS %d#mum Pedestal Sigma; HV [V] ;#sigma [pC]",5));
        gPed_sig_5mum_c[ich] ->SetTitle("; HV [V] ;#sigma [pC]");
    gPeak_sig_2mum_c[ich] = new TGraphErrors();
    gPeak_sig_2mum_c[ich] -> SetName(Form("gPeak_sig_2mum_c_%d",ich));
    gPeak_sig_2mum_c[ich] -> SetMarkerStyle(20);
    gPeak_sig_2mum_c[ich] -> SetMarkerColor(kRainBow);
    //    gPeak_sig_2mum_c[ich] ->SetTitle(Form("CIGS %d#mum Peak Sigma; HV [V] ; #sigma [pC]",2));
    gPeak_sig_2mum_c[ich] ->SetTitle(" ;HV [V]; #sigma [pC]");
    gPeak_sig_5mum_c[ich] = new TGraphErrors();
    gPeak_sig_5mum_c[ich] -> SetName(Form("gPeak_sig_5mum_c_%d",ich));
    gPeak_sig_5mum_c[ich] -> SetMarkerStyle(41);
    gPeak_sig_5mum_c[ich] -> SetMarkerColor(kRainBow);
    //    gPeak_sig_5mum_c[ich] ->SetTitle(Form("CIGS %d#mum Peak Sigma; HV [V] ; #sigma [pC]",5));
    gPeak_sig_5mum_c[ich] ->SetTitle("; HV [V] ; #sigma [pC]");
    gPed_mean_2mum_c[ich] = new TGraphErrors();
    gPed_mean_2mum_c[ich] -> SetName(Form("gPed_mean_2mum_c_%d",ich));
    gPed_mean_2mum_c[ich] -> SetMarkerStyle(20);
    gPed_mean_2mum_c[ich] -> SetMarkerColor(kRainBow);
    //    gPed_mean_2mum_c[ich] ->SetTitle(Form("CIGS %d#mum Ped. Mean; HV [V] ; Mean [pC]",2));
    gPed_mean_2mum_c[ich] ->SetTitle("; HV [V] ; Mean [pC]");
    gPed_mean_5mum_c[ich] = new TGraphErrors();
    gPed_mean_5mum_c[ich] -> SetName(Form("gPed_mean_5mum_c_%d",ich));
    gPed_mean_5mum_c[ich] -> SetMarkerStyle(41);
    gPed_mean_5mum_c[ich] -> SetMarkerColor(kRainBow);
    //    gPed_mean_5mum_c[ich] ->SetTitle(Form("CIGS %d#mum Ped. Mean; HV [V] ; Mean [pC]",5));
    gPed_mean_5mum_c[ich] ->SetTitle("; HV [V]; Mean [pC]");    
    gPeak_mean_2mum_c[ich] = new TGraphErrors();
    gPeak_mean_2mum_c[ich] -> SetName(Form("gPeak_mean_2mum_c_%d",ich));
    gPeak_mean_2mum_c[ich] -> SetMarkerStyle(20);
    gPeak_mean_2mum_c[ich] -> SetMarkerColor(kRainBow);
    //    gPeak_mean_2mum_c[ich] ->SetTitle(Form("CIGS %d#mum Peak Mean; HV [V] ; Mean [pC]",2));
    gPeak_mean_2mum_c[ich] ->SetTitle("; HV [V] ; Mean [pC]");
    gPeak_mean_5mum_c[ich] = new TGraphErrors();
    gPeak_mean_5mum_c[ich] -> SetName(Form("gPeak_mean_5mum_c_%d",ich));
    gPeak_mean_5mum_c[ich] -> SetMarkerStyle(41);
    gPeak_mean_5mum_c[ich] -> SetMarkerColor(kRainBow);
    //    gPeak_mean_5mum_c[ich] ->SetTitle(Form("CIGS %d#mum Peak Mean; HV [V] ; Mean [pC]",5));
    gPeak_mean_5mum_c[ich] ->SetTitle("; HV [V] ; Mean [pC]");
    gPeak_Res_2mum_c[ich] = new TGraphErrors();
    gPeak_Res_2mum_c[ich] -> SetName(Form("gPeak_Res_2mum_c_%d",ich));
    gPeak_Res_2mum_c[ich] -> SetMarkerStyle(20);
    gPeak_Res_2mum_c[ich] -> SetMarkerColor(kRainBow);
    //    gPeak_Res_2mum_c[ich] ->SetTitle(Form("CIGS %d#mum Peak Res.; HV [V] ; #sigma/mean",2));
    gPeak_Res_2mum_c[ich] ->SetTitle("; HV [V] ; #sigma/mean");
    gPeak_Res_5mum_c[ich] = new TGraphErrors();
    gPeak_Res_5mum_c[ich] -> SetName(Form("gPeak_Res_5mum_c_%d",ich));
    gPeak_Res_5mum_c[ich] -> SetMarkerStyle(41);
    gPeak_Res_5mum_c[ich] -> SetMarkerColor(kRainBow);
    //    gPeak_Res_5mum_c[ich] ->SetTitle(Form("CIGS %d#mum Peak Res.; HV [V] ; #sigma/mean",5));
    gPeak_Res_5mum_c[ich] ->SetTitle("; HV [V] ; #sigma/mean");

    
    for(int i=0; i<s; i++){
      // ------- Pedestal ------------//
      fPed[i][ich] = new TF1(Form("fPed_%d_%d",i,ich),"gausn(0)",-100.,100.);
      double mean = hPed[i][ich]->GetBinCenter(hPed[i][ich]->GetMaximumBin());
      fPed[i][ich]->SetParameter(1,mean);
      fPed[i][ich]->SetParameter(2,20);
      hPed[i][ich] ->Fit(Form("fPed_%d_%d",i,ich),"QR","QR",-100.,100.);
      double sigma = fPed[i][ich]->GetParameter(2);
      mean = fPed[i][ich]->GetParameter(1);
      double mean_err = fPed[i][ich]->GetParError(1);
      double sigma_err = fPed[i][ich]->GetParError(2);
      if(fabs(mean)> 20){
	hPed[i][ich] ->Fit(Form("fPed_%d_%d",i,ich),"QR","QR",-100.,40.);
	sigma = fPed[i][ich]->GetParameter(2);
	sigma_err = fPed[i][ich]->GetParError(2);
	mean = fPed[i][ich]->GetParameter(2);
	mean_err = fPed[i][ich]->GetParError(1);
      }
      double mean_c[2],mean_err_c[2];
      double sigma_c[2],sigma_err_c[2];      
      if(60 < run[i] && run[i] <72){

	mean_c[0] = fCIGS_CH0_5um->Eval(mean);
	mean_c[1] = fCIGS_CH1_5um->Eval(mean);
	mean_err_c[0] = fCIGS_CH0_5um->Eval(mean_err);
	mean_err_c[1] = fCIGS_CH1_5um->Eval(mean_err);

	sigma_c[0] = fCIGS_CH0_5um->Eval(sigma);
	sigma_c[1] = fCIGS_CH1_5um->Eval(sigma);
	sigma_err_c[0] = fCIGS_CH0_5um->Eval(sigma_err);
	sigma_err_c[1] = fCIGS_CH1_5um->Eval(sigma_err);	

	gPed_sig_5mum[ich] ->SetPoint(i,GetHV(run[i]),sigma);
	gPed_sig_5mum[ich] ->SetPointError(i,GetHV(run[i]),sigma_err);
	gPed_mean_5mum[ich] ->SetPoint(i,GetHV(run[i]),mean);
	gPed_mean_5mum[ich] ->SetPointError(i,GetHV(run[i]),mean_err);

	gPed_sig_5mum_c[ich] ->SetPoint(i,GetHV(run[i]),sigma_c[ich]);
	gPed_sig_5mum_c[ich] ->SetPointError(i,GetHV(run[i]),sigma_err_c[ich]);
	gPed_mean_5mum_c[ich] ->SetPoint(i,GetHV(run[i]),mean_c[ich]);
	gPed_mean_5mum_c[ich] ->SetPointError(i,GetHV(run[i]),mean_err_c[ich]);	

      }else{
	mean_c[0] = fCIGS_CH0_2um->Eval(mean);
	mean_c[1] = fCIGS_CH1_2um->Eval(mean);
	mean_err_c[0] = fCIGS_CH0_2um->Eval(mean_err);
	mean_err_c[1] = fCIGS_CH1_2um->Eval(mean_err);

	sigma_c[0] = fCIGS_CH0_2um->Eval(sigma);
	sigma_c[1] = fCIGS_CH1_2um->Eval(sigma);
	sigma_err_c[0] = fCIGS_CH0_2um->Eval(sigma_err);
	sigma_err_c[1] = fCIGS_CH1_2um->Eval(sigma_err);	

	gPed_sig_2mum[ich] ->SetPoint(i,GetHV(run[i]),sigma);
	gPed_sig_2mum[ich] ->SetPointError(i,GetHV(run[i]),sigma_err);
	gPed_mean_2mum[ich] ->SetPoint(i,GetHV(run[i]),mean);
	gPed_mean_2mum[ich] ->SetPointError(i,GetHV(run[i]),mean_err);

	gPed_sig_2mum_c[ich] ->SetPoint(i,GetHV(run[i]),sigma_c[ich]);
	gPed_sig_2mum_c[ich] ->SetPointError(i,GetHV(run[i]),sigma_err_c[ich]);
	gPed_mean_2mum_c[ich] ->SetPoint(i,GetHV(run[i]),mean_c[ich]);
	gPed_mean_2mum_c[ich] ->SetPointError(i,GetHV(run[i]),mean_err_c[ich]);	
      }
      // ------- Peak  ---------------- //
      fPeak[i][ich] = new TF1(Form("fPeak_%d_%d",i,ich),"gausn(0)",100.,1000.);
      mean = hPeak[i][ich]->GetBinCenter(hPeak[i][ich]->GetMaximumBin());
      fPeak[i][ich]->SetParameter(1,mean);
      if(ich<2)
	hPeak[i][ich] ->Fit(Form("fPeak_%d_%d",i,ich),"QR","QR",100.,1000.);
      sigma = fPeak[i][ich]->GetParameter(2);
      mean = fPeak[i][ich]->GetParameter(1);
      mean_err = fPeak[i][ich]->GetParError(1);
      sigma_err = fPeak[i][ich]->GetParError(2);

      if(60 < run[i] && run[i] <72){
	gPeak_sig_5mum[ich] ->SetPoint(i,GetHV(run[i]),sigma);
	gPeak_sig_5mum[ich] ->SetPointError(i,GetHV(run[i]),sigma_err);
	gPeak_mean_5mum[ich] ->SetPoint(i,GetHV(run[i]),mean);
	gPeak_mean_5mum[ich] ->SetPointError(i,GetHV(run[i]),mean_err);
	gPeak_Res_5mum[ich] ->SetPoint(i,GetHV(run[i]),sigma/mean);

	mean_c[0] = fCIGS_CH0_5um->Eval(mean);
	mean_c[1] = fCIGS_CH1_5um->Eval(mean);
	mean_err_c[0] = fCIGS_CH0_5um->Eval(mean_err);
	mean_err_c[1] = fCIGS_CH1_5um->Eval(mean_err);

	sigma_c[0] = fCIGS_CH0_5um->Eval(sigma);
	sigma_c[1] = fCIGS_CH1_5um->Eval(sigma);
	sigma_err_c[0] = fCIGS_CH0_5um->Eval(sigma_err);
	sigma_err_c[1] = fCIGS_CH1_5um->Eval(sigma_err);	

	gPeak_sig_5mum_c[ich] ->SetPoint(i,GetHV(run[i]),sigma_c[ich]);
	gPeak_sig_5mum_c[ich] ->SetPointError(i,GetHV(run[i]),sigma_err_c[ich]);
	gPeak_mean_5mum_c[ich] ->SetPoint(i,GetHV(run[i]),mean_c[ich]);
	gPeak_mean_5mum_c[ich] ->SetPointError(i,GetHV(run[i]),mean_err_c[ich]);
	gPeak_Res_5mum_c[ich] ->SetPoint(i,GetHV(run[i]),sigma_c[ich]/mean_c[ich]);
      }else{
	gPeak_sig_2mum[ich] ->SetPoint(i,GetHV(run[i]),sigma);
	gPeak_sig_2mum[ich] ->SetPointError(i,GetHV(run[i]),sigma_err);
	gPeak_mean_2mum[ich] ->SetPoint(i,GetHV(run[i]),mean);
	gPeak_mean_2mum[ich] ->SetPointError(i,GetHV(run[i]),mean_err);
	gPeak_Res_2mum[ich] ->SetPoint(i,GetHV(run[i]),sigma/mean);

	mean_c[0] = fCIGS_CH0_2um->Eval(mean);
	mean_c[1] = fCIGS_CH1_2um->Eval(mean);
	mean_err_c[0] = fCIGS_CH0_2um->Eval(mean_err);
	mean_err_c[1] = fCIGS_CH1_2um->Eval(mean_err);

	sigma_c[0] = fCIGS_CH0_2um->Eval(sigma);
	sigma_c[1] = fCIGS_CH1_2um->Eval(sigma);
	sigma_err_c[0] = fCIGS_CH0_2um->Eval(sigma_err);
	sigma_err_c[1] = fCIGS_CH1_2um->Eval(sigma_err);
	
	gPeak_sig_2mum_c[ich] ->SetPoint(i,GetHV(run[i]),sigma_c[ich]);
	gPeak_sig_2mum_c[ich] ->SetPointError(i,GetHV(run[i]),sigma_err_c[ich]);
	gPeak_mean_2mum_c[ich] ->SetPoint(i,GetHV(run[i]),mean_c[ich]);
	gPeak_mean_2mum_c[ich] ->SetPointError(i,GetHV(run[i]),mean_err_c[ich]);
	gPeak_Res_2mum_c[ich] ->SetPoint(i,GetHV(run[i]),sigma_c[ich]/mean_c[ich]);
      }
    }
  }

  TLegend*leg = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
  leg->AddEntry(gPed_sig_2mum[0], "CIGS 2#mum CH0 Pedestal Sigma ","lpe");
  leg->AddEntry(gPed_sig_2mum[1], "CIGS 2#mum CH1 Pedestal Sigma ","lpe");
  leg->AddEntry(gPed_sig_5mum[0], "CIGS 5#mum CH0 Pedestal Sigma ","lpe");
  leg->AddEntry(gPed_sig_5mum[1], "CIGS 5#mum CH1 Pedestal Sigma ","lpe");
  
  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  gPed_sig_2mum[0] ->GetYaxis()->SetRangeUser(0,30);
  //  gPed_sig_2mum[0] ->SetTitleSize(0);
  gPed_sig_2mum[0] ->Draw("PLC PMC AP");
  gPed_sig_2mum[1] ->Draw("PLC PMC P");
  gPed_sig_5mum[0] ->Draw("PLC PMC P");
  gPed_sig_5mum[1] ->Draw("PLC PMC P");
  leg->Draw();

  TLegend*leg1 = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
  leg1->AddEntry(gPeak_sig_2mum[0], "CIGS 2#mum CH0 Peak Sigma ","lpe");
  leg1->AddEntry(gPeak_sig_2mum[1], "CIGS 2#mum CH1 Peak Sigma ","lpe");
  leg1->AddEntry(gPeak_sig_5mum[0], "CIGS 5#mum CH0 Peak Sigma ","lpe");
  leg1->AddEntry(gPeak_sig_5mum[1], "CIGS 5#mum CH1 Peak Sigma ","lpe");

  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  //  gPeak_sig_2mum[0] ->SetTitleSize(0);
  gPeak_sig_2mum[0] ->GetYaxis()->SetRangeUser(0,100);
  gPeak_sig_2mum[0] ->Draw("PLC PMC AP");
  gPeak_sig_2mum[1] ->Draw("PLC PMC P");
  gPeak_sig_5mum[0] ->Draw("PLC PMC P");
  gPeak_sig_5mum[1] ->Draw("PLC PMC P");
  leg1->Draw();


  TLegend*leg2 = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
  leg2->AddEntry(gPeak_Res_2mum[0], "CIGS 2#mum CH0 Peak Res ","lpe");
  leg2->AddEntry(gPeak_Res_2mum[1], "CIGS 2#mum CH1 Peak Res ","lpe");
  leg2->AddEntry(gPeak_Res_5mum[0], "CIGS 5#mum CH0 Peak Res ","lpe");
  leg2->AddEntry(gPeak_Res_5mum[1], "CIGS 5#mum CH1 Peak Res ","lpe");

  TCanvas* c2 = new TCanvas("c2","c2");
  c2->cd();
  //  gPeak_Res_2mum[0] ->SetTitleSize(0);
  gPeak_Res_2mum[0] ->GetYaxis()->SetRangeUser(0.05,0.15);
  gPeak_Res_2mum[0] ->Draw("PLC PMC AP");
  gPeak_Res_2mum[1] ->Draw("PLC PMC P");
  gPeak_Res_5mum[0] ->Draw("PLC PMC P");
  gPeak_Res_5mum[1] ->Draw("PLC PMC P");
  leg2->Draw();

  TLegend*leg3 = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
  leg3->AddEntry(gPeak_mean_2mum[0], "CIGS 2#mum CH0 Peak mean ","lpe");
  leg3->AddEntry(gPeak_mean_2mum[1], "CIGS 2#mum CH1 Peak mean ","lpe");
  leg3->AddEntry(gPeak_mean_5mum[0], "CIGS 5#mum CH0 Peak mean ","lpe");
  leg3->AddEntry(gPeak_mean_5mum[1], "CIGS 5#mum CH1 Peak mean ","lpe");

  TCanvas* c3 = new TCanvas("c3","c3");
  c3->cd();
  gPeak_mean_2mum[0] ->GetYaxis()->SetRangeUser(0,1000);
  //  gPeak_mean_2mum[0] ->SetTitleSize(0);
  gPeak_mean_2mum[0] ->Draw("PLC PMC AP");
  gPeak_mean_2mum[1] ->Draw("PLC PMC P");
  gPeak_mean_5mum[0] ->Draw("PLC PMC P");
  gPeak_mean_5mum[1] ->Draw("PLC PMC P");
  leg3->Draw();
  
  




  TLegend*leg00 = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
  leg00->AddEntry(gPed_sig_2mum_c[0], "CIGS 2#mum CH0 Pedestal Sigma ","lpe");
  leg00->AddEntry(gPed_sig_2mum_c[1], "CIGS 2#mum CH1 Pedestal Sigma ","lpe");
  leg00->AddEntry(gPed_sig_5mum_c[0], "CIGS 5#mum CH0 Pedestal Sigma ","lpe");
  leg00->AddEntry(gPed_sig_5mum_c[1], "CIGS 5#mum CH1 Pedestal Sigma ","lpe");
  
  TCanvas* c00 = new TCanvas("c00","c00");
  c00->cd();
  gPed_sig_2mum_c[0] ->GetYaxis()->SetRangeUser(0,3);
  //  gPed_sig_2mum[0] ->SetTitleSize(0);
  gPed_sig_2mum_c[0] ->Draw("PLC PMC AP");
  gPed_sig_2mum_c[1] ->Draw("PLC PMC P");
  gPed_sig_5mum_c[0] ->Draw("PLC PMC P");
  gPed_sig_5mum_c[1] ->Draw("PLC PMC P");
  leg00->Draw();

  TLegend*leg01 = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
  leg01->AddEntry(gPeak_sig_2mum_c[0], "CIGS 2#mum CH0 Peak Sigma ","lpe");
  leg01->AddEntry(gPeak_sig_2mum_c[1], "CIGS 2#mum CH1 Peak Sigma ","lpe");
  leg01->AddEntry(gPeak_sig_5mum_c[0], "CIGS 5#mum CH0 Peak Sigma ","lpe");
  leg01->AddEntry(gPeak_sig_5mum_c[1], "CIGS 5#mum CH1 Peak Sigma ","lpe");

  TCanvas* c01 = new TCanvas("c01","c01");
  c01->cd();
  //  gPeak_sig_2mum_c[0] ->SetTitleSize(0);
  gPeak_sig_2mum_c[0] ->GetYaxis()->SetRangeUser(0,0.1);
  gPeak_sig_2mum_c[0] ->Draw("PLC PMC AP");
  gPeak_sig_2mum_c[1] ->Draw("PLC PMC P");
  gPeak_sig_5mum_c[0] ->Draw("PLC PMC P");
  gPeak_sig_5mum_c[1] ->Draw("PLC PMC P");
  leg01->Draw();


  TLegend*leg02 = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
  leg02->AddEntry(gPeak_Res_2mum_c[0], "CIGS 2#mum CH0 Peak Res ","lpe");
  leg02->AddEntry(gPeak_Res_2mum_c[1], "CIGS 2#mum CH1 Peak Res ","lpe");
  leg02->AddEntry(gPeak_Res_5mum_c[0], "CIGS 5#mum CH0 Peak Res ","lpe");
  leg02->AddEntry(gPeak_Res_5mum_c[1], "CIGS 5#mum CH1 Peak Res ","lpe");

  TCanvas* c02 = new TCanvas("c02","c02");
  c02->cd();
  //  gPeak_Res_2mum_c[0] ->SetTitleSize(0);
  gPeak_Res_2mum_c[0] ->GetYaxis()->SetRangeUser(0.0,0.2);
  gPeak_Res_2mum_c[0] ->Draw("PLC PMC AP");
  gPeak_Res_2mum_c[1] ->Draw("PLC PMC P");
  gPeak_Res_5mum_c[0] ->Draw("PLC PMC P");
  gPeak_Res_5mum_c[1] ->Draw("PLC PMC P");
  leg02->Draw();

  TLegend*leg03 = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
  leg03->AddEntry(gPeak_mean_2mum_c[0], "CIGS 2#mum CH0 Peak mean ","lpe");
  leg03->AddEntry(gPeak_mean_2mum_c[1], "CIGS 2#mum CH1 Peak mean ","lpe");
  leg03->AddEntry(gPeak_mean_5mum_c[0], "CIGS 5#mum CH0 Peak mean ","lpe");
  leg03->AddEntry(gPeak_mean_5mum_c[1], "CIGS 5#mum CH1 Peak mean ","lpe");

  TCanvas* c03 = new TCanvas("c03","c03");
  c03->cd();
  gPeak_mean_2mum_c[0] ->GetYaxis()->SetRangeUser(0.1,0.5);
  //  gPeak_mean_2mum_c[0] ->SetTitleSize(0);
  gPeak_mean_2mum_c[0] ->Draw("PLC PMC AP");
  gPeak_mean_2mum_c[1] ->Draw("PLC PMC P");
  gPeak_mean_5mum_c[0] ->Draw("PLC PMC P");
  gPeak_mean_5mum_c[1] ->Draw("PLC PMC P");
  leg03->Draw();
  

  
  
    
  
}


vector<string> split(string&input, char delimiter){

  istringstream stream(input);
  string field;
  vector<string> result;
  while(getline(stream,field,delimiter))
    result.push_back(field);
  return result;
};



double GetHV(int run ){

  double HV = 0;
  if(run==64) HV = -2.5;
  else if(run==65) HV = -1.5;
  else if(run==66) HV = -1.0;
  else if(run==67) HV = -0.5;
  else if(run==68) HV = -0.3;
  else if(run==69) HV = -0.1;
  else if(run==70) HV = -3.0;
  else if(run==71) HV = -3.0;
  else if(run==33) HV = -1.5;
  else if(run==34) HV = -1.0;
  else if(run==35) HV = -0.5;
  else if(run==36) HV = -0.3;
  else if(run==37) HV = -0.1;
  else  HV = -2.0;


  return HV;

};


double GetCharge(double*x, double*par){

  double a,b; // y = a*x +b ;
  par[0] ; // Channel param 0=CH0 or 1=CH1
  par[1] ; // Number of config file : 1 = CIGS 2um , 2= CIGS 5um
  if(par[0]==0 && par[1]==1){a = 2894.8; b = 17.4009;} // config 1 CH0
  else if(par[0]==1 && par[1]==1){a = 2790.5; b = 24.6268;} // config 1 CH1
  else if(par[0]==0 && par[1]==2){a = 1113.78; b = 11.8572;} // config 2 CH0
  else if(par[0]==1 && par[1]==2){a = 1076.6; b = 3.30293;} // config 2 CH1
  //  if(par[0]==0 && par[1]==1){a = 87.7; b = 17.4009;} // config 1 CH0
  //  else if(par[0]==1 && par[1]==1){a = 84.6; b = 24.6268;} // config 1 CH1
  //  else if(par[0]==0 && par[1]==2){a = 33.8; b = 11.8572;} // config 2 CH0
  //  else if(par[0]==1 && par[1]==2){a = 32.6; b = 3.30293;} // config 2 CH1
  else {a=0; b=0;}


  double y = (x[0]-b)/a ;
  return y;

}
