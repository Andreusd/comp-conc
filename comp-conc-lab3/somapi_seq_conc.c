// Andre Uziel de Souza Dantas 119051475 CC 2019.1 computacao concorrente 2020.2

// importante: compilar com -lm!!!

// comando sugerido: gcc somapi_seq_conc.c -o somapi_seq_conc -lm -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

long long int N; // numero de elementos
int nthreads; //numero de threads

void * tarefa(void * arg) {
   long int id = (long int) arg; //identificador da thread
   double *somaLocal; //variavel local da soma de elementos
   somaLocal = (double*) malloc(sizeof(double));
   if(somaLocal==NULL) {exit(1);}
   long int tamBloco = N/nthreads;  //tamanho do bloco de cada thread 
   long int ini = id * tamBloco; //elemento inicial do bloco da thread
   long int fim; //elemento final(nao processado) do bloco da thread
   if(id == nthreads-1) fim = N;
   else fim = ini + tamBloco; //trata o resto se houver
   //soma os elementos do bloco da thread
   for(long int i=ini; i<fim; i++)
      *somaLocal += pow(-1,i)*1.0/(1+2*i);
   //retorna o resultado da soma local
   pthread_exit((void *) somaLocal); 
}

int main(int argc, char * argv[]) {
    if(argc<3) {
        printf("digite %s <no de elementos> <no de threads>\n",argv[0]);
        return 1;
    }
    double inicio,fim; // variaveis para medir o tempo
    pthread_t *tid; //identificadores das threads no sistema
    double *retorno; //valor de retorno das threads
    double somaSeq=0, somaConc=0, piSeq, piConc; //valores da soma e de pi(=soma*4)
    double tempoSeq, tempoConc;

    N = atoll(argv[1]);
    nthreads = atoll(argv[2]);

   //soma sequencial dos elementos==========
    GET_TIME(inicio);
    for(int i=0;i<N;i++) {
        somaSeq += pow(-1,i)*1.0/(1+2*i);
    }
    piSeq = 4*somaSeq;
    GET_TIME(fim);
    //======================================
    tempoSeq=fim-inicio;
    //soma concorrente dos elementos===============================
    GET_TIME(inicio);

    //cria os identificadores
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid==NULL) {
        fprintf(stderr, "ERRO--malloc\n");
        return 2;
    }
    //criar as threads
    for(long int i=0; i<nthreads; i++) {
        if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
    //aguardar o termino das threads
    for(long int i=0; i<nthreads; i++) {
        if(pthread_join(*(tid+i), (void**) &retorno)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
        //soma global
        somaConc += *retorno;
    }
    piConc = 4*somaConc;
    GET_TIME(fim);
    //==============================================================
    tempoConc = fim-inicio;
    //exibir os resultados
    printf("Tempo sequencial:  %lf\n",tempoSeq);
    printf("Tempo concorrente: %lf\n",tempoConc);
    if(tempoSeq<=tempoConc) {
        printf("Algoritmo sequencial foi mais rapido com uma diferença de %lf ms\n",(tempoConc-tempoSeq));
    }
    else {
        printf("Algoritmo concorrente foi mais rapido com uma diferença de %lf ms\n",(tempoSeq-tempoConc));
    }

    printf("PI sequencial:   %.15lf\n",piSeq);
    printf("PI concorrente:  %.15lf\n",piConc);
    printf("PI math.h:       %.15lf\n",M_PI);

    if(M_PI-piSeq<M_PI-piConc) {
        printf("PI sequencial ficou mais perto de PI. Diferença %.15lf\n",(M_PI-piSeq));
    }
    else {
        printf("PI concorrente ficou mais perto de PI math.h. Diferença %.15lf\n",(M_PI-piConc));
    }
    double diferenca = piSeq-piConc;
    printf("A diferença do entre o PI sequencial e o concorrente foi de %.15lf\n",(diferenca>=0 ? diferenca : -diferenca));

    free(tid);

    return 0;
}