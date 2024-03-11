//#ifdef hist_h
//#define hist_h 1
#include "Setting.h"
#include "Param.h"
//#include "TGraphErrors.h"
TGraphErrors* gCV[nfile_max];
TGraphErrors* gV_C[nfile_max];
TGraphErrors* gWV[nfile_max];
TGraphErrors* gNV[nfile_max];
TGraphErrors* gWN[nfile_max];
TGraphErrors* gCV_Diff[nfile_max];
TGraphErrors* gWV_Diff[nfile_max];
TGraphErrors* gNV_Diff[nfile_max];
TGraphErrors* gWV_Diff_2V[nch];
TGraphErrors* gCV_Diff_2V[nch];


TGraphErrors* gIV[nfile_max];
TGraphErrors* gJV[nfile_max];
TGraphErrors* gIV_mA[nfile_max];
TGraphErrors* gJV_mA[nfile_max];
TGraphErrors* gIV_abs[nfile_max];
TGraphErrors* gJV_abs[nfile_max];
TGraphErrors* gIV_Diff[nfile_max];
TGraphErrors* gJV_Diff[nfile_max];
TGraphErrors* gJV_mA_Diff[nfile_max];



TGraphErrors* gIsc;
TGraphErrors* gJsc;
TGraphErrors* gFF;
TGraphErrors* geta;
TGraphErrors* gPm;
TGraphErrors* gRsh;
TGraphErrors* gRs;

TGraphErrors* gIsc_TIME[NID][NCH];
TGraphErrors* gJsc_TIME[NID][NCH];
TGraphErrors* gI1V_TIME[NID][NCH];
TGraphErrors* gJ1V_TIME[NID][NCH];
TGraphErrors* gI2V_TIME[NID][NCH];
TGraphErrors* gJ2V_TIME[NID][NCH];
TGraphErrors* gVoc_TIME[NID][NCH];
TGraphErrors* gVm_TIME[NID][NCH];
TGraphErrors* gIm_TIME[NID][NCH];
TGraphErrors* gJm_TIME[NID][NCH];
TGraphErrors* gFF_TIME[NID][NCH];
TGraphErrors* geta_TIME[NID][NCH];
TGraphErrors* gPm_TIME[NID][NCH];
TGraphErrors* gRsh_TIME[NID][NCH];
TGraphErrors* gRs_TIME[NID][NCH];

TGraphErrors* gN_TIME[NID][NCH];
TGraphErrors* gN2_TIME[NID][NCH];

TGraphErrors* gJsc_Diff_TIME[NID][NCH];
TGraphErrors* gIsc_Diff_TIME[NID][NCH];
TGraphErrors* gJ1V_Diff_TIME[NID][NCH];
TGraphErrors* gJ2V_Diff_TIME[NID][NCH];

TGraphErrors* gIsc_TIME_R[NID][NCH];
TGraphErrors* gJsc_TIME_R[NID][NCH];
TGraphErrors* gI1V_TIME_R[NID][NCH];
TGraphErrors* gJ1V_TIME_R[NID][NCH];
TGraphErrors* gI2V_TIME_R[NID][NCH];
TGraphErrors* gJ2V_TIME_R[NID][NCH];
TGraphErrors* gVoc_TIME_R[NID][NCH];
TGraphErrors* gVm_TIME_R[NID][NCH];
TGraphErrors* gIm_TIME_R[NID][NCH];
TGraphErrors* gJm_TIME_R[NID][NCH];
TGraphErrors* gFF_TIME_R[NID][NCH];
TGraphErrors* geta_TIME_R[NID][NCH];
TGraphErrors* gPm_TIME_R[NID][NCH];
TGraphErrors* gRsh_TIME_R[NID][NCH];
TGraphErrors* gRs_TIME_R[NID][NCH];

TGraphErrors* gJsc_Diff_TIME_R[NID][NCH];
TGraphErrors* gIsc_Diff_TIME_R[NID][NCH];
TGraphErrors* gJ1V_Diff_TIME_R[NID][NCH];
TGraphErrors* gJ2V_Diff_TIME_R[NID][NCH];


TGraphErrors* gJsc_CYRIC_TIME_R[NID][NCH];
TGraphErrors* gJ2V_CYRIC_TIME_R[NID][NCH];
TGraphErrors* geta_CYRIC_TIME_R[NID][NCH];

TGraphErrors* gJsc_TIME_DIFF_R[NID][NCH];
TGraphErrors* gJ2V_TIME_DIFF_R[NID][NCH];
TGraphErrors* geta_TIME_DIFF_R[NID][NCH];


TGraphErrors* gIsc_TIME_H[NID][NCH];
TGraphErrors* gI1V_TIME_H[NID][NCH];
TGraphErrors* gI2V_TIME_H[NID][NCH];
TGraphErrors* gJsc_TIME_H[NID][NCH];
TGraphErrors* gJ1V_TIME_H[NID][NCH];
TGraphErrors* gJ2V_TIME_H[NID][NCH];
TGraphErrors* gIsc_TIME_H_R[NID][NCH];
TGraphErrors* gJsc_TIME_H_R[NID][NCH];
TGraphErrors* gI1V_TIME_H_R[NID][NCH];
TGraphErrors* gI2V_TIME_H_R[NID][NCH];
TGraphErrors* gJ1V_TIME_H_R[NID][NCH];
TGraphErrors* gJ2V_TIME_H_R[NID][NCH];

// =========< CV Hist >==================//
TGraphErrors* gW_TIME[NID][NCH];
TGraphErrors* gW_TIME_R[NID][NCH];
TGraphErrors* gC_TIME[NID][NCH];
TGraphErrors* gC_TIME_R[NID][NCH];
TGraphErrors* gNa_TIME[NID][NCH];
TGraphErrors* gNa_TIME_R[NID][NCH];






TGraphErrors* gIsc_AIST;
TGraphErrors* gJsc_AIST;
TGraphErrors* gFF_AIST;
TGraphErrors* geta_AIST;
TGraphErrors* gPm_AIST;
TGraphErrors* gRsh_AIST;
TGraphErrors* gRs_AIST;

TGraphErrors* gIsc_KEK;
TGraphErrors* gJsc_KEK;
TGraphErrors* gFF_KEK;
TGraphErrors* geta_KEK;
TGraphErrors* gPm_KEK;
TGraphErrors* gRsh_KEK;
TGraphErrors* gRs_KEK;

TGraphErrors* gIsc_Rel;
TGraphErrors* gJsc_Rel;
TGraphErrors* gFF_Rel;
TGraphErrors* geta_Rel;
TGraphErrors* gPm_Rel;
TGraphErrors* gRsh_Rel;
TGraphErrors* gRs_Rel;


//#endif
