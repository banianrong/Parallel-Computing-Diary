#include"mpi.h"
#include<stdio.h>

int main() {
  int myid, num = 0;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    printf("processor %d running\n", myid); fflush(stdout);
    if (myid == 0 ) scanf("%d", &num); //其他进程没法访问，会导致死锁
    printf("%d %d\n", myid, num);
  MPI_Finalize();
  return 0;
}