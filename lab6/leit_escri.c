/* Problema clássico de leitores e escritores usando 
 * sincronização coletiva (barreiras).
 *
 * A aplicação é interrompida ao dar ctrl+c
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define L 4 // Número de leitores
#define E 2 // Número de escritores

// Variáveis de sincronização
pthread_mutex_t lock;
pthread_cond_t  cond_leit, cond_escr;

// Variáveis globais do programa
int leit = 0;
int escr = 0;

// Entrada leitura
void entrada_leitura(int id){
	// Entrada seção crítica
	pthread_mutex_lock(&lock);

	printf("L[%d] quer ler.\n", id);

	// Se tiver alguém escrevendo
	/* While pois se algum escritor terminar sua escrita,
	 * pode ocorrer de outro escritor pegar a CPU e começar
	 * a escrever, então é preciso ficar verificando até
	 * estar disponível.
	 */
	while(escr > 0){
		printf("L[%d] bloqueado!\n", id);
		pthread_cond_wait(&cond_leit, &lock);
		printf("L[%d] desbloqueado!\n", id);
	}
	leit++;
	
	// Saída seção crítica
	pthread_mutex_unlock(&lock);
}

// Saída leitura
void saida_leitura(int id){
	// Entrada seção crítica
	pthread_mutex_lock(&lock);

	printf("L[%d] leu!\n", id);
	
	// Se não houver mais leitores lendo
	if(--leit == 0)
		pthread_cond_signal(&cond_escr);

	// Saída seção crítica
	pthread_mutex_unlock(&lock);
}

// Entrada escrita
void entrada_escrita(int id){
	// Entrada seção crítica
	pthread_mutex_lock(&lock);

	printf("E[%d] quer escrever!\n", id);

	// Se houver algum leitor lendo ou algum escritor escrevendo
	while(leit > 0 || escr > 0){
		printf("E[%d] bloqueado!\n", id);
		pthread_cond_wait(&cond_escr, &lock);
		printf("E[%d] desbloqueado!\n", id);
	}
	
	escr++;

	// Saída seção crítica
	pthread_mutex_unlock(&lock);
}

// Saída escrita
void saida_escrita(int id){
	// Entrada seção crítica
	pthread_mutex_lock(&lock);

	printf("E[%d] escreveu!\n", id);
	
	escr--;

	pthread_cond_broadcast(&cond_leit);
	pthread_cond_signal(&cond_escr);

	// Saída seção crítica
	pthread_mutex_unlock(&lock);
	
}

// Tarefa threads leitoras
void *leitor(void *arg){
	int id = *(int *)arg;

	while(1){
		entrada_leitura(id);
		printf("Leitora %d está lendo...\n", id);
		saida_leitura(id);
		sleep(1);
	}

	pthread_exit(NULL);
}

// Tarefa threads escritoras
void *escritor(void *arg){
	int id = *(int *)arg;

	while(1){
		entrada_escrita(id);
		printf("Escritora %d está escrevendo...\n", id);
		saida_escrita(id);
		sleep(1);
	}

	pthread_exit(NULL);
}

// Fluxo principal
int main(int argc, char *argv[]){	
	pthread_t tid[E + L];
	int id[E + L];

	// Inicializa as variáveis de sincronização
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond_escr, NULL);
	pthread_cond_init(&cond_leit, NULL);


	// Cria as threads leitoras
	for(int i = 0; i < L; i++){
		id[i] = i + 1;
		if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i])){
			fprintf(stderr, "ERRO -- pthread_create()\n");
			return 1;
		}
	}
	
	// Cria as threads escritoras
	for(int i = 0; i < E; i++){
		id[i + L] = i + 1;
		if(pthread_create(&tid[i + L], NULL, escritor, (void *) &id[i + L])){
			fprintf(stderr, "ERRO -- pthread_create()\n");
			return 3;
		}
	}

	pthread_exit(NULL);
	return 0;

}

