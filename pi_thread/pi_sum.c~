#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


long thread_count;
double sum;
long long n;
pthread_mutex_t mutex;

void *pi_sum_value(void*);


int main(int argc, char *argv[])
{
	if(argc < 3)
		exit(0);
	
	long thread;
	pthread_t* thread_handles;

	n = strtoll(argv[1], NULL, 10);
	thread_count = strtol(argv[2], NULL, 10);

	thread_handles = (pthread_t*) malloc(thread_count*sizeof(pthread_t));
	pthread_mutex_init(&mutex, NULL);
	sum=0.0;

	for(thread=0; thread < thread_count; thread++)
		pthread_create(&thread_handles[thread], NULL, pi_sum_value, (void*) thread);

	for(thread=0; thread < thread_count; thread++)
		pthread_join(thread_handles[thread], NULL);

	sum = sum*4;	
	printf("%.15f\n", sum);

	pthread_mutex_destroy(&mutex);
	free(thread_handles);


	return 0;
}



void *pi_sum_value(void * rank)
{
	long my_rank = (long) rank;
	double factor;
	long long i;
	long long my_n = n / thread_count;
	long long first_i = my_rank * my_n;
	long long last_i = first_i + my_n;
	double my_sum = 0.0;

	if(first_i % 2 == 0)
		factor =  1.0;
	else 
		factor = -1.0;	

	for(i = first_i; i < last_i; i++, factor=-factor)
		my_sum +=factor/(2*i+1);	
	
	pthread_mutex_lock(&mutex);
	sum += my_sum;
	pthread_mutex_unlock(&mutex);

			
	return NULL;
}
























