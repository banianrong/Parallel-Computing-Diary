#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define BUFLEN 512

int main() {
  int myid;
  char buf[BUFLEN];
  MPI_Status status;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    printf("processor %d running\n", myid); fflush(stdout);
    memset(buf, 0, sizeof(buf));
    if(myid != 0) {
      sprintf(buf, "hello, I\'m processor %d", myid);
      MPI_Send(buf, strlen(buf)+1, MPI_CHAR, 0, 101, MPI_COMM_WORLD);
    }
    if(myid == 0) {
      for(int i = 0; i < 3; i++) {
        MPI_Recv(buf, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("get message : %s\n", buf); fflush(stdout);
        int cnt;
        MPI_Get_count(&status, MPI_CHAR, &cnt);
        printf("count %d from processor %d tag %d\n", cnt, status.MPI_SOURCE, status.MPI_TAG);
      } 
    }
    
  MPI_Finalize();
  return 0;
}