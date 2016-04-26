#!/bin/bash
#SBATCH --output=acc_debug.txt
#SBATCH --partition=gpufermi
#SBATCH --gres=gpu:1
COMPILE_CARS="pgcc -ta=nvidia:cc20 -acc k_medoids.c -lm -o k_medoids.o -D CARS -D ACC"
COMPILE_IRIS="pgcc -ta=nvidia:cc20 -acc k_medoids.c -lm -o k_medoids.o -D IRIS -D ACC"
COMPILE_SOYBEAN="pgcc -ta=nvidia:cc20 -acc k_medoids.c -lm -o k_medoids.o -D SOYBEAN -D ACC"
module load pgi

cp *.c $PFSDIR/.
cd $PFSDIR

$COMPILE
./k_medoids.o >> acc_time.txt

cp *.txt ~/Parallel_K_Medoids