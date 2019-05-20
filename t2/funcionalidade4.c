#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade4.h"
#include "escreverTela2.h"
#include "lista.h"

//funcao que remove os registros
char* remocao(char* pagina, int *offset, int offsetAnterior, Dados d){
	*offset = offsetAnterior;
	pagina[*offset] = '*';	//indicar que o registro foi removido
	*offset += 5;			//pular o indicador de tamanho, que sera mantido
	*offset += 8;			//pular encadeamentoLista enquanto nao sei o que fazer com ele
	while(*offset < (offsetAnterior + 5 + d.tamanhoRegistro)){
		pagina[*offset] = '@';
		*offset++;
	}

	return pagina;
}

//funcao que compara dois valores de telefoneServidor
int comparaTelefone(char *valor1, char *valor2, int tamanho){
	for(int i = 0; i < tamanho; i++){
		if(valor1[i] != valor2[i]){
			return 1;
		}
	}
	return 0;
}

//funcionalidade 4
void removeRegistro(char *nomeArquivo){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
	FILE* novoArquivoBin;
    int numeroRemocoes;
    //////////////////////////////////////////
    char paginaCab[32000];			//array da pagina de disco
    int bytesLidos, achou, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)
	long int arquivoOffset;
	Lista l;

    scanf("%d", &numeroRemocoes);

    char nomeCampoArray[numeroRemocoes][100], *valorCampoArray[numeroRemocoes];

	//obtencao das entradas
	for(int i = 0; i < numeroRemocoes; i++){
        char nomeCampo[100];
        char valorCampo[1000];

        scanf("%s", nomeCampo); // Vai salvar nomeDoCampo em str1
		scan_quote_string(valorCampo); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
		trim(nomeCampo);
		trim(valorCampo);

		strcpy(nomeCampoArray[i], nomeCampo);
		valorCampoArray[i] = (char *) malloc(strlen(valorCampo) + 1);
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

	arquivoOffset = ftell(arquivoBin);

	//obtencao da lista
	inicializaLista(&l);
	getLista(arquivoBin, &l);

	fseek(arquivoBin, arquivoOffset, SEEK_CUR);

	novoArquivoBin = fopen("arquivo-novo.bin", "wb");

	paginaCab[0] = '0'; //status 0
	fwrite(paginaCab, 32000, 1, novoArquivoBin);
	paginasAcessadas++;

	//busca pelos registros
	while(!feof(arquivoBin)){
		int offset = 0, offsetLista;			//armazena o byte offset
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
							//printf("entrou aqui 2\n");
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
							//printf("entrou aqui 4\n");
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
							//printf("entrou aqui 5\n");
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

	//printLista(l);
	getEncadLista(novoArquivoBin, l);

	char status = '1';
	rewind(novoArquivoBin);
	fwrite(&status, 1, 1, novoArquivoBin);	//status 1

    fclose(arquivoBin);
	fclose(novoArquivoBin);

	//desaloca(&l);

	binarioNaTela2("arquivo-novo.bin");

	for(int i = 0; i < numeroRemocoes; i++){
		free(valorCampoArray[i]);
	}

	desaloca(&l);


}