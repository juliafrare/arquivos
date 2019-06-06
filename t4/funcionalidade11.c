#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcoes_t4.h"
#include "funcionalidade11.h"

int funcionalidade11(char *nomeArquivo, char *nomeArquivo2, char *campo, char *valor){
    FILE* arquivoBin;			//ponteiro para o arquivo .bin
	FILE* arquivoIndice;
    char *paginaCab1, *paginaCab2;			//array da pagina de cabecalho
    //char campo[20], *valor;
    int paginasIndice = 0, paginasDados = 0;

    //printf("%s\n", nomeArquivo);
    //printf("%s\n", nomeArquivo2);

	//abertura do arquivo de entrada
    paginaCab1 = (char *) malloc(sizeof(char) * 32000);
	int init1, init2;
	init1 = inicializa(&arquivoBin, nomeArquivo, &paginaCab1);
	if(init1 == 1){
        free(paginaCab1);
		return paginasDados;
    }
    paginasDados++;

    //abertura do arquivo de indice
    paginaCab2 = (char *) malloc(sizeof(char) * 32000);
	init2 = inicializa(&arquivoIndice, nomeArquivo2, &paginaCab2);
	if(init2 == 1){
		fclose(arquivoBin);
		free(paginaCab1);
        free(paginaCab2);
		return paginasDados;
	}
    paginasIndice++;

    //obtenção dos metadados
    Cabecalho c;
    c = getCabBin(paginaCab1);

    //obtenção das entradas de campo e valor
    //scanf("%s ", campo);
    //valor = readLine();

    //declaração do vetor
    DadosT4 *regIndice;
	int nIndice = 0;
	regIndice = (DadosT4 *) malloc(sizeof(DadosT4) * 10000);

    //obtenção do vetor
    while(!feof(arquivoIndice)){
		int offset = 0;	//armazena o byte offset da pagina
		char *pagina;

        pagina = (char *) calloc(32000, sizeof(char));

		//obtencao da pagina de dados
		fread(pagina, 32000, 1, arquivoIndice);

        while(offset < 32000){

            if(pagina[offset] == '\0'){
                offset = 32000;
                break;
            }

            regIndice[nIndice] = getIndice(pagina, &offset);
            nIndice++;
        }

        paginasIndice++;

        free(pagina);
	}

    //busca pelos indices correspondentes a chave
    int arrIndex;
    int primeiro, ultimo;
    arrIndex = buscaBinIndice(regIndice, valor, 0, nIndice - 1);
    if(arrIndex != -1){
        int i = arrIndex, j = arrIndex;
        while(!strcmp(valor, regIndice[i].chaveBusca)){
            primeiro = i;
            i--;
        }
        while(!strcmp(valor, regIndice[j].chaveBusca)){
            ultimo = j;
            j++;
        }
    }
    else{
        printf("Registro inexistente.\n");
        return paginasDados;
    }

    int encontrados = ultimo - primeiro + 1;    //numero de registros encontrados
    int paginaReg[encontrados];                 //array que armazena o numero da pagina em que o registro esta encontrado
    int indexPagina[encontrados];               //array que armazena o offset do registro dentro da pagina

    //calculo das paginas onde os registros estao localizados e seus indices nas paginas
    for(int i = primeiro; i <= ultimo; i++){
        indexPagina[i - primeiro] = regIndice[i].byteOffset % 32000;
        paginaReg[i - primeiro] = (regIndice[i].byteOffset - indexPagina[i - primeiro]) / 32000;
    }

    //encontrando os registros
    for(int i = 0; i < encontrados; i++){
        if(paginaReg[i] != paginaReg[i - 1])
            paginasDados++;
        
        fseek(arquivoBin, paginaReg[i] * 32000, SEEK_SET);

        //obtenção da página
        char pagina[32000];
        fread(pagina, 32000, 1, arquivoBin);

        //obtenção do registro
        Dados d;
        d = copiaRegistro(pagina, &indexPagina[i]);

        printDados3(c, d);
    
        if(d.tamNomeServidor > 0)
            free(d.nomeServidor);
        if(d.tamCargoServidor > 0)
            free(d.cargoServidor);
    }

    printf("Número de páginas de disco para carregar o arquivo de índice: %d\n", paginasIndice);
    printf("Número de páginas de disco para acessar o arquivo de dados: %d\n", paginasDados);    

	fclose(arquivoBin);
    fclose(arquivoIndice);

    //free
    free(regIndice);
    free(paginaCab1);
    free(paginaCab2);

    return paginasDados;
}