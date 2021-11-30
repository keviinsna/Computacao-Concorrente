/*
 * Disciplina: Computação Concorrente
 * Professora: Silvana Rosseto
 * Módulo 1  - Laboratório 2
 * Atividade:  Projetar e implementar um algoritmo concorrente para o problema de
 *             multiplicação de matrizes e coletar informações sobre o seu tempo de execução
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *mat1;  // Matriz 1
float *mat2;  // Matriz 2
float *saida; // Matriz saída
int nthreads; // Número de threads


typedef struct{
	int id; // Idenfiticador do elemento que a thread irá processar
	int dim; // Dimensão das matrizes
}t_args;


// Tarefa das threads
void * mat_x_mat(void * arg){
	t_args *args = (t_args *) arg;
	int dim = args->dim;

	for(int i = args->id; i < dim; i += nthreads){
		for(int j = 0; j < dim; j++){
			for(int k = 0; k < dim; k++)
				saida[i * dim + j] += mat1[i * dim + k] * mat2[k * dim + j];
		}
	}
	pthread_exit(NULL);
}

// Produto matricial de forma sequencial
void mat_x_mat_sequencial(float *saida_seq, int dim){
	for(int i = 0; i < dim; i++){
		for(int j = 0; j < dim; j++){
			for(int k = 0; k < dim; k++)
				saida_seq[i * dim + j] += mat1[i * dim + k] * mat2[k * dim + j];
		}
	}
}

// Corretude das tarefas
void verifica_saida(float *saida_seq, int dim){
	for(int i = 0; i < dim; i++){
		for(int j = 0; j < dim; j++){
			if(saida[i * dim +j] != saida_seq[i * dim + j]){
				printf("Incoerência em [%d][%d]\n", i, j);
				return;
			}
		}
	}
	printf("Tudo certo!\n"); 
} 

int main(int argc, char * argv[]){
	int dim; // Dimensão das threads
	t_args *args; // Identificadores locais e dimensão das threads
	pthread_t *tid; // Identificadores das threads
	float *saida_seq; // Matriz saída código sequencial
	double inicio, fim, t_seq, t_conc; // Variáveis para calcular o tempo de execução


	// Leitura e verificação dos parâmetros de entrada
	if(argc < 3){
		printf("Digite: %s <dimensão das matrizes> <número de threads>\n", argv[0]);
		return 1;
	}
	dim  = atoi(argv[1]);
	nthreads = atoi(argv[2]); 
	if(nthreads > dim) nthreads = dim;

	// Alocação de memória para as estruturas
	mat1 = malloc(sizeof(float) * dim * dim);
	if(mat1 == NULL){ printf("ERRO -- malloc()\n"); return 2;}

	mat2 = malloc(sizeof(float) * dim * dim);
	if(mat2 == NULL){ printf("ERRO -- malloc()\n"); return 2;}
	
	saida = malloc(sizeof(float) * dim * dim);
	if(saida == NULL){ printf("ERRO -- malloc()\n"); return 2;}

	saida_seq = malloc(sizeof(float) * dim * dim);
	if(saida_seq == NULL){ printf("ERRO -- malloc()\n"); return 2;}

	// Inicialização das estruturas
	srand(time(NULL));
	for(int i = 0; i < dim; i ++){
		for(int j = 0; j < dim; j++){
			mat1[i * dim + j] = rand() % 201;
			mat2[i * dim + j] = rand() % 201;
			saida[i * dim + j] = 0;
		}
	}

	// Produto matricial
	
	// Alocação de memória das estruturas
	args = (t_args *) malloc(sizeof(t_args) * nthreads);
	if(args == NULL){ printf("ERRO -- malloc()\n"); return 2;}

	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if(tid == NULL){ printf("ERRO -- malloc()\n"); return 2;}

	// Criação das threads 
	GET_TIME(inicio);
	for(int i = 0; i < nthreads; i++){
		(args + i) -> id = i;
		(args + i) -> dim = dim;
		if(pthread_create((tid + i), NULL, mat_x_mat, (void *) (args + i))){
			printf("ERRO -- pthread_create()\n"); return 3;
		}
	}

	// Espera pelo término de cada thread
	for(int i = 0; i < nthreads; i++){
		pthread_join(*(tid + i), NULL);
	}
	GET_TIME(fim);
	t_conc = fim - inicio;
	
	printf("Conc: %.4lf\n", t_conc);

	// Cálculo tempo sequencial
	GET_TIME(inicio);
	mat_x_mat_sequencial(saida_seq, dim);
	GET_TIME(fim);
	
	t_seq = fim - inicio;

	printf("Seq: %.4lf\n", t_seq);

	printf("Desempenho: %.4lf\n", t_seq / t_conc);
	
	// Verificação das saídas
	verifica_saida(saida_seq, dim);
		
	// Liberação de memória
	free(mat1); free(mat2); free(saida); free(args); free(tid);
	
	return 0;
}

