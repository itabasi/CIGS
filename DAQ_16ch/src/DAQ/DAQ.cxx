#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <stdlib.h>

#include "TSystem.h"
#include "TApplication.h"
#include "TTimer.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH2F.h"
#include "TStyle.h"
#include "include/setup.h"
#include "include/DAQ.h"

#define SERVPORT 24

#define CHIP_NUM 2

//#define DEBUG 

const static option options[] = {
    {"FileName",        required_argument, NULL, 'f'},
    {"EventNum",        required_argument, NULL, 'e'},
    {"WindowSize",      required_argument, NULL, 'w'},
    {"UpdateInterval",  required_argument, NULL, 'u'},
    {"SamplingSpeed",   required_argument, NULL, 's'},
    {"IPaddress",       required_argument, NULL, 'I'},
    {0,0,0,0}
};    

int CLKperiod = 20; //20ns

union data_u{
    unsigned long data_long;
    unsigned int data_int[2];

};


int main(int argc, char* argv[]){

    int ii, index;

    //argument read
    std::string fname;
    bool fname_tag = false;
    int total_event_num = 0;
    unsigned short WindowSize = 1024;
    int UpdateInterval = 100;
    int SamplingSpeed = 250;
    int IPaddress = 16;
    bool version = 1;
    bool reset = true;
    bool RESET_TIAMAT = false;

    while( (ii = getopt_long(argc, argv, "f:e:w:u:s:I:vrR", options, &index)) !=-1 ){
        switch(ii){
            case 'f':
                fname = optarg;
                fname_tag = true;
                break;
            case 'e':
                total_event_num = atoi(optarg);
                break;
            case 'w':
                WindowSize = (unsigned short)atoi(optarg);
                break;
            case 'u':
                UpdateInterval = atoi(optarg);
                break;
            case 's':
                SamplingSpeed = atoi(optarg);
                break;
            case 'I':
                IPaddress = atoi(optarg);
                break;
            case 'v':
                version = 0;
                break;
            case 'r':
                reset = false;
                break;
            case 'R':
                RESET_TIAMAT = true;
                break;
        }
    }    


    //check argument
    //    if(WindowSize > 2048){
    //        printf("ERROR: WindowSize (-w) need to be less than 2048. \n");
    //        exit(1);
    //    }
    //    else if(WindowSize <= 0){
    //        printf("ERROR: WindowSize (-w) need to be larger than 0. \n");
    //        exit(1);
    //    }

    //unsigned long previous_time = 0;

    //setup
    setup(SamplingSpeed, WindowSize, IPaddress, version, reset, RESET_TIAMAT);

    //TApplication* app = new TApplication("app",&argc,argv);
    gApplication = new TApplication("app", &argc, argv);
    gStyle->SetOptStat(0);

	//std::cout << "#D ReturnFromRun : " << gApplication->ReturnFromRun() << std::endl;
	//gApplication->SetReturnFromRun(true);
	//std::cout << "#D ReturnFromRun : " << gApplication->ReturnFromRun() << std::endl;
	//TTimer *fTimer = new TTimer(100);
	//fTimer->SetCommand("gSystem->ProcessEvernt()");
	//fTimer->Start();
    
    int sock;
    struct sockaddr_in servaddr;

    FILE* fp;
    std::string filepath;

	std::string null_c("/dev/null");
	std::string file("../rawdata/");

    if(fname_tag){
      filepath = file + fname;
    }
    else{
        //std::string null_c = "/dev/null";
      filepath = null_c;
    }
    printf("save to %s\n", filepath.c_str());
    fp = fopen(filepath.c_str(), "wb");
    if(fp == NULL){
        fprintf(stderr, "ERROR: err fopen\n");
        exit(1);
    }

    TCanvas* c[CHIP_NUM];
    TGraph* graph[16][CHIP_NUM];
    char c_name[255];
    char graph_name[255];
    TH2F *frame[CHIP_NUM];

    for(int chip = 0; chip < CHIP_NUM; chip++){

        sprintf(c_name, "c_%d", chip);
        c[chip] = new TCanvas(c_name, c_name);
        c[chip]->Divide(4, 2);


        frame[chip] = new TH2F("", "", WindowSize, 0, WindowSize, 200, 400, 600);
        //frame[chip] = new TH2F("", "", WindowSize, 0, WindowSize, 200, 490, 520);
        //frame[chip] = new TH2F("", "", WindowSize, 0, WindowSize, 1000, 0 , 1000);


        for(int i = 0; i < 8; i++){
            c[chip]->cd(i + 1);
            frame[chip]->GetYaxis()->SetLabelSize(0.06);
            frame[chip]->GetXaxis()->SetLabelSize(0.06);
            frame[chip]->GetYaxis()->SetTitleOffset(1.5);
            frame[chip]->Draw();
            sprintf(graph_name, "Graph%d_%d", i, chip);
            graph[i][chip] = new TGraph();
            graph[i][chip]->SetName(graph_name);
        }
    
        c[chip]->Update();
    }


	gSystem->ProcessEvents();
	//std::cout << "#D TApplication befoe RUN" << std::endl;
	//gApplication->Run();
	//std::cout << "#D TApplication after RUN" << std::endl;

    //DAQ
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVPORT);
    
    char IP_address_char[256];
    sprintf(IP_address_char, "192.168.10.%d", IPaddress);

    if(inet_pton(AF_INET, IP_address_char, &servaddr.sin_addr) <= 0){
        fprintf(stderr, "ERROR: inet_pton\n");
        exit(1);
    }

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "ERROR: err socket\n");
        exit(1);
    }

    //increase send buffer
    int sendbuffer_size = 33554432;
    if ( setsockopt (sock, SOL_SOCKET, SO_RCVBUF, &sendbuffer_size, sizeof(sendbuffer_size)) < 0){
        fprintf(stderr, "ERROR: setsockopt failed\n");
        exit(1);
    }

    if(connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        fprintf(stderr, "ERROR: err connetct \n");
        exit(1);
    }

    sleep(1);

    char recvbuff[1024];
    char recv_header[16];
    char recv_footer[4];
    int HEADERSIZE = 16;
    int FOTTERSIZE = 4;

    union Length_f{
        char Length_char[4];
        unsigned Length_int;
    }Length;
    
    //DAQ start

    int data_count = 0;


    #ifdef DEBUG
        unsigned char pre_chip = 0;
        bool first_event = true;
    #endif


    for(;;){

        //header 
        int header_read = 0;
        int n = 0;

        unsigned char header[16];
        while( (n = recv(sock, recv_header + header_read, HEADERSIZE - header_read, 0)) > 0){
            fwrite(recv_header + header_read, 1, n, fp);

            if(data_count % UpdateInterval == 0){
                memcpy(header + header_read, recv_header, n);
            }
            header_read = header_read + n;
            if(header_read == HEADERSIZE)break;
        }
   
        memcpy(&Length.Length_char[0], &recv_header[4], 4); 
        unsigned length2 = ntohl(Length.Length_int);

        
        int chip = (int)recv_header[3];

        #ifdef DEBUG
            if(first_event == true){

                first_event = false;
            }
            else{

                if(chip == pre_chip){
                    printf("ERROR continued chip\n");
                }
            }

            pre_chip = chip;
        #endif

        //read data
        int data_read = 0;
        int data_read_size = 0;
        unsigned char data[32768 * 2 * 8];       //32768(MAX window size) * 2(datasize) * 8(total ch)
        for(;;){
            if((length2 - data_read) < 1024){
                data_read_size = length2 - data_read;
            }
            else{
                data_read_size = 1024;
            }
            while( (n = recv(sock, recvbuff, data_read_size, 0)) > 0){
                fwrite(recvbuff, 1, n, fp);

                if(data_count % UpdateInterval == 0){
                    memcpy(data + data_read, recvbuff, n);
                }
                data_read = data_read + n;

                //change read size
                if((length2 - data_read) <= 0){
                    break;
                }
                else if((length2 - data_read) < 1024){
                    data_read_size = length2 - data_read;
                }
                else{
                    data_read_size = 1024;
                }
            }

            if(data_read ==  (int)length2)break;
        }
        


        //read footer
        int footer_read = 0;
        while( (n = recv(sock, recv_footer + footer_read, FOTTERSIZE - footer_read, 0)) > 0){
            fwrite(recv_footer + footer_read, 1, n, fp);
            footer_read = footer_read + n;
            if(footer_read == FOTTERSIZE)break;
        }

        //make graph
        if((data_count % UpdateInterval) == 0){
            
            unsigned int *Trigger_0 = (unsigned int*)&header[8];
            unsigned int Trigger = ntohl(*Trigger_0);

            #ifdef DEBUG   
                printf("%d \n", recv_header[3]);
            #endif

            sprintf(c_name, "CHIP %d, Trigger %u ", chip, Trigger);
            c[chip]->SetTitle(c_name);
            double waveform[8][32768];
            for(int ch = 0; ch < 8; ch++){
                c[chip]->cd(ch + 1);
                for(int samplenum = 0; samplenum < WindowSize; samplenum++){
                    int position_read = (samplenum * 8 + ch) * 2;
                    short *waveform_f = (short *)&data[position_read];
                    short waveform_ADC = ntohs(*waveform_f);
                    //waveform[ch][samplenum] = ((double)waveform_ADC/2047 * 1000);
                    waveform[ch][samplenum] = waveform_ADC;
                    graph[ch][chip]->SetPoint(samplenum, samplenum, waveform[ch][samplenum]);
                }
                graph[ch][chip]->Draw();
            }
            c[chip]->Update();
		gSystem->ProcessEvents();
                
        }

        //check data_count
        if(total_event_num != 0 && data_count >= total_event_num)break;

        if(chip == 1){
            data_count++;
        }
    }    

    printf("close\n");

    if(close(sock) < 0){
        fprintf(stderr, "ERROR: err close\n");
        exit(1);
    }

    fclose(fp);

    return 0;
}
