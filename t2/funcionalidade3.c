#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade3.h"

//funcionalidade 3
void buscaArquivoBin(char *nomeArquivo){
	FILE* arquivoBin;			//ponteiro para o arquivo .bin	
	char pagina[32000];			//array da pagina de disco
	char nomeDoCampo[100];		//string do nome do campo que sera buscado (exemplo: cargoServidor)
	char *valor;				//string do valor que sera buscado (exemplo: ENGENHEIRO)
	int bytesLidos, paginasAcessadas = 0;	//bytesLidos sera o retorno do fread(bytes lidos com sucesso)
	int i, achou = 0;		//a variavel achou indica se foi encontrado algum registro equivalente (0 = registro nao encontrado, 1 = registro encontrado)
	Cabecalho c;

	//obtencao das entradas nomeDoCampo e valor
	scanf("%s ", nomeDoCampo);
	valor = readLine();

	arquivoBin = fopen(nomeArquivo, "rb");

	//caso de erro #1: nao existe um arquivo com o nome indicado pelo usuario
	if(arquivoBin == NULL){
		printf("Falha no processamento do arquivo.");
		free(valor);
		return;
	}

	//leitura da pagina do registro de cabecalho
	bytesLidos = fread(pagina, 32000, 1, arquivoBin);
	paginasAcessadas++;
	
	//caso de erro #2: o arquivo esta inconsistente (status 0)
	if(pagina[0] == '0'){
		printf("Falha no processamento do arquivo.");
		fclose(arquivoBin);
		free(valor);
		return;
	}

	//copia dos dados da pagina do cabecalho para a struct
	memcpy(&c.status, &pagina[0], 1);
	memcpy(&c.topoLista, &pagina[1], 8);
	memcpy(&c.tagCampo1, &pagina[9], 1);
	memcpy(&c.desCampo1, &pagina[10], 40);
	for(i = 0; i < sizeof(c.desCampo1); i++){
		if(c.desCampo1[i] == '@'){
			c.desCampo1[i] = '\0';
		}
	}
	memcpy(&c.tagCampo2, &pagina[50], 1);
	memcpy(&c.desCampo2, &pagina[51], 40);
	for(i = 0; i < sizeof(c.desCampo2); i++){
		if(c.desCampo2[i] == '@'){
			c.desCampo2[i] = '\0';
		}
	}
	memcpy(&c.tagCampo3, &pagina[91], 1);
	memcpy(&c.desCampo3, &pagina[92], 40);
	for(i = 0; i < sizeof(c.desCampo3); i++){
		if(c.desCampo3[i] == '@'){
			c.desCampo3[i] = '\0';
		}
	}
	memcpy(&c.tagCampo4, &pagina[132], 1);
	memcpy(&c.desCampo4, &pagina[133], 40);
	for(i = 0; i < sizeof(c.desCampo4); i++){
		if(c.desCampo4[i] == '@'){
			c.desCampo4[i] = '\0';
		}
	}
	memcpy(&c.tagCampo5, &pagina[173], 1);
	memcpy(&c.desCampo5, &pagina[174], 40);
	for(i = 0; i < sizeof(c.desCampo5); i++){
		if(c.desCampo5[i] == '@'){
			c.desCampo5[i] = '\0';
		}
	}
	//fim da copia do registro de cabecalho

	//busca pelos registros
	while(bytesLidos != 0){		//bytesLidos = 0 indica o final do arquivo
		int offset = 0;			//armazena o byte offset, que sera atualizado a cada copia de dados para a struct
		
		bytesLidos = fread(pagina, 32000, 1, arquivoBin);

		//caso de erro #3: nao ha registros
		if(bytesLidos == 0 && paginasAcessadas == 1){
			printf("Registro inexistente.");
			fclose(arquivoBin);
			free(valor);
			return;
		}
		
		if(pagina[0] == '-' || pagina[0] == '*'){
			while(offset < 32000){			//o ultimo byte da pagina esta no offset 32000 - 1 (ou seja, pagina[31999])
				Dados d;

				//copia dos bytes da pagina de disco para a struct do registro de dados
				if(pagina[offset] == '-'){
					d = copiaRegistro(pagina, &offset);

					//imprimir o registro de dados compativeis com a pesquisa
					if(strcmp(nomeDoCampo, "idServidor") == 0){
						if(atoi(valor) == d.idServidor){
							achou = 1;
							printDados3(c, d);
							paginasAcessadas++;
							printf("Número de páginas de disco acessadas: %d", paginasAcessadas);
							return;							
						}
					}
					else if(strcmp(nomeDoCampo, "salarioServidor") == 0){
						if(atof(valor) == d.salarioServidor){
							achou = 1;
							printDados3(c, d);
						}
					}
					else if(strcmp(nomeDoCampo, "telefoneServidor") == 0){
						if(strcmp(valor, d.telefoneServidor) == 0){
							achou = 1;
							printDados3(c, d);
						}
					}
					else if(strcmp(nomeDoCampo, "nomeServidor") == 0){
						if(strcmp(valor, d.nomeServidor) == 0){
							achou = 1;
							printDados3(c, d);
						}
					}
					else if(strcmp(nomeDoCampo, "cargoServidor") == 0){
						if(strcmp(valor, d.cargoServidor) == 0){
							achou = 1;
							printDados3(c, d);
						}
					}
				}
				else if(pagina[offset] == '*'){
					int tamanho;
					offset += 1;
					memcpy(&tamanho, &pagina[offset], 4);
					offset += tamanho + 4;
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
	

	if(achou == 1)
		printf("Número de páginas de disco acessadas: %d", paginasAcessadas);
	else
		printf("Registro inexistente.");

	free(valor);
	fclose(arquivoBin);
}