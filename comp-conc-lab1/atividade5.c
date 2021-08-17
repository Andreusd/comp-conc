#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define TAM 10000
#define NTHREADS 2

int vetordm[TAM];

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, inicio, fim;
} t_Args;

//funcao executada pelas threads
void *preencheVetor (void *arg) {
  t_Args *args = (t_Args *) arg;

  printf("Sou a thread %d e vou preencher de %d ate %d!\n", args->idThread, args->inicio, args->fim);

  for(int i=args->inicio;i<args->fim;i++) {
    vetordm[i]++;
  }


  free(arg); //aqui pode liberar a alocacao feita na main

  pthread_exit(NULL);
}

//funcao principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS];
  t_Args *arg; //receberá os argumentos para a thread

  //inicializa vetor
  for(int i=0;i<TAM;i++) {
    vetordm[i]=0;
  }

  //thread 1
  printf("--Aloca e preenche argumentos para thread 1\n");
  arg = malloc(sizeof(t_Args));
  if (arg == NULL) {
    printf("--ERRO: malloc()\n"); exit(-1);
  }
  arg->idThread = 1;
  arg->inicio=0;
  arg->fim=TAM/2;
  printf("--Cria a thread 1\n");
  if (pthread_create(&tid_sistema[0], NULL, preencheVetor, (void*) arg)) {
    printf("--ERRO: pthread_create()\n"); exit(-1);
  }
  //thread 2
  printf("--Aloca e preenche argumentos para thread 2\n");
  arg = malloc(sizeof(t_Args));
  if (arg == NULL) {
    printf("--ERRO: malloc()\n"); exit(-1);
  }
  arg->idThread=2;
  arg->inicio=TAM/2;
  arg->fim=TAM;
  printf("--Cria a thread 2\n");
  if (pthread_create(&tid_sistema[1], NULL, preencheVetor, (void*) arg)) {
    printf("--ERRO: pthread_create()\n"); exit(-1);
  }

  //--espera todas as threads terminarem
  for (int i=0; i<NTHREADS; i++) {
    if (pthread_join(tid_sistema[i], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }

  //verificar se os valores finais estao corretos
  for(int i=0;i<TAM;i++) {
    if(vetordm[i]!=1) {
      printf("algo nao esta certo!\n");
      exit(1);
    }
  }
  printf("vetor preenchido com sucesso!\n");


  printf("--Thread principal terminou\n");
  pthread_exit(NULL);
}

