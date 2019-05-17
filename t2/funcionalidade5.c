#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade5.h"
#include "escreverTela2.h"
#include "lista.h"

//obtem os dados para a funcionalidade 5
Dados getDados2(){
	Dados d;
	char valorIdServidor[20], valorSalarioServidor[20], valorTelefoneServidor[14], valorNomeServidor[500], valorCargoServidor[500];

	scan_quote_string(valorIdServidor); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	scan_quote_string(valorSalarioServidor);
	scan_quote_string(valorTelefoneServidor);
	scan_quote_string(valorNomeServidor);
	scan_quote_string(valorCargoServidor);

	/*printf("%s\n", valorIdServidor);
	printf("%s\n", valorSalarioServidor);
	printf("%s\n", valorTelefoneServidor);
	printf("%s\n", valorNomeServidor);
	printf("%s\n", valorCargoServidor);*/

	d.idServidor = atoi(valorIdServidor);
	//printf("1\n");
	if(strlen(valorSalarioServidor) == 0)
		d.salarioServidor = -1;
	else
		d.salarioServidor = atof(valorSalarioServidor);
	//printf("2\n");
	strcpy(d.telefoneServidor, valorTelefoneServidor);
	//printf("3\n");
	d.nomeServidor = (char *) malloc(sizeof(valorNomeServidor));
	//printf("4\n");
	strcpy(d.nomeServidor, valorNomeServidor);
	//printf("5\n");
	d.cargoServidor = (char *) malloc(sizeof(valorCargoServidor));
	//printf("6\n");
	strcpy(d.cargoServidor, valorCargoServidor);
	//printf("7\n");

	/*printf("%d\n", d.idServidor);
	printf("%lf\n",d.salarioServidor);
	printf("%s\n", d.telefoneServidor);
	printf("%s\n", d.nomeServidor);
	printf("%s\n", d.cargoServidor);
	printf("\n");*/
    
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
	Lista l;

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

	arquivoOffset = ftell(arquivoBin);

	//obtenção da lista ordenada
	inicializaLista(&l);
	getLista(arquivoBin, &l);

	fseek(arquivoBin, arquivoOffset, SEEK_CUR);

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