#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade7.h"
#include "escreverTela2.h"

void ordenaArquivo(char *nomeArquivo){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
	FILE* arquivoSaida;
	char nomeArquivoSaida[100];
    char *paginaCab;			//array da pagina de cabecalho

	paginaCab = (char *) malloc(sizeof(char) * 32000);

	//abertura do arquivo original
	int init;
	init = inicializa(&arquivoBin, nomeArquivo, &paginaCab);
	if(init == 1){
		free(paginaCab);
		return;
	}

	//nome do arquivo de saida
	scan_quote_string(nomeArquivoSaida);

	//abertura do arquivo de saida
    arquivoSaida = fopen(nomeArquivoSaida, "wb");

    //copia do registro de cabecalho
	paginaCab[0] = '0'; //status 0
	fwrite(paginaCab, 32000, 1, arquivoSaida);
	free(paginaCab);

	//adicionar registros p/array
	Dados *registros;
	registros = (Dados *) malloc(sizeof(Dados) * 100000);
	int nReg = 0;

	//busca pelos registros
	while(!feof(arquivoBin)){
		int offset = 0;	//armazena o byte offset
		char pagina[32000];

		//obtencao da pagina de dados
		fread(pagina, 32000, 1, arquivoBin);
		
		if(pagina[0] == '-' || pagina[0] == '*'){

			while(offset < 32000){			//o ultimo byte da pagina esta no offset 32000 - 1 (ou seja, pagina[31999])

				//se o registro ja estiver removido, proceder para o proximo registro
				if(pagina[offset] == '*')
					pulaRemovido(&offset, pagina);

				//se ele nao estiver removido, verificar se ele possui os dados
				else if(pagina[offset] == '-'){
					
					registros[nReg] = inicializaRegDados();
					
					//copia dos bytes da pagina de disco para a struct do registro de dados
					registros[nReg] = copiaRegistro(pagina, &offset);
					atualizaTamanho(&registros[nReg]);

					nReg++;
				}

				if(pagina[offset] != '-' && pagina[offset] != '*'){
					//os chars '-' e '*' marcam o inicio de um registro de dados.
					//assim, a nao existencia desses chars logo apos o fim do registro anterior indica que nao existem
					//mais registros, marcando assim o fim da pagina de disco.
					offset = 32000;
				}
			}
		}
	}

	//mergesort na array
	mergesort(registros, 0, nReg - 1);

	//copiar registros p/arquivo
	int j = 0;

	while(j < nReg){
		char *pagina;
		int offset = 0;
		int offsetTamRegistro = 0, tamRegistro = 0, tamRegAnterior = 0;

		pagina = (char *) malloc(sizeof(char) * 32000);

		while(offset < 32000){
			tamRegAnterior = tamRegistro;
			tamRegistro = registros[j].tamanhoRegistro;
			if(j == nReg - 1){
				//indica o final do arquivo
				pagina = getPaginaDados(registros[j], pagina, &offset, &offsetTamRegistro);
				fwrite(pagina, offset, 1, arquivoSaida);
				offset = 32000;
				j++;
			}
			else if((registros[j].tamanhoRegistro + 5) <= (32000 - offset)){
				//caso o registro ainda couber na pagina ele sera copiado para a mesma
				pagina = getPaginaDados(registros[j], pagina, &offset, &offsetTamRegistro);

				if(offset == 32000)
					fwrite(pagina, 32000, 1, arquivoSaida);

				j++;
			}
			else{
				//caso o registro nao couber na pagina, mas a array ainda nao estiver no final, preenche-se o resto
				//da pagina com '@', e retorna-se o ponteiro de posicao do arquivo para a posicao que estava antes
				//de ler o registro atual, de forma que ele seja lido novamente e copiado para a proxima pagina.
				for(int i = offset; i < 32000; i++){
					pagina[i] = '@';
					tamRegAnterior++;
				}
				memcpy(&pagina[offsetTamRegistro], &tamRegAnterior, 4);
				offset = 32000;
				fwrite(pagina, 32000, 1, arquivoSaida);
			}
		}

		free(pagina);
	}

	fseek(arquivoSaida, 1, SEEK_SET);
	long int encad = -1;
	fwrite(&encad, 8, 1, arquivoSaida);

	//atualização do status
	char status = '1';
	rewind(arquivoSaida);
	fwrite(&status, 1, 1, arquivoSaida);	//status 1

    //fechando os arquivos
	fclose(arquivoBin);
    fclose(arquivoSaida);

	//escrita do arquivo bin na tela
	binarioNaTela2(nomeArquivoSaida);

	//free
	for(int i = 0; i < nReg; i++){
		if(registros[i].tamNomeServidor > 0)
			free(registros[i].nomeServidor);
		if(registros[i].tamCargoServidor > 0)
			free(registros[i].cargoServidor);
	}
	free(registros);
}