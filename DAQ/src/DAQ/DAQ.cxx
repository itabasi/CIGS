#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <stdlib.h>
#include "TCanvas.h"
#include "TApplication.h"
#include "TGraph.h"
#include "TH2F.h"
#include "TStyle.h"
#include "include/setup.h"
#include "include/DAQ.h"
#include "include/rdf2root.h"
using namespace std;
#define SERVPORT 24

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
    int IPaddress=16;
    bool version=1;
    bool reset=true;
    bool RESET_TIAMAT = false;
    int runnum=0;
    while( (ii = getopt_long(argc, argv, "f:e:w:u:s:I:vrR", options, &index)) !=-1 ){
        switch(ii){
            case 'f':
                fname = optarg;
		runnum = atoi(fname.c_str());
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

    unsigned long previous_time = 0;

    //setup
    setup(SamplingSpeed, WindowSize, IPaddress, version, reset, RESET_TIAMAT);

    TApplication* app = new TApplication("app",&argc,argv);
    gStyle->SetOptStat(0);
    
    int sock;
    struct sockaddr_in servaddr;

    FILE* fp;
    std::string filepath;
    std::string file("../rawdata/");
    std::string null_c("/dev/null");
    cout<<"fname_tag "<<fname_tag<<endl;
    if(fname_tag){
      filepath = Form("../rawdata/run_%03d.dat",runnum);
      //      std::cout<<"file "<<file.c_str()<<" fname "<<fname.c_str()<<" runnum "<<runnum<<" file path "<<filepath.c_str()<<std::endl;
      //filepath = file + fname ;
    }
    else{
        filepath = null_c.c_str();
    }
    printf("save to %s\n", filepath.c_str());
    fp = fopen(filepath.c_str(), "wb");
    if(fp == NULL){
        fprintf(stderr, "ERROR: err fopen\n");
        exit(1);
    }

    TCanvas* c = new TCanvas("graph", "", 100, 100, 1000, 700);
    c->Divide(4, 2);
    TGraph* graph[16];
    char c_name[255];
    sprintf(c_name, "CAVALIER");

    char graph_name[255];
    TH2F *frame = new TH2F("", "", WindowSize, 0, WindowSize, 500, 250, 750);
    //TH2F *frame = new TH2F("", "", 200, 400, 600, 200, 400, 600);
    //TH2F *frame = new TH2F("", "", WindowSize, 0, WindowSize, 1024, 0, 1024);
    //TH2F *frame = new TH2F("", "", WindowSize, 0, 100, 1024, 0, 1024);
    //TH2F *frame = new TH2F("", "", WindowSize, 634, 644, 2000, -1000, 1000);
    for(int i = 0; i < 8; i++){
        c->cd(i + 1);
        graph[i] = new TGraph();
        frame->GetYaxis()->SetLabelSize(0.06);
        frame->GetXaxis()->SetLabelSize(0.06);
        frame->GetYaxis()->SetTitleOffset(1.5);
        frame->Draw();
        sprintf(graph_name, "Graph%d", i);
        graph[i] = new TGraph();
        graph[i]->SetName(graph_name);

    }

    c->Update();

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
                       

            sprintf(c_name, "Trigger %u ", Trigger);
            c->SetTitle(c_name);
            double waveform[8][32768];
            for(int ch = 0; ch < 8; ch++){
                c->cd(ch + 1);
                for(int samplenum = 0; samplenum < WindowSize; samplenum++){
                    int position_read = (samplenum * 8 + ch) * 2;
                    short *waveform_f = (short *)&data[position_read];
                    short waveform_ADC = ntohs(*waveform_f);
                    //waveform[ch][samplenum] = ((double)waveform_ADC/2047 * 1000);
                    waveform[ch][samplenum] = waveform_ADC;
                    graph[ch]->SetPoint(samplenum, samplenum, waveform[ch][samplenum]);
                }
                graph[ch]->Draw();
            }
            c->Update();
                
        }

        //check data_count
        if(total_event_num != 0 && data_count >= total_event_num)break;
        data_count++;
    }    

    printf("close\n");


    // ============= rdf2root  ==================== //

    //    Read(runnum);
    
    if(close(sock) < 0){
        fprintf(stderr, "ERROR: err close\n");
        exit(1);
    }

    fclose(fp);

    return 0;
}
