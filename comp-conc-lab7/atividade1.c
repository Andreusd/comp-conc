/* Disciplina: Computacao Concorrente 2020.2 REMOTO*/
/* Aluno Andre Uziel 119051475 */
/* Lab 7 */
/* Codigo: Uso de semaforos para exclusão mútua e sincronização por condição */

/***** Condicao logica da aplicacao: A thread 4 deve imprimir antes das threads 2 e 3 e estas antes da thread 1  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

/* Variaveis globais */
sem_t condt23, condt1;     //semaforos para sincronizar a ordem de execucao das threads

/* Thread T1 */
void * T1(void *t) {

  sem_wait(&condt1); //espera T2 (ou T3) executar
  sem_wait(&condt1); //espera T2 (ou T3) executar

  printf("Volte sempre!\n");

  pthread_exit(NULL);
}

/* Thread T2 */
void * T2(void *t) {

  sem_wait(&condt23); //espera T4 executar

  printf("Fique a vontade.\n");

  sem_post(&condt1); //permite que T1 execute

  pthread_exit(NULL);
}

/* Thread T3 */
void * T3(void *t) {

  sem_wait(&condt23); //espera T4 executar

  printf("Sente-se por favor\n");

  sem_post(&condt1); //permite que T1 execute

  pthread_exit(NULL);
}

/* Thread T4 */
void * T4(void *t) {

  printf("Seja bem vindo!\n");

  sem_post(&condt23); //permite que T2 (ou T3) execute
  sem_post(&condt23); //permite que T2 (ou T3) execute

  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  pthread_t tid[4];

  //inicia os semaforos
  sem_init(&condt23, 0, 0);
  sem_init(&condt1, 0, 0);

  //cria as tres threads
  if (pthread_create(&tid[0], NULL, T1, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[1], NULL, T2, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[2], NULL, T3, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[3], NULL, T4, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }

  //--espera todas as threads terminarem
  for (int i=0; i<4; i++) {
    if (pthread_join(tid[i], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  } 
  pthread_exit(NULL);
}
