/* Uso de variáveis de condição e suas operações básicas para sincronização
 * por condição.
 *
 * Condição lógica da aplicação: A thread B só pode imprimir "Bye" depois
 * que as duas threads A's imprimirem "Hello".
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 3

// Variáveis globais 
int x = 0;
pthread_mutex_t mutex_x; // Sincronização por exclusão mútua
pthread_cond_t  cond_x; // Sincronização por condição

// Thread A
void *A(void *arg){
	int b1, b2; 
	
	printf("A: Comecei!\n");

	// Faz algo para gastar o tempo... 
	b1 = 10000; b2 = -10000;
	while(b1 > b2) b2++;

	printf("HELLO\n");

	// Seção crítica
	// Entrada da seção crítica
	pthread_mutex_lock(&mutex_x);
	
	x++;
	if(x == 2){
		printf("A: x = %d, vai sinalizar a condição.\n", x);
		pthread_cond_signal(&cond_x);
	}

	// Saída da seção crítica
	pthread_mutex_unlock(&mutex_x);

	pthread_exit(NULL);
}

void *B(void *arg){
	printf("B: comecei!\n");

	pthread_mutex_lock(&mutex_x);	
	if(x < 2){
		printf("B: x = %d, vai se bloquear...\n", x);
		pthread_cond_wait(&cond_x, &mutex_x);
		printf("B: sinal recebido e mutex realocado, x = %d\n", x);
	}
	printf("BYE\n");
	pthread_mutex_unlock(&mutex_x);
	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char *argv[]){
	pthread_t tid[NTHREADS]; // Id das threads

	// Inicializa as variáveis de sincronização
	pthread_mutex_init(&mutex_x, NULL);
	pthread_cond_init(&cond_x, NULL);

	// Cria as threads
	pthread_create(&tid[0], NULL, A, NULL);
	pthread_create(&tid[1], NULL, A, NULL);
	pthread_create(&tid[2], NULL, B, NULL);


	// Espera o término das threads
	for(int i = 0; i < NTHREADS; i++){
		if(pthread_join(tid[i], NULL)){
			printf("ERRO -- pthread_join()\n");
			return 1;
		}
	}

	printf("\nFIM\n");

	// Libera as variáveis de sincronização
	pthread_mutex_destroy(&mutex_x);
	pthread_cond_destroy(&cond_x);

	return 0;
}
