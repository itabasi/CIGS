#!/bin/csh

set RUNNUM=$argv[1] 

    root -b<<EOF
    .L Read.C+
Read($RUNNUM)
.q
EOF
endif



