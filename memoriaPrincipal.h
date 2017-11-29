#ifndef _TADMEMORIA_H_
#define _TADMEMORIA_H_

#define FIFO 0
#define LFU 1
#define LRU 2
/*
* Tipo Abstrato de Dados Memoria
*
* Contém funções de manipulação (criação, inserção, remoção e liberação de memória), e também de impressão e cópia.
*
*/

#include <stdlib.h>
#include <stdio.h>
#include "arvoreb.h"



typedef struct Moldura *LinkMoldura;

typedef struct Moldura {
	LinkPagina pagina;
	int hits;

	LinkMoldura anterior;
	LinkMoldura prox;
} Moldura;


typedef struct {
  LinkMoldura primeiro;
  LinkMoldura ultimo;
  int len;
  int tipo;
  int maximo;
  int pageMiss;

} Memoria;

typedef  Memoria *LinkMemoria;


void inserePagina(LinkPagina pagina,LinkMemoria memoria);

int calculaNumPaginas(int tam, int ordem);

void removePagina(LinkPagina pagina,int tipo, LinkMemoria memoria);

void criaMemoria(LinkMemoria lista, int tipo, int tam, int ordem);

int vazia(Memoria lista);

void inserePrimeiro(LinkPagina pagina, LinkMemoria memoria);

void removePrimeiro(LinkMemoria lista);

void liberaMemoria(LinkMemoria lista);

void imprimeMemoria(Memoria lista);

void imprimeReverso(Memoria lista);

int getTamanhoMemoria(LinkMemoria lista);

void pesquisaMemoriaSecundaria(Registro *x, LinkPagina Ap, LinkMemoria memoria );

int registroNaMemoria(Registro x, LinkMemoria memoria, int ordem);




#endif
