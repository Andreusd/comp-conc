/* Disciplina: Computacao Concorrente 2020.2 REMOTO*/
/* Aluno Andre Uziel 119051475 */
/* Lab 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/***** Condicao logica da aplicacao: A thread 4 deve inprimir antes das threads 2 e 3 e estas antes da thread 1  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread T1 */
void * T1(void *t) {
  pthread_mutex_lock(&x_mutex);

  while(x<3) {
    pthread_cond_wait(&x_cond, &x_mutex);
  }
  printf("Volte sempre!\n");

  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Thread T2 */
void * T2(void *t) {
  pthread_mutex_lock(&x_mutex);

  while(x<1) {
    pthread_cond_wait(&x_cond, &x_mutex);
  }
  x++;
  printf("Fique a vontade.\n");
  pthread_cond_broadcast(&x_cond);

  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Thread T3 */
void * T3(void *t) {
  pthread_mutex_lock(&x_mutex);

  while(x<1) {
    pthread_cond_wait(&x_cond, &x_mutex);
  }
  x++;
  printf("Sente-se por favor\n");
  pthread_cond_broadcast(&x_cond);

  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Thread T4 */
void * T4(void *t) {
  printf("Seja bem vindo!\n");
  pthread_mutex_lock(&x_mutex);

  x++;
  pthread_cond_broadcast(&x_cond);

  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[3], NULL, T4, NULL);
  pthread_create(&threads[2], NULL, T3, NULL);
  pthread_create(&threads[1], NULL, T2, NULL);
  pthread_create(&threads[0], NULL, T1, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}
