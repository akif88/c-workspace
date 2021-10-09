#include <stdio.h>
#include <string.h>
#include <mpi/mpi.h>

const int MAX_STRING = 100;


int main(void)
{
	char greeting[MAX_STRING];
	int comm_sz; /* Number of processes  */
	int my_rank; /* My process rank  */


	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// if-else construct makes our program SPMD(Single-Program Multiple-Data)
	if(my_rank != 0)
	{
		sprintf(greeting, "Greetings from process %d of %d", my_rank, comm_sz);
		
		//send data main machine. firt zero number to send main computer
		MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
			printf("Greetings from Process %d of %d!\n", my_rank, comm_sz);
			int q;
			for(q=1; q < comm_sz; q++)
			{		// q is other machine
					MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, 
						MPI_STATUS_IGNORE); //receive messages other process
					printf("%s\n", greeting);
			}
	}//end if-else

	MPI_Finalize();
	return 0;
} /* main */


