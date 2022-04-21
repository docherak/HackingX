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
void *increase_counter(void *);

pthread_mutex_t lock;
int counter = 0;

int main()
{

	//create 50 threads of increase_counter, each thread adding 1 to the counter
	//for (int j = 0; j <= NTHREADS; j++) {
	//	pthread_t thread_x;
	//	if(pthread_create(&thread_x, NULL, increase_counter, NULL)) {
	//		fprintf(stderr, "Error creating thread\n");
        //		return 1;	
	//	}	
	//}
	//
	while (counter <= NTHREADS) {
		pthread_t thread_x;
		if(pthread_create(&thread_x, NULL, increase_counter, NULL)) {
			fprintf(stderr, "Error creating threan\n");
			return 1;
		}
		if (pthread_mutex_init(&lock, NULL) != 0){
        		printf("\n mutex init failed\n");
        		return 1;
		}
		increase_counter(NULL);	
	}

	printf("\nFinal counter value: %d\n", counter);


	return 0;
}

void *increase_counter(void *arg)
{
	printf("Thread number %ld, working on counter. The current value is %d\n", (long)pthread_self(), counter);
	pthread_mutex_lock(&lock);    
	int i = counter;
//	usleep (10000); //simulate the data processing
	counter = i+1;
	pthread_mutex_unlock(&lock);    

}
