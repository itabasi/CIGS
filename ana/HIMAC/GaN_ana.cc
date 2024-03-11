
const int nfile =6;
const int nch =16;
void GaN_ana(){


  string rname[nfile];
  rname[0] = "./run_30.root";
  rname[1] = "./run_31.root";
  rname[2] = "./run_56.root";
  rname[3] = "./run_57.root";
  rname[4] = "./run_58.root";
  rname[5] = "./run_59.root";

  string temp[nfile];
  temp[0]="GaN (run30): HV=4.6V (#theta =0^{#circ})";
  temp[1]="GaN (run31): HV=5.5V (#theta =0^{#circ})";
  temp[2]="GaN (run56): HV=5.6V (#theta =0^{#circ})";
  temp[3]="GaN (run57): HV=4.6V (#theta =0^{#circ})";
  temp[4]="GaN (run58): HV=2.6V (#theta =0^{#circ})";
  temp[5]="GaN (run59): HV=5.6V (#theta =45^{#circ})";

  string  temp_ratio[nfile];
  temp_ratio[0] ="GaN ADC Ratio :ADC(HV= 4.6V)/ADC(HV=2.6V)";
  temp_ratio[1] ="GaN ADC Ratio :ADC(HV= 5.5V)/ADC(HV=2.6V)";
  temp_ratio[2] ="GaN ADC Ratio :ADC(HV= 5.6V)/ADC(HV=2.6V)";
  temp_ratio[3] ="GaN ADC Ratio :ADC(HV= 4.6V)/ADC(HV=2.6V)";  
  temp_ratio[4] ="GaN ADC Ratio :ADC(HV= 2.6V)/ADC(HV=2.6V)";
  temp_ratio[5] ="GaN ADC Ratio :ADC(HV= 5.6V)/ADC(HV=2.6V)";

  TFile* ifr[nfile];
  TGraphErrors* gADC[nfile];
  TGraphErrors* gADC_Ratio = new TGraphErrors();
  TGraphErrors* gADC_ratio[nfile];

  TGraphErrors* gADC_frame = new TGraphErrors();
  gADC_frame->SetPoint(1,0,0.0);
  gADC_frame->SetPoint(2,17,500.0);
  gADC_frame->SetTitle(";Channel ; mean [ADC]");

  TGraphErrors* gADC_ratio_frame = new TGraphErrors();
  gADC_ratio_frame->SetPoint(1,0,0.5);
  gADC_ratio_frame->SetPoint(2,17,2.0);
  gADC_ratio_frame->SetTitle(";Channel ; ADC Ratio");
  
  for(int i=0; i<nfile; i++){

    
    ifr[i] = new TFile(rname[i].c_str());

    gADC[i] = (TGraphErrors*)ifr[i] ->Get("gADC");
    gADC[i] ->SetName(Form("gADC_%d",i));
    gADC[i] ->SetMarkerColor(i+1);
    gADC[i] ->SetMarkerStyle(20);

    gADC_ratio[i] = new TGraphErrors();
    gADC_ratio[i] ->SetName(Form("gADC_ratio_%d",i));
    gADC_ratio[i] ->SetTitle(";channel ; Ratio");
    gADC_ratio[i] ->SetMarkerColor(i+1);
    gADC_ratio[i] ->SetMarkerStyle(20);        
    
  }

  for(int i=0;i<nch;i++){

    double y0 = gADC[2]->GetPointY(i);
    double y1 = gADC[5]->GetPointY(i);
    double y0_err = gADC[2]->GetErrorY(i);
    double y1_err = gADC[5]->GetErrorY(i);

    gADC_Ratio->SetPoint(i,i,y1/y0);
    gADC_Ratio->SetPointError(i,0.0,(y1/y0)*(y1_err/y1 + y0_err/y0));

    
    double yy0 = gADC[4]->GetPointY(i);
    double yy0_err = gADC[4]->GetErrorY(i);

    for(int j=0;j<nfile;j++){    
    double yy1 = gADC[j]->GetPointY(i);
    double yy1_err = gADC[j]->GetErrorY(i);

    gADC_ratio[j]->SetPoint(i,i,yy1/yy0);
    gADC_ratio[j]->SetPointError(i,0.0,(yy1/yy0)*(yy1_err/yy1 + yy0_err/yy0));

    }
    
  }

  TLegend*leg = new TLegend(0.15,0.60,0.5,0.90,"","NDC");

  
  
  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  gADC_frame->SetTitle(";Channel ; mean [ADC]");
  gADC_frame->Draw("AP");
  for(int i = 0 ; i< nfile; i++){
    //    if(i==2 || i==3 || i==4){
    if(i==2 || i==5){
      leg->AddEntry(gADC[i], temp[i].c_str(),"p");
      gADC[i]->Draw("P");
    }
  }

  leg->Draw();

  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  gADC_Ratio->SetTitle(";Channel ; Ratio (ADC_{45}/ADC_{0})");
  gADC_Ratio->Draw("AP");

  TLine* line = new TLine(0.0,1.4,17,1.4);
  line->SetLineColor(2);
  line->SetLineWidth(2);
  line->SetLineStyle(2);

  line->Draw("same");



  TCanvas* c2 = new TCanvas("c2","c2");
  c2->cd();
  TLegend*leg2 = new TLegend(0.15,0.60,0.5,0.90,"","NDC");
  gADC_ratio_frame->Draw("AP");
  for(int i = 0 ; i< nfile; i++){
    if(i ==2 || i==3){
    leg2->AddEntry(gADC_ratio[i], temp_ratio[i].c_str(),"p");
    gADC_ratio[i]->Draw("P");
    }
  }

  leg2->Draw();

  
}
