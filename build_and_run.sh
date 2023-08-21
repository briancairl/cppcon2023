#!/bin/bash

echo
echo

for (( i = 0; i < 4; i++ )); do
  echo
  echo $i "-------------------------------------------------"
  ./build.sh $i && ./run.sh
  echo $i "-------------------------------------------------"
  echo
done
