#!/bin/bash

#SBATCH --job-name dwnl 

#SBATCH -N 1

#SBATCH -n 1

#SBATCH --mem=4G
#SBATCH --nodelist=reslab32ai8111
#SBATCH --mail-type=ALL
#SBATCH --mail-user=go2432@wayne.edu

#SBATCH -o output_%j.out

#SBATCH -e errors_%j.err

#SBATCH -t 7-0:0:0
#eval "$(conda shell.bash hook)"
echo $HOSTNAME
bash download_all.txt
