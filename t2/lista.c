#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

//funcao que inicializa a lista
void inicializaLista(Lista *l){
	Node *ptr;

	ptr = (Node *) malloc(sizeof(Node));

	ptr->offset = -1;
	ptr->size = -1;
	ptr->prox = NULL;

	l->ini = ptr;
	l->fim = ptr;
}

//funcao que insere elementos na lista p/funcao getLista
void insereLista(Lista *l, long int off){
	Node *ptr;
	ptr = (Node *) malloc(sizeof(Node));

	if(ptr == NULL)
	return;

	ptr->offset = off;
	ptr->prox = NULL;
	
	l->fim->prox = ptr;
	l->fim = ptr;
}

//funcao que insere um elemento na lista
void insereListaTamanho(Lista *l, long int off, int size){
	Node *ptr, *aux;
	ptr = (Node *) malloc(sizeof(Node));

	if(ptr == NULL)
	return;

	ptr->offset = off;
	ptr->size = size;
	ptr->prox = NULL;
	
	
	aux = l->ini;
	while(aux != NULL){
		if(aux->prox != NULL){
			if(aux->prox->size >= ptr->size){
				ptr->prox = aux->prox;
				aux->prox = ptr;
				break;
			}
		}
		else{
			l->fim->prox = ptr;
			l->fim = ptr;
			break;
		}
		
		aux = aux->prox;
	}
}

//funcao que insere um elemento na lista
void insereListaTamanho2(Lista *l, long int off, int size){
	Node *ptr, *aux;
	ptr = (Node *) malloc(sizeof(Node));

	if(ptr == NULL)
	return;

	ptr->offset = off;
	ptr->size = size;
	ptr->prox = NULL;
	
	
	aux = l->ini;
	while(aux != NULL){
		if(aux->prox != NULL){
			if(aux->prox->size > ptr->size){
				ptr->prox = aux->prox;
				aux->prox = ptr;
				break;
			}
		}
		else{
			l->fim->prox = ptr;
			l->fim = ptr;
			break;
		}
		
		aux = aux->prox;
	}
}

//funcao que remove um elemento da lista
long int removeLista(Lista *l, int size){
	Node *ptr, *aux;
	long int off = -1;
	
	ptr = l->ini;
	aux = NULL;
	while(ptr != NULL){
		if(size <= ptr->size){
			off = ptr->offset;

			if(ptr == l->ini)
				l->ini = l->ini->prox;
			else{
				aux->prox = ptr->prox;

				if(ptr == l->fim)
					l->fim = aux;
			}
			free(ptr);
			return off;	//registro sera adicionado no lugar de um registro removido
		}
		aux = ptr;
		ptr = ptr->prox;
	}

	return off;	//registro sera adicionado no final
}

//funcao que obtem os elementos ja removidos
void getLista(FILE *fp, Lista *l){
	int size;
	long int off;

	fseek(fp, 1, SEEK_SET);

	fread(&off, 8, 1, fp);

	while(off != -1){
		insereLista(l, off);
		fseek(fp, off + 1, SEEK_SET);
		fread(&size, 4, 1, fp);
		fread(&off, 8, 1, fp);
		l->fim->size = size;
	}

	fseek(fp, 0, SEEK_SET);
}

//funcao que imprime a lista na tela
void printLista(Lista l){
	Node *ptr;

	ptr = l.ini;

	while(ptr != NULL){
		printf("%ld %d\n", ptr->offset, ptr->size);
		ptr = ptr->prox;
	}
}

//funcao que altera o topoLista e o encadeamentoLista de cada registro removido
void getEncadLista(FILE *fp, Lista l){
	Node *ptr;

	ptr = l.ini;
	
	while(ptr != NULL){
		fseek(fp, 0, SEEK_SET);


		if(ptr->prox == NULL){
			if(ptr == l.ini){
				fseek(fp, 1, SEEK_SET);
				fwrite(&l.ini->offset, 8, 1, fp);
			}
			else{
				fseek(fp, ptr->offset + 5, SEEK_SET);
				fwrite(&l.ini->offset, 8, 1, fp);
			}
		}
		else if(ptr == l.ini){
			fseek(fp, 1, SEEK_SET);
			fwrite(&ptr->prox->offset, 8, 1, fp);
		}
		else{
			fseek(fp, ptr->offset + 5, SEEK_SET);
			fwrite(&ptr->prox->offset, 8, 1, fp);
		}

		ptr = ptr->prox;
	}
}

//funcao que desaloca a lista
void desaloca(Lista *l){
	Node *ptr;

	ptr = l->ini;

	while(l->ini != NULL){
		l->ini = l->ini->prox;
		free(ptr);
		ptr = l->ini;
	}

	l->fim = NULL;
}