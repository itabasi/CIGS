#!/bin/bash

  echo ":SOURCE:FUNCTION VOLTAGE" >> /dev/ttyUSB0
  echo ":SOURCE:VOLTAGE:MODE FIXED" >> /dev/ttyUSB0
  echo ":SOURCE:VOLTAGE:RANGE MAX" >> /dev/ttyUSB0

  echo ":TRIGGER:COUNT 1" >> /dev/ttyUSB0
  echo ":SENSE:CURRENT:RANGE 10E-6" >> /dev/ttyUSB0
  echo ":SENSE:FUNCTION 'CURR'" >> /dev/ttyUSB0
  echo ":SENSE:FUNCTION 'VOLT'" >> /dev/ttyUSB0
  echo ":FORMAT:ELEMENTS VOLTAGE,CURRENT" >> /dev/ttyUSB0

  # added by itbashi 2022/09/24
  echo ":SENS:AVER:TCON REP" >> /dev/ttyUSB0
#  echo ":SENS:AVER:TCON MOV" >> /dev/ttyUSB0
#  echo ":SENS:AVER:COUN 100" >> /dev/ttyUSB0
  echo ":SENS:AVER:COUN 10" >> /dev/ttyUSB0
  echo ":SENS:AVER ON" >> /dev/ttyUSB0
#Configuring Voltage and the current compliance.
  echo ":SENSE:CURRENT:PROTECTION 100E-6" >> /dev/ttyUSB0
  echo ":SOURCE:VOLTAGE:LEVEL 0" >> /dev/ttyUSB0

