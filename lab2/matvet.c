/*Multiplicação de matriz-vetor (considenrando matrizes quadradas)*/

#include <stdio.h>
#include <stdlib.h>

float *mat; // matriz de entrada
float *vet; // vetor de entrada
float *saida; // vetor de saída

int main(int argc, char * argv[]){
	int dim; // dimensao da matriz de entrada

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
	for(int i = 0; i < dim; i++){
		for(int j = 0; j < dim; j++)
			saida[i] += mat[i * dim + j] * vet[j];
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

	free(mat); free(vet); free(saida);

	return 0;
}

