// Ideia dada pelo Gabriel Leão

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "cods-lab3/helper_funcs.h"
#include "cods-lab3/exec_time.h"

typedef struct{
   FloatMatriz* fm1;
   FloatMatriz *fm2;
   FloatMatriz *fm3;
   int idx_ini;
   int idx_fim;

}t_args;


FloatMatriz* transpor(FloatMatriz* fm){

	FloatMatriz* res = (FloatMatriz*) malloc(sizeof(FloatMatriz));
	if(!res){fprintf(stderr,"--ERRO-- Malloc struct floatmatriz\n");exit(501);} 

	res->M = fm->N;
	res->N = fm->M;

	res->matriz = (float*) malloc(sizeof(float)*fm->N*fm->M);
	if(!res->matriz){fprintf(stderr,"--ERRO-- Malloc resposta matriz transposta\n");exit(501);} 

	for(int i=0; i< fm->N; i++){
		
		for(int j=0; j < fm->M; j++){
		
			res->matriz[j*res->M + i] = fm->matriz[i*fm->M + j];
		
		}
	}

	return res;
}

void *thread_dot_prod_transposto(void* arg){
	float val;
	t_args* args = (t_args*) arg;

	for(int k = args->idx_ini; k < args->idx_fim; k++){

		val = 0;

		int linha_atual  = k / args->fm3->M; 
		int coluna_atual = k % args->fm3->M;

		for(int i = 0; i < args->fm1->M ; i++){ 

			val += args->fm1->matriz[linha_atual * args->fm1->M + i]  * args->fm2->matriz[coluna_atual * args->fm2->M + i];

		}

		args->fm3->matriz[k] = val;
	}

	free(args);
    pthread_exit(NULL);
}



FloatMatriz*  conc_matriz_mult_transposto(FloatMatriz* m1, FloatMatriz* m2, int nthreads){
	if(m1->M != m2->N){
		fprintf(stderr, "--ERRO-- Dimensões incompatíveis\n");
		return NULL;
	}
	

	FloatMatriz* res = (FloatMatriz*) malloc(sizeof(FloatMatriz)); 
	if(!res){fprintf(stderr,"--ERRO-- Malloc struct floatmatriz\n");exit(100);} 

	res->N = m1->N;
	res->M = m2->M;
    int size = res->N * res->M;

	res->matriz = (float*) malloc(sizeof(float) * size);
	if(!res->matriz){fprintf(stderr, "--ERRO-- Malloc floatmatriz\n");exit(101);}


    //Lidar com número de threads maior que número de células na matriz
    if(nthreads > size){
        nthreads = size;
    }


    pthread_t* pthread_ids = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(!pthread_ids){fprintf(stderr, "--ERRO-- malloc pthread ids \n");exit(102);}

    //SEPARAÇÃO LINHAS E COLUNAS
	int total_size =  res->M * res->N;
	int idx_size = total_size / nthreads;
	int offset = 0;

    for(int i=0; i < nthreads; i++){
	    t_args* arg = (t_args*) malloc(sizeof(t_args));
        arg->fm1 = m1;
        arg->fm2 = m2;
        arg->fm3 = res;


		arg->idx_ini =  i * idx_size + offset;
		arg->idx_fim =  arg->idx_ini + idx_size;

		if(i < total_size % nthreads){
			offset++;
			arg->idx_fim++;
		}

		if(pthread_create(&pthread_ids[i], NULL, thread_dot_prod_transposto, (void*)arg)){
            fprintf(stderr, "--ERRO-- pthread create: thread %d\n", i);
        }

    }


	//Join threads
	for(int i = 0; i < nthreads; i++){
		if(pthread_join(pthread_ids[i], NULL)){
			fprintf(stderr, "--ERRO -- pthread join : thread %d\n",i);
		}
	}

	return res;
}

int main(int argc, char* argv[]){
	FloatMatriz *m1, *m2, *m3;
    int nthreads;
	double iniciacao, iniciacao_fim, processamento, processamento_fim, finalizacao, finalizacao_fim;

    GET_TIME(iniciacao);

	if(argc < 5){
		printf("Entrada: .\\<nome do programa> <numero de threads> <arquivo da matriz 1> <arquivo da matriz 2> <arquivo de saida>\n");
		return 1;
	}
	
    nthreads = atoi(argv[1]);

	m1 = pegar_matriz(argv[2]);
	m2 = pegar_matriz(argv[3]);
	m2 = transpor(m2);


	GET_TIME(iniciacao_fim);
    printf("Tempo de Iniciacao :%f\n", iniciacao_fim - iniciacao);
    GET_TIME(processamento);

	m3 = conc_matriz_mult_transposto(m1,m2, nthreads);
	if(m3 == NULL){return 12;}

	GET_TIME(processamento_fim);
	printf("Tempo de Execucao: %f\n", processamento_fim - processamento);
    GET_TIME(finalizacao);

	escrever_matriz(m3, argv[4]);	

	free_floatMatriz(m1);
	free_floatMatriz(m2);
	free_floatMatriz(m3);

    GET_TIME(finalizacao_fim);
    printf("Tempo de Finalizacao: %f\n", finalizacao_fim - finalizacao);

	return 0;
}
