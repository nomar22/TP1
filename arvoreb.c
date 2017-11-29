#include<stdlib.h>
#include<stdio.h>

#define FALSE 0
#define TRUE  1

#include "arvoreb.h"


void InicializaArvoreB(LinkPagina *Dicionario)
{
    *Dicionario = NULL;
}

LinkPagina inicializa(int ordem){
	LinkPagina pagina = malloc(sizeof(Pagina));
	pagina->n = 0;
	pagina->p = NULL;
	pagina->r = NULL;
	int m = 2*ordem+1;
	pagina->r =calloc(m,sizeof(Registro));
	pagina->p = calloc(m,sizeof(LinkPagina));
	return pagina;
}

void pesquisa(Registro *x, LinkPagina Ap, FILE *file)
{
    long i = 1;

    if(file != NULL){
		imprimePagina(Ap, file);
	}

    if (Ap == NULL)
    {
        printf("TipoRegistro nao esta presente na arvore\n");
        return;
    }
    while (i < Ap->n && x->Chave > Ap->r[i-1].Chave) i++;
    if (x->Chave == Ap->r[i-1].Chave)
    {
        *x = Ap->r[i-1];
        return;
    }
    if (x->Chave < Ap->r[i-1].Chave)
        pesquisa(x, Ap->p[i-1],file);
    else pesquisa(x, Ap->p[i],file);
}

void imprimePagina(LinkPagina ap,FILE *file){
	int i ;
	for(i=0;i<ap->n;i++){
		fprintf(file,"%d ", ap->r[i]);
	}

}

int estaNaPagina(LinkRegistro x, LinkPagina ap, int ordem){
	int inPage = FALSE;
	int i;
	for(i=0;i<ap->n;i++){
		if(x->Chave == ap->r[i].Chave){
			return TRUE;
		}
	}
	return inPage;
}

void InsereNaPagina(LinkPagina Ap,Registro Reg, LinkPagina ApDir)
{
    short NaoAchouPosicao;
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);
    while (NaoAchouPosicao)
    {
        if (Reg.Chave >= Ap->r[k-1].Chave)
        {
            NaoAchouPosicao = FALSE;
            break;
        }
        Ap->r[k] = Ap->r[k-1];
        Ap->p[k+1] = Ap->p[k];
        k--;
        if (k < 1) NaoAchouPosicao = FALSE;
    }
    Ap->r[k] = Reg;
    Ap->p[k+1] = ApDir;
    Ap->n++;
}

void Ins(Registro Reg, LinkPagina Ap, short *Cresceu,Registro *RegRetorno,  LinkPagina *ApRetorno,int ordem)
{
    long i = 1;
    long j;
    LinkPagina ApTemp;
    if (Ap == NULL)
    {
        *Cresceu = TRUE;
        (*RegRetorno) = Reg;
        (*ApRetorno) = NULL;
        return;
    }
    while (i < Ap->n && Reg.Chave > Ap->r[i-1].Chave)  i++;
    if (Reg.Chave == Ap->r[i-1].Chave)
    {
        printf(" Erro: Registro ja esta presente\n");
        *Cresceu = FALSE;
        return;
    }
    if (Reg.Chave < Ap->r[i-1].Chave) i--;
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno, ordem);
    if (!*Cresceu) return;
    if (Ap->n < 2 * ordem)   /* Pagina tem espaco */
    {
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = FALSE;
        return;
    }
    /* Overflow: Pagina tem que ser dividida */
    ApTemp = inicializa(ordem);
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;
    if (i < ordem + 1)
    {
        InsereNaPagina(ApTemp, Ap->r[2*ordem-1], Ap->p[2*ordem]);
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    }
    else InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
    for (j = ordem + 2; j <= 2*ordem; j++)
        InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j]);
    Ap->n = ordem; ApTemp->p[0] = Ap->p[ordem+1];
    *RegRetorno = Ap->r[ordem];
    *ApRetorno = ApTemp;
}

void Insere(Registro Reg, LinkPagina *Ap, int ordem)
{
    short Cresceu;
    Registro RegRetorno;
    Pagina *ApRetorno, *ApTemp;
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno, ordem);
    if (Cresceu)  /* Arvore cresce na altura pela raiz */
    {
        ApTemp = inicializa(ordem);
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap;
        *Ap = ApTemp;
    }
}

