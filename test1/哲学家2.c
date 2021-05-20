#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include<stdio.h>
sem_t bin_sem[5];
sem_t sum_count;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void my_error(const char *error_string, int line)
{
    fprintf(stderr, "line:%d",line);
    perror(error_string);
    
}
void thinking(int number)
{
    printf("thinker %d is thinking now\n",number);
    sleep(2);
}
void eating(int number)
{
    printf("thinker %d is eating now\n",number);
    sleep(3);
}
void *thinker(void *arg)
{
    int num=*(int*)arg;
    pthread_mutex_unlock(&mutex);
    while (1)
    { 
        thinking(num);
            sem_wait(&sum_count);
            sem_wait(&(bin_sem[num]));
            sem_wait(&(bin_sem[(num+1)%5]));
            eating(num);
            sem_post(&(bin_sem[num]));
            sem_post(&(bin_sem[(num+1)%5]));
            sem_post(&sum_count);
            printf("%d thinker is complete eating\n",num);
        sleep(5);
    }
    exit(0);
}
int main()
{
    pthread_t pid[5];
    int ret;
    int i=0;
    for (i=0; i<5; i++)
    {
        if ((ret=sem_init(&bin_sem[i],0,1))==-1)
        {
            my_error("sem_init",__LINE__);
            exit(1);
        }
    }
    if ((ret=sem_init(&sum_count,0,4))==-1)
        {
            my_error("sem_init",__LINE__);
            exit(1);
        }
    for (i=0;i<5;i++)
    {
        pthread_mutex_lock(&mutex);
        int index=i;
        int *a=&index;
        if ((ret=pthread_create(&(pid[i]),NULL,thinker,a))!=0)
        {
            my_error("pthread_cread",__LINE__);
            exit(1);
        }
    }
    for (i=0; i<5; i++)
    {
        pthread_join(pid[i],NULL);
    }
    
}