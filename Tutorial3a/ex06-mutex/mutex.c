/*
 * mutex.c
 *
 *  Created on: Mar 19, 2016
 *      Author: jiaziyi
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NTHREADS 50

volatile int counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *increase_counter(void *arg)
{
	pthread_mutex_lock(&lock);

	printf("Thread number %ld, working on counter. The current value is %d\n", (long)pthread_self(), counter);
//	int i;// = counter;
//	usleep (10000); //simulate the data processing
	
//	printf("\nStart: %d\n", counter);
	counter++;// = i+1;i
//	printf("\nFinish: %d\n", counter);

	pthread_mutex_unlock(&lock);

	return NULL;
}

int main()
{

	//create 50 threads of increase_counter, each thread adding 1 to the counter
	pthread_t tid[NTHREADS];

	while (counter < NTHREADS) {
		if(pthread_create(&tid[counter], NULL, increase_counter, NULL)) {
        		fprintf(stderr, "Error creating thread\n");
        		return 1;
		}
	}

//	for (int id = 0; id < NTHREADS; id++) {
//		pthread_join(tid[id], NULL);
//	}

	pthread_mutex_destroy(&lock);	

	printf("\nFinal counter value: %d\n", counter);
}

