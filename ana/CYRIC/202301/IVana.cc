#include "IVana.h"
extern vector<string> split(string&input, char delimiter);

bool CYRIC_ANALYSIS = false;
const     double A_to_mA = 1000.;
void IVana::SetRun(string ifname){

  string buf;
  ifstream ifp(Form("%s",ifname.c_str()),ios::in);
  vector<string>strvec;
  vector<string>strvec2;
  vector<string>strvec3;
  
  nfile=0;
  while(1){
    getline(ifp,buf);
    if(ifp.eof() || buf.size()==0)break;
    if( buf[0]=='#' ){ continue; }
    stringstream sbuf(buf);
    sbuf >> IVdata[nfile];
    strvec  = split(IVdata[nfile],'/');
    strvec2 = split(strvec[strvec.size()-1],'_');
    strvec3 = split(strvec2[strvec2.size()-1],'.');
    file_name[nfile] =strvec2[1] + strvec2[2] +" " + strvec3[0] ;
    file_ID[nfile] = strvec2[1];
    file_CH[nfile] = strvec2[2];
    file_LIGHT[nfile] = strvec2[3];
    string id_st = file_ID[nfile].substr(2);
    string ch_st = file_CH[nfile].substr(2);
    int id = stoi(id_st.c_str());
    int ch = stoi(ch_st.c_str());

    for(int iid =0; iid<=n_id;iid++){
      if(id == id_num[iid]) break;
      else if(iid == n_id && id != id_num[iid]){
	id_num[iid] = id;
	n_id ++;
	break;
      }
    }

    for(int ich =0; ich<=n_ch;ich++){
      if(ch == ch_num[ich]) break;
      else if(ich == n_ch && ch != ch_num[ich]){
	ch_num[ich] = ch;
	n_ch ++;
	break;
      }
    }

    
    if(strvec2.size() > 5){
      CYRIC_ANALYSIS = true;
      file_Temp[nfile] = strvec2[4];
      file_Time[nfile] = strvec2[5];
      temp = stod(file_Temp[nfile]);
      int st=0;
      if(file_Time[nfile].find("m")!=-1){ time_unit ="min"; st = file_Time[nfile].find("m");}
      if(file_Time[nfile].find("h")!=-1){ time_unit = "hour";st = file_Time[nfile].find("h");}
      if(file_Time[nfile].find("d")!=-1){ time_unit = "day";st = file_Time[nfile].find("d");}
      TIME[nfile] = stod(file_Time[nfile].substr(0,st));

      if(TIME_MAX < TIME[nfile] )
	TIME_MAX = TIME[nfile];

    }
    
    file_SU[nfile] = strvec3[0];

    nfile++;
  }
}


//======================================================= //
/*
void IVana::GetIV(string ifname, int ifile){
  string buf;
  ifstream ifp(Form("%s",ifname.c_str()),ios::in);
  niv[ifile] =0;
  vector<string>strvec;

  while(1){
    getline(ifp,buf);
    if(ifp.eof() || buf.size()==0)break;
    strvec = split(buf,',');
    V[ifile][niv[ifile]] = stof(strvec.at(0));
    if(file_SU[ifile]=="AIST"){
      J[ifile][niv[ifile]] = stof(strvec.at(1));
      I[ifile][niv[ifile]] = J[ifile][niv[ifile]]*cell_area;
    }else{
      I[ifile][niv[ifile]] = stof(strvec.at(1));
      J[ifile][niv[ifile]] = I[ifile][niv[ifile]]/cell_area;
    }
    W[ifile][niv[ifile]] = I[ifile][niv[ifile]]*V[ifile][niv[ifile]];
   
    SetVal(ifile, niv[ifile]);
    niv[ifile]++;
  }
  
}
*/

// =================================================== //

double IVana::InitialVal(string par, int id, int ch){

  id =12;  // test
  if(par.find("Jsc")!=-1){ // Jsc Val
    if(id == 12 && ch == 1) return 34.871e-3; // A/cm2
    else if(id ==12 && ch ==2 ) return 34.858e-3; // A/cm2
    else return 0;
  }else if(par.find("Isc")!=-1){
    if(id == 12 && ch == 1) return 34.871e-3*cell_area; // A
    else if(id ==12 && ch ==2 ) return 34.858e-3*cell_area; // A
    else return 0;
  }else if(par.find("Voc")!=-1){
    if(id == 12 && ch == 1) return 0.74449; // V
    else if(id ==12 && ch ==2 ) return 0.74666; // V
    else return 0;
  }else if(par.find("FF")!=-1){
    if(id == 12 && ch == 1) return 0.73904; 
    else if(id ==12 && ch ==2 ) return 0.75403; 
    else return 0;
  }else if(par.find("eta")!=-1){
    if(id == 12 && ch == 1) return 19.1861; // [%]
    else if(id ==12 && ch ==2 ) return 19.6253; // [%]
    else return 0;    
  }else if(par.find("Rs")!=-1){
    if(id == 12 && ch == 1) return 0.95471/cell_area; // [Ome]
    else if(id ==12 && ch ==2 ) return 0.95068/cell_area; // [Ome]
    else return 0;    
  }else if(par.find("Rsh")!=-1){
    if(id == 12 && ch == 1) return 6.0396e3/cell_area; // [Ome]
    else if(id ==12 && ch ==2 ) return 1.0233e3/cell_area; // [Ome]
    else return 0;    
  }else return 0;
  
};

// ================================================== //
void IVana::GetIV(string ifname, int ifile){
  string buf;
  ifstream ifp(Form("%s",ifname.c_str()),ios::in);
  niv[ifile] =0;
  vector<string>strvec;

  while(1){
    getline(ifp,buf);
    if(ifp.eof() || buf.size()==0)break;
    strvec = split(buf,',');
    V[ifile][niv[ifile]] = stof(strvec.at(0));
    if(file_SU[ifile]=="AIST"){
      GetIV_AIST(strvec, ifile);
    }else{
      if(strvec.size()!=2) continue;
      GetIV_KEK(strvec, ifile);
    }

    W[ifile][niv[ifile]] = I[ifile][niv[ifile]]*V[ifile][niv[ifile]];    
    GetVal(ifile, niv[ifile]);
    SetVal(ifile, niv[ifile]);
    niv[ifile]++;

  }
  
}

// ======================================================= //
void IVana::GetIV_AIST(vector <string> strvec , int ifile){
  
  J[ifile][niv[ifile]] = - stof(strvec.at(1)) * 1.0e-3; // mA -> A
  I[ifile][niv[ifile]] = J[ifile][niv[ifile]]*cell_area;
  W[ifile][niv[ifile]] = I[ifile][niv[ifile]]*V[ifile][niv[ifile]];
  

}

// ======================================================= //
void IVana::GetIV_KEK(vector <string> strvec, int ifile){
      V[ifile][niv[ifile]] = stof(strvec.at(0));
      I[ifile][niv[ifile]] = -1. * stof(strvec.at(1));
      J[ifile][niv[ifile]] = stof(strvec.at(1))/cell_area;
      W[ifile][niv[ifile]] = I[ifile][niv[ifile]]*V[ifile][niv[ifile]];

      
}

// ============================================================== //

void IVana::SetVal(int ifile, int isamp){

  gIV[ifile] -> SetPoint(isamp,V[ifile][isamp], I[ifile][isamp]);
  gJV[ifile] -> SetPoint(isamp,V[ifile][isamp], J[ifile][isamp]);
  gIV_mA[ifile] -> SetPoint(isamp,V[ifile][isamp], -I[ifile][isamp]*1000.);
  gJV_mA[ifile] -> SetPoint(isamp,V[ifile][isamp], -J[ifile][isamp]*1000.);  
  gIV_abs[ifile] -> SetPoint(isamp,V[ifile][isamp], fabs(I[ifile][isamp]));
  gJV_abs[ifile] -> SetPoint(isamp,V[ifile][isamp], fabs(J[ifile][isamp]));  
  gWV[ifile] -> SetPoint(isamp,V[ifile][isamp], W[ifile][isamp]);

  
}

// ============================================================== //

void IVana::GetVal(int ifile, int isamp){

  double I1=0;
  double I0=0;
  double V1=0;
  double V0=0;
  double a=0;
  double b=0;



  
  // ==================================================== //
  if(V[ifile][0] >=0 && isamp==0){
    Isc[ifile] = I[ifile][0];
    Jsc[ifile] = Isc[ifile]/cell_area;
    I_1V[ifile] = I[ifile][0];
    J_1V[ifile] = I_1V[ifile]/cell_area;
    I_2V[ifile] = I[ifile][0];
    J_2V[ifile] = I_2V[ifile]/cell_area;
    I1 = I[ifile][isamp+1];
    I0 = I[ifile][isamp];
    V1 = V[ifile][isamp+1];
    V0 = V[ifile][isamp];
    a = (I1 - I0)/(V1 - V0);
    b = I1 - a*V1;
    Rsh[ifile] = fabs(1./a);
  }else if (V[ifile][0] >=-1 && isamp==0){
    I_1V[ifile] = I[ifile][0];
    J_1V[ifile] = I_1V[ifile]/cell_area;
    I_2V[ifile] = I[ifile][0];
    J_2V[ifile] = I_2V[ifile]/cell_area;
  }else if (V[ifile][0] >=-2 && isamp==0){
    I_2V[ifile] = I[ifile][0];
    J_2V[ifile] = I_2V[ifile]/cell_area;
  }
  
  if(isamp ==0) return;

  
  I1 = I[ifile][isamp];
  I0 = I[ifile][isamp-1];
  V1 = V[ifile][isamp];
  V0 = V[ifile][isamp-1];
  a = (I1 - I0)/(V1 - V0);
  b = I1 - a*V1;
  
  if(Pm[ifile] < W[ifile][isamp] ) Pm[ifile] =  W[ifile][isamp];

  if(isamp>0 && V0 < 0 && V1 >= 0 ){ /// Set Jsc
    Isc[ifile] = b;
    Jsc[ifile] = Isc[ifile]/cell_area;
    Rsh[ifile] = fabs(1./a);
  }else if(isamp>0 && V0 < -1 && V1 >= -1){ // Set Jsc at -1V
    I_1V[ifile] = a*-1.0+b;
    J_1V[ifile] = I_1V[ifile]/cell_area;    
  }else if(isamp>0 && V0 < -2 && V1 >= -2){ // Set Jsc at -2V
    I_2V[ifile] = a*-2.0+b;
    J_2V[ifile] = I_2V[ifile]/cell_area;    
  }
  
  if(isamp>0 && I0 > 0 && I1 < 0 ){
    Voc[ifile] = -b/a;
    Rs[ifile]  = fabs(1./a);
  }

  if(fabs(Voc[ifile] * Isc[ifile])>0)
    FF[ifile ] = Pm[ifile] /(Voc[ifile] * Isc[ifile]);
  else FF[ifile] = 0.0;
  eta[ifile] = Pm[ifile]/(Pin* cell_area) * 100.; // [%]
  
}

// ============================================================== //
void IVana::Initialization(){

  for(int i=0; i<nfile; i++){
    Pm[i] = 0.0;
    Voc[i] = 0.0;
    Isc[i] = 0.0;
    Jsc[i] = 0.0;
    FF[i] = 0.0;
    eta[i] = 0.0;
    J_1V[i] =0.0;
    J_2V[i] =0.0;
    I_1V[i] =0.0;
    I_2V[i] =0.0;    
  }
}

