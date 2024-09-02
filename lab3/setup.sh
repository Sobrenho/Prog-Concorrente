#!/bin/bash
gcc -o geraMatriz ./cods-lab3/geraMatrizBinario.c
gcc -o matmult ./matrizMultSeq.c ./cods-lab3/helper_funcs.c 
gcc -o matmultconc ./matrizMultConc.c ./cods-lab3/helper_funcs.c -lpthread
gcc -o matmultconctransp ./matrizMultConcTransp.c ./cods-lab3/helper_funcs.c -lpthread