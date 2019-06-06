#include <stdio.h>
#include "funcionalidade3.h"
#include "funcionalidade11.h"
#include "funcionalidade14.h"

void funcionalidade14(char *nomeArquivo, char *nomeArquivo2, char *campo, char *valor){
    int paginasF3, paginasF11, diferenca;

    printf("*** Realizando a busca sem o auxílio de índice\n");
    paginasF3 = buscaArquivoBin(nomeArquivo, campo, valor);
    printf("*** Realizando a busca com o auxílio de um índice secundário fortemente ligado\n");
    paginasF11 = funcionalidade11(nomeArquivo, nomeArquivo2, campo, valor);

    if(paginasF3 < paginasF11)
        diferenca = paginasF11 - paginasF3;
    else
        diferenca = paginasF3 - paginasF11;
    

    printf("\nA diferença no número de páginas de disco acessadas: %d\n", diferenca);
}