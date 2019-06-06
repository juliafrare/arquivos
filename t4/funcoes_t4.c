#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcoes_t4.h"

void inicializaCabIndice(CabecalhoT4 *c){
    c->status = '0';
    c->nroRegistros = 0;
}

char *getPaginaCabIndice(CabecalhoT4 c){
    char *p;

    p = (char *) malloc(sizeof(char) * 32000);

    memcpy(&p[0], &c.status, 1);
    memcpy(&p[1], &c.nroRegistros, 4);

    for(int i = 5; i < 32000; i++)
        p[i] = '@';

    return p;
}

//copia um registro de indice para uma pagina de disco
char *getPaginaIndice(DadosT4 d, int *offsetPagina, char *p){
    memcpy(&p[*offsetPagina], d.chaveBusca, 120);
    *offsetPagina += 120;
    memcpy(&p[*offsetPagina], &d.byteOffset, 8);
    *offsetPagina += 8;

    return p;
}

//copia um registro de indice para outro
DadosT4 copiaRegIndice(DadosT4 d1){
    DadosT4 d2;

    //copia dos bytes da pagina de disco para a struct do registro de dados
	for(int i = 0; i < 120; i++)
    	d2.chaveBusca[i] = d1.chaveBusca[i];
    d2.byteOffset = d1.byteOffset;

    return d2;
}

DadosT4 getIndice(char *p, int *offset){
    DadosT4 d;

    memcpy(d.chaveBusca, &p[*offset], 120);
    *offset += 120;
    memcpy(&d.byteOffset, &p[*offset], 8);
    *offset += 8;

    return d;
}

//funções do mergesort
void mergeIndice(DadosT4 *d, int ini, int meio, int fim) { 
    int i, j, k;

    i = ini;
    j = meio + 1;
    k = 0;

    DadosT4 aux[fim];
  
    while(i <= meio && j <= fim){
        if(strcmp(d[i].chaveBusca, d[j].chaveBusca) < 0){
            aux[k] = copiaRegIndice(d[i]);
            i++;
        }
        else if(strcmp(d[i].chaveBusca, d[j].chaveBusca) == 0 && d[i].byteOffset < d[j].byteOffset){
            aux[k] = copiaRegIndice(d[i]);
            i++;
        }
        else{
            aux[k] = copiaRegIndice(d[j]);
            j++;
        }
        k++;
    }

	while (i <= meio) {
		aux[k] = copiaRegIndice(d[i]);
		i++;
		k++;
	}	

	while (j <= fim) {
		aux[k] = copiaRegIndice(d[j]);
		j++;
		k++;
	}

	for (i = ini, k = 0; i <= fim; i++, k++) {
		d[i] = aux[k];
	}
} 

void sortIndice(DadosT4 *d, int ini, int fim){ 
    if(ini < fim){ 
        int meio = (ini + fim) / 2; 

        sortIndice(d, ini, meio); 
        sortIndice(d, meio + 1, fim); 
        mergeIndice(d, ini, meio, fim);
    } 
}

int buscaBinIndice(DadosT4 *d, char *busca, int ini, int fim){

	if(ini > fim)
		return -1;

	int meio = (ini + fim) / 2;

	if(!strcmp(busca, d[meio].chaveBusca))
		return meio;

	if(strcmp(busca, d[meio].chaveBusca) < 0)
		return buscaBinIndice(d, busca, ini, meio - 1);

	if(strcmp(busca, d[meio].chaveBusca) > 0)
		return buscaBinIndice(d, busca, meio + 1, fim);
	
}