#ifndef CVana_h
#define CVana_h 1
#include "Setting.h"
#include "Param.h"
#include "hist.h"

const  int ncanvas =8;
const  int ncanvas_CYRIC =8;
class CVana{

public :
  CVana();
  ~CVana();

public:

  void ReadFile(string ifname);
  void GetCV(string ifname, int ifile);
  void SetGraph();
  void GetVal(double Vol, double Cp, int ifile, int isample);
  void SetVal(int ifile, int isample);
  double GetVal_N(TGraphErrors*g,int i);
  TGraphErrors* GetNW(TGraphErrors* gN, TGraphErrors* gW);
  void Loop();
  void Draw();
  void Ana();
  void Ana_CYRIC();
  int nfile =0;
  int ncv[nfile_max];
  double scale_w[2]={2.15662, 2.15625}; // HIMAC Data
  TCanvas* c[ncanvas];
  TCanvas* cv[ncanvas];
  string CVdata[nfile_max];
  double C[nfile_max][nsample_max];
  double V[nfile_max][nsample_max];
  double W[nfile_max][nsample_max];
  double Na[nfile_max][nsample_max];
  double Na0[nfile_max];
  string file_status[nfile_max];
  string file_anealing_time[nfile_max];
  int SEG[nfile_max];
  TLegend* leg[ncanvas];
  TLegend* leg_cv[100];

  TF1* f = new TF1("f","[0]*x+[1]",-3,0);  
};

CVana::CVana(){}
CVana::~CVana(){}
  
  
#endif
