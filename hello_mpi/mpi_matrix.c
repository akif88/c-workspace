#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>


void Check_for_error(int, char, char, MPI_Comm);
void Get_dims(int*, int*, int*, int*, int, int, MPI_Comm);
void Allocate_arrays(double**, double**, double**, int, int, int,);
void Read_matrix(char, double, int, int, int, int, MPI_Comm);
void Read_vector(char, double, int, int, int, MPI_Comm);
void Print_matrix(char, double, int, int, int, int, MPI_Comm);
void Print_vector(char, double, int, int, int, MPI_Comm);
void Mat_vect_mult(double, double, double, int, int, int, MPI_Comm);



int main()
{
	double *local_A;
	double *local_x;
	double *local_y;
	int m, local_m, n, local_n;
	int my_rank, comm_sz;
	MPI_Comm comm;

	MPI_Init(NULL, NULL);
	comm=MPI_COMM_WORLD;
	MPI_Comm_size(comm, &comm_sz);
	MPI_Comm_rank(comm, &my_rank);

	// get user row and column size and broadcast them
	Get_dims(&m, &local_m, &n, &local_n, my_rank, comm_sz, comm);
	//allocated A, x and y
	Allocate_arrays(&local_A, &local_x, &local_y, local_m, n, local_n);
	//get user matrix value and scatter local matrix each process
	Read_matrix("A", local_A, m, local_m, n, local_n, my_rank, comm);
	//print A matrix after mpi_gather
	Print_matrix("A", local_A, m, local_m, n, local_n, my_rank, comm);


	MPI_Finalize();
	return 0;

}



void Get_dims(int* m_p, int* local_m_p, int* n_p, int* local_n_p, int my_rank, int comm_sz, MPI_Comm  comm) 
{
   if (my_rank == 0) {
      printf("Enter the number of rows\n");
      scanf("%d", m_p);
      printf("Enter the number of columns\n");
      scanf("%d", n_p);
   }
   MPI_Bcast(m_p, 1, MPI_INT, 0, comm);
   MPI_Bcast(n_p, 1, MPI_INT, 0, comm);

   *local_m_p = *m_p/comm_sz;
   *local_n_p = *n_p/comm_sz;
}



void Allocate_arrays(double** local_A_pp, double** local_x_pp, double** local_y_pp, int local_m, int n, int local_n,)
{
   *local_A_pp = malloc(local_m*n*sizeof(double));
   *local_x_pp = malloc(local_n*sizeof(double));
   *local_y_pp = malloc(local_m*sizeof(double));
}


void Read_matrix(char prompt[], double local_A[], int m, int local_m, int n, int my_rank, MPI_Comm  comm) 
{
	double* A = NULL;
	int i,j;

	if(my_rank == 0)
	{
		A = malloc(m*n*sizeof(double));
		printf("Enter the matrix %s\n", prompt);
      	
		for (i = 0; i < m; i++)
	    	for (j = 0; j < n; j++)
        	    scanf("%lf", &A[i*n+j]);
		MPI_Scatter(A, local_m*n, MPI_DOUBLE, local_A, local_m*n, MPI_DOUBLE, 0, comm);
		free(A);
	}
	else //must use for all process if we use mpi_scatter or mpi_gather
		MPI_Scatter(A, local_m*n, MPI_DOUBLE, local_A, local_m*n, MPI_DOUBLE, 0, comm);
}


void Print_matrix(char title[], double local_A[], int m, int local_m, int n, int my_rank, MPI_Comm  comm) 
{
	double* A = NULL;
	int i,j;

	if(my_rank == 0)
	{
		A = malloc(m*n*sizeof(double));
		MPI_Gather(local_A, local_m*n, MPI_DOUBLE, A, local_m*n, MPI_DOUBLE, 0, comm);
      	printf("\nThe matrix %s\n", title);
		for (i = 0; i < m; i++){
	    	for (j = 0; j < n; j++)
        	   printf("%f", A[i*n+j]);
			printf("\n");
		}
		printf("\n");
		free(A);
	}
	else //must use for all process if we use mpi_scatter or mpi_gather
		MPI_Gather(local_A, local_m*n, MPI_DOUBLE, A, local_m*n, MPI_DOUBLE, 0, comm);
		
}





