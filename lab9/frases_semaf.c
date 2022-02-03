/* Disciplina: Computação Concorrente
 * Professora: Silvana Rossetto
 * Módulo 3  - Laboratório 9
 * Atividade: Projetar e implementar um programa concorrente onde
 * 	      a ordem de execução das threads é controlada no
 * 	      programa, usando semáforos.
 *
 * 	      A thread 5 deve sempre ser a primeira a imprimir.
 * 	      A thread 1 deve sempre ser a última a imprimir.
 * 	      A ordem de impressão das threads 2, 3 e 4 não
 * 	      importam, mas todas devem imprimir antes da 1
 * 	      e depois da 5.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 5   // Número de threads
#define TAM	 30 //  Tamanho máximo da mensagem

// Variáveis globais
sem_t cond_prim, cond_ulti; // Semáforos para sincronização por condição
sem_t lock;		    // Semáforo para sincronização por exclusão mútua

int x = 3; // Número de threads intermediárias

// Estrutura que guarda o id e a frase de cada thread
typedef struct {
	int id;
	char frase[TAM];
}tArgs;

// Tarefa das threads
void *imprime_frase(void *t_args){
	tArgs *args = (tArgs *) t_args;

	if(args->id == 5){ // Thread 5 imprime a frase e libera as 3 intermediárias
		printf("%s\n", args->frase);
		sem_post(&cond_prim);
		sem_post(&cond_prim);
		sem_post(&cond_prim);
	}
	else if(args->id == 1){ // Thread 1 última a imprimir
		sem_wait(&cond_ulti); // Verifica se pode imprimir (depois das demais)
		printf("%s\n", args->frase);
	} 
	else{ // Threads intermediárias, imprime depois da thread 5

		sem_wait(&cond_prim); // Verifica se pode imprimir a mensagem (depois da thread 5)
		printf("%s\n", args->frase);
		

		/* Para verificar se é a última thread, precisa envolver com lock para
		 * evitar a condição de corrida
		 */
		
		sem_wait(&lock); // Pega o lock
		
		if(--x == 0)
			sem_post(&cond_ulti); // Libera a thread 1

		sem_post(&lock); // Libera o lock
	}

	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char *argv[]){
	pthread_t tid[NTHREADS];
	tArgs *args;
	
	// Inicializa os semáforos
	sem_init(&cond_prim, 0, 0); // Começa com 0 porque a thread 5 vem e libera as intermediárias
	sem_init(&cond_ulti, 0, 0); // Começa com 0 porque a última thread intermediária a imprimir libera a última thread
	sem_init(&lock, 0, 1); 	    // Começa com 1 porque serve como mutex

	// Aloca memória para a estrutura
	args = (tArgs *) malloc(sizeof(tArgs) * NTHREADS);
	if(args == NULL) {fprintf(stderr, "ERRO -- malloc()\n"); return 1;}

	// Crias as threads
	for(int i = 0; i < NTHREADS; i++){
		switch(i){
			case 0:
				strcpy((args + i)->frase, "Volte sempre!");
				break;
			case 1:
				strcpy((args + i)->frase, "Fique a vontade.");
				break;
			case 2:
				strcpy((args + i)->frase, "Sente-se, por favor.");
				break;
			case 3:
				strcpy((args + i)->frase, "Aceita um copo d'água?.");
				break;
			default:
				strcpy((args + i)->frase, "Seja bem-vindo!");
		}
		(args + i)->id = i + 1;

		if( pthread_create(&tid[i], NULL, imprime_frase, (void *) (args + i)) ){
			fprintf(stderr, "ERRO -- pthread_create()\n");
			return 2;
		}
	}

	// Espera pelo término das threads
	for(int i = 0; i < NTHREADS; i++){
		if(pthread_join(tid[i], NULL)){
			fprintf(stderr, "ERRO -- pthread_joi()\n");
			return 3;
		}
	}

	return 0;
}
