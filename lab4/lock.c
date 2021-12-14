/*Condição de corrida entre as 2 threads - pt2*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2

int s = 0; // Variável compartilhada

// Função execu11otada pelas threads
void *tarefa(void *arg){
	long int id = (long int) arg;
	printf("Thread %ld está executando...\n", id);

	for(int i = 0; i < 10000; i++)
		s++; // Variável compartilhada
	printf("Thread %ld terminou.\n", id);
	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char * argv[]){
	pthread_t tid[NTHREADS];

	// Cria as threads
	for(long int i = 0; i < NTHREADS; i++){
		if(pthread_create(&tid[i], NULL, tarefa, (void *) i)){
			fprintf(stderr, "ERRO -- pthread_create()\n");
			return 1;
		}
	}

	// Espera pelo término das threads
	for(long int i = 0; i < NTHREADS; i++){
		if(pthread_join(tid[i], NULL)){
			fprintf(stderr, "ERRO -- pthread_join()\n");
			return 2;
		}
	}
	printf("Valor de s: %d\n", s);
	return 0;
}
