#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

/*global matrix variable */
int thread_count;
int N;
int **A;
int *x;
int *y;

void *matrix_multiply(void*);

void fill_rand_value_2d(int **, int);
void print_rand_value_2d(int **, int);

int main(int argc, char *argv[])
{
	if(argc < 3)
		exit(0);
	
	int i;
	N = atoi(argv[1]);

	A = (int **) malloc(N*sizeof(int *));
	for(i=0;i<N;i++)
		A[i]=(int *)malloc(N*sizeof(int));

	x = (int *)malloc(N*sizeof(int));
	y = (int *)malloc(N*sizeof(int));
	
	fill_rand_value_2d(A, N);
	//print_rand_value_2d(A, N);
	fill_rand_value(x, N);
	//print_rand_value(x, N);
	
	
	/*___________________pthread____________________________*/

	long thread;
	pthread_t* thread_handles;
	
	thread_count=strtol(argv[2], NULL, 10);
	thread_handles = malloc(thread_count*sizeof(pthread_t));

	for(thread=0; thread < thread_count; thread++)
		pthread_create(&thread_handles[thread], NULL, matrix_multiply, (void *) thread);



	for(thread=0; thread < thread_count; thread++)
		pthread_join(thread_handles[thread], NULL);


	/*-________________fin__________________________-*/


//	print_rand_value(y, N);
	
	free(thread_handles);
	free(A);
	free(x);
	free(y);

	return 0;
}


void *matrix_multiply(void *rank)
{
	long my_rank = (long) rank;

	/*row per thread */
	int local_N = N/thread_count; 	    	 /* for local_N=5  */
	int my_first_row = my_rank*local_N; 	 /* 0  1  2  3 	   */
	int my_last_row = (my_rank+1)*local_N-1; /* 4  9  14 19    */
	

	int i,j;
	for(i=my_first_row; i <= my_last_row; i++){
		y[i] = 0;
		
		for(j=0; j<N; j++)
			y[i] += A[i][j]*x[j];
	}



	return NULL;
}




void fill_rand_value_2d(int **A, int N)
{
	srand(time(NULL));
	int i,j;
	for(i=0; i < N; i++)
		for(j=0; j < N; j++)
			A[i][j] = rand()%100;


}


void fill_rand_value(int *arr, int N)
{
	srand(time(NULL));
	int i;
	for(i=0; i < N; i++)
		arr[i] = rand()%100;

}

void print_rand_value_2d(int **A, int N)
{
	int i,j;
	for(i=0; i < N; i++){
		for(j=0; j < N; j++)
			printf("%d\t", A[i][j]);

		printf("\n");
	}
}


void print_rand_value(int *arr, int N)
{
	int i;
	for(i=0; i < N; i++)
		printf("%d\n", arr[i]);
}





