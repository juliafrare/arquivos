#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade6.h"
#include "escreverTela2.h"

//atualizacao de registros
char* atualiza(char *pagina, int *offset, int offsetAnterior, char *nomeCampoAtualizaArray, char *valorCampoAtualizaArray){
	int tamanho;
	*offset = offsetAnterior;

	if(strcmp(nomeCampoAtualizaArray, "idServidor")){	//verifica a tag idServidor
		int valor;

		valor = atoi(valorCampoAtualizaArray);
		*offset += 13;
		memcpy(&pagina[*offset], &valor, 4);

		}
	else if(strcmp(nomeCampoAtualizaArray, "salarioServidor")){	//verifica a tag salarioServidor
		double valor;
		
		if(strcmp(valorCampoAtualizaArray, "NULO")){
			valor = -1;
			*offset += 17;
			memcpy(&pagina[*offset], &valor, 8);
		}
		else{
			valor = atof(valorCampoAtualizaArray);
			*offset += 17;
			memcpy(&pagina[*offset], &valor, 8);
		}
	}
	else if(strcmp(nomeCampoAtualizaArray, "telefoneServidor")){	//verifica a tag telefoneServidor
		if(strcmp(valorCampoAtualizaArray, "NULO")){
			char valor[14] = {'@','@','@','@','@','@','@','@','@','@','@','@','@','@'};
			*offset += 25;
			memcpy(&pagina[*offset], valor, 14);
		}
		else{
			*offset += 25;
			memcpy(&pagina[*offset], &valorCampoAtualizaArray, 14);
		}
	}
	/*else if(strcmp(nomeCampoAtualizaArray, "nomeServidor")){	//verifica a tag nomeServidor
		int tamanhoNome;
		tamanhoNome = strlen(valorCampoAtualizaArray);
		
		*offset += 39;

		memcpy(&pagina[*offset], &tamanhoNome, 4);
		memcpy(&pagina[*offset], &valorCampoAtualizaArray, 14);
	}
	else if(strcmp(nomeCampoAtualizaArray, "cargoServidor")){	//verifica a tag cargoServidor
		
	}*/

	*offset = offsetAnterior;
	*offset += 1;
	memcpy(&tamanho, &pagina[*offset], 4);
	*offset += tamanho + 4;

}

