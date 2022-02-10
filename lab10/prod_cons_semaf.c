/* Disciplina: Computação Concorrente
 * Professora: Silvana Rossetto
 * Módulo 3  - Laboratório 10
 * Atividade: Implementar o problema clássico de produtor/consumidor, mas com uma variação.
 * 	      A cada execução de um produtor, ele preenche o buffer inteiro, e não apenas 
 * 	      um único item (para isso ele deve esperar o buffer ficar completamente vazio).
 * 	      O consumidor continua com a mesma logica, i.e., consome um item de cada vez.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5	// Tamanho do buffer

// Semáforos
sem_t buffer_cheio, buffer_vazio;  // Semáforos para controlar o buffer
sem_t mutex_cons;		   // Semáforos para exclusão mútua entre consumidores

// Variáveis compartilhadas
int buffer[N]; // Buffer compartilhado
int out = 0;   // Variável para remover item

// Função para retirar do buffer
int retira(int id){
	int item;

	printf("C[%d] quer consumir...\n", id);

	// Verifica se pode retirar do buffer (buffer não vazio)
	sem_wait(&buffer_vazio);

	// Exclusão mútua entre os consumidores
	sem_wait(&mutex_cons);	

	item = buffer[out];
	buffer[out] = 0;
	out = (out + 1) % N;

	printf("C[%d] consumiu o item: %d.\n\n", id, item);

	// Se o buffer estiver vazio, libera as threads produtoras
	if(out == 0)
		sem_post(&buffer_cheio); // Sinaliza que o buffer tá vazio

	sem_post(&mutex_cons);   // Sinaliza para outros consumidores

	return item;
}

// Função para preencher buffer totalmente
void preenche_buffer(int id){

	printf("P[%d] quer preencher o buffer...\n", id);
	
	/* Como buffer_cheio é um semáforo binário, ele pode ser usado
	 * tanto para verificação do buffer vazio quanto para mutex entre
	 * os produtores, já que somente 1 produtor pode preencher o buffer
	 * de cada vez.
	 */

	// Verifica se pode preencher o buffer
	sem_wait(&buffer_cheio);

	// Preenche o buffer e libera consumidor
	for(int i = 0; i < N; i++){
		buffer[i] = i;
	}
	
	printf("P[%d] preencheu o buffer!\n\n", id);
	
	// Libera as consumidores
	for(int i = 0; i < N; i++){
		sem_post(&buffer_vazio); // Sinaliza que slot pode ser consumido
	}
}

// Thread produtora
void *produtor(void *arg){
	long int id = (long int) arg;
	
	while(1){
		preenche_buffer(id);
		sleep(1);
	}

	pthread_exit(NULL);
}


// Thread consumidora
void *consumidor(void *arg){
	long int id = (long int) arg;

	while(1){
		retira(id);
		sleep(1);
	}
	pthread_exit(NULL);
}


// Fluxo principal
int main(int argc, char *argv[]){
	pthread_t *tid; // Identificadores das threads no SO
	int P;	// Quantidade de produtores
	int C;	// Quantidade de consumidores


	if(argc < 3){
		fprintf(stdout, "Digite: %s <N° threads produtoras> <N° threads consumidoras>\n", argv[0]);
		exit(1);
	}
	P = atoi(argv[1]);
	C = atoi(argv[2]);
	
	// Aloca memória para tid
	tid = (pthread_t *) malloc(sizeof(pthread_t) * (P + C));
	if(tid == NULL){
		fprintf(stderr, "ERRO -- malloc()\n");
		exit(1);
	}
	
	// Inicialização dos semáforos
	
	// Semáforos para o buffer
	sem_init(&buffer_cheio, 0, 0); // buffer_cheio (binário) será usado pelos produtores para verificar se pode produzir e mutex
	sem_init(&buffer_vazio, 0, N); // buffer_vazio será usado pelos consumidores para verificar se pode consumir
	
	// Semáforos para exclusão mútua
	sem_init(&mutex_cons, 0, 1);

	// Inicializa o buffer
	for(int i = 0; i < N; i++)
		buffer[i] = i;

	// Cria as threads
	for(long int i = 0; i < C; i++){
		if(pthread_create((tid + i), NULL, consumidor, (void *) (i + 1))){
			fprintf(stderr, "ERRO -- pthread_create()\n");
			return 1;
		}
	}

	for(long int i = 0; i < P; i++){
		if(pthread_create((tid + i + C), NULL, produtor, (void *) (i + 1))){
			fprintf(stderr, "ERRO -- pthread_create()\n");
			return 1;
		}
	}

	// Libera memória de tid
	free(tid);
	
	pthread_exit(NULL);

	return 0;
}


