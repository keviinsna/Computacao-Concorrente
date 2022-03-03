/* Disciplina: Computação Concorrente
 * Professora: Silvana Rossetto
 * Módulo 3  - Lista 2
 *
 * Atividade (questão 5): Estenda a solução da questão 4, permitindo que a fila
 * 			  possa ser consultada para saber quantas requisições 
 * 			  estão aguardando para serem atendidas. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "estrutura_fila.c"

#define TAM_MAX_FILA 5   // Tamanho limite da fila de impressão.
#define P	     3	//  Número de threads produtoras.

// Variáveis globais
FILA *impressora; // estrutura fila (FIFO) que será a impressora.

// Semáforos
sem_t mutex;		       // Semáforo para mutex 
sem_t slot_cheio, slot_vazio; //  Semáforos para verificar se a fila de impressão está cheia ou vazia.

/* Threads que irão executar (consumir) as requisições de impressão.*/
void *consumidor(void *arg){
	int id = *(int *)arg;

	while(1){
		
		printf("C[%d] quer executar a primeira requisição da fila...\n", id);

		/* Verifica se há slot cheio para consumir. 
		 *  Se sim, o consome. Senão, fica bloqueada.
		 */
		sem_wait(&slot_cheio);
	
		sem_wait(&mutex);

		printf("C[%d] consumiu info: Texto %d\n", id, remove_fila(impressora));
		printf("A fila está com %d requisição(ões) pendente(s).\n", impressora->qtd);
		print_fila(impressora);

		sem_post(&mutex);

		// Sinaliza que há mais um slot vazio.
		sem_post(&slot_vazio);
	}
	
	pthread_exit(NULL);
}

/* Threads que irão gerar (produzir) as requisições de impressão. */
void *produtor(void *arg){
	int id = *(int *) arg;
	static int k = 0;

	while(1){

		printf("P[%d] quer gerar nova requisição...\n", id);

		/* Verifica se há slot vazio para inserir.
		 * Se sim, insere novo elemento. Senão, fica bloqueada.
		 */
		sem_wait(&slot_vazio);

		// Exclusão mútua entre os produtores
		sem_wait(&mutex);
		
		insere_fila(impressora, k);
		printf("P[%d] gerou requisição: Texto %d\n", id, k);
		k++;
		printf("A fila está com %d requisição(ões) pendente(s).\n", impressora->qtd);
		print_fila(impressora);
	
		sem_post(&mutex);

		// Sinaliza que há mais um slot cheio.
		sem_post(&slot_cheio);
	}

	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char *argv[]){
	pthread_t tid[P + 1]; // Identificadores das threads no SO.
	int id[P + 1];       //  Identificadores das threads no programa.
		
	// Inicialização dos semáforos
	
	// Semáforos de sincronização por exclusão mútua
	sem_init(&mutex, 0, 1); //  Mutex entre produtores e consumidores.

	// Semáforos de sincronização condicional
	sem_init(&slot_cheio, 0, 0);             // Semáforo que indica quantos slots estão cheios
	sem_init(&slot_vazio, 0, TAM_MAX_FILA);	//  Semáforo que indica quantos slots estão vazios

	// Cria a impressora
	impressora = cria_fila();
	
	// Cria a thread consumidora de requisição (apenas 1)	
	id[0] = 1;
	if( pthread_create(&tid[0], NULL, consumidor, (void *) &id[0]) ){
		fprintf(stderr, "ERRO -- pthread_create()\n");
		return 1;
	}
	
	// Crias as threads produtoras de requisição
	for(int i = 0; i < P; i++){
		id[i + 1] = i + 1;
		if( pthread_create(&tid[i + 1], NULL, produtor, (void *) &id[i + 1]) ){
			fprintf(stderr, "ERRO -- pthread_create()\n");
			return 1;
		}
	}
	
	pthread_exit(NULL);

	return 0;
}
