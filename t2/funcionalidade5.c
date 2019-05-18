#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade5.h"
#include "escreverTela2.h"
#include "lista.h"

//obtem os dados para a funcionalidade 5
Dados getDados2(Dados d){

	//OBTENÇÃO DAS ENTRADAS
	char valorIdServidor[20], valorSalarioServidor[20], valorTelefoneServidor[14], valorNomeServidor[500], valorCargoServidor[500];

	scan_quote_string(valorIdServidor);
	scan_quote_string(valorSalarioServidor);
	scan_quote_string(valorTelefoneServidor);
	scan_quote_string(valorNomeServidor);
	scan_quote_string(valorCargoServidor);

	//COPIA DAS ENTRADAS PARA A STRUCT DO REG. DE DADOS
	d.idServidor = atoi(valorIdServidor);

	if(strlen(valorSalarioServidor) == 0)
		d.salarioServidor = -1;
	else
		d.salarioServidor = atof(valorSalarioServidor);

	if(strlen(valorTelefoneServidor) == 0){
		d.telefoneServidor[0] = '\0';
		for(int i = 1; i < sizeof(d.telefoneServidor); i++){
			d.telefoneServidor[i] = '@';
		}
	}
	else
		strcpy(d.telefoneServidor, valorTelefoneServidor);
	
	d.nomeServidor = (char *) malloc(sizeof(valorNomeServidor));
	strcpy(d.nomeServidor, valorNomeServidor);

	d.cargoServidor = (char *) malloc(sizeof(valorCargoServidor));
	strcpy(d.cargoServidor, valorCargoServidor);


	if(strlen(d.nomeServidor) == 0){
		d.tamNomeServidor = strlen(d.nomeServidor);
		d.tamanhoRegistro -= 5;
	}
	else{
		d.tamNomeServidor = strlen(d.nomeServidor) + 2;
		d.tamanhoRegistro += d.tamNomeServidor - 1;
	}

	if(strlen(d.cargoServidor) == 0){
		d.tamCargoServidor = strlen(d.cargoServidor);
		d.tamanhoRegistro -= 5;
	}
	else{
		d.tamCargoServidor = strlen(d.cargoServidor) + 2;
		d.tamanhoRegistro += d.tamCargoServidor - 1;
	}
    
    return d;
}

//funcionalidade 5
void insereRegistro(char *nomeArquivo){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
	FILE* novoArquivoBin;
    int numRegistros;
    char paginaCab[32000];			//array da pagina de cabecalho
    int bytesLidos, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)
	long int arquivoOffset, removeOffset;
	Lista l;

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

	//obtenção da lista ordenada
	arquivoOffset = ftell(arquivoBin);
	inicializaLista(&l);
	getLista(arquivoBin, &l);
	fseek(arquivoBin, arquivoOffset, SEEK_CUR);


	novoArquivoBin = fopen("arquivo-novo.bin", "wb");

	paginaCab[0] = '0'; //status 0
	fwrite(paginaCab, 32000, 1, novoArquivoBin);

	scanf("%d", &numRegistros);

    Dados d[numRegistros];

	for(int i = 0; i < numRegistros; i++){
		d[i] = inicializaRegDados();
        d[i] = getDados2(d[i]);	
    }

	//copia das paginas p/ arquivo novo
	copiaArquivo(arquivoBin, novoArquivoBin, paginasAcessadas);

	//escrever os registros no arquivo
	for(int i = 0; i < numRegistros; i++){
		removeOffset = removeLista(&l, d[i].tamanhoRegistro);

		if(removeOffset != -1){
			fseek(novoArquivoBin, removeOffset, SEEK_SET);
			fwrite(&d[i].removido, 1, 1, novoArquivoBin);
			fseek(novoArquivoBin, 4, SEEK_CUR);
		}
		else{
			fseek(novoArquivoBin, 0, SEEK_END);
			fwrite(&d[i].removido, 1, 1, novoArquivoBin);
			fwrite(&d[i].tamanhoRegistro, 4, 1, novoArquivoBin);
		}

		fwrite(&d[i].encadeamentoLista, 8, 1, novoArquivoBin);
		fwrite(&d[i].idServidor, 4, 1, novoArquivoBin);
		fwrite(&d[i].salarioServidor, 8, 1, novoArquivoBin);
		fwrite(&d[i].telefoneServidor, 14, 1, novoArquivoBin);
		if(d[i].tamNomeServidor > 0){
			fwrite(&d[i].tamNomeServidor, 4, 1, novoArquivoBin);
			fwrite(&d[i].tagCampo4, 1, 1, novoArquivoBin);
			fwrite(d[i].nomeServidor, strlen(d[i].nomeServidor) + 1, 1, novoArquivoBin);
		}
		if(d[i].tamCargoServidor > 0){
			fwrite(&d[i].tamCargoServidor, 4, 1, novoArquivoBin);
			fwrite(&d[i].tagCampo5, 1, 1, novoArquivoBin);
			fwrite(d[i].cargoServidor, strlen(d[i].cargoServidor) + 1, 1, novoArquivoBin);
		}

		if(removeOffset != -1)
			getEncadLista(novoArquivoBin, l);
	}

	char status = '1';
	rewind(novoArquivoBin);
	fwrite(&status, 1, 1, novoArquivoBin);	//status 1

	fclose(novoArquivoBin);
    fclose(arquivoBin);

	binarioNaTela2("arquivo-novo.bin");
}