// ============================================================== //
void IVana::Loop(){

  Initialization();
  int i_AIST = 0 ;
  int i_KEK  = 0 ;
  int i_ID_CH[NID][NCH];

  
  for(int iid=0;iid<n_id;iid++)
      for(int ich=0;ich<n_ch;ich++)
	i_ID_CH[iid][ich] =0;

  for(int i=0; i<nfile; i++){
    GetIV(IVdata[i], i);
    gIsc ->SetPoint(i, i , Isc[i]);
    gJsc ->SetPoint(i, i , Jsc[i]);
    gFF ->SetPoint(i, i , FF[i]);
    geta ->SetPoint(i, i , eta[i]);
    gPm ->SetPoint(i, i , Pm[i]);
    gRsh ->SetPoint(i, i , Rsh[i]);
    gRs  ->SetPoint(i, i , Rs[i]);    

    
    string id_st = file_ID[i].substr(2);
    string ch_st = file_CH[i].substr(2);
    int id = stoi(id_st.c_str());
    int ch = stoi(ch_st.c_str());


    //    if(file_LIGHT[i]=="LIGHT" && CYRIC_ANALYSIS){ // set IV parameters in Light -IV measurement
    if(file_LIGHT[i]=="LIGHT" && CYRIC_ANALYSIS && file_SU[i]=="KEK"){ // set IV parameters in Light -IV measurement
      for(int iid=0;iid<n_id;iid++){
	if(id_num[iid]== id){
	  for(int ich=0;ich<n_ch;ich++){
	    if(ch_num[ich]== ch){
	      TEMP[iid][ich] = file_Temp[i];
	      gIsc_TIME[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],Isc[i]*A_to_mA);
	      gJsc_TIME[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],Jsc[i]*A_to_mA);

	      gIsc_TIME_H[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i]*60.,Isc[i]*A_to_mA);
	      gJsc_TIME_H[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i]*60.,Jsc[i]*A_to_mA);   
	      gI1V_TIME[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],I_1V[i]*A_to_mA);
	      gJ1V_TIME[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],J_1V[i]*A_to_mA);
	      gI2V_TIME[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],I_2V[i]*A_to_mA);
	      gJ2V_TIME[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],J_2V[i]*A_to_mA);
	      gFF_TIME[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],FF[i]);
	      geta_TIME[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],eta[i]);
	      gPm_TIME[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],Pm[i]);
	      gVoc_TIME[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],Voc[i]);

	      gIsc_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],Isc[i]/InitialVal("Isc",id,ch));
	      gJsc_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],Jsc[i]/InitialVal("Jsc",id,ch));
	      gIsc_TIME_H_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i]*60.,Isc[i]/InitialVal("Isc",id,ch));
	      gJsc_TIME_H_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i]*60.,Jsc[i]/InitialVal("Jsc",id,ch));	      
	      gI1V_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],I_1V[i]/InitialVal("Isc",id,ch));
	      gJ1V_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],J_1V[i]/InitialVal("Jsc",id,ch));
	      gI2V_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],I_2V[i]/InitialVal("Isc",id,ch));
	      gJ2V_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],J_2V[i]/InitialVal("Jsc",id,ch));
	      gFF_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],FF[i]/InitialVal("FF",id,ch));
	      geta_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],eta[i]/InitialVal("eta",id,ch));
	      gPm_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],Pm[i]/InitialVal("Pm",id,ch));
	      gVoc_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich],TIME[i],Voc[i]/InitialVal("Voc",id,ch));

	      cout<<"i "<<i<<" TIME "<<TIME[i]<<" TEMP "<<TEMP[i]<<" Jsc "<<Jsc[i]/InitialVal("Jsc",id,ch)<<endl;


	      // ======================================================= //
	      if(CYRIC_ANALYSIS){
		if(i_ID_CH[iid][ich] == 0){
		  gJsc_CYRIC_TIME_R[iid][ich] -> SetPoint(0,-10.0, 1.0);
		  gJsc_CYRIC_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich]+1,TIME[i],Jsc[i]/InitialVal("Jsc",id,ch));

		  gJ2V_CYRIC_TIME_R[iid][ich] -> SetPoint(0,-10.0, 1.0);
		  gJ2V_CYRIC_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich]+1,TIME[i],J_2V[i]/InitialVal("Jsc",id,ch));

		  geta_CYRIC_TIME_R[iid][ich] -> SetPoint(0,-10.0, 1.0);
		  geta_CYRIC_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich]+1,TIME[i],eta[i]/InitialVal("eta",id,ch));		  
		}else{
		  gJsc_CYRIC_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich]+1,TIME[i],Jsc[i]/InitialVal("Jsc",id,ch));
		  gJ2V_CYRIC_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich]+1,TIME[i],J_2V[i]/InitialVal("Jsc",id,ch));
		  geta_CYRIC_TIME_R[iid][ich] -> SetPoint(i_ID_CH[iid][ich]+1,TIME[i],eta[i]/InitialVal("eta",id,ch));		  
		}
	      }
	      // ======================================================= //

	      i_ID_CH[iid][ich]++;
	    }
	  }
	}
      }
    }

    
      
    
    if(file_SU[i]=="AIST"){
      gIsc_AIST ->SetPoint(i_AIST, i_AIST , Isc[i]);
      gJsc_AIST ->SetPoint(i_AIST, i_AIST , Jsc[i]);
      gFF_AIST ->SetPoint(i_AIST, i_AIST , FF[i]);
      geta_AIST ->SetPoint(i_AIST, i_AIST , eta[i]);
      gPm_AIST ->SetPoint(i_AIST, i_AIST , Pm[i]);
      gRsh_AIST ->SetPoint(i_AIST, i_AIST , Rsh[i]);
      gRs_AIST  ->SetPoint(i_AIST, i_AIST , Rs[i]);
      i_AIST ++;
    }

    if(file_SU[i]=="KEK"){
      gIsc_KEK ->SetPoint(i_KEK, i_KEK , Isc[i]);
      gJsc_KEK ->SetPoint(i_KEK, i_KEK , Jsc[i]);
      gFF_KEK ->SetPoint(i_KEK, i_KEK , FF[i]);
      geta_KEK ->SetPoint(i_KEK, i_KEK , eta[i]);
      gPm_KEK ->SetPoint(i_KEK, i_KEK , Pm[i]);
      gRsh_KEK ->SetPoint(i_KEK, i_KEK , Rsh[i]);
      gRs_KEK  ->SetPoint(i_KEK, i_KEK , Rs[i]);
      i_KEK ++;
    }


    
  }



  
  // ================= Diff ========================== //
  for(int i=0; i<nfile;i++){
    for(int j=i+1; j<nfile;j++){
      if(file_ID[j] == file_ID[i] && file_CH[j] == file_CH[i]){

	int iii=0;
	string file_diff_ID[100],file_diff_CH[100];
	//	int id_diff[100],ch_diff[100];
	for(int i=0; i<nfile;i++){
	  for(int j=i+1; j<nfile;j++){
	    if(file_ID[j] == file_ID[i] && file_CH[j] == file_CH[i]){
	      if(file_Time[i] == file_Time[j]){
		//		file_diff_ID[iii] = file_ID[j];
		//		file_diff_CH[iii] = file_CH[j];
		file_diff_ID[iii] = file_ID[j].substr(2);
		file_diff_CH[iii] = file_CH[j].substr(2);
				id_diff[iii] = stoi(file_diff_ID[iii]);
		ch_diff[iii] = stoi(file_diff_CH[iii]);

		if(file_LIGHT[i] == "LIGHT" && file_LIGHT[j] == "DARK"){
		  gJV_mA_Diff[iii] = GetIV_Diff(gJV_mA[i], gJV_mA[j]);
		  SetIVDiff(gJV_mA_Diff[iii]);
		  Jsc_Diff[i] = Jsc_tmp;
		  J1V_Diff[i] = J1V_tmp;
		  J2V_Diff[i] = J2V_tmp;
		  iii++;
		}else if(file_LIGHT[i] == "DARK" && file_LIGHT[j] == "LIGHT"){
		  gJV_mA_Diff[iii] = GetIV_Diff(gJV_mA[j], gJV_mA[i]);
		  //		  SetIVDiff(gJV_mA_Diff[iii]);
		  Jsc_Diff[i] = Jsc_tmp;
		  J1V_Diff[i] = J1V_tmp;
		  J2V_Diff[i] = J2V_tmp;
		  iii++;
		}
	      }
	    }
	  }
	}
      }
    }
  }



  int i_Diff_ID_CH[NID][NCH];  
  for(int iid=0;iid<n_id;iid++)
    for(int ich=0;ich<n_ch;ich++)
      i_Diff_ID_CH[iid][ich] =0;


  
  for(int i=0; i<nfile;i++){
    string id_st = file_ID[i].substr(2);
    string ch_st = file_CH[i].substr(2);
    int id = stoi(id_st.c_str());
    int ch = stoi(ch_st.c_str());
    if(file_LIGHT[i]=="LIGHT" && CYRIC_ANALYSIS && file_SU[i]=="KEK"){ // set IV parameters in Light -Dark IV measurement
      for(int iid=0;iid<n_id;iid++){
	if(id_num[iid]== id){
	  for(int ich=0;ich<n_ch;ich++){
	    if(ch_num[ich]== ch){
	      TEMP[iid][ich] = file_Temp[i];
	      gJsc_Diff_TIME_R[iid][ich] -> SetPoint(i_Diff_ID_CH[iid][ich],TIME[i],Jsc_Diff[i]/InitialVal("Jsc",id,ch)/A_to_mA);
	      gJ1V_Diff_TIME_R[iid][ich] -> SetPoint(i_Diff_ID_CH[iid][ich],TIME[i],J1V_Diff[i]/InitialVal("Jsc",id,ch)/A_to_mA);
	      gJ2V_Diff_TIME_R[iid][ich] -> SetPoint(i_Diff_ID_CH[iid][ich],TIME[i],J2V_Diff[i]/InitialVal("Jsc",id,ch)/A_to_mA);
	      i_Diff_ID_CH[iid][ich]++;
	    }
	  }
	}
      }
      diff_file[i] = N_DIFF;
      N_DIFF++;
    }
  }
  

  // ================================================= //

  

  if(i_KEK == i_AIST){
    for(int i=0 ; i<i_KEK;i++){

      double eta_KEK = geta_KEK ->GetPointY(i);
      double eta_AIST = geta_AIST ->GetPointY(i);
      double eta_rel  = (eta_AIST - eta_KEK)/eta_AIST*100.;
      geta_Rel ->SetPoint(i,i,eta_rel);

      double Jsc_KEK = gJsc_KEK ->GetPointY(i);
      double Jsc_AIST = gJsc_AIST ->GetPointY(i);
      double Jsc_rel  = (Jsc_AIST - Jsc_KEK)/Jsc_AIST*100.;
      gJsc_Rel ->SetPoint(i,i,Jsc_rel);
      
      
    }

  }
  
}


// ============================================================== //

TGraphErrors* IVana::GetIV_Diff(TGraphErrors* gLIGHT, TGraphErrors* gDARK){
  
  int npoint_light = gLIGHT ->GetN();
  int npoint_dark  = gDARK  ->GetN();
  TGraphErrors* gDiff = new TGraphErrors();
  
  if(npoint_light != npoint_dark )
    return gDiff ;
  for(int i=0; i<npoint_light ; i++){
    double xL,xD,yL,yD;
    gLIGHT -> GetPoint(i,xL,yL);
    gDARK  -> GetPoint(i,xD,yD);
    gDiff -> SetPoint(i,xD,yL-yD);
  }

  return gDiff ;
  
}

// ============================================================== //
void IVana::SetIVDiff(TGraphErrors* gDiff){

  int Npoint = gDiff->GetN();
  double x,y;
  double v0 = 1000;
  double v1 = 1000;
  double v2 = 1000;
  double y0,y1,y2;
  for(int i=0; i<Npoint; i++){
    gDiff->GetPoint(i,x,y);
    if(fabs(x -   0 )  < fabs(v0 -    0)){
      v0 = x;
      y0 = y;
    }
    if(fabs(x - (-1) ) < fabs(v1 - (-1))){
      v1 = x;
      y1 = y;
    }
    if(fabs(x - (-2) ) < fabs(v2 - (-2))){
      v2 = x;
      y2 = y;
    }
  }

  
  Jsc_tmp = y0;
  J1V_tmp = y1;
  J2V_tmp = y2;

  
}
// ============================================================== //


