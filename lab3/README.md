# Instruções 

O arquivo `.sh` contém um teste do algoritmo compilando e executando com tamanhos variados de matrizes para usuários Linux.

Se estiver usando Windows utilize o arquivo `.bat`

Caso queira compilar separadamente siga as instruções abaixo:

Para executar a multiplicação matricial sequencial

```powershell
gcc -o matmult.exe matrizMultSeq.c helper_funcs.c
```

Execute o ``matmult.exe``
```
.\matmult.exe matriz1.bin matriz2.bin saida.bin
```

Para executar a multiplicação matricial concorrente

```powershell
gcc -o matmultconc.exe matrizMultConc.c helper_funcs.c -lpthread
```

Execute o ``matmultconc.exe`` substituindo \<número de threads\> na linha abaixo pela quantidade desejada.
```
.\matmultconc.exe  <número de threads> matriz1.bin matriz2.bin saida.bin
```

Para executar a multiplicação matricial concorrente utilizando a transposta

```powershell
gcc -o matmultconctransp.exe matrizMultConcTransp.c helper_funcs.c -lpthread
```

Execute o ``matmultconctransp.exe`` substituindo \<número de threads\> na linha abaixo pela quantidade desejada.
```
.\matmultconctransp.exe  <número de threads> matriz1.bin matriz2.bin saida.bin
```