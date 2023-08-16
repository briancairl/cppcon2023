#!/bin/bash

echo
echo
echo "-------------------------------------------------"

./build.sh -DDIJKSTRAS_VERSION_1 && ./run.sh \
&& \
./build.sh -DDIJKSTRAS_VERSION_2 && ./run.sh \
