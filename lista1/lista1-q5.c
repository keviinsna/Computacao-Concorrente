/*
 * Questão 5 da lista 1
 *
 * Condição lógica da aplicação: A Thread 2 printa o valor de contador toda
 * 				 vez que ele for múltiplo de 100. Enquanto
 * 				 que a Thread 1 incrementa o valor de contador.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 500
#define NTHREADS 2

// Variável compartilhada
int contador = 0;
int print = 0; // variável booleana para auxiliar no primeiro print

// Variáveis de sincronização
pthread_mutex_t lock;
pthread_cond_t  cond;

// Thread 1
void *Thread1(void *arg){

	pthread_mutex_lock(&lock);
	if(!print)
		pthread_cond_wait(&cond, &lock);
	pthread_mutex_unlock(&lock);
	
	for(int i = 0; i <= N; i++){
		// Entrada seção crítica
		pthread_mutex_lock(&lock);
		contador++;
		if((contador % 100) == 0){
			//printf("Thread 1 encontrou múltiplo de 100, contador = %d, então vai se bloquear!\n", contador);

			// Thread 1 fica bloqueada esperando a thread 2 printar o valor de contador			
			// Primeiro libera a thread 2, depois se bloqueia
			pthread_cond_signal(&cond);
			pthread_cond_wait(&cond, &lock);
		}
		// Saída seção crítica
		pthread_mutex_unlock(&lock);
	}	

	// Libera a thread 2 
	pthread_cond_signal(&cond);

	pthread_exit(NULL);
}

// Thread 2
void *Thread2(void *arg){
	
	// Entrada da seção crítica
	pthread_mutex_lock(&lock);

	print = 1;		
	while(contador <= N){
		//printf("\nThread 2 vai printar...\n");
		printf("contador = %d\n", contador);
		//printf("Thread 2 printou, agora vai se bloquear...\n\n");
	 
		// Thread 2 vai liberar a thread 1 e se bloquear
		pthread_cond_signal(&cond);
		pthread_cond_wait(&cond, &lock);
	}
	
	// Saída da seção crítica
	pthread_mutex_unlock(&lock);

	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char *argv[]){
	pthread_t tid[NTHREADS];

	// Inicializa as variáveis de sincronização
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);

	// Cria as threads
	if(pthread_create(&tid[0], NULL, Thread1, NULL) || pthread_create(&tid[1], NULL, Thread2, NULL)){
		fprintf(stderr, "ERRO -- pthread_create()\n");
		return 1;
	}

	// Espera o término das threads
	for(int i = 0; i < NTHREADS; i++){
		if(pthread_join(tid[i], NULL)){
			fprintf(stderr, "ERRO -- pthread_join()\n");
			return 2;
		}
	}

	// Libera as variáveis de sincronização
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);
	
	return 0;
}
