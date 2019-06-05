#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade4.h"
#include "escreverTela2.h"
#include "lista.h"

//funcionalidade 4
void removeRegistro(char *nomeArquivo){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
	FILE* novoArquivoBin;
    int numeroRemocoes;
    char *paginaCab;			//array da pagina de disco
    int achou, paginasAcessadas = 0;
	long int arquivoOffset;
	Lista l;

	paginaCab = (char *) malloc(sizeof(char) * 32000);

	//abertura do arquivo original
    int init;
	init = inicializa(&arquivoBin, nomeArquivo, &paginaCab);
	if(init == 1)
		return;

	//obtencao das entradas
    scanf("%d", &numeroRemocoes);

    char nomeCampoArray[numeroRemocoes][100], *valorCampoArray[numeroRemocoes];

	for(int i = 0; i < numeroRemocoes; i++){
        char nomeCampo[100];
        char valorCampo[1000];

        scanf("%s", nomeCampo);
		scan_quote_string(valorCampo);
		trim(nomeCampo);
		trim(valorCampo);

		strcpy(nomeCampoArray[i], nomeCampo);
		valorCampoArray[i] = (char *) malloc(strlen(valorCampo) + 1);
		strcpy(valorCampoArray[i], valorCampo);
    }

	//obtenção da lista
	arquivoOffset = ftell(arquivoBin);
	inicializaLista(&l);
	getLista(arquivoBin, &l);
	fseek(arquivoBin, arquivoOffset, SEEK_CUR);

	//abertura do arquivo novo para escrita
	novoArquivoBin = fopen("arquivo-novo.bin", "wb");

	//atualização do status
	paginaCab[0] = '0'; //status 0
	fwrite(paginaCab, 32000, 1, novoArquivoBin);
	paginasAcessadas++;

	free(paginaCab);

	//busca pelos registros
	while(!feof(arquivoBin)){
		int offset = 0, offsetLista;			//armazena o byte offset
		char pagina[32000];
		int bytesLidos;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)

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

				//se o registro ja estiver removido, proceder para o proximo registro
				if(pagina[offset] == '*')
					pulaRemovido(&offset, pagina);

				//se ele nao estiver removido, verificar se ele possui os dados
				else if(pagina[offset] == '-'){
					Dados d;
					offsetAnterior = offset;
					
					//copia dos bytes da pagina de disco para a struct do registro de dados
					d = copiaRegistro(pagina, &offset);

					//verificar para todas as entradas
					for(int i = 0; i < numeroRemocoes; i++){
						if(!strcmp(nomeCampoArray[i], "idServidor")){	//verifica a tag idServidor
							if(atoi(valorCampoArray[i]) == d.idServidor){	//verifica o valor
								offset = offsetAnterior;
								offsetLista = paginasAcessadas * 32000 + offsetAnterior;
								insereListaTamanho(&l, offsetLista, d.tamanhoRegistro);	//insere o registro na lista
								pagina[offset] = '*';	//indica que o registro foi removido
								offset += 5;			//pula o indicador de tamanho, que sera mantido
								offset += 8;			//pula encadeamentoLista (sera modificado no final)
								while(offset < (offsetAnterior + 5 + d.tamanhoRegistro)){
									pagina[offset] = '@';
									offset++;
								}
								break;
							}
						}
						else if(!strcmp(nomeCampoArray[i], "salarioServidor")){	//verifica a tag salarioServidor
							if(strlen(valorCampoArray[i]) == 0){
								if(d.salarioServidor == -1){
									offset = offsetAnterior;
									offsetLista = paginasAcessadas * 32000 + offsetAnterior;
									insereListaTamanho(&l, offsetLista, d.tamanhoRegistro);	//insere o registro na lista
									pagina[offset] = '*';	//indica que o registro foi removido
									offset += 5;			//pula o indicador de tamanho, que sera mantido
									offset += 8;			//pula encadeamentoLista (sera modificado no final)
									while(offset < (offsetAnterior + 5 + d.tamanhoRegistro)){
										pagina[offset] = '@';
										offset++;
									}
									break;
								}
							}
							else if(atof(valorCampoArray[i]) == d.salarioServidor){	//verifica o valor
								offset = offsetAnterior;
								offsetLista = paginasAcessadas * 32000 + offsetAnterior;
								insereListaTamanho(&l, offsetLista, d.tamanhoRegistro);	//insere o registro na lista
								pagina[offset] = '*';	//indica que o registro foi removido
								offset += 5;			//pula o indicador de tamanho, que sera mantido
								offset += 8;			//pula encadeamentoLista (sera modificado no final)
								while(offset < (offsetAnterior + 5 + d.tamanhoRegistro)){
									pagina[offset] = '@';
									offset++;
								}
								break;
							}
						}
						else if(!strcmp(nomeCampoArray[i], "telefoneServidor")){	//verifica a tag telefoneServidor
							if(!strcmp(valorCampoArray[i], d.telefoneServidor)){	//verifica o valor
								offset = offsetAnterior;
								offsetLista = paginasAcessadas * 32000 + offsetAnterior;
								insereListaTamanho2(&l, offsetLista, d.tamanhoRegistro);	//insere o registro na lista
								pagina[offset] = '*';	//indica que o registro foi removido
								offset += 5;			//pula o indicador de tamanho, que sera mantido
								offset += 8;			//pula encadeamentoLista (sera modificado no final)
								while(offset < (offsetAnterior + 5 + d.tamanhoRegistro)){
									pagina[offset] = '@';
									offset++;
								}
								break;
							}
						}
						else if(!strcmp(nomeCampoArray[i], "nomeServidor")){	//verifica a tag nomeServidor
							if(!strcmp(valorCampoArray[i], d.nomeServidor)){	//verifica o valor
								offset = offsetAnterior;
								offsetLista = paginasAcessadas * 32000 + offsetAnterior;
								insereListaTamanho(&l, offsetLista, d.tamanhoRegistro);	//insere o registro na lista
								pagina[offset] = '*';	//indica que o registro foi removido
								offset += 5;			//pula o indicador de tamanho, que sera mantido
								offset += 8;			//pula encadeamentoLista (sera modificado no final)
								while(offset < (offsetAnterior + 5 + d.tamanhoRegistro)){
									pagina[offset] = '@';
									offset++;
								}
								break;
							}
						}
						else if(!strcmp(nomeCampoArray[i], "cargoServidor")){	//verifica a tag cargoServidor
							if(!strcmp(valorCampoArray[i], d.cargoServidor)){	//verifica o valor
								offset = offsetAnterior;
								offsetLista = paginasAcessadas * 32000 + offsetAnterior;
								insereListaTamanho(&l, offsetLista, d.tamanhoRegistro);	//insere o registro na lista
								pagina[offset] = '*';	//indica que o registro foi removido
								offset += 5;			//pula o indicador de tamanho, que sera mantido
								offset += 8;			//pula encadeamentoLista (sera modificado no final)
								while(offset < (offsetAnterior + 5 + d.tamanhoRegistro)){
									pagina[offset] = '@';
									offset++;
								}
								break;
							}
						}
					}

					//desaloca d.tamNomeServidor e d.tamCargoServidor
					if(d.tamNomeServidor > 0)
						free(d.nomeServidor);
					if(d.tamCargoServidor > 0)
						free(d.cargoServidor);
				}

				if(pagina[offset] != '-' && pagina[offset] != '*'){
					//os chars '-' e '*' marcam o inicio de um registro de dados.
					//assim, a nao existencia desses chars logo apos o fim do registro anterior indica que nao existem
					//mais registros, marcando assim o fim da pagina de disco.
					if(!feof(arquivoBin)){
						paginasAcessadas++;
						offset = 32000;
						fwrite(pagina, 32000, 1, novoArquivoBin);
					}
					else{
						paginasAcessadas++;
						fwrite(pagina, offset, 1, novoArquivoBin);
						offset = 32000;
					}
					
				}
			}
		}
	}

	//atualização do encadeamento
	getEncadLista(novoArquivoBin, l);

	//atualização do status
	char status = '1';
	rewind(novoArquivoBin);
	fwrite(&status, 1, 1, novoArquivoBin);	//status 1

	//fechando os arquivos
    fclose(arquivoBin);
	fclose(novoArquivoBin);

	//escrita do arquivo bin na tela
	binarioNaTela2("arquivo-novo.bin");

	//desalocação da heap
	for(int i = 0; i < numeroRemocoes; i++){
		free(valorCampoArray[i]);
	}
	desaloca(&l);

}