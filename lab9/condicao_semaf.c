/*
 * Código: Comunicação entre as threads usando variável compartilhada
 *	   e sincronização por condição usando semáforos.
 *
 * Lógica da aplicação: A thread 1 tem que ser a primeira a executar, a
 * 			thread 2 a segunda a executar e a thread 3 a 
 * 			última. A thread 1 escreve 1 na variável compar-
 * 			tilhada. A thread 2 escreve 2. A thread 3 impri-
 * 			me a variável. Portanto, o resultado final deve 
 * 			ser sempre 2.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 3

// Variáveis globais
int x = 0; // Variável compartilhada entre as threads
sem_t cond2, cond3; // Semáforos para sincronizar a ordem de excecução das threads

// Função Thread 1
void *t1(void *id){
	int *tid = (int *) id;
	printf("Thread %d está executando...\n", *tid);

	x = 1;
	sem_post(&cond2); // Permite que a T2 execute

	printf("Thread %d terminou!\n", *tid);	
	pthread_exit(NULL);
}
// Função Thread 2
void *t2(void *id){
	int *tid = (int *) id;
	printf("Thread %d está executando...\n", *tid);

	sem_wait(&cond2); // Verifica se pode executar (depois da 1)
	x = 2;
	sem_post(&cond3);

	printf("Thread %d terminou!\n", *tid);
	pthread_exit(NULL);
}

// Função Thread 3
void *t3(void *id){
	int y;
	int *tid = (int *) id;
	printf("Thread %d está executando...\n", *tid);

	sem_wait(&cond3); // Verifica se pode executar (depois da 2)
	y = x;
	printf("Y = %d\n", y);

	printf("Thread %d terminou!\n", *tid);
	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char *argv[]){
	pthread_t tid[NTHREADS];
	int *id[3];

	// Aloca memória para os ids
	for(int i = 0; i < NTHREADS; i++){
		if( (id[i] = (int *) malloc(sizeof(int)))  == NULL){
			pthread_exit(NULL); return 1;
		}
		*id[i] = i + 1;
	}
	
	// Inicializa os semáforos
	sem_init(&cond2, 0, 0);
	sem_init(&cond3, 0, 0);

	// Cria as threads
	if(pthread_create(&tid[1], NULL, t2, (void *) id[1]) ||
	   pthread_create(&tid[0], NULL, t1, (void *) id[0]) ||
	   pthread_create(&tid[2], NULL, t3, (void *) id[2]) )
	{
		fprintf(stderr, "ERRO -- pthread_create()\n"); return 2;
	}

	// Espera pelo término das threads
	for(int i = 0; i < NTHREADS; i++){
		if( pthread_join(tid[i], NULL) ){
			fprintf(stderr, "ERRO -- pthread_join()\n"); return 3;
		}
		free(id[i]);
	}

	return 0;
}
