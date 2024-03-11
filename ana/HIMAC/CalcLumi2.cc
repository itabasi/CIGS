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
extern int unixt_run(int runnum);
const int nch =8;
//const int nrun =29;
const int nrun =21;
//const int runID[nrun]={32, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 53, 54, 55, 60, 73, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84};
const int runID[nrun]={38, 39, 40, 41, 42, 43, 44, 45, 46, 48, 49, 50, 53, 54, 55, 60, 73, 75, 76, 78, 79};
const int runID_anealing[2]={60,73};
const int anealing_st[2] ={1669364720,1669371120};
const int anealing_end[2]={1669371120,1669377000};
string TIMES_s[100000];
int TIMES[100000];
int TIMES_u[100000];
float TIMES_f[100000];
int SCALE0[10000];
int SCALE1[10000];
int SCALE2[10000];
int SCALE3[10000];
float SCALE4[10000];
int ID[10000];


void CalcLumi2(){

  string sfile[2];
  sfile[0] ="./scaler_25.txt";
  sfile[1] ="./scaler_26.txt";

  ifstream ifS(sfile[0].c_str(),ios::in); // Scale file
  ifstream ifS1(sfile[1].c_str(),ios::in); // Scale file
  string buf;

  
  int iii=0;
  int day;
  float Lumi[2] ={0.0};
  float Lumi_Run[nrun][2];
  float Lumi_err[2] ={0.0};
  float Lumi_Run_err[nrun][2];
  double Time_Run[nrun];
  for(int i=0;i<nrun;i++){
    Time_Run[i]=0.0;
    for(int j=0;j<2;j++){
      Lumi_Run[i][j] = 0.0;
      Lumi_Run_err[i][j] = 0.0;
    }
  }
    

  int scale_num =0;

  float threshold0 = 35000;
  // threshold0 = 0.0;
  float threshold1 = 60000;
  float threshold_under = 100;
  double nhits_sec[2][3];
  double nhits_sec_err[2][3];
  double daq_fac = 1./64.5e6 * 2048.;

  nhits_sec[0][0] = 13.6 /daq_fac;
  nhits_sec[1][0] = 15.1 /daq_fac;
  nhits_sec[0][1] = 10.1 /daq_fac;
  nhits_sec[1][1] = 9.8  /daq_fac;
  nhits_sec[0][2] = 8.1  /daq_fac;
  nhits_sec[1][2] = 10.5 /daq_fac;

  nhits_sec_err[0][0] = 13.6 /daq_fac*0.174;
  nhits_sec_err[1][0] = 15.1 /daq_fac*0.183;
  nhits_sec_err[0][1] = 10.1 /daq_fac*0.715;
  nhits_sec_err[1][1] = 9.8  /daq_fac*0.486;
  nhits_sec_err[0][2] = 8.1  /daq_fac*0.277;
  nhits_sec_err[1][2] = 10.5 /daq_fac*0.465;


  int th_num;
  double dtime ;
  double dtime_anealing;
  double sum_time =0;
  vector<string>strvec;
  double cigs_factor = 5.7e3*2.0e-6*4.0e-7;
  double beam_eff = 0.162;
  double MGy_fac =6.3/6.24e12/cigs_factor*1.0e-6;

  
  for(int i =0; i<2; i++){
    while(1){
      if(i==0){
	getline(ifS,buf);
	if(ifS.eof() || buf.size()==0)
	  break;
      }
      if(i==1){
	getline(ifS1,buf);
	if(ifS1.eof() || buf.size()==0)break;
      }
      
      TIMES[iii] =-100.; SCALE1[iii] = -100.; SCALE2[iii] = -100.;
      SCALE3[iii]= -100.;
      strvec = split(buf,',');
      stringstream sbuf(buf);
      SCALE3[iii] = stof(strvec.at(5));
      TIMES_s[iii] = strvec.at(1);
      if(strvec.size()<3) break;
      TIMES_u[iii] =time_to_unixt(TIMES_s[iii],25+i);
      
      if(SCALE3[iii] < threshold0)th_num =0;
      else if(SCALE3[iii] < threshold1)th_num =1;
      else th_num =2;

      //      th_num =2; // test
      
      if(iii>0 && SCALE3[iii] > threshold_under ){
	  dtime = (double)(TIMES_u[iii]-TIMES_u[iii-1]);
	  if((anealing_st[0] < TIMES_u[iii] && TIMES_u[iii] < anealing_end[0]) ||	     (anealing_st[1] < TIMES_u[iii] && TIMES_u[iii] < anealing_end[1]))	   dtime = 0.0;
	 
	  
      }else dtime = 0.0;

      for(int irun =0; irun <nrun; irun ++){
	if(TIMES_u[iii] < unixt_run(runID[irun])){	    
	  Lumi_Run[irun][0] += nhits_sec[0][th_num]*dtime*beam_eff*MGy_fac;
	  Lumi_Run[irun][1] += nhits_sec[1][th_num]*dtime*beam_eff*MGy_fac;
	  Lumi_Run_err[irun][0] += nhits_sec_err[0][th_num]*dtime*beam_eff*MGy_fac;
	  Lumi_Run_err[irun][1] += nhits_sec_err[1][th_num]*dtime*beam_eff*MGy_fac;
	  Time_Run[irun]  += dtime;
	}
      }
	Lumi[0] += nhits_sec[0][th_num]*dtime*beam_eff*MGy_fac;
	Lumi[1] += nhits_sec[1][th_num]*dtime*beam_eff*MGy_fac;
	Lumi_err[0] += nhits_sec_err[0][th_num]*dtime*beam_eff*MGy_fac;
	Lumi_err[1] += nhits_sec_err[1][th_num]*dtime*beam_eff*MGy_fac;

	sum_time += dtime;

      iii++;
    }
  }




  // ====== output Luminosity File ========= //
  string ofp_name ="./runlist/luminosity.txt";
  ofstream ofp(ofp_name.c_str());
  for(int irun =0; irun<nrun ; irun++){
    ofp << runID[irun] <<" "<<Lumi_Run[irun][0] <<" "<<Lumi_Run[irun][1]<<" "<<Time_Run[irun]<<endl;
  }

  cout<<"nhits_sec run39 "<<nhits_sec[0][1]*Time_Run[1]*beam_eff<<" "<<" time "<<Time_Run[1]<<" clock "<<endl;
  
  cout<<"======= Number of particle at CIGS ========== "<<endl;
  cout<<"Total ppp "<<SCALE3[iii-1]<<endl;
  cout<<"Total Beam time "<<sum_time<<" sec "<<endl;

  cout<<"Total Hits 0 "<<Lumi[0]/MGy_fac<<" +- "<<Lumi_err[0]/MGy_fac<<endl;
  cout<<"Total Hits 1 "<<Lumi[1]/MGy_fac<<" +- "<<Lumi_err[1]/MGy_fac<<endl;
  cout<<"Lumi 0 "<<Lumi[0]<<" +- " <<Lumi_err[0]<<" MGy "<<endl;
  cout<<"Lumi 1 "<<Lumi[1]<<" +- "<<Lumi_err[1]<<" MGy "<<endl;


  cout<<"======= Before Anealing =============="<<endl;
  cout<<"Total Hits 0 "<<Lumi_Run[14][0]/MGy_fac<<" +- "<<Lumi_Run_err[14][0]/MGy_fac<<endl;
  cout<<"Total Hits 1 "<<Lumi_Run[14][1]/MGy_fac<<" +- "<<Lumi_Run_err[14][1]/MGy_fac<<endl;
  cout<<"Lumi 0 "<<Lumi_Run[14][0]<<" +- "<<Lumi_Run_err[14][0]<<" MGy "<<endl;
  cout<<"Lumi 1 "<<Lumi_Run[14][1]<<" +- "<<Lumi_Run_err[14][1]<<" MGy "<<endl;

  
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


int unixt_run(int runnum){

  int unixt[100];
  //  unixt[0] = 1669302000; // RUN32 2022/11/25 00:00:00
  unixt[0] = 1669305540; // RUN38 2022/11/25 00:59:00
  unixt[1] = 1669307400; // RUN39 2022/11/25 01:30:00
  unixt[2] = 1669312260; // RUN40 2022/11/25 02:51:00
  unixt[3] = 1669316880; // RUN41 2022/11/25 04:08:00
  unixt[4] = 1669321260; // RUN42 2022/11/25 05:21:00
  unixt[5] = 1669326300; // RUN43 2022/11/25 06:45:00
  unixt[6] = 1669330500; // RUN44 2022/11/25 07:55:00
  unixt[7] = 1669334700; // RUN45 2022/11/25 09:05:00
  unixt[8] = 1669339380; // RUN46 2022/11/25 10:23:00
  //  unixt[10] = 1669339740; // RUN47 2022/11/25 10:29:00
  unixt[9] = 1669343880; // RUN48 2022/11/25 11:38:00
  unixt[10] = 1669348080; // RUN49 2022/11/25 12:48:00
  unixt[11] = 1669352280; // RUN50 2022/11/25 13:58:00  
  unixt[12] = 1669356180; // RUN53 2022/11/25 15:03:00
  unixt[13] = 1669360200; // RUN54 2022/11/25 16:10:00
  unixt[14] = 1669360800; // RUN55 2022/11/25 16:20:00
  unixt[15] = 1669371120; // RUN60 2022/11/25 19:12:00
  unixt[16] = 1669377000; // RUN73 2022/11/25 20:50:00
  unixt[17] = 1669382220; // RUN75 2022/11/25 22:17:00
  unixt[18] = 1669386480; // RUN76 2022/11/25 23:28:00
  //  unixt[21] = 1669387260; // RUN77 2022/11/25 23:41:00  
  unixt[19] = 1669395360; // RUN78 2022/11/26 1:56:00
  unixt[20] = 1669399980; // RUN79 2022/11/26 3:13:00

  // end of RUN55 2022/11/25 17:14:20 1669305540			     
  
  for(int irun =0; irun<nrun;irun++){
    if(runnum == runID[irun])
      return unixt[irun];
  }


  return 0;


}


