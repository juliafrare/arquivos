#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade6.h"
#include "escreverTela2.h"
#include "lista.h"

void escreveRegistro2(FILE *fp, Lista *l, Dados d){
	int removeOffset;

	removeOffset = removeLista(l, d.tamanhoRegistro);

	if(removeOffset != -1){
		fseek(fp, removeOffset, SEEK_SET);
		fwrite(&d.removido, 1, 1, fp);
		fseek(fp, 4, SEEK_CUR);
	}
	else{
		fseek(fp, 0, SEEK_END);
		fwrite(&d.removido, 1, 1, fp);
		fwrite(&d.tamanhoRegistro, 4, 1, fp);
	}

	fwrite(&d.encadeamentoLista, 8, 1, fp);
	fwrite(&d.idServidor, 4, 1, fp);
	fwrite(&d.salarioServidor, 8, 1, fp);
	fwrite(&d.telefoneServidor, 14, 1, fp);
	if(d.tamNomeServidor > 0){
		fwrite(&d.tamNomeServidor, 4, 1, fp);
		fwrite(&d.tagCampo4, 1, 1, fp);
		fwrite(d.nomeServidor, strlen(d.nomeServidor) + 1, 1, fp);
	}
	if(d.tamCargoServidor > 0){
		fwrite(&d.tamCargoServidor, 4, 1, fp);
		fwrite(&d.tagCampo5, 1, 1, fp);
		fwrite(d.cargoServidor, strlen(d.cargoServidor) + 1, 1, fp);
	}

	if(removeOffset != -1)
		getEncadLista(fp, *l);
}

