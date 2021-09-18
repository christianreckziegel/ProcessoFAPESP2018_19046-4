#!/bin/bash
#SBATCH --job-name=pGarfield100Entries
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --time=01:00:00
#SBATCH	-p serial
#SBATCH --mem-per-cpu=3700
#a partição pode ser "long" (14 dias) e "medium*" (4 dias)

#Este script submete um array de scripts "meuJobArray.sh"

# Carregando módulos ROOT e Garfield++
module load gnu8/8.3.0
module load root-6.16.00-gcc-8.3.0-ih7ao2c
module load garfieldpp/3.0

# Compilar o leitor de entradas do particles
g++ -o nEntries.exe nEntries.C `root-config --cflags --glibs`
# Atribui o valor às variáveis
read -ra values <<< `./nEntries.exe`
particleEntries=${values[0]}
JobsNum=${values[1]}
restoDivisao=${values[2]}

# Compilando o programa gem.C
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$GARFIELD_HOME/build
make

# Mostra os valores
echo "O arquivo particles.root tem $particleEntries entradas,"
echo "haverão $JobsNum jobs,"
echo "e o último job lerá apenas $restoDivisao entradas."

# Linha de execução do seu experimento
echo "Submetendo job array..."
sbatch --array=0-$JobsNum meuJobArray.sh




