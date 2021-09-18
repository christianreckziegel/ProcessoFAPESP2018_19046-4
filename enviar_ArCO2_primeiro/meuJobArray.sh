#!/bin/bash
#SBATCH --output=ArCO2_70-30_1mT_20cm_2GeV_%A.%a.out
#SBATCH --job-name=pArCO2
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --time=14-00:00:00
#SBATCH	-p serial
#SBATCH --mem-per-cpu=3700
#SBATCH --mail-user=christian.reckziegel@aluno.ufabc.edu.br

#a partição pode ser "long" (14 dias) e "medium*" (4 dias)

#Este script submete um job com parâmetro do índice do job array

#########################
# Tarefas dos seus jobs #
#########################

# Carregando módulos ROOT e Garfield++ (já foram carregados no script de submissão)
#module load gnu8/8.3.0
#module load root-6.16.00-gcc-8.3.0-ih7ao2c
#module load garfieldpp/3.0

# Linha de execução do seu experimento
echo -e "\n## Submetendo job $SLURM_ARRAY_TASK_ID em $(date +'%d-%m-%Y as %T') #####################\n"
srun ./gem $SLURM_ARRAY_TASK_ID
echo -e "\n## Job $SLURM_ARRAY_TASK_ID finalizado em $(date +'%d-%m-%Y as %T') ###################"


