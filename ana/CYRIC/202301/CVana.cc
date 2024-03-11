#include "CVana.h"
extern vector<string> split(string&input, char delimiter);
extern double NAT_function(double* x, double* par);
bool detector_mode = false;
bool CYRIC_ANALYSIS = false;
bool HIMAC_ANALYSIS = false;
// ============================================================== //

void CVana::ReadFile(string ifname){

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
    sbuf >> CVdata[nfile];

    strvec  = split(CVdata[nfile],'/');
    strvec2 = split(strvec[strvec.size()-1],'_');
    strvec3 = split(strvec2[strvec2.size()-1],'.');
    file_name[nfile] =strvec2[1] + strvec2[2] +" " + strvec3[0] ;


    file_ID[nfile] = strvec2[1];
    file_CH[nfile] = strvec2[2];
    file_SU[nfile] = strvec3[0];
    
    if(strvec2.size()>=4 && strvec3[0]=="HIMAC"){
      HIMAC_ANALYSIS = true;
      // HIMAC ANALYSIS MODE 
      if(strvec2.size()>=5) file_status[nfile] = strvec2[3];
      if(strvec2.size()>=6) file_anealing_time[nfile] = strvec2[4];
      if(file_status[nfile] =="Init")file_status[nfile] = "Before Irradiation";
      if(file_status[nfile] =="Rad1")file_status[nfile] = "After Irradiation";
      if(file_status[nfile] =="Rad2")file_status[nfile] = "After 2nd Irradiation";
      file_name[nfile] = strvec2[1] + strvec2[2] +" " + file_status[nfile] +" "+ file_anealing_time[nfile];

      
    }else if(strvec2.size()>=4 && strvec3[0]=="CYRIC"){
      
      CYRIC_ANALYSIS = true;

      file_Temp[nfile] = strvec2[3]; // Annealing Temp.
      file_Time[nfile] = strvec2[4]; // Annealing Time
      temp = stod(file_Temp[nfile]);
      int st=0;
      if(file_Time[nfile].find("m")!=-1){time_unit ="min"; st = file_Time[nfile].find("m");}
      if(file_Time[nfile].find("h")!=-1){ time_unit = "hour";st = file_Time[nfile].find("h");}
      if(file_Time[nfile].find("d")!=-1){ time_unit = "day";st = file_Time[nfile].find("d");}
      TIME[nfile] = stod(file_Time[nfile].substr(0,st));
      file_name[nfile] =strvec2[1] +"Temp : "+file_Temp[nfile]+" " + file_Time[nfile] +" ";

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
    }
    
    nfile++;
  }
}

// ============================================================= //

void CVana::GetCV(string ifname, int ifile){

  string buf;
  ifstream ifp(Form("%s",ifname.c_str()),ios::in);
  ncv[ifile] =0;
  vector<string>strvec;
  
  while(1){
    getline(ifp,buf);
    if( buf[0]=='#' ){ continue; }
    if(ifp.eof() || buf.size()==0)break;
    strvec = split(buf,',');
    V[ifile][ncv[ifile]] = stof(strvec.at(0));
    C[ifile][ncv[ifile]] = stof(strvec.at(1));
    GetVal(V[ifile][ncv[ifile]],C[ifile][ncv[ifile]], ifile, ncv[ifile]);
    SetVal(ifile, ncv[ifile]);
    ncv[ifile]++;
  }

  for(int i=0; i<ncv[ifile];i++){
    double N = GetVal_N(gV_C[ifile],i);
    //    cout<<"i "<<i<<" V "<<V[ifile][i]<<" N "<<N<<endl;
    gNV[ifile]->SetPoint(i,V[ifile][i],N);
  }
  
}

// ============================================================== //

