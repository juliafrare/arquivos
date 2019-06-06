#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcoes_t4.h"
#include "funcionalidade4.h"
#include "funcionalidade10.h"
#include "funcionalidade12.h"
#include "escreverTela2.h"

void funcionalidade12(char *nomeArquivo, char *nomeArquivo2){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
	FILE* arquivoIndice;
    char *paginaCab1, *paginaCab2;	//array da pagina de cabecalho
    int init;

	//execução da funcionalidade 4
    DadosT4 *novos;  //array dos indices novos
    init = removeRegistro(nomeArquivo);
    if(init == 1){
        return;
	}

    //abertura do arquivo de indice p/leitura
    paginaCab2 = (char *) malloc(sizeof(char) * 32000);
	init = inicializa(&arquivoIndice, nomeArquivo2, &paginaCab2);
	if(init == 1){
        free(paginaCab2);
		return;
	}
    free(paginaCab2);
    fclose(arquivoIndice);

    //execução da funcionalidade 10
    init = funcionalidade10("arquivo-novo.bin", nomeArquivo2);
    if(init == 1){
        return;
	}

    //escrita do arquivo bin na tela
	binarioNaTela2(nomeArquivo2);
}