É possível compilar uma macro ROOT usando apenas o compilador C++ (g++) já que o ROOT não é nada mais que uma coleção 
de bibliotecas C++. Fazendo do código um "stand alone application":
chmod u+x programa.C
g++ -o programa programa.C `root-config --cflags --glibs`
./programa
Fonte:
https://en.wikibooks.org/wiki/ROOT/Getting_Started/Many_Ways_to_Use_ROOT

Para ler o resultado de uma impressão de algum código C++ para uma variável de shell script deve ser feito: 
variavel=$(./programa)
Para ler mais de um valor:
read -ra values <<< `./programa`
    x=${values[0]}
    y=${values[1]}
    z=${values[2]}
Fonte:
https://stackoverflow.com/questions/11571572/how-to-use-output-of-c-program-as-variables-in-a-bash-script
https://stackoverflow.com/questions/21197207/returning-values-from-a-c-program-into-a-bash-script

