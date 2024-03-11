#include <iostream>


#include "include/setup.h"
//#include "clock_250MHz.h"
#define N 256

int IP_array[10];

int rbcp(const char* sitcpIpAddr, unsigned int sitcpPort, const char* szVerb, const char* szArg1, const char* szArg2, unsigned char* parameter);

int setup(int SamplingSpeed, unsigned short WindowSize, int IPaddress, bool version, bool reset, bool RESET_TIAMAT){

    printf("setup start\n");

    char IP_address_char[256];
    sprintf(IP_address_char, "192.168.10.%d", IPaddress);
    unsigned char parameter[256];
    int err_board=0;


    //CLK.GEN. Setup
    std::string fname_s;
    if(SamplingSpeed == 250){
        fname_s = "./SETUP_FILE/250MHz.txt";
    }
    else if(SamplingSpeed == 160){
        fname_s = "./SETUP_FILE/160MHz.txt";
    }
    else if(SamplingSpeed == 100){
        fname_s = "./SETUP_FILE/100MHz.txt";
    }
    else {
        printf("ERROR: SamplingSpeed (-s) need to be choised in 250, 160, or 100.\n");
        exit(1);
    }

    const char *fname = fname_s.c_str();

    FILE *f_clkgen;
    f_clkgen = fopen(fname, "r");
    if(f_clkgen == NULL){
        printf("ERROR: %s file not open\n", fname);
        exit(1);
    }


    //Another Setup
    unsigned char WindowSize_upper = (unsigned char)(WindowSize >> 8);
    unsigned char WindowSize_lower = (unsigned char)(WindowSize & 0x00ff);

    char WindowSize_upper_c[256];
    char WindowSize_lower_c[256];
    sprintf(WindowSize_upper_c, "%d", WindowSize_upper);
    sprintf(WindowSize_lower_c, "%d", WindowSize_lower);


    if(RESET_TIAMAT){

        //init1_B
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x410", "0x20", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x411", "0x00", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x412", "0x08", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x413", "0xAC", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x414", "0x83", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x10000", "0x01", parameter) == -1){err_board=0; return -1;} usleep(50000);

        //init2_B
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x410", "0x00", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x411", "0x00", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x412", "0x28", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x413", "0xAC", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x414", "0x83", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x10000", "0x01", parameter) == -1){err_board=0; return -1;} usleep(50000);

        //init3_B
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x410", "0x80", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x411", "0x00", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x412", "0x28", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x413", "0xAC", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x414", "0x83", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x10000", "0x01", parameter) == -1){err_board=0; return -1;} usleep(50000);

        //init4_B
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x410", "0x00", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x411", "0x00", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x412", "0x28", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x413", "0xAC", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x414", "0x83", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x10000", "0x01", parameter) == -1){err_board=0; return -1;} usleep(50000);

        //init1_B
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x410", "0x20", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x411", "0x00", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x412", "0x08", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x413", "0xAC", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x414", "0x83", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x10000", "0x01", parameter) == -1){err_board=0; return -1;} usleep(50000);

        //init2_B
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x410", "0x00", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x411", "0x00", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x412", "0x28", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x413", "0xAC", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x414", "0x83", parameter) == -1){err_board=0; return -1;} usleep(5000);
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x10000", "0x01", parameter) == -1){err_board=0; return -1;} usleep(50000);
    }


    if(version == 0){
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x1f", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    }

//	std::cout << "#D TrigMode 0x22 0x0" << std::endl;
//   if(rbcp(IP_address_char,  UDP_port, "wrb", "0x22", "0x0", parameter) == -1){err_board=0; return -1;} usleep(5000);



//CHIP A TAP
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x202", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x204", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x206", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x208", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x20A", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x20C", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x20E", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x210", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x212", "0x03", parameter) == -1){err_board=0; return -1;} usleep(5000);
//CHIP B TAP

    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x214", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x216", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x218", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x21A", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x21C", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x21E", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x220", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x222", "0x01", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x224", "0x03", parameter) == -1){err_board=0; return -1;} usleep(5000);

    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x20", "0x03", parameter) == -1){err_board=0; return -1;} usleep(50000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x17", "0x01", parameter) == -1){err_board=0; return -1;} usleep(50000);
    if(reset){
        if(rbcp(IP_address_char,  UDP_port, "wrb", "0x01", "0xf0", parameter) == -1){err_board=0; return -1;} usleep(5000);
    }        
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x03", WindowSize_upper_c, parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x04", WindowSize_lower_c, parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x12", "0xff", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x13", "0xff", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x14", "0x55", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x15", "0x55", parameter) == -1){err_board=0; return -1;} usleep(5000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x19", "11", parameter) == -1){err_board=0; return -1;} usleep(50000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x02", "0x1", parameter) == -1){err_board=0; return -1;} usleep(50000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x555", "0x02", parameter) == -1){err_board=0; return -1;} usleep(50000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x555", "0x00", parameter) == -1){err_board=0; return -1;} usleep(50000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x555", "0x04", parameter) == -1){err_board=0; return -1;} usleep(50000);
    if(rbcp(IP_address_char,  UDP_port, "wrb", "0x555", "0x00", parameter) == -1){err_board=0; return -1;} usleep(50000);


    return 0;
}

