// Soma os elementos de um vetor

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long int dim; // Dimensão do vetor de entrada
int nthreads; // Número de threads
int *vetor;   // Vetor de entrada

// Tarefa das threads
void * soma_vetor(void * arg){
	long int id = (long int) arg; // Identificador da thread
	long int soma_local; // Variável da soma de elementos
	long int tam_bloco = dim / nthreads; // Tamanho de cada bloco da thread
	long int ini = id * tam_bloco; // Elemento inicial do bloco da thread
	long int fim = (id == nthreads - 1)? dim : ini + tam_bloco; //Elemento final do bloco da thread (já tratando o resto)

	for(long int i = ini; i < fim; i++)
		soma_local += vetor[i];

	// Retorna o resultado da soma local
	pthread_exit((void *) soma_local);

}


int main(int argc, char * argv[]){
	long int soma_seq = 0; // Somatório elementos sequencial
	long int soma_conc = 0; // Somatório elementos concorrente
	double ini, fim; // Tomada de tempo
	long int retorno; // Retorno do resultado de cada thread
	pthread_t *tid; // Identificadores das threads do sistema

	// Recebe e verifica os valores de entrada
	if(argc < 3){
		// Permite escolher o dispositivo para a saída
		// stderr: saída padrão para erros
		fprintf(stderr, "Digite: %s <dimensão do vetor> <número de threads>\n", argv[0]);
		return 1;
	}
	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]);
	if(nthreads > dim) nthreads = dim;

	// Aloca memória para o vetor
	vetor = (int *) malloc(sizeof(int) * dim);
	if(vetor == NULL){
		fprintf(stderr, "ERRO -- malloc()\n");
	}

	// Preenche o vetor 
	for(long int i = 0; i < dim; i++)
		vetor[i] = i % 1000;

	GET_TIME(ini);
	// Soma sequencial dos elementos
	for(long int i = 0; i < dim; i++)
		soma_seq += vetor[i];
	GET_TIME(fim);
	printf("Tempo sequencial: %lf\n", fim - ini);


	// Soma concorrente dos elementos
	GET_TIME(ini);
	// Alocação de memória para a estrutura
	tid = (pthread_t *) malloc(sizeof(pthread_t) * dim);
	if(tid == NULL){
		fprintf(stderr, "ERRO -- malloc()\n");
		return 2;
	}

	// Cria as threads
	for(long int i = 0; i < nthreads; i++){
		if(pthread_create((tid + i), NULL, soma_vetor, (void *) i)){
			fprintf(stderr," ERRO -- pthread_create()\n");
			return 3;
		}
	}
	// Espera pelo fim das threads	
	for(long int i = 0; i < nthreads; i++){
		if(pthread_join(*(tid + i), (void **) &retorno)){
			fprintf(stderr, "ERRO -- pthread_join()\n");
			return 3;
		}
		soma_conc += retorno;
	}
	GET_TIME(fim); 

	// Exibição dos resultados
	printf("Tempo concorrente: %lf\n", fim - ini);
	printf("Soma elementos sequenial:   %ld\n", soma_seq);
	printf("Soma elementos concorrente: %ld\n", soma_conc);

	// Libera memória
	free(vetor); free(tid);
	
	return 0;
}
