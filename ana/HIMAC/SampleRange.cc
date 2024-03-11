#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <TGraph.h>
extern vector<string> split(string&input, char delimiter);

const int nch=2;

void SampleRange(){

  string fname ="runlist/SampleRoot.txt";
  string rname[200];
  TFile* f[200];
  ifstream ifp(fname.c_str(),ios::in);
  string buf;
  TH1D* hIADC_ped[200][nch];
  TH1D* hIADC_peak[200][nch];
  TH1D* hIADC[200][nch];
  int s=0;
  int x_st[200],x_end[200], run[200];
  vector<string> strvec;
  vector<string> strvec2;
  vector<string> strvec3;
  string sbuf2 ;

  
  int nst =10;
  int nend=10;
  int st_min = 10;
  int st_max =37;
  int end_min =40;
  int end_max =85;

  if(fname=="runlist/SampleRoot.txt"){
    nst =10;
    nend=10;
    st_min = 10;
    st_max =28;
    end_min =45;
    end_max =81;
  }
  
  while(1){
    getline(ifp,buf);
    if(buf[0]=='#'){continue;}
    if(ifp.eof()) break;
    stringstream sbuf(buf);
    sbuf >> rname[s] ;
    f[s] = new TFile(rname[s].c_str());

    strvec = split(buf,'/');
    strvec2  = split(strvec.at(strvec.size()-1),'_');
    strvec3  = split(strvec2.at(strvec2.size()-1),'.');
    run[s]   = atoi(strvec2.at(strvec2.size()-3).c_str());
    x_st[s]  = atoi(strvec2.at(strvec2.size()-2).c_str());
    x_end[s] = atoi(strvec3.at(0).c_str());

    
    for(int i=0; i< nch; i++){
      hIADC_ped[s][i] = (TH1D*)f[s]->Get(Form("h_IntegratedADC_%d",i));
      hIADC_ped[s][i] -> SetName(Form("hIADC_ped_%d_%d_%d",x_st[s], x_end[s],i));
      hIADC_ped[s][i]->SetTitle(Form("hIADC CH%d  x_st = %3d, x_end =%3d ; IntegratedADC [ch] ; Counts",i,x_st[s],x_end[s]));
      hIADC_peak[s][i] = (TH1D*)f[s]->Get(Form("h_IntegratedADC_%d",i));
      hIADC_peak[s][i] -> SetName(Form("hIADC_peak_%d_%d_%d",x_st[s], x_end[s],i));      
    }
    s++;
  }

  

  //============ Fitting ============================//
  double ped_sigma[200][nch];
  double ped_mean[200][nch];
  double peak_sigma[200][nch];
  double peak_mean[200][nch];
  double peak_chi2[200][nch];
  double ped_chi2[200][nch];
  double ped_mean_def = 0.0;
  double ped_sigma_def = 5.0;
  

  TF1* fPed[200][nch];
  TF1* fPeak[200][nch];

  
  double peak_mean_def = 250.0;
  double peak_sigma_def = 60.0;


  TH2D* hPed_mean[nch];
  TH2D* hPed_sigma[nch];
  TH2D* hPed_Chi2[nch];


  TH2D* hPeak_mean[nch];
  TH2D* hPeak_sigma[nch];
  TH2D* hPeak_Chi2[nch];
  TH2D* hSN[nch];
  TH2D* hRes_peak[nch];
  TH2D* hRes_ped[nch];
  
  for(int ich=0; ich<nch;ich++){

    double xmax_st = (st_max -st_min)/(double)(nst-1)*(double)(ich) + st_min;
    double st_dx   = (st_max -st_min)/(double)(nst-1)/2.;
    double xmax_end = (end_max -end_min)/(double)(nend-1)*(double)(ich) + end_min;
    double end_dx   = (end_max -end_min)/(double)(nend-1)/2.;    

    hPed_mean[ich] = new TH2D(Form("hPed_mean_%d",ich),Form("hPed_mean_%d ; sample_st [ch] ; sample_end [ch]",ich),nst,st_min-st_dx,st_max +st_dx,nend,end_min-end_dx,end_max+end_dx);
    hPed_sigma[ich] = new TH2D(Form("hPed_sigma_%d",ich),Form("hPed_sigma_%d ; sample_st [ch] ; sample_end [ch]",ich),nst,st_min-st_dx,st_max +st_dx,nend,end_min-end_dx,end_max+end_dx);
    hPed_Chi2[ich] = new TH2D(Form("hPed_Chi2_%d",ich),Form("hPed_Chi2_%d ; sample_st [ch] ; sample_end [ch]",ich),nst,st_min-st_dx,st_max +st_dx,nend,end_min-end_dx,end_max+end_dx);

    hPeak_mean[ich] = new TH2D(Form("hPeak_mean_%d",ich),Form("hPeak_mean_%d ; sample_st [ch] ; sample_end [ch]",ich),nst,st_min-st_dx,st_max +st_dx,nend,end_min-end_dx,end_max+end_dx);
    hPeak_sigma[ich] = new TH2D(Form("hPeak_sigma_%d",ich),Form("hPeak_sigma_%d ; sample_st [ch] ; sample_end [ch]",ich),nst,st_min-st_dx,st_max +st_dx,nend,end_min-end_dx,end_max+end_dx);
    hPeak_Chi2[ich] = new TH2D(Form("hPeak_Chi2_%d",ich),Form("hPeak_Chi2_%d ; sample_st [ch] ; sample_end [ch]",ich),nst,st_min-st_dx,st_max +st_dx,nend,end_min-end_dx,end_max+end_dx);
    hSN[ich] = new TH2D(Form("hSN_%d",ich),Form("hSN_%d ; sample_st [ch] ; sample_end [ch]",ich),nst,st_min-st_dx,st_max +st_dx,nend,end_min-end_dx,end_max+end_dx);
    hRes_ped[ich] = new TH2D(Form("hRes_ped_%d",ich),Form("hRes_ped_%d ; sample_st [ch] ; sample_end [ch]",ich),nst,st_min-st_dx,st_max +st_dx,nend,end_min-end_dx,end_max+end_dx);
    hRes_peak[ich] = new TH2D(Form("hRes_peak_%d",ich),Form("hRes_peak_%d ; sample_st [ch] ; sample_end [ch]",ich),nst,st_min-st_dx,st_max +st_dx,nend,end_min-end_dx,end_max+end_dx);                

    
    for(int i=0; i<s;i++){
      fPed[i][ich] = new TF1(Form("fPed_%d_%d",i,ich),"gausn(0)",-100,100);
      fPeak[i][ich] = new TF1(Form("fPeak_%d_%d",i,ich),"gausn(0)",100,800);

      fPed[i][ich]->SetParameter(1,ped_mean_def);
      fPed[i][ich]->SetParameter(2,ped_sigma_def);
      hIADC_ped[i][ich] ->Fit(Form("fPed_%d_%d",i,ich),"QR","QR",-50,50);
      ped_mean[i][ich]  = fPed[i][ich]->GetParameter(1);
      ped_sigma[i][ich] = fPed[i][ich]->GetParameter(2);
      ped_chi2[i][ich]  = fPed[i][ich]->GetChisquare() / fPed[i][ich]->GetNDF();
      
      hPed_sigma[ich] ->Fill((double)x_st[i],(double)x_end[i],ped_sigma[i][ich]);
      hPed_mean[ich] ->Fill((double)x_st[i],(double)x_end[i],ped_mean[i][ich]);
      hPed_Chi2[ich] ->Fill((double)x_st[i],(double)x_end[i],ped_chi2[i][ich]);
      fPeak[i][ich]->SetParameter(1,peak_mean_def);
      fPeak[i][ich]->SetParameter(2,peak_sigma_def);
      hIADC_peak[i][ich] ->Fit(Form("fPeak_%d_%d",i,ich),"QR","QR",0,800);
      peak_mean[i][ich]  = fPeak[i][ich]->GetParameter(1);
      peak_sigma[i][ich] = fPeak[i][ich]->GetParameter(2);
      peak_chi2[i][ich]  = fPeak[i][ich]->GetChisquare() / fPeak[i][ich]->GetNDF();
      if(peak_chi2[i][ich]>30){ // re-fit
      fPeak[i][ich]->SetParameter(1,peak_mean[i-1][ich]);
      fPeak[i][ich]->SetParameter(2,peak_sigma[i-1][ich]);
      hIADC_peak[i][ich] ->Fit(Form("fPeak_%d_%d",i,ich),"QR","QR",0,800);
      peak_mean[i][ich]  = fPeak[i][ich]->GetParameter(1);
      peak_sigma[i][ich] = fPeak[i][ich]->GetParameter(2);
      peak_chi2[i][ich]  = fPeak[i][ich]->GetChisquare() / fPeak[i][ich]->GetNDF();
      if(peak_chi2[i][ich]>30){
      peak_mean[i][ich]  = fPeak[i-1][ich]->GetParameter(1);
      peak_sigma[i][ich] = fPeak[i-1][ich]->GetParameter(2);
      peak_chi2[i][ich]  = fPeak[i-1][ich]->GetChisquare() / fPeak[i-1][ich]->GetNDF();

      
      }

      }


      hRes_peak[ich] ->Fill((double)x_st[i],(double)x_end[i],peak_sigma[i][ich]/peak_mean[i][ich]);
      hRes_ped[ich] ->Fill((double)x_st[i],(double)x_end[i],ped_sigma[i][ich]/ped_mean[i][ich]);


      
      hPeak_sigma[ich] ->Fill((double)x_st[i],(double)x_end[i],peak_sigma[i][ich]);
      hPeak_mean[ich] ->Fill((double)x_st[i],(double)x_end[i],peak_mean[i][ich]);      
      hPeak_Chi2[ich] ->Fill((double)x_st[i],(double)x_end[i],peak_chi2[i][ich]);

      hSN[ich] ->Fill((double)x_st[i],(double)x_end[i],peak_mean[i][ich]/(ped_mean[i][nch] + ped_sigma[i][nch]));


    }
  }

  //================================================//


  TCanvas* c[100];
  int ncanvas=24;
  for(int i=0;i<ncanvas;i++){
    c[i] = new TCanvas(Form("c%d",i),Form("c%d",i));
  }

  c[0]->Divide(2,2);
  c[0] ->cd(1);
  hPed_mean[0]->Draw("colz");
  c[0] ->cd(2);
  hPed_mean[1]->Draw("colz");
  c[0] ->cd(3);
  hPeak_mean[0]->Draw("colz");
  c[0] ->cd(4);
  hPeak_mean[1]->Draw("colz");


  c[1]->Divide(2,2);
  c[1]->cd(1);
  hPed_sigma[0]->Draw("colz");
  c[1] ->cd(2);
  hPed_sigma[1]->Draw("colz");
  c[1] ->cd(3);
  hPeak_sigma[0]->Draw("colz");
  c[1] ->cd(4);
  hPeak_sigma[1]->Draw("colz");

  c[2]->Divide(2,2);
  c[2]->cd(1);
  hPed_Chi2[0]->Draw("colz");
  c[2] ->cd(2);
  hPed_Chi2[1]->Draw("colz");
  c[2] ->cd(3);
  hPeak_Chi2[0]->Draw("colz");
  c[2] ->cd(4);
  hPeak_Chi2[1]->Draw("colz");
  
 
  c[3]->Divide(2,2);
  c[3] ->cd(1);
  hRes_peak[0]->Draw("colz");
  //  hSN[0]->Draw("colz");
  c[3] ->cd(2);
  hRes_peak[1]->Draw("colz");
  //  hSN[1]->Draw("colz");  
  c[3] ->cd(3);
  hRes_ped[0]->Draw("colz");
  c[3] ->cd(4);
  hRes_ped[1]->Draw("colz");


  for(int i =0; i<10;i++){
    int j = i+4;
    c[j] -> Divide(3,3);
    c[j+10] -> Divide(3,3);
    for(int k =0; k<9; k++){
      c[j]->cd(k+1);
      gPad->SetLogy(1);
      hIADC_ped[i*10+k][0] ->Draw();
      fPed[i*10+k][0]      ->Draw("same");
      fPeak[i*10+k][0]      ->Draw("same");
      c[j+10]->cd(k+1);
      gPad->SetLogy(1);
      hIADC_ped[i*10+k][1] ->Draw();
      fPed[i*10+k][1]      ->Draw("same");
      fPeak[i*10+k][1]      ->Draw("same");
    }
  }
  

  gStyle->SetOptStat(0000);
  
  string pdf_name ="test.pdf";
  c[0]->Print(Form("%s[",pdf_name.c_str()));
  for(int i=0; i<ncanvas;i++){
  c[i]->Print(Form("%s",pdf_name.c_str()));
  }
  c[ncanvas-1]->Print(Form("%s]",pdf_name.c_str()));
  
}


vector<string> split(string&input, char delimiter){

  istringstream stream(input);
  string field;
  vector<string> result;
  while(getline(stream,field,delimiter))
    result.push_back(field);
  return result;
};
