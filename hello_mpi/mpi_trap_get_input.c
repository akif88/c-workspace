#include <stdio.h>
#include <mpi/mpi.h> 

/* Get the input values */
void get_input(int,int,double*,double*,int*);

double Trap(double,double,int,double);
double f(double);

int main()
{
	int my_rank, comm_sz, n=1024, local_n;
	double a=0.0, b=3.0, h, local_a, local_b;
   	double local_int, total_int;//integral
	int source;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	//get input from user and broadcast other process
	get_input(my_rank, comm_sz, &a, &b, &n);

	h=(b-a)/n; //same all process
	local_n=n/comm_sz; //each process own local_n
	
	//each process calculates its own area
	local_a = a+my_rank*local_n*h;
	local_b=local_a+local_n*h;
	local_int = Trap(local_a, local_b, local_n, h);
		
	if(my_rank !=0)
	{	//each process sends its calculated integral value.       
		MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
		total_int = local_int;
		for(source=1; source < comm_sz; source++)
		{	// the main process receive its integral value of the other process
			MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, 
				MPI_STATUSES_IGNORE);
			total_int +=local_int;
		}
	}

   if (my_rank == 0) {
      printf("With n = %d trapezoids, our estimate\n", n);
      printf("of the integral from %f to %f = %.15e\n", a, b, total_int);
   }
	
   MPI_Finalize();
   return 0;

}


void get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p)
{
	int dest;

	if(my_rank==0)
	{
		printf("Enter a, b, and n\n");
		scanf("%lf %lf %d", a_p, b_p, n_p);
		for(dest=1; dest<comm_sz; dest++)
		{	//send inputs from user to other process
			MPI_Send(a_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
			MPI_Send(b_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
			MPI_Send(n_p, 1, MPI_INT,    dest, 0, MPI_COMM_WORLD);
		}
	}
	else
	{	//receive inputs from main process
		MPI_Recv(a_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		MPI_Recv(b_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		MPI_Recv(n_p, 1, MPI_INT,    0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	}
}



double Trap(
      double left_endpt  /* in */, 
      double right_endpt /* in */, 
      int    trap_count  /* in */, 
      double base_len    /* in */) {
   double estimate, x; 
   int i;

   estimate = (f(left_endpt) + f(right_endpt))/2.0;
   for (i = 1; i <= trap_count-1; i++) {
      x = left_endpt + i*base_len;
      estimate += f(x);
   }
   estimate = estimate*base_len;

   return estimate;
} /*  Trap  */


/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input args:  x
 */
double f(double x) {
   return x*x;
} /* f */
