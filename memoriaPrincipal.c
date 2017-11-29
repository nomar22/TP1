/*
* Tipo Abstrato de Dados de Lista
*
* Contém funções de manipulação (criação, inserção, remoção e liberação de memória), e também de impressão e cópia.
*
*/

#include <stdlib.h>
#include <stdio.h>
#include "memoriaPrincipal.h"


void criaMemoria(LinkMemoria memoria, int tipo, int tam,int ordem){
	tam = calculaNumPaginas(tam, ordem);
	memoria->primeiro = NULL;
    memoria -> len=0;
    memoria -> pageMiss = 0;
    memoria ->maximo =tam;
    memoria->tipo = tipo;
    memoria->ultimo = memoria->primeiro;
}


int calculaNumPaginas(int tam, int ordem){
	int retorno = 2*(2*ordem*sizeof(int)) + 2*sizeof(int);

	return tam/retorno;
}
int vazia(Memoria memoria){
    return (memoria.len == 0);
}

void inserePrimeiro(LinkPagina pagina, LinkMemoria memoria){

	LinkMoldura primeiro  = (LinkMoldura) malloc(sizeof(Moldura));
	primeiro->anterior = NULL;
	primeiro->prox= memoria -> primeiro;

	if(memoria->len == 0){
		memoria ->ultimo = primeiro;
	}
	else{
		memoria->primeiro->anterior = primeiro;
	}
	memoria->primeiro = primeiro;
	memoria->primeiro->pagina = pagina;
    memoria -> len++;
}

void insereUltimo(LinkPagina pagina, LinkMemoria memoria){

	LinkMoldura ultimo = (LinkMoldura) malloc(sizeof(Moldura));
	ultimo -> prox = NULL;
	ultimo -> anterior = memoria -> ultimo;

	if(memoria->len == 0){
		memoria->primeiro = ultimo;
	}
	else{
		memoria->ultimo->prox = ultimo;
	}
	memoria->ultimo = ultimo;
	memoria->ultimo->pagina = pagina;
	memoria -> len ++;

}
int estaNoTopo(LinkMoldura moldura, LinkMemoria memoria){
	if(moldura->anterior == NULL)
		return TRUE;
	return FALSE;
}
int estaNoFundo(LinkMoldura moldura, LinkMemoria memoria){
	if(moldura->prox == NULL)
		return TRUE;
	return FALSE;
}
void sobeNaPilha(LinkMoldura vaiSubir, LinkMemoria memoria){
	if(!estaNoTopo(vaiSubir,memoria)){
		LinkMoldura anterior, proximo;
		anterior = vaiSubir->anterior;
		proximo = vaiSubir ->prox;

		anterior->prox = proximo;
		if(!estaNoFundo(vaiSubir,memoria)){
			proximo ->anterior = anterior;
		}
		else{
			memoria ->ultimo = anterior;

		}
		vaiSubir ->anterior = NULL;
		vaiSubir -> prox = memoria->primeiro;

		memoria->primeiro->anterior = vaiSubir;
		memoria->primeiro = vaiSubir;
	}
}
void removePrimeiro(LinkMemoria memoria){

    // Se for o único elemento da lista
    if (memoria->len==0){
//    	liberaMemoria(memoria);
        return;
    }
    else{
        LinkMoldura primeiro = memoria ->primeiro ;
        memoria -> primeiro = primeiro->prox;
        memoria -> primeiro ->anterior = NULL;
        memoria -> len--;
        free(primeiro);
    }
}
void removeUltimo(LinkMemoria memoria){
	if(memoria ->len == 0){

	}
	else{
		LinkMoldura ultimo = memoria->ultimo;
		memoria ->ultimo = ultimo->anterior;
		memoria ->ultimo ->prox = NULL;
		memoria->len --;
		free (ultimo);
	}
}


int getTamanhoMemoria(LinkMemoria lista){
	return lista->len;
}

void retira(LinkMoldura moldura, LinkMemoria memoria){

	moldura->anterior->prox = moldura->prox;
	moldura->prox->anterior = moldura->anterior;
	memoria -> len --;
	free(moldura);

}