void IVana::SetGraph(){

  for(int i=0; i<nfile; i++){

    gIV[i] = new TGraphErrors();
    gIV[i]->SetTitle("IV-curve ; Voltage [V] ; Current [A]");
    gIV[i]->SetName(Form("gIV_%d",i));
    gIV[i] ->SetMarkerStyle(20);
    gIV[i] ->SetMarkerColor(kRainBow);
    gIV[i] ->SetLineColor(kRainBow);

    gIV_mA[i] = new TGraphErrors();
    gIV_mA[i]->SetTitle("IV-curve ; Voltage [V] ; Current [mA]");
    gIV_mA[i]->SetName(Form("gIV_mA_%d",i));
    gIV_mA[i] ->SetMarkerStyle(20);
    gIV_mA[i] ->SetMarkerColor(kRainBow);
    gIV_mA[i] ->SetLineColor(kRainBow);
    
    
    gWV[i] = new TGraphErrors();
    gWV[i]->SetTitle("WV-curve ; Voltage [V] ; W [W]");
    gWV[i]->SetName(Form("gWV_%d",i));
    gWV[i] ->SetMarkerStyle(20);
    gWV[i] ->SetMarkerColor(kRainBow);
    gWV[i] ->SetLineColor(kRainBow);    

    gJV[i] = new TGraphErrors();
    gJV[i]->SetTitle("JV-curve ; Voltage [V] ; Current [A/cm2]");
    gJV[i]->SetName(Form("gJV_%d",i));
    gJV[i] ->SetMarkerStyle(20);
    gJV[i] ->SetMarkerColor(kRainBow);
    gJV[i] ->SetLineColor(kRainBow);    

    gIV_abs[i] = new TGraphErrors();
    gIV_abs[i]->SetTitle("IV-curve ; Voltage [V] ; Current [A]");
    gIV_abs[i]->SetName(Form("gIV_%d",i));
    gIV_abs[i] ->SetMarkerStyle(20);
    gIV_abs[i] ->SetMarkerColor(kRainBow);
    gIV_abs[i] ->SetLineColor(kRainBow);

    gJV_abs[i] = new TGraphErrors();
    gJV_abs[i]->SetTitle("JV-curve ; Voltage [V] ; Current [A/cm2]");
    gJV_abs[i]->SetName(Form("gJV_%d",i));
    gJV_abs[i] ->SetMarkerStyle(20);
    gJV_abs[i] ->SetMarkerColor(kRainBow);
    gJV_abs[i] ->SetLineColor(kRainBow);    


    gJV_mA[i] = new TGraphErrors();
    gJV_mA[i]->SetTitle("JV-curve ; Voltage [V] ; Current [mA/cm2]");
    gJV_mA[i]->SetName(Form("gJV_mA_%d",i));
    gJV_mA[i] ->SetMarkerStyle(20);
    gJV_mA[i] ->SetMarkerColor(kRainBow);
    gJV_mA[i] ->SetLineColor(kRainBow);    

    
    gIV_Diff[i] = new TGraphErrors();
    gIV_Diff[i]->SetTitle("IV-curve ; Voltage [V] ; Current Diff");
    gIV_Diff[i]->SetName(Form("gIV_Diff_%d",i));
    gIV_Diff[i] ->SetMarkerStyle(20);
    gIV_Diff[i] ->SetMarkerColor(kRainBow);
    gIV_Diff[i] ->SetLineColor(kRainBow);

    gJV_Diff[i] = new TGraphErrors();
    gJV_Diff[i]->SetTitle("JV-curve ; Voltage [V] ; Current Diff");
    gJV_Diff[i]->SetName(Form("gJV_Diff_%d",i));
    gJV_Diff[i] ->SetMarkerStyle(20);
    gJV_Diff[i] ->SetMarkerColor(kRainBow);
    gJV_Diff[i] ->SetLineColor(kRainBow);


    //    gJV_mA_Diff[i] = new TGraphErrors();
    //    gJV_mA_Diff[i]->SetTitle("JV-curve (JLighgt-JDark); Voltage [V] ; Current [mA]");
    //    gJV_mA_Diff[i]->SetName(Form("gJV_mA_Diff_%d",i));
    //    gJV_mA_Diff[i] ->SetMarkerStyle(20);
    //    gJV_mA_Diff[i] ->SetMarkerColor(kRainBow);
    //    gJV_mA_Diff[i] ->SetLineColor(kRainBow); 

    gWV_Diff[i] = new TGraphErrors();
    gWV_Diff[i]->SetTitle("WV-curve ; Voltage [V] ; Current Diff [A]");
    gWV_Diff[i]->SetName(Form("gWV_Diff_%d",i));
    gWV_Diff[i] ->SetMarkerStyle(20);
    gWV_Diff[i] ->SetMarkerColor(kRainBow);
    gWV_Diff[i] ->SetLineColor(kRainBow);    

  }


  // ================== Annealing Graph ===================== //
  for(int iid = 0; iid<n_id;iid++){
    for(int ich =0; ich <n_ch; ich++){
      gIsc_TIME[iid][ich] = new TGraphErrors();
      gIsc_TIME[iid][ich]->SetName(Form("gIsc_TIME_%d_%d",iid,ich));
      gIsc_TIME[iid][ich] -> SetTitle(Form("Isc Annealing Time Dependence at %3.f degree; TIME [%s]; Isc [mA]",temp,time_unit.c_str()));
      gIsc_TIME[iid][ich] ->SetMarkerStyle(20);
      gIsc_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gIsc_TIME[iid][ich] ->SetLineColor(kRainBow);

      gJsc_TIME[iid][ich] = new TGraphErrors();
      gJsc_TIME[iid][ich]->SetName(Form("gJsc_TIME_%d_%d",iid,ich));
      gJsc_TIME[iid][ich] -> SetTitle(Form("Jsc Annealing Time Dependence at %3.f degree; TIME [%s]; Jsc [mA/cm2]",temp,time_unit.c_str()));
      gJsc_TIME[iid][ich] ->SetMarkerStyle(20);
      gJsc_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gJsc_TIME[iid][ich] ->SetLineColor(kRainBow);

      gJsc_Diff_TIME[iid][ich] = new TGraphErrors();
      gJsc_Diff_TIME[iid][ich]->SetName(Form("gJsc_Diff_TIME_%d_%d",iid,ich));
      gJsc_Diff_TIME[iid][ich] -> SetTitle(Form("Jsc (Light-Dark) Annealing Time Dependence at %3.f degree; TIME [%s]; Jsc [mA/cm2]",temp,time_unit.c_str()));
      gJsc_Diff_TIME[iid][ich] ->SetMarkerStyle(20);
      gJsc_Diff_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gJsc_Diff_TIME[iid][ich] ->SetLineColor(kRainBow);

      gIsc_TIME_H[iid][ich] = new TGraphErrors();
      gIsc_TIME_H[iid][ich]->SetName(Form("gIsc_TIME_H_%d_%d",iid,ich));
      gIsc_TIME_H[iid][ich] -> SetTitle(Form("Isc Annealing Time Dependence at %3.f degree; TIME [h]; Isc [mA]",temp));
      gIsc_TIME_H[iid][ich] ->SetMarkerStyle(20);
      gIsc_TIME_H[iid][ich] ->SetMarkerColor(kRainBow);
      gIsc_TIME_H[iid][ich] ->SetLineColor(kRainBow);

      gJsc_TIME_H[iid][ich] = new TGraphErrors();
      gJsc_TIME_H[iid][ich]->SetName(Form("gJsc_TIME_H_%d_%d",iid,ich));
      gJsc_TIME_H[iid][ich] -> SetTitle(Form("Jsc Annealing Time Dependence at %3.f degree; TIME [h]; Jsc [mA/cm2]",temp));
      gJsc_TIME_H[iid][ich] ->SetMarkerStyle(20);
      gJsc_TIME_H[iid][ich] ->SetMarkerColor(kRainBow);
      gJsc_TIME_H[iid][ich] ->SetLineColor(kRainBow);
      

      gI1V_TIME[iid][ich] = new TGraphErrors();
      gI1V_TIME[iid][ich]->SetName(Form("gI1V_TIME_%d_%d",iid,ich));
      gI1V_TIME[iid][ich] -> SetTitle(Form("Isc Annealing Time Dependence at %3.f degree; TIME [%s]; Isc at V=-1 [mA]",temp,time_unit.c_str()));
      gI1V_TIME[iid][ich] ->SetMarkerStyle(20);
      gI1V_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gI1V_TIME[iid][ich] ->SetLineColor(kRainBow);
      
      gJ1V_TIME[iid][ich] = new TGraphErrors();
      gJ1V_TIME[iid][ich]->SetName(Form("gJ1V_TIME_%d_%d",iid,ich));
      gJ1V_TIME[iid][ich] -> SetTitle(Form("J1V Annealing Time Dependence at %3.f degree; TIME [%s]; Jsc at V=-1V [mA/cm2]",temp,time_unit.c_str()));
      gJ1V_TIME[iid][ich] ->SetMarkerStyle(20);
      gJ1V_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gJ1V_TIME[iid][ich] ->SetLineColor(kRainBow);


      gJ1V_Diff_TIME[iid][ich] = new TGraphErrors();
      gJ1V_Diff_TIME[iid][ich]->SetName(Form("gJ1V_Diff_TIME_%d_%d",iid,ich));
      gJ1V_Diff_TIME[iid][ich] -> SetTitle(Form("J1V (LIGHT-DARK) Annealing Time Dependence at %3.f degree; TIME [%s]; Jsc at V=-1V [mA/cm2]",temp,time_unit.c_str()));
      gJ1V_Diff_TIME[iid][ich] ->SetMarkerStyle(20);
      gJ1V_Diff_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gJ1V_Diff_TIME[iid][ich] ->SetLineColor(kRainBow);

      
      gI2V_TIME[iid][ich] = new TGraphErrors();
      gI2V_TIME[iid][ich]->SetName(Form("gI2V_TIME_%d_%d",iid,ich));
      gI2V_TIME[iid][ich] -> SetTitle(Form("Isc Annealing Time Dependence at %3.f degree; TIME [%s]; Isc at V=-1 [mA]",temp,time_unit.c_str()));
      gI2V_TIME[iid][ich] ->SetMarkerStyle(20);
      gI2V_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gI2V_TIME[iid][ich] ->SetLineColor(kRainBow);
      
      gJ2V_TIME[iid][ich] = new TGraphErrors();
      gJ2V_TIME[iid][ich]->SetName(Form("gJ2V_TIME_%d_%d",iid,ich));
      gJ2V_TIME[iid][ich] -> SetTitle(Form("J2V Annealing Time Dependence at %3.f degree; TIME [%s]; Jsc at V=-2V [mA/cm2]",temp,time_unit.c_str()));
      gJ2V_TIME[iid][ich] ->SetMarkerStyle(20);
      gJ2V_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gJ2V_TIME[iid][ich] ->SetLineColor(kRainBow);


      gJ2V_Diff_TIME[iid][ich] = new TGraphErrors();
      gJ2V_Diff_TIME[iid][ich]->SetName(Form("gJ2V_Diff_TIME_%d_%d",iid,ich));
      gJ2V_Diff_TIME[iid][ich] -> SetTitle(Form("J2V(LIGHT-DARK) Annealing Time Dependence at %3.f degree; TIME [%s]; Jsc at V=-2V [mA/cm2]",temp,time_unit.c_str()));
      gJ2V_Diff_TIME[iid][ich] ->SetMarkerStyle(20);
      gJ2V_Diff_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gJ2V_Diff_TIME[iid][ich] ->SetLineColor(kRainBow);
      
      gFF_TIME[iid][ich] = new TGraphErrors();
      gFF_TIME[iid][ich]->SetName(Form("gFF_TIME_%d_%d",iid,ich));
      gFF_TIME[iid][ich] -> SetTitle(Form("FF Annealing Time Dependence at %3.f degree; TIME [%s]; FF ",temp,time_unit.c_str()));
      gFF_TIME[iid][ich] ->SetMarkerStyle(20);
      gFF_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gFF_TIME[iid][ich] ->SetLineColor(kRainBow);

      geta_TIME[iid][ich] = new TGraphErrors();
      geta_TIME[iid][ich]->SetName(Form("geta_TIME_%d_%d",iid,ich));
      geta_TIME[iid][ich] -> SetTitle(Form("eta Annealing Time Dependence at %3.f degree; TIME [ %s ]; eta [%%]",temp,time_unit.c_str()));
      geta_TIME[iid][ich] ->SetMarkerStyle(20);
      geta_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      geta_TIME[iid][ich] ->SetLineColor(kRainBow);

      gPm_TIME[iid][ich] = new TGraphErrors();
      gPm_TIME[iid][ich]->SetName(Form("gPm_TIME_%d_%d",iid,ich));
      gPm_TIME[iid][ich] -> SetTitle(Form("Pm Annealing Time Dependence at %3.f degree; TIME [ %s ]; Pm [W]",temp,time_unit.c_str()));
      gPm_TIME[iid][ich] ->SetMarkerStyle(20);
      gPm_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gPm_TIME[iid][ich] ->SetLineColor(kRainBow);

      gVoc_TIME[iid][ich] = new TGraphErrors();
      gVoc_TIME[iid][ich]->SetName(Form("gVoc_TIME_%d_%d",iid,ich));
      gVoc_TIME[iid][ich] -> SetTitle(Form("Voc Annealing Time Dependence at %3.f degree; TIME [ %s ]; Voc [W]",temp,time_unit.c_str()));
      gVoc_TIME[iid][ich] ->SetMarkerStyle(20);
      gVoc_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gVoc_TIME[iid][ich] ->SetLineColor(kRainBow);

      
      gRsh_TIME[iid][ich] = new TGraphErrors();
      gRsh_TIME[iid][ich]->SetName(Form("gRsh_TIME_%d_%d",iid,ich));
      gRsh_TIME[iid][ich] -> SetTitle(Form("Rsh Annealing Time Dependence at %3.f degree; TIME [ %s ]; Rsh [#Omega]",temp,time_unit.c_str()));
      gRsh_TIME[iid][ich] ->SetMarkerStyle(20);
      gRsh_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gRsh_TIME[iid][ich] ->SetLineColor(kRainBow);

      gRs_TIME[iid][ich] = new TGraphErrors();
      gRs_TIME[iid][ich]->SetName(Form("gRs_TIME_%d_%d",iid,ich));
      gRs_TIME[iid][ich] -> SetTitle(Form("Rs Annealing Time Dependence at %3.f degree; TIME [%s]; Rs [#Omega]",temp,time_unit.c_str()));
      gRs_TIME[iid][ich] ->SetMarkerStyle(20);
      gRs_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gRs_TIME[iid][ich] ->SetLineColor(kRainBow);


      gIsc_TIME_R[iid][ich] = new TGraphErrors();
      gIsc_TIME_R[iid][ich]->SetName(Form("gIsc_TIME_R_%d_%d",iid,ich));
      gIsc_TIME_R[iid][ich] -> SetTitle(Form("Isc Annealing Time Dependence at %3.f degree; TIME [%s]; Isc Normalized Value",temp,time_unit.c_str()));
      gIsc_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gIsc_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gIsc_TIME_R[iid][ich] ->SetLineColor(kRainBow);

      gJsc_TIME_R[iid][ich] = new TGraphErrors();
      gJsc_TIME_R[iid][ich]->SetName(Form("gJsc_TIME_R_%d_%d",iid,ich));
      gJsc_TIME_R[iid][ich] -> SetTitle(Form("Jsc Annealing Time Dependence at %3.f degree; TIME [%s]; Jsc Normalized Value",temp,time_unit.c_str()));
      gJsc_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gJsc_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gJsc_TIME_R[iid][ich] ->SetLineColor(kRainBow);


      gJsc_Diff_TIME_R[iid][ich] = new TGraphErrors();
      gJsc_Diff_TIME_R[iid][ich]->SetName(Form("gJsc_Diff_TIME_R_%d_%d",iid,ich));
      gJsc_Diff_TIME_R[iid][ich] -> SetTitle(Form("Jsc(LIGHT-DARK) Annealing Time Dependence at %3.f degree; TIME [%s]; Jsc Normalized Value",temp,time_unit.c_str()));
      gJsc_Diff_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gJsc_Diff_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gJsc_Diff_TIME_R[iid][ich] ->SetLineColor(kRainBow);
      

      gJsc_CYRIC_TIME_R[iid][ich] = new TGraphErrors();
      gJsc_CYRIC_TIME_R[iid][ich]->SetName(Form("gJsc_CYRIC_TIME_R_%d_%d",iid,ich));
      gJsc_CYRIC_TIME_R[iid][ich] -> SetTitle(Form("Jsc Annealing Time Dependence at %3.f degree; TIME [%s]; Jsc Normalized Value",temp,time_unit.c_str()));
      gJsc_CYRIC_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gJsc_CYRIC_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gJsc_CYRIC_TIME_R[iid][ich] ->SetLineColor(kRainBow);

      
      gIsc_TIME_H_R[iid][ich] = new TGraphErrors();
      gIsc_TIME_H_R[iid][ich]->SetName(Form("gIsc_TIME_H_R_%d_%d",iid,ich));
      gIsc_TIME_H_R[iid][ich] -> SetTitle(Form("Isc Annealing Time Dependence at %3.f degree; TIME [h]; Isc Normalized Value",temp));
      gIsc_TIME_H_R[iid][ich] ->SetMarkerStyle(20);
      gIsc_TIME_H_R[iid][ich] ->SetMarkerColor(kRainBow);
      gIsc_TIME_H_R[iid][ich] ->SetLineColor(kRainBow);

      gJsc_TIME_H_R[iid][ich] = new TGraphErrors();
      gJsc_TIME_H_R[iid][ich]->SetName(Form("gJsc_TIME_H_R_%d_%d",iid,ich));
      gJsc_TIME_H_R[iid][ich] -> SetTitle(Form("Jsc Annealing Time Dependence at %3.f degree; TIME_H [h]; Jsc Normalized Value",temp));
      gJsc_TIME_H_R[iid][ich] ->SetMarkerStyle(20);
      gJsc_TIME_H_R[iid][ich] ->SetMarkerColor(kRainBow);
      gJsc_TIME_H_R[iid][ich] ->SetLineColor(kRainBow);

     
      gI1V_TIME_R[iid][ich] = new TGraphErrors();
      gI1V_TIME_R[iid][ich]->SetName(Form("gI1V_TIME_R_%d_%d",iid,ich));
      gI1V_TIME_R[iid][ich] -> SetTitle(Form("Isc Annealing Time Dependence at %3.f degree; TIME_R [%s]; Isc (at -1V) Nomalized Value",temp,time_unit.c_str()));
      gI1V_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gI1V_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gI1V_TIME_R[iid][ich] ->SetLineColor(kRainBow);
      
      gJ1V_TIME_R[iid][ich] = new TGraphErrors();
      gJ1V_TIME_R[iid][ich]->SetName(Form("gJ1V_TIME_R_%d_%d",iid,ich));
      gJ1V_TIME_R[iid][ich] -> SetTitle(Form("J1V Annealing Time Dependence at %3.f degree; TIME_R [%s]; Jsc (at V=-1) Nomalized Value",temp,time_unit.c_str()));
      gJ1V_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gJ1V_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gJ1V_TIME_R[iid][ich] ->SetLineColor(kRainBow);


      gJ1V_Diff_TIME_R[iid][ich] = new TGraphErrors();
      gJ1V_Diff_TIME_R[iid][ich]->SetName(Form("gJ1V_Diff_TIME_R_%d_%d",iid,ich));
      gJ1V_Diff_TIME_R[iid][ich] -> SetTitle(Form("J1V(LIGHT-DARK) Annealing Time Dependence at %3.f degree; TIME_R [%s]; Jsc (at V=-1) Nomalized Value",temp,time_unit.c_str()));
      gJ1V_Diff_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gJ1V_Diff_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gJ1V_Diff_TIME_R[iid][ich] ->SetLineColor(kRainBow);

      
      gI2V_TIME_R[iid][ich] = new TGraphErrors();
      gI2V_TIME_R[iid][ich]->SetName(Form("gI2V_TIME_R_%d_%d",iid,ich));
      gI2V_TIME_R[iid][ich] -> SetTitle(Form("Isc Annealing Time Dependence at %3.f degree; TIME_R [%s]; Isc (at -2V) Normalized Value",temp,time_unit.c_str()));
      gI2V_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gI2V_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gI2V_TIME_R[iid][ich] ->SetLineColor(kRainBow);
      
      gJ2V_TIME_R[iid][ich] = new TGraphErrors();
      gJ2V_TIME_R[iid][ich]->SetName(Form("gJ2V_TIME_R_%d_%d",iid,ich));
      gJ2V_TIME_R[iid][ich] -> SetTitle(Form("J2V Annealing Time Dependence at %3.f degree; TIME_R [%s]; Jsc at V=-2V Nomalized Value",temp,time_unit.c_str()));
      gJ2V_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gJ2V_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gJ2V_TIME_R[iid][ich] ->SetLineColor(kRainBow);


      gJ2V_Diff_TIME_R[iid][ich] = new TGraphErrors();
      gJ2V_Diff_TIME_R[iid][ich]->SetName(Form("gJ2V_Diff_TIME_R_%d_%d",iid,ich));
      gJ2V_Diff_TIME_R[iid][ich] -> SetTitle(Form("J2V(LIGHT-DARK) Annealing Time Dependence at %3.f degree; TIME_R [%s]; Jsc at V=-2V Nomalized Value",temp,time_unit.c_str()));
      gJ2V_Diff_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gJ2V_Diff_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gJ2V_Diff_TIME_R[iid][ich] ->SetLineColor(kRainBow);

     
      gJ2V_CYRIC_TIME_R[iid][ich] = new TGraphErrors();
      gJ2V_CYRIC_TIME_R[iid][ich]->SetName(Form("gJ2V_CYRIC_TIME_R_%d_%d",iid,ich));
      gJ2V_CYRIC_TIME_R[iid][ich] -> SetTitle(Form("J2V Annealing Time Dependence at %3.f degree; TIME_R [%s]; Jsc at V=-2V Nomalized Value",temp,time_unit.c_str()));
      gJ2V_CYRIC_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gJ2V_CYRIC_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gJ2V_CYRIC_TIME_R[iid][ich] ->SetLineColor(kRainBow);
      

      gI1V_TIME_H_R[iid][ich] = new TGraphErrors();
      gI1V_TIME_H_R[iid][ich]->SetName(Form("gI1V_TIME_H_R_%d_%d",iid,ich));
      gI1V_TIME_H_R[iid][ich] -> SetTitle(Form("Isc Annealing Time Dependence at %s degree; TIME [h]; Isc (at -1V) Nomalized Value",TEMP[iid][ich].c_str()));
      gI1V_TIME_H_R[iid][ich] ->SetMarkerStyle(20);
      gI1V_TIME_H_R[iid][ich] ->SetMarkerColor(kRainBow);
      gI1V_TIME_H_R[iid][ich] ->SetLineColor(kRainBow);
      
      gJ1V_TIME_H_R[iid][ich] = new TGraphErrors();
      gJ1V_TIME_H_R[iid][ich]->SetName(Form("gJ1V_TIME_H_R_%d_%d",iid,ich));
      gJ1V_TIME_H_R[iid][ich] -> SetTitle(Form("J1V Annealing Time Dependence at %s degree; TIME [h]; Jsc (at V=-1) Nomalized Value",TEMP[iid][ich].c_str()));
      gJ1V_TIME_H_R[iid][ich] ->SetMarkerStyle(20);
      gJ1V_TIME_H_R[iid][ich] ->SetMarkerColor(kRainBow);
      gJ1V_TIME_H_R[iid][ich] ->SetLineColor(kRainBow);

      gI2V_TIME_H_R[iid][ich] = new TGraphErrors();
      gI2V_TIME_H_R[iid][ich]->SetName(Form("gI2V_TIME_H_R_%d_%d",iid,ich));
      gI2V_TIME_H_R[iid][ich] -> SetTitle(Form("Isc Annealing Time Dependence at %s degree; TIME [h]; Isc (at -2V) Normalized Value",TEMP[iid][ich].c_str()));
      gI2V_TIME_H_R[iid][ich] ->SetMarkerStyle(20);
      gI2V_TIME_H_R[iid][ich] ->SetMarkerColor(kRainBow);
      gI2V_TIME_H_R[iid][ich] ->SetLineColor(kRainBow);
      
      gJ2V_TIME_H_R[iid][ich] = new TGraphErrors();
      gJ2V_TIME_H_R[iid][ich]->SetName(Form("gJ2V_TIME_H_R_%d_%d",iid,ich));
      gJ2V_TIME_H_R[iid][ich] -> SetTitle(Form("J2V Annealing Time Dependence at %s degree; TIME [h]; Jsc at V=-2V Nomalized Value",TEMP[iid][ich].c_str()));
      gJ2V_TIME_H_R[iid][ich] ->SetMarkerStyle(20);
      gJ2V_TIME_H_R[iid][ich] ->SetMarkerColor(kRainBow);
      gJ2V_TIME_H_R[iid][ich] ->SetLineColor(kRainBow);


      
      gFF_TIME_R[iid][ich] = new TGraphErrors();
      gFF_TIME_R[iid][ich]->SetName(Form("gFF_TIME_R_%d_%d",iid,ich));
      gFF_TIME_R[iid][ich] -> SetTitle(Form("FF Annealing Time Dependence at %s degree; TIME [%s]; FF Normalized Value",TEMP[iid][ich].c_str(),time_unit.c_str()));
      gFF_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gFF_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gFF_TIME_R[iid][ich] ->SetLineColor(kRainBow);

      geta_TIME_R[iid][ich] = new TGraphErrors();
      geta_TIME_R[iid][ich]->SetName(Form("geta_TIME_R_%d_%d",iid,ich));
      geta_TIME_R[iid][ich] -> SetTitle(Form("eta Annealing Time Dependence at %s degree; TIME [%s]; eta Normalized Value",TEMP[iid][ich].c_str(),time_unit.c_str()));
      geta_TIME_R[iid][ich] ->SetMarkerStyle(20);
      geta_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      geta_TIME_R[iid][ich] ->SetLineColor(kRainBow);


      geta_CYRIC_TIME_R[iid][ich] = new TGraphErrors();
      geta_CYRIC_TIME_R[iid][ich]->SetName(Form("geta_CYRIC_TIME_R_%d_%d",iid,ich));
      geta_CYRIC_TIME_R[iid][ich] -> SetTitle(Form("eta Annealing Time Dependence at %s degree; TIME [%s]; eta Normalized Value",TEMP[iid][ich].c_str(),time_unit.c_str()));
      geta_CYRIC_TIME_R[iid][ich] ->SetMarkerStyle(20);
      geta_CYRIC_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      geta_CYRIC_TIME_R[iid][ich] ->SetLineColor(kRainBow);

      
      gPm_TIME_R[iid][ich] = new TGraphErrors();
      gPm_TIME_R[iid][ich]->SetName(Form("gPm_TIME_R_%d_%d",iid,ich));
      gPm_TIME_R[iid][ich] -> SetTitle(Form("Pm Annealing Time Dependence at %s degree; TIME [%s]; Pm Normalized Value",TEMP[iid][ich].c_str(),time_unit.c_str()));
      gPm_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gPm_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gPm_TIME_R[iid][ich] ->SetLineColor(kRainBow);

      gVoc_TIME_R[iid][ich] = new TGraphErrors();
      gVoc_TIME_R[iid][ich]->SetName(Form("gVoc_TIME_R_%d_%d",iid,ich));
      gVoc_TIME_R[iid][ich] -> SetTitle(Form("Voc Annealing Time Dependence at %s degree; TIME [%s]; Voc Normalized Value",TEMP[iid][ich].c_str(),time_unit.c_str()));
      gVoc_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gVoc_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gVoc_TIME_R[iid][ich] ->SetLineColor(kRainBow);

      
      gRsh_TIME_R[iid][ich] = new TGraphErrors();
      gRsh_TIME_R[iid][ich]->SetName(Form("gRsh_TIME_R_%d_%d",iid,ich));
      gRsh_TIME_R[iid][ich] -> SetTitle(Form("Rsh Annealing Time Dependence at %s degree; TIME [%s]; Rsh Normalized Value",TEMP[iid][ich].c_str(),time_unit.c_str()));
      gRsh_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gRsh_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gRsh_TIME_R[iid][ich] ->SetLineColor(kRainBow);

      gRs_TIME_R[iid][ich] = new TGraphErrors();
      gRs_TIME_R[iid][ich]->SetName(Form("gRs_TIME_R_%d_%d",iid,ich));
      gRs_TIME_R[iid][ich] -> SetTitle(Form("Rs Annealing Time Dependence at %s degree; TIME [%s]; Rs Normalized Value",TEMP[iid][ich].c_str(),time_unit.c_str()));
      gRs_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gRs_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gRs_TIME_R[iid][ich] ->SetLineColor(kRainBow);
      
    }
  }
  // ======================================================== //


  gIsc = new TGraphErrors();
  gIsc ->SetName("gIsc");
  gIsc->SetTitle("Isc ; ID ;Isc [A]");
  gIsc ->SetMarkerColor(1);
  gIsc ->SetMarkerStyle(20);
  gJsc = new TGraphErrors();
  gJsc ->SetName("gJsc");
  gJsc->SetTitle("Jsc ; ID ;Jsc [A/cm2]");
  gJsc ->SetMarkerColor(1);
  gJsc ->SetMarkerStyle(20);
  gFF = new TGraphErrors();
  gFF ->SetName("gFF");
  gFF->SetTitle("FF ; ID ; FF");
  gFF ->SetMarkerColor(1);
  gFF ->SetMarkerStyle(20);
  geta = new TGraphErrors();
  geta ->SetName("geta");
  geta->SetTitle("eta ; ID ;eta [%%]");
  geta ->SetMarkerColor(1);
  geta ->SetMarkerStyle(20);
  gPm = new TGraphErrors();
  gPm ->SetName("gPm");
  gPm->SetTitle("Pm ; ID ;Pm [W]");
  gPm ->SetMarkerColor(1);
  gPm ->SetMarkerStyle(20);
  gRsh = new TGraphErrors();
  gRsh ->SetName("gRsh");
  gRsh->SetTitle("Rsh ; ID ; Rsh [#Omega]");
  gRsh ->SetMarkerColor(1);
  gRsh ->SetMarkerStyle(20);
  gRs = new TGraphErrors();
  gRs ->SetName("gRs");
  gRs->SetTitle("Rs ; ID ; Rs [#Omega]");
  gRs ->SetMarkerColor(1);
  gRs ->SetMarkerStyle(20);


  gIsc_AIST = new TGraphErrors();
  gIsc_AIST ->SetName("gIsc_AIST");
  gIsc_AIST->SetTitle("Isc_AIST ; ID ;Isc [A]");
  gIsc_AIST ->SetMarkerColor(2);
  gIsc_AIST ->SetMarkerStyle(20);
  gJsc_AIST = new TGraphErrors();
  gJsc_AIST ->SetName("gJsc_AIST");
  gJsc_AIST->SetTitle("Jsc_AIST ; ID ;Jsc [A/cm2]");
  gJsc_AIST ->SetMarkerColor(2);
  gJsc_AIST ->SetMarkerStyle(20);
  gFF_AIST = new TGraphErrors();
  gFF_AIST ->SetName("gFF_AIST");
  gFF_AIST->SetTitle("FF_AIST ; ID ; FF");
  gFF_AIST ->SetMarkerColor(2);
  gFF_AIST ->SetMarkerStyle(20);
  geta_AIST = new TGraphErrors();
  geta_AIST ->SetName("geta_AIST");
  geta_AIST->SetTitle("eta_AIST ; ID ;eta [%%]");
  geta_AIST ->SetMarkerColor(2);
  geta_AIST ->SetMarkerStyle(20);
  gPm_AIST = new TGraphErrors();
  gPm_AIST ->SetName("gPm_AIST");
  gPm_AIST->SetTitle("Pm_AIST ; ID ;Pm [W]");
  gPm_AIST ->SetMarkerColor(2);
  gPm_AIST ->SetMarkerStyle(20);
  gRsh_AIST = new TGraphErrors();
  gRsh_AIST ->SetName("gRsh_AIST");
  gRsh_AIST->SetTitle("Rsh ; ID ; Rsh [#Omega]");
  gRsh_AIST ->SetMarkerColor(2);
  gRsh_AIST ->SetMarkerStyle(20);
  gRs_AIST = new TGraphErrors();
  gRs_AIST ->SetName("gRs");
  gRs_AIST->SetTitle("Rs ; ID ; Rs [#Omega]");
  gRs_AIST ->SetMarkerColor(2);
  gRs_AIST ->SetMarkerStyle(20);

  gIsc_KEK = new TGraphErrors();
  gIsc_KEK ->SetName("gIsc_KEK");
  gIsc_KEK->SetTitle("Isc_KEK ; ID ;Isc [A]");
  gIsc_KEK ->SetMarkerColor(4);
  gIsc_KEK ->SetMarkerStyle(20);
  gJsc_KEK = new TGraphErrors();
  gJsc_KEK ->SetName("gJsc_KEK");
  gJsc_KEK->SetTitle("Jsc_KEK ; ID ;Jsc [A/cm2]");
  gJsc_KEK ->SetMarkerColor(4);
  gJsc_KEK ->SetMarkerStyle(20);
  gFF_KEK = new TGraphErrors();
  gFF_KEK ->SetName("gFF_KEK");
  gFF_KEK->SetTitle("FF_KEK ; ID ; FF");
  gFF_KEK ->SetMarkerColor(4);
  gFF_KEK ->SetMarkerStyle(20);
  geta_KEK = new TGraphErrors();
  geta_KEK ->SetName("geta_KEK");
  geta_KEK->SetTitle("eta_KEK ; ID ;eta [%%]");
  geta_KEK ->SetMarkerColor(4);
  geta_KEK ->SetMarkerStyle(20);
  gPm_KEK = new TGraphErrors();
  gPm_KEK ->SetName("gPm_KEK");
  gPm_KEK->SetTitle("Pm_KEK ; ID ;Pm [W]");
  gPm_KEK ->SetMarkerColor(4);
  gPm_KEK ->SetMarkerStyle(20);
  gRsh_KEK = new TGraphErrors();
  gRsh_KEK ->SetName("gRsh_KEK");
  gRsh_KEK->SetTitle("Rsh ; ID ; Rsh [#Omega]");
  gRsh_KEK ->SetMarkerColor(4);
  gRsh_KEK ->SetMarkerStyle(20);
  gRs_KEK = new TGraphErrors();
  gRs_KEK ->SetName("gRs");
  gRs_KEK->SetTitle("Rs ; ID ; Rs [#Omega]");
  gRs_KEK ->SetMarkerColor(4);
  gRs_KEK ->SetMarkerStyle(20);  


  gIsc_Rel = new TGraphErrors();
  gIsc_Rel ->SetName("gIsc_Rel");
  gIsc_Rel->SetTitle("Isc_Rel ; ID ;Isc [A]");
  gIsc_Rel ->SetMarkerColor(4);
  gIsc_Rel ->SetMarkerStyle(20);
  gJsc_Rel = new TGraphErrors();
  gJsc_Rel ->SetName("gJsc_Rel");
  gJsc_Rel->SetTitle("Jsc_Rel ; ID ;Relative Error [%%]");
  gJsc_Rel ->SetMarkerColor(4);
  gJsc_Rel ->SetMarkerStyle(20);
  gFF_Rel = new TGraphErrors();
  gFF_Rel ->SetName("gFF_Rel");
  gFF_Rel->SetTitle("FF_Rel ; ID ; FF");
  gFF_Rel ->SetMarkerColor(4);
  gFF_Rel ->SetMarkerStyle(20);
  geta_Rel = new TGraphErrors();
  geta_Rel ->SetName("geta_Rel");
  geta_Rel->SetTitle("eta_Rel ; ID ;eta [%%]");
  geta_Rel ->SetMarkerColor(4);
  geta_Rel ->SetMarkerStyle(20);
  gPm_Rel = new TGraphErrors();
  gPm_Rel ->SetName("gPm_Rel");
  gPm_Rel->SetTitle("Pm_Rel ; ID ;Pm [W]");
  gPm_Rel ->SetMarkerColor(4);
  gPm_Rel ->SetMarkerStyle(20);
  gRsh_Rel = new TGraphErrors();
  gRsh_Rel ->SetName("gRsh_Rel");
  gRsh_Rel->SetTitle("Rsh ; ID ; Rsh [#Omega]");
  gRsh_Rel ->SetMarkerColor(4);
  gRsh_Rel ->SetMarkerStyle(20);
  gRs_Rel = new TGraphErrors();
  gRs_Rel ->SetName("gRs");
  gRs_Rel->SetTitle("Rs ; ID ; Rs [#Omega]");
  gRs_Rel ->SetMarkerColor(4);
  gRs_Rel ->SetMarkerStyle(20);  


  
}


