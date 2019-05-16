#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade5.h"

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
	FILE* novoArquivoBin;
    int numRegistros;
    //////////////////////////////////////////
    char paginaCab[32000];			//array da pagina de disco
    int bytesLidos, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)
	long int arquivoOffset;

    arquivoBin = fopen(nomeArquivo, "rb");

    //caso de erro #1: nao existe um arquivo com o nome indicado pelo usuario
	if(arquivoBin == NULL){
		printf("Falha no processamento do arquivo.");
        //free(valorCampo);
		return;
	}

    //leitura da pagina do registro de cabecalho
	bytesLidos = fread(paginaCab, 32000, 1, arquivoBin);
	paginasAcessadas++;

    //caso de erro #2: o arquivo esta inconsistente (status 0)
    if(paginaCab[0] == '0'){
		printf("Falha no processamento do arquivo.");
		fclose(arquivoBin);
        //free(valorCampo);
		return;
	}

	novoArquivoBin = fopen("arquivo-novo.bin", "wb");

	fwrite(paginaCab, 32000, 1, novoArquivoBin);
	/*arquivoOffset = ftell(novoArquivoBin);
	rewind(novoArquivoBin);
	fwrite('0', 1, 1, novoArquivoBin);	//status 0
	fseek(novoArquivoBin, arquivoOffset, SEEK_SET);*/

	scanf("%d", &numRegistros);

    Dados d[numRegistros];

    for(int i = 0; i < numRegistros; i++){
        d[i] = getDados2();
    }

	//fseek(arquivoBin, 0, SEEK_END);

	//rewind(novoArquivoBin);
	//fwrite('1', 1, 1, novoArquivoBin);	//status 1

	fclose(novoArquivoBin);
    fclose(arquivoBin);
}