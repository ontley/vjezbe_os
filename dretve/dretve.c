#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

unsigned int **Fields;
unsigned int numNumbers;

int randint(int low, int high) {
  return rand() % (high - low) + low;
}

void *writer(void *x) {
  unsigned int idx = *(int*)x;
  unsigned int* numField = Fields[idx];
  int i;
  printf("Thread %d, array: [", idx + 1);
  for (i = 0; i < numNumbers - 1; i++) {
    numField[i] = randint(0, INT_MAX);
    printf("%d ", numField[i]);
  }
  numField[i++] = randint(0, INT_MAX);
  printf("%d", numField[i]);
  printf("]\n");
}

void *reader(void *x) {
  unsigned int idx = *(int*)x;
  unsigned int* numField = Fields[idx];
  sleep(1);
  unsigned long sum = 0;
  int i;
  for (i = 0; i < numNumbers; i++) {
    sum += (unsigned long)numField[i];
  }
  double avg = (double)sum / (double)numNumbers;
  printf("Thread %d, sum: %lu, avg: %.2f\n", idx + 1, sum, avg);
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
  Fields = (unsigned int**)malloc(numThreads*sizeof(unsigned int*));
  int i;
  for (i = 0; i < numThreads; i++) {
    // printf("Pravim dretvu %d\n", i);
    unsigned int *numField = (unsigned int*)malloc(numNumbers*sizeof(unsigned int));
    Fields[i] = numField;
    if (pthread_create(&threadIds[i], NULL, writer, &i) != 0) {
      printf("Greska pri stvaranju dretve!\n");
      exit(1);
    }
    if (pthread_create(&threadIds[i], NULL, reader, &i) != 0) {
      printf("Greska pri stvaranju dretve!\n");
      exit(1);
    }
  }
  for (i = 0; i < numThreads; i++) {
    pthread_join(threadIds[i], NULL);
    free(Fields[i]);
    Fields[i] = NULL;
  }
  return 0;
}
