/* Código: Sincronização coletiva com barreiras usando semáforos
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 3
#define PASSOS   5

// Variáveis globais
sem_t cond;	// Semáforo para sincronização condicional
sem_t mutex;	// Semáforo para sincronização por exclusão mútua

int chegaram = 0; // Variável para auxiliar na barreira

// Barreira
void barreira(int nthreads){
	// Pega o lock
	sem_wait(&mutex);

	chegaram++; // Registra que uma thread chegou

	// Se ainda tiver threads para chegar
	if(chegaram < nthreads){
		sem_post(&mutex); // Libera o lock
		sem_wait(&cond);  // Se bloqueia

		/* Se for a última thread a sair, libera o lock 
		 * para o próximo passo. Senão, apenas libera as
		 * threads da fila sinalizando que elas podem 
		 * sair da tarefa.
		 */

		chegaram--; // Registra que está saindo

		if(chegaram == 0) sem_post(&mutex);
		else	sem_post(&cond);
	}
	else{
		printf("\n");
	 	/* Se for a última thread a chegar, libera
		* apenas 1 thread, para que ela sinalize que 
		* está saindo.
		*/
		chegaram--;
		sem_post(&cond);
	}
}

// Tarefa das threads
void *tarefa(void *arg){
	int id = *(int *) arg;
	int b1 = 1000, b2 = -1000;

	for(int i = 0; i < PASSOS; i++){
		printf("Thread %d: passo = %d\n", id, i);
		
		barreira(NTHREADS);
		
		// Loop bobo
		while(b1 > b2)	b2++;
	}

	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char *argv[]){
	pthread_t tid[NTHREADS];
	int 	  id[NTHREADS];

	// Inicializa os semáforos
	sem_init(&mutex, 0, 1);	// Somente 1 thread poderá estar executando
	sem_init(&cond, 0, 0); //  A última thread libera as demais
	
	// Cria as threads
	for(int i = 0; i < NTHREADS; i++){
		id[i] = i + 1;
		if( pthread_create(&tid[i], NULL, tarefa, (void *) &id[i]) ){
			fprintf(stderr, "ERRO -- pthread_create()\n");
			return 1;
		}
	}

	// Espera pelo término das threads
	for(int i = 0; i < NTHREADS; i++){
		if(pthread_join(tid[i], NULL)){
			fprintf(stderr, "ERRO -- pthread_join()\n");
			return 2;
		}
	}

	printf("FIM.\n");

	return 0;
}
