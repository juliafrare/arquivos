#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Aluno: Julia Carolina Frare Peixoto
//nUSP: 10734727

//STRUCT DO REG. DE CABEÇALHO
typedef struct cabecalho{
	char status;	//1 byte, 0 quando abre, 1 quando fecha
	long topoLista; //sempre igual a -1, 8 bytes
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

//STRUCT DA PAGINA DE DISCO
typedef struct pagDisco{
	char p[32000];		//representa a pagina de disco em si
	int ocupado;		//indica quantos bytes da pagina estao ocupados
}PagDisco;

//funcao para ler uma string ate a quebra de linha, utilizada para ler a ultima entrada (valor) da funcionalidade 3
char *readLine(){
	char *str;
	int i = 0, flag = 0;
	
	str = (char *)malloc(sizeof(char));
	
	while(flag == 0){
		if(i == 0){
			scanf("%c", &str[i]);
			i++;
			str = (char *)realloc(str, sizeof(char) * i+1);
		}
		else if(i > 0){
			scanf("%c", &str[i]);
			i++;
			str = (char *)realloc(str, sizeof(char) * i+1);
		}
		if(str[i-1] == '\n'){
			flag = 1;
			str[i-1] = '\0';
		}
		if(str[i-1] == '\r'){
			flag = 1;
			str[i-1] = '\0';
		}
	}
	
	return str;
}

//funcao para ler uma string ate fechar aspas, utilizada para ler a penultima entrada (valor) da funcionalidade 5
char *readLine2(){
	char *str;
	int i = 0, flag = 0;
	
	str = (char *)malloc(sizeof(char));
	
	while(flag == 0){
		if(i == 0){
			scanf("%c", &str[i]);
			i++;
			str = (char *)realloc(str, sizeof(char) * i+1);
		}
		else if(i > 0){
			scanf("%c", &str[i]);
			i++;
			str = (char *)realloc(str, sizeof(char) * i+1);
		}
		if(str[i-1] == ' '){
            if(str[i-2] == '\"'){
                flag = 1;
			    str[i-1] = '\0';
            }
		}
	}
	
	return str;
}

//funcao para ler uma string ate um espacos, utilizada para ler entradas da funcionalidade 5
char *readLine3(){
	char *str;
	int i = 0, flag = 0;
	
	str = (char *)malloc(sizeof(char));
	
	while(flag == 0){
		if(i == 0){
			scanf("%c", &str[i]);
			i++;
			str = (char *)realloc(str, sizeof(char) * i+1);
		}
		else if(i > 0){
			scanf("%c", &str[i]);
			i++;
			str = (char *)realloc(str, sizeof(char) * i+1);
		}
		if(str[i-1] == ' '){
            flag = 1;
			str[i-1] = '\0';
		}
	}
	
	return str;
}

//funcao que inicializa o registro de cabecalho
Cabecalho inicializaCab(){		
	Cabecalho c;

	c.topoLista = -1;			//topoLista sera sempre -1
	c.tagCampo1 = 'i';			//tagCampo1 sera sempre "i"
	c.tagCampo2 = 's';			//tagCampo2 sera sempre "s"
	c.tagCampo3 = 't';			//tagCampo3 sera sempre "t"
	c.tagCampo4 = 'n';			//tagCampo4 sera sempre "n"
	c.tagCampo5 = 'c';			//tagCampo5 sera sempre "c"

	return c;
}

//funcao que obtem o registro de cabecalho do arquivo .csv
Cabecalho getCabecalho(FILE *arquivoLido, Cabecalho c){		
	int n;

	//obtencao do c.desCampo1
	n = fscanf(arquivoLido, "%[^,],", c.desCampo1);
	if(n == 0){
		c.desCampo1[0] = '\0';
		fscanf(arquivoLido, ",");
	}

	//obtencao do c.desCampo2
	n = fscanf(arquivoLido, "%[^,],", c.desCampo2);
	if(n == 0){
		c.desCampo2[0] = '\0';
		fscanf(arquivoLido, ",");
	}

	//obtencao do c.desCampo3
	n = fscanf(arquivoLido, "%[^,],", c.desCampo3);
	if(n == 0){
		c.desCampo3[0] = '\0';
		fscanf(arquivoLido, ",");
	}

	//obtencao do c.desCampo4
	n = fscanf(arquivoLido, "%[^,],", c.desCampo4);
	if(n == 0){
		c.desCampo4[0] = '\0';
		fscanf(arquivoLido, ",");
	}

	//obtencao do c.desCampo5
	n = fscanf(arquivoLido, "%[^\r\n]", c.desCampo5);
	if(n == 0){
		c.desCampo5[0] = '\0';
		fscanf(arquivoLido, ",");
	}

	return c;
}

//funcao que ajusta o registro de cabecalho para escrever no binario
Cabecalho ajustaCabecalho(Cabecalho c){		
	int i;

	for(i = strlen(c.desCampo1) + 1; i < sizeof(c.desCampo1); i++){
		c.desCampo1[i] = '@';
	}
	for(i = strlen(c.desCampo2) + 1; i < sizeof(c.desCampo2); i++){
		c.desCampo2[i] = '@';
	}
	for(i = strlen(c.desCampo3) + 1; i < sizeof(c.desCampo3); i++){
		c.desCampo3[i] = '@';
	}
	for(i = strlen(c.desCampo4) + 1; i < sizeof(c.desCampo4); i++){
		c.desCampo4[i] = '@';
	}
	for(i = strlen(c.desCampo5) + 1; i < sizeof(c.desCampo5); i++){
		c.desCampo5[i] = '@';
	}

	return c;
}

//funcao que inicializa o registro de dados
Dados inicializaRegDados(){
	Dados r;

	r.tamanhoRegistro = sizeof(r.encadeamentoLista) + sizeof(r.idServidor) + sizeof(r.salarioServidor) + sizeof(r.telefoneServidor) + sizeof(r.tamNomeServidor) + sizeof(r.tagCampo4) + sizeof(r.tamCargoServidor) + sizeof(r.tagCampo5);
	r.removido = '-';				//registro sera sempre inicializado com "-"
	r.encadeamentoLista = -1;		//encadeamentoLista sera sempre -1
	r.idServidor = -1;
	r.salarioServidor = -1;
	r.tamNomeServidor = -1;
	r.tagCampo4 = 'n';			//tagCampo4 sera sempre "n"
	r.tamCargoServidor = -1;
	r.tagCampo5 = 'c';			//tagCampo5 sera sempre "c"

	return r;

}

//funcao que obtem o registro de dados do csv
Dados getDados(FILE *arquivoLido, Dados d){		
	char charNome[100], charCargo[100];	//funcoes auxiliares para a obtencao dos campos nomeServidor e cargoServidor
	int n;	//n sera o valor de retorno do fscanf, que sera zero em caso de campos vazios

	//obtencao do idServidor
	n = fscanf(arquivoLido, "%d,", &d.idServidor);
	if(n == 0){
		d.idServidor = -1;		//identifica linha vazia/invalida, uma vez q o idServidor nao aceita valores nulos/vazios
		return d;
	}

	//obtencao do salarioServidor
	fscanf(arquivoLido, "%lf,", &d.salarioServidor);

	//obtencao do telefoneServidor
	n = fscanf(arquivoLido, "%[^,],", d.telefoneServidor);
	if(n == 0){
		d.telefoneServidor[0] = '\0';
		for(int i = 1; i < sizeof(d.telefoneServidor); i++)
			d.telefoneServidor[i] = '@';
		fscanf(arquivoLido, ",");
	}

	//obtencao do nomeServidor e tamNomeServidor
	n = fscanf(arquivoLido, "%[^,],", charNome);
	if(n == 0){
		/*caso o campo estiver vazio, a variavel nomeServidor da struct sera preenchida apenas com '\0'
		e tamNomeServidor sera strlen(nomeServidor), que nesse caso sera sempre zero. isso sera levado em
		consideracao na hora de copiar os dados para a pagina (se tamNomeServidor == 0, as variaveis
		tamNomeServidor, tagCampo4 e nomeServidor nao serao copiadas).*/
		d.nomeServidor = malloc(sizeof(char));
		d.nomeServidor[0] = '\0';
		d.tamNomeServidor = strlen(d.nomeServidor);
		fscanf(arquivoLido, ",");
		d.tamanhoRegistro -= 5;	//subtrair os campos tamNomeServidor e tagCampo4 do tamanho do registro
	}
	else{
		d.nomeServidor = malloc(sizeof(char) * (strlen(charNome) + 1));
		strcpy(d.nomeServidor, charNome);
		d.tamNomeServidor = strlen(d.nomeServidor) + 2;
		d.tamanhoRegistro += d.tamNomeServidor - 1;
	}

	//obtencao do cargoServidor e tamCargoServidor
	n = fscanf(arquivoLido, "%[^\r\n],", charCargo);
	if(n == 0){
		/*caso o campo estiver vazio, a variavel cargoServidor da struct sera preenchida apenas com '\0'
		e tamCargoServidor sera strlen(cargoServidor), que nesse caso sera sempre zero. isso sera levado em
		consideracao na hora de copiar os dados para a pagina (se tamCargoServidor == 0, as variaveis 
		tamCargoServidor, tagCampo5 e cargoServidor nao serao copiadas).*/
		d.cargoServidor = malloc(sizeof(char));
		d.cargoServidor[0] = '\0';
		d.tamCargoServidor = strlen(d.cargoServidor);
		fscanf(arquivoLido, ",");
		d.tamanhoRegistro -= 5;	//subtrair os campos tamCargoServidor e tagCampo5 do tamanho do registro
	}
	else{
		d.cargoServidor = malloc(sizeof(char) * (strlen(charCargo) + 1));
		strcpy(d.cargoServidor, charCargo);
		d.tamCargoServidor = strlen(d.cargoServidor) + 2;
		d.tamanhoRegistro += d.tamCargoServidor - 1;
	}

	return d;
}

//funcao que imprime os dados da struct Dados (funcionalidade 2)
void printDados2(Dados d){	
	int i;

	printf("%d ", d.idServidor);

	if(d.salarioServidor != -1)
		printf("%.2lf ", d.salarioServidor);
	else
		printf("         ");
	

	if(d.telefoneServidor[0] == '('){
		for(i = 0; i < sizeof(d.telefoneServidor); i++)
			printf("%c", d.telefoneServidor[i]);
		printf(" ");
	}
	else{
		printf("               ");
	}

	if(d.tamNomeServidor != -1){
		printf("%d ", d.tamNomeServidor - 2);
		printf("%s ", d.nomeServidor);
	}

	if(d.tamCargoServidor != -1){
		printf("%d ", d.tamCargoServidor - 2);
		printf("%s", d.cargoServidor);
	}

	printf("\n");

}

//funcao que imprime os dados da struct Dados (funcionalidade 3)
void printDados3(Cabecalho c, Dados d){	
	int i;

	printf("%s: %d\n", c.desCampo1, d.idServidor);

	if(d.salarioServidor != -1)
		printf("%s: %.2lf\n", c.desCampo2, d.salarioServidor);
	else
		printf("%s: valor nao declarado\n", c.desCampo2);
	

	if(d.telefoneServidor[0] == '('){
		printf("%s: ", c.desCampo3);
		for(i = 0; i < sizeof(d.telefoneServidor); i++)
			printf("%c", d.telefoneServidor[i]);
		printf("\n");
	}
	else{
		printf("%s: valor nao declarado\n", c.desCampo3);
	}

	if(d.tamNomeServidor != -1){
		printf("%s: %s\n", c.desCampo4, d.nomeServidor);
	}
	else{
		printf("%s: valor nao declarado\n", c.desCampo4);
	}

	if(d.tamCargoServidor != -1){
		printf("%s: %s\n", c.desCampo5, d.cargoServidor);
	}
	else{
		printf("%s: valor nao declarado\n", c.desCampo5);
	}

	printf("\n");

}

//funcao que obtem a pagina de dados do cabecalho
char* getPaginaCab(Cabecalho c){		
	int offset = 0;
	char *pagina;

	pagina = (char *) malloc(sizeof(char) * 32000);

	//copia o conteudo do registro de cabecalho para a pagina de disco

	//copia do c.status
	memcpy(&pagina[offset], &c.status, 1);
	offset += 1;

	//copia do c.topoLista
	memcpy(&pagina[offset], &c.topoLista, 8);
	offset += 8;

	//copia do c.tagCampo1
	memcpy(&pagina[offset], &c.tagCampo1, 1);
	offset += 1;

	//copia do c.desCampo1
	memcpy(&pagina[offset], &c.desCampo1, 40);
	offset += 40;

	//copia do c.tagCampo2
	memcpy(&pagina[offset], &c.tagCampo2, 1);
	offset += 1;

	//copia do c.desCampo2
	memcpy(&pagina[offset], &c.desCampo2, 40);
	offset += 40;

	//copia do c.tagCampo3
	memcpy(&pagina[offset], &c.tagCampo3, 1);
	offset += 1;

	//copia do c.desCampo3
	memcpy(&pagina[offset], &c.desCampo3, 40);
	offset += 40;

	//copia do c.tagCampo4
	memcpy(&pagina[offset], &c.tagCampo4, 1);
	offset += 1;

	//copia do c.desCampo4
	memcpy(&pagina[offset], &c.desCampo4, 40);
	offset += 40;

	//copia do c.tagCampo5
	memcpy(&pagina[offset], &c.tagCampo5, 1);
	offset += 1;

	//copia do c.desCampo5
	memcpy(&pagina[offset], &c.desCampo5, 40);
	offset += 40;

	for(int i = offset; i < 32000; i++)
		pagina[i] = '@';	//preenche o resto da pagina com o indicador de lixo

	offset = 32000;

	return pagina;
}

//funcao que obtem paginas de dados do registro de dados
char* getPaginaDados(Dados d, char* pagina, int *offset, int *offsetTamRegistro){		

	//copia do conteudo do registro de dados para a pagina de disco

	//copia do d.removido
	memcpy(&pagina[*offset], &d.removido, 1);
	*offset += 1;
	*offsetTamRegistro = *offset;

	//copia do d.tamanhoRegistro
	memcpy(&pagina[*offset], &d.tamanhoRegistro, 4);
	*offset += 4;

	//copia do d.encadeamentoLista
	memcpy(&pagina[*offset], &d.encadeamentoLista, 8);
	*offset += 8;

	//copia do d.idServidor
	memcpy(&pagina[*offset], &d.idServidor, 4);
	*offset += 4;

	//copia do d.salarioServidor
	memcpy(&pagina[*offset], &d.salarioServidor, 8);
	*offset += 8;

	//copia do d.telefoneServidor
	memcpy(&pagina[*offset], &d.telefoneServidor, 14);
	*offset += 14;

	//copia do d.tamNomeServidor, d.tagCampo4 e d.nomeServidor
	if(d.tamNomeServidor != 0){		//se tamNomeServidor == 0, nada sera copiado (ver funcao getDados)
		memcpy(&pagina[*offset], &d.tamNomeServidor, 4);
		*offset += 4;
		memcpy(&pagina[*offset], &d.tagCampo4, 1);
		*offset += 1;
		memcpy(&pagina[*offset], d.nomeServidor, strlen(d.nomeServidor) + 1);
		*offset += strlen(d.nomeServidor) + 1;
	}

	//copia do d.tamCargoServidor, d.tagCampo5 e d.cargoServidor
	if(d.tamCargoServidor != 0){	//se tamCargoServidor == 0, nada sera copiado (ver funcao getDados)
		memcpy(&pagina[*offset], &d.tamCargoServidor, 4);
		*offset += 4;
		memcpy(&pagina[*offset], &d.tagCampo5, 1);
		*offset += 1;
		memcpy(&pagina[*offset], d.cargoServidor, strlen(d.cargoServidor) + 1);
		*offset += strlen(d.cargoServidor) + 1;
	}

	return pagina;
}

//funcionalidade 1
void criaArquivoBin(char *nomeArquivo){
	char registro[1000];
	char *paginaCab;
	FILE *arquivoLido, *arquivoBin;
	Cabecalho c;

	//abrindo os arquivos
	arquivoBin = fopen("arquivoTrab1.bin", "wb");
	arquivoLido = fopen(nomeArquivo, "r");
	
	//caso de erro: um dos arquivos nao existe
	if(arquivoLido == NULL || arquivoBin == NULL){
		printf("Falha no carregamento do arquivo.");
		if(arquivoLido != NULL)
			fclose(arquivoLido);
		if(arquivoBin != NULL)
			fclose(arquivoBin);
		return;
	}

	c = inicializaCab();		//inicaliza o registro de cabecalho
	c.status = '0';				//status '0' para indicar que o arquivo binario esta aberto

	//obtencao do registro de cabecalho
	c = getCabecalho(arquivoLido, c);
	c = ajustaCabecalho(c);

	//obtencao da pagina de cabecalho
	paginaCab = getPaginaCab(c);
	fwrite(paginaCab, 32000, 1, arquivoBin);

	//obtencao das demais paginas
	while(!feof(arquivoLido)){
		char *pagina;
		int offset = 0, offsetTamRegistro = 0, tamRegistro = 0, tamRegAnterior = 0;
		fpos_t posAnterior;

		pagina = (char *) malloc(sizeof(char) * 32000);

		while(offset < 32000){
			Dados d;
			d = inicializaRegDados();
			fgetpos(arquivoLido, &posAnterior);
			tamRegAnterior = tamRegistro;
			d = getDados(arquivoLido, d);
			tamRegistro = d.tamanhoRegistro;
			if(d.idServidor == -1){
				/* uma vez que o idServidor nao admite valores invalidos (o que ja eh garantido pelo csv), um valor
				invalido nesse campo (ocorre quando fscanf retorna 0, ver funcao getDados) indica o final do arquivo csv.*/
				fwrite(pagina, offset, 1, arquivoBin);
				offset = 32000;
			}
			else if((d.tamanhoRegistro + 5) <= (32000 - offset)){
				//caso o registro ainda couber na pagina ele sera copiado para a mesma
				pagina = getPaginaDados(d, pagina, &offset, &offsetTamRegistro);
			}
			else{
				/*caso o registro nao couber na pagina, mas o csv ainda nao estiver no final, preenche-se o resto
				da pagina com '@', e retorna-se o ponteiro de posicao do arquivo para a posicao que estava antes
				de ler o registro atual, de forma que ele seja lido novamente e copiado para a proxima pagina.*/
				for(int i = offset; i < 32000; i++){
					pagina[i] = '@';
					tamRegAnterior++;
				}
				memcpy(&pagina[offsetTamRegistro], &tamRegAnterior, 4);
				offset = 32000;
				fwrite(pagina, 32000, 1, arquivoBin);
				fsetpos(arquivoLido, &posAnterior);				
			}
			free(d.nomeServidor);
			free(d.cargoServidor);
		}

		free(pagina);
	}

	free(paginaCab);

	c.status = '1';			//status '1' indica que o arquivo binario foi fechado corretamente
	rewind(arquivoBin);
	fwrite(&c.status, 1, 1, arquivoBin);
	
	//fechando os arquivos
	fclose(arquivoLido);
	fclose(arquivoBin);

	printf("%s", "arquivoTrab1.bin");

}

//funcionalidade 2
void leArquivoBin(char *nomeArquivo){
	FILE* arquivoBin;										//ponteiro para o arquivo .bin	
	char pagina[32000];										//array da pagina de disco
	int bytesLidos, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)

	arquivoBin = fopen(nomeArquivo, "rb");

	//caso de erro #1: nao existe um arquivo com o nome indicado pelo usuario
	if(arquivoBin == NULL){
		printf("Falha no processamento do arquivo.");
		return;
	}

	//leitura da pagina do registro de cabecalho
	bytesLidos = fread(pagina, 32000, 1, arquivoBin);
	paginasAcessadas++;
	
	//caso de erro #2: o arquivo esta inconsistente (status 0)
	if(pagina[0] == '0'){
		printf("Falha no processamento do arquivo.");
		fclose(arquivoBin);
		return;
	}

	while(bytesLidos != 0){
		int offset = 0;			//armazena o byte offset, que sera atualizado a cada copia de dados para a struct
		
		bytesLidos = fread(pagina, 32000, 1, arquivoBin);

		//caso de erro #3: nao ha registros
		if(bytesLidos == 0 && paginasAcessadas == 1){
			printf("Registro inexistente.");
			fclose(arquivoBin);
			return;
		}
		
		if(pagina[0] == '-'){
			while(offset < 32000){					//o ultimo byte da pagina esta no offset 32000 - 1 (ou seja, pagina[31999])
				Dados d;

				//copia dos bytes da pagina de disco para a struct do registro de dados
				memcpy(&d.removido, &pagina[offset], 1);
				offset += 1;
				memcpy(&d.tamanhoRegistro, &pagina[offset], 4);
				offset += 4;
				memcpy(&d.encadeamentoLista, &pagina[offset], 8);
				offset += 8;
				memcpy(&d.idServidor, &pagina[offset], 4);
				offset += 4;
				memcpy(&d.salarioServidor, &pagina[offset], 8);
				offset += 8;
				memcpy(&d.telefoneServidor, &pagina[offset], 14);
				offset += 14;

				if(pagina[offset + 4] == 'n'){		//verifica se o campo nomeServidor existe verificando a presenca da tag 'n'
					/*caso o campo existir, os dados sao copiados normalmente*/
					memcpy(&d.tamNomeServidor, &pagina[offset], 4);
					offset += 4;
					memcpy(&d.tagCampo4, &pagina[offset], 1);
					offset += 1;

					d.nomeServidor = (char *) malloc(d.tamNomeServidor);
					memcpy(d.nomeServidor, &pagina[offset], d.tamNomeServidor);
					offset += d.tamNomeServidor - 1;
				}
				else{
					/*caso o campo nao existir, o campo tamNomeServidor adquire valor nulo (-1), que sera utilizado
					pela funcao printDados para verificar se um valor deve ser impresso ou nao*/
					d.tamNomeServidor = -1;
					d.nomeServidor = "";
				}
				
				if(pagina[offset + 4] == 'c'){			//verifica se o campo cargoServidor existe verificando a presenca da tag 'c'
						/*caso o campo existir, os dados sao copiados normalmente*/
						memcpy(&d.tamCargoServidor, &pagina[offset], 4);
						offset += 4;
						memcpy(&d.tagCampo5, &pagina[offset], 1);
						offset += 1;

						d.cargoServidor = (char *) malloc(d.tamCargoServidor);
						memcpy(d.cargoServidor, &pagina[offset], d.tamCargoServidor);
						offset += d.tamCargoServidor - 1;
				}
				else{
					/*caso o campo nao existir, o campo tamCargoServidor adquire valor nulo (-1), que sera utilizado
					pela funcao printDados para verificar se um valor deve ser impresso ou nao*/
					d.tamCargoServidor = -1;
					d.cargoServidor = "";
				}

				printDados2(d);		//imprime o registro de dados

				if(pagina[offset] != '-'){
					/*o char '-' marca o inicio de um registro de dados (uma vez que nao ha registros removidos).
					assim, a nao existencia desse char logo apos o fim do registro anterior indica que nao existem
					mais registros, marcando assim o fim da pagina de disco.*/
					paginasAcessadas++;
					offset = 32000;
				}

				if(d.tamNomeServidor != -1)
					free(d.nomeServidor);

				if(d.tamCargoServidor != -1)
					free(d.cargoServidor);
			}
			
		}
	}

	//imprime o numero de paginas de disco que foram acessadas apos a leitura dos registros
	printf("Número de páginas de disco acessadas: %d", paginasAcessadas);

	fclose(arquivoBin);

}

