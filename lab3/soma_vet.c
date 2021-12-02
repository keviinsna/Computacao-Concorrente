// Soma os elementos de um vetor

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long int dim; // Dimensão do vetor de entrada
int nthreads; // Número de threads
int *vetor;   // Vetor de entrada

int main(int argc, char * argv[]){
	long int soma_seq = 0; // Somatório elementos sequencial
	double ini, fim; // Tomada de tempo

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
	printf("Soma elementos: %ld\n", soma_seq);

	// Cria as threads
	
	// Espera pelo fim das threads	

	// Libera memória
	free(vetor);
	
	return 0;
}
