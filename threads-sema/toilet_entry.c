#include <semaphore.h>


int male_cnt = 0;
int female_cnt = 0;
sem_t empty;
sem_t mutex;

void Male(){

  sem_wait(&mutex);
  male_cnt++;
  if(male_cnt == 1) sem_wait(&empty);
  sem_post(&mutex);

  // do something

  sem_wait(&mutex);
  male_cnt--;
  if(male_cnt == 0) sem_post(&empty);
  sem_post(&mutex);
}

void Female(){

  sem_wait(&mutex);
  female_cnt++;
  if(female_cnt== 1) sem_wait(&empty);
  sem_post(&mutex);

  // do something

  sem_wait(&mutex);
  female_cnt--;
  if(female_cnt == 0) sem_post(&empty);
  sem_post(&mutex);
}

int main(){
  // semaphore initialize
  sem_init(&empty, 0, 1);
  sem_init(&mutex, 0, 1);


  // validation : male_cnt && female_cnt != 0 i.e. male and female exists simultatneously

}