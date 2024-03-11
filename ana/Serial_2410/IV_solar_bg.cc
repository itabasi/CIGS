const  string fname ="./data/20230501_14:29:20.txt";
const  string fname_dark ="./data/20230501_14:34:54.txt";
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
      gW->SetPoint(ii,HV[ii] + HV_OFF,HV[ii]*(-I[ii]));
      ii++;

    }
  }


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
  double Pin = 0.100;//[W/cm^2]
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


  // ======= Set IV data @AIST ========== //
  string IV_AIST = "solar_IV_AIST.txt";
  ifstream ifp_AIST(IV_AIST.c_str(),ios::in);

  int iiii=0;
  double V[300];
  int nid =4;
  int nch =2;
  double J[nid][nch][2][300];
  TGraphErrors* gIV_AIST[nid][nch][2];
  TGraphErrors* gIV_AIST_abs[nid][nch][2];
  for(int iid=0;iid<nid;iid++){
    for(int ich =0; ich<nch;ich++){
      for(int i=0; i<2; i++){
	gIV_AIST[iid][ich][i]= new TGraphErrors();
	gIV_AIST[iid][ich][i] -> SetName(Form("gIV_AIST_%d_%d_%d",iid,ich,i));
	gIV_AIST[iid][ich][i] -> SetTitle(Form("ID%d CH%d I-V curve @AIST",iid+13,ich+1));
	gIV_AIST[iid][ich][i] -> SetMarkerStyle(20);
	gIV_AIST[iid][ich][i] -> SetMarkerSize(0.4);
	gIV_AIST[iid][ich][i] -> SetMarkerColor(kRainBow);
	gIV_AIST_abs[iid][ich][i]= new TGraphErrors();
	gIV_AIST_abs[iid][ich][i] -> SetName(Form("gIV_AIST_abs_%d_%d_%d",iid+13,ich,i));
	gIV_AIST_abs[iid][ich][i] -> SetTitle(Form("ID%d CH%d I-V curve @AIST",iid+13,ich+1));
	gIV_AIST_abs[iid][ich][i] -> SetMarkerStyle(20);
	gIV_AIST_abs[iid][ich][i] -> SetMarkerSize(0.4);
	gIV_AIST_abs[iid][ich][i] -> SetMarkerColor(kRainBow);	
      }
    }
  }

  
  while(1){
    getline(ifp_AIST,buf);
    if(ifp_AIST.eof() || buf.size()==0)break;
    strvec = split(buf,',');
    if(strvec.size()>0){
      V[iiii] = stof(strvec.at(0));
      for(int i=0;i<nid*nch*2;i++){
	J[i/nid][(i%(nch*2))/2][i%2][iiii] = stof(strvec.at(1+3*i));
	gIV_AIST[i/nid][(i%(nch*2))/2][i%2] -> SetPoint(iiii,V[iiii], J[i/nid][(i%(nch*2))/2][i%2][iiii]);
	gIV_AIST_abs[i/nid][(i%(nch*2))/2][i%2] -> SetPoint(iiii,V[iiii], fabs(J[i/nid][(i%(nch*2))/2][i%2][iiii]));
      }
      iiii++;
    }
  }



  // ========= <Relative Error> =================== //
  int ch = 0;
  double xx,yy;
  double xx_d,yy_d;
  int xi=0;
  double xxx,yyy,xxx2,yyy2,yyy3;
  double xxx_d,yyy_d,xxx2_d,yyy2_d,yyy3_d;
  TGraphErrors* gIV_Solar_R = new TGraphErrors();
  gIV_Solar_R->SetTitle("CIGS with Solar Relative Error [%];HV [V]; Relative Error [%]");
  gIV_Solar_R->SetMarkerStyle(20);
  gIV_Solar_R->SetMarkerColor(4);
  gIV_Solar_R->SetLineColor(4);
  TGraphErrors* gIV_Dark_R = new TGraphErrors();
  gIV_Dark_R->SetMarkerStyle(20);
  gIV_Dark_R->SetMarkerColor(2);
  gIV_Dark_R->SetLineColor(2);

  TGraphErrors* gIV_Solar_Diff = new TGraphErrors();
  gIV_Solar_Diff->SetTitle("CIGS with Solar Diff ;HV [V]; Diff [mA]");
  gIV_Solar_Diff->SetMarkerStyle(20);
  gIV_Solar_Diff->SetMarkerColor(4);
  gIV_Solar_Diff->SetLineColor(4);
  
  for(int i=0;i<iiii;i++){
    gIV_AIST[id-13][0][0] ->GetPoint(i,xx,yy);
    gmA->GetPoint(xi,xxx,yyy);
    gmA->GetPoint(xi+1,xxx2,yyy2);

    gIV_AIST[id-13][0][1] ->GetPoint(i,xx_d,yy_d);
    gmA_Dark->GetPoint(xi,xxx_d,yyy_d);
    gmA_Dark->GetPoint(xi+1,xxx2_d,yyy2_d);
    if(xxx <= xx && xx <= xxx2){
      double a = (xxx2-xxx)/(yyy2 -yyy);
      yyy3 = a*xx + (yyy - a* xxx);

      double a_d =(xxx2_d-xxx_d)/ (yyy2_d -yyy_d);
      yyy3_d = a_d*xx_d + (yyy_d - a_d* xxx_d);      
    }else if(xx < xxx){
      yyy3 = yyy;
    }else{
      xi++;
      gmA->GetPoint(xi,xxx,yyy);
      gmA->GetPoint(xi+1,xxx2,yyy2);
      double a = (xxx2-xxx)/(yyy2 -yyy);
      yyy3 = a*xx + (yyy - a* xxx);
      gmA_Dark->GetPoint(xi,xxx_d,yyy_d);
      gmA_Dark->GetPoint(xi+1,xxx2_d,yyy2_d);
      double a_d =(xxx2_d-xxx_d)/ (yyy2_d -yyy_d);
      yyy3_d = a_d*xx_d + (yyy_d - a_d* xxx_d);      
    }

    gIV_Solar_R->SetPoint(i,xx,(yyy3-yy)/yy*100.);
    gIV_Solar_Diff->SetPoint(i,xx,(yyy3-yy));
    gIV_Dark_R->SetPoint(i,xx_d,(yyy3_d-yy_d)/yy_d*100.);
  }

  
  TF1* fI = new TF1("fI",IV,-2,0,2);
  fI ->SetParameter(0,1);
  fI ->SetParameter(1,1);
  //  fI ->FixParameter(1,1);
  //  gmA_Dark->Fit("fI","","",-1,-0.1);
  gDark->Fit("fI","","",0.5,0.8);
  ///gIV_AIST[id-13][ICH-1][1]->Fit("fI","","",-1,-0.1);
  //  gIV_AIST[id-13][ICH-1][1]->Fit("fI","","",0.6,0.8);
  double Jo = fI->GetParameter(0);
  double n  = fI->GetParameter(1);

  /*  
      TF1*fIL = new TF1("fIL",IV_diode_LV,-2,2,2);
      fIL->SetParameters(Jo,n);
      fIL->SetLineColor(5);
      gDark->Fit("fIL","QR","QR",-0.1,0.2);
      Jo = fIL->GetParameter(0);
      n  = fIL->GetParameter(1);
      cout<<"Fitted by LV Jo "<<Jo<<" n "<<n<<endl;


      TF1*fIH = new TF1("fIH",IV_diode_HV,-2,2,2);
      fIH->SetParameters(Jo,n);
      fIH->SetLineColor(4);
      gDark->Fit("fIH","QR","QR",0.7,0.9);
      Jo = fIH->GetParameter(0);
      n  = fIH->GetParameter(1);
      cout<<"Fitted by LV Jo "<<Jo<<" n "<<n<<endl;
  */
  

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
  leg1->AddEntry(gIV_AIST[id-13][0][0],Form( "CIGS w/ Solar light CH%d ",0),"pl");
  leg1->AddEntry(gIV_AIST[id-13][1][0],Form( "CIGS w/ Solar light CH%d ",1),"pl");
  leg1->AddEntry(gmA_Dark, "CIGS Dark  current","pl");
  leg1->AddEntry(gIV_AIST[id-13][0][1],Form( "CIGS Dark current CH%d ",0),"pl");
  leg1->AddEntry(gIV_AIST[id-13][1][1],Form( "CIGS Dark current CH%d ",1),"pl");

  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  gmA->Draw("APL");  
  gIV_AIST[id-13][0][0]->Draw("PLC PMC P");
  gIV_AIST[id-13][1][0]->Draw("PLC PMC P");
  gIV_AIST[id-13][0][1]->Draw("PLC PMC P");
  gIV_AIST[id-13][1][1]->Draw("PLC PMC P");
  gmA_Dark->Draw("PL");
  leg1->Draw();
  TCanvas* c2 = new TCanvas("c2","c2");
  c2->cd();
  gA->Draw("APL");
  fRs->Draw("same");
  fRsh->Draw("same");
  gW->Draw("PL");

  TCanvas* c3 = new TCanvas("c3","c3");
  c3->cd();
  c3->SetGrid();
  gIV_Solar_R->GetYaxis()->SetRangeUser(-5,5);
  gIV_Solar_R->Draw("APL");

  TCanvas* c4 = new TCanvas("c4","c4");
  c4->cd();
  c4->SetGrid();
  gIV_Solar_Diff->GetYaxis()->SetRangeUser(-5,5);
  gIV_Solar_Diff->Draw("APL");
  
  
  TCanvas* c5 = new TCanvas("c5","c5");
  c5->cd();
  c5->SetGrid();
  TLegend*leg5 = new TLegend(0.15,0.20,0.5,0.50,"","NDC");
  leg5->AddEntry(gmA,Form( "CIGS ID%d CH%d  @KEK ",id,ICH),"pl");
  leg5->AddEntry(gIV_AIST[id-13][ICH-1][0],Form( "CIGS ID%d CH%d @AIST",id,ICH),"pl");
  gmA->Draw("APL");  
  gIV_AIST[id-13][ICH-1][0]->Draw("PLC PMC P");
  leg5->Draw();
  
  TCanvas* c6 = new TCanvas("c6","c6");
  c6->cd();
  c6->SetGrid();
  gPad->SetLogy(1);
  gIV_AIST_abs[id-13][ICH-1][0]->Draw("PLC PMC APL");
  gmA_abs->Draw("PL");
  leg5->Draw();
  //  gIV_AIST_abs[id-13][ICH-1][1]->Draw("PLC PMC AP");


  TLegend*leg7 = new TLegend(0.15,0.20,0.5,0.50,"","NDC");
  leg7->AddEntry(gA,Form( "CIGS ID%d CH%d  IV-curve @KEK ",id,ICH),"pl");
  leg7->AddEntry(fRs2,Form( "linear fit  Rs=%1.2f #Omega cm^{2}",Rs*area),"pl");
  leg7->AddEntry(fRsh,Form( "Fitting Result of Rh=%4.1f #Omega cm^{2}",Rsh*area),"pl");

  TCanvas* c7 = new TCanvas("c7","Check Rs & Rsh Fitting");
  c7->cd();
  c7->SetGrid();
  fRs2->SetTitle("I-V Curve; Bias Voltage [V] ;Current [A] ");
  fRs2->GetXaxis()->SetRangeUser(0,1.);
  fRs2->GetYaxis()->SetRangeUser(-0.015,0.03);
  fRs2->Draw("");
  fRsh->Draw("same");
  gA->Draw("PL");
  leg7->Draw();

  TCanvas* c8 = new TCanvas("c8","");
  c8->cd();
  TLegend*leg8 = new TLegend(0.15,0.20,0.5,0.50,"","NDC");
  leg8->AddEntry(gmA_Dark_abs,Form( "CIGS ID%d CH%d Dark  @KEK ",id,ICH),"pl");
  leg8->AddEntry(gIV_AIST_abs[id-13][ICH-1][1],Form( "CIGS ID%d CH%d  Dark @AIST",id,ICH),"pl");
  c8->SetGrid();
  gPad->SetLogy(1);
  gIV_AIST_abs[id-13][ICH-1][1]->GetYaxis()->SetRangeUser(1.e-5,1.e4);
  gIV_AIST_abs[id-13][ICH-1][1]->Draw("PLC PMC APL");
  gmA_Dark_abs->Draw("PL");
  fI->Draw("same");
  leg8->Draw();

  TCanvas* c9 = new TCanvas("c9","");
  c9->cd();
  gDark->Draw();
  fI->Draw("same");
  


  
  cout<<"======== < COMMENT OUT > =============="<<endl;
  cout<<"Jsc "<<Jsc <<" mA "<<endl;
  cout<<"Voc "<<Voc<<" V "<<endl;
  cout<<"Pm "<<Pm*1.e3/area<<" mW/cm2 "<<endl;
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




  // ======== Print out ========== //
  string ofpname =Form("./pdf/IV_pic_ID%d_CH%d.pdf",id,ICH);
  c5->Print(Form("%s[",ofpname.c_str()));
  c5->Print(Form("%s",ofpname.c_str()));
  c6->Print(Form("%s",ofpname.c_str()));
  c4->Print(Form("%s",ofpname.c_str()));
  c3->Print(Form("%s",ofpname.c_str()));
  c7->Print(Form("%s",ofpname.c_str()));
  c8->Print(Form("%s",ofpname.c_str()));
  c8->Print(Form("%s]",ofpname.c_str()));

  ofstream ofp("test00.txt");
  ofp<<" ID "<<id<<" ch "<<ICH<<endl;
  ofp << Jsc <<endl;
  ofp << Voc <<endl;
  ofp << Pm/area*1.0e3 <<endl;
  ofp << Vpm <<endl;
  ofp << Jpm <<endl;
  ofp << FF <<endl;
  ofp << Rs*area <<endl;
  ofp << Rsh*area <<endl;
  ofp << area <<endl;
  ofp << eta <<endl;
  ofp<< Pin*1000. <<endl;

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






