/* Arthur Sobrinho Ferreira da Rocha*/
/* DRE: 122055527 */
/* Laboratório: 2 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


#define DEBUG

double dot_product(float* v1, float* v2, int n){
    double dotprod = 0;
    for(int i=0; i< n; i++){
        dotprod += v1[i]*v2[i];
    }
    return dotprod;
}

void write_random_vectors(char* filename, int vec_size){
    size_t status; 

    float *vec1 = (float*) malloc(sizeof(float)*vec_size);
    float *vec2 = (float*) malloc(sizeof(float)*vec_size);

    FILE* f = fopen(filename, "wb");
    if(!f){
        fprintf(stderr, "--ERRO-- Falha ao abrir arquivo");
        exit(2);
    }
    

    //armazenar tamanho do vetor
    status = fwrite(&vec_size, sizeof(int), 1, f);
    if(!status){fprintf(stderr, "--ERRO-- Falha escrevendo tamanho do vetor no arquivo");exit(3);}

    //seed para o pseudo gerador de números aleatórios
    srand(time(NULL));

    //Variação entre positivo e negativo
    int neg = 1;
    //gerar valores aleatórios e escrever no arquivo
    for (int i=0; i < vec_size; i++){
        vec1[i] = (float) (rand() % 100) * 0.66 * neg;
        vec2[i] = (float) (rand() % 100) * 0.66;

        neg *= -1;       
    }

    //Escrever vetores no arquivo
    status = fwrite(vec1, sizeof(float), vec_size,f);
    if(status < vec_size){fprintf(stderr, "--ERRO-- Falha escrevendo vetor no arquivo");exit(3);}
    
    status = fwrite(vec2, sizeof(float), vec_size,f);
    if(status < vec_size){fprintf(stderr, "--ERRO-- Falha escrevendo vetor no arquivo");exit(4);}
        
    //Calculo do produto interno "sequencial" entre os vetores
    double dotprod = dot_product(vec1,vec2, vec_size);

    //Escrever produto interno no arquivo
    status = fwrite(&dotprod, sizeof(int), 1, f);
    if(!status){fprintf(stderr, "--ERRO-- Falha escrevendo produto interno no arquivo");exit(3);}


#ifdef DEBUG

    char debugfile_name[16];
    sprintf(debugfile_name, "debug_file%d.txt", vec_size);

    FILE *debugfile = fopen(debugfile_name, "w");
    for(int k=0; k < vec_size; k++){
        fprintf(debugfile, "%f ", vec1[k]);
    }

    fprintf(debugfile, "\n");

    for(int k=0; k < vec_size; k++){
        fprintf(debugfile, "%f ", vec2[k]);
    }
    fprintf(debugfile, "\nDot Product: %f", dotprod);
#endif


    fclose(f);
}


int main(int argc, char* argv[]){

    if (argc < 3){
        fprintf(stderr, "Entrada: .\\<nome do programa> <tamanho do vetor> <arquivo de saida> ");
    }

    int size = atoi(argv[1]);
    char* arquivo_saida = argv[2];

    write_random_vectors(arquivo_saida, size);



 return 0;
}