//funcionalidade 3
void buscaArquivoBin(char *nomeArquivo){
	FILE* arquivoBin;			//ponteiro para o arquivo .bin	
	char pagina[32000];			//array da pagina de disco
	char nomeDoCampo[100];		//string do nome do campo que sera buscado (exemplo: cargoServidor)
	char *valor;				//string do valor que sera buscado (exemplo: ENGENHEIRO)
	int bytesLidos, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)
	int i, achou = 0;		//a variavel achou indica se foi encontrado algum registro equivalente (0 = registro nao encontrado, 1 = registro encontrado)
	Cabecalho c;

	//obtencao das entradas nomeDoCampo e valor
	scanf("%s ", nomeDoCampo);
	valor = readLine();

	arquivoBin = fopen(nomeArquivo, "rb");

	//caso de erro #1: nao existe um arquivo com o nome indicado pelo usuario
	if(arquivoBin == NULL){
		printf("Falha no processamento do arquivo.");
		free(valor);
		return;
	}

	//leitura da pagina do registro de cabecalho
	bytesLidos = fread(pagina, 32000, 1, arquivoBin);
	paginasAcessadas++;
	
	//caso de erro #2: o arquivo esta inconsistente (status 0)
	if(pagina[0] == '0'){
		printf("Falha no processamento do arquivo.");
		fclose(arquivoBin);
		free(valor);
		return;
	}

	//copia dos dados da pagina do cabecalho para a struct
	memcpy(&c.status, &pagina[0], 1);
	memcpy(&c.topoLista, &pagina[1], 8);
	memcpy(&c.tagCampo1, &pagina[9], 1);
	memcpy(&c.desCampo1, &pagina[10], 40);
	for(i = 0; i < sizeof(c.desCampo1); i++){
		if(c.desCampo1[i] == '@'){
			c.desCampo1[i] = '\0';
		}
	}
	memcpy(&c.tagCampo2, &pagina[50], 1);
	memcpy(&c.desCampo2, &pagina[51], 40);
	for(i = 0; i < sizeof(c.desCampo2); i++){
		if(c.desCampo2[i] == '@'){
			c.desCampo2[i] = '\0';
		}
	}
	memcpy(&c.tagCampo3, &pagina[91], 1);
	memcpy(&c.desCampo3, &pagina[92], 40);
	for(i = 0; i < sizeof(c.desCampo3); i++){
		if(c.desCampo3[i] == '@'){
			c.desCampo3[i] = '\0';
		}
	}
	memcpy(&c.tagCampo4, &pagina[132], 1);
	memcpy(&c.desCampo4, &pagina[133], 40);
	for(i = 0; i < sizeof(c.desCampo4); i++){
		if(c.desCampo4[i] == '@'){
			c.desCampo4[i] = '\0';
		}
	}
	memcpy(&c.tagCampo5, &pagina[173], 1);
	memcpy(&c.desCampo5, &pagina[174], 40);
	for(i = 0; i < sizeof(c.desCampo5); i++){
		if(c.desCampo5[i] == '@'){
			c.desCampo5[i] = '\0';
		}
	}
	//fim da copia do registro de cabecalho

	//busca pelos registros
	while(bytesLidos != 0){		//bytesLidos = 0 indica o final do arquivo
		int offset = 0;			//armazena o byte offset, que sera atualizado a cada copia de dados para a struct
		
		bytesLidos = fread(pagina, 32000, 1, arquivoBin);

		//caso de erro #3: nao ha registros
		if(bytesLidos == 0 && paginasAcessadas == 1){
			printf("Registro inexistente.");
			fclose(arquivoBin);
			free(valor);
			return;
		}
		
		if(pagina[0] == '-'){
			while(offset < 32000){			//o ultimo byte da pagina esta no offset 32000 - 1 (ou seja, pagina[31999])
				Dados d;

				//copia dos bytes da pagina de disco para a struct do registro de dados
				memcpy(&d.removido, &pagina[offset], 1);
				offset += 1;
				memcpy(&d.tamanhoRegistro, &pagina[offset], 4);
				offset += 4;
				memcpy(&d.encadeamentoLista, &pagina[offset], 8);
				offset += 8;
				memcpy(&d.idServidor, &pagina[offset], 4);
				offset += 4;
				memcpy(&d.salarioServidor, &pagina[offset], 8);
				offset += 8;
				memcpy(&d.telefoneServidor, &pagina[offset], 14);
				offset += 14;

				if(pagina[offset + 4] == 'n'){		//verifica se o campo nomeServidor existe verificando a presenca da tag 'n'
					/*caso o campo existir, os dados sao copiados normalmente*/
					memcpy(&d.tamNomeServidor, &pagina[offset], 4);
					offset += 4;
					memcpy(&d.tagCampo4, &pagina[offset], 1);
					offset += 1;

					d.nomeServidor = (char *) malloc(d.tamNomeServidor);
					memcpy(d.nomeServidor, &pagina[offset], d.tamNomeServidor);
					offset += d.tamNomeServidor - 1;
				}
				else{
					/*caso o campo nao existir, o campo tamNomeServidor adquire valor nulo (-1), que sera utilizado
					pela funcao printDados para verificar se um valor deve ser impresso ou nao*/
					d.tamNomeServidor = -1;
					d.nomeServidor = "";
				}
				
				if(pagina[offset + 4] == 'c'){			//verifica se o campo cargoServidor existe verificando a presenca da tag 'c'
						/*caso o campo existir, os dados sao copiados normalmente*/
						memcpy(&d.tamCargoServidor, &pagina[offset], 4);
						offset += 4;
						memcpy(&d.tagCampo5, &pagina[offset], 1);
						offset += 1;

						d.cargoServidor = (char *) malloc(d.tamCargoServidor);
						memcpy(d.cargoServidor, &pagina[offset], d.tamCargoServidor);
						offset += d.tamCargoServidor - 1;
				}
				else{
					/*caso o campo nao existir, o campo tamCargoServidor adquire valor nulo (-1), que sera utilizado
					pela funcao printDados para verificar se um valor deve ser impresso ou nao*/
					d.tamCargoServidor = -1;
					d.cargoServidor = "";
				}

				//imprimir o registro de dados compativeis com a pesquisa
				if(strcmp(nomeDoCampo, "idServidor") == 0){
					if(atoi(valor) == d.idServidor){
						achou = 1;
						printDados3(c, d);
						paginasAcessadas++;
						printf("Número de páginas de disco acessadas: %d", paginasAcessadas);
						return;							
					}
				}
				else if(strcmp(nomeDoCampo, "salarioServidor") == 0){
					if(atof(valor) == d.salarioServidor){
						achou = 1;
						printDados3(c, d);
					}
				}
				else if(strcmp(nomeDoCampo, "telefoneServidor") == 0){
					if(strcmp(valor, d.telefoneServidor) == 0){
						achou = 1;
						printDados3(c, d);
					}
				}
				else if(strcmp(nomeDoCampo, "nomeServidor") == 0){
					if(strcmp(valor, d.nomeServidor) == 0){
						achou = 1;
						printDados3(c, d);
					}
				}
				else if(strcmp(nomeDoCampo, "cargoServidor") == 0){
					if(strcmp(valor, d.cargoServidor) == 0){
						achou = 1;
						printDados3(c, d);
					}
				}
					

				if(pagina[offset] != '-'){
					/*o char '-' marca o inicio de um registro de dados (uma vez que nao ha registros removidos).
					assim, a nao existencia desse char logo apos o fim do registro anterior indica que nao existem
					mais registros, marcando assim o fim da pagina de disco.*/
					paginasAcessadas++;
					offset = 32000;
				}

				if(d.tamNomeServidor != -1)
					free(d.nomeServidor);

				if(d.tamCargoServidor != -1)
					free(d.cargoServidor);
				
			}
			
		}
	}
	

	if(achou == 1)
		printf("Número de páginas de disco acessadas: %d", paginasAcessadas);
	else
		printf("Registro inexistente.");

	free(valor);
	fclose(arquivoBin);
}

