#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define BUFLEN 512

int main() {
  int myid;
  char buf[BUFLEN];
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    printf("processor %d\n", myid); fflush(stdout);
    memset(buf, 0, sizeof(buf));
    if(myid == 0) {
      strcpy(buf, "hello, I\'m processor 0");
      MPI_Send(buf, strlen(buf)+1, MPI_CHAR, 1, 101, MPI_COMM_WORLD);
    }
    if(myid == 1) {
      MPI_Recv(buf, BUFLEN, MPI_CHAR, 0, 101, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("%s\n", buf);
    }
  MPI_Finalize();
  return 0;
}