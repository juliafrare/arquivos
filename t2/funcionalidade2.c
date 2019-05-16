#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade1.h"

//funcionalidade 2
void leArquivoBin(char *nomeArquivo){
	FILE* arquivoBin;										//ponteiro para o arquivo .bin	
	char pagina[32000];										//array da pagina de disco
	int bytesLidos, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)

	arquivoBin = fopen(nomeArquivo, "rb");

	//caso de erro #1: nao existe um arquivo com o nome indicado pelo usuario
	if(arquivoBin == NULL){
		printf("Falha no processamento do arquivo.");
		return;
	}

	//leitura da pagina do registro de cabecalho
	bytesLidos = fread(pagina, 32000, 1, arquivoBin);
	paginasAcessadas++;
	
	//caso de erro #2: o arquivo esta inconsistente (status 0)
	if(pagina[0] == '0'){
		printf("Falha no processamento do arquivo.");
		fclose(arquivoBin);
		return;
	}

	while(bytesLidos != 0){
		int offset = 0;			//armazena o byte offset, que sera atualizado a cada copia de dados para a struct
		
		bytesLidos = fread(pagina, 32000, 1, arquivoBin);

		//caso de erro #3: nao ha registros
		if(bytesLidos == 0 && paginasAcessadas == 1){
			printf("Registro inexistente.");
			fclose(arquivoBin);
			return;
		}
		
		if(pagina[0] == '-' || pagina[0] == '*'){
			while(offset < 32000){					//o ultimo byte da pagina esta no offset 32000 - 1 (ou seja, pagina[31999])
				Dados d;

				//copia dos bytes da pagina de disco para a struct do registro de dados
				if(pagina[offset] == '-'){
					d = copiaRegistro(pagina, &offset);

					printDados2(d);		//imprime o registro de dados
				}
				else if(pagina[offset] == '*'){
					int tamanho;
					offset += 1;
					memcpy(&tamanho, &pagina[offset], 4);
					offset += tamanho + 4;
					//printf("%x", pagina[offset]);
				}					

				if(pagina[offset] != '-' && pagina[offset] != '*'){
					/*os chars '-' e '*' marcam o inicio de um registro de dados.
					assim, a nao existencia desses chars logo apos o fim do registro anterior indica que nao existem
					mais registros, marcando assim o fim da pagina de disco.*/
					paginasAcessadas++;
					offset = 32000;
				}

				if(pagina[offset] == '-'){
					if(d.tamNomeServidor != -1)
						free(d.nomeServidor);

					if(d.tamCargoServidor != -1)
						free(d.cargoServidor);
				}
			}
			
		}
	}

	//imprime o numero de paginas de disco que foram acessadas apos a leitura dos registros
	printf("Número de páginas de disco acessadas: %d", paginasAcessadas);

	fclose(arquivoBin);

}