#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define MAXN 512

int main() {
  int myid, namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Status status;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);
    printf("processor %d running on %s\n", myid, processor_name);
    if(myid == 0) {
      char message[MAXN];
      strcpy(message, "hello, I'm processor 0\n");
      printf("processor 0 sending message: %s", message);
      fflush(stdout);
      MPI_Send(message, strlen(message)+1, MPI_CHAR, 2, 101, MPI_COMM_WORLD);
      strcpy(message, "goodbye, I'm processor 0\n");
      printf("processor 0 sending message: %s", message);
      fflush(stdout);
      MPI_Send(message, strlen(message)+1, MPI_CHAR, 2, 110, MPI_COMM_WORLD);
    }
    if(myid == 2) {
      char message[MAXN];
      MPI_Recv(message, MAXN, MPI_CHAR, 0, 101, MPI_COMM_WORLD, &status);
      printf("processor 2 received message: %s", message);
      fflush(stdout);
      MPI_Recv(message, MAXN, MPI_CHAR, 0, 110, MPI_COMM_WORLD, &status);
      printf("processor 2 received message: %s", message);
      fflush(stdout);
    }
  MPI_Finalize();
  return 0;
}