#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include "TGraph.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TAxis.h"
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>


extern int Read(int runnum);

extern int setup(int SamplingSpeed, unsigned short WindowSize, int IPaddress, bool version, bool reset, bool RESET_TIAMAT);


