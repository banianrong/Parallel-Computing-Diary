#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define MAXN 512

int main() {
  MPI_Comm a;
  MPI_Status status;
  int myid, numprocs;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    printf("MPI_COMM_WORLD:%d\n", myid);
    MPI_Comm_split(MPI_COMM_WORLD, myid%2, myid, &a);
    MPI_Comm_size(a, &numprocs);
    printf("%d\n", numprocs); 
    MPI_Comm_rank(a, &myid);
    printf("a:%d\n", myid);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    if(myid == 0) {
       char buf[MAXN];
       strcpy(buf, "hello world from 0\n");
       printf("processor 0 sending : %s", buf);
       MPI_Send(buf, strlen(buf)+1, MPI_CHAR, 1, 110, a);
    }
    if(myid == 2) { // 这里发现0和2是一组，0和3不是一组
      char buf[MAXN];
      MPI_Recv(buf, MAXN, MPI_CHAR, 0, 110, a, &status);
      printf("%d\n", status.MPI_TAG);
      printf("%s", buf);
    }
  MPI_Finalize();
  return 0;
}