/* Sincronização coletiva usando barreiras
 *
 * Condição lógica da aplicação: As threads só podem executar o próximo passo
 * depois que todas terminarem o passo em questão.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 5
#define PASSOS 	 5

// Variáveis de sincronização
pthread_mutex_t lock;
pthread_cond_t  cond;

// Variáveis globais
int bloqueadas = 0;

// Função barreira
void barreira(int nthreads){
	// Início da seção crítica
	pthread_mutex_lock(&lock);
	
	// Se é a última thread a chegar na barreira
	if(bloqueadas == (nthreads - 1)){
		pthread_cond_broadcast(&cond);
		bloqueadas = 0;
	}else{
		bloqueadas++;
		pthread_cond_wait(&cond, &lock);
	}
	
	// Fim da seção crítica
	pthread_mutex_unlock(&lock);

}

// Tarefa das threads
void *tarefa(void *arg){
	int id = *(int *)arg;
	int b1 = 100, b2 = -100;

	for(int passo = 0; passo < PASSOS; passo++){
		printf("Thread %d: passo %d\n", id, passo);
	
	
		// Faz algo só pra gastar tempo...
		while(b1 > b2) b2++;

		// Sincronização condicional com barreira
		barreira(NTHREADS);
	}
	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char *argv[]){
	pthread_t tid[NTHREADS];
	int id[NTHREADS];

	// Inicializa as variáveis de sincronização
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);
	
	// Crias as threads
	for(int i = 0; i < NTHREADS; i++){
		id[i] = i;
		if(pthread_create(&tid[i], NULL, tarefa, (void*) &id[i])){
			fprintf(stderr, "ERRO -- pthread_create()\n");
			return 1;
		}
	}

	// Espera as threads terminarem
	for(int i = 0; i < NTHREADS; i++){
		if(pthread_join(tid[i], NULL)){
			fprintf(stderr, "ERRO -- pthread_join()\n");
			return 2;
		}
	}

	printf("FIM\n");

	// Libera as variáveis de sincronização
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);
	
	return 0;
}
