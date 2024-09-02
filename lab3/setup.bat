@echo off

gcc -o geraMatriz.exe ./cods-lab3/geraMatrizBinario.c

gcc -o matmult.exe ./matrizMultSeq.c ./cods-lab3/helper_funcs.c 

gcc -o matmultconc.exe ./matrizMultConc.c ./cods-lab3/helper_funcs.c -lpthread

gcc -o matmultconctransp.exe ./matrizMultConcTransp.c ./cods-lab3/helper_funcs.c -lpthread