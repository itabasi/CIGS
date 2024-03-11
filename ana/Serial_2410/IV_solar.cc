const  string fname ="./data/20230803_17:13:34.txt";  // ID 12 CH1
const  string fname_dark ="./data/20230707_23:38:35.txt";
//const  string fname_dark ="/home/togawa/work/SENSOR/CIGS/Ana/analysis/itamacro/IVdata/CYRIC202301/IVdata_ID12_CH1_LIGHT_130_0min_AIST.txt";

#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <TGraph.h>
extern vector<string> split(string&input, char delimiter);
extern double IV(double* x, double*par);
extern double IV_Dark(double* x, double*par);
extern double IV_Solar(double* x, double*par);
extern double IV_diode_LV(double*x ,double*par);
extern double IV_diode_HV(double*x ,double*par);
extern double J0(double*x , double*par);
const double area = 0.522; // cm2 : 1 cm * 0.5 cm
const   int id = 15;
const   int ICH =1;
//const   double kb = 1.38e-23; // J K
const   double kb = 8.67333e-5; // eV K
const   double q  = 1.602e19; // C
const  double Pin = 0.100;//[W/cm^2]
void IV_solar(){
  gStyle->SetPalette(kRainBow);
  //  string fname ="../../data/20230309_17:27:24.txt";
  string buf;
  float HV[1000],I[1000];
  int ii=0;
  float HV_OFF=0.0;
  vector<string>strvec;
  ifstream ifp(fname.c_str(),ios::in);
  TGraph* g = new TGraph();
  TGraph* gA = new TGraph();
  TGraph* gDark = new TGraph();
  TGraph* gmA_Dark = new TGraph();
  TGraph* gmA_Dark_abs = new TGraph();
  TGraph* gDark_Isc = new TGraph();
  TGraph* gmA = new TGraph();
  TGraph* gmA_abs = new TGraph();
  TGraph* gW = new TGraph();
  TGraph* gEta = new TGraph();
  g->SetTitle("; HV[V]; Current [mA/cm^{2}]");
  g -> SetMarkerStyle(20);
  g -> SetMarkerColor(4);
  g -> SetLineColor(4);  
  gmA->SetTitle("; HV[V]; Current [mA/cm^{2}]");
  gmA -> SetMarkerStyle(20);
  gmA -> SetMarkerColor(4);
  gmA -> SetLineColor(4);

  gmA_abs->SetTitle("; HV[V]; Current [mA/cm^{2}]");
  gmA_abs -> SetMarkerStyle(20);
  gmA_abs -> SetMarkerColor(4);
  gmA_abs -> SetLineColor(4);
  
  
  gW -> SetMarkerStyle(20);
  gW -> SetMarkerColor(3);

  gEta -> SetMarkerStyle(20);
  gEta -> SetMarkerColor(3);

  
  gmA_Dark->SetTitle("; HV[V]; Current [mA/cm^{2}]");
  gmA_Dark -> SetMarkerStyle(20);
  gmA_Dark -> SetMarkerColor(2);
  gmA_Dark -> SetLineColor(2);

  gmA_Dark_abs->SetTitle("; HV[V]; Current [mA_Dark/cm^{2}]");
  gmA_Dark_abs -> SetMarkerStyle(20);
  gmA_Dark_abs -> SetMarkerColor(2);
  gmA_Dark_abs -> SetLineColor(2); 

  gW -> SetMarkerStyle(20);
  gW -> SetMarkerColor(3);
  
  gA->SetTitle("; HV[V]; Current [A]");
  gA -> SetMarkerStyle(20);
  gA -> SetMarkerColor(6);
  gA -> SetLineColor(6);
  
  // ======= IV Curve without Solar light ============= //
  
  float HV_dark[1000],I_dark[1000];
  gDark->SetTitle("; HV[V]; Current [mA/cm^{2}]");
  gDark -> SetMarkerStyle(20);
  gDark -> SetMarkerColor(2);
  gDark -> SetLineColor(2);

  gDark_Isc->SetTitle("; HV[V]; Current [mA/cm^{2}]");
  gDark_Isc -> SetMarkerStyle(20);
  gDark_Isc -> SetMarkerColor(7);
  gDark_Isc -> SetLineColor(7);
  
  int iii=0;
  //  float HV_OFF=0.0;
  ifstream ifp_dark(fname_dark.c_str(),ios::in);  
  while(1){
    getline(ifp_dark,buf);
    if(ifp_dark.eof() || buf.size()==0)break;

    strvec = split(buf,',');
    HV_dark[iii] =-100.;
    I_dark[iii]  =-100.;
    if(strvec.size()>1){
      HV_dark[iii] = stof(strvec.at(0));
      I_dark[iii]  = stof(strvec.at(1));
      gDark->SetPoint(iii,HV_dark[iii] + HV_OFF,I_dark[iii]*1.e3/area);
      gmA_Dark->SetPoint(iii,HV_dark[iii] + HV_OFF,-I_dark[iii]*1.e3/area );
      gmA_Dark_abs->SetPoint(iii,HV_dark[iii] + HV_OFF,fabs(-I_dark[iii]*1.e3/area) );
      iii++;   
    }
  }

  // ======= IV Curve with Solar light ============= //
  
  while(1){
    getline(ifp,buf);
    if(ifp.eof() || buf.size()==0)break;
    strvec = split(buf,',');
    HV[ii] =-100.;
    I[ii]  =-100.;
    if(strvec.size()>1){
      HV[ii] = stof(strvec.at(0));
      I[ii]  = stof(strvec.at(1));
      //      cout<<"ii "<<ii<<" HV "<<HV[ii]<<" I "<<I[ii]<<endl;
      gA->SetPoint(ii,HV[ii] + HV_OFF,-I[ii] );
      g->SetPoint(ii,HV[ii] + HV_OFF,I[ii]/area*1.e3);
      gmA->SetPoint(ii,HV[ii] + HV_OFF,(-I[ii])/area*1.e3);
      gmA_abs->SetPoint(ii,HV[ii] + HV_OFF,fabs((-I[ii])/area*1.e3));
      //      gW->SetPoint(ii,HV[ii] + HV_OFF,HV[ii]*(-I[ii]));
      gW->SetPoint(ii,HV[ii] + HV_OFF,HV[ii]*(-I[ii])*1.0e3/area);
      gEta->SetPoint(ii,HV[ii] + HV_OFF,HV[ii]*(-I[ii])/area/Pin*100.);
      ii++;

    }
  }


  TF1* fI = new TF1("fI",IV,-2,0,2);
  fI ->SetParameter(0,1);
  fI ->SetParameter(1,1);
  gDark->Fit("fI","","",0.5,0.8);
  double Jo = fI->GetParameter(0);
  double n  = fI->GetParameter(1);
  

  // ========= Calc Voc Values =============== //

  double x0,x1,y0,y1, Voc;
  double a,b;
  double Jsc; // [mA/cm2]
  double Isc; // [A]
  double Jpm; // [mA/cm2]
  double Ipm; // [A]
  double Vpm; // [V]
  double W=0;
  double Pm =0;
  double Rsh;
  double Rs;
  double xx0 =100;
  double yy0 =100;
  double a0;
  double aRs;
  double bRs;
  //  gmA ->GetPoint(0,x0,Jsc);
  //  Isc = Jsc*1.e-3*area;

  for(int i=1;i<ii;i++){
    gA->GetPoint(i-1,x0,y0);
    gA->GetPoint(i,x1,y1);

    
    W = x1*y1;
    if(Pm < W ){
      Ipm = y1; // [A]
      Jpm = y1*1.e3/area; // [mA/cm2]
      Vpm = x1; // [V]
      Pm  = W;  // [W]

    }

    a = (y1-y0)/(x1-x0);
    b = y1 - a*x1;

    if(fabs(x0)<fabs(xx0)){
      xx0 = x0;
      Isc = y0;
      a0 = a;
      Rsh = fabs(1./a);
    } 


    
    if(y0 > 0 && y1 <0){      
      Voc = -b/a;
      Rs = fabs(1./a);
      aRs = a;
      bRs = b;
    }

  }

  
  Jsc = Isc / area*1.0e3;

  double xDark,yDark;
  for(int i=0;i<iii;i++){
    gDark->GetPoint(i,xDark,yDark);
    gDark_Isc->SetPoint(i,xDark,(yDark-Jsc));
  }
  
  double FF = Pm/(Voc*Isc);
  //  double Pin = 0.100;//[W/cm^2]
  //  double eta = Isc/Voc*FF/Pin*100.; //[%]
  double eta = Pm/(Pin*area)*100.; //[%]

  TF1* fRs = new TF1("fRs","[0]*x[0]+[1]",-2,2);
  fRs->SetParameter(0,aRs);
  gA->Fit("fRs","QR","QR",Voc-0.1,Voc+0.1);
  cout<<"Rs "<<Rs*area<<" Fit "<< fabs(1./fRs->GetParameter(0))*area<<endl;
  //  Rs = fabs(1./fRs->GetParameter(0));
  TF1* fRs2 = new TF1("fRs2","[0]*x[0]+[1]",-2,2);  
  fRs2->SetParameters(aRs,bRs);  
  fRs2->SetLineColor(4);
 
  TF1* fRsh = new TF1("fRsh","[0]*x[0]+[1]",-2,2);
  fRsh->SetLineColor(1);
  fRsh->SetParameter(0,a0);
  //  fRsh->SetParameter(1,Isc);
  gA->Fit("fRsh","QR","QR",-0.2,0.2);
  cout<<"Rsh "<<Rsh*area<<" Fit "<< fabs(1./fRsh->GetParameter(0))*area<<endl;
  Rsh = fabs(1./fRsh->GetParameter(0));


  

  // ====== Draw ======== //
  TLegend*leg = new TLegend(0.15,0.60,0.5,0.90,"","NDC");
  leg->AddEntry(g, "CIGS w/ Solar light ","pl");
  leg->AddEntry(gDark, "CIGS w/o Solar light ","pl");
  leg->AddEntry(gDark_Isc, "CIGS w/o Solar light - Isc ","pl");
  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  g->Draw("APL");
  gDark->Draw("PL");
  gDark_Isc->Draw("PL");
  leg->Draw();
  TLegend*leg1 = new TLegend(0.15,0.20,0.5,0.50,"","NDC");
  leg1->AddEntry(gmA, "CIGS w/ Solar light ","pl");
  leg1->AddEntry(gmA_Dark, "CIGS Dark  current","pl");
  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  gmA->Draw("APL");  
  gmA_Dark->Draw("PL");
  leg1->Draw();

  TLegend*leg2 = new TLegend(0.15,0.20,0.5,0.50,"","NDC");
  leg2->AddEntry(gmA, "CIGS w/ Solar light ","pl");
  leg2->AddEntry(gmA_Dark, "CIGS w/o Solar light ","pl");
  //  leg2->AddEntry(gW, "CIGS Output Power [mW/cm2]","pl");
  leg2->AddEntry(gEta, "#eta","pl");
  TCanvas* c2 = new TCanvas("c2","c2");
  c2->cd();
  gmA->Draw("APL");
  gmA_Dark->Draw("PL");
  //  gW->Draw("PL");
  gEta->Draw("PL");
  leg2->Draw();
  


  
  
  cout<<"======== < COMMENT OUT > =============="<<endl;
  cout<<"Jsc "<<Jsc <<" mA/cm2 "<<endl;
  cout<<"Voc "<<Voc<<" V "<<endl;
  cout<<"Pm "<<Pm*1.e3/area<<" mW/cm2 "<<endl;
  cout<<"Ipm "<<Ipm <<" A "<<endl;
  cout<<"Vpm "<<Vpm<<" V "<<endl;
  cout<<"Jpm "<<Jpm<<" mA/cm2"<<endl;
  cout<<"FF "<<FF<<endl;
  cout<<"Rs "<<Rs*area<<" Ome*cm2 "<<endl;
  cout<<"Rsh "<<Rsh*area<<" Ome*cm2 "<<endl;
  cout<<"Area "<<area<<" cm2"<<endl;
  cout<<"eta "<<eta<<" %"<<endl;
  cout<<"J0 "<<Jo<<" mA/cm2 "<<endl;
  cout<<"n "<<n<<endl;

  cout<<"======== < COMMENT OUT FOR XCEL > =============="<<endl;
  cout<<"ID "<<id<<" CH "<<ICH<<endl;
  cout << Jsc <<endl;
  cout << Voc <<endl;
  cout << Pm/area*1.0e3 <<endl;
  cout << Vpm <<endl;
  cout << Jpm <<endl;
  cout << FF <<endl;
  cout << Rs*area <<endl;
  cout << Rsh*area <<endl;
  cout << area <<endl;
  cout << eta <<endl;
  cout<< Pin*1000. <<endl;



}


vector<string> split(string&input, char delimiter){

  istringstream stream(input);
  string field;
  vector<string> result;
  while(getline(stream,field,delimiter))
    result.push_back(field);

  return result;
};




double IV(double*x, double*par){


  // par[0] : saturation current density [mA/cm2]
  // par[1] : ideality factor
  double T = 25+273 ; // K
  double q =1.0;
  double I = par[0]*(exp(x[0]*q/(par[1]* kb*T))-1 );
  //  I *= 1.0e3; // A to mA;
  return I;
}

double IV_Solar(double*x, double*par){


  double alpha = 25e-3; // [V] (at R.T. = kT/q)

  double I = par[0]*(exp(x[0]/alpha/par[1]) -1 ) - par[2];

  I *= 1.0e3; // A to mA;

  return I;
}




double IV_diode_LV(double*x,double*par){

  double alpha = 25e-3; // [V] (at R.T. = kT/q)
  double I = par[0]*alpha*x[0]/par[1];

  return I;

}


double IV_diode_HV(double*x,double*par){

  double alpha = 25e-3; // [V] (at R.T. = kT/q)
  double I = par[0]*exp(alpha*x[0]/par[1]);
  return I;

}






