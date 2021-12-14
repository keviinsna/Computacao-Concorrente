/*Tratando a condição de corrida com lock*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2

int s = 0; // Variável compartilhada
pthread_mutex_t lock; // Variável para sincronização de exclusão mútua

// Função execu11otada pelas threads
void *tarefa(void *arg){
	long int id = (long int) arg;
	printf("Thread %ld está executando...\n", id);

	for(int i = 0; i < 10000; i++){
		// Entrada de seção crítica
		pthread_mutex_lock(&lock);
	
		s++; // Variável compartilhada
	
		// Saída de seção crítica
		pthread_mutex_unlock(&lock);
	}
	printf("Thread %ld terminou.\n", id);
	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char * argv[]){
	pthread_t tid[NTHREADS];
	
	// Inicializa a variável de exclusão mútua
	pthread_mutex_init(&lock, NULL);

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
	
	// Libera o lock
	pthread_mutex_destroy(&lock);
	return 0;
}
