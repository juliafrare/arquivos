#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade8.h"
#include "escreverTela2.h"

void mergeArquivos(char *nomeArquivo, char *nomeArquivo2){
    FILE* arquivoBin1;
	FILE* arquivoBin2;			//ponteiro para o arquivo .bin
	FILE* arquivoSaida;
	char nomeArquivoSaida[100];
    char *paginaCab, *paginaCab2;	//array da pagina de cabecalho

	paginaCab = (char *) malloc(sizeof(char) * 32000);
	paginaCab2 = (char *) malloc(sizeof(char) * 32000);

	//abertura dos arquivos originais
	int init1, init2;
	init1 = inicializa(&arquivoBin1, nomeArquivo, &paginaCab);
	if(init1 == 1){
		free(paginaCab);
		free(paginaCab2);
		return;
	}

	init2 = inicializa(&arquivoBin2, nomeArquivo2, &paginaCab2);
	if(init2 == 1){
		fclose(arquivoBin1);
		free(paginaCab);
		free(paginaCab2);
		return;
	}

	//nome do arquivo de saida
	scan_quote_string(nomeArquivoSaida);

	//abertura do arquivo de saida
	arquivoSaida = fopen(nomeArquivoSaida, "wb");

    //copia do registro de cabecalho
	paginaCab[0] = '0'; //status 0
	fwrite(paginaCab, 32000, 1, arquivoSaida);
	fseek(arquivoBin2, 32000, SEEK_SET);

	//adicionar registros p/array
	Dados *registros;
	registros = (Dados *) malloc(sizeof(Dados) * 100000);
	int nReg = 0;

	//busca pelos registros
	while(!feof(arquivoBin1)){
		int offset = 0;	//armazena o byte offset
		char pagina[32000];

		//obtencao da pagina de dados
		fread(pagina, 32000, 1, arquivoBin1);
		
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

	while(!feof(arquivoBin2)){
		int offset = 0;	//armazena o byte offset
		char pagina[32000];

		//obtencao da pagina de dados
		fread(pagina, 32000, 1, arquivoBin2);
		
		if(pagina[0] == '-' || pagina[0] == '*'){

			while(offset < 32000){			//o ultimo byte da pagina esta no offset 32000 - 1 (ou seja, pagina[31999])

				//se o registro ja estiver removido, proceder para o proximo registro
				if(pagina[offset] == '*'){
					int tamanho;
					offset += 1;
					memcpy(&tamanho, &pagina[offset], 4);
					offset += tamanho + 4;	//atualiza o byte offset para o offset do proximo registro
				}
				//se ele nao estiver removido, verificar se ele possui os dados
				else if(pagina[offset] == '-'){
					Dados d;

					d = inicializaRegDados();
					
					//copia dos bytes da pagina de disco para a struct do registro de dados
					d = copiaRegistro(pagina, &offset);
					atualizaTamanho(&d);

					if(buscaBin(registros, d.idServidor, 0, nReg - 1) == -1){
						registros[nReg] = copiaRegistro2(d);
						nReg++;
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
					offset = 32000;
				}
			}
		}
	}

	//ordenar lista
	mergesort(registros, 0, nReg - 1);

	//copiar registros para arquivo de saida
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
				//free(pagina);
				j++;
			}
			else if((registros[j].tamanhoRegistro + 5) <= (32000 - offset)){
				//caso o registro ainda couber na pagina ele sera copiado para a mesma
				pagina = getPaginaDados(registros[j], pagina, &offset, &offsetTamRegistro);
				
				if(offset == 32000)
					fwrite(pagina, 32000, 1, arquivoSaida);
					//free(pagina);
				
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
				//free(pagina);
			}
		}

		free(pagina);
	}

	//atualização do status
	char status = '1';
	rewind(arquivoSaida);
	fwrite(&status, 1, 1, arquivoSaida);	//status 1

    //fechando os arquivos
	fclose(arquivoSaida);
    fclose(arquivoBin1);
	fclose(arquivoBin2);

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
	free(paginaCab);
	free(paginaCab2);
}