//funcionalidade 6
void atualizaRegistro(char *nomeArquivo){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
	FILE* novoArquivoBin;
    int numeroRemocoes;
    //////////////////////////////////////////
    char paginaCab[32000];			//array da pagina de cabecalho
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

	scanf("%d", &numeroRemocoes);

    char nomeCampoBuscaArray[numeroRemocoes][100], *valorCampoBuscaArray[numeroRemocoes];
	char nomeCampoAtualizaArray[numeroRemocoes][100], *valorCampoAtualizaArray[numeroRemocoes];

	//obtencao dos valores de entrada
    for(int i = 0; i < numeroRemocoes; i++){
        char nomeCampoBusca[100], nomeCampoAtualiza[100];
        char valorCampoBusca[1000], valorCampoAtualiza[1000];

        scanf("%s", nomeCampoBusca); // Vai salvar nomeDoCampo em str1
		scan_quote_string(valorCampoBusca); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
		scanf("%s", nomeCampoAtualiza); // Vai salvar nomeDoCampo em str1
		scan_quote_string(valorCampoAtualiza);


		strcpy(nomeCampoBuscaArray[i], nomeCampoBusca);
		valorCampoBuscaArray[i] = (char *) malloc(strlen(valorCampoBusca) + 1);
		strcpy(valorCampoBuscaArray[i], valorCampoBusca);

		strcpy(nomeCampoAtualizaArray[i], nomeCampoAtualiza);
		valorCampoAtualizaArray[i] = (char *) malloc(strlen(valorCampoAtualiza) + 1);
		strcpy(valorCampoAtualizaArray[i], valorCampoAtualiza);

        /*printf("%s\n", nomeCampoBuscaArray[i]);
        printf("%s\n", valorCampoBuscaArray[i]);
        printf("%s\n", nomeCampoAtualizaArray[i]);
        printf("%s\n", valorCampoAtualizaArray[i]);
		printf("%d\n", i);*/

    }

/*
	//busca pelos registros
	while(!feof(arquivoBin)){
		int offset = 0;			//armazena o byte offset
		char *pagina;

		pagina = (char *) malloc(sizeof(char) * 32000);

		//obtencao da pagina de dados
		bytesLidos = fread(pagina, 32000, 1, arquivoBin);

		//caso de erro #3: nao ha registros
		if(bytesLidos == 0 && paginasAcessadas == 1){
			printf("Registro inexistente.");
			fclose(novoArquivoBin);
			fclose(arquivoBin);
			return;
		}
		
		if(pagina[0] == '-' || pagina[0] == '*'){
			int offsetAnterior = 0;			//byte offset do inicio do registro

			while(offset < 32000){			//o ultimo byte da pagina esta no offset 32000 - 1 (ou seja, pagina[31999])
				//printf("%d\n", offset);

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
					offsetAnterior = offset;
					
					//copia dos bytes da pagina de disco para a struct do registro de dados
					d = copiaRegistro(pagina, &offset);

					//verificar para todas as entradas
					for(int i = 0; i < numeroRemocoes; i++){
						if(strcmp(nomeCampoBuscaArray[i], "idServidor")){	//verifica a tag idServidor
							if(atoi(valorCampoBuscaArray[i]) == d.idServidor){	//verifica o valor
								pagina = atualiza(pagina, &offset, offsetAnterior, nomeCampoAtualizaArray[i], valorCampoAtualizaArray[i]);
							}
							break;
						}
						if(strcmp(nomeCampoBuscaArray[i], "salarioServidor")){	//verifica a tag salarioServidor
							if(atof(valorCampoBuscaArray[i]) == d.salarioServidor){	//verifica o valor
								pagina = atualiza(pagina, &offset, offsetAnterior, nomeCampoAtualizaArray[i], valorCampoAtualizaArray[i]);
							}
							break;
						}
						if(strcmp(nomeCampoBuscaArray[i], "telefoneServidor")){	//verifica a tag telefoneServidor
							if(strcmp(valorCampoBuscaArray[i], d.telefoneServidor)){	//verifica o valor
								pagina = atualiza(pagina, &offset, offsetAnterior, nomeCampoAtualizaArray[i], valorCampoAtualizaArray[i]);
							}
							break;
						}
						if(strcmp(nomeCampoBuscaArray[i], "nomeServidor")){	//verifica a tag nomeServidor
							if(strcmp(valorCampoBuscaArray[i], d.nomeServidor)){	//verifica o valor
								pagina = atualiza(pagina, &offset, offsetAnterior, nomeCampoAtualizaArray[i], valorCampoAtualizaArray[i]);
							}
							break;
						}
						if(strcmp(nomeCampoBuscaArray[i], "cargoServidor")){	//verifica a tag cargoServidor
							if(strcmp(valorCampoBuscaArray[i], d.cargoServidor)){	//verifica o valor
								pagina = atualiza(pagina, &offset, offsetAnterior, nomeCampoAtualizaArray[i], valorCampoAtualizaArray[i]);
							}
							break;
						}
					}
				}

				if(pagina[offset] != '-' || pagina[offset] != '*'){
					//os chars '-' e '*' marcam o inicio de um registro de dados.
					//assim, a nao existencia desses chars logo apos o fim do registro anterior indica que nao existem
					//mais registros, marcando assim o fim da pagina de disco.
					paginasAcessadas++;
					offset = 32000;
				}
			}
		}
	}*/

	//rewind(novoArquivoBin);
	//fwrite('1', 1, 1, novoArquivoBin);	//status 1

	fclose(novoArquivoBin);
    fclose(arquivoBin);
}