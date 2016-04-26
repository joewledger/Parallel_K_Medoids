#!/bin/bash
#SBATCH --output=omp_debug.txt
#SBATCH -c 12
COMPILE_CARS="gcc -fopenmp k_medoids.c -o k_medoids_omp.o -D OMP -D CARS"
COMPILE_IRIS="gcc -fopenmp k_medoids.c -o k_medoids_omp.o -D OMP -D IRIS"
COMPILE_SOYBEAN="gcc -fopenmp k_medoids.c -o k_medoids_omp.o -D OMP -D SOYBEAN"
export OMP_NUM_THREADS=12
module load pgi

$COMPILE_CARS
./k_medoids_omp.o >> omp_time.txt
$COMPILE_IRIS
./k_medoids_omp.o >> omp_time.txt
$COMPILE_SOYBEAN
./k_medoids_omp.o >> omp_time.txt
