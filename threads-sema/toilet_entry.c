#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int male_cnt = 0;
int female_cnt = 0;
sem_t empty;
sem_t mutex_m, mutex_f;

void do_something(){
  int n = 10000000;
  while(n--);
  return;
}

void Male(){
  sem_wait(&mutex_m);
  if(male_cnt == 0) sem_wait(&empty);
  male_cnt++;
  printf("male %d entry\n", male_cnt);
  sem_post(&mutex_m);

  do_something();

  sem_wait(&mutex_m);
  printf("male %d out\n", male_cnt);
  male_cnt--;
  if(male_cnt == 0) sem_post(&empty);
  sem_post(&mutex_m);
}

void Female(){
  sem_wait(&mutex_f);
  if(female_cnt== 0) sem_wait(&empty);
  female_cnt++;
  printf("female %d entry\n", female_cnt);
  sem_post(&mutex_f);

  do_something();

  sem_wait(&mutex_f);
  printf("female %d out\n", female_cnt);
  female_cnt--;
  if(female_cnt == 0) sem_post(&empty);
  sem_post(&mutex_f);
}

typedef struct {
  int n;
} myarg_t;

void *routine1(void *arg){
  myarg_t *args = (myarg_t *) arg;
  int n = args->n;
  while(n--){
    Male();
  }
  return NULL;
}

void *routine2(void *arg){
  myarg_t *args = (myarg_t *) arg;
  int n = args->n;
  while(n--){
    Female();
    Male();
  }
  return NULL;
}

int main(){
  // semaphore initialize
  sem_init(&empty, 0, 1);
  sem_init(&mutex_m, 0, 1);
  sem_init(&mutex_f, 0, 1);

  pthread_t *p1 = malloc(sizeof(pthread_t));
  pthread_t *p2 = malloc(sizeof(pthread_t));
  myarg_t arg1 = {5};
  myarg_t arg2 = {7};

  if(pthread_create(p1, NULL, routine1, &arg1) < 0){
    printf("error on p1");
    return 0;
  }
  if(pthread_create(p2, NULL, routine2, &arg2) < 0){
    printf("error on p2");
    return 0;
  }

  pthread_join(*p1, NULL);
  pthread_join(*p2, NULL);

  // TODO : validation => male_cnt && female_cnt != 0 i.e. male and female exists simultatneously

  return 0;
}