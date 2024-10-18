#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define N 500
#define NTHREADS 3
#define N2 2*N

char buffer1[N];
char buffer2[N2]; 

sem_t sem_buffer1_vazio;
sem_t sem_buffer1;
sem_t sem_buffer2_vazio;
sem_t sem_buffer2;

pthread_mutex_t mutex;

char* filename;
int file_done=0;
int fim_buffer2=0;
pthread_mutex_t mutex;

void *carrega_buffer1(void* arg){
	size_t chars_read=0;
	size_t total_chars_read = 0;

	FILE* f = fopen(filename, "r");
	if(!f){fprintf(stderr, "Erro abrindo arquivo %s", filename);exit(200);}

	while(1){
		sem_wait(&sem_buffer1_vazio);

		chars_read = fread(&buffer1, sizeof(char), N-1 ,f);
		if(chars_read < N-1){
			buffer1[chars_read+1] = '\0';
			pthread_mutex_lock(&mutex);
			file_done=1;
			pthread_mutex_unlock(&mutex);
			sem_post(&sem_buffer1);
			break;	
		}

		total_chars_read += chars_read;

		sem_post(&sem_buffer1);
	}

	fclose(f);	
	pthread_exit(NULL);

}

void *carrega_buffer2(void* arg){	
	int k=0; // armazena qual linha estamos atualmente.
	int count=0; //contador de caracteres lidos na linha atual
	int offset = 0; // armazena offset causado pela inserção de \n
	int char_count=0;
	while(1){

		sem_wait(&sem_buffer1);
		sem_wait(&sem_buffer2_vazio);

		offset = 0;
		for(int i=0; i<N-1;i++){
			
			if(buffer1[i] == '\0'){ //Não tem mais caracter
				buffer2[i+offset] = '\0'; //A linha deve acabar aqui
				break;
			}

			if(count == 2*k+1 && k <= 10){
				buffer2[i+offset] = '\n';
				offset++;
				k++;
				count=0;

					char_count++;
			
			}else if(k > 10 && count % 10 == 0){
				buffer2[i+offset] = '\n';
				offset++;
				count=0;

					char_count++;
			}

			buffer2[i+offset] = buffer1[i];
					char_count++;
			count++;
		}

		buffer2[N+offset] = '\0';
		sem_post(&sem_buffer1_vazio);
		sem_post(&sem_buffer2);

		pthread_mutex_lock(&mutex);
		if(file_done){
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);
	}

	pthread_mutex_lock(&mutex);
	fim_buffer2 = 1;
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);

}


void *printa_buffer2(void* arg){
	int chars_read=0;
	while(1){
		sem_wait(&sem_buffer2);

		chars_read += printf("%s", buffer2); // ler até '\0'
		
		sem_post(&sem_buffer2_vazio);

		
		pthread_mutex_lock(&mutex);
		if(fim_buffer2){
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);

}

void inicializar_modelo_concorrencia(){
	pthread_mutex_init(&mutex, NULL);

	sem_init(&sem_buffer1_vazio, 0, 1);
	sem_init(&sem_buffer2_vazio, 0, 1);
	sem_init(&sem_buffer1, 0, 0);
	sem_init(&sem_buffer2, 0, 0);

}


void finalizar_modelo_concorrencia(){
	sem_destroy(&sem_buffer1);
	sem_destroy(&sem_buffer1_vazio);
	sem_destroy(&sem_buffer2);
	sem_destroy(&sem_buffer2_vazio);
	pthread_mutex_destroy(&mutex);

}


int main(int argc, char* argv[]){

	if(argc < 2){
		fprintf(stderr, "Uso: %s < filename > ", argv[0]);
		exit(1);
	}

	filename = argv[1];

 	pthread_t tid[NTHREADS]; 
	
	inicializar_modelo_concorrencia();

	if(pthread_create(&tid[0], NULL, carrega_buffer1, NULL)){
		fprintf(stderr, "Erro pthread create 0\n"); exit(10);
	}	
	if(pthread_create(&tid[1], NULL, carrega_buffer2, NULL)){
		fprintf(stderr, "Erro pthread create 1\n");exit(11);
	} 
	if(pthread_create(&tid[2], NULL, printa_buffer2, NULL)){
		fprintf(stderr, "Erro pthread create 2\n");exit(12);
	}

	for(int i=0; i< NTHREADS; i++){
		if(pthread_join(tid[i], NULL)){
			fprintf(stderr, "Erro pthread join %d\n", i);
			exit(50);
		}
	}


	finalizar_modelo_concorrencia();

	return 0;
}
