#!/bin/bash
FILE=data/`date '+%Y%m%d_%T'`.txt
echo "Saving in $FILE"
touch $FILE
PID=0
nsample=20
VOFF=-1.65
VMIN=-3
VMAX=50
vol=0


echo "HV ON "
echo ":OUTPUT ON" >> /dev/ttyUSB0
## HV OFF ###

sign=1.0
step=5
zero=0.0
if [ `echo "$VMIN > $zero" |bc` -eq 1 ]; then
    sign=+1.0
    while [ `echo "( $VMIN - ($vol) + ($VOFF)) >= $step" |bc` -eq 1 ]
    do
	vol=`echo "scale=3; $vol $sign*$step" |bc | xargs printf "%.3f\n"`
        echo ":SOURCE:VOLTAGE:LEVEL $vol" >> /dev/ttyUSB0
	echo "SOURCE:VOLTAGE:LEVEL $vol "
	sleep 0.2
    done
else
    sign=-1.0
        while [ `echo "( -( $VMIN) + $vol + ($VOFF)) >= $step" |bc` -eq 1 ]
	do
	    vol=`echo "scale=3; $vol + ($sign)*$step" |bc | xargs printf "%.3f\n"`
	    echo ":SOURCE:VOLTAGE:LEVEL $vol" >> /dev/ttyUSB0
	    echo "SOURCE:VOLTAGE:LEVEL $vol "
	    sleep 0.2
	done
fi







for ((i=0; i<$nsample; i++)); do
    vol=`echo "scale=3;$VMIN+(($VMAX-($VMIN))/($nsample-1.)*$i) -($VOFF)" |bc | xargs printf "%.3f\n"`
    echo "Set V = $vol ($i/$nsample)"
    echo ":SOURCE:VOLTAGE:LEVEL $vol" >> /dev/ttyUSB0
#    sleep 2
    echo `date +%s` >> $FILE
    cat /dev/ttyUSB0 >> $FILE &
    PID=$!
    echo ":READ?" >> /dev/ttyUSB0 
#    sleep 10
    sleep 2
    kill $PID
    cat  $FILE

done


## HV OFF ###

if [ `echo "$vol > $zero" |bc` -eq 1 ]; then
    sign=-1
    while [ `echo "($vol - $step) >= $step" |bc` -eq 1 ]
    do
	vol=`echo "scale=3;$vol $sign*($step)" |bc | xargs printf "%.3f\n"`
	echo ":SOURCE:VOLTAGE:LEVEL $vol" >> /dev/ttyUSB0
	echo "SOURCE:VOLTAGE:LEVEL $vol"
	sleep 0.2
    done
else
    sign=+1
    while [ `echo "($vol - $step) >= $step" |bc` -eq 1 ]
    do
	vol=`echo "scale=3;$vol  $sign*($step)" |bc | xargs printf "%.3f\n"`
	echo ":SOURCE:VOLTAGE:LEVEL $vol" >> /dev/ttyUSB0
	echo "SOURCE:VOLTAGE:LEVEL $vol"
	sleep 0.2
    done
fi




echo "HV OFF "
echo ":SOURCE:VOLTAGE:LEVEL 0" >> /dev/ttyUSB0
echo ":OUTPUT OFF" >> /dev/ttyUSB0

echo `cp $FILE data/tmp.txt`

echo "========================================"
echo "===== < COMPLEATED IV MEASUREMENT > ===="
echo "========================================"
echo "NEW DATA FILE : $FILE"