// ================================================================= //
void IVana::Draw_CYRIC(){


  
  int ncanvas_TIME=4;
  TGraphErrors* gTemp_TIME[ncanvas_TIME];
  for(int i=0; i<ncanvas_TIME; i++){
    c[i] = new TCanvas(Form("c%d",i),Form("c%d",i));
    leg[i] = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
    cTIME[i] = new TCanvas(Form("cTIME%d",i),Form("cTIME%d",i));
    leg_TIME[i] = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
    gTemp_TIME[i] = new TGraphErrors();
  }


  

    for(int iid =0; iid<n_id; iid++){
      for(int ich =0; ich<n_ch; ich++){
	if(iid ==0 && ich ==0){
	  cTIME[0]->cd();
	  gPad->SetGridy(1);
	  gTemp_TIME[0]->SetMarkerColor(0);
	  gTemp_TIME[0]->SetLineColor(0);
	  gTemp_TIME[0]->SetPoint(0,0,0.5);
	  gTemp_TIME[0]->SetPoint(1,TIME_MAX,1.0);
	  gTemp_TIME[0]->SetTitle("; Annealing time [min]; Relative ratio of J_{SC}");
	  gTemp_TIME[0]->GetXaxis()->SetTitleSize(0.06);
	  gTemp_TIME[0]->GetYaxis()->SetTitleSize(0.06);
	  gTemp_TIME[0]->GetXaxis()->SetLabelSize(0.04);
	  gTemp_TIME[0]->GetYaxis()->SetLabelSize(0.06);

	  gTemp_TIME[0]->Draw("AP");

	  cTIME[1]->cd();
	  gPad->SetGridy(1);
	  gTemp_TIME[1]->SetMarkerColor(0);
	  gTemp_TIME[1]->SetLineColor(0);
	  gTemp_TIME[1]->SetPoint(0,0,0.5);
	  gTemp_TIME[1]->SetPoint(1,TIME_MAX,1.2);
	  gTemp_TIME[1]->Draw("AP");
	  gTemp_TIME[1]->SetTitle("; TIME [m];Normarized Factor");

	  cTIME[2]->cd();
	  gPad->SetGridy(1);
	  gTemp_TIME[2]->SetMarkerColor(0);
	  gTemp_TIME[2]->SetLineColor(0);
	  gTemp_TIME[2]->SetPoint(0,0,0.0);
	  gTemp_TIME[2]->SetPoint(1,TIME_MAX,1.2);
	  gTemp_TIME[2]->SetTitle("Jsc Light -Dark; TIME [m];Normarized Factor");
	  gTemp_TIME[2]->Draw("AP");

	  cTIME[3]->cd();
	  gPad->SetGridy(1);
	  gTemp_TIME[3]->SetMarkerColor(0);
	  gTemp_TIME[3]->SetLineColor(0);
	  gTemp_TIME[3]->SetPoint(0,0,0.0);
	  gTemp_TIME[3]->SetPoint(1,TIME_MAX,1.2);
	  gTemp_TIME[3]->SetTitle("J2V Light -Dark; TIME [m];Normarized Factor");
	  gTemp_TIME[3]->Draw("AP");
	  
	  
	}
	  cTIME[0]->cd();
	  if(ch_num[ich] == 1){
	    //	      leg_TIME[0] ->AddEntry(gJsc_TIME_R[iid][ich],Form("Jsc ID%d-CH%d Temp. %s",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJsc_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	// ----------- For Picture IV_CYRIC_202301.list
	//	leg_TIME[0] ->AddEntry(gJsc_TIME_R[iid][ich],Form("iid =%d Jsc ID%d-CH%d Temp. %s",iid,id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	if(iid == 0  )
	  leg_TIME[0] ->AddEntry(gJsc_TIME_R[iid][ich]," 90#circC annealing","pe");
	if(iid == 1  )
	  leg_TIME[0] ->AddEntry(gJsc_TIME_R[iid][ich],"110#circC annealing","pe");
	if(iid == 2  )
	  leg_TIME[0] ->AddEntry(gJsc_TIME_R[iid][ich],"130#circC annealing","pe");

	
	  }
	  cTIME[1]->cd();
	  leg_TIME[1] ->AddEntry(gJ2V_TIME_R[iid][ich],Form("Jsc(-2V) ID%d-CH%d Temp. %s",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJ2V_TIME_R[iid][ich] -> Draw("PMC PLC PL");

	cTIME[2]->cd();
	  leg_TIME[2] ->AddEntry(gJsc_Diff_TIME_R[iid][ich],Form("Jsc(0V) ID%d-CH%d Temp. %s",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJsc_Diff_TIME_R[iid][ich] -> Draw("PMC PLC PL");

	cTIME[3]->cd();
	  leg_TIME[3] ->AddEntry(gJ2V_Diff_TIME_R[iid][ich],Form("Jsc(-2V) ID%d-CH%d Temp. %s",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJ2V_Diff_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	

      }
    }



    for(int i=0; i<ncanvas_TIME; i++){
      cTIME[i]->cd();
      leg_TIME[i]->Draw();
    }


    //      temp = stod(file_Temp[nfile]);

    // ====== Jsc-V curve ========  //
    bool IV_Draw = true;
    // get N Temp.
    int n_tmp =0;
    double TEMP_ID[nfile];
     for(int i =0; i<nfile; i++)
       TEMP_ID[i] = 0.0;
     
    for(int i =0; i<nfile; i++){
      double temp_i = stod(file_Temp[i]);
      for(int j =0; j<nfile; j++){
	if(TEMP_ID[j] != temp_i){
	  TEMP_ID[j] = temp_i;
	  n_tmp ++;
	  break;
	}else
	  break;
      }
    }	

    int ncanvas_IV = n_ch * n_id;
      TGraphErrors* gIV_tmp = new TGraphErrors();
      gIV_tmp ->SetPoint(0,-2.0,-100.0);
      gIV_tmp ->SetPoint(1, 1.0, 40.0);
      gIV_tmp ->SetMarkerColor(0);
      gIV_tmp ->SetTitle("J-V curve ; Voltage [V] ;J [mA/cm2] ");

      // id lavel
      int id_id[n_id];
      int iiii=1;
      id_id[0] = stoi(file_ID[0].substr(2));

      for(int i=0;i<nfile;i++){
	string id_st_i = file_ID[i].substr(2);
	for(int j=0; j<nfile;j++ ){
	  string id_st_j = file_ID[j].substr(2);
	  if(id_st_i != id_st_j ){
	    id_id[iiii] = stoi(id_st_j);
	    iiii++;
	    break;
	  }
	}
      }

      if(IV_Draw){
      TCanvas* cIV[ncanvas_IV];
	for(int i =0; i<ncanvas_IV; i++){
	  cIV[i] = new TCanvas(Form("cIV_%d",i),Form("cIV_%d",i));
	  cIV[i]->cd();
	  gIV_tmp->Draw("AP");
	}
	
	//      diff_file[i] = N_DIFF;
      	for(int i =0; i<N_DIFF; i++){
	if(file_LIGHT[diff_file[i]] == "LIGHT"){
	    int id = id_diff[i];
	    int ch = ch_diff[i];
	    int id_num =0;
	    if(ch == 1){
	      cIV[0]->cd();
	      gJV_mA_Diff[i] -> SetLineColor(kRainBow);
	      gJV_mA_Diff[i] -> SetMarkerColor(kRainBow);
	      gJV_mA_Diff[i] -> SetMarkerStyle(20);
	      gJV_mA_Diff[i] -> Draw("PMC PLC PL");
	    }else{
	      cIV[1]->cd();
	      gJV_mA_Diff[i] -> SetLineColor(kRainBow);
	      gJV_mA_Diff[i] -> SetMarkerColor(kRainBow);
	      gJV_mA_Diff[i] -> SetMarkerStyle(20);
	      gJV_mA_Diff[i] -> Draw("PMC PLC PL");
	    }
	  }
	}
      }


      
}


// ================================================================= //
void IVana::Draw(){

  if(!CYRIC_ANALYSIS){

  for(int i=0; i<ncanvas; i++){
    c[i] = new TCanvas(Form("c%d",i),Form("c%d",i));
    leg[i] = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
  }

  c[0]->cd();
  gPad->SetLogy(1);
  for(int i=0; i<nfile; i++){
    leg[0] ->AddEntry(gIV_abs[i],file_name[i].c_str(),"pe");
    if(i==0){ gIV_abs[i] ->Draw("PMC AP");
    }
    else  gIV_abs[i] ->Draw("PMC P");
  }
  leg[0]->Draw();
  
  c[1]->cd();
  for(int i=0; i<nfile; i++){
    leg[1] ->AddEntry(gWV[i],file_name[i].c_str(),"pe");
    if(i==0){ gWV[i] ->Draw("PMC APC");
    }
    else  gWV[i] ->Draw("PMC P");
  }
  leg[1]->Draw();


  c[2]->cd();
  for(int i=0; i<nfile; i++){
    leg[2] ->AddEntry(gJV_abs[i],file_name[i].c_str(),"pe");
    if(i==0){ gJV_abs[i] ->Draw("PMC APC");
    }
    else  gJV_abs[i] ->Draw("PMC P");
  }
  leg[2]->Draw();


  c[3] ->cd();
  gIsc->Draw("AP");
  gIsc_AIST->Draw("P");
  gIsc_KEK->Draw("P");

  c[4] ->cd();
  gJsc->Draw("AP");
  gJsc_AIST->Draw("P");
  gJsc_KEK->Draw("P");  

  c[5] ->cd();
  gFF->Draw("AP");
  gFF_AIST->Draw("P");
  gFF_KEK->Draw("P");  

  c[6] ->cd();
  geta->Draw("AP");
  geta_AIST->Draw("P");  
  geta_KEK->Draw("P");
  
  c[7] ->cd();
  gPm->Draw("AP");
  gPm_AIST->Draw("P");
  gPm_KEK->Draw("AP");  

  c[8] ->cd();
  geta_Rel->Draw("AP");

  c[9] ->cd();
  gJsc_Rel->Draw("AP");
  
}

  
  if(CYRIC_ANALYSIS){
    int ncanvas_CYRIC = 11 ;
    for(int i=0; i<ncanvas_CYRIC;i++){
      cCYRIC[i] = new TCanvas(Form("cCYRIC_%d",i),Form("cCYRIC_%d",i));
      leg_CYRIC[i] = new TLegend(0.1,0.7,0.5,0.9,"","NDC");
    }

    int ncanvas_pic = 14;
    for(int i=0; i<ncanvas_pic;i++){
      cpic[i] = new TCanvas(Form("cpic_%d",i),Form("cpic_%d",i));
      leg_pic[i] = new TLegend(0.1,0.7,0.5,0.9,"","NDC");
    }      
  
  
    for(int iid =0; iid<n_id; iid++){
      for(int ich =0; ich<n_ch; ich++){
	if(iid ==0 && ich ==0){
	cCYRIC[0]->cd();
	gPad->SetGridy(1);
	leg_CYRIC[0] ->AddEntry(gIsc_TIME[iid][ich],Form("ID%d-CH%d TEMP %s",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	gIsc_TIME[iid][ich] -> Draw("PMC PLC APL");
	gIsc_TIME[iid][ich] ->GetYaxis()->SetRangeUser(-0.01e3,0.2e3); // mA
	cCYRIC[1]->cd();

	leg_CYRIC[1] ->AddEntry(gJsc_TIME[iid][ich],Form("ID%d-CH%d TEMP %s",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	gJsc_TIME[iid][ich] -> Draw("PMC PLC APL");
	gJsc_TIME[iid][ich] ->GetYaxis()->SetRangeUser(-0.01e3,0.04e3); //mA

	cCYRIC[2]->cd();
	gPad->SetGridy(1);
	leg_CYRIC[2] ->AddEntry(geta_TIME[iid][ich],Form("ID%d-CH%d TEMP %s",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	geta_TIME[iid][ich] -> Draw("APL PLC PMC");
	geta_TIME[iid][ich] ->GetYaxis()->SetRangeUser(0,25);

	cCYRIC[3]->cd();
	gPad->SetGridy(1);
	leg_CYRIC[3] ->AddEntry(gFF_TIME[iid][ich],Form("ID%d-CH%d TEMP %s",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	gFF_TIME[iid][ich] -> Draw("PMC PLC APL");
       	gFF_TIME[iid][ich] ->GetYaxis()->SetRangeUser(0,1);

	cCYRIC[4]->cd();
	gPad->SetGridy(1);
	leg_CYRIC[4] ->AddEntry(gPm_TIME[iid][ich],Form("ID%d-CH%d TEMP %s",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	gPm_TIME[iid][ich] -> Draw("PMC PLC APL");
       	gPm_TIME[iid][ich] ->GetYaxis()->SetRangeUser(-0.00005,0.005);

	cCYRIC[5]->cd();
	gPad->SetGridy(1);
	leg_CYRIC[5] ->AddEntry(gVoc_TIME[iid][ich],Form("ID%d-CH%d TEMP %s",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	gVoc_TIME[iid][ich] -> Draw("PMC PLC APL");
       	gVoc_TIME[iid][ich] ->GetYaxis()->SetRangeUser(-0.00005,1);	

	cCYRIC[6]->cd();
	gPad->SetGridy(1);
	leg_CYRIC[6] ->AddEntry(geta_TIME_R[iid][ich],Form("ID%d-CH%d TEMP %s #eta",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	geta_TIME_R[iid][ich] -> Draw("PMC PLC APL");
       	geta_TIME_R[iid][ich] ->GetYaxis()->SetRangeUser(-0.00005,1);	
	leg_CYRIC[6] ->AddEntry(gJsc_TIME_R[iid][ich],Form("ID%d-CH%d TEMP %s Jsc ",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	gJsc_TIME_R[iid][ich] -> Draw("PMC PLC PL");


	cCYRIC[7]->cd(); // Isc V=0,  V=-1, V=-2
	gPad->SetGridy(1);
	leg_CYRIC[7] ->AddEntry(gJsc_TIME_H_R[iid][ich],Form("ID%d-CH%d TEMP %s (Jsc at 0V)",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	gJsc_TIME_H_R[iid][ich] -> Draw("PMC PLC APL");
	gJsc_TIME_H_R[iid][ich] ->GetYaxis()->SetRangeUser(0.0,0.04e3);
	leg_CYRIC[7] ->AddEntry(gJ1V_TIME_H_R[iid][ich],Form("ID%d-CH%d TEMP %s (Jsc at -1V)",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	gJ1V_TIME_H_R[iid][ich] -> Draw("PMC PLC PL");
	leg_CYRIC[7] ->AddEntry(gJ2V_TIME_H_R[iid][ich],Form("ID%d-CH%d TEMP %s (Jsc at -2V)",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	gJ2V_TIME_H_R[iid][ich] -> Draw("PMC PLC PL");
	
       
	cCYRIC[8]->cd(); // Jsc V=0, V=-1, V=-2
	gPad->SetGridy(1);
	leg_CYRIC[8] ->AddEntry(gJsc_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s (Jsc at 0V)",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJsc_TIME_R[iid][ich] -> Draw("PMC PLC APL");
	gJsc_TIME_R[iid][ich] ->GetYaxis()->SetRangeUser(0.0,1.3);
	leg_CYRIC[8] ->AddEntry(gJ1V_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s (Jsc at -1V)",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJ1V_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	leg_CYRIC[8] ->AddEntry(gJ2V_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s (Jsc at -2V)",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJ2V_TIME_R[iid][ich] -> Draw("PMC PLC PL");

	cCYRIC[9]->cd(); // Jsc V=0, V=-1, V=-2
	gPad->SetGridy(1);
	TGraphErrors* gTemp_CYRIC9=new TGraphErrors();
	gTemp_CYRIC9 ->SetPoint(0,0,5);
	gTemp_CYRIC9 ->SetPoint(1,150,1.1);
	gTemp_CYRIC9->Draw("AP");
	leg_CYRIC[9] ->AddEntry(gJsc_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s (at 0V)",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	gJsc_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	gJsc_TIME_R[iid][ich] ->GetYaxis()->SetRangeUser(0.0,1.3);



	cCYRIC[10]->cd(); // Jsc V=0,  V=-2
	gPad->SetGridy(1);

	TGraphErrors* gTemp = new TGraphErrors();
	gTemp ->SetPoint(0,0,0.5);
	gTemp ->SetPoint(1,2000,1.2);
	gTemp -> SetLineColor(0);
	gTemp -> SetMarkerColor(0);
	gTemp ->Draw("AP");
	leg_CYRIC[10] ->AddEntry(gJsc_CYRIC_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s Jsc (at 0V)",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");

	gJsc_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	gJsc_CYRIC_TIME_R[iid][ich] ->GetYaxis()->SetRangeUser(0.0,1.3);

	leg_CYRIC[10] ->AddEntry(gJ2V_CYRIC_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s Jsc (at -2V)",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	gJ2V_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC PL");


	// ================= Canvas For Slide Pictures =================== //
	TGraphErrors* gJsc_CYRIC_TIME_FRAME = new TGraphErrors();
	gJsc_CYRIC_TIME_FRAME ->SetPoint(0,-10,0.5);
	gJsc_CYRIC_TIME_FRAME ->SetPoint(1,100,1.0);
	gJsc_CYRIC_TIME_FRAME ->SetMarkerColor(0);
	gJsc_CYRIC_TIME_FRAME ->SetLineColor(0);
	
	cpic[0] ->cd(); //Jsc and eta at 0 V
	gJsc_CYRIC_TIME_FRAME->Draw("AP");
	leg_pic[0] ->AddEntry(gJsc_CYRIC_TIME_R[iid][ich],Form("ID%d-CH%d TEMP %s Jsc (at 0V)",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	gJsc_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC APL");
	gJsc_CYRIC_TIME_R[iid][ich] ->GetYaxis()->SetRangeUser(0.0,1.0);
	leg_pic[0] ->AddEntry(geta_CYRIC_TIME_R[iid][ich],Form("ID%d-CH%d TEMP %s eta (at 0V)",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	geta_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	
	cpic[1]->cd(); 
	gPad->SetGridy(1);
	gJsc_CYRIC_TIME_FRAME->Draw("AP");
	if(ch_num[ich]==1){
	  //	  leg_pic[1] ->AddEntry(gJ2V_CYRIC_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s Jsc (at -2V)",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	  //	  gJ2V_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC PL");

	  leg_pic[1] ->AddEntry(gJsc_CYRIC_TIME_R[iid][ich],Form(" Temp. %s Jsc (at 0V)",TEMP[iid][ich].c_str()),"pe");
	  gJsc_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	  //	  leg_pic[1] ->AddEntry(geta_CYRIC_TIME_R[iid][ich],Form("ID%d-CH%d TEMP %s eta (at 0V)",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	  //	geta_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	  cout<<"======================="<<endl;
	}
	}else{
	cCYRIC[0]->cd();
	leg_CYRIC[0] ->AddEntry(gIsc_TIME[iid][ich],Form("ID%d-CH%d Temp. %s",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gIsc_TIME[iid][ich] -> Draw("PMC PLC PL");
	cCYRIC[1]->cd();
	leg_CYRIC[1] ->AddEntry(gJsc_TIME[iid][ich],Form("ID%d-CH%d Temp. %s",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJsc_TIME[iid][ich] -> Draw("PMC PLC PL");
	cCYRIC[2]->cd();
	leg_CYRIC[2] ->AddEntry(geta_TIME[iid][ich],Form("ID%d-CH%d Temp. %s",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	geta_TIME[iid][ich] -> Draw("PL PLC PMC");
	cCYRIC[3]->cd();
	leg_CYRIC[3] ->AddEntry(gFF_TIME[iid][ich],Form("ID%d-CH%d Temp. %s",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gFF_TIME[iid][ich] -> Draw("PMC PLC PL");
	cCYRIC[4]->cd();
	leg_CYRIC[4] ->AddEntry(gFF_TIME[iid][ich],Form("ID%d-CH%d Temp. %s",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gPm_TIME[iid][ich] -> Draw("PMC PLC PL");
	cCYRIC[5]->cd();
	leg_CYRIC[5] ->AddEntry(gVoc_TIME[iid][ich],Form("ID%d-CH%d Temp. %s",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gVoc_TIME[iid][ich] -> Draw("PMC PLC PL");
	cCYRIC[6]->cd();
	leg_CYRIC[6] ->AddEntry(geta_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s #eta",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	geta_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	leg_CYRIC[6] ->AddEntry(gJsc_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s Jsc",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJsc_TIME_R[iid][ich] -> Draw("PMC PLC PL");

	cCYRIC[7]->cd(); // Isc V=0,  V=-1, V=-2
	leg_CYRIC[7] ->AddEntry(gIsc_TIME[iid][ich],Form("ID%d-CH%d Temp. %s (at 0V)",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gIsc_TIME[iid][ich] -> Draw("PMC PLC PL");
	leg_CYRIC[7] ->AddEntry(gI1V_TIME[iid][ich],Form("ID%d-CH%d Temp. %s (at -1V)",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gI1V_TIME[iid][ich] -> Draw("PMC PLC PL");
	leg_CYRIC[7] ->AddEntry(gI2V_TIME[iid][ich],Form("ID%d-CH%d Temp. %s (at -2V)",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gI2V_TIME[iid][ich] -> Draw("PMC PLC PL");


	cCYRIC[8]->cd(); // Jsc V=0, V=-1, V=-2
	leg_CYRIC[8] ->AddEntry(gJsc_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s (at 0V)",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJsc_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	leg_CYRIC[8] ->AddEntry(gJ1V_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s (at -1V)",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJ1V_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	leg_CYRIC[8] ->AddEntry(gJ2V_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s (at -2V)",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJ2V_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	cCYRIC[9]->cd(); // Jsc V=0, V=-1, V=-2
	leg_CYRIC[9] ->AddEntry(gJsc_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s (at 0V)",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJsc_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	gJsc_TIME_R[iid][ich] ->GetYaxis()->SetRangeUser(0.0,1.3);


	cCYRIC[10]->cd(); // Jsc V=0, V=-2
	leg_CYRIC[10] ->AddEntry(gJsc_CYRIC_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s Jsc (at 0V)",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJsc_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	leg_CYRIC[10] ->AddEntry(gJ2V_CYRIC_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %s Jsc (at -2V)",id_num[iid],ch_num[ich], TEMP[iid][ich].c_str()),"pe");
	gJ2V_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC PL");

	// ================= Canvas For Slide Pictures =================== //
	
	cpic[0] ->cd(); //Jsc and eta at 0 V
	leg_pic[0] ->AddEntry(gJsc_CYRIC_TIME_R[iid][ich],Form("ID%d-CH%d TEMP %s Jsc (at 0V)",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	gJsc_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	gJsc_CYRIC_TIME_R[iid][ich] ->GetYaxis()->SetRangeUser(0.0,1.0);
	leg_pic[0] ->AddEntry(geta_CYRIC_TIME_R[iid][ich],Form("ID%d-CH%d TEMP %s eta (at 0V)",id_num[iid],ch_num[ich],TEMP[iid][ich].c_str()),"pe");
	geta_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC PL");

      	cpic[1]->cd(); 
	gPad->SetGridy(1);
	if(ch_num[ich]==1){
	  //	  leg_pic[1] ->AddEntry(gJ2V_CYRIC_TIME_R[iid][ich],Form(" Temp. %s Jsc (at -2V)",TEMP[iid][ich].c_str()),"pe");
	  //	  gJ2V_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	  leg_pic[1] ->AddEntry(gJsc_CYRIC_TIME_R[iid][ich],Form(" Temp. %s Jsc (at 0V)",TEMP[iid][ich].c_str()),"pe");
	  gJsc_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC PL");
	  //	  leg_pic[1] ->AddEntry(geta_CYRIC_TIME_R[iid][ich],Form(" Temp. %s eta ",TEMP[iid][ich].c_str()),"pe");
	  //	  geta_CYRIC_TIME_R[iid][ich] -> Draw("PMC PLC PL");	  	  
	}
	}
	
      } // ch
    } // id

    cpic[0]->cd();
    leg_pic[0]->Draw();

    cpic[1]->cd();
    leg_pic[1]->Draw();    



    for(int i=0; i<ncanvas_CYRIC;i++){
      cCYRIC[i]->cd();
      leg_CYRIC[i] ->Draw();
    }

    TGraphErrors* gTemp[nfile];
    bool pass_flag[nfile];
    for(int i=0;i<nfile;i++)
      pass_flag[i] = false;
    int iv_ID=0;

    
    for(int i=0; i<nfile;i++){
      bool first_flag = true;
      if(pass_flag[i]) continue;
    for(int j=i+1; j<nfile;j++){
      if(file_ID[j] == file_ID[i] && file_CH[j] == file_CH[i] && file_LIGHT[i] == file_LIGHT[j]){
	pass_flag[j] = true;	    
	if(first_flag){
	  civ[iv_ID] = new TCanvas(Form("civ%d",iv_ID),Form("civ%d",iv_ID));
	  gTemp[iv_ID] = new TGraphErrors();
	  gTemp[iv_ID] ->SetTitle("J-V curve; Voltage [V]; J [mA/cm2]");
	  gTemp[iv_ID] ->GetXaxis()->SetTitleSize(0.06);
	  gTemp[iv_ID] ->GetYaxis()->SetTitleSize(0.06);
	  gTemp[iv_ID] ->GetXaxis()->SetLabelSize(0.06);
	  gTemp[iv_ID] ->GetYaxis()->SetLabelSize(0.06);

	  //	  gTemp[iv_ID]->SetPoint(0,-2.1,-20.0);
	  //	  gTemp[iv_ID]->SetPoint(1,1.1,50.0);
	  //  gTemp[iv_ID]->SetPoint(0,-0.5,0.0);
	    gTemp[iv_ID]->SetPoint(0,0.0,0.0);
	  gTemp[iv_ID]->SetPoint(1,0.8,35.0);
	  gTemp[iv_ID]->SetMarkerColor(0);
	  gTemp[iv_ID] ->Draw("AP");
	  leg_iv[iv_ID] = new TLegend(0.1,0.40,0.6,0.9,"","NDC");
	  civ[iv_ID] ->cd();
	  gPad->SetGridy(1);
	  gJV_mA[iv_ID] ->Draw("PMC PLC PL");

	  cpic[2+iv_ID]->cd();
	  gPad->SetGridy(1);
	  gJV_mA[iv_ID] ->Draw("PMC PLC APL");

	  if(file_SU[i]=="AIST"){
	    //	    leg_iv[iv_ID] ->AddEntry(gJV_mA[i],Form("%s-%s (%s-JV) Temp. at %3.0f Before Irradiation",file_ID[i].c_str(),file_CH[i].c_str(),file_LIGHT[i].c_str(),temp),"pe");
	    	    leg_iv[iv_ID] ->AddEntry(gJV_mA[i],"Before Irradiation","pe");
		    //		    leg_pic[iv_ID+2] ->AddEntry(gJ_mA[i],Form("Temp. at %3.0f Before Irradiation",temp),"pe");
		    leg_pic[iv_ID+2] ->AddEntry(gJV_mA[i],"Before Irradiation","pe");
	  }else{
	    //	    leg_iv[iv_ID] ->AddEntry(gJV_mA[i],Form("%s-%s (%s-JV) Temp. at %3.0f (%s)",file_ID[i].c_str(),file_CH[i].c_str(),file_LIGHT[i].c_str(),temp,file_Time[i].c_str()),"pe");
	    leg_pic[iv_ID+2]->AddEntry(gJV_mA[i],Form("Temp. at %3.0f (%s)",temp,file_Time[i].c_str()),"pe");

	    /// ------ For pic
	    leg_iv[iv_ID] ->AddEntry(gJV_mA[i],"7.5#times10^{15} MeV n_{nq}/cm^{2}","pe");
	  }
	  first_flag =false;
	  
	} // end first_flag
	
	if(file_SU[j]=="AIST"){
	  //	  leg_iv[iv_ID] ->AddEntry(gJV_mA[j],Form("%s-%s (%s-JV) Temp. at %3.0f Before Irradiation",file_ID[j].c_str(),file_CH[j].c_str(),file_LIGHT[j].c_str(),temp),"pe");
	  leg_pic[iv_ID+2] ->AddEntry(gJV_mA[j],Form("J-V curve Temp. at %3.0f Before Irradiation",temp),"pe");

	  // ------------- For picture
	  leg_iv[iv_ID] ->AddEntry(gJV_mA[j],"Before Irradiation","pe");
	}else{
	  //	leg_iv[iv_ID] ->AddEntry(gJV_mA[j],Form("%s-%s (%s-JV) Temp. at %3.0f (%s)",file_ID[j].c_str(),file_CH[j].c_str(),file_LIGHT[j].c_str(),temp,file_Time[j].c_str()),"pe");
	  //	  leg_iv[iv_ID] ->AddEntry(gJV_mA[j],Form("%s-%s (%s-JV) Temp. at %3.0f (%s)",file_ID[j].c_str(),file_CH[j].c_str(),file_LIGHT[j].c_str(),temp,file_Time[j].c_str()),"pe");
	  leg_pic[iv_ID+2] ->AddEntry(gJV_mA[j],Form("J-V curve Temp. at %3.0f (%s)",temp,file_Time[j].c_str()),"pe");
	  // --------- For picture
	  if(j==2)
	    leg_iv[iv_ID] ->AddEntry(gJV_mA[j],"130 #circC annealing for 1.5 h","pe");
	  if(j==4)
	    leg_iv[iv_ID] ->AddEntry(gJV_mA[j],"130 #circC annealing for 2.5 h","pe");
	  
	  
	}
	//	gJV_abs[i]->GetYaxis()->SetRangeUser(1e-11,1e-7);
	civ[iv_ID] ->cd();
	gJV_mA[j] ->Draw("PMC PLC PL");
       	cpic[iv_ID+2] ->cd();
	gJV_mA[j] ->Draw("PMC PLC PL");	
      }
    }
    //    if(!first_flag)
    civ[iv_ID]->cd();
    leg_iv[iv_ID]->Draw();
    cpic[iv_ID+2]->cd();
    leg_pic[iv_ID+2]->Draw();      
    iv_ID++;

    }


  
  // ==================== IV Diff TGraphErrors ===================================== //

    //SetIVDiff(TGraphErrors* gDiff)
    int iii=0;
    string file_diff_ID[100],file_diff_CH[100];
    /// ---------- Set gJV_mA_Diff TGraphErrors 
    for(int i=0; i<nfile;i++){
      for(int j=i+1; j<nfile;j++){
	if(file_ID[j] == file_ID[i] && file_CH[j] == file_CH[i]){
	  if(file_Time[i] == file_Time[j]){
	    file_diff_ID[iii] = file_ID[j];
	    file_diff_CH[iii] = file_CH[j];
	    //	    id_diff[iii] = stoi(file_diff_ID[iii]);
	    //	    ch_diff[iii] = stoi(file_diff_CH[iii]);
	    if(file_LIGHT[i] == "LIGHT" && file_LIGHT[j] == "DARK"){
	      gJV_mA_Diff[iii] = GetIV_Diff(gJV_mA[i], gJV_mA[j]);
	      gJV_mA_Diff[iii]->SetLineColor(kRainBow);
	      gJV_mA_Diff[iii]->SetMarkerStyle(20);

	      iii++;
	      N_DIFF= iii;
	    }else if(file_LIGHT[i] == "DARK" && file_LIGHT[j] == "LIGHT"){
	      gJV_mA_Diff[iii] = GetIV_Diff(gJV_mA[j], gJV_mA[i]);
	      gJV_mA_Diff[iii]->SetLineColor(kRainBow);
	      gJV_mA_Diff[iii]->SetMarkerStyle(20);
	      iii++;
	      N_DIFF= iii;
	    }

	  }
	}
      }
    } // end for nfile

    /// -------- Draw
    //    TGraphErrors* gTemp_Diff[100];
    for(int i=0; i<iii;i++){
      bool first_flag = true;
      for(int j=i+1; j<iii;j++){
	if(file_diff_ID[j] == file_diff_ID[i] && file_diff_CH[j] == file_diff_CH[i]){
	  if(first_flag){
	    civ_diff[i] = new TCanvas(Form("civ_d%d",i),Form("civ_d%d",i));
	    gTemp_Diff[i] = new TGraphErrors();
	    gTemp_Diff[i] ->SetTitle("J-V curve; V [V]; J [mA/cm2]");
	    gTemp_Diff[i]->SetPoint(0,-2.1,-20.0);
	    gTemp_Diff[i]->SetPoint(1,1.1,50.0);
	    gTemp_Diff[i]->SetMarkerColor(0);
	    gTemp_Diff[i]->SetLineColor(0);
	    leg_iv_diff[i] = new TLegend(0.1,0.40,0.6,0.9,"","NDC");
	    civ_diff[i] ->cd();
	    gTemp_Diff[i] ->Draw("AP");
	    gJV_mA_Diff[i] ->Draw("PMC PLC PL");
	    leg_iv_diff[i] ->AddEntry(gJV_mA_Diff[i],Form("%s-%s (JV: Light-Dark) Temp. at %3.0f (%s)",file_diff_ID[i].c_str(),file_diff_CH[j].c_str(),temp,file_Time[i].c_str()),"pe");
	    gPad->SetGridy(1);
	    first_flag =false;
	  } // end first_flag
	  gJV_mA_Diff[j] ->Draw("PMC PLC PL");
	  leg_iv_diff[i] ->AddEntry(gJV_mA_Diff[j],Form("%s-%s (JV: Light-Dark) Temp. at %3.0f (%s)",file_diff_ID[j].c_str(),file_diff_CH[j].c_str(),temp,file_Time[j].c_str()),"pe");
	}
      }
      if(!first_flag){
	civ_diff[i]->cd();
	leg_iv_diff[i]->Draw();
      }
      }

    // ================================================================================ //




  }



  
  if(!CYRIC_ANALYSIS){
    for(int i=0; i<nfile;i++){
      for(int j=i+1; j<nfile;j++){
	if(j+1 >= nfile) break;
	if(file_ID[j] == file_ID[i] && file_CH[j] == file_CH[i]){
	  civ[i] = new TCanvas(Form("civ%d",i),Form("civ%d",i));
	  leg_iv[i] = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
	  civ[i] ->cd();
	  gPad->SetGridy(1);
	  leg_iv[i] ->AddEntry(gJV_mA[i],file_name[i].c_str(),"pe");
	  leg_iv[i] ->AddEntry(gJV_mA[j],file_name[j].c_str(),"pe");
	  //	gJV_abs[i]->GetYaxis()->SetRangeUser(1e-11,1e-7);
	  gJV_mA[i] ->Draw("PMC APL");
	  gJV_mA[i] ->GetXaxis()->SetRangeUser(-2.1,1.1);
	  gJV_mA[j] ->Draw("PMC PL");
	  leg_iv[i]->Draw();
	}
      }
    }
  }
  
}


// ===================================================================== //


void IVana::NewROOT(string ofrname){
  ofr = new TFile(ofrname.c_str(),"recreate");

}

// ======================================================================= //
void IVana::Write(){

  ofr->Write();

}

// ======================================================================= //













/// ==================================================== ///
/// ===================== MAIN ========================= ///
/// ==================================================== ///


int main(int argc, char** argv){

  gStyle->SetPalette(kRainBow);
  gErrorIgnoreLevel = kError;
  int ch;
  extern char *optarg;
  string fname;
  string pname;
  string rname;
  bool root_flag =false;
  while((ch=getopt(argc,argv,"s:f:r:op"))!=-1){
    switch(ch){
    case 'f':
      fname = optarg;
      break;
    case 'p':
      pname = optarg;
      break;

    case 'r':
      rname = optarg;
      root_flag = true;
      break;
    }
  }

  //  gROOT->SetBatch(1);    
  gStyle->SetOptStat(0000);  

  IVana*ivana = new IVana();
  TApplication theApp("App", &argc, argv);
  ivana->SetRun(fname);
  ivana->SetGraph();
  ivana->Loop();
  //ivana->Draw();
  ivana->Draw_CYRIC();
  if(root_flag ) ivana->NewROOT(rname);
  if(root_flag ) ivana->Write();
  //  gSystem->Exit(1);
  theApp.Run();
  return 0;

}


//================= Function ==============================//

vector<string> split(string&input, char delimiter){

  istringstream stream(input);
  string field;
  vector<string> result;
  while(getline(stream,field,delimiter))
    result.push_back(field);

  return result;
};

