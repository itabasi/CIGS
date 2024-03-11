



void VW_sum(){

  TFile* ifr[10];
  string rname[10];

  rname[0] ="./RootFiles/CIGS20_KEK_VW.root";
  rname[1] ="./RootFiles/CIGS20_0h_VW.root";
  rname[2] ="./RootFiles/CIGS20_1h_VW.root";
  rname[3] ="./RootFiles/CIGS20_2h_VW.root";


  TGraphErrors* gCH6[10];
  int color[10];
  color[0] = 1;
  color[1] = 2;
  color[2] = 4;
  color[3] = 8;
  
  for(int i = 0; i < 4; i++){
    ifr[i] = new TFile(rname[i].c_str());
    gCH6[i] = (TGraphErrors*)ifr[i]->Get("g6");
    gCH6[i] -> SetName(Form("gCH6_%d",i));
    gCH6[i] -> SetMarkerColor(color[i]);
    gCH6[i] -> SetMarkerStyle(40);
    gCH6[i] -> SetLineColor(color[i]);
    gCH6[i] -> SetLineWidth(2);
  }


  TLegend*leg = new TLegend(0.15,0.60,0.5,0.90,"","NDC");
  string a[10];
  a[0] ="Before irradiation";
  a[1] ="After irradiation";
  //  a[2] ="Annealing 1h (130 ^{#circ}C)";
  //  a[3] ="Annealing 2h (130 ^{#circ}C)";

  TCanvas* c0 = new TCanvas("c0","c0");
  
  c0->cd();
  gCH6[0]->Draw("AL");
  for(int i = 0; i <2 ; i++){
    leg->AddEntry(gCH6[i], a[i].c_str(),"l");
    gCH6[i]->Draw("L");

  }
      leg->Draw();
  
}
