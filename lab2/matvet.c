/*Multiplicação de matriz-vetor (considenrando matrizes quadradas)*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

float *mat; // matriz de entrada
float *vet; // vetor de entrada
float *saida; // vetor de saída


typedef struct{
	int id; // Identificador do elemento que a thread irá processar
	int dim; // Dimensão das estruturas de entrada

}t_args;

// Função que as treads executarão
void * mat_x_vet(void * arg){
	/* Para cada nova linha da matriz, se cria uma nova thread e realiza o produto mat x vet */
	t_args *args = (t_args *) arg;
	for(int i = 0; i < args->dim; i++){
		saida[args->id] += mat[(args->id) * (args->dim) + i] * vet[i];	
	}
	pthread_exit(NULL);
}

int main(int argc, char * argv[]){
	int dim; // Dimensao da matriz de entrada
	pthread_t *tid; // Identificadores das threads no sistema
	t_args *args; // Identificadores locais e dimensão
	
	// Leitura e avaliação dos parâmetros de entrada
	if(argc < 2){
		printf("Digite: %s <dimensão da matriz\n", argv[0]);
		return 1;
	}
	dim = atoi(argv[1]); // Conversão de char para int


	// Alocação de memória para as estruturas de dados
	mat = malloc(sizeof(float) * dim * dim); // Multiplica por dim² para pegar todas as posições de mat
	if(mat == NULL){printf("ERRO -- malloc()\n"); return 2;}

	vet = malloc(sizeof(float) * dim);
	if(mat == NULL){printf("ERRO -- malloc()\n"); return 2;}

	saida = malloc(sizeof(float) * dim);
	if(saida == NULL){printf("ERRO -- malloc()\n"); return 2;}


	// Inicialização das estruturas de dados
	for(int i = 0; i < dim; i++){
		for(int j = 0; j < dim; j++)
			mat[i * dim + j] = 1; // Equivalente a mat[i][j]
		vet[i] = 1;
		saida[i] = 0;
	}

	// Multiplicação da matriz pelo vetor
	// Alocação das estruturas
	tid = (pthread_t *) malloc(sizeof(pthread_t) * dim);
	if(tid == NULL) {printf("EROO -- malloc\n"); return 2;}

	args = (t_args *) malloc(sizeof(t_args) * dim);
	if(args == NULL) {printf("EROO -- malloc\n"); return 2;}

	// Criação das threads
	for(int i = 0; i < dim; i++){
		(args + i) -> id = i;
		(args + i) -> dim = dim;
		if(pthread_create((tid + i), NULL, mat_x_vet, (void *) (args + i))){
			printf("ERRO -- pthread_create\n"); return 3;
		}
	}
	
	// Espera pelo término das threads	
	for(int i = 0; i < dim; i++){
		pthread_join(*(tid + i), NULL);
	}	

	// Exibiçãodos resultados
	printf("Matriz de entrada:\n");
	for(int i = 0; i < dim; i++){
		for(int j = 0; j < dim; j++)
			printf("%.1f%c", mat[i * dim + j], (j != dim - 1)? '\t':'\n');
	}

	printf("Vetor de entrada:\n");
	for(int j = 0; j < dim; j++)
		printf("%.1f%c", vet[j], (j != dim - 1)? '\t':'\n');

	printf("Vetor de saída:\n");
	for(int j = 0; j < dim; j++)
		printf("%.1f%c", saida[j], (j != dim - 1)? '\t':'\n');

	
	// Liberação de memória

	free(mat); free(vet); free(saida); free(args); free(tid);

	return 0;
}

