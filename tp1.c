#include <stdio.h>
#include <stdlib.h>


#include "arvoreb.h"
#include "memoriaPrincipal.h"


int main(int argc, char *argv[]){

    Registro x;
    LinkPagina  arvoreb;
    int ordem,tamBytes,numChaves,
    numInstancias,count,i;
    char *ArquivoInput = argv[1];  //Primeiro Parametro:Arquivo de Entrada.
    char *ArquivoOutput = argv[2]; //Segundo Parametro:Arquivo de Saída.
    Memoria fifo, lru,lfu;



	FILE *arquivoSaida;
	arquivoSaida = fopen(ArquivoOutput,"w"); //Abre arquivo para escrever
	FILE *fp;
	fp = fopen(ArquivoInput,"r"); //Abre arquivo para escrever
	if(!fp){
		printf("Erro na leitura de entrada!");
		return 0;
	}
	fscanf (fp,"%d\n",&numInstancias);

    for(count=0;count<numInstancias;count++){
    	fscanf (fp,"%d %d\n",&tamBytes,&ordem);

    	arvoreb = inicializa(ordem);
    	criaMemoria(&fifo,FIFO,tamBytes,ordem);
    	criaMemoria(&lfu,LFU,tamBytes,ordem);
    	criaMemoria(&lru,LRU,tamBytes,ordem);



    	fscanf (fp,"%d\n",&numChaves);
    	for(i=0;i<numChaves;i++){
			fscanf(fp,"%d ", &x.Chave);
			Insere(x, &arvoreb,ordem);
    	}
    	fscanf (fp,"%d \n",&numChaves);
    	for(i=0;i<numChaves;i++){
    		fscanf(fp,"%d ",&x.Chave);
    		Retira(x.Chave, &arvoreb,ordem);
    	}
		fscanf (fp,"%d \n",&numChaves);
    	fscanf(fp,"%d ",&x.Chave);
    	for(i=0;i<numChaves;i++) {
    		if(!registroNaMemoria(x,&fifo,ordem)){
				pesquisaMemoriaSecundaria(&x, arvoreb, &fifo);
    		}
    		if(!registroNaMemoria(x,&lru,ordem)){
    			pesquisaMemoriaSecundaria(&x, arvoreb, &lru);

    		}
    		if(!registroNaMemoria(x,&lfu,ordem)){
    			pesquisaMemoriaSecundaria(&x, arvoreb, &lfu);

    		}
        	fscanf(fp,"%d ",&x.Chave);
        }


    	fprintf(arquivoSaida,"%d ",fifo.pageMiss);
    	fprintf(arquivoSaida,"%d ",lru.pageMiss);
    	fprintf(arquivoSaida,"%d\n",lfu.pageMiss);
    	numChaves = x.Chave;
    	for (i=0;i<numChaves;i++ ){
    		fscanf(fp,"%d ",&x.Chave);
    		//Árvore
    		pesquisa(&x,arvoreb,arquivoSaida);
    		fprintf(arquivoSaida,"\n");

    	}
    	free(arvoreb);
    	arvoreb= NULL;
    }

	fclose(arquivoSaida);
    return 0;
}
