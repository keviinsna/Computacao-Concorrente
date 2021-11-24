#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 10

int vetor[NTHREADS];

/*Estrutura de dados para armazenar os argumentos da thread*/
typedef struct{
	int id_thread, n_threads;
}t_args;

/* função que a thread irá executar */
void * print_hello(void * arg){
	t_args *args  = (t_args *) arg;

	printf("Olá, sou a thread %d de %d threads\n", args->id_thread, args->n_threads);
	free(arg); /*Liberrar a alocação feita na main*/
	pthread_exit(NULL);
}

/* função principal */
int main(void){
	pthread_t tid[NTHREADS]; /* identificadores de cada thread no sistema*/
	int i;	
	t_args *arg; /*Receberá os argumentos para a thread*/

	

	for(i = 0; i < NTHREADS; i++){	
		/*Aloca e preenche os argumentos cada thread*/
		arg = malloc(sizeof(t_args));
		if(arg == NULL)
			printf("ERRO -- malloc()\n");
		arg->id_thread = i;
		arg->n_threads = NTHREADS; 
		
		/*Cria cada thread*/
		if(pthread_create(&tid[i], NULL, print_hello, (void *) arg))
				printf("ERRO -- pthread_create()\n");
	}
	for(i = 0; i < NTHREADS; i++){	
		if(pthread_join(tid[i], NULL))
			printf("EROO -- pthread_create\n");
	}

	printf("Thread principal terminou!\n");
	/*pthread_exit(NULL);	Não precisa mais forçar, já que o join já espera por todas as threads*/
	return 0;
}

