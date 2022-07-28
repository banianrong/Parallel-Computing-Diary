//first send last receive
#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define MAXN 512

int main() {
  int myid, numprocs, namelen, next;
  char processor_name[MPI_MAX_PROCESSOR_NAME], buffer[MAXN];
  MPI_Status status;
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(processor_name, &namelen);
    printf("%d of %d running on %s\n", myid, numprocs, processor_name);
    next = (myid + 1) % numprocs;
    strcpy(buffer, "hello world\n");
    printf("%d sending message : %s", myid, buffer);
    fflush(stdout);
    MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, next, 101, MPI_COMM_WORLD);
    printf("%d receiving message\n", myid);
    fflush(stdout);
    MPI_Recv(buffer, MAXN, MPI_CHAR, MPI_ANY_SOURCE, 101, MPI_COMM_WORLD, &status);
    printf("%d received message : %s", myid, buffer);
    fflush(stdout);
  /*
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(processor_name, &namelen);
    printf("%d of %d running on %s\n", myid, numprocs, processor_name);
    next = (myid + 1) % numprocs;
    printf("%d receiving message\n", myid);
    fflush(stdout);
    MPI_Recv(buffer, MAXN, MPI_CHAR, MPI_ANY_SOURCE, 101, MPI_COMM_WORLD, &status);
    printf("%d received message : %s", myid, buffer);
    fflush(stdout);
    strcpy(buffer, "hello world\n");
    printf("%d sending message : %s", myid, buffer);
    MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, next, 101, MPI_COMM_WORLD);
    //先接收后发送，全部死锁
  */
  MPI_Finalize();
  return 0;
}