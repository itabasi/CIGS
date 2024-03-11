

void CV_pic(){

  //  int runnum =15;
  int runnum =14;
  //  string name = "CIGS15_woGamma";
  string name = Form("CIGS%d_woGamma",runnum);
  string fname =    Form("./%s.txt",name.c_str());
  string frname =   Form("./RootFile/%s.root",name.c_str());
  TFile* f = new TFile(frname.c_str());  

  TGraph* g6 = (TGraph*)f->Get("g6");
  g6->SetName("g6");  
  TGraph* g7 = (TGraph*)f->Get("g7");
  g7->SetName("g7");
  g6->SetTitle("; HV [V]; 1/C^{2} [10^{16} (F/cm^{2})^{-2}");
  g6 -> SetMarkerStyle(20);
  g6 -> SetMarkerColor(4);
  g7 -> SetMarkerStyle(20);
  g7 -> SetMarkerColor(2);
  
  //  string name_g = "CIGS15_wGamma";
  string name_g = Form("CIGS%d_wGamma",runnum);
  string fname_g =    Form("./%s.txt",name_g.c_str());
  string frname_g =   Form("./RootFile/%s.root",name_g.c_str());

  TFile* fg = new TFile(frname_g.c_str());  
  TGraph* g6_g = (TGraph*)fg->Get("g6");
  g6_g->SetName("g6_g");
  TGraph* g7_g = (TGraph*)fg->Get("g7");
  g7_g->SetName("g7_g");

  g6_g -> SetMarkerStyle(2);
  g6_g -> SetMarkerColor(4);
  g7_g -> SetMarkerStyle(2);
  g7_g -> SetMarkerColor(2);


  TLegend*leg = new TLegend(0.15,0.60,0.5,0.90,"","NDC");

  leg->AddEntry(g6, Form("CIGS No%d ch6(before) ",runnum),"p");
  leg->AddEntry(g7, Form("CIGS No%d ch7(before) ",runnum),"p");
  leg->AddEntry(g6_g, Form("CIGS No%d ch6(after) ",runnum),"p");
  leg->AddEntry(g7_g, Form("CIGS No%d ch7(after) ",runnum),"p");


  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  g6->GetYaxis()->SetRangeUser(0,10);
  //  gPad->SetLogy(1);
  gPad->SetGridy(1);
  g6->Draw("AP");
  g7->Draw("P");
  g6_g->Draw("P");
  g7_g->Draw("P");
  leg->Draw();
  
  

}
