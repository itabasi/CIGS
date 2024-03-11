#include "TTree.h"
#include "TFile.h"
#include <arpa/inet.h>

int Read(int RUNNUM=0){

  int HEADERSIZE = 16;
  int FOOTERSIZE = 4;
  int CHNUM = 8;
  int SAMPLEMAX = 32768;

  /*
  unsigned char header[HEADERSIZE];
  unsigned char readdata[CHNUM * SAMPLEMAX*2];
  unsigned char footer[FOOTERSIZE];
  unsigned int readnum = 1;
  unsigned short waveform[CHNUM][SAMPLEMAX];
  */
  unsigned char header[16];
  unsigned char readdata[8 * 32768*2];
  unsigned char footer[4];
  unsigned int readnum = 1;
  unsigned short waveform[8][32768];

  int adc_h[8],adc_l[8];
  
  int EventNo = 0;
  //  unsigned short adc[8][1024] = {{0},{0}};
  unsigned short adc[8][100] = {{0},{0}};
  //  int adc[8][1024] = {{0},{0}};
  float Pedestal[8] = {0};
  //  float Pedestal[8] = {505.5, 505.3, 509.6, 507.4, 504.7, 504.1, 504.3, 509.5};
  float PeakHeight[8] = {0};
  for(int i=0;i<8;i++){
    adc_l[i] = 4096;
    adc_h[i] = 0;
    PeakHeight[i] = 4096;
  }
  
  float PeakTime[8] = {0};
  float CFTime[8] = {0};
  float IntegratedADC[8] = {0};
  int sign[8] = {0};
  
  TFile *hfile = new TFile(Form("RootFile/run_%d.root",RUNNUM),"recreate");
  TTree *tree=new TTree("T","");
  tree->Branch("EventNo",&EventNo,"EventNo/I");
  tree->Branch("adc",&adc,"adc[8][100]/s");  
  tree->Branch("Pedestal",&Pedestal,"Pedestal[8]/F");
  tree->Branch("PeakHeight",&PeakHeight,"PeakHeight[8]/F");
  tree->Branch("PeakTime",&PeakTime,"PeakTime[8]/F");
  tree->Branch("CFTime",&CFTime,"CFTime[8]/F");
  tree->Branch("IntegratedADC",&IntegratedADC,"IntegratedADC[8]/F");
  FILE *fp;
  //  fp = fopen("data_gan.dat","rt");
  //  fp = fopen("data_si_1.dat","rt");
  fp = fopen(Form("run_%03d.dat",RUNNUM),"rt");

  
  while(readnum>0){

    //Header read
    readnum = fread(header, 1, HEADERSIZE, fp);
    if(readnum != (unsigned int)HEADERSIZE) continue;
    
    unsigned int *datalength_f = (unsigned int *)&header[4];
    unsigned int datalength = ntohl(*datalength_f);
    unsigned int *eventnumber_f = (unsigned int *)&header[8];
    unsigned int get_eventnumber = ntohl(*eventnumber_f);
    unsigned char tag = (unsigned int)header[15];
    int sample_allnum = datalength / 2 / CHNUM;
    
    //check
    if(header[0] != 0xff || header[1] != 0xff || header[2] != 0x55 || header[3] != 0x55){
      printf("Header Magic Word uncorrected\n");
      return -1;
    }

    //data read
    readnum = fread(readdata, 1, datalength, fp);
    if(readnum != datalength) continue;

    //    printf("All Sample Num = %d\n",sample_allnum);
    for(int samplenum = 0; samplenum < sample_allnum; samplenum++){
      for(int ch = 0; ch < 8; ch++){
	int position_read = (samplenum * 8 + ch) * 2;
	short *waveform_f = (short *)&readdata[position_read];
	short waveform_ADC = ntohs(*waveform_f);
	//	waveform[ch][samplenum] = waveform_ADC;
	adc[ch][samplenum] = waveform_ADC;
	//	if(ch==0)printf("CH=%d Sample=%d ADC=%d\n",ch,samplenum,waveform_ADC);
	//fprintf(FP_out[count_event][ch], "%u \r\n", waveform[ch][samplenum]);    
	//	graph[ch]->SetPoint(samplenum, samplenum,  waveform[ch][samplenum]);
      }
    }
    
    //Footer read
    readnum = fread(footer, 1, FOOTERSIZE, fp);
    if(readnum != (unsigned int)FOOTERSIZE) continue;

    
    for(int i=0;i<8;i++){
      for(int j=0;j<100;j++){
	//	if(j< 20 || 60<=j)
	if(j< 24 || (64 <= j)){
	Pedestal[i] += adc[i][j] / 60.;
	}
      }
    }


    // ======== Set Sign ============ //
    // if peak height is positive : sign =+1
    // if peak height is negative : sign =-1
    if(EventNo==0){
      double adc_low[8] = {2000};
      double adc_high[8] = {0};
      for(int i=0;i<8;i++){
	for(int j =0; j<100;j++){
	  if(adc_low[i]  > adc[i][j]) adc_low[i] = adc[i][j];
	  if(adc_high[i] < adc[i][j]) adc_high[i] = adc[i][j];
	}
	if(fabs(adc_low[i]-Pedestal[i]) < fabs(adc_high[i]-Pedestal[i]))
	  sign[i] = 1;
	else
	  sign[i] =-1;
      }
    }
    // ================================= //

    
    for(int i=0;i<8;i++){
      for(int j=0;j<100;j++){
	if( (sign[i] >0  && PeakHeight[i] > adc[i][j])
	    || (sign[i] <0 && PeakHeight[i] < adc[i][j]) ){
	    PeakHeight[i] = adc[i][j];
	    PeakTime[i] = j;
	  }
      }
    }
  


    
    for(int i=0;i<8;i++){
      for(int j=20;j<60;j++){
	IntegratedADC[i] += sign[i]*(adc[i][j] - Pedestal[i]);
      }
    }
    

    /*
    for(int i=0;i<8;i++){
      double pmax = Pedestal[i];
      int    pmax_i = 0;
      for(int j=30;j<50;j++){
	if(adc[i][j] < pmax ) pmax_i = j;
      }
      for(int j=pmax_i;30<=j ;j--){
	if(adc[i][j] < Pedestal[i])
	  IntegratedADC[i] += -(adc[i][j] - Pedestal[i]);	  
      }
      for(int j=pmax_i;j<=50 ;j++){
	if(adc[i][j] < Pedestal[i])
	  IntegratedADC[i] += -(adc[i][j] - Pedestal[i]);	  
      }
    }
   */
     


    
    
    tree -> Fill();

    if(EventNo%100==0) printf("EvetnNo=%d\n",EventNo);
    EventNo++;

    for(int i=0;i<8;i++){ 
      Pedestal[i] = 0;
      //      PeakHeight[i] = 4096;
      if(sign[i] >0 ) PeakHeight[i] = 0;
      else            PeakHeight[i] = 4096;
      PeakTime[i] = 0;
      IntegratedADC[i] = 0;
   }    

  }

  fclose(fp);

  hfile -> cd(); 
  tree -> Write();
  hfile -> Close();

  return 0;
}
