// Soma os elementos de um vetor

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long int dim; // Dimensão do vetor de entrada
int nthreads; // Número de threads
float *vetor;   // Vetor de entrada

// Tarefa das threads
void * soma_vetor(void * arg){
	long int id = (long int) arg; // Identificador da thread

	/* Obs.: Como todo ponteiro tem 8 bytes e float tem 4, 
	 * a conversão de ponteiro de void * para float quebra, 
	 * pois não espaço suficiente para a conversão.
	 */
	float *soma_local; // Variável da soma de elementos

	long int tam_bloco = dim / nthreads; // Tamanho de cada bloco da thread
	long int ini = id * tam_bloco; // Elemento inicial do bloco da thread
	long int fim = (id == nthreads - 1)? dim : ini + tam_bloco; //Elemento final do bloco da thread (já tratando o resto)

	// Comom soma_local agora é ponteiro, devemos alocar memória
	soma_local = (float *) malloc(sizeof(float));
	if(soma_local == NULL){fprintf(stderr, "ERRO -- malloc()\n"); exit(1);}
	*soma_local = 0;

	for(long int i = ini; i < fim; i++)
		*soma_local += vetor[i];

	/* soma_local é um ponteiro, logo guarda o endereço da soma.
	 *  Então aqui a conversão fica tudo certo. 
	 */
	pthread_exit((void *) soma_local);
}


int main(int argc, char * argv[]){
	float soma_seq = 0; // Somatório elementos sequencial
	float soma_conc = 0; // Somatório elementos concorrente
	double ini, fim; // Tomada de tempo

	/* Como soma_local é um ponteiro lá na tarefa das threads, 
	 * e é o endereço do resultado que é retornado,
	 * então a variável retorno também deve ser um ponteiro.
	 */
	float *retorno; // Retorno do resultado de cada thread
	
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
	vetor = (float *) malloc(sizeof(float) * dim);
	if(vetor == NULL){
		fprintf(stderr, "ERRO -- malloc()\n");
	}

	// Preenche o vetor 
	for(long int i = 0; i < dim; i++)
		vetor[i] = 1000.1 / (i + 1);

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
		/* Não será necessário alocar memória para retorno, pois
		 * como retornamos o endereço de soma_local, a variável
		 * retorno já será preenchida com o valor, já que ela 
		 * também é um ponteiro para float e está recebendo 
		 * justamente o endereço do resultado
		 */
		if(pthread_join(*(tid + i), (void **) &retorno)){
			fprintf(stderr, "ERRO -- pthread_join()\n");
			return 3;
		}
		soma_conc += *retorno;
		
		// Libera soma_local automaticamente
		free(retorno);
	}
	GET_TIME(fim); 

	// Exibição dos resultados
	printf("Tempo concorrente: %lf\n", fim - ini);
	printf("Soma elementos sequenial:   %f\n", soma_seq);
	printf("Soma elementos concorrente: %f\n", soma_conc);

	// Libera memória
	free(vetor); free(tid);
	
	return 0;
}
