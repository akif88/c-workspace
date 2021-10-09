#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;


void *Hello(void* rank);


int main(int argc, char *argv[])
{
	long thread; /* long for 64-bit system */

	pthread_t* thread_handles; /*pthread type*/

	/*string to long*/
	thread_count = strtol(argv[1], NULL, 10);

	thread_handles = malloc(thread_count*sizeof(pthread_t));

	for(thread = 0; thread < thread_count; thread++)
		pthread_create(&thread_handles[thread], NULL, Hello, (void *) thread);


	printf("Hello from the main thread\n");


	for(thread = 0; thread < thread_count; thread++)
		pthread_join(thread_handles[thread], NULL);/*stop thread*/

	free(thread_handles);

	return 0;
}


void *Hello(void* rank)
{	
	long my_rank = (long) rank;

	printf("Hello from thread %ld of %d\n", my_rank, thread_count);

	return NULL;
}