void CVana::SetGraph(){


  for(int i=0; i<nch ; i++){
    gCV_Diff_2V[i] = new TGraphErrors();
    gCV_Diff_2V[i]->SetTitle("CV-curve ; ID ; Diff");
    gCV_Diff_2V[i]->SetName(Form("gCV_Diff_2V_%d",i));
    gCV_Diff_2V[i] ->SetMarkerStyle(20);
    gCV_Diff_2V[i] ->SetMarkerColor(kRainBow);
    gCV_Diff_2V[i] ->SetLineColor(kRainBow);

    gWV_Diff_2V[i] = new TGraphErrors();
    gWV_Diff_2V[i]->SetTitle("WV-curve ; ID ; Diff");
    gWV_Diff_2V[i]->SetName(Form("gWV_Diff_2V_%d",i));
    gWV_Diff_2V[i] ->SetMarkerStyle(20);
    gWV_Diff_2V[i] ->SetMarkerColor(kRainBow);
    gWV_Diff_2V[i] ->SetLineColor(kRainBow);    
  }

  for(int i=0; i< nfile ;i++){
    gCV[i] = new TGraphErrors();
    gCV[i]->SetTitle("CV-curve ; Voltage [V] ; Cp [F]");
    gCV[i]->SetName(Form("gCV_%d",i));
    gCV[i] ->SetMarkerStyle(20);
    gCV[i] ->SetMarkerColor(kRainBow);
    gCV[i] ->SetLineColor(kRainBow);

    gV_C[i] = new TGraphErrors();
    gV_C[i]->SetTitle("(1/C2)V-curve ; Voltage [V] ; 1/Cp^2 [1/F^2]");
    gV_C[i]->SetName(Form("gCV_%d",i));
    gV_C[i] ->SetMarkerStyle(20);
    gV_C[i] ->SetMarkerColor(kRainBow);
    gV_C[i] ->SetLineColor(kRainBow);

    
    gWV[i] = new TGraphErrors();
    gWV[i]->SetName(Form("gWV_%d",i));
    gWV[i]->SetTitle("WV-curve ; Voltage [V] ; W [um]");
    gWV[i] ->SetMarkerStyle(20);
    gWV[i] ->SetMarkerColor(kRainBow);
    gWV[i] ->SetLineColor(kRainBow);

    gNV[i] = new TGraphErrors();
    gNV[i]->SetName(Form("gNV_%d",i));
    gNV[i]->SetTitle("NV-curve ; Voltage [V] ; Na [cm^{-3}]");
    gNV[i] ->SetMarkerStyle(20);
    gNV[i] ->SetMarkerColor(kRainBow);
    gNV[i] ->SetLineColor(kRainBow);

    gWN[i] = new TGraphErrors();
    gWN[i]->SetName(Form("gWV_%d",i));
    gWN[i]->SetTitle("WN-curve ; W [um] ; Na [cm^{-3}]");
    gWN[i] ->SetMarkerStyle(20);
    gWN[i] ->SetMarkerColor(kRainBow);
    gWN[i] ->SetLineColor(kRainBow);

    
    gCV_Diff[i] = new TGraphErrors();
    gCV_Diff[i]->SetTitle("CV-curve ; Voltage [V] ; Relative Ratio [%]");
    gCV_Diff[i]->SetName(Form("gCV_%d",i));
    gCV_Diff[i] ->SetMarkerStyle(20);
    gCV_Diff[i] ->SetMarkerColor(kRainBow);
    gCV_Diff[i] ->SetLineColor(kRainBow);

    gWV_Diff[i] = new TGraphErrors();
    gWV_Diff[i]->SetName(Form("gWV_%d",i));
    gWV_Diff[i]->SetTitle("WV-curve ; Voltage [V] ; Relative Ratio [%]");
    gWV_Diff[i] ->SetMarkerStyle(20);
    gWV_Diff[i] ->SetMarkerColor(kRainBow);
    gWV_Diff[i] ->SetLineColor(kRainBow);

    gNV_Diff[i] = new TGraphErrors();
    gNV_Diff[i]->SetName(Form("gNV_%d",i));
    gNV_Diff[i]->SetTitle("NV-curve ; Voltage [V] ; Relative Ratio [%]");
    gNV_Diff[i] ->SetMarkerStyle(20);
    gNV_Diff[i] ->SetMarkerColor(kRainBow);
    gNV_Diff[i] ->SetLineColor(kRainBow);            
  }


  // =============== Annealing Graph ==================== //

  for(int iid = 0; iid<n_id;iid++){
    for(int ich =0; ich <n_ch; ich++){
      gW_TIME[iid][ich] = new TGraphErrors();
      gW_TIME[iid][ich]->SetName(Form("gW_TIME_%d_%d",iid,ich));
      gW_TIME[iid][ich] -> SetTitle(Form("Depletion layer width at %1.1fV  Annealing Time Dependence at %3.f degree; TIME [%s]; Depletion width [#mum]",Vset,temp,time_unit.c_str()));
      gW_TIME[iid][ich] ->SetMarkerStyle(20);
      gW_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gW_TIME[iid][ich] ->SetLineColor(kRainBow);

      gNa_TIME[iid][ich] = new TGraphErrors();
      gNa_TIME[iid][ich]->SetName(Form("gNa_TIME_%d_%d",iid,ich));
      gNa_TIME[iid][ich] -> SetTitle(Form("Ion concentration at %1.1fV  Annealing Time Dependence at %3.f degree; TIME [%s]; Ion concentration [1e16 cm^{-3}]",Vset,temp,time_unit.c_str()));
      gNa_TIME[iid][ich] ->SetMarkerStyle(20);
      gNa_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gNa_TIME[iid][ich] ->SetLineColor(kRainBow);

      gC_TIME[iid][ich] = new TGraphErrors();
      gC_TIME[iid][ich]->SetName(Form("gC_TIME_%d_%d",iid,ich));
      gC_TIME[iid][ich] -> SetTitle(Form("Depletion layer width at %1.1fV  Annealing Time Dependence at %3.f degree; TIME [%s]; Capacitance [nF]",Vset,temp,time_unit.c_str()));
      gC_TIME[iid][ich] ->SetMarkerStyle(20);
      gC_TIME[iid][ich] ->SetMarkerColor(kRainBow);
      gC_TIME[iid][ich] ->SetLineColor(kRainBow);


      gW_TIME_R[iid][ich] = new TGraphErrors();
      gW_TIME_R[iid][ich]->SetName(Form("gW_TIME_R_%d_%d",iid,ich));
      gW_TIME_R[iid][ich] -> SetTitle(Form("Depletion layer width at %1.1fV  Annealing Time Dependence at %3.f degree; TIME [%s]; Normalized Value",Vset,temp,time_unit.c_str()));
      gW_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gW_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gW_TIME_R[iid][ich] ->SetLineColor(kRainBow);

      gNa_TIME_R[iid][ich] = new TGraphErrors();
      gNa_TIME_R[iid][ich]->SetName(Form("gNa_TIME_R_%d_%d",iid,ich));
      gNa_TIME_R[iid][ich] -> SetTitle(Form("Ion concerntration at %1.1fV  Annealing Time Dependence at %3.f degree; TIME [%s]; Normalized Value",Vset,temp,time_unit.c_str()));
      gNa_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gNa_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gNa_TIME_R[iid][ich] ->SetLineColor(kRainBow);

      gC_TIME_R[iid][ich] = new TGraphErrors();
      gC_TIME_R[iid][ich]->SetName(Form("gC_TIME_R_%d_%d",iid,ich));
      gC_TIME_R[iid][ich] -> SetTitle(Form("Depletion layer width at %1.1fV  Annealing Time Dependence at %3.f degree; TIME [%s]; Normalized Value",Vset,temp,time_unit.c_str()));
      gC_TIME_R[iid][ich] ->SetMarkerStyle(20);
      gC_TIME_R[iid][ich] ->SetMarkerColor(kRainBow);
      gC_TIME_R[iid][ich] ->SetLineColor(kRainBow);
      
      
    }
  }

  // ===================================================== //

  

}

