//STRUCT DO REG. DE CABEÇALHO
typedef struct cabecalho{
	char status;	//1 byte, 0 quando abre, 1 quando fecha
	long topoLista; //igual a -1 quando nao ha removidos, 8 bytes
	char tagCampo1;	//1 byte, "i"
	char desCampo1[40]; //40 bytes, nao armazenar '\0'
	char tagCampo2;	//1 byte, "s"
	char desCampo2[40]; //40 bytes, nao armazenar '\0'
	char tagCampo3;	//1 byte, "t"
	char desCampo3[40]; //40 bytes, nao armazenar '\0'
	char tagCampo4;	//1 byte, "n"
	char desCampo4[40]; //40 bytes, nao armazenar '\0'
	char tagCampo5;	//1 byte, "c"
	char desCampo5[40]; //40 bytes, nao armazenar '\0'
}Cabecalho;

//STRUCT DO REG. DE DADOS
typedef struct regDados{
	char removido;					//1 byte, inicializar com '-'
	int tamanhoRegistro;			//4 bytes
	long encadeamentoLista;			//8 bytes, armazena byte offsets de registros removidos (inicializar com -1)
	int idServidor;					//4 bytes, nao aceita valores repetidos ou nulos
	double salarioServidor;			//8 bytes, aceita valores repetidos ou nulos (valor nulo = -1)
	char telefoneServidor[14];		//14 bytes, aceita valores repetidos ou nulos (valor nulo = ‘\0@@@@@@@@@’)
	int tamNomeServidor;			//indicador de tamanho, 4 bytes (caso nulo nao armazenar)
	char tagCampo4;					//caso nulo nao armazenar
	char *nomeServidor;				//tam variavel, armazenar '\0', lixo = @, aceita valores repetidos ou nulos (caso nulo nao armazenar)
	int tamCargoServidor;			//indicador de tamanho, 4 bytes (caso nulo nao armazenar)
	char tagCampo5;					//caso nulo nao armazenar
	char *cargoServidor;			//tam variavel, armazenar '\0', lixo = @, aceita valores repetidos ou nulos (caso nulo nao armazenar)
}Dados;

char *readLine();
Cabecalho inicializaCab();
Cabecalho getCabecalho(FILE *arquivoLido, Cabecalho c);
Cabecalho ajustaCabecalho(Cabecalho c);
Dados inicializaRegDados();
Dados getDados(FILE *arquivoLido, Dados d);
void printDados2(Dados d);
void printDados3(Cabecalho c, Dados d);
char* getPaginaCab(Cabecalho c);
char* getPaginaDados(Dados d, char* pagina, int *offset, int *offsetTamRegistro);
Dados copiaRegistro(char *pagina, int *offset);
Dados copiaRegistro2(Dados d1);
Dados moveRegistro(Dados d1);
void atualizaTamanho(Dados *d);
void copiaArquivo(FILE *arquivoBin, FILE *novoArquivoBin, int paginasAcessadas);
int inicializa(FILE **arquivoBin, char *nomeArquivo, char **paginaCab);
void mergesort(Dados *d, int ini, int fim);
int buscaBin(Dados *d, int id, int ini, int fim);
void pulaRemovido(int *offset, char *pagina);