void inserePagina(LinkPagina pagina,LinkMemoria memoria){
	if (memoria->tipo == FIFO){

		if(memoria->len < memoria->maximo){
			inserePrimeiro(pagina,memoria);
		}
		else{
			removePagina(pagina,FIFO,memoria);
			inserePrimeiro(pagina,memoria);
		}
	}
	else if(memoria->tipo==LRU){

		if(memoria->len < memoria->maximo){
			inserePrimeiro(pagina, memoria);
		}
		else{
			removePagina(pagina,LRU,memoria);
			inserePrimeiro(pagina,memoria);
		}

	}
	else if (memoria->tipo ==LFU){

		if(memoria->len < memoria->maximo){
			inserePrimeiro(pagina,memoria);
		}
		else{
			removePagina(pagina,LFU,memoria);
			inserePrimeiro(pagina,memoria);
		}
	}
	else{
		printf("Deve ser selecionado um algoritmo valido de reposicao de paginas");
	}

}

void retiraMenosUsado(LinkMemoria memoria){
	LinkMoldura rmv = memoria->ultimo;
	LinkMoldura aux ;
	aux = rmv->anterior;
	int i;
	//Caso o primeiro seja 0, este já será eliminado
	if(rmv->hits > 0){
		//Quando encontrar o primeiro com 0 hits ou encontrar o menor
		//que este será o eliminado
		for(i = 0 ; i< memoria->len -1 && rmv->hits >0 ; i++){
			if(aux->hits < rmv->hits ){
				rmv = aux;
			}
			aux = aux->anterior;
		}
	}
	if(estaNoTopo(rmv,memoria)){
		removePrimeiro(memoria);
	}
	else if(estaNoFundo(rmv,memoria)){
		removeUltimo(memoria);
	}
	else{
		retira(rmv,memoria);
	}
}

void removePagina(LinkPagina pagina,int tipo, LinkMemoria memoria){
	if (tipo == FIFO){
		removeUltimo(memoria);
	}
	else if(tipo ==LRU){
		removeUltimo(memoria);
	}
	else if (tipo ==LFU){
		retiraMenosUsado(memoria);
	}
	else{
		printf("Escolha uma politica de remocao");
	}

}

//Reorganiza ou contabiliza de acordo com a política de reposição'
void acessaMemoria(LinkMoldura moldura,LinkMemoria memoria){

	if (memoria->tipo == FIFO){
		//Do nothing
	}
	else if(memoria->tipo ==LRU){
		sobeNaPilha(moldura,memoria);
	}
	else if (memoria ->tipo ==LFU){
		moldura ->hits++;
	}
	else{
		printf("Escolha uma politica ");
	}


}

LinkMoldura paginaNaMemoria(LinkPagina ap,LinkMemoria memoria){
	LinkMoldura retorno = NULL;
	int i = 0;
	retorno = memoria->primeiro;

	for(i=0;i<memoria->len;i++){
		if(retorno->pagina == ap){
			return retorno;
		}
		retorno = retorno->prox;
	}



	return NULL;

}
int registroNaMemoria(Registro x, LinkMemoria memoria, int ordem){
	int i =0;
	LinkMoldura retorno = NULL;
	retorno = memoria->primeiro;
	for(i=0;i<memoria->len;i++){
		if(estaNaPagina(&x,retorno->pagina, ordem)){
			acessaMemoria(retorno,memoria);
			return TRUE;
		}
		retorno = retorno->prox;
	}
	return FALSE;

}


void pesquisaMemoriaSecundaria(LinkRegistro x, LinkPagina Ap, LinkMemoria memoria ){
	long i = 1;

	LinkMoldura moldura = paginaNaMemoria(Ap,memoria);

	if( moldura == NULL){
		memoria->pageMiss ++;
		inserePagina(Ap,memoria);
	}
	else{
		acessaMemoria(moldura,memoria);
	}
	if (Ap == NULL)
	{
		printf("TipoRegistro nao esta presente na arvore\n");
		return;
	}
	//Caminhando nos registros  ordenados para encontrar sub-árvore
	while (i < Ap->n && x->Chave > Ap->r[i-1].Chave) i++;
	//Caso encontre o registro finaliza
	if (x->Chave == Ap->r[i-1].Chave){
		*x = Ap->r[i-1];
		return;
	}

	if (x->Chave < Ap->r[i-1].Chave)
		pesquisaMemoriaSecundaria(x, Ap->p[i-1],memoria);
	else pesquisaMemoriaSecundaria(x, Ap->p[i],memoria);
}


