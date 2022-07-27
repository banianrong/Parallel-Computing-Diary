#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define MAXLEN 512

int main(int argc, char* argv[]) {
  int myid, namelen, numprocs;
  char buffer[MAXLEN], pro_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Status status;
  MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(pro_name, &namelen);
    printf("myid %d of %d running on %s\n", myid, numprocs, pro_name);
    if(myid == 0) {
      strcpy(buffer, "hello world");
      printf("processor 0 sending message: %s\n", buffer);
      fflush(stdout);
      MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, 3, 110, MPI_COMM_WORLD);
      printf("send %d data\n", strlen(buffer)+1);
      fflush(stdout);
    }
    if(myid == 3) {
      MPI_Recv(buffer, MAXLEN, MPI_CHAR, 0, 110, MPI_COMM_WORLD, &status);
      printf("processor 3 received message: %s\n", buffer);
      fflush(stdout);
      int count;
      MPI_Get_count(&status, MPI_CHAR, &count);
      printf("the data num is %d\n", count);
    }
  MPI_Finalize();
  return 0;
}