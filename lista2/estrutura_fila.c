// Estrutura de cada nó da fila
typedef struct _NO{
        int info;             // informação genéricas das requisições
        struct _NO *proximo; //   Ponteiro para o próximo nó
}NO;

// Estrutura da lista
typedef struct _FILA{
        NO *inicio;  //  Ponteiro para o início da fila, onde sairão as requisições
        NO *fim;    //   Ponteiro para o fim da fila, onde entrarão novas requisições
}FILA;

/* Cria uma fila. */
FILA *cria_fila(){
        FILA *f = (FILA *) malloc(sizeof(FILA));
        if(f == NULL){
                fprintf(stderr, "ERRO -- cria_fila()\n");
                exit(1);
        }

        f->inicio = NULL;
        f->fim = NULL;

        return f;
}

/*
 * Insere um novo item no fim da fila.
 *
 * Parâmetros:
 * 	FILA *f:    Fila onde o novo item será inserido.
 * 	char *info: Informação/valor do novo item.
 */
void insere_fila(FILA *f, int info){
        NO *no = (NO *) malloc(sizeof(NO));
        if(no == NULL){
                fprintf(stderr, "ERRO -- insere_requisicao()\n");
                exit(1);
        }
	
	no->info = info;
        no->proximo = NULL;

        if(f->fim == NULL){ // Lista vazia
                f->inicio = no;
        }else{
                f->fim->proximo = no;
        }

        f->fim = no;
}

/*
 * Remove um item do início da fila.
 *
 * Parâmetros:
 * 	FILA *f: Fila de onde o primeiro item será removido.
 *
 * Retorno:
 * 	A informação/valor do primeiro item da fila.
 */
int remove_fila(FILA *f){
        NO *no_inicial;
	int info;
        
	no_inicial = f->inicio;
        info = no_inicial->info;
	f->inicio = f->inicio->proximo;

        if(f->inicio == NULL)
                f->fim = NULL;

	free(no_inicial);

        return info;
}

/*
 * Printa os itens da fila.
 *
 * Parâmetros:
 * 	FILA *f: Fila que será impressa na tela.
 */
void print_fila(FILA *f){
        NO *no;

	if(f->inicio != NULL){
		no = f->inicio;
	        do{
	                printf("%d\t", no->info);
       	         no = no->proximo;
       		}while(no != NULL);
		printf("\n\n");
	}else{
		printf("Lista vazia!\n\n");
	}
}

