/*
 * Disciplina: Computação Concorrente
 * Professora: Silvana Rosseto
 * Módulo 1  - Laboratório 3
 * Atividade: Implementar uma versão concorrente e uma sequencial 
 * 	      para o problema de encontrar a quantidade de valores
 * 	      de um vetor de números reais que estão dentro de uma
 * 	      faixa de valores dada. Em seguida, analisar e avaliar
 * 	      o desempenho em termos de tempo de execução
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "timer.h"

#define NUM 100000

long int dim; // Dimensão do vetor
int nthreads; // Número de threads
float *vetor; // Vetor de entrada

// Estrutura que será passada para a tarefa das threads
typedef struct{
	int id; // Identificador do elemento que a thread irá processar
	float l_inferior, l_superior; // Limiar inferior e limiar superior
}t_args;

// Inicializa o vetor de entrada
void inicializa_vetor(){
	srand(time(NULL));
	for(long int i = 0; i < dim; i++)
		vetor[i] = ((float) rand() / (float) RAND_MAX) * NUM;
}

// Tarefa que as thredas irão executar
void * tarefa(void * arg){
	t_args *args = (t_args *) arg;

	/* Como todo ponteiro tem 8 bytes e int apenas 4,
	 * a conversão de ponteiro de void para int quebra, 
	 * pois não há espaço suficiente para a conversão.
	 * Então, é necessário que qtd_local seja um ponteiro.
	 */

	int *qtd_local;
	long int tam_bloco = dim / nthreads; // Tamanho de cada bloco da thread
	long int ini = args->id * tam_bloco; // Início do bloco da thread
	long int fim = (args->id == nthreads - 1)? dim : ini + tam_bloco; // Fim do bloco da thread

	// Como qtd_local agora é ponteiro, devemos alocar memória
	qtd_local = (int *) malloc(sizeof(int));
	if(qtd_local == NULL){fprintf(stderr, "ERRO -- malloc()\n"); exit(1);}
	*qtd_local = 0;

	for(long int i = ini; i < fim; i++){
		if(args->l_inferior < vetor[i] && vetor[i] < args->l_superior)
			(*qtd_local)++;
	}
	//printf("Thread %d encontrou %d valores\n", args->id,*qtd_local);
	
	// Retorna quantidade
	pthread_exit((void *) qtd_local);
}

// Fluxo principal
int main(int argc, char *argv[]){
	float l_inferior, l_superior; // Limiar inferior e limiar superior, respectivamente
	int qtd_sequencial = 0, qtd_concorrente = 0;  // Quantidade de valores dentro do intervalo
	
	pthread_t *tid; // Identificador das threads no sistema
	t_args *args; // Estrutura com os identificadores locais e os limares
	
	/* Como qtd_local é um ponteiro lá na tarefa das threads,
	 * e é o endereço de memória do resultado que é retornado,
	 * então a variável retorno também deve ser um ponteiro.
	 * */
	int *retorno; // Retorno do resultado de cada thread

	double ini, fim, t_seq, t_conc; // Tomada de tempo

	// Recebe e verifica os valores de entrada
	if(argc < 5){
		fprintf(stderr, "Digite: %s <Dimensão do vetor> <Número de threads> <Limiar inferior> <Limiar superior>\n", argv[0]);
		return 1;
	}
	dim = atoll(argv[1]);
	nthreads = atoi(argv[2]);
	l_inferior = atof(argv[3]);
	l_superior = atof(argv[4]);

	// Aloca memória para o vetor
	vetor = (float *) malloc(sizeof(float) * dim);
	if(vetor == NULL){
		fprintf(stderr, "ERRO -- malloc()\n");
		return 2;
	}

	// Inicializa o vetor
	inicializa_vetor();

	// Calcula o número de valores dentro do intervalo de modo sequencial e calcula o tempo
	GET_TIME(ini);
	for(long int i = 0; i < dim; i++){
		if(l_inferior < vetor[i] && vetor[i] < l_superior)
			qtd_sequencial++; 
	}
	GET_TIME(fim);
	t_seq = fim - ini;
	
	// Cálculo do número de valores dentro do intervalo de modo concorrente	
	// Aloca memória para as estruturas
	args = (t_args *) malloc(sizeof(t_args) * nthreads);
	if(args == NULL){fprintf(stderr, "erro -- malloc()\n"); return 2;}

	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if(tid == NULL){fprintf(stderr, "erro -- malloc()\n"); return 2;}

	// Cria as threads
	GET_TIME(ini);
	for(int i = 0; i < nthreads; i++){
		(args + i)->id = i;
		(args + i)->l_inferior = l_inferior;
		(args + i)->l_superior = l_superior;
		if(pthread_create((tid + i), NULL, tarefa, (void *)(args + i))){
			fprintf(stderr, "ERRO -- pthrea_create()\n");
			return 3;
		}
	}

	// Espera o término de cada thread
	for(int i = 0; i < nthreads; i++){
		if(pthread_join(*(tid + i), (void **) &retorno)){
			fprintf(stderr, "ERRO -- pthread_join()\n");
			return 4;
		}
		qtd_concorrente += *retorno;
		free(retorno);
	}
	GET_TIME(fim);
	t_conc = fim - ini;

	// Confere resultados
	if(qtd_sequencial == qtd_concorrente)
		printf("Tudo certo!\n");
	else
		printf("ERRO -- Cálculos diferentes!\n");

	printf("Tempo sequencial: %lf\n", t_seq);
	printf("Tempo concorrente: %lf\n", t_conc);
	printf("Desempenho: %lf\n", t_seq / t_conc);
	
/*	printf("Número de valores dentro do intervalo (Seq):  %d\n", qtd_sequencial);
	printf("Número de valores dentro do intervalo (Conc): %d\n", qtd_concorrente);
*/
	// Libera memória
	free(vetor); free(tid); free(args);
	return 0;
}


