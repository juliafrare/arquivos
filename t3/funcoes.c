#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

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
	r.encadeamentoLista = -1;		//encadeamentoLista sera sempre inicializado com -1
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
	if(d.tamNomeServidor > 0){		//se tamNomeServidor <= 0, nada sera copiado (ver funcao getDados)
		memcpy(&pagina[*offset], &d.tamNomeServidor, 4);
		*offset += 4;
		memcpy(&pagina[*offset], &d.tagCampo4, 1);
		*offset += 1;
		memcpy(&pagina[*offset], d.nomeServidor, strlen(d.nomeServidor) + 1);
		*offset += strlen(d.nomeServidor) + 1;
	}

	//copia do d.tamCargoServidor, d.tagCampo5 e d.cargoServidor
	if(d.tamCargoServidor > 0){	//se tamCargoServidor <= 0, nada sera copiado (ver funcao getDados)
		memcpy(&pagina[*offset], &d.tamCargoServidor, 4);
		*offset += 4;
		memcpy(&pagina[*offset], &d.tagCampo5, 1);
		*offset += 1;
		memcpy(&pagina[*offset], d.cargoServidor, strlen(d.cargoServidor) + 1);
		*offset += strlen(d.cargoServidor) + 1;
	}

	return pagina;
}

//funcao que copia os registros do binario para a struct
Dados copiaRegistro(char *pagina, int *offset){
    Dados d;

    //copia dos bytes da pagina de disco para a struct do registro de dados
    memcpy(&d.removido, &pagina[*offset], 1);
    *offset += 1;
    memcpy(&d.tamanhoRegistro, &pagina[*offset], 4);
    *offset += 4;
    memcpy(&d.encadeamentoLista, &pagina[*offset], 8);
    *offset += 8;
    memcpy(&d.idServidor, &pagina[*offset], 4);
    *offset += 4;
    memcpy(&d.salarioServidor, &pagina[*offset], 8);
    *offset += 8;
    memcpy(&d.telefoneServidor, &pagina[*offset], 14);
    *offset += 14;

    if(pagina[*offset + 4] == 'n'){		//verifica se o campo nomeServidor existe verificando a presenca da tag 'n'
        /*caso o campo existir, os dados sao copiados normalmente*/
        memcpy(&d.tamNomeServidor, &pagina[*offset], 4);
        *offset += 4;
        memcpy(&d.tagCampo4, &pagina[*offset], 1);
        *offset += 1;

        d.nomeServidor = (char *) malloc(d.tamNomeServidor);
        memcpy(d.nomeServidor, &pagina[*offset], d.tamNomeServidor);
        *offset += d.tamNomeServidor - 1;
    }
    else{
        /*caso o campo nao existir, o campo tamNomeServidor adquire valor nulo (-1), que sera utilizado
        pela funcao printDados para verificar se um valor deve ser impresso ou nao*/
        d.tamNomeServidor = -1;
        d.nomeServidor = "";
    }
    
    if(pagina[*offset + 4] == 'c'){			//verifica se o campo cargoServidor existe verificando a presenca da tag 'c'
            /*caso o campo existir, os dados sao copiados normalmente*/
            memcpy(&d.tamCargoServidor, &pagina[*offset], 4);
            *offset += 4;
            memcpy(&d.tagCampo5, &pagina[*offset], 1);
            *offset += 1;

            d.cargoServidor = (char *) malloc(d.tamCargoServidor);
            memcpy(d.cargoServidor, &pagina[*offset], d.tamCargoServidor);
            *offset += d.tamCargoServidor - 1;
    }
    else{
        /*caso o campo nao existir, o campo tamCargoServidor adquire valor nulo (-1), que sera utilizado
        pela funcao printDados para verificar se um valor deve ser impresso ou nao*/
        d.tamCargoServidor = -1;
        d.cargoServidor = "";
    }

    return d;
}

//funcao que copia um registro p/outro
Dados copiaRegistro2(Dados d1){
    Dados d2;

    //copia dos bytes da pagina de disco para a struct do registro de dados
    d2.removido = d1.removido;
    d2.tamanhoRegistro = d1.tamanhoRegistro;
    d2.encadeamentoLista = d1.encadeamentoLista;
    d2.idServidor = d1.idServidor;
    d2.salarioServidor = d1.salarioServidor;
	for(int i = 0; i < 14; i++)
    	d2.telefoneServidor[i] = d1.telefoneServidor[i];
	d2.tamNomeServidor = d1.tamNomeServidor;
	if(d1.tamNomeServidor > 0){
		d2.tagCampo4 = d1.tagCampo4;
		d2.nomeServidor = (char *) malloc(strlen(d1.nomeServidor) + 1);
		strcpy(d2.nomeServidor, d1.nomeServidor);
	}
	d2.tamCargoServidor = d1.tamCargoServidor;
	if(d1.tamCargoServidor > 0){
		d2.tagCampo5 = d1.tagCampo5;
		d2.cargoServidor = (char *) malloc(strlen(d1.cargoServidor) + 1);
		strcpy(d2.cargoServidor, d1.cargoServidor);
	}

    return d2;
}

