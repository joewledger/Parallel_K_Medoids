#!/bin/bash
#SBATCH -n 16
#SBATCH -p normal
#SBATCH -o mic.txt
#SBATCH -t 00:10:00

export MIC_ENV_PREFIX=MIC
export MIC_OMP_NUM_THREADS=240
export KMP_AFFINITY=scatter
