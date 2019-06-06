#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcionalidade1.h"
#include "funcionalidade2.h"
#include "funcionalidade3.h"
#include "funcionalidade4.h"
#include "funcionalidade5.h"
#include "funcionalidade6.h"
#include "funcionalidade7.h"
#include "funcionalidade8.h"
#include "funcionalidade9.h"
#include "funcionalidade10.h"
#include "funcionalidade11.h"
#include "funcionalidade12.h"
#include "funcionalidade13.h"
#include "funcionalidade14.h"
#include "escreverTela2.h"

//Aluno: Julia Carolina Frare Peixoto
//nUSP: 10734727

//main
int main(){
	int funcionalidade;
	char nomeArquivo[100], nomeArquivo2[100];
	char campo[20], *valor;
	FILE *arquivoLido, *arquivoBin;
	int numRegistros;
	int init;

	scanf("%d", &funcionalidade);

	switch (funcionalidade){
		case 1:
			scanf("%s", nomeArquivo);
			criaArquivoBin(nomeArquivo);
			break;
		case 2:
			scanf("%s", nomeArquivo);
			leArquivoBin(nomeArquivo);
			break;
		case 3:
			//obtencao das entradas
			scanf("%s", nomeArquivo);
			scanf("%s ", campo);
    		valor = readLine();

			buscaArquivoBin(nomeArquivo, campo, valor);

			free(valor);
			break;
        case 4:
			scanf("%s", nomeArquivo);
			init = removeRegistro(nomeArquivo);
			if(init == 0)
				binarioNaTela2("arquivo-novo.bin");
			break;

        case 5:
			scanf("%s", nomeArquivo);
			scanf("%d", &numRegistros);
			if(insereRegistro(nomeArquivo, &numRegistros) != NULL)
				binarioNaTela2("arquivo-novo.bin");
			break;

        case 6:
			scanf("%s", nomeArquivo);
			atualizaRegistro(nomeArquivo);
			break;
		
		case 7:
			scanf("%s", nomeArquivo);
			ordenaArquivo(nomeArquivo);
			break;
		
		case 8:
			scanf("%s", nomeArquivo);
			scanf("%s", nomeArquivo2);
			mergeArquivos(nomeArquivo, nomeArquivo2);
			break;
		
		case 9:
			scanf("%s", nomeArquivo);
			scanf("%s", nomeArquivo2);
			matchArquivos(nomeArquivo, nomeArquivo2);
			break;

		case 10:
			scanf("%s", nomeArquivo);
			scanf("%s", nomeArquivo2);
			init = funcionalidade10(nomeArquivo, nomeArquivo2);
			if(init == 0)
				binarioNaTela2(nomeArquivo2);
			break;

		case 11:
			//obtencao das entradas
			scanf("%s", nomeArquivo);
			scanf("%s", nomeArquivo2);
			scanf("%s ", campo);
    		valor = readLine();

			funcionalidade11(nomeArquivo, nomeArquivo2, campo, valor);

			free(valor);
			break;

		case 12:
			scanf("%s", nomeArquivo);
			scanf("%s", nomeArquivo2);
			funcionalidade12(nomeArquivo, nomeArquivo2);
			break;
		
		case 13:
			scanf("%s", nomeArquivo);
			scanf("%s", nomeArquivo2);
			funcionalidade13(nomeArquivo, nomeArquivo2);
			break;

		case 14:
			//obtencao das entradas
			scanf("%s", nomeArquivo);
			scanf("%s", nomeArquivo2);
			scanf("%s ", campo);
    		valor = readLine();

			funcionalidade14(nomeArquivo, nomeArquivo2, campo, valor);

			free(valor);
			break;
	
		default:
			break;
	}

	return 0;
}