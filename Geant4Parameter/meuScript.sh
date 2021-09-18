#!/bin/bash

#vai para a basta do código do Geant4
cd Teste-build/
source ~/Programas/Geant4/geant4.10.05.p01-install/bin/geant4.sh
cmake -DGeant4_DIR=~/Programas/Geant4/geant4.10.05.p01-install/lib/Geant4-10.5.1 ~/Documents/Simulação/Titanio/Geant4Parameter/Teste
make -j4

echo "INÍCIO DO LOOP"
for((i=2000; i<=5000; i=i+100))
do
	echo "INÍCIO DA EXECUÇÃO $i DO GEANT4"
	./Teste $i
	echo "FIM DA EXECUÇÃO $i DO GEANT4"
	#depois que os dados dos vetores foram criados, executa código do Root para criação dos histogramas root angleHistog.c
	#echo "INÍCIO DA EXECUÇÃO $i DE angleHistog.c"
	#root -q angleHistog.c
	#echo "FIM DA EXECUÇÃO $i DE angleHistog.c"
	#muda o nome do arquivo particles.root especificando E,B e d
	echo "MUDANDO NOME DO ARQUIVO particles.root"
	E=$(echo "scale=1; $i/1000;" | bc)
	#cd Particles
	mv particles.root particles_$E"GeV_0.5T_50cm".root
	#cd ..
	
done
echo "FIM DO LOOP"

echo "FIM DA EXECUÇÃO INTEIRA DO GEANT4"
#depois que os dados dos vetores foram criados, executa código do Root para criação dos histogramas root angleHistog.c
#echo "Rodando macro da perda de energia..."
#root -q energyLossMean.c