//funcao que move um registro p/outro
Dados moveRegistro(Dados d1){
    Dados d2;

    //copia dos bytes da pagina de disco para a struct do registro de dados
    d2.removido = d1.removido;
    d2.tamanhoRegistro = d1.tamanhoRegistro;
    d2.encadeamentoLista = d1.encadeamentoLista;
    d2.idServidor = d1.idServidor;
    d2.salarioServidor = d1.salarioServidor;
	for(int i = 0; i < 14; i++)
    	d2.telefoneServidor[i] = d1.telefoneServidor[i];
	d2.tamNomeServidor = d1.tamNomeServidor;
	if(d1.tamNomeServidor > 0){
		d2.tagCampo4 = d1.tagCampo4;
		d2.nomeServidor = (char *) malloc(strlen(d1.nomeServidor) + 1);
		strcpy(d2.nomeServidor, d1.nomeServidor);
		free(d1.nomeServidor);
	}
	d2.tamCargoServidor = d1.tamCargoServidor;
	if(d1.tamCargoServidor > 0){
		d2.tagCampo5 = d1.tagCampo5;
		d2.cargoServidor = (char *) malloc(strlen(d1.cargoServidor) + 1);
		strcpy(d2.cargoServidor, d1.cargoServidor);
		free(d1.cargoServidor);
	}

    return d2;
}

void atualizaTamanho(Dados *d){
	(*d).tamanhoRegistro = sizeof((*d).encadeamentoLista) + sizeof((*d).idServidor) + sizeof((*d).salarioServidor) + sizeof((*d).telefoneServidor);
	if((*d).tamNomeServidor > 0){
		(*d).tamanhoRegistro += sizeof((*d).tamNomeServidor);
		(*d).tamanhoRegistro += sizeof((*d).tagCampo4);
		(*d).tamanhoRegistro += strlen((*d).nomeServidor) + 1;
	}
	if((*d).tamCargoServidor > 0){
		(*d).tamanhoRegistro += sizeof((*d).tamCargoServidor);
		(*d).tamanhoRegistro += sizeof((*d).tagCampo5);
		(*d).tamanhoRegistro += strlen((*d).cargoServidor) + 1;
	}
}

//copia as paginas p/ arquivo novo
void copiaArquivo(FILE *arquivoBin, FILE *novoArquivoBin, int paginasAcessadas){
	int bytesLidos;

	while(!feof(arquivoBin)){
		int offset = 0;	//armazena o byte offset
		char pagina[32000];

		//obtencao da pagina de dados
		bytesLidos = fread(pagina, 32000, 1, arquivoBin);

		//caso de erro #3: nao ha registros
		if(bytesLidos == 0 && paginasAcessadas == 1){
			printf("Registro inexistente.");
			fclose(arquivoBin);
			fclose(novoArquivoBin);
			return;
		}

		if(pagina[0] == '-' || pagina[0] == '*'){
			while(offset < 32000){
				int tamanho;
				offset += 1;
				memcpy(&tamanho, &pagina[offset], 4);
				offset += tamanho + 4;	//atualiza o byte offset para o offset do proximo registro
				if(pagina[offset] != '-' && pagina[offset] != '*'){
					//os chars '-' e '*' marcam o inicio de um registro de dados.
					//assim, a nao existencia desses chars logo apos o fim do registro anterior indica que nao existem
					//mais registros, marcando assim o fim da pagina de disco.
					if(!feof(arquivoBin)){
						paginasAcessadas++;
						offset = 32000;
						fwrite(pagina, 32000, 1, novoArquivoBin);
					}
					else{
						paginasAcessadas++;
						fwrite(pagina, offset, 1, novoArquivoBin);
						offset = 32000;
					}
					
				}
			}
		}
	}
}

//função que inicializa um arquivo e verifica se ele foi processado corretamente
int inicializa(FILE **arquivoBin, char *nomeArquivo, char **paginaCab){
	*arquivoBin = fopen(nomeArquivo, "rb");

	//caso de erro #1: nao existe um arquivo com o nome indicado pelo usuario
	if(*arquivoBin == NULL){
		printf("Falha no processamento do arquivo.");
		return 1;
	}

	//leitura da pagina do registro de cabecalho
	fread(*paginaCab, 32000, 1, *arquivoBin);

    //caso de erro #2: o arquivo esta inconsistente (status 0)
    if((*paginaCab)[0] == '0'){
		printf("Falha no processamento do arquivo.");
		fclose(*arquivoBin);
		return 1;
	}

	return 0;
}

//funções do mergesort
void merge(Dados *d, int ini, int meio, int fim) { 
    int i, j, k;

    i = ini;
    j = meio + 1;
    k = 0;

    Dados aux[fim];
  
    while(i <= meio && j <= fim){
        if(d[i].idServidor <= d[j].idServidor){
            aux[k] = moveRegistro(d[i]);
            i++;
        }
        else{
            aux[k] = moveRegistro(d[j]);
            j++;
        }
        k++;
    }

	while (i <= meio) {
		aux[k] = moveRegistro(d[i]);
		i++;
		k++;
	}	

	while (j <= fim) {
		aux[k] = moveRegistro(d[j]);
		j++;
		k++;
	}

	for (i = ini, k = 0; i <= fim; i++, k++) {
		d[i] = aux[k];
	}
} 

void mergesort(Dados d[], int ini, int fim){ 
    if(ini < fim){ 
        int meio = (ini + fim) / 2; 

        mergesort(d, ini, meio); 
        mergesort(d, meio + 1, fim); 
        merge(d, ini, meio, fim); 
    } 
}

//função de busca binaria
int buscaBin(Dados *d, int id, int ini, int fim){

	if(ini > fim)
		return -1;

	int meio = (ini + fim) / 2;

	if(id == d[meio].idServidor)
		return meio;

	if(id < d[meio].idServidor)
		return buscaBin(d, id, ini, meio - 1);

	if(id > d[meio].idServidor)
		return buscaBin(d, id, meio + 1, fim);
	
}

//função que pula os registros removidos
void pulaRemovido(int *offset, char *pagina){
	int tamanho;
	*offset += 1;
	memcpy(&tamanho, &pagina[*offset], 4);
	*offset += tamanho + 4;	//atualiza o byte offset para o offset do proximo registro
}