#include"mpi.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAXLEN 512

int main() {
  int myid, namelen;
  MPI_Status status;
  char name[MPI_MAX_PROCESSOR_NAME], buf[MAXLEN];
  MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(name, &namelen);
    printf("processor %d is started on %s\n", myid, name);
    if(myid == 0) {
        double A[100];
        int buffersize;
        MPI_Pack_size(50, MPI_DOUBLE, MPI_COMM_WORLD, &buffersize);
        void* tempbuffer = malloc(buffersize);
        int j = sizeof(MPI_DOUBLE);
        int position = 0;
        for(int i = 0; i < 100; i++) A[i] = i * 1.1;
        printf("position : %d\n", position);
        for(int i = 0; i < 50; i++) 
          MPI_Pack(A+i*2, 1, MPI_DOUBLE, tempbuffer, buffersize, &position, MPI_COMM_WORLD);
        MPI_Send(tempbuffer, position, MPI_PACKED, 1, 101, MPI_COMM_WORLD);
        free(tempbuffer);
    }
    if(myid == 1) {
      void* B = malloc(MAXLEN);
      MPI_Recv(B, MAXLEN, MPI_PACKED, 0, 101, MPI_COMM_WORLD,&status);
      int num;
      MPI_Get_count(&status, MPI_PACKED, &num);
      printf("%d\n", num);
      double* C = (double*)B;
      for(int i = 0; i < 50; i++) {
        printf("%lf\n", C[i]);
      }
      free(B);
    }
  MPI_Finalize();
  return 0;
}