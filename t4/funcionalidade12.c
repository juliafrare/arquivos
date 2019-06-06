#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcoes_t4.h"
#include "funcionalidade12.h"
#include "escreverTela2.h"

void funcionalidade12(char *nomeArquivo, char *nomeArquivo2){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
	FILE* arquivoIndice;
    int n;
    char *paginaCab1, *paginaCab2;	//array da pagina de cabecalho

	//abertura do arquivo de entrada
    paginaCab1 = (char *) malloc(sizeof(char) * 32000);
	int init1, init2;
	init1 = inicializa(&arquivoBin, nomeArquivo, &paginaCab1);
	if(init1 == 1){
        free(paginaCab1);
		return;
    }

    //abertura do arquivo de indice
    paginaCab2 = (char *) malloc(sizeof(char) * 32000);
	init2 = inicializa(&arquivoIndice, nomeArquivo2, &paginaCab2);
	if(init2 == 1){
		fclose(arquivoBin);
		free(paginaCab1);
        free(paginaCab2);
		return;
	}

    scanf("%d", &n);

    char nomeCampo[n][25], valorCampo[n][150];
    for(int i = 0; i < n; i++){
        scanf("%s ", nomeCampo[i]);
        scan_quote_string(valorCampo[i]);

        //printf("%s\n", nomeCampo[i]);
        //printf("%s\n", valorCampo[i]);
    }

    //funcionalidade 4
    //obter indice
    //sort
    //reescrever arquivo indice

    fclose(arquivoBin);
    fclose(arquivoIndice);

    free(paginaCab1);
    free(paginaCab2);

    //escrita do arquivo bin na tela
	//binarioNaTela2(nomeArquivo2);
}