#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define RAND_HIGH 15

unsigned int numNumbers;

int randint(int low, int high) {
  int res =  rand() % (high - low) + low;
  return res;
}

void *writer(void *field) {
  int* numField = (int*)field;
  int i;
  for (i = 0; i < numNumbers; i++) {
    numField[i] = randint(0, RAND_HIGH);
  }
  printf("Thread %lu, array: [", pthread_self());
  for (i = 0; i < numNumbers - 1; i++) {
    printf("%d ", numField[i]);
  }
  printf("%d", numField[i++]);
  printf("]\n");
}

void *reader(void *field) {
  int* numField = (int*)field;
  sleep(1);
  unsigned long sum = 0;
  int i;
  for (i = 0; i < numNumbers; i++) {
    sum += (unsigned long)numField[i];
  }
  double avg = (double)sum / (double)numNumbers;
  printf("Thread %lu, sum: %lu, avg: %.2f\n", pthread_self(), sum, avg);
}

int main(int argc, char* argv[]) {
  srand(time(NULL));
  if (argc != 3) {
    printf("Ocekivam 2 parametra");
    exit(1);
  }
  unsigned int numThreads = (unsigned int)atoi(argv[1]);
  numNumbers = (unsigned int)atoi(argv[2]);
  pthread_t *threadIds = (pthread_t*)malloc(numThreads*sizeof(pthread_t));
  int **fields = (int**)malloc(numThreads*sizeof(int*));
  int i;
  for (i = 0; i < numThreads; i++) {
    // printf("Pravim dretvu %d\n", i);
    int *numField = (int*)malloc(numNumbers*sizeof(fields[i]));
    fields[i] = numField;
    if (pthread_create(&threadIds[i], NULL, writer, fields[i]) != 0) {
      printf("Greska pri stvaranju dretve!\n");
      exit(1);
    }
    if (pthread_create(&threadIds[i], NULL, reader, fields[i]) != 0) {
      printf("Greska pri stvaranju dretve!\n");
      exit(1);
    }
  }
  for (i = 0; i < numThreads; i++) {
    pthread_join(threadIds[i], NULL);
    free(fields[i]);
    fields[i] = NULL;
  }
  return 0;
}