void Reconstitui(LinkPagina ApPag, LinkPagina ApPai,int PosPai, short *Diminuiu, int ordem)
{
    Pagina *Aux;
    long DispAux, j;
    if (PosPai < ApPai->n)  /* Aux = TipoPagina a direita de ApPag */
    {
        Aux = ApPai->p[PosPai+1];
        DispAux = (Aux->n - ordem + 1) / 2;
        ApPag->r[ApPag->n] = ApPai->r[PosPai];
        ApPag->p[ApPag->n + 1] = Aux->p[0];
        ApPag->n++;
        if (DispAux > 0)  /* Existe folga: transfere de Aux para ApPag */
        {
            for (j = 1; j < DispAux; j++)
                InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j]);
            ApPai->r[PosPai] = Aux->r[DispAux-1];
            Aux->n -= DispAux;
            for (j = 0; j < Aux->n; j++) Aux->r[j] = Aux->r[j + DispAux];
            for (j = 0; j <= Aux->n; j++) Aux->p[j] = Aux->p[j + DispAux];
            *Diminuiu = FALSE;
        }
        else /* Fusao: intercala Aux em ApPag e libera Aux */
        {
            for (j = 1; j <= ordem; j++)
                InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j]);
            free(Aux);
            for (j = PosPai + 1; j < ApPai->n; j++)
            {
                ApPai->r[j-1] = ApPai->r[j];
                ApPai->p[j] = ApPai->p[j+1];
            }
            ApPai->n--;
            if (ApPai->n >= ordem) *Diminuiu = FALSE;
        }
    }
    else /* Aux = TipoPagina a esquerda de ApPag */
    {
        Aux = ApPai->p[PosPai-1];
        DispAux = (Aux->n - ordem + 1) / 2;
        for (j = ApPag->n; j >= 1; j--) ApPag->r[j] = ApPag->r[j-1];
        ApPag->r[0] = ApPai->r[PosPai-1];
        for (j = ApPag->n; j >= 0; j--) ApPag->p[j+1] = ApPag->p[j];
        ApPag->n++;
        if (DispAux > 0) /* Existe folga: transf. de Aux para ApPag */
        {
            for (j = 1; j < DispAux; j++)
                InsereNaPagina(ApPag, Aux->r[Aux->n - j],
                               Aux->p[Aux->n - j + 1]);
            ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
            ApPai->r[PosPai-1] = Aux->r[Aux->n - DispAux];
            Aux->n -= DispAux;
            *Diminuiu = FALSE;
        }
        else /* Fusao: intercala ApPag em Aux e libera ApPag */
        {
            for (j = 1; j <= ordem; j++)
                InsereNaPagina(Aux, ApPag->r[j-1], ApPag->p[j]);
            free(ApPag);
            ApPai->n--;
            if (ApPai->n >= ordem)  *Diminuiu = FALSE;
        }
    }
}

void Antecessor(LinkPagina Ap, int Ind,LinkPagina ApPai, short *Diminuiu, int ordem)
{
    if (ApPai->p[ApPai->n] != NULL)
    {
        Antecessor(Ap, Ind, ApPai->p[ApPai->n], Diminuiu, ordem);
        if (*Diminuiu)
            Reconstitui(ApPai->p[ApPai->n], ApPai, (int)ApPai->n, Diminuiu, ordem);
        return;
    }
    Ap->r[Ind-1] = ApPai->r[ApPai->n - 1];
    ApPai->n--;
    *Diminuiu = (ApPai->n < ordem);
}

void Ret(Chave Ch, LinkPagina *Ap, short *Diminuiu, int ordem)
{
    long j, Ind = 1;
    LinkPagina Pag;
    if (*Ap == NULL)
    {
        printf("Erro: registro nao esta na arvore\n");
        *Diminuiu = FALSE;
        return;
    }
    Pag = *Ap;
    while (Ind < Pag->n && Ch > Pag->r[Ind-1].Chave) Ind++;
    if (Ch == Pag->r[Ind-1].Chave)
    {
        if (Pag->p[Ind-1] == NULL)   /* TipoPagina folha */
        {
            Pag->n--;
            *Diminuiu = (Pag->n < ordem);
            for (j = Ind; j <= Pag->n; j++)
            {
                Pag->r[j-1] = Pag->r[j];
                Pag->p[j] = Pag->p[j+1];
            }
            return;
        }
        /* TipoPagina nao e folha: trocar com antecessor */
        Antecessor(*Ap, Ind, Pag->p[Ind-1], Diminuiu, ordem);
        if (*Diminuiu)
            Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu, ordem);
        return;
    }
    if (Ch > Pag->r[Ind-1].Chave) Ind++;
    Ret(Ch, &Pag->p[Ind-1], Diminuiu,ordem);
    if (*Diminuiu) Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu,ordem);
}

void Retira(Chave Ch, LinkPagina *Ap, int ordem)
{
    short Diminuiu;
    LinkPagina Aux;
    Ret(Ch, Ap, &Diminuiu, ordem);
    if (Diminuiu && (*Ap)->n == 0)  /* Arvore diminui na altura */
    {
        Aux = *Ap;
        *Ap = Aux->p[0];
        free(Aux);
    }
}

void ImprimeI(LinkPagina p, int nivel)
{
    long i;
    if (p == NULL) return;
    printf("Nivel %d : ", nivel);
    for (i = 0; i < p->n; i++)
        printf("%d ",(int)p->r[i].Chave);
    putchar('\n');
    nivel++;
    for (i = 0; i <= p->n; i++)
        ImprimeI(p->p[i], nivel);
}

void Imprime(LinkPagina p)
{
    int  n = 0;
    ImprimeI(p, n);
}

void TestaI(LinkPagina p, int pai, short direita)
{
    int i;
    int antecessor = 0;
    if (p == NULL) return;
    if (p->r[0].Chave > pai && direita == FALSE)
    {
        printf("Erro: filho %12d maior que pai %d\n", p->r[0].Chave, pai);
        return;
    }
    for (i = 0; i < p->n; i++)
    {
        if (p->r[i].Chave <= antecessor)
        {
            printf("Erro: irmao %d maior que irmao a esquerda %d\n",
                   (int)p->r[i].Chave, antecessor);
            return;
        }
        antecessor = p->r[i].Chave;
    }
    for (i = 0; i < p->n; i++)
        TestaI(p->p[i], p->r[i].Chave, FALSE);
    TestaI(p->p[p->n], p->r[i].Chave, TRUE);
}

void Testa(LinkPagina p)
{
    int i;
    if (p == NULL) return;
    for (i = 0; i < p->n; i++)
        TestaI(p->p[i], p->r[i].Chave, FALSE);
    TestaI(p->p[p->n], p->r[i].Chave, TRUE);
}


