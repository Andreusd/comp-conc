/* Disciplina: Computacao Concorrente 2020.2 REMOTO */
/* Aluno Andre Uziel 119051475 */
/* Prof.: Silvana Rossetto */
/* Codigo: Atividade 1 */

/***** Logica: Soma de prefixo concorrente ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Variaveis globais */
int bloqueadas = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
int nthreads;
int * g_vetor;

/* debug */
void imprime_vetor_debug(int * vetor) {
  for(int i=0;i<nthreads;i++) {
    printf("%d ",vetor[i]);
  }
  printf("\n");
}

/* funcao barreira ( copiada do leitesc.c, nao mudei nada ) */
void barreira(int nthreads) {
    pthread_mutex_lock(&x_mutex); /* inicio secao critica */
    if (bloqueadas == (nthreads-1)) { 
      /* ultima thread a chegar na barreira */
      pthread_cond_broadcast(&x_cond);
      bloqueadas=0;
    } else {
      bloqueadas++;
      pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); /* fim secao critica */
}

//funcao das threads
void *tarefa (void *arg) {
  int id = *(int*)arg;

  for (int salto = 1; nthreads > salto; salto *= 2) {

    //   printf("Thread %d: passo=%ld salto=%ld\n", id, passo, salto);

    /* define o auxiliar */
    int auxiliar = 0;
    if(id-salto >= 0)
      auxiliar = g_vetor[id-salto];

    /* sincronizacao condicional para garantir que todas leem antes de alguma delas alterar a sua posicao */
    barreira(nthreads);

    g_vetor[id] += auxiliar;

    /* sincronizacao condicional para garantir que todas escrevem antes de alguma delas ler na proxima iteração */
    barreira(nthreads);

  }
  pthread_exit(NULL);
}


/* Funcao principal */
int main(int argc, char *argv[]) {

  if(argc > 1) {
    nthreads = atoi(argv[1]);
    // if((nthreads & (nthreads - 1)) != 0) {
    //   fprintf(stderr,"numero precisa ser uma potencia de 2\n");
    //   return 1;
    // }
  }
  else {
    printf("digite %s <nthreads>\n", argv[0]);
    return 2;
  }

  g_vetor = (int*) malloc(sizeof(int) * nthreads);
  if(g_vetor==NULL) {
    fprintf(stderr,"erro malloc");
  }

  pthread_t threads[nthreads];
  int id[nthreads];
  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Inicializa o vetor */
  for(int i=0;i<nthreads;i++) {
    g_vetor[i]=i+1;
  }
  // imprime_vetor_debug(g_vetor);

  /* Cria as threads */
  for(int i=0;i<nthreads;i++) {
    id[i]=i;
    if(pthread_create(&threads[i], NULL, tarefa, (void *) &id[i]))
      fprintf(stderr,"erro pthread create");
  }

  /* Espera todas as threads completarem */
  for (int i = 0; i < nthreads; i++) {
    pthread_join(threads[i], NULL);
  }

  // printf ("FIM.\n");

  // imprime_vetor_debug(g_vetor);

  /* Desaloca variaveis */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);

  /* Cria vetor sequencial para comparacao */
  int l_vetor[nthreads];
  l_vetor[0] = 1;
  for(int i=1; i<nthreads; i++) {
    l_vetor[i] = i+1;
    l_vetor[i] = l_vetor[i] + l_vetor[i-1];
  }

  imprime_vetor_debug(g_vetor);
  imprime_vetor_debug(l_vetor);

  /* Comparacao */
  for(int i=0; i<nthreads; i++) {
    if(g_vetor[i] != l_vetor[i]) {
      fprintf(stderr,"Erro comparacao vetor concorrente e sequencial\n");
      return 404;
    }
  }

  printf("Os vetores concorrente e sequencial coincidem\n");

  return 0;
}
