/*
 * Disciplina: Computação Concorrente
 * Professora: Silvana Rosseto
 * Módulo 2  - Laboratório 6
 * Atividade:  Implementar e projetar uma aplicação que requer 
 * 	       sincronização coletiva entre as threads.
 *
 * 	       A cada iteração as threads acumulam o somatório de
 * 	       todos os valores do vetor e depois cada uma altera 
 * 	       sua posição correspondente ao seu id. Em seguida, 
 * 	       segue a próxima iteração repetindo as duas tarefas.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Variáveis de sincronização
pthread_mutex_t lock;
pthread_cond_t	cond;

// Variáveis globais do problema
int *vet; // Vetor compartilhado pelas threads
int N; // Tamanho do vetor, número de threads e número de iterações
int bloqueadas = 0; // Auxiliar no funcionamento das barreiras

// Sincronização coletiva por barreiras
void barreira(int nthreads){
	// Entrada seção crítica
	pthread_mutex_lock(&lock);
	
	// Última thread desbloqueia todas as outras
	if(bloqueadas == (nthreads - 1)){
		bloqueadas = 0;
		pthread_cond_broadcast(&cond);
	}else{
		bloqueadas++;
		pthread_cond_wait(&cond, &lock);
	}

	// Saída seção crítica
	pthread_mutex_unlock(&lock);
}

// Tarefa das threads
void *tarefa(void *arg){
	long int id = (long int) arg;
	
	int *soma = (int *) malloc(sizeof(int));
	if(soma == NULL){ fprintf(stderr, "ERRO -- malloc()\n"); exit(1);}
	*soma = 0;

	for(int i = 0; i < N; i++){
		// Soma todos os elementos do vetor
		for(int j = 0; j < N; j++)
			*soma += *(vet + j);

		// Espera pelo término de todas até esse passo
		barreira(N);

		// Gera um número aleatório e guarda no vetor
		*(vet + id) = rand() % 10;

		// Espera pelo término de todas até esse passo
		barreira(N);
	
	}
	pthread_exit((void *) soma);
}

// Fluxo principal
int main(int argc, char *argv[]){
	pthread_t *tid;
	int *resultado_thread; // Retorno de cada thread
	int r, ok = 1; // Variáveis auxiliares para ajudar na verificação dos resultados das threads

	if(argc < 2){
		fprintf(stderr, "Digite: %s <número de elementos do vetor>\n", argv[0]);
		return 1;
	}

	N = atoi(argv[1]); // Converte string para int
	
	// Inicializa as variáveis de sincronização
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);

	// Aloca memória para os id's das threads e para o vetor principal
	tid = (pthread_t *) malloc(sizeof(pthread_t) * N);
	if(tid == NULL){ fprintf(stderr, "ERRO -- malloc()\n"); return 2;}
	
	vet = (int *) malloc(sizeof(int) * N);
	if(vet == NULL){ fprintf(stderr, "ERRO -- malloc()\n"); return 2;}
	
	// Inicializa o vetor
	srand(time(NULL));
	for(int i = 0; i < N; i++){
		vet[i] = rand() % 10;
	}
	
	// Cria as threads
	for(long int i = 0; i < N; i++){
		if(pthread_create((tid + i), NULL, tarefa, (void *) i)){
			fprintf(stderr, "ERRO -- pthread_create()\n");
			return 3;
		}
	}
	// Espera o término das threads
	for(long int i = 0; i < N; i++){
		if(pthread_join(*(tid + i), (void **) &resultado_thread)){
			fprintf(stderr, "ERRO -- pthread_join()\n");
			return 4;
		}

		// Verifica os valores
		if(!i) r = *resultado_thread;
		if(r != *resultado_thread) ok = 0;

		free(resultado_thread);
	}

	ok == 1? printf("Todas as threads encontraram o mesmo somatório: %d\n", r): printf("Resultados diferentes encontrados!\n");

	// Libera as variáveis de sincronização
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);

	// Libera as memórias alocadas
	free(vet); free(tid);

	return 0;
}
