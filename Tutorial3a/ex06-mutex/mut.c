#include <stdio.h>
#include <pthread.h>

#define N 10
#define M 1000000 

pthread_mutex_t mutex;
volatile int counter=0;

void *thread_routine(void *parameter)
{
    pthread_mutex_lock(&mutex);
    int i;
    for (i=0; i<M; i++)
        counter++;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(void)
{
    pthread_t v[N];
    int i;

    pthread_mutex_init(&mutex,NULL);

    for (i=0; i<N; i++)
    {
        pthread_create(&v[i],NULL,thread_routine,NULL);
    }

    for (i=0; i<N; i++)
    {
        pthread_join(v[i],NULL);
    }

    printf("%d %d\n",counter,N*M);
    if (N*M==counter)
        printf("Success!\n");
    pthread_mutex_destroy(&mutex);

    return 0;
}
