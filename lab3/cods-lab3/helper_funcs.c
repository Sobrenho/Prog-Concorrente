#include <stdio.h>
#include <stdlib.h>
#include "helper_funcs.h"


void escrever_matriz(FloatMatriz *m, char* filename){
	
	size_t err;

	FILE* f = fopen(filename, "wb");
	if(!f) {fprintf(stderr, "--ERRO-- abertura de arquivo\n"); exit(200);}

	err = fwrite(&(m->N),sizeof(int), 1, f);
	if(!err){fprintf(stderr, "--ERRO-- fwrite N\n"); exit(201);}

	err = fwrite(&(m->M), sizeof(int), 1, f);
	if(!err){fprintf(stderr, "--ERRO-- fwrite M\n"); exit(202);}

	size_t size = m->N * m->M; 

	err = fwrite(m->matriz, sizeof(float), size, f);	
	if(err != size){fprintf(stderr, "--ERRO-- fwrite entradas matriz\n"); exit(203);}

	return;

	fclose(f);
}


FloatMatriz* pegar_matriz(char* filename){
	size_t err;

	FILE* f = fopen(filename, "rb");
	if(!f) {fprintf(stderr, "--ERRO-- abertura de arquivo\n"); exit(300);}

	FloatMatriz* matriz = (FloatMatriz*) malloc(sizeof(FloatMatriz));
	if(!matriz){fprintf(stderr, "--ERRO-- malloc matriz\n"); exit(301);}

	err = fread(&(matriz->N), sizeof(int), 1, f);
	if(!err){fprintf(stderr, "--ERRO-- fread N\n"); exit(302);}

	err = fread(&(matriz->M), sizeof(int), 1, f);
	if(!err){fprintf(stderr, "--ERRO-- fread M\n"); exit(303);}

	size_t size = matriz->N * matriz->M; 

	matriz->matriz = (float*) malloc(sizeof(float) * size);


	err = fread(matriz->matriz, sizeof(float), size, f);	
	if(err != size){fprintf(stderr, "--ERRO-- fread entradas matriz\n"); exit(304);}


	fclose(f);

	return matriz;
}


void free_floatMatriz(FloatMatriz* fm){
    free(fm->matriz);
    free(fm);
    return;
}