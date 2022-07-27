#include"mpi.h"
#include<stdio.h>
#include<string.h>

const int MAXN = 256;

int main() {
  char processor_name[MPI_MAX_PROCESSOR_NAME], buf[MAXN];
  int myid, numprocs, namelen;
  MPI_Status status;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(processor_name, &namelen);
    printf("processor %d of %d on %s\n", myid, numprocs, processor_name);
    fflush(stdout);
    if(myid == 0) {
      strcpy(buf, "I'm processor 0, call 3\nover\n");
      printf("processor 0 sending\n");
      fflush(stdout);
      MPI_Send(buf, strlen(buf)+1, MPI_CHAR, 3, 101, MPI_COMM_WORLD);
      MPI_Recv(buf, MAXN, MPI_CHAR, 3, 101, MPI_COMM_WORLD, &status);
      printf("processor 0 received from 3, message:\n%s", buf);
    }
    if(myid == 3) {
      MPI_Recv(buf, MAXN, MPI_CHAR, 0, 101, MPI_COMM_WORLD, &status);
      printf("processor 3 from %d tag %d error %d\n", status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR);
      fflush(stdout);
      printf("processor 3 received message:\n%s", buf);
      fflush(stdout);
      strcpy(buf, "I'm processor 3, reveived from 0.\nover\n");
      printf("processor 3 sending to 0\n");
      fflush(stdout);
      MPI_Send(buf, strlen(buf)+1, MPI_CHAR, 0, 101, MPI_COMM_WORLD);
    }
  MPI_Finalize();
  return 0;
}