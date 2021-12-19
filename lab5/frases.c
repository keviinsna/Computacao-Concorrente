/*
 * Disciplina: Computação Concorrente
 * Professora: Silvana Rosseto
 * Módulo 2  - Laboratório 5
 * Atividade: Projetar e implementar um programa concorrente onde
 * 	      a ordem de execução das threads é controlada no
 * 	      programa, usando variáveis de condição.
 *
 * 	      A thread 5 deve sempre ser a primeira a imprimir.
 * 	      A thread 1 deve sempre ser a última a imprimir.
 * 	      A ordem de impressão das threads 2, 3 e 4 não
 * 	      importam, mas todas devem imprimir antes da 1
 * 	      e depois da 5.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NTHREADS 5  // Número de threads
#define TAM	 30 // Tamanho máximo da mensagem

// Variáveis de sincronização 
pthread_mutex_t lock;
pthread_cond_t  cond;

// Variável compartilhada que contém as condições de cada thread (booleanas)
int ct[NTHREADS]; // ct = condição das threads

// Estrutura de cada thread
typedef struct {
	int id;
	char msg[TAM];
}t_args;

// Inicialização de ct
void inicializa_ct(){
	for(int i = 0; i < NTHREADS; i++)
		ct[i] = 0;
}

// Tarefa das threads
void *print_frase(void *t){
	t_args *args = (t_args*) t;
	int pos = args->id - 1;

	if(args->id == 5){ // Tarefa da thread 5

		// Não usei lock, pois ela não acessa a variável compartilhada

		// Printa a mensagem e atualiza variável global
		printf("%s\n", args->msg);
		ct[pos] = 1;

		// Libera quem estiver na fila esperando
		pthread_cond_broadcast(&cond);

	}else if(args->id == 1){ // Tarefa da thread 1

		// Entrada seção crítica
		pthread_mutex_lock(&lock);
		
		// Equanto as threads 2, 3, 4 e 5 não imprimirem, thread 1 se bloqueia
		while(!(ct[1] && ct[2] && ct[3] && ct[4])){
			//printf("Thread %d bloqueada!\n", args->id);
			pthread_cond_wait(&cond, &lock);
			//printf("Thread %d desbloqueada!\n", args->id);
		}
		
		// Printa a mensagem e atualiza variável compartilhada
		printf("%s\n", args->msg);
		ct[pos] = 1;

		//Saída seção crítica
		pthread_mutex_unlock(&lock);

	}else{ // Tarefa das threads 2, 3 e 4
		// Entrada seção crítica
		pthread_mutex_lock(&lock);

		// Enquanto a thread 5 não imprimir, threads 2, 3 e 4 se bloqueiam
		while(!ct[4]){
			//printf("Thread %d bloqueada!\n", args->id);
			pthread_cond_wait(&cond, &lock);
			//printf("Thread %d desbloqueada!\n", args->id);
		}

		// Printa mensagem e altera variável compartilhada
		printf("%s\n", args->msg);
		ct[pos] = 1;
		
		//Saída seção crítica
		pthread_mutex_unlock(&lock);

		// Libera as threads bloqueadas
		pthread_cond_signal(&cond);
	}

	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char *argv[]){
	pthread_t tid[NTHREADS]; // Identificadores das threads no sistema
	t_args *args; // Estrutura das threads
	
	// Inicializa as variáveis de sincronização
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);
	
	// Inicializa a variável compartilhada
	inicializa_ct();

	// Alocação de memória para a estrutura
	args = (t_args *) malloc(sizeof(t_args) * NTHREADS);
	if(args == NULL){
		fprintf(stderr, "ERRO -- malloc()\n");
		return 1;
	}

	// Cria as threads
	for(int i = 0; i < NTHREADS; i++){
		switch(i){
			case 0:
				strcpy((args + i)->msg, "Volte sempre!");
				break;
			case 1:
				strcpy((args + i)->msg, "Fique a vontade.");
				break;
			case 2:
				strcpy((args + i)->msg, "Sente-se, por favor.");
				break;
			case 3:
				strcpy((args + i)->msg, "Aceita um copo d'água?.");
				break;
			default:
				strcpy((args + i)->msg, "Seja bem-vindo!");
		}

		(args + i)->id = i + 1; // Só para ficar direitinho 1, 2, ... 
		if(pthread_create(&tid[i], NULL, print_frase, (void *)(args + i))){
			fprintf(stderr, "ERRO -- pthread_create()\n");
			return 2;
		}
	}

	// Espera pelo término das threads
	for(int i = 0; i < NTHREADS;i++){
		if(pthread_join(tid[i], NULL)){
			fprintf(stderr, "ERRO -- pthread_join()\n");
			return 3;
		}
	}

	// Libera as variáveis de sincronização
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);
	
	// Libera a memória da estrutura
	free(args);
	
	return 0;
}