// ============================================================== //

void CVana::GetVal(double Vol, double Cp, int ifile, int isample){

  W[ifile][isample] = 0; // Initailization
  W[ifile][isample] = ep_CIGS/C[ifile][isample]*cell_area;
  int id =atoi(file_ID[ifile].substr(2,2).c_str());
  int seg =atoi(file_CH[ifile].substr(2,1).c_str())-1;    
  if(detector_mode && id ==20){
    W[ifile][isample] *= 2.0 / scale_w[seg];
  }
  Na[ifile][isample] = 0; // Initailization
  double dC_dV = 0;
  if( isample > 0 ){
    dC_dV =(cell_area*cell_area/(C[ifile][isample]*C[ifile][isample]) - cell_area*cell_area/(C[ifile][isample-1]*C[ifile][isample-1]))/(V[ifile][isample]-V[ifile][isample-1]);
    if(dC_dV < 0 )  Na[ifile][isample] = 2./(e*ep_CIGS*dC_dV);
    else Na[ifile][isample] = 0.0;

  }
}

// ============================================================  //

void CVana::Loop(){

  for(int i=0; i<nfile; i++){
    GetCV(CVdata[i], i);
  }
  
}

// ============================================================= //

void CVana::SetVal(int ifile, int isample){

  gCV[ifile] ->SetPoint(isample, V[ifile][isample],C[ifile][isample]);
  gWV[ifile] ->SetPoint(isample, V[ifile][isample],W[ifile][isample]*1e4);
  gV_C[ifile] ->SetPoint(isample, V[ifile][isample],1/C[ifile][isample]/C[ifile][isample]);

}
// ============================================================ //

