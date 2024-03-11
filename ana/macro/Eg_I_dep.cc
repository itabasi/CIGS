//Correlation of Eregy Gap vs e-h pair creation 




void Eg_I_dep(){

  double Eg[100],I[100];

  Eg[0] = 1.12; I[0] = 3.6; //Silicon
  Eg[1] = 3.3; I[1] = 7.8; // SiC
  Eg[2] = 5.5; I[2] = 13.0; // Diamond
  Eg[3] = 3.39; I[3] = 8.9; // GaN


  TGraph* g = new TGraph();
  g->SetMarkerColor(4);
  g->SetMarkerStyle(4);
  for(int i =0; i<4; i++)
    g->SetPoint(i,Eg[i],I[i]);

  TF1* f =new TF1("f","[0]*x+[1]",0,6);
  g->Fit("f","","",0,6);
  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  g->SetTitle(";Energy Gap [eV] ; Average excited energy [eV]");
  g->Draw("AP");

}
