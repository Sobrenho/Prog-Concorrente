/*
Arthur Sobrinho
DRE: 122055527

Código Referente ao lab 6
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>




//Constantes
int qnt_leitores=0;
int qnt_escritores=0;
int quer_escrever=0;

pthread_cond_t* cond_escritores;
pthread_cond_t* cond_leitores;

// Para printar informações de log descomente a linha abaixo
//#define LOGGING 
 
#ifdef LOGGING
#define NOVO_LEITOR 0 
#define FIM_LEITOR 1
#define NOVO_ESCRITOR 2
#define ESCRITOR_EM_ESPERA 3
#define FIM_ESCRITOR 4

void printEstado(int status){
    FILE* f = fopen("log.txt", "a");

    switch (status){
        case NOVO_LEITOR:
            fprintf(f, "Novo leitor!\n");
            break;
        case FIM_LEITOR:
            fprintf(f,"Finalizacao leitor!\n");
            break;
        case NOVO_ESCRITOR:
            fprintf(f,"Novo escritor!\n");
            break;
        case ESCRITOR_EM_ESPERA:
            fprintf(f,"Escritor em espera!\n");
            break;
        case FIM_ESCRITOR:
            fprintf(f,"Finalizacao escritor!\n");
            break;

    }
    fprintf(f,"-------\n");
    fprintf(f,"Leitores:%d\n", qnt_leitores);
    fprintf(f,"Escritores:%d\n", qnt_escritores);
    fprintf(f,"Escritores em espera:%d\n", quer_escrever);
    fprintf(f,"-------\n\n");

    fclose(f);
}
#endif

// ---------------------------------------------------------------------------------

void read_lock(pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    
    while(qnt_escritores > 0 || quer_escrever > 0){
        pthread_cond_wait(mutex, cond_leitores);
    }
    qnt_leitores++;

    #ifdef LOGGING
    printEstado(NOVO_LEITOR);
    #endif
    pthread_mutex_unlock(mutex);
}


void write_lock(pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    quer_escrever++;

    #ifdef LOGGING
    printEstado(ESCRITOR_EM_ESPERA);
    #endif

    if(qnt_escritores > 0 || qnt_leitores > 0){
        pthread_cond_wait(mutex, cond_escritores);

    }
    quer_escrever--;
    qnt_escritores++;

    #ifdef LOGGING
    printEstado(NOVO_ESCRITOR);
    #endif

}


void write_unlock(pthread_mutex_t* mutex){
    //Acessando variável de estado ainda possuindo o lock
    qnt_escritores--;

    #ifdef LOGGING
    printEstado(FIM_ESCRITOR);
    #endif

    if(qnt_escritores == 0 && quer_escrever == 0){
        pthread_cond_broadcast(cond_leitores);
    }

    if(qnt_escritores == 0){ //Sempre executará, pois temos no máximo 1 escritor executando
        pthread_cond_signal(cond_escritores);
    }

    pthread_mutex_unlock(mutex);
}

void read_unlock(pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    qnt_leitores--;

    #ifdef LOGGING
    printEstado(FIM_LEITOR);
    #endif

    pthread_mutex_unlock(mutex);

    if(quer_escrever > 0){
        pthread_cond_signal(cond_escritores);
    }
        
}

void init_my_rwlock(){
    pthread_cond_init(cond_escritores, NULL);
    pthread_cond_init(cond_leitores, NULL);

}


void destroy_my_rwlock(){
    pthread_cond_destroy(cond_escritores);
    pthread_cond_destroy(cond_leitores);
}


// Tentativa falha de executar um lock comum a escritores e leitores como é feito no rwlock da pthread

//void rw_unlock(pthread_mutex_t * mutex){
//
//    pthread_mutex_lock(&mutex);  (Precisa de mutex recursivo) // Lock funciona no caso write, mas não no caso read
//    if(qnt_escritores > 0){ //Sabemos que o unlock atual é de escrita
//        
//        qnt_escritores--;
//
//        if(qnt_escritores == 0 && quer_escrever == 0){
//            pthread_cond_broadcast(&cond_leitores);
//        }
//    }
//    else{ //Sabemos que o unlock atual é de leitura
//        qnt_leitores--;
//        if(qnt_leitores == 0){
//            pthread_cond_signal();
//        }
//    }
//}