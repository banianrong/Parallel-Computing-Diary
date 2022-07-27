#include"mpi.h"
#include<stdio.h>
#include<string.h>

#define BUFLEN 512

int main(int argc, char* argv[]) {
  int myid, numprocs, next, namelen;
  char buffer[BUFLEN], processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Status status;
  MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);

    printf("Process %d on %s\n", myid, processor_name);
    printf("Process %d of %d\n", myid, numprocs);
    memset(buffer, 0, BUFLEN*sizeof(char));
    if(myid == numprocs-1)
      next = 0;
    else 
      next = myid+1;
    if(myid == 0)
    {
        strcpy(buffer, "hello there");
        printf("%d sending '%s'\n", myid, buffer); 
        fflush(stdout);
        MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, next, 99, MPI_COMM_WORLD);
        printf("#d receiving\n", myid); 
        fflush(stdout);
        MPI_Recv(buffer, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
        printf("%d received '%s'\n", myid, buffer);
        fflush(stdout);
        {
          int num = 0;
          printf("buffer has %d length\n", strlen(buffer));
          MPI_Get_count(&status, MPI_CHAR, &num);
          printf("I get %d of CHAR\n", num);
          MPI_Get_count(&status, MPI_INT, &num);
          printf("I get %d of INT\n", num);
          num = -1;
          MPI_Get_count(&status, MPI_DOUBLE, &num);
          printf("I get %d of DOUBLE\n", num);
          //printf("%d in status.count\n", status.count);
        }
    }else{
        printf("%d receiving\n", myid);
        fflush(stdout);
        MPI_Recv(buffer, BUFLEN, MPI_CHAR, MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &status);
        printf("%d received '%s'\n", myid, buffer);
        fflush(stdout);
        MPI_Send(buffer, strlen(buffer)+1, MPI_CHAR, next, 99, MPI_COMM_WORLD);
        printf("%d sent '%s'\n", myid, buffer);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}