void CVana::Ana(){

  int iii[nch];
  for(int i=0; i<nch ; i++){
    iii[i] =0;}
  
  for(int i=0; i<nfile/2;i ++){
    for(int j=i+1 ; j<nfile; j++){
      if(file_ID[i] == file_ID[j] && file_CH[i] == file_CH[j]){
	int nbins_i = gCV[i]->GetXaxis()->GetNbins();
	int nbins_j = gCV[j]->GetXaxis()->GetNbins();
	// ---------------------------------------------------------- //
	if(nbins_i==nbins_j && gCV[i]->GetX()[0] == gCV[j]->GetX()[0]
	   && gCV[i]->GetX()[nbins_i-1] == gCV[j]->GetX()[nbins_j-1]){
	  int xbin_2V=0;
	  double x_2V=0;
	  double yCV_2V =0;
	  double yWV_2V =0;
	  for(int ibin =0; ibin<nbins_i; ibin++){
	    double x  = gCV[i]->GetX()[ibin];
	    double yi = gCV[i]->GetY()[ibin];
	    double yj = gCV[j]->GetY()[ibin];
	    gCV_Diff[i] ->SetPoint(ibin,x,(yi-yj)/yi*100.);
	    yi = gWV[i]->GetY()[ibin];
	    yj = gWV[j]->GetY()[ibin];
	    gWV_Diff[i] ->SetPoint(ibin,x,(yi-yj)/yi*100.);
	    yi = gNV[i]->GetY()[ibin];
	    yj = gNV[j]->GetY()[ibin];
	    gNV_Diff[i] ->SetPoint(ibin,x,(yi-yj)/yi*100.);
	    if(fabs(-2. - x) < fabs(-2. - x_2V)){
	      x_2V = x;
	      xbin_2V = ibin;
	      yi = gCV[i]->GetY()[ibin];
	      yj = gCV[j]->GetY()[ibin];
	      yCV_2V = (yi-yj)/yi*100.;
	      yi = gWV[i]->GetY()[ibin];
	      yj = gWV[j]->GetY()[ibin];
	      yWV_2V = (yi-yj)/yi*100.;	      
	    }
	  }
	  
	  // --------------------------------------------------------- // 
	  int seg =atoi(file_CH[i].substr(2,1).c_str())-1;
	  int id =atoi(file_ID[i].substr(2,2).c_str());
	  gCV_Diff_2V[seg] ->SetPoint(iii[seg],id,yCV_2V);
	  gWV_Diff_2V[seg] ->SetPoint(iii[seg],id,yWV_2V);
	  iii[seg]++;
	  // --------------------------------------------------------- //
	  
	}	
      }
    }
  }

  for(int i=0; i<iii[0];i++){
    double x0 = gCV_Diff_2V[0]->GetX()[i];
    double x1 = gCV_Diff_2V[1]->GetX()[i];
    double y0 = gCV_Diff_2V[0]->GetY()[i];
    double y1 = gCV_Diff_2V[1]->GetY()[i];
  }
  
  
}
// ================================================================= //


