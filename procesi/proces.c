#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>

int Id;

typedef struct {
  int numSides;
  float sideLength;
} Polygon;

Polygon *Polygons;

void brisi(int sig) {
  shmdt(Polygons);
  shmctl(Id, IPC_RMID, NULL);
  exit(0);
}

int main(int argc, char* argv[]) {
  argc -= 1;
  if (argc < 2) {
    printf("Ocekivam barem 2 argumenta");
    exit(1);
  }
  if (argc % 2 != 0) {
    printf("Ocekivam paran broj argumenata (jednak broj stranica i duljina)");
    exit(1);
  }
  if (argc > 10) {
    printf("Previse mnogokuta");
    exit(1);
  }
  Id = shmget(IPC_PRIVATE, sizeof(Polygon)*5, 0600);
  if (Id == -1) {
    printf("Nije dobivena zajednicka memorija");
    exit(1);
  }
  Polygons = (Polygon*)shmat(Id, NULL, 0);
  Polygon polygon;
  int i;
  for (i = 0; i < argc / 2; i++) {
    int n = atoi(argv[i*2 + 1]);
    float a = atof(argv[i*2 + 2]);
    polygon.numSides = n;
    polygon.sideLength = a;
    Polygons[i] = polygon;
  }

  signal(SIGINT, brisi);

  for (i = 0; i < argc / 2; i++) {
    Polygon polygon;
    int n;
    float a, area, circum;
    switch (fork()) {
      case 0:
        polygon = Polygons[i];
        n = polygon.numSides;
        a = polygon.sideLength;
        area = 0.25 * (float)n * a * a / tan(M_PI / (float)n);
        printf("n = %-5d a = %-7.2f P = %.2f\n", n, a, area);
        // sleep(1);
        circum = (float)n * a;
        printf("n = %-5d a = %-7.2f O = %.2f\n", n, a, circum);

        exit(0);
      case -1:
        printf("Greska u stvaranju procesa\n");
        exit(1);
    }
  }
  while (i--) wait(NULL);
  brisi(0);
  return 0;
}
