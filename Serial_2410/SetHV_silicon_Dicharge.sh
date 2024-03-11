#!/bin/bash

FILE=`date '+%Y%m%d_%T'`.txt
echo "Saving in $FILE"
PID=0

for ((i=0; i<1001; i=i+50)); do
#for ((i=0; i<20; i++)); do

    echo -$i

    echo ":SOURCE:VOLTAGE:LEVEL -$i" >> /dev/tty.Plser
    sleep 3

    echo `date +%s` >> data/$FILE
    cat /dev/tty.Plser >> data/$FILE &
    PID=$!
    echo ":READ?" >> /dev/tty.Plser
    sleep 3
    kill $PID
    cat  $FILE

done

for ((i=1000; i>=0; i=i-50)); do
#for ((i=0; i<20; i++)); do

    echo -$i

    echo ":SOURCE:VOLTAGE:LEVEL -$i" >> /dev/tty.Plser
    sleep 3

    echo `date +%s` >> data/$FILE
    cat /dev/tty.Plser >> data/$FILE &
    PID=$!
    echo ":READ?" >> /dev/tty.Plser
    sleep 3
    kill $PID
    cat  $FILE

done


