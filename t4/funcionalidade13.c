#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade5.h"
#include "funcionalidade13.h"
#include "escreverTela2.h"

void funcionalidade13(char *nomeArquivo, char *nomeArquivo2){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
	FILE* arquivoIndice;
    int n;
    char *paginaCab1, *paginaCab2;	//array da pagina de cabecalho
    int numRegistros;

    scanf("%d", &numRegistros);

    //execução da funcionalidade 5
    DadosT4 *novos;  //array dos indices novos
    novos = insereRegistro(nomeArquivo, &numRegistros);

    //abertura do arquivo de indice p/leitura
    paginaCab2 = (char *) malloc(sizeof(char) * 32000);
    int init2;
	init2 = inicializa(&arquivoIndice, nomeArquivo2, &paginaCab2);
	if(init2 == 1){
        free(paginaCab2);
		return;
	}

    DadosT4 *regIndice;
    int nIndice = 0;
	regIndice = (DadosT4 *) malloc(sizeof(DadosT4) * 10000);

    //obtenção do vetor de índice
    while(!feof(arquivoIndice)){
		int offset = 0;	//armazena o byte offset da pagina
		char *pagina;

        pagina = (char *) calloc(32000, sizeof(char));

		//obtencao da pagina de dados
		fread(pagina, 32000, 1, arquivoIndice);

        while(offset < 32000){

            if(pagina[offset] == '\0'){
                offset = 32000;
                break;
            }

            regIndice[nIndice] = getIndice(pagina, &offset);
            nIndice++;
        }

        free(pagina);
	}

    free(paginaCab2);
    fclose(arquivoIndice);

    //adicionar registros no índice
    for(int i = 0; i < numRegistros; i++){
        for(int j = strlen(novos[i].chaveBusca) + 1; j < 120; j++)
            novos[i].chaveBusca[j] = '@';
        regIndice[nIndice + i] = copiaRegIndice(novos[i]);
    }
    nIndice += numRegistros;

    //sort
    sortIndice(regIndice, 0, nIndice - 1);

    arquivoIndice = fopen(nomeArquivo2, "wb");

    //atualização do status
    CabecalhoT4 c;
	inicializaCabIndice(&c);
	paginaCab2 = getPaginaCabIndice(c);
    fwrite(paginaCab2, 32000, 1, arquivoIndice);
    fseek(arquivoIndice, 32000, SEEK_SET);

    //atualizar arquivo de índice
    int j = 0;
	while(j < nIndice){
		char *paginaIndice;
		int offset = 0;

		paginaIndice = (char *) malloc(sizeof(char) * 32000);

		while(offset < 32000){	
			if(j == nIndice - 1){
				paginaIndice = getPaginaIndice(regIndice[j], &offset, paginaIndice);
				fwrite(paginaIndice, offset, 1, arquivoIndice);
				offset = 32000;
				j++;
			}
			else{
				paginaIndice = getPaginaIndice(regIndice[j], &offset, paginaIndice);
				
				if(offset == 32000)
					fwrite(paginaIndice, 32000, 1, arquivoIndice);

				j++;
			}
		}
		//if(j != nIndice - 1)
		//fwrite(paginaIndice, 32000, 1, arquivoIndice);

		free(paginaIndice);
	}

    //atualização do status
	char status = '1';
	rewind(arquivoIndice);
	fwrite(&status, 1, 1, arquivoIndice);	//status 1
    fwrite(&nIndice, 4, 1, arquivoIndice); //atualização do nroRegistros

    fclose(arquivoIndice);

    free(regIndice);
    free(paginaCab2);
    free(novos);

    //escrita do arquivo bin na tela
	binarioNaTela2(nomeArquivo2);
}