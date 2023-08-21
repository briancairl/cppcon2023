#!/bin/bash

g++ -c dijkstras_$1.cpp -std=c++23 -O0 -g -o dijkstras_$1.o -I . -DDIJKSTRAS=$1
ar rvs dijkstras_$1.a dijkstras_$1.o
g++ main.cpp dijkstras_$1.a -std=c++23 -O0 -g -o main.out -I . -DDIJKSTRAS=$1
