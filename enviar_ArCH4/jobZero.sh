#!/bin/bash
#SBATCH --output=ArrayHundred.%A_0.out
#SBATCH --error=ArrayHundred.%A_0.error
#SBATCH --job-name=garfield100Entries
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --partition=medium
#SBATCH --time=14:00:00
#a partição pode ser "long" (14 dias) e "medium*" (4 dias)

#Este script submete um job simples

#########################
# Tarefas dos seus jobs #
#########################

# Carregando módulos C++, ROOT e Garfield++ 
module load gnu8/8.3.0
module load root-6.16.00-gcc-8.3.0-ih7ao2c
module load garfieldpp/3.0

# Linha de execução do seu experimento
echo "Submetendo job 0"
srun ./gem 0




