#!/bin/bash
FILE=data/`date '+%Y%m%d_%T'`.txt
echo "Saving in $FILE"
touch $FILE
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
echo `date +%s` >> $FILE
echo `date +'%M:%S'`
cat /dev/ttyUSB0 >> $FILE &
PID=$!
echo ":READ?" >> /dev/ttyUSB0
echo "Raedy for interuptipn by Ctrl-C"
sleep 10
kill $PID
#cat /dev/ttyUSB0 >> $FILE &
cat  $FILE 
done

echo `cp $FILE data/tmp.txt`

echo "========================================"
echo "===== < COMPLEATED IV MEASUREMENT > ===="
echo "========================================"
echo "NEW DATA FILE : $FILE"