//atualizacao de registros
char* atualiza(FILE *fp, long int offset, char *nomeCampoAtualizaArray, char *valorCampoAtualizaArray, Lista *l, Dados d){
	int tamanhoReg; //tamanho do registro

	//printf("%ld\n", offset);

	fseek(fp, offset, SEEK_SET);	//pula p/offset do registro
	fseek(fp, 1, SEEK_CUR);	//pula removido
	fseek(fp, 4, SEEK_CUR);	//pula tamanhoRegistro
	fseek(fp, 8, SEEK_CUR);	//pula encadeamentoLista

	if(!strcmp(nomeCampoAtualizaArray, "idServidor")){	//verifica a tag idServidor
		int valor;

		valor = atoi(valorCampoAtualizaArray);
		fwrite(&valor, 4, 1, fp);

	}
	else if(!strcmp(nomeCampoAtualizaArray, "salarioServidor")){	//verifica a tag salarioServidor
		double valor;

		fseek(fp, 4, SEEK_CUR);	//pula idServidor
		
		if(!strlen(valorCampoAtualizaArray)){
			valor = -1;
		}
		else{
			valor = atof(valorCampoAtualizaArray);
		}

		fwrite(&valor, 8, 1, fp);

	}
	else if(!strcmp(nomeCampoAtualizaArray, "telefoneServidor")){	//verifica a tag telefoneServidor
		fseek(fp, 4, SEEK_CUR);	//pula idServidor
		fseek(fp, 8, SEEK_CUR);	//pula salarioServidor
		
		if(!strlen(valorCampoAtualizaArray)){
			char valor[14];
			valor[0] = '\0';
			for(int i = 1; i < 14; i++){
				valor[i] = '@';
			}
			fwrite(&valor, 14, 1, fp);
		}
		else{
			fwrite(valorCampoAtualizaArray, 14, 1, fp);
		}
	}
	else if(!strcmp(nomeCampoAtualizaArray, "nomeServidor")){	//verifica a tag nomeServidor
		//printf("atualiza nome\n");
		//printf("%d\n", d.tamNomeServidor);
		int tamanhoNome;
		//char tag;
		char lixo = '@';
		tamanhoNome = strlen(valorCampoAtualizaArray) + 2;

		fseek(fp, 4, SEEK_CUR);	//pula idServidor
		fseek(fp, 8, SEEK_CUR);	//pula salarioServidor
		fseek(fp, 14, SEEK_CUR);	//pula telefoneServidor

		if(d.tamNomeServidor == -1){
			//printf("entrou aqui\n");
			if(tamanhoNome > 0){
				long int novoOffset;

				//remover registro
				char removido = '*';
				fseek(fp, offset, SEEK_SET);
				insereListaTamanho(l, offset, d.tamanhoRegistro);	//insere o registro na lista
				fwrite(&removido, 1, 1, fp);	//indica que o registro foi removido
				fseek(fp, 4, SEEK_CUR);	//pula o indicador de tamanho, que sera mantido
				fseek(fp, 8, SEEK_CUR);	//pula encadeamentoLista (sera modificado no final)
				for(int i = 0; i < (d.tamanhoRegistro - 8); i++){
					fwrite(&lixo, 1, 1, fp);
				}

				//atualizar tamanho do registro e campo nomeServidor
				d.tamNomeServidor = tamanhoNome;
				d.tamanhoRegistro += d.tamNomeServidor + 4;
				d.tagCampo4 = 'n';
				d.nomeServidor = (char *) malloc(strlen(valorCampoAtualizaArray) + 1);
				strcpy(d.nomeServidor, valorCampoAtualizaArray);
				//procurar por registro removido q tenha espaço
				//copiar dados p/novo registro
				escreveRegistro2(fp, l, d);

			}
		}
		else{
			if(tamanhoNome > d.tamNomeServidor){
				long int novoOffset;

				//remover registro
				char removido = '*';
				fseek(fp, offset, SEEK_SET);
				insereListaTamanho(l, offset, d.tamanhoRegistro);	//insere o registro na lista
				fwrite(&removido, 1, 1, fp);	//indica que o registro foi removido
				fseek(fp, 4, SEEK_CUR);	//pula o indicador de tamanho, que sera mantido
				fseek(fp, 8, SEEK_CUR);	//pula encadeamentoLista (sera modificado no final)
				for(int i = 0; i < (d.tamanhoRegistro - 8); i++){
					fwrite(&lixo, 1, 1, fp);
				}

				//atualizar tamanho do registro e campo nomeServidor
				d.tamanhoRegistro += (tamanhoNome - d.tamNomeServidor);
				d.tamNomeServidor = tamanhoNome;
				d.nomeServidor = (char *) malloc(sizeof(valorCampoAtualizaArray));
				strcpy(d.cargoServidor, valorCampoAtualizaArray);
				//procurar por registro removido q tenha espaço
				//copiar dados p/novo registro
				escreveRegistro2(fp, l, d);
			}
			else{
				fwrite(valorCampoAtualizaArray, sizeof(valorCampoAtualizaArray), 1, fp);

				if(tamanhoNome < d.tamNomeServidor){	//se o valor novo for MENOR que o original
					for(int i = 0; i < (d.tamNomeServidor - tamanhoNome); i++)
						fwrite(&lixo, 1, 1, fp);

				}
			}
		}
	}
	else if(!strcmp(nomeCampoAtualizaArray, "cargoServidor")){	//verifica a tag cargoServidor
		//printf("atualiza cargo\n");
		int tamanhoCargo;
		//char tag;
		char lixo = '@';
		tamanhoCargo = strlen(valorCampoAtualizaArray) + 2;

		fseek(fp, 4, SEEK_CUR);	//pula idServidor
		fseek(fp, 8, SEEK_CUR);	//pula salarioServidor
		fseek(fp, 14, SEEK_CUR);	//pula telefoneServidor
		//pula nomeServidor
		if(d.tamNomeServidor > 0){
			fseek(fp, 4, SEEK_CUR);	//pula tamNomeServidor
			fseek(fp, 1, SEEK_CUR);	//pula tagCampo4
			fseek(fp, strlen(d.nomeServidor) + 1, SEEK_CUR); //pula nomeServidor
		}

		if(d.tamCargoServidor == -1){
			if(tamanhoCargo > 0){
				long int novoOffset;

				//remover registro
				char removido = '*';
				fseek(fp, offset, SEEK_SET);
				insereListaTamanho(l, offset, d.tamanhoRegistro);	//insere o registro na lista
				fwrite(&removido, 1, 1, fp);	//indica que o registro foi removido
				fseek(fp, 4, SEEK_CUR);	//pula o indicador de tamanho, que sera mantido
				fseek(fp, 8, SEEK_CUR);	//pula encadeamentoLista (sera modificado no final)
				for(int i = 0; i < (d.tamanhoRegistro - 8); i++){
					fwrite(&lixo, 1, 1, fp);
				}

				//atualizar tamanho do registro e campo nomeServidor
				d.tamCargoServidor = tamanhoCargo;
				d.tamanhoRegistro += d.tamCargoServidor + 4;
				d.tagCampo5 = 'c';
				d.cargoServidor = (char *) malloc(strlen(valorCampoAtualizaArray) + 1);
				strcpy(d.cargoServidor, valorCampoAtualizaArray);
				//procurar por registro removido q tenha espaço
				//copiar dados p/novo registro
				escreveRegistro2(fp, l, d);

			}
		}
		else{
			if(tamanhoCargo > d.tamCargoServidor){
				long int novoOffset;

				//remover registro
				char removido = '*';
				fseek(fp, offset, SEEK_SET);
				insereListaTamanho(l, offset, d.tamanhoRegistro);	//insere o registro na lista
				fwrite(&removido, 1, 1, fp);	//indica que o registro foi removido
				fseek(fp, 4, SEEK_CUR);	//pula o indicador de tamanho, que sera mantido
				fseek(fp, 8, SEEK_CUR);	//pula encadeamentoLista (sera modificado no final)
				for(int i = 0; i < (d.tamanhoRegistro - 8); i++){
					fwrite(&lixo, 1, 1, fp);
				}

				//atualizar tamanho do registro e campo nomeServidor
				d.tamanhoRegistro += (tamanhoCargo - d.tamCargoServidor);
				d.tamCargoServidor = tamanhoCargo;
				d.cargoServidor = (char *) malloc(strlen(valorCampoAtualizaArray) + 1);
				strcpy(d.cargoServidor, valorCampoAtualizaArray);
				//procurar por registro removido q tenha espaço
				//copiar dados p/novo registro
				escreveRegistro2(fp, l, d);
			}
			else{
				fwrite(valorCampoAtualizaArray, sizeof(valorCampoAtualizaArray), 1, fp);

				if(tamanhoCargo < d.tamCargoServidor){	//se o valor novo for MENOR que o original
					for(int i = 0; i < (d.tamCargoServidor - tamanhoCargo); i++)
						fwrite(&lixo, 1, 1, fp);

				}
			}
		}
	}

}

