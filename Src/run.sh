#!/bin/bash
rm k_medoids.o
gcc k_medoids.c -lm -o k_medoids.o
./k_medoids.o