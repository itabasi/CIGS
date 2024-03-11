



void CV_sum(){

  TFile* ifr[10];
  string rname[10];

  rname[0] ="./RootFiles/CIGS20_0h_CH6.root";
  rname[1] ="./RootFiles/CIGS20_1h_CH6.root";
  rname[2] ="./RootFiles/CIGS20_2h_CH6.root";
  rname[3] ="./RootFiles/CIGS20_KEK.root";

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
  }



  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  gCH6[0]->Draw("AP");
  for(int i = 0; i <4 ; i++){
    gCH6[i]->Draw("P");

  }

  
}
