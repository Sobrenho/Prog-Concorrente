/* Lab 5 */
/* Arthur Sobrinho */
/* Prof.: Silvana Rossetto */
/* Modificação do código em cods-lab5/soma-lock-atom.c para imprimir os primeiros 20 múltiplos de 10 ao invés de quaisquer múltiplos de 10 com repetição*/

/* Codigo: Comunicação entre threads usando variável compartilhada e exclusao mutua com bloqueio */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NUM 10 // Definição do número que printaremos os múltiplos
#define MULTIPLOS 20 //Quantidade de múltiplos que será printada na tela

long int multiplos_printados = 0;
long int soma = 0; 
pthread_mutex_t mutex;
pthread_cond_t print_multiplo;
pthread_cond_t sum_thread;


void *ExecutaTarefa (void *arg) {
  long int id = (long long int) arg;
  printf("Thread : %ld esta executando...\n", id);

  for (int i=0; i<100000; i++) {

    pthread_mutex_lock(&mutex);

    //Enquanto soma for o próximo multiplo de 10 a ser impresso e ainda existem prints a serem feitos
    while(multiplos_printados*NUM == soma && soma < NUM*(MULTIPLOS)){ 

        //Indicação para thread extra que chegamos em um múltiplo de 10 para printar
        pthread_cond_signal(&print_multiplo);

        //Auto-bloqueio até printar o múltiplo de 10 atual
        pthread_cond_wait(&sum_thread, &mutex);
    }

    soma++; 

    pthread_mutex_unlock(&mutex);

  }

  printf("Thread : %ld terminou!\n", id, soma);
  pthread_exit(NULL);
}


void *extra (void *args) {

  printf("Extra : esta executando...\n");

  //Enquanto não chegamos no último múltiplo para ser printado
  while (multiplos_printados < MULTIPLOS) {
    pthread_mutex_lock(&mutex);

    //Printamos somente se for o próximo múltiplo de 10
    if (soma == NUM*multiplos_printados){
       printf("soma = %ld \n", soma);
       multiplos_printados++;
    }
    else{ //O valor de soma não é o múltiplo de 10 ou acabamos de printar um múltiplo de 10 
        pthread_cond_broadcast(&sum_thread);
        pthread_cond_wait(&print_multiplo, &mutex);
    }

    pthread_mutex_unlock(&mutex);
  }

  pthread_cond_broadcast(&sum_thread);  
  
  printf("Extra : terminou!\n");
  pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
   pthread_t *tid; //identificadores das threads no sistema
   int nthreads; //qtde de threads (passada linha de comando)


   //--le e avalia os parametros de entrada
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]);
      return 1;
   }
   nthreads = atoi(argv[1]);

   //--aloca as estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*(nthreads+1));
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}

    //--inicializa as variáveis de condição (sincronização)
    pthread_cond_init(&sum_thread, NULL);
    pthread_cond_init(&print_multiplo, NULL);
 
   //--inicializa o mutex (lock de exclusao mutua)
   pthread_mutex_init(&mutex, NULL);


   //--cria thread de log
   if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
   }

   //--cria as threads
   for(long long int t=0; t<nthreads; t++) {
     if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
       printf("--ERRO: pthread_create()\n"); exit(-1);
     }
   }
   //--espera todas as threads terminarem
   for (int t=0; t<nthreads+1; t++) {
     if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
     } 
   } 

   //--finaliza o mutex
   pthread_mutex_destroy(&mutex);
   
   //finaliza as variáveis de condição
   pthread_cond_destroy(&sum_thread);
   pthread_cond_destroy(&print_multiplo);

   printf("Valor de 'soma' = %ld\n", soma);

   return 0;
}
