#!/bin/bash

for ((i=200; i>11; i=i-10)); do

    echo -$i

    echo ":SOURCE:VOLTAGE:LEVEL -$i" >> /dev/tty.Plser
    echo ":READ?" >> /dev/tty.Plser
    sleep 1
   
done

for ((i=10; i>-1; i--)); do

    echo -$i

    echo ":SOURCE:VOLTAGE:LEVEL -$i" >> /dev/tty.Plser
    echo ":READ?" >> /dev/tty.Plser
    sleep 1
   
done


