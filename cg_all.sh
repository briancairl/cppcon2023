#!/bin/bash

for (( i = 0; i < 6; i++ )); do
  valgrind --tool=callgrind --cache-sim=yes --callgrind-out-file=dijkstras.callgrind.v$i ./build/main v$i ~/Downloads/supermarket.json all ~/Downloads/supermarket.paths.v$i.json
done