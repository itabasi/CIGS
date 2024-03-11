#!/bin/csh

set RUNNUM=$argv[1] 
set Sign=$argv[2]


if($Sign>0) then
    root -b<<EOF
    .L ReadPosi.C+
Read($RUNNUM)
.q
EOF
else 
    root -b<<EOF
    .L ReadNega.C+
Read($RUNNUM)
.q
EOF
endif