double CVana::GetVal_N(TGraphErrors* g, int i){

  double dC_2,V,N;
  double dV = 0.3;
  g->GetPoint(i,V,dC_2);
  f->SetParameters(-3.58222e+14,1.40413e+15);
  g ->Fit("f","QR","QR",V-dV,V+dV);
  double d = f->GetParameter(0);
  N  = fabs(2./(d*e*ep_CIGS));
  //  cout<<" i "<<i<<" V "<<V<<" 1/C2 "<<dC_2<<" N "<<N<<endl; 
  return N;
}

// ================================================================= //
TGraphErrors* CVana::GetNW(TGraphErrors* gN, TGraphErrors* gW){


  //  TF1* f = new TF1("f","[0]*x+[1]",-3,0);
  TGraphErrors* gWN= new TGraphErrors();
  int nbins = gN->GetN();
  double V,dN,W,N;
  //  double dV = 0.3; 
  for(int i=0; i<nbins;i++){
    gN->GetPoint(i,V,dN);
    gW->GetPoint(i,V,W);
    //    f->SetParameters(-3.58222e+14,1.40413e+15);
    //    gN ->Fit("f","QR","QR",V-dV,V+dV);
    //    double d = f->GetParameter(0);
    //    N  = fabs(2./(d*e*ep_CIGS));
    N = GetVal_N(gN, i);
    gWN->SetPoint(i,W,N);
  }


  return gWN;
  

  
}

// ================================================================== //
void CVana::Ana_CYRIC(){

  double V_set =1000;
  double C_set =0;
  double N_set =0;
  double W_set =0;
  int i_ID_CH[NID][NCH];
  double W_init =1.0;
  double C_init =1.0;
  double N_init =1.0;
  
  for(int iid=0;iid<n_id;iid++)
    for(int ich=0;ich<n_ch;ich++)
      i_ID_CH[iid][ich] =0;

  TF1* f0 = new TF1("f0","[0]*x+[1]",-3,0);

  for(int i=0; i<nfile; i++){

    string id_st = file_ID[i].substr(2);
    string ch_st = file_CH[i].substr(2);
    int id = stoi(id_st.c_str());
    int ch = stoi(ch_st.c_str());

    for(int iid=0;iid<n_id;iid++){
      if(id_num[iid]== id){
	for(int ich=0;ich<n_ch;ich++){
	  if(ch_num[ich]== ch){
	    V_set = 100000;
	    for(int isample =0; isample< ncv[i]; isample++){
	      if(fabs(V[i][isample] - Vset) <= V_set){
		V_set = V[i][isample];
		C_set = C[i][isample]*1.0e9;       // nF
		N_set = fabs(Na[i][isample])/1.0e16;  // [10e16 cm^2]
		W_set = W[i][isample]*1.0e4; // [um]
	      }
	    }

	    //	    f->SetParameters(-0.000119939e14,-13.9997);
	    f0->SetParameters(-3.58222e+14,1.40413e+15);
	    gV_C[i]->Fit("f0","QR","QR",Vset-0.5,Vset+0.5);
	      double d = f0->GetParameter(0);
	      N_set  = fabs(2./(d*e*ep_CIGS))/1.0e16;
	    if(TIME[i]<0){
	      W_init = W_set;
	      C_init = C_set;
	      N_init = N_set;
	    }else{
	      gW_TIME[iid][ich] ->SetPoint(i_ID_CH[iid][ich],TIME[i],W_set);
	      gC_TIME[iid][ich] ->SetPoint(i_ID_CH[iid][ich],TIME[i],C_set);
	      gNa_TIME[iid][ich] ->SetPoint(i_ID_CH[iid][ich],TIME[i],N_set);
	      i_ID_CH[iid][ich]++;
	    }
	  }
	}
      }
    }

    
    gWN[i] =GetNW(gV_C[i],gWV[i]);


    
  } // nfile


      /// ====== Set Scaled Values ============= //
  for(int i=0; i<nfile; i++){
    for(int iid=0;iid<n_id;iid++){
      for(int ich=0;ich<n_ch;ich++){
	int nbins = gW_TIME[iid][ich] ->GetN();
	for(int ibin =0; ibin<nbins;ibin++){
	  double time;
	  gW_TIME[iid][ich] ->GetPoint(ibin,time,W_set);
	  gC_TIME[iid][ich] ->GetPoint(ibin,time,C_set);
	  gNa_TIME[iid][ich] ->GetPoint(ibin,time,N_set);
	  gW_TIME_R[iid][ich] ->SetPoint(ibin,time,W_set/W_init);
	  gC_TIME_R[iid][ich] ->SetPoint(ibin,time,C_set/C_init);
	  gNa_TIME_R[iid][ich] ->SetPoint(ibin,time,N_set/N_init);
	}
      }
    }
  }
 

}

