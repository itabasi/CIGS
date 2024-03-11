#!/bin/bash

if [ $# -ne 1 ]; then
  echo "You should put HV value"
  exit 1
fi

echo ":SOURCE:VOLTAGE:LEVEL $1" >> /dev/ttyUSB0

