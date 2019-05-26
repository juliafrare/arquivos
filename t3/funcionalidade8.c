#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade8.h"
#include "escreverTela2.h"
#include "lista.h"

void mergeArquivos(char *nomeArquivo, char *nomeArquivo2){
    FILE* arquivoBin1;
	FILE* arquivoBin2;			//ponteiro para o arquivo .bin
	FILE* arquivoSaida;
	char nomeArquivoSaida[100];
    int numeroAtualiza;
    char paginaCab[32000], status;			//array da pagina de cabecalho
    int bytesLidos, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)
	long int arquivoOffset;
	Lista l;

	//abertura do arquivo original
    arquivoBin1 = fopen(nomeArquivo, "rb");
	arquivoBin2 = fopen(nomeArquivo2, "rb");

    //caso de erro #1: nao existe um arquivo com o nome indicado pelo usuario
	if(arquivoBin1 == NULL || arquivoBin2 == NULL){
		printf("Falha no processamento do arquivo.");
		if(arquivoBin1 != NULL)
			fclose(arquivoBin1);
		else if(arquivoBin2 != NULL)
			fclose(arquivoBin2);
		return;
	}

    //leitura da pagina do registro de cabecalho
	bytesLidos = fread(paginaCab, 32000, 1, arquivoBin1);
	paginasAcessadas++;
	fread(&status, 1, 1, arquivoBin2);

    //caso de erro #2: o arquivo esta inconsistente (status 0)
    if(paginaCab[0] == '0' || status == '0'){
		printf("Falha no processamento do arquivo.");
		fclose(arquivoBin1);
		fclose(arquivoBin2);
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
    fclose(arquivoBin1);
	fclose(arquivoBin2);
	fclose(arquivoSaida);

	//free
}