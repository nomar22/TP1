#ifndef ARVOREB_H_INCLUDED
#define ARVOREB_H_INCLUDED

#define FALSE 0
#define TRUE  1

typedef int Chave;

typedef struct Registro *LinkRegistro;

typedef struct Registro
{
    Chave Chave;
    /*outros componentes*/
} Registro;

typedef struct Pagina *LinkPagina;


typedef struct Pagina
{
    short n;
    Registro *r;
    LinkPagina *p;
} Pagina;


void InicializaArvoreB(LinkPagina *Dicionario);

LinkPagina inicializa(int ordem);

void pesquisa(LinkRegistro x, LinkPagina Ap, FILE *file);

void imprimePagina(LinkPagina ap,FILE *file);

void Imprime(LinkPagina p);

void InsereNaPagina(LinkPagina Ap,Registro Reg, LinkPagina ApDir);

int estaNaPagina(LinkRegistro x, LinkPagina Ap, int ordem);

void Ins(Registro Reg, LinkPagina Ap, short *Cresceu,Registro *RegRetorno,  LinkPagina *ApRetorno, int ordem);

void Insere(Registro Reg, LinkPagina *Ap, int ordem);

void Reconstitui(LinkPagina ApPag, LinkPagina ApPai,int PosPai, short *Diminuiu, int ordem);

void Antecessor(LinkPagina Ap, int Ind,LinkPagina ApPai, short *Diminuiu, int ordem);

void Ret(Chave Ch, LinkPagina *Ap, short *Diminuiu, int ordem);

void Retira(Chave Ch, LinkPagina *Ap, int ordem);

void ImprimeI(LinkPagina p, int nivel);

void TestaI(LinkPagina p, int pai, short direita);

void Testa(LinkPagina p);

#endif // ARVOREB_H_INCLUDED
