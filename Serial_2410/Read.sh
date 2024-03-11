#!/bin/bash

FILE=`date '+%Y%m%d_%T'`.txt
#touch /date/$FILE
echo "Saving in $FILE"
PID=0
echo ":OUTPUT ON" >> /dev/ttyUSB0
function on_signal_interrupt( ){
    echo "trapped.!"
    kill $PID
    exit 1
}

trap on_signal_interrupt 2

while :
do

    
echo `date +%s` >> data/$FILE
echo `date +'%M:%S'`
cat /dev/ttyUSB0 >> data/$FILE &
PID=$!
echo ":READ?">> /dev/ttyUSB0
echo $val
#trap on_signal_interrupt 2
echo "Raedy for interuptipn by Ctrl-C"
sleep 5
#sleep 1
kill $PID 
cat  data/$FILE

done
