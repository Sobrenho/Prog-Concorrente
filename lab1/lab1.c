/* Arthur Sobrinho Ferreira da Rocha*/
/* DRE: 122055527 */
/* Laboratório: 1 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct{
	int n;
	int* vec;

}vetorParaThread;


void *AddToVector(void  *arg) {
	
	vetorParaThread int_vec = *(vetorParaThread *) arg;
	
	for(int i = 0; i < int_vec.n; i++){
		int_vec.vec[i] += 1;
	}
	
	free(arg);	
	pthread_exit(NULL);
}

int* inicializarVetor(int n){
	int* vetor = (int*) malloc(n* sizeof(int));

	for (int i = 0; i < n; i++){
		vetor[i] = i*i;
	}

	return vetor;
}


void verificarVetor(int* vetor, int n){
	for (int i=0;i<n;i++){
		if(vetor[i] != i*i + 1){
			printf("--ERRO: Falha no vetor resultante.\nEsperava: %d\nConsegui: %d\n",i*i+1 ,vetor[i]);
			return;
		}
	}

	printf("Adicao no vetor bem sucedida\n");
}

int main(int argc, char* argv[]){
	
	int nthreads, tamanho;
	
	if (argc < 3){
		printf("--ERRO: Informe numero de threads: <.\\atividade5> <numero de threads> <tamanho do vetor>\n");
		return 2;
	}
	

	nthreads = atoi(argv[1]);
	tamanho  = atoi(argv[2]);

	if(tamanho < nthreads) {nthreads = tamanho;} //Não faz sentido existirem mais threads do que posições no vetor.

	int* vetor = inicializarVetor(tamanho);

	pthread_t thread_ids[nthreads];
	
	int posicoes_extras = tamanho % nthreads;
	
	for(int i=0; i < posicoes_extras; i++){
		vetorParaThread * iv = (vetorParaThread*) malloc(sizeof(vetorParaThread*));
		if (iv == NULL){
			printf("--ERRO: Falha no Malloc\n");
			exit(20);
		}

		iv->n = tamanho/nthreads + 1;
		iv->vec = &(vetor[i*iv->n]);
		
		pthread_create(&thread_ids[i], NULL, AddToVector, (void*) iv);
	}

	for(int i=posicoes_extras; i < nthreads;i++){
		vetorParaThread * iv = (vetorParaThread*) malloc(sizeof(vetorParaThread*));
		if (iv == NULL){
			printf("--ERRO: Falha no Malloc\n");
		}

		iv->n = tamanho/nthreads;
		iv->vec = &(vetor[i*iv->n + posicoes_extras]);
		
		pthread_create(&thread_ids[i], NULL, AddToVector, (void*) iv);
	}

	for (int i=0; i<nthreads;i++){
		if(pthread_join(thread_ids[i], NULL)){
			printf("--ERRO: pthread_join da thread %d\n", i);
		}
	}
	verificarVetor(vetor, tamanho);

	free(vetor);
}
