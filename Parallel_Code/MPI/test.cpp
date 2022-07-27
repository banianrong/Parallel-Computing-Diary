#include"mpi.h"
#include<stdio.h>

#define MAXN 520

int main() {
  int myid, num[MAXN];
  MPI_Status status;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    if(myid == 0) {
      for(int i = 0; i < 10; i++) num[i] = i;
      MPI_Send(num, 10, MPI_CHAR, 1, 110, MPI_COMM_WORLD);
    }
    if(myid == 1) {
      MPI_Recv(num, MAXN, MPI_CHAR, 0, 110, MPI_COMM_WORLD, &status);
      for(int i = 0; i < 10; i++) printf("%d ", num[i]);
      int temp = 0;
      MPI_Get_count(&status, MPI_CHAR, &temp);
      printf("\n%d\n", temp);
    }
  MPI_Finalize();
  return 0;
}