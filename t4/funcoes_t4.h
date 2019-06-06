//STRUCT DO REG. DE CABEÇALHO
typedef struct cabecalhoT4{
	char status;	//1 byte, 0 quando abre, 1 quando fecha
	int nroRegistros; //armazena o numero de registros contidos em um arquivo
}CabecalhoT4;

//STRUCT DO REG. DE DADOS
typedef struct regDadosT4{
	char chaveBusca[120];	//chave de busca do indice
	long int byteOffset;	//byte offset do registro
}DadosT4;

void inicializaCabIndice(CabecalhoT4 *c);
char *getPaginaCabIndice(CabecalhoT4 c);
char *getPaginaIndice(DadosT4 d, int *offsetPagina, char *p);
DadosT4 copiaRegIndice(DadosT4 d1);
DadosT4 getIndice(char *p, int *offset);
void sortIndice(DadosT4 *d, int ini, int fim);
int buscaBinIndice(DadosT4 *d, char *busca, int ini, int fim);