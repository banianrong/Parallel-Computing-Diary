#include"mpi.h"
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

#define LEN 10
#define BASE 1000

int main() {
  int myid, numprocs, num[LEN], out[LEN];
  srand(time(NULL));
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    for(int i = 0; i < LEN; i++) num[i] = 10*myid + i+1;
    printf("processor %d array: ", myid);
    for(int i = 0; i < LEN; i++) printf("%d ", num[i]);
    printf("\n");fflush(stdout);
    memset(out, 0, sizeof(out));
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scan(&num, &out, 10, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    printf("processor %d array:", myid);
    if(out[0] == 0) printf("no\n");
    else{
      for(int i = 0; i < 10; i++) printf("%d ", out[i]);
      printf("\n");
    }
    fflush(stdout);
  MPI_Finalize();
  return 0;
}