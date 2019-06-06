#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcoes_t4.h"
#include "funcionalidade10.h"
#include "escreverTela2.h"
/*
void getOffset(FILE *arquivoBin){

}*/

void funcionalidade10(char *nomeArquivo, char *nomeArquivo2){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
	FILE* arquivoIndice;
    char *paginaCab1;			//array da pagina de cabecalho
	int paginasAcessadas = 0;

	paginaCab1 = (char *) malloc(sizeof(char) * 32000);

	//abertura do arquivo de entrada
	int init;
	init = inicializa(&arquivoBin, nomeArquivo, &paginaCab1);
	if(init == 1){
		free(paginaCab1);
		return;
	}

	free(paginaCab1);

	//abertura do arquivo de indice
	arquivoIndice = fopen(nomeArquivo2, "wb");

	//obtencao da pagina de cabecalho
	char *paginaCabIndice;
	CabecalhoT4 c;

	inicializaCabIndice(&c);
	paginaCabIndice = getPaginaCabIndice(c);
	fwrite(paginaCabIndice, 32000, 1, arquivoIndice);

	paginasAcessadas++;
	free(paginaCabIndice);

	//obtencao das demais paginas

	//busca pelos registros
	DadosT4 *regIndice;
	int nIndice = 0;

	regIndice = (DadosT4 *) malloc(sizeof(DadosT4) * 10000);

	while(!feof(arquivoBin)){
		int offset = 0;	//armazena o byte offset
		char pagina[32000];

		//obtencao da pagina de dados
		fread(pagina, 32000, 1, arquivoBin);
		
		if(pagina[0] == '-' || pagina[0] == '*'){

			while(offset < 32000){
				//se o registro ja estiver removido, proceder para o proximo registro
				if(pagina[offset] == '*')
					pulaRemovido(&offset, pagina);

				//se ele nao estiver removido, verificar se ele possui os dados
				else if(pagina[offset] == '-'){
					Dados d;
					long int regOffset;

					regOffset = (paginasAcessadas * 32000) + offset;

					d = inicializaRegDados();
					d = copiaRegistro(pagina, &offset);
					
					if(d.tamNomeServidor > 0){
						//armazenamento do offset no vetor
						regIndice[nIndice].byteOffset = regOffset;
						//armazenamento da chaveBusca
						strcpy(regIndice[nIndice].chaveBusca, d.nomeServidor);
						for(int i = strlen(d.nomeServidor) + 1; i < 120; i++)
							regIndice[nIndice].chaveBusca[i] = '@';
						
						nIndice++;
					}

					if(d.tamNomeServidor > 0)
						free(d.nomeServidor);
					if(d.tamCargoServidor > 0)
						free(d.cargoServidor);

				}

				if(pagina[offset] != '-' && pagina[offset] != '*'){
					//os chars '-' e '*' marcam o inicio de um registro de dados.
					//assim, a nao existencia desses chars logo apos o fim do registro anterior indica que nao existem
					//mais registros, marcando assim o fim da pagina de disco.
					paginasAcessadas++;
					offset = 32000;
				}
			}
		}
	}

	//sort
	sortIndice(regIndice, 0, nIndice - 1);

	//copia para arquivo de indice
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

	//atualização do nroRegistros
	fseek(arquivoIndice, 1, SEEK_SET);
	fwrite(&nIndice, 4, 1, arquivoIndice);

	//atualização do status
	char status = '1';
	rewind(arquivoIndice);
	fwrite(&status, 1, 1, arquivoIndice);	//status 1

	
	fclose(arquivoBin);
	fclose(arquivoIndice);

	//free
	free(regIndice);

    //escrita do arquivo bin na tela
	binarioNaTela2(nomeArquivo2);
}