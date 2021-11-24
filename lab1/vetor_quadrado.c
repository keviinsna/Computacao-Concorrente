/*
 * Disciplina: Computação Concorrente
 * Professora: Silvana Rosseto
 * Módulo 1  - Laboratório 1
 * Atividade:  Implementar um programa concorrente, com duas threads (além da thread principal),  para elevar ao quandrado cada elemento de um vetor de 10000 elementos. Para cada ai do vetor, calcular (ai)² e escrever o resultado na mesma posição do elemento.
*/


#include <stdio.h>
#include <pthread.h>

#define TAM 10000
#define NTHREADS 2
#define NUM 3

int vetor[TAM];

void * quadrado(void * arg){
	int pos = *(int *) arg;
	for(pos = pos-1; pos < TAM; pos += 2){
		vetor[pos] *= vetor[pos];
	}

	pthread_exit(NULL);	
}

void verifica_vetor(int * vet){
	int i;
	for(i = 0; i < TAM; i++){
		if(vet[i] != NUM * NUM){
			printf("Erro -- vetor[%d] = %d\n", i, vet[i]);
			return;
		}
	}
	printf("Tudo certo!\n");
}

int main(void){	
	pthread_t tid[NTHREADS]; /*identificadores de cada thread*/
	int i, ident[NTHREADS]; /*identificadores locais de cada thread*/

	/*Inicialização do vetor*/
	for(i = 0; i < TAM; i++){
		vetor[i] = NUM;
	}

	/*Cria as duas threads*/
	for(i = 0; i < NTHREADS; i++){
		ident[i] = i;
		if(pthread_create(&tid[i], NULL, quadrado, (void *)&ident[i]))
			printf("ERRO -- pthread_create()\n");
	}

	/*Espera ambas as threads terminarem*/
	for(i = 0; i < NTHREADS; i++){
		if(pthread_join(tid[i], NULL))
			printf("ERRO -- pthread_join()\n");
	}

	/*Verificar vetor*/
	verifica_vetor(vetor);

	return 0;
}
