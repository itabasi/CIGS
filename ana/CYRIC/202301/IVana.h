#ifndef IVana_h
#define IVana_h 1
#include "Setting.h"
#include "Param.h"
#include "hist.h"
const  int ncanvas =11;
class IVana{
  
 public:
  IVana();
  ~IVana();

 public:
  void SetRun(string ifname);
  void GetIV_AIST(vector<string> strvec, int ifile);
  void GetIV_KEK(vector<string> strvec, int ifile);
  void GetIV(string ifname, int ifile);
  void GetVal(int ifile, int isamp);
  void SetVal(int ifile, int isamp);
  TGraphErrors* GetIV_Diff(TGraphErrors* gLIGHT, TGraphErrors* gDARK);
  void SetIVDiff(TGraphErrors* gDiff);
  void Initialization();
  void Loop();
  void SetGraph();
  void Draw_CYRIC();
  void Draw();
  void Ana();
  void NewROOT(string ofrname);
  void Write();
  double InitialVal(string par, int id, int ch);
  string IVdata[nfile_max];
  double I[nfile_max][nsample_max];
  double J[nfile_max][nsample_max];
  double V[nfile_max][nsample_max];
  double W[nfile_max][nsample_max];
  double Pm[nfile_max];
  double Voc[nfile_max];
  double Isc[nfile_max];
  double Jsc[nfile_max];
  double I_1V[nfile_max];
  double I_2V[nfile_max];
  double J_1V[nfile_max];
  double J_2V[nfile_max];
  double Rs[nfile_max];
  double Rsh[nfile_max];
  double FF[nfile_max];
  double eta[nfile_max];
  double Jsc_Diff[nfile_max];
  double J1V_Diff[nfile_max];
  double J2V_Diff[nfile_max];
  double Jsc_tmp;
  double J1V_tmp;
  double J2V_tmp;

  int id_diff[100];
  int ch_diff[100];
  int diff_file[100];
  double TIME_MAX =0;
  int N_DIFF =0;
  //  int id_diff[nfile_max];
  //  int ch_diff[nfile_max];
  //  double temp;
  //  string time_unit;
  //  double TIME[nfile_max];
  //  string file_name[nfile_max];
  //  string file_ID[nfile_max];
  //  string file_CH[nfile_max];
  //  string file_LIGHT[nfile_max];
  //  string file_Temp[nfile_max];
  //  string file_Time[nfile_max];
  //  string file_SU[nfile_max]; // Setup KEK or AIST
  //  string diff_name[nfile_max]; // Setup KEK or AIST
  //  int n_id=0;
  //  int n_ch=0;
  //  int id_num[NID];
  //  int ch_num[NCH];
  TFile* ofr;
  int nfile =0;
  int niv[nfile_max];
  TCanvas* c[ncanvas];
  TLegend* leg[ncanvas];

  TCanvas* cTIME[ncanvas];
  TLegend* leg_TIME[ncanvas];

  TCanvas* civ[100];
  TLegend* leg_iv[100];

  TCanvas* civ_diff[100];
  TLegend* leg_iv_diff[100];

  
  TCanvas* cCYRIC[ncanvas];
  TLegend* leg_CYRIC[100];

  TCanvas* cpic[100];
  TLegend* leg_pic[100];

  TGraphErrors* gTemp_Diff[100];
  
  
};

IVana::IVana(){}
IVana::~IVana(){}

#endif
