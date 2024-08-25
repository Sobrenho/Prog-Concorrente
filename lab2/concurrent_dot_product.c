/* Arthur Sobrinho Ferreira da Rocha*/
/* DRE: 122055527 */
/* Laboratório: 2 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct {
    float *vec1;
    float *vec2;
    int size;
    double dotprod; 
}Vectors;
/*
    Essa estrutura foi utilizada em 2 situações diferentes:
        1- Como entrada do arquivo
            vec1: primeiro vetor no arquivo
            vec2: segundo vetor no arquivo
            size: tamanho dos vetores
            dotprod: produto interno calculado na geração dos vetores
        
        2- Na passagem de argumento para as threads
            vec1: parte do primeiro vetor que será multiplicada pela thread
            vec2: parte do primeiro vetor que será multiplicada pela thread
            size: tamanho do subvetor
            dotprod: armazenar retorno do produto interno parcial
*/


Vectors* getVectors(char* filename){
    size_t ret;
    int size;

    //Abertura do Arquivo
    FILE* f  = fopen(filename, "rb");
    if(!f){fprintf(stderr, "--ERRO-- Falha ao abrir arquivo");exit(20);}


    //ler tamanho do vetor
    ret = fread(&size, sizeof(int),1,f);
    if(!ret){fprintf(stderr, "--ERRO-- Tamanho do vetor no arquivo");exit(40);}

    //Criação das estruturas
    Vectors* vectors = (Vectors*) malloc(sizeof(Vectors));
    if(!vectors){fprintf(stderr, "--ERRO-- Malloc Vectors Struct");exit(50);}

    vectors->size = size;
    vectors->vec1 = (float*) malloc(sizeof(float)*size);
    vectors->vec2 = (float*) malloc(sizeof(float)*size);
    if(!vectors->vec1 || !vectors->vec2){fprintf(stderr, "--ERRO-- Malloc Vetores");exit(60);}
    
    //leitura dos vetores
    ret = fread(vectors->vec1, sizeof(float), size, f);
    if(!ret){fprintf(stderr, "--ERRO-- Leitura vetor 1 do arquivo");exit(70);}


    ret = fread(vectors->vec2, sizeof(float), size, f);
    if(!ret){fprintf(stderr, "--ERRO-- Falha lendo vetor 2 do arquivo");exit(80);}


    ret = fread(&vectors->dotprod, sizeof(double), 1, f);
    if(!ret){fprintf(stderr, "--ERRO-- Falha lendo produto interno do arquivo");exit(90);}

    fclose(f);

    return vectors;
}

void *DotProdConc(void* arg){
    Vectors* vecs = (Vectors*) arg;

    float* ret = (float*) malloc(sizeof(float));
    if(!ret){
        fprintf(stderr, "Erro Alocando retorno para thread"); 
        pthread_exit(NULL);
    }
    *ret = 0;

    for(int i=0; i < vecs->size; i++){
        *ret += vecs->vec1[i] * vecs->vec2[i];
    }

    pthread_exit((void*) ret);
}

double comparacaoRelativa(float concorrente, double original){
    return fabs((original - concorrente)/original);
}

int main(int argc, char* argv[]){
    char* arqv_entrada;
    int nthreads, vec_size;
    double dotprod, erro;
    float* dotprod_parcial;


    if (argc < 3){
        fprintf(stderr, "--ERRO-- Entrada : .\\<nome do programa> <numero de threads> <arquivo de entrada>");
        return 1;
    }

    char* filename = argv[2];

    Vectors* vecs = getVectors(filename);

    nthreads = atoi(argv[1]);
    pthread_t idthreads[nthreads];

    //Edge Case. Não faz sentido mais threads do que espaços no vetor
    if(vecs->size < nthreads){nthreads = vecs->size;}

    int offset = 0;
    //Inicialização das threads
    for(int i=0; i< nthreads;i++){
        //Alocar struct com endereço do vetor e index de começar
        Vectors* thread_vecs = (Vectors*) malloc(sizeof(Vectors));
        if(!thread_vecs){fprintf(stderr,"Erro Alocando struct para thread %d", i); return 4;}

        int step = vecs->size/nthreads;

        thread_vecs->size = step;
        thread_vecs->vec1 = &vecs->vec1[i*step + offset];
        thread_vecs->vec2 = &vecs->vec2[i*step + offset];

        //Lidar com tamanho não divisível pelo número de threads
        if(i < vecs->size % nthreads){
            offset++;
            thread_vecs->size++;
        } 
    
        if(pthread_create(&idthreads[i], NULL, DotProdConc, (void*)thread_vecs)){
            fprintf(stderr, "--ERRO-- Criacao da thread %d", i); return 2;
        }

    }


    dotprod = 0;
    for(int i=0; i<nthreads; i++){
        if(pthread_join(idthreads[i], (void**) &dotprod_parcial)){
            fprintf(stderr, "--ERRO-- pthread_join da thread %d\n", i); return 3;
        }
        //printf("\ndotprod_parcial: %f\n", *dotprod_parcial);
        dotprod += *((float*) dotprod_parcial);

        free(dotprod_parcial);

    }

    erro = comparacaoRelativa(dotprod, vecs->dotprod);  

    printf("Produto Interno Inicial: %.15f\nProduto Interno Concorrente: %.15f\nErro Relativo: %.15f\n",
        vecs->dotprod, dotprod, erro);


    return 0;
}   