//funcionalidade 4
void removeRegistro(char *nomeArquivo){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
    int numeroRemocoes;
    //////////////////////////////////////////
    char pagina[32000];			//array da pagina de disco
    int bytesLidos, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)

    scanf("%d", &numeroRemocoes);

    char nomeCampoArray[numeroRemocoes][100], *valorCampoArray[numeroRemocoes];

    for(int i = 0; i < numeroRemocoes; i++){
        char nomeCampo[100];
        char *valorCampo;

        scanf("%s ", nomeCampo);
        valorCampo = readLine();
        strcpy(nomeCampoArray[i], nomeCampo);
        valorCampoArray[i] = (char *) malloc(sizeof(valorCampo));
        strcpy(valorCampoArray[i], valorCampo);


        //printf("%s\n", nomeCampoArray[i]);
        //printf("%s\n", valorCampoArray[i]);
    }

    arquivoBin = fopen(nomeArquivo, "rb");

    //caso de erro #1: nao existe um arquivo com o nome indicado pelo usuario
	if(arquivoBin == NULL){
		printf("Falha no processamento do arquivo.");
        //free(valorCampo);
		return;
	}

    //leitura da pagina do registro de cabecalho
	bytesLidos = fread(pagina, 32000, 1, arquivoBin);
	paginasAcessadas++;

    //caso de erro #2: o arquivo esta inconsistente (status 0)
    if(pagina[0] == '0'){
		printf("Falha no processamento do arquivo.");
		fclose(arquivoBin);
        //free(valorCampo);
		return;
	}

    fclose(arquivoBin);
}

