#include <stdio.h>
#include <pthread.h>

#define NTHREADS 10

/* função que a thread irá executar */
void * tarefa (void * arg){
	 /* Como estou passando o endereço, precisamos usar o * para usar o valor do ponteiro*/
	int ident = * (int *) arg;
	printf("Olá, sou a thread %d!\n", ident);
	
        /* função que finaliza a thread e retorna seu valor*/	
	pthread_exit(NULL);
}

/* função principal */
int main(void){
	pthread_t tid[NTHREADS]; /* identificadores de cada thread no sistema*/
	int ident[NTHREADS];   /* identificador local da thread*/	
	int i;

	for(i = 0; i < NTHREADS; i++){	
		ident[i] = i + 1; /* 1 a 10  */
		if(pthread_create(&tid[i], NULL, tarefa, (void *)&ident[i]))
			printf("EROO -- pthread_create\n");
	}
	for(i = 0; i < NTHREADS; i++){	
		/* Função pthread_join() espera pelo término de uma thread específica recebendo 2 parâmentros.
		 * 1 - O identificador da thread que se deseja esperar o fim.
		 * 2 - É o retorno da tarefa que a thread espeficiada faz. Como a tarefa não retorna nada, então passaremos NULL.
		 * Caso a thread termine antes da pthread_join esperar por ela, ela apenas pega o retorno da thread. 
		 * Há uma sinalização do sistema que indica se a thread terminou ou não. 
		 */
		if(pthread_join(tid[i], NULL))
			printf("EROO -- pthread_create\n");
	}

	printf("Olá, sou a thread principal!\n");
	pthread_exit(NULL);	
	return 0;
}

