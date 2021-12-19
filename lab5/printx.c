/* Código: Uso de variáveis de condição e sua operações básicas para sincronizção
 * por condição.
 *
 * Condição lógica: A thread B só poderá imprimir o valor de x quando x for
 * 		    múltiplo de 10, e deverá fazer isso uma única vez e depois
 * 		    terminar sua execução.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 3
#define TCOUNT	20

// Variáveis globais
int x = 0;
pthread_mutex_t mutex_x;
pthread_cond_t  cond_x;

// Threads que incrementam x
void *A(void *arg){
	int *id = (int *) arg;
	int b = 1000000, b2 = -1000;

	printf("A: comecei, thread %d\n", *id);

	for(int i = 0; i < TCOUNT; i++){
		pthread_mutex_lock(&mutex_x);
		x++;
		if((x % 10) == 0){
			printf("A: thread %d, x = %d, encontrou padrão\n", *id, x);
			pthread_cond_signal(&cond_x);
		}
		pthread_mutex_unlock(&mutex_x);

		// Faz algo para gastar tempo...
		while(b > b2) b2++;
	}
	free(id);
	pthread_exit(NULL);
}

// Thread que imprime
void *B(void *arg){
	int *id = (int *) arg;

	printf("B: comecei, thread %d\n", *id);

	pthread_mutex_lock(&mutex_x);	
	//while((x % 10) != 0){
	if((x % 10) != 0){
		printf("B: thread %d, x = %d, vai se bloquear...\n", *id, x);
		pthread_cond_wait(&cond_x, &mutex_x);
		printf("B: thread %d, sinal recebido e mutex realocado. x = %d\n", *id, x);
	}

	printf("x = %d\n", x);
	pthread_mutex_unlock(&mutex_x);
	
	free(id);
	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char* argv[]){
	int *t1, *t2, *t3;
	pthread_t tid[NTHREADS];

	// Aloca memória para os id's das threads
	t1 = (int *) malloc(sizeof(int));
	t2 = (int *) malloc(sizeof(int));
	t3 = (int *) malloc(sizeof(int));
	*t1 = 1; *t2 = 2; *t3 = 3;
	
	// Inicializa as variáveis de sincronização
	pthread_mutex_init(&mutex_x, NULL);
	pthread_cond_init(&cond_x, NULL);

	// Cria as threads
	pthread_create(&tid[0], NULL, A, (void *)t1);
	pthread_create(&tid[1], NULL, B, (void *)t3);
	pthread_create(&tid[2], NULL, A, (void *)t2);

	// Espera pelo término das threads
	for(int i  = 0; i < NTHREADS; i++)
		pthread_join(tid[i], NULL);

	printf("\nFIM\n");

	// Libera variáveis de sincronização
	pthread_mutex_destroy(&mutex_x);
	pthread_cond_destroy(&cond_x);

	// Libera memória dos id's das threads
	free(t1); free(t2); free(t3);
	return 0;
}
