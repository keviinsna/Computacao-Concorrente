#include <stdio.h>
#include <pthread.h>

/* função que a thread irá executar */
void * tarefa (void * arg){
	printf("Olá, sou a thread nova!\n");
	
        /* função que finaliza a thread e retorna seu valor*/	
	pthread_exit(NULL);
}

/* função principal */
int main(void){
	pthread_t tid; /* identificador da thread no sistema*/
	
	/* Cria uma nova thread recebendo seus 4 parâmetros:
	 * 1 - O identificador da thread que será usado pelo SO e por nós para nos referirmos à tal thread.
	 * 2 - Atributos para a thread, tais como: tamanho da pilha de execução, questões de prioridades etc.
	 * 3 - função que será executada.
	 * 4 - argumentos da função que será executada
	*/	
	if(pthread_create(&tid, NULL, tarefa, NULL))
		printf("EROO -- pthread_create\n");
	
	printf("Olá, sou a thread principal!\n");

	/* Desvincula o término da mais do término do programa. 
	 * Caso a main termine antes, o programa continua ativo até que todas as threads terminem.
	 */
	pthread_exit(NULL);	
	return 0;
}

