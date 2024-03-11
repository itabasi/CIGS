double ID13_CH2[10]={27.3,27.7,27.9,28.3,28.3,28.4,28.3,28.5,27.9,28.3};
double ID16_CH1[10]={25.8,25.8,25.9,26.0,26.2,26.0,26.1,26.3,25.4,26.2};

void CVdata_0V(){


  TH1D* hID13_CH2 = new TH1D("hID13_CH2","",51,24.95,29.05);
  hID13_CH2->SetLineColor(2);
  TH1D* hID16_CH1 = new TH1D("hID16_CH1","",51,24.95,29.05);
  hID16_CH1->SetLineColor(4);

  double mean[2] ={0.0,0.0};
  double vari[2] ={0.0,0.0};
  for(int i=0; i<10; i++){

    hID13_CH2->Fill(ID13_CH2[i]);
    hID16_CH1->Fill(ID16_CH1[i]);

    mean[0] += ID13_CH2[i]/10.;
    mean[1] += ID16_CH1[i]/10.;

  }

  for(int i=0; i<10; i++){

    vari[0] += pow(ID13_CH2[i] - mean[0],2)/10.;
    vari[1] += pow(ID16_CH1[i] - mean[1],2)/10.;

  }

  


  cout<<"========< ID13 CH2 > ==============="<<endl;
  cout<<"Mean ID13 CH2: "<<mean[0]<<endl;
  cout<<"Variance ID13 CH2: "<<vari[0]<<endl;
  cout<<"========< ID16 CH1 > ==============="<<endl;
  cout<<"Mean ID16 CH1: "<<mean[1]<<endl;
  cout<<"Variance ID16 CH1: "<<vari[1]<<endl;
  

  TCanvas* c0 = new TCanvas("c0","c0");
  c0->cd();
  hID13_CH2->Draw();
  hID16_CH1->Draw("same");


}
