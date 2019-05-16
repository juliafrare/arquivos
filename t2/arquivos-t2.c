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

//Aluno: Julia Carolina Frare Peixoto
//nUSP: 10734727

//main
int main(){
	int funcionalidade;
	char nomeArquivo[100];
	FILE *arquivoLido, *arquivoBin;

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
			scanf("%s", nomeArquivo);
			buscaArquivoBin(nomeArquivo);
			break;
        case 4:
			scanf("%s", nomeArquivo);
			removeRegistro(nomeArquivo);
			break;

        case 5:
			scanf("%s", nomeArquivo);
			insereRegistro(nomeArquivo);
			break;

        case 6:
			scanf("%s", nomeArquivo);
			atualizaRegistro(nomeArquivo);
			break;
	
		default:
			break;
	}

	return 0;
}