#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define BUFLEN 512

int main() {
  int myid, numprocs, namelen;
  char buf[BUFLEN], Buf[BUFLEN], name[MPI_MAX_PROCESSOR_NAME];
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(name, &namelen);
    printf("%d of %d running on %s\n", myid, numprocs, name); fflush(stdout);
    memset(buf, 0, sizeof(buf));
    memset(Buf, 0, sizeof(Buf));
    if(myid == 0) {
      strcpy(buf, "hello, I\'m processor 0\n");
    }
    printf("processor %d\'s buf : %s", myid, buf); fflush(stdout);
    printf("\nMPI_Bcast is started\n"); fflush(stdout);
    if(myid == 0) MPI_Bcast(buf, strlen(buf)+1, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(Buf, BUFLEN, MPI_CHAR, 0, MPI_COMM_WORLD);
    printf("processor %d\'s now buf : %s", myid, Buf); fflush(stdout);
  MPI_Finalize();
  return 0;
}