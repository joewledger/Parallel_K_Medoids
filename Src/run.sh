#!/bin/bash
rm k_medoids.o
gcc -fopenmp k_medoids.c -lm -o k_medoids.o -D CARS >> regular_time.txt
./k_medoids.o
gcc -fopenmp k_medoids.c -lm -o k_medoids.o -D IRIS >> regular_time.txt
./k_medoids.o
gcc -fopenmp k_medoids.c -lm -o k_medoids.o -D SOYBEAN >> regular_time.txt
./k_medoids.o