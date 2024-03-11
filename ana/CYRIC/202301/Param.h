#ifndef Param_h
#define Param_h 1
const int nfile_max = 100;
const int nsample_max =200;
const int NID =20;
const int NCH =2;
const int NTEMP=10;
const double epsilon_0 = 8.854188e-14; // F/m
const double ep_CIGS = 13.5*epsilon_0;
const double e =  1.602176565*1e-19;    // electron charge magnitude (C)
double cell_area = 0.522; // [cm^2]
const double Pin = 0.1; //[W/cm^2]
const int nch =2;
//const double Vset =-2.0; // [V]
const double Vset =0.0; // [V]
double TIME[nfile_max];
string file_name[nfile_max];
string file_ID[nfile_max];
string file_CH[nfile_max];
string file_LIGHT[nfile_max];
string file_Temp[nfile_max];
string file_Time[nfile_max];
string file_SU[nfile_max]; // Setup KEK or AIST
string diff_name[nfile_max]; // Setup KEK or AIST
int n_id=0;
int n_ch=0;
int id_num[NID];
int ch_num[NCH];
string TEMP[NID][NCH];
double temp;
string time_unit;

#endif 
