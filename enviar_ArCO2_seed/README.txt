
É necessário:

-redefinir a variável export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$GARFIELD_HOME/build antes de compilar gem.C
-necessário criar um script de submissão de job um script job scheduler

https://its.tntech.edu/display/MON/Submitting+Groups+of+HPC+Jobs+with+Job+Arrays

Para cada simulação, parâmetros devem ser mudados em:
nEntries.C -> energia (string) e composição gasosa (string)
gem.C -> energia(string e variável), composição gasosa (string e MagBoltz) e campo magnético (string e variável)
meuJobArray.sh -> nome do output na diretiva
