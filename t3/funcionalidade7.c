#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade7.h"
#include "escreverTela2.h"
#include "lista.h"

void ordenaArquivo(char *nomeArquivo){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
	FILE* arquivoSaida;
	char nomeArquivoSaida[100];
    int numeroAtualiza;
    char paginaCab[32000];			//array da pagina de cabecalho
    int bytesLidos, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)
	long int arquivoOffset;
	Lista l;

	//abertura do arquivo original
    arquivoBin = fopen(nomeArquivo, "rb");

    //caso de erro #1: nao existe um arquivo com o nome indicado pelo usuario
	if(arquivoBin == NULL){
		printf("Falha no processamento do arquivo.");
		return;
	}

    //leitura da pagina do registro de cabecalho
	bytesLidos = fread(paginaCab, 32000, 1, arquivoBin);
	paginasAcessadas++;

    //caso de erro #2: o arquivo esta inconsistente (status 0)
    if(paginaCab[0] == '0'){
		printf("Falha no processamento do arquivo.");
		fclose(arquivoBin);
		return;
	}

	//nome do arquivo de saida
	scanf("%s", nomeArquivoSaida);

	//abertura do arquivo de saida
    arquivoSaida = fopen(nomeArquivoSaida, "wb");

    //copia do registro de cabecalho


	//adicionar registros p/array


	//mergesort na array


	//copiar registros p/arquivo (ver funcionalidade 1)


	//pegar encadeamento


    //fechando os arquivos
	fclose(arquivoBin);
    fclose(arquivoSaida);

	//free
}