#!/bin/csh

set RUNNUM=$argv[1] 

    root -b<<EOF
    .L rdf2root.C+
Read($RUNNUM)
.q
EOF
endif