//obtem os dados para a funcionalidade 5
Dados getDados2(){
	Dados d;
	int idServidor;
	char *salarioServidor, telefoneServidor[15], *nomeServidor, *cargoServidor;

	scanf("%d ", &idServidor);
	//printf("%d\n", idServidor);
	salarioServidor = readLine3();
	//printf("%s\n", salarioServidor);
	//scanf("%s", telefoneServidor);
	//telefoneServidor = readLine3();
	scanf(" ");
	fgets(telefoneServidor, 15, stdin);
	//printf("%s\n", telefoneServidor);
	scanf(" ");
	nomeServidor = readLine2();
	//printf("1\n");
	//printf("%s\n", nomeServidor);
	//printf("1\n");
	cargoServidor = readLine();
	//printf("%s\n", cargoServidor);

	d.idServidor = idServidor;
	//printf("1\n");
	if(strcmp(salarioServidor, "NULO"))
		d.salarioServidor = atof(salarioServidor);
	//printf("2\n");
	strcpy(d.telefoneServidor, telefoneServidor);
	//printf("3\n");
	d.nomeServidor = (char *) malloc(sizeof(nomeServidor));
	//printf("4\n");
	strcpy(d.nomeServidor, nomeServidor);
	//printf("5\n");
	d.cargoServidor = (char *) malloc(sizeof(cargoServidor));
	//printf("6\n");
	strcpy(d.cargoServidor, cargoServidor);
	//printf("7\n");

	//printf("%d\n", d.idServidor);
	//printf("%lf\n",d.salarioServidor);
	//printf("%s\n", d.telefoneServidor);
	//printf("%s\n", d.nomeServidor);
	//printf("%s\n", d.cargoServidor);
    
    return d;
}

