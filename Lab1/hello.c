#include <stdio.h>
#include <pthread.h>

#define NTHREADS 10

int vetor[NTHREADS];

/* função que a thread irá executar */
void * tarefa (void * arg){
	 /* Como estou passando o endereço, precisamos usar o * para usar o valor do ponteiro*/
	int ident = * (int *) arg;
	printf("Olá, sou a thread %d!\n", ident);

	vetor[ident - 1] = ident;	
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
		if(pthread_join(tid[i], NULL))
			printf("EROO -- pthread_create\n");
	}
	for(i = 0; i < NTHREADS; i++){	
		printf("%d%c", vetor[i], (i != NTHREADS - 1)? ' ' : '\n');
	}

	printf("Olá, sou a thread principal!\n");
	/*pthread_exit(NULL);	Não precisa mais forçar, já que o join já espera por todas as threads*/
	return 0;
}

