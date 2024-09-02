#include <stdio.h>
#include <stdlib.h>
#include "cods-lab3/helper_funcs.h"
#include "cods-lab3/exec_time.h"


FloatMatriz*  matriz_mult_seq(FloatMatriz* m1, FloatMatriz* m2){
	if(m1->M != m2->N){
		fprintf(stderr, "--ERRO-- Dimensões incompatíveis\n");
		return NULL;
	}
	
	float elem;

	FloatMatriz* res = (FloatMatriz*) malloc(sizeof(FloatMatriz)); 
	if(!res){fprintf(stderr,"--ERRO-- Malloc struct floatmatriz\n");exit(100);} 

	res->N = m1->N;
	res->M = m2->M;

	res->matriz = (float*) malloc(sizeof(float) * (res->N*res->M));
	if(!res->matriz){fprintf(stderr, "--ERRO-- Malloc floatmatriz\n");exit(101);}

	for(int i=0; i < m1->N; i++){
		for(int j=0; j < m2->M; j++){
			elem = 0;
			for (int k=0; k < m1->M; k++){
				elem += m1->matriz[i*m1->M + k] * m2->matriz[k*m2->M + j];
			}
			res->matriz[i*res->M + j] = elem;
		}
	}
	
	return res;
}

int main(int argc, char* argv[]){
	FloatMatriz *m1, *m2, *m3;	
	double iniciacao, iniciacao_fim, processamento, processamento_fim, finalizacao, finalizacao_fim;

	GET_TIME(iniciacao);

	if(argc < 4){
		printf("Entrada: .\\< nome do programa > < arquivo da matriz 1 > < arquivo da matriz 2 > < arquivo de saida >\n");
		exit(1);
	}

	
	m1 = pegar_matriz(argv[1]);
	m2 = pegar_matriz(argv[2]);
	
	GET_TIME(iniciacao_fim);
    printf("Tempo de Iniciacao :%f\n", iniciacao_fim - iniciacao);
    GET_TIME(processamento);

	m3 = matriz_mult_seq(m1,m2);
	if(m3 == NULL){return 2;}
	

	escrever_matriz(m3, argv[3]);	

	GET_TIME(processamento_fim);
	printf("Tempo de Execucao: %f\n", processamento_fim - processamento);
    GET_TIME(finalizacao);

	free_floatMatriz(m1);
	free_floatMatriz(m2);
	free_floatMatriz(m3);

	GET_TIME(finalizacao_fim);
    printf("Tempo de Finalizacao: %f\n", finalizacao_fim - finalizacao);


	return 0;
}

