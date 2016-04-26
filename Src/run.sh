#!/bin/bash
rm k_medoids.o
gcc -fopenmp k_medoids.c -lm -o k_medoids.o -D CARS
./k_medoids.o >> regular_time.txt
gcc -fopenmp k_medoids.c -lm -o k_medoids.o -D IRIS
./k_medoids.o >> regular_time.txt
gcc -fopenmp k_medoids.c -lm -o k_medoids.o -D SOYBEAN
./k_medoids.o >> regular_time.txt