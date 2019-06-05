#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade1.h"

//funcionalidade 1
void criaArquivoBin(char *nomeArquivo){
	char registro[1000];
	char *paginaCab;
	FILE *arquivoLido, *arquivoBin;
	Cabecalho c;

	//abrindo os arquivos
	arquivoBin = fopen("arquivoTrab1.bin", "wb");
	arquivoLido = fopen(nomeArquivo, "r");
	
	//caso de erro: um dos arquivos nao existe
	if(arquivoLido == NULL || arquivoBin == NULL){
		printf("Falha no carregamento do arquivo.");
		if(arquivoLido != NULL)
			fclose(arquivoLido);
		if(arquivoBin != NULL)
			fclose(arquivoBin);
		return;
	}

	c = inicializaCab();		//inicaliza o registro de cabecalho
	c.status = '0';				//status '0' para indicar que o arquivo binario esta aberto

	//obtencao do registro de cabecalho
	c = getCabecalho(arquivoLido, c);
	c = ajustaCabecalho(c);

	//obtencao da pagina de cabecalho
	paginaCab = getPaginaCab(c);
	fwrite(paginaCab, 32000, 1, arquivoBin);

	//obtencao das demais paginas
	while(!feof(arquivoLido)){
		char *pagina;
		int offset = 0, offsetTamRegistro = 0, tamRegistro = 0, tamRegAnterior = 0;
		fpos_t posAnterior;

		pagina = (char *) malloc(sizeof(char) * 32000);

		while(offset < 32000){
			Dados d;
			d = inicializaRegDados();
			fgetpos(arquivoLido, &posAnterior);
			tamRegAnterior = tamRegistro;
			d = getDados(arquivoLido, d);
			tamRegistro = d.tamanhoRegistro;
			if(d.idServidor == -1){
				/* uma vez que o idServidor nao admite valores invalidos (o que ja eh garantido pelo csv), um valor
				invalido nesse campo (ocorre quando fscanf retorna 0, ver funcao getDados) indica o final do arquivo csv.*/
				fwrite(pagina, offset, 1, arquivoBin);
				offset = 32000;
			}
			else if((d.tamanhoRegistro + 5) <= (32000 - offset)){
				//caso o registro ainda couber na pagina ele sera copiado para a mesma
				pagina = getPaginaDados(d, pagina, &offset, &offsetTamRegistro);
			}
			else{
				/*caso o registro nao couber na pagina, mas o csv ainda nao estiver no final, preenche-se o resto
				da pagina com '@', e retorna-se o ponteiro de posicao do arquivo para a posicao que estava antes
				de ler o registro atual, de forma que ele seja lido novamente e copiado para a proxima pagina.*/
				for(int i = offset; i < 32000; i++){
					pagina[i] = '@';
					tamRegAnterior++;
				}
				memcpy(&pagina[offsetTamRegistro], &tamRegAnterior, 4);
				offset = 32000;
				fwrite(pagina, 32000, 1, arquivoBin);
				fsetpos(arquivoLido, &posAnterior);				
			}
			free(d.nomeServidor);
			free(d.cargoServidor);
		}

		free(pagina);
	}

	free(paginaCab);

	c.status = '1';			//status '1' indica que o arquivo binario foi fechado corretamente
	rewind(arquivoBin);
	fwrite(&c.status, 1, 1, arquivoBin);
	
	//fechando os arquivos
	fclose(arquivoLido);
	fclose(arquivoBin);

	printf("%s", "arquivoTrab1.bin");

}