//funcionalidade 6
void atualizaRegistro(char *nomeArquivo){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
	FILE* novoArquivoBin;
    int numeroAtualiza;
    //////////////////////////////////////////
    char paginaCab[32000];			//array da pagina de cabecalho
    int bytesLidos, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)
	long int arquivoOffset;
	Lista l;

    arquivoBin = fopen(nomeArquivo, "rb");

    //caso de erro #1: nao existe um arquivo com o nome indicado pelo usuario
	if(arquivoBin == NULL){
		printf("Falha no processamento do arquivo.");
		return;
	}

    //leitura da pagina do registro de cabecalho
	bytesLidos = fread(paginaCab, 32000, 1, arquivoBin);
	paginasAcessadas++;

    //caso de erro #2: o arquivo esta inconsistente (status 0)
    if(paginaCab[0] == '0'){
		printf("Falha no processamento do arquivo.");
		fclose(arquivoBin);
		return;
	}

	//obtenção da lista ordenada
	arquivoOffset = ftell(arquivoBin);
	inicializaLista(&l);
	getLista(arquivoBin, &l);
	fseek(arquivoBin, arquivoOffset, SEEK_CUR);

	//abrindo o arquivo novo
	novoArquivoBin = fopen("arquivo-novo.bin", "wb+");

	paginaCab[0] = '0'; //status 0
	fwrite(paginaCab, 32000, 1, novoArquivoBin);

	scanf("%d", &numeroAtualiza);

	//obtencao dos valores de entrada
	long int offsetRegistros[numeroAtualiza];	//armazena o byte offset dos registros
	Dados registros[numeroAtualiza];	//armazena os registros (caso eles tenham q ser removidos)
    char nomeCampoBuscaArray[numeroAtualiza][100], *valorCampoBuscaArray[numeroAtualiza];
	char nomeCampoAtualizaArray[numeroAtualiza][100], *valorCampoAtualizaArray[numeroAtualiza];
    
	for(int i = 0; i < numeroAtualiza; i++){
        char nomeCampoBusca[100], nomeCampoAtualiza[100];
        char valorCampoBusca[1000], valorCampoAtualiza[1000];

		offsetRegistros[i] = -1; //inicializa offsetRegistros

        scanf("%s", nomeCampoBusca);
		scan_quote_string(valorCampoBusca);
		scanf("%s", nomeCampoAtualiza);
		scan_quote_string(valorCampoAtualiza);

		trim(nomeCampoBusca);
		trim(valorCampoBusca);
		trim(nomeCampoAtualiza);
		trim(valorCampoAtualiza);

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

	//copia das paginas p/ arquivo novo
	copiaArquivo(arquivoBin, novoArquivoBin, paginasAcessadas);

	fseek(arquivoBin, 0, SEEK_SET);

	//busca pelos registros e armazenamento dos seus offsets
	while(!feof(arquivoBin)){
		int offset = 0;	//armazena o byte offset
		char pagina[32000];

		//printf("%ld\n", ftell(arquivoBin));

		//obtencao da pagina de dados
		fread(pagina, 32000, 1, arquivoBin);
		
		if(pagina[0] == '-' || pagina[0] == '*'){
			int offsetAnterior = 0;			//byte offset do inicio do registro
			int achou = 0;

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

					d = inicializaRegDados();
					
					//copia dos bytes da pagina de disco para a struct do registro de dados
					d = copiaRegistro(pagina, &offset);
					//printf("%c\n", pagina[offset]);

					//verificar para todas as entradas
					for(int i = 0; i < numeroAtualiza; i++){
						if(!strcmp(nomeCampoBuscaArray[i], "idServidor")){	//verifica a tag idServidor
							if(atoi(valorCampoBuscaArray[i]) == d.idServidor){	//verifica o valor
								achou = 1;
								registros[i] = copiaRegistro2(d);
								//registros[i] = d;
								offsetRegistros[i] = paginasAcessadas * 32000 + offsetAnterior;
							}
						}
						else if(!strcmp(nomeCampoBuscaArray[i], "salarioServidor")){	//verifica a tag salarioServidor
							if(atof(valorCampoBuscaArray[i]) == d.salarioServidor){	//verifica o valor
								achou = 1;
								registros[i] = copiaRegistro2(d);
								//registros[i] = d;
								offsetRegistros[i] = paginasAcessadas * 32000 + offsetAnterior;
							}
						}
						else if(!strcmp(nomeCampoBuscaArray[i], "telefoneServidor")){	//verifica a tag telefoneServidor
							if(!strcmp(valorCampoBuscaArray[i], d.telefoneServidor)){	//verifica o valor
								achou = 1;
								registros[i] = copiaRegistro2(d);
								//registros[i] = d;
								offsetRegistros[i] = paginasAcessadas * 32000 + offsetAnterior;
							}
						}
						else if(!strcmp(nomeCampoBuscaArray[i], "nomeServidor")){	//verifica a tag nomeServidor
							//printf("busca nome\n");
							//printf("%s ", valorCampoBuscaArray[i]);
							//printf("%s\n", d.nomeServidor);
							if(!strcmp(valorCampoBuscaArray[i], d.nomeServidor)){	//verifica o valor
								//printf("busca nome2\n");
								achou = 1;
								registros[i] = copiaRegistro2(d);
								//registros[i] = d;
								offsetRegistros[i] = paginasAcessadas * 32000 + offsetAnterior;
							}
						}
						else if(!strcmp(nomeCampoBuscaArray[i], "cargoServidor")){	//verifica a tag cargoServidor
							if(!strcmp(valorCampoBuscaArray[i], d.cargoServidor)){	//verifica o valor
								achou = 1;
								registros[i] = copiaRegistro2(d);
								//registros[i] = d;
								offsetRegistros[i] = paginasAcessadas * 32000 + offsetAnterior;
							}
						}
					}

					/*if(achou != 1){
						if(d.tamNomeServidor != -1)
							free(d.nomeServidor);
						if(d.tamCargoServidor != -1)
							free(d.cargoServidor);
					}*/
					/*if(d.tamNomeServidor > 0)
						free(d.nomeServidor);
					if(d.tamCargoServidor > 0)
						free(d.cargoServidor);*/
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

	//realizar alteracoes
	for(int i = 0; i < numeroAtualiza; i++)
		atualiza(novoArquivoBin, offsetRegistros[i], nomeCampoAtualizaArray[i], valorCampoAtualizaArray[i], &l, registros[i]);
	
	getEncadLista(novoArquivoBin, l);

	desaloca(&l);

	char status = '1';
	rewind(novoArquivoBin);
	fwrite(&status, 1, 1, novoArquivoBin);	//status 1

	fclose(novoArquivoBin);
    fclose(arquivoBin);

	binarioNaTela2("arquivo-novo.bin");

	for(int i = 0; i < numeroAtualiza; i++){
		free(valorCampoBuscaArray[i]);
		free(valorCampoAtualizaArray[i]);
		if(registros[i].tamNomeServidor != -1)
			free(registros[i].nomeServidor);
		if(registros[i].tamCargoServidor != -1)
			free(registros[i].cargoServidor);
	}
}