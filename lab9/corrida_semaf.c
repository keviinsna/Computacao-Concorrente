/*
 * Código: Comunicação entre threads usando variável compartilhada
 *	   e sincronização por exclusão mútua com semáforos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 2
#define TAM 1000000
// Variáveis globais
int s = 0; // Variável compartilhada entre as threads
sem_t em; //  Semáforo para exclusão mútua entre as threads

// Função executada pelas threads
void *Tarefa(void *id){
	int *tid = (int *) id;

	printf("Thread: %d está executando...\n", *tid);
	for(int i = 0; i < TAM; i++){
		
		/* Como o semáforo começa com valor inicial 1, então
		 * apenas 1 thread poderá ser executada de cada vez.
		 * Porque quando a primeira der wait(), o inteiro interno
		 * do semáforo será decrementado para 0 e quando a segunda
		 * thread for tentar incrementar a variável compartilhada,
		 * ela será bloqueada até que a thread que já começou dê o
		 * post() no semáforo, incrementando o inteiro para 1 nova-
		 * mente.
		 */
		
		// Entrada da seção crítica
		sem_wait(&em);

		s++; 

		// Saída da seção crítica
		sem_post(&em);
	}
	printf("Thread %d terminou!\n", *tid);
	free(tid);
	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char *argv[]){
	pthread_t tid[NTHREADS];
	int *id;

	// Inicialização do semáforo binário
	sem_init(&em, 0, 1);


	// Aloca memória para os ids e cria as threads
	for(int i = 0; i < NTHREADS; i++){
		// Aloca espaço
		if( (id = (int *) malloc(sizeof(int))) == NULL){
			pthread_exit(NULL); return 1;
		}

		*id = i;

		// Cria as threads
		if(pthread_create(&tid[i], NULL, Tarefa, (void *) id)){
			fprintf(stderr, "ERRO -- pthread_create()\n"); return 2;
		}
	}

	// Espera pelo término das threads
	for(int i = 0; i < NTHREADS; i++){
		if(pthread_join(tid[i], NULL)){
			fprintf(stderr, "ERRO -- pthread_join()\n"); return 3;
		}
	}
	
	printf("Valor de s: %d\n", s);

	return 0;
}
