typedef struct node{
	long int offset;
	int size;
	struct node *prox;
}Node;

typedef struct lista{
	int tamanho;
	Node *ini, *fim;
}Lista;

void inicializaLista(Lista *l);
void insereListaTamanho(Lista *l, long int off, int size);
long int removeLista(Lista *l, int size);
void getLista(FILE *fp, Lista *l);
void printLista(Lista l);
void getEncadLista(FILE *fp, Lista l);