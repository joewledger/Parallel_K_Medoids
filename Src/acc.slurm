#!/bin/bash
#SBATCH --output=acc_debug.txt
#SBATCH --partition=gpufermi
#SBATCH --gres=gpu:1
COMPILE="pgcc -ta=nvidia:cc20 -acc k_medoids.c -lm -o k_medoids.o -D CARS -D ACC"
module load pgi

cp *.c $PFSDIR/.
cd $PFSDIR

$COMPILE
./k_medoids.o >> acc_time.txt

cp *.txt ~/Parallel_K_Medoids