//funcionalidade 5
void insereRegistro(char *nomeArquivo){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
    int numRegistros;
    //////////////////////////////////////////
    char pagina[32000];			//array da pagina de disco
    int bytesLidos, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)

    arquivoBin = fopen(nomeArquivo, "rb");

    //caso de erro #1: nao existe um arquivo com o nome indicado pelo usuario
	if(arquivoBin == NULL){
		printf("Falha no processamento do arquivo.");
        //free(valorCampo);
		return;
	}

    //leitura da pagina do registro de cabecalho
	bytesLidos = fread(pagina, 32000, 1, arquivoBin);
	paginasAcessadas++;

    //caso de erro #2: o arquivo esta inconsistente (status 0)
    if(pagina[0] == '0'){
		printf("Falha no processamento do arquivo.");
		fclose(arquivoBin);
        //free(valorCampo);
		return;
	}

	scanf("%d", &numRegistros);

    Dados d[numRegistros];

    for(int i = 0; i < numRegistros; i++){
        d[i] = getDados2();
    }

	fseek(arquivoBin, 0, SEEK_END);

    fclose(arquivoBin);
}

//funcionalidade 6
void atualizaRegistro(char *nomeArquivo){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
    int numeroRemocoes;
    //////////////////////////////////////////
    char pagina[32000];			//array da pagina de disco
    int bytesLidos, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)

    arquivoBin = fopen(nomeArquivo, "rb");

    //caso de erro #1: nao existe um arquivo com o nome indicado pelo usuario
	if(arquivoBin == NULL){
		printf("Falha no processamento do arquivo.");
        //free(valorCampo);
		return;
	}

    //leitura da pagina do registro de cabecalho
	bytesLidos = fread(pagina, 32000, 1, arquivoBin);
	paginasAcessadas++;

    //caso de erro #2: o arquivo esta inconsistente (status 0)
    if(pagina[0] == '0'){
		printf("Falha no processamento do arquivo.");
		fclose(arquivoBin);
        //free(valorCampo);
		return;
	}

	scanf("%d", &numeroRemocoes);

    char *nomeCampoBuscaArray[numeroRemocoes], *valorCampoBuscaArray[numeroRemocoes];
	char *nomeCampoAtualizaArray[numeroRemocoes], *valorCampoAtualizaArray[numeroRemocoes];

    for(int i = 0; i < numeroRemocoes; i++){
        char *nomeCampoBusca, *valorCampoBusca, *nomeCampoAtualiza, *valorCampoAtualiza;

        scanf("%s ", nomeCampoBusca);
        valorCampoBusca = readLine2();
        scanf("%s ", nomeCampoAtualiza);
        valorCampoAtualiza = readLine();

        nomeCampoBuscaArray[i] = (char *) malloc(sizeof(nomeCampoBusca));
        strcpy(nomeCampoBuscaArray[i], nomeCampoBusca);
        valorCampoBuscaArray[i] = (char *) malloc(sizeof(valorCampoBusca));
        strcpy(valorCampoBuscaArray[i], valorCampoBusca);
        nomeCampoAtualizaArray[i] = (char *) malloc(sizeof(nomeCampoAtualiza));
        strcpy(nomeCampoAtualizaArray[i], nomeCampoAtualiza);
        valorCampoAtualizaArray[i] = (char *) malloc(sizeof(valorCampoAtualiza));
        strcpy(valorCampoAtualizaArray[i], valorCampoAtualiza);

        printf("%s\n", nomeCampoBuscaArray[i]);
        printf("%s\n", valorCampoBuscaArray[i]);
        printf("%s\n", nomeCampoAtualizaArray[i]);
        printf("%s\n", valorCampoAtualizaArray[i]);

    }

    fclose(arquivoBin);
}

//main
int main(){
	int funcionalidade;
	char nomeArquivo[100];
	FILE *arquivoLido, *arquivoBin;

	scanf("%d", &funcionalidade);

	switch (funcionalidade){
		case 1:
			scanf("%s", nomeArquivo);
			criaArquivoBin(nomeArquivo);
			break;
		case 2:
			scanf("%s", nomeArquivo);
			leArquivoBin(nomeArquivo);
			break;
		case 3:
			scanf("%s", nomeArquivo);
			buscaArquivoBin(nomeArquivo);
			break;
        case 4:
			scanf("%s", nomeArquivo);
			removeRegistro(nomeArquivo);
			break;

        case 5:
			scanf("%s", nomeArquivo);
			insereRegistro(nomeArquivo);
			break;

        case 6:
			scanf("%s", nomeArquivo);
			atualizaRegistro(nomeArquivo);
			break;
	
		default:
			break;
	}

	return 0;
}