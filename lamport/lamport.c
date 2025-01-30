#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

#include <stdio.h>
#define NUM_THREAD 5

char Entering [NUM_THREAD];
int Number [NUM_THREAD];

int max(int* nums, int n) {
  int i, max = INT_MIN;
  for (i = 0; i < n; i++) {
    if (nums[i] > max) {
      max = nums[i];
    }
  }
  return max;
}

void lock(unsigned int i) {
  Entering[i] = 1;
  Number[i] = max(Number, NUM_THREAD) + 1;
  Entering[i] = 0;
  int j;
  for (j = 0; j < NUM_THREAD; j++) {
    while (Entering[i] != 0);
    while (
      Number[j] != 0 &&
      (
        Number[j] < Number[i] ||
        Number[j] == Number[i] && j < i
      )
    );
  }
}

void unlock(unsigned int i) {
  Number[i] = 0;
}

void *thread(void *x) {
  int i = *(int*)x;
  int k, m;
  for (k = 0; k < 5; k++) {
    lock(i);
    for (m = 0; m < 5; m++) {
      printf("Dretva %d, k = %d, m = %d\n", i, k, m);
    }
    unlock(i);
  }
}

int main() {
  pthread_t threads[NUM_THREAD];
  unsigned int nums[NUM_THREAD];
  unsigned int i;
  for (i = 0; i < NUM_THREAD; i++) {
    nums[i] = i;
    if (pthread_create(&threads[i], NULL, thread, &nums[i]) != 0) {
      printf("Greska pri stvaranju dretve!\n");
      exit(1);
    };
  }
  for (i = 0; i < NUM_THREAD; i++) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}