// ============================================================ //

void CVana::Draw(){

  if(HIMAC_ANALYSIS){
  
    for(int i=0; i<ncanvas; i++){
      c[i] = new TCanvas(Form("c%d",i),Form("c%d",i));
      leg[i] = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
    }

    c[0]->cd();
    gPad->SetLogy(1);
    for(int i=0; i<nfile; i++){
      leg[0] ->AddEntry(gCV[i],file_name[i].c_str(),"pe");
      if(i==0){ gCV[i] ->Draw("PMC AP");
	gCV[i]->GetYaxis()->SetRangeUser(1e-11,1e-7);
      }
      else  gCV[i] ->Draw("PMC P");
    }
    leg[0]->Draw();
  
    c[1]->cd();
    for(int i=0; i<nfile; i++){
      leg[1] ->AddEntry(gCV[i],file_name[i].c_str(),"pe");
      if(i==0){ gWV[i] ->Draw("PMC APC");
	gWV[i] ->GetYaxis()->SetRangeUser(0,2.5);
      }
      else  gWV[i] ->Draw("PMC P");
    }
    leg[1]->Draw();

    c[2]->cd();
    for(int i=0; i<nfile; i++){
      leg[2] ->AddEntry(gNV[i],file_name[i].c_str(),"pe");
      if(i==0){ gNV[i] ->Draw("PMC AP");
      }
      else  gNV[i] ->Draw("PMC P");
    }
    leg[2]->Draw();


    c[3]->cd();
    for(int i=0; i<nfile/2; i++){
      diff_name[i]=file_ID[i]+file_CH[i]+" Diff";
      leg[3] ->AddEntry(gCV_Diff[i],diff_name[i].c_str(),"pe");
      if(i==0){ gCV_Diff[i] ->Draw("PMC AP");
      }
      else  gCV_Diff[i] ->Draw("PMC P");
    }
    leg[3]->Draw();

    c[4]->cd();
    for(int i=0; i<nfile/2; i++){
      diff_name[i]=file_ID[i]+file_CH[i]+" Diff";
      leg[4] ->AddEntry(gWV_Diff[i],diff_name[i].c_str(),"pe");
      if(i==0){ gWV_Diff[i] ->Draw("PMC AP");
      }
      else  gWV_Diff[i] ->Draw("PMC P");
    }
    leg[4]->Draw();


    c[5]->cd();

    gCV_Diff_2V[0]->GetYaxis()->SetRangeUser(-100,100);
    gCV_Diff_2V[0]->Draw("PMC AP");
    gCV_Diff_2V[1]->Draw("PMC P");
  


    c[6]->cd();
    for(int i=0; i<nfile; i++){
      leg[6] ->AddEntry(gWN[i],file_name[i].c_str(),"pe");
      if(i==0){ gWN[i] ->Draw("PMC AP");
      }
      else  gWN[i] ->Draw("PMC P");
    }
    leg[6]->Draw();

    for(int i=0; i<nfile;i++){
      for(int j=i+1; j<nfile;j++){
	if(j+1 >= nfile) break;
	if(file_ID[j] == file_ID[i] && file_CH[j] == file_CH[i]){
	  cv[i] = new TCanvas(Form("cv%d",i),Form("cv%d",i));
	  leg_cv[i] = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
	  cv[i] ->cd();
	  string name =file_name[i]+file_SU[i];
	  leg_cv[i] ->AddEntry(gCV[i],name.c_str(),"pe");
	  string name2 =file_name[j]+file_SU[j];
	  leg_cv[i] ->AddEntry(gCV[j],name2.c_str(),"pe");
	  gCV[i]->GetYaxis()->SetRangeUser(1e-11,1e-7);
	  gCV[i] ->Draw("PMC AP");
	  gCV[j] ->Draw("PMC P");
	  leg_cv[i]->Draw();
	}
      }
    }

  }

  if(CYRIC_ANALYSIS){
    TCanvas* cR =new TCanvas("cR","cR");
    TLegend* legR = new TLegend(0.5,0.50,0.98,0.98,"","NDC");
    for(int i=0; i<ncanvas_CYRIC; i++){
      c[i] = new TCanvas(Form("c%d",i),Form("c%d",i));
      if(i<3) leg[i] = new TLegend(0.5,0.80,0.98,0.98,"","NDC");
      else leg[i] = new TLegend(0.5,0.50,0.98,0.98,"","NDC");
    }


    for(int iid =0; iid<n_id; iid++){
      for(int ich =0; ich<n_ch; ich++){
	if(iid ==0 && ich ==0){
	  c[0]->cd();
	  leg[0] ->AddEntry(gW_TIME[iid][ich],Form("ID%d-CH%d Temp. %3.0f",id_num[iid],ch_num[ich],temp,"pe"));
	  gW_TIME[iid][ich] -> Draw("PMC PLC APL");
	  c[1]->cd();
	  leg[1] ->AddEntry(gC_TIME[iid][ich],Form("ID%d-CH%d Temp. %3.0f",id_num[iid],ch_num[ich],temp,"pe"));
	  gC_TIME[iid][ich] -> Draw("PMC PLC APL");
	  c[2]->cd(); // Carrier Density 
	  leg[2] ->AddEntry(gNa_TIME[iid][ich],Form("ID%d-CH%d Temp. %3.0f",id_num[iid],ch_num[ich],temp,"pe"));
	  gNa_TIME[iid][ich] -> Draw("PMC PLC APL");

	  cR->cd();
	  legR ->AddEntry(gW_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %3.0f width",id_num[iid],ch_num[ich],temp,"pe"));	  
	  gW_TIME_R[iid][ich]->Draw("PMC PLC APL");
	  gW_TIME_R[iid][ich]->GetYaxis()->SetRangeUser(0.,5.);
	  legR ->AddEntry(gNa_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %3.0f Na ",id_num[iid],ch_num[ich],temp,"pe"));	  
	  gNa_TIME_R[iid][ich]->Draw("PMC PLC PL");
	  
	}else{
	  c[0]->cd();
	  leg[0] ->AddEntry(gW_TIME[iid][ich],Form("ID%d-CH%d Temp. %3.0f",id_num[iid],ch_num[ich],temp,"pe"));
	  gW_TIME[iid][ich] -> Draw("PMC PLC PL");
	  c[1]->cd();
	  leg[1] ->AddEntry(gC_TIME[iid][ich],Form("ID%d-CH%d Temp. %3.0f",id_num[iid],ch_num[ich],temp,"pe"));
	  gC_TIME[iid][ich] -> Draw("PMC PLC PL");
	  c[2]->cd();
	  leg[2] ->AddEntry(gNa_TIME[iid][ich],Form("ID%d-CH%d Temp. %3.0f",id_num[iid],ch_num[ich],temp,"pe"));
	  gNa_TIME[iid][ich] -> Draw("PMC PLC PL");

	  cR->cd();
	  legR ->AddEntry(gW_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %3.0f width ",id_num[iid],ch_num[ich],temp,"pe"));	  
	  gW_TIME_R[iid][ich]->Draw("PMC PLC PL");
	  legR ->AddEntry(gNa_TIME_R[iid][ich],Form("ID%d-CH%d Temp. %3.0f Na ",id_num[iid],ch_num[ich],temp,"pe"));	  
	  gNa_TIME_R[iid][ich]->Draw("PMC PLC PL");	  
	}
      }
    }

  

    for(int i=0; i<nfile;i++){
      if(file_CH[i]=="CH2") continue;
      c[3]->cd(); // CV curve
      if(i==0){ gCV[i] ->Draw("PMC PLC APL");
	gCV[i]->GetYaxis()->SetRangeUser(1.0e-9,5.0e-8);
      }else gCV[i] ->Draw("PMC PLC PL");
      leg[3]->AddEntry(gCV[i],Form("%s-%s (Temp. %3.0f) %s",file_ID[i].c_str(),file_CH[i].c_str(),temp,file_Time[i].c_str()));
      c[4]->cd();
      if(i==0 ){ gWV[i] ->Draw("PMC PLC APL");
	gWV[i]->GetYaxis()->SetRangeUser(0,2.0);
      }else gWV[i] ->Draw("PMC PLC PL");
      leg[4]->AddEntry(gWV[i],Form("%s-%s (Temp. %3.0f) %s",file_ID[i].c_str(),file_CH[i].c_str(),temp,file_Time[i].c_str()));
      c[5]->cd();
      if(i==0 ){
	gPad->SetLogy(1);
	gNV[i] ->Draw("PMC PLC APL");
	gNV[i]->GetYaxis()->SetRangeUser(1e15,1e17);
      }else gNV[i] ->Draw("PMC PLC PL");
      leg[5]->AddEntry(gNV[i],Form("%s-%s (Temp. %3.0f) %s",file_ID[i].c_str(),file_CH[i].c_str(),temp,file_Time[i].c_str()));

      c[6]->cd();
      if(i==0 ){ gV_C[i] ->Draw("PMC PLC APL");
       	gV_C[i]->GetYaxis()->SetRangeUser(1.0e15,1.0e16);
      }else gV_C[i] ->Draw("PMC PLC PL");
      leg[6]->AddEntry(gV_C[i],Form("%s-%s (Temp. %3.0f) %s",file_ID[i].c_str(),file_CH[i].c_str(),temp,file_Time[i].c_str()));

      c[7]->cd();
      gPad->SetLogy(1);
      if(i==0 ){
	TGraphErrors* gWN_frame = new TGraphErrors();
	gWN_frame->SetTitle("WN-curve ; W [um] ; Na [cm^{-3}]");
	gWN_frame->SetPoint(0,0,1.0e15);
	gWN_frame->SetPoint(1,1,1.0e18);
	gWN_frame->SetMarkerSize(0);
	gWN_frame ->Draw("PMC PLC AP");
	gWN[i] ->Draw("PMC PLC PL");
	//	gWN[i]->GetXaxis()->SetRangeUser(0.,2.);
	//       	gWN[i]->GetYaxis()->SetRangeUser(1.0e15,1e18);
      }else gWN[i] ->Draw("PMC PLC PL");
      leg[7]->AddEntry(gWN[i],Form("%s-%s (Temp. %3.0f) %s",file_ID[i].c_str(),file_CH[i].c_str(),temp,file_Time[i].c_str()));



      
    }

    for(int i=0; i<ncanvas_CYRIC;i++){
      c[i]->cd();
      leg[i]->Draw();
    }
    cR->cd();
    legR->Draw();
  } // end CYRIC_ANALYSIS
  
}

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
  while((ch=getopt(argc,argv,"s:f:r:dop"))!=-1){
    switch(ch){
    case 'f':
      fname = optarg;
      break;

    case 'p':
      pname = optarg;
      break;

    case 'r':
      rname = optarg;
      break;

    case 'd':
      detector_mode = true;
      cell_area = 0.005; // [cm2]
      break;
         
    }
  }


  CVana*cvana = new CVana();
  TApplication theApp("App", &argc, argv);

  cvana->ReadFile(fname.c_str());
  cvana->SetGraph();
  cvana->Loop();

  if(CYRIC_ANALYSIS)cvana->Ana_CYRIC();
  else   cvana->Ana();
  cvana->Draw();
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





double NAT_function(double* x, double* par){

  double N0 = par[0]; // carrier density before irradiation
  double NAO = par[1];// carrier density after irradiation
  double time = par[2]; //  Annealing time [min]
  double A; // acctivity energy [eV] fitting parameters

  
  

  
};
