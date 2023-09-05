#!/bin/bash

for (( i = 0; i < 5; i++ )); do
  valgrind --tool=cachegrind --cache-sim=yes --cachegrind-out-file=dijkstras.cachegrind.v$i ./build/main v$i ~/Downloads/supermarket.json ~/Downloads/supermarket.problems.json ~/Downloads/supermarket.paths.v$i.json
done