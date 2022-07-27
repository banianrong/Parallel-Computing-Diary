#include"mpi.h"
#include<stdio.h>
#include<string.h>

const int MAXN = 20;

int main() {
  int myid, numprocs, namelen;
  char buf[MAXN], name[MPI_MAX_PROCESSOR_NAME];
  MPI_Status status;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(name, &namelen);
    printf("%d of %d on %s\n", myid, numprocs, name); fflush(stdout);
    if(myid == 1) {
      strcpy(buf, "message from 1\n");
      printf("1 sending:\n  %s", buf);
      fflush(stdout);
      MPI_Send(buf, strlen(buf)+1, MPI_CHAR, 0, 101, MPI_COMM_WORLD);
    }
    /*
    if(myid == 2) {
      strcpy(buf, "message from 2\n");
      printf("2 sending:\n  %s", buf);
      fflush(stdout);
      MPI_Send(buf, strlen(buf)+1, MPI_CHAR, 0, 101, MPI_COMM_WORLD);
    }//发生通信错误
    */
    if(myid == 0) {
      MPI_Recv(buf, MAXN, MPI_CHAR, MPI_ANY_SOURCE, 101, MPI_COMM_WORLD, &status);
      printf("0 received message:\n  %s", buf);
      fflush(stdout);
      int num = 0;
      MPI_Get_count(&status, MPI_CHAR, &num);
      printf("the sum of data is %d\n", num);
      fflush(stdout);
    }
  MPI_Finalize();
  return 0;
}