/* Problema dos produtores/consumidores e usando sincronização condicional
 * com variáveis de condição.
 */



#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5 // Tamanho do buffer
#define P 2 // Quantidade de threads produtoras
#define C 4 // Quantidade de threads consumidoras

// Variáveis do problema
int buffer[N]; // Espaço de dados compatilhado
int count = 0, in = 0, out = 0; 

pthread_mutex_t mutex; // Variável de sincronização por exclusão mútua
pthread_cond_t cond_cons, cond_prod; // Variáveis de sincronização por condição

// Inicializa o buffer
void inicializa_buffer(int n){
	for(int i = 0; i < n; i++)
		buffer[i] = 0;
}

// Imprime o buffer
void imprime_buffer(int n){
	for(int i = 0; i < n; i++)
		printf("%d%c", buffer[i], (i != n - 1)?'\t':'\n');
}

// Insere um elemento no buffer ou bloqueia a thread caso o buffer esteja cheio
void insere(int item, int id){
	// Exclusão mútua para evitar que o consumidor e o produtor acessem o buffer ao mesmo tempo
	pthread_mutex_lock(&mutex);
	printf("P[%d] quer inserir\n", id);

	/* Se o buffer estiver cheio, bloqueia a thread produtora e impede novas inserções.
	 *
	 * Além disso, é preciso que esteja dentro de um loop, pois quando uma thread é
	 * sinalizada (signal), ela não ganha a CPU automaticamente, então pode ocorrer de uma
	 * thread consumidora consumir e liberar um espaço novo no buffer, mas rapidamente
	 * vir outra thread produtora (já que há mais de uma) e encher o buffer novamente.
	 * Com isso, a thread que foi interrompida, ao ser liberada pelo signal vai encontrar
	 * o buffer cheio e não conseguirá inserir um novo produto. Dessa forma, o loop garante
	 * que quando a thread for sinalizada, ela terá que verificar novamente se pode avançar.
	 *
	 * Só poderia trocar o while por um if, se houvesse apenas 1 thread produtora. 
	 */
	while(count == N){
		printf("P[%d] bloqueou!\n", id);
		pthread_cond_wait(&cond_prod, &mutex);
		printf("P[%d] desbloqueou!\n", id);
	}
	
	buffer[in] = item;
	in  = (in + 1) % N;
	count++;
	printf("P[%d] inseriu!\n", id);
	 imprime_buffer(N);
	
	// Libera o lock
	pthread_mutex_unlock(&mutex);

	// Libera a thread consumidora (mesmo que não haja nenhuma na fila)
	pthread_cond_signal(&cond_cons);

}
// Retira um elemento do buffer ou bloqueia a thread caso o buffer esteja vazio
int retira(int id){
	int item;	
	// Exclusão mútua para evitar que o consumidor e o produtor acessem o buffer ao mesmo tempo
	pthread_mutex_lock(&mutex);
	printf("C[%d] quer consumir\n", id);

	while(count == 0){
		printf("C[%d] bloqueou!\n", id);
		pthread_cond_wait(&cond_cons, &mutex);
		printf("C[%d] desbloqueou!\n", id);
	}

	item = buffer[out];
	buffer[out] = 0;
	out = (out + 1) % N;
	count--;
	printf("C[%d] consumiu %d\n", id, item);
	imprime_buffer(N);
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond_prod);

	return item;
}

// Thread produtora
void *produtor(void *arg){
	int *id = (int *) arg;
	printf("Thread produtora.\n");

	while(1){
		insere(*id, *id);
		sleep(1);
	}
	free(arg);
	pthread_exit(NULL);
}

// Thread consumidora
void *consumidor(void *arg){	
	int *id = (int *) arg, item;
	printf("Thread consumidora.\n");

	while(1){
		item = retira(*id);
		sleep(1);
	}
	free(arg);
	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char *argv[] ){
	// Identificadores das threads
	pthread_t tid[P + C];
	int *id[P + C];

	// Aloca espaço para os ids das threads
	for(int i = 0; i < P + C; i++){
		id[i] = malloc(sizeof(int));
		if(id[i] == NULL) exit(-1);
		*id[i] = i + 1;
	}

	// Inicializa o buffer
	inicializa_buffer(N);

	// Inicializa as variáveis de sincronização
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_cons, NULL);
	pthread_cond_init(&cond_prod, NULL);

	// Cria as threads produtoras
	for(int i = 0; i < P; i++){
		if(pthread_create(&tid[i], NULL, produtor, (void *) id[i])) exit(-1);
	}

	// Cria as threads consumidoras
	for(int i = 0; i < C; i++){
		if(pthread_create(&tid[i + P], NULL, consumidor, (void *) id[i + P])) exit(-1);
	}

	pthread_exit(NULL);

	return 0;
}
