#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade4.h"

//funcionalidade 4
void removeRegistro(char *nomeArquivo){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
	FILE* novoArquivoBin;
    int numeroRemocoes;
    //////////////////////////////////////////
    char paginaCab[32000];			//array da pagina de disco
    int bytesLidos, achou, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)
	long int arquivoOffset;

    scanf("%d", &numeroRemocoes);

    char nomeCampoArray[numeroRemocoes][100], *valorCampoArray[numeroRemocoes];

    for(int i = 0; i < numeroRemocoes; i++){
        char nomeCampo[100];
        char *valorCampo;

        scanf("%s ", nomeCampo);
        valorCampo = readLine();
        strcpy(nomeCampoArray[i], nomeCampo);
        valorCampoArray[i] = (char *) malloc(sizeof(valorCampo));
        strcpy(valorCampoArray[i], valorCampo);


        //printf("%s\n", nomeCampoArray[i]);
        //printf("%s\n", valorCampoArray[i]);
    }

    arquivoBin = fopen(nomeArquivo, "rb");

    //caso de erro #1: nao existe um arquivo com o nome indicado pelo usuario
	if(arquivoBin == NULL){
		printf("Falha no processamento do arquivo.");
        //free(valorCampo);
		return;
	}

    //leitura da pagina do registro de cabecalho
	bytesLidos = fread(paginaCab, 32000, 1, arquivoBin);
	
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
	paginasAcessadas++;

	//busca pelos registros
	while(!feof(arquivoBin)){		//bytesLidos = 0 indica o final do arquivo
		int offset = 0;			//armazena o byte offset
		char pagina[32000];

		//obtencao da pagina de dados
		bytesLidos = fread(pagina, 32000, 1, arquivoBin);

		//caso de erro #3: nao ha registros
		if(bytesLidos == 0 && paginasAcessadas == 1){
			printf("Registro inexistente.");
			fclose(arquivoBin);
			fclose(novoArquivoBin);
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
						if(strcmp(nomeCampoArray[i], "idServidor")){	//verifica a tag idServidor
							if(atoi(valorCampoArray[i]) == d.idServidor){	//verifica o valor
								offset = offsetAnterior;
								pagina[offset] = '*';	//indicar que o registro foi removido
								offset += 5;			//pular o indicador de tamanho, que sera mantido
								offset += 8;			//pular encadeamentoLista enquanto nao sei o que fazer com ele
								while(offset < (offsetAnterior + 5 + d.tamanhoRegistro)){
									pagina[offset] = '@';
									offset++;
								}
							}
							break;
						}
						if(strcmp(nomeCampoArray[i], "salarioServidor")){	//verifica a tag salarioServidor
							if(atof(valorCampoArray[i]) == d.salarioServidor){	//verifica o valor
								offset = offsetAnterior;
								pagina[offset] = '*';	//indicar que o registro foi removido
								offset += 5;			//pular o indicador de tamanho, que sera mantido
								offset += 8;			//pular encadeamentoLista enquanto nao sei o que fazer com ele
								while(offset < (offsetAnterior + 5 + d.tamanhoRegistro)){
									pagina[offset] = '@';
									offset++;
								}
							}
							break;
						}
						if(strcmp(nomeCampoArray[i], "telefoneServidor")){	//verifica a tag telefoneServidor
							if(strcmp(valorCampoArray[i], d.telefoneServidor)){	//verifica o valor
								offset = offsetAnterior;
								pagina[offset] = '*';	//indicar que o registro foi removido
								offset += 5;			//pular o indicador de tamanho, que sera mantido
								offset += 8;			//pular encadeamentoLista enquanto nao sei o que fazer com ele
								while(offset < (offsetAnterior + 5 + d.tamanhoRegistro)){
									pagina[offset] = '@';
									offset++;
								}
							}
							break;
						}
						if(strcmp(nomeCampoArray[i], "nomeServidor")){	//verifica a tag nomeServidor
							if(strcmp(valorCampoArray[i], d.nomeServidor)){	//verifica o valor
								offset = offsetAnterior;
								pagina[offset] = '*';	//indicar que o registro foi removido
								offset += 5;			//pular o indicador de tamanho, que sera mantido
								offset += 8;			//pular encadeamentoLista enquanto nao sei o que fazer com ele
								while(offset < (offsetAnterior + 5 + d.tamanhoRegistro)){
									pagina[offset] = '@';
									offset++;
								}
							}
							break;
						}
						if(strcmp(nomeCampoArray[i], "cargoServidor")){	//verifica a tag cargoServidor
							if(strcmp(valorCampoArray[i], d.cargoServidor)){	//verifica o valor
								offset = offsetAnterior;
								pagina[offset] = '*';	//indicar que o registro foi removido
								offset += 5;			//pular o indicador de tamanho, que sera mantido
								offset += 8;			//pular encadeamentoLista enquanto nao sei o que fazer com ele
								while(offset < (offsetAnterior + 5 + d.tamanhoRegistro)){
									pagina[offset] = '@';
									offset++;
								}
							}
							break;
						}
					}
				}

				if(pagina[offset] != '-' || pagina[offset] != '*'){
					/*os chars '-' e '*' marcam o inicio de um registro de dados.
					assim, a nao existencia desses chars logo apos o fim do registro anterior indica que nao existem
					mais registros, marcando assim o fim da pagina de disco.*/
					paginasAcessadas++;
					offset = 32000;
					fwrite(pagina, 32000, 1, novoArquivoBin);
				}
			}
		}
	}
	

	/*if(achou == 1)
		printf("Número de páginas de disco acessadas: %d", paginasAcessadas);
	else
		printf("Registro inexistente.");*/

	//rewind(novoArquivoBin);
	//fwrite('1', 1, 1, novoArquivoBin);	//status 1

    fclose(arquivoBin);
	fclose(novoArquivoBin);
}