# Compilação
Para compilar o código utilize:
```
gcc -o exec.exe .\lab6.c .\list_int.c .\updated_main_rwlock.c -lpthread
``` 

# Modificações
* Alterei o tipo do argumento da função das threads para `long long int` para evitar problemas de compatibilidade entre inteiro e ponteiro no meu computador.
* Alterei as chamadas de lock/unlock na função de threads `tarefa` em main_rwlock.c para a minha implementação.
* Na função `main` de main_rwlock.c retirei o rwlock da biblioteca pthread e coloquei um mutex simples. 
* Também inicializei e finalizei as variáveis de condição que serão utilizadas pelas funções em  `lab6.c`


# Análise

- Os testes foram realizados com a linha ``#define LOGGING`` comentada para não considerar tempo perdido em I/O. 
- Os programas foram compilados sem otimização
```
gcc -o exec.exe .\lab6.c .\list_int.c .\updated_main_rwlock.c -lpthread
``` 

```
gcc -o original_exec.exe .\main_rwlock.c .\list_int.c -lpthread
```

Nos testes que realizei minha implementação teve uma melhora considerável em relação ao rwlock da pthread. Acredito que isso decorra da minha implementação ser especifica para esse problema e não considera certos casos (apesar de não saber quais são eles até o ponto dessa escrita) e a rwlock ser mais geral.




## Exemplos de tempo de execução:

### Com 8 threads
- RWLOCK da pthread 
```
.\original_exec.exe 8

Thread 7: in=129 out=128 read=12243
Thread 3: in=129 out=128 read=12243
Thread 4: in=129 out=128 read=12243
Thread 0: in=129 out=128 read=12243
Thread 6: in=129 out=128 read=12243
Thread 2: in=129 out=128 read=12243
Thread 1: in=129 out=128 read=12243
Thread 5: in=129 out=128 read=12243
Tempo: 0.035660
```

- Minha Implementação
```
.\exec.exe 8

Thread 2: in=129 out=128 read=12243
Thread 1: in=129 out=128 read=12243
Thread 5: in=129 out=128 read=12243
Thread 7: in=129 out=128 read=12243
Thread 6: in=129 out=128 read=12243
Thread 3: in=129 out=128 read=12243
Thread 4: in=129 out=128 read=12243
Thread 0: in=129 out=128 read=12243
Tempo: 0.019026
```

--- 
### Com 4 threads 

- RWLOCK da pthread
```
.\original_exec.exe 4

Thread 1: in=242 out=252 read=24506
Thread 0: in=242 out=252 read=24506
Thread 3: in=242 out=252 read=24506
Thread 2: in=242 out=252 read=24506
Tempo: 0.033909
```

- Minha Implementação
```
.\exec.exe 4

Thread 1: in=242 out=252 read=24506
Thread 2: in=242 out=252 read=24506
Thread 3: in=242 out=252 read=24506
Thread 0: in=242 out=252 read=24506
Tempo: 0.015404
```

---
### Com 1 Thread
- RWLOCK da pthread
```
Thread 0: in=967 out=1000 read=98033
Tempo: 0.011860
```
- Minha Implementação 
```
.\exec.exe 4
Thread 0: in=967 out=1000 read=98033
Tempo: 0.008874
```
