#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NTHREADS 50

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
volatile int counter = 0;

void *increase_counter(void *arg)
{
	pthread_mutex_lock(&lock);

	printf("Thread number %ld, working on counter. The current value is %d\n", (long)pthread_self(), counter);
	int i;
//	usleep (10000); //simulate the data processing
	counter++;

	pthread_mutex_unlock(&lock);

}

int main()
{

	//create 50 threads of increase_counter, each thread adding 1 to the counter
	pthread_t tid[NTHREADS];
	int k;

//	while (counter < NTHREADS) {
//		if(pthread_create(&tid[counter], NULL, increase_counter, NULL)) {
//        		fprintf(stderr, "Error creating thread\n");
//        		return 1;
//		}
//	}

	for (k = 0; k < NTHREADS; k++) {
		if(pthread_create(&tid[k], NULL, increase_counter, NULL)) {
        		fprintf(stderr, "Error creating thread\n");
        		return 1;
		}
	}
	
	for (k = 0; k < NTHREADS; k++) {
		pthread_join(tid[k], NULL);
	}

	pthread_mutex_destroy(&lock);	

	printf("\nFinal counter value: %d\n